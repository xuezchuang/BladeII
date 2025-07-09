#include "BladeII.h"
#include "B2UIScenarioDialog.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "Event.h"

UB2UIScenarioDialog::UB2UIScenarioDialog(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShownDialogBox = false;
	TempPrevSpeakerName = TEXT("");

	YOffsetForBattleUIHidden = 100.0f;
	CachedOriginalLayoutPos = FVector2D(0.0f, 0.0f);

	MaxCharShowInterval = 0.15f;
	
	ManualRealDeltaTime = 0.0f;
	ManualLastRealTime = 0.0f;
	ManualPauseDiffTime = 0.0f;
	
	bPause = false;
}

void UB2UIScenarioDialog::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* TheWorld = GetWorld();
	ManualLastRealTime = TheWorld ? TheWorld->GetRealTimeSeconds() : 0.0f;
}

void UB2UIScenarioDialog::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Pause ������ �� �����Ƿ� ���� RealTime ���� ī��Ʈ�Ѵ�.
	UpdateManualTimeCounter();

	bool IsLastChar = false;
	if (ProgressiveDialogText.Tick(ManualLastRealTime, InDeltaTime, IsLastChar))
		FinalApplyDialogText();
}

void UB2UIScenarioDialog::UpdateManualTimeCounter()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		float ThisRealTime = TheWorld->GetRealTimeSeconds();
		ManualRealDeltaTime = FMath::Max(ThisRealTime - ManualLastRealTime - ManualPauseDiffTime, 0.0f);
		// bPause ��Ȳ������ ManualLastRealTime �� ������Ű�� ��..
		if (bPause) 
			ManualPauseDiffTime += ManualRealDeltaTime;

		ManualLastRealTime = ThisRealTime - ManualPauseDiffTime;
	}
	else
	{
		ResetTimeStates();
	}
}

void UB2UIScenarioDialog::CacheAssets()
{
	GET_SLOT(UCanvasPanel, CP_OverallLayout);
	if (CP_OverallLayout.IsValid())
	{
		UCanvasPanelSlot* LayoutSlot = Cast<UCanvasPanelSlot>(CP_OverallLayout->Slot);
		if (LayoutSlot)
		{
			CachedOriginalLayoutPos = LayoutSlot->GetPosition();
		}
	}

	GET_SLOT(UButton, BTN_Area)
	GET_SLOT(UButton, BTN_Next)
	GET_SLOT(UButton, BTN_Skip)
	GET_SLOT(UImage, IMG_Speaker)
	GET_SLOT(UTextBlock, TB_Speaker)
	GET_SLOT(UTextBlock, TB_Dialog)
	GET_SLOT(UButton, BTN_ScenarioTouch);
}

void UB2UIScenarioDialog::UpdateStaticText()
{
}

void UB2UIScenarioDialog::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UIScenarioDialog::OnClickBtnNext)
	BIND_CLICK_FUNC_TO_BTN(BTN_Next, &UB2UIScenarioDialog::OnClickBtnNext)
	BIND_CLICK_FUNC_TO_BTN(BTN_Skip, &UB2UIScenarioDialog::OnClickBtnSkip)
	BIND_CLICK_FUNC_TO_BTN(BTN_ScenarioTouch, &UB2UIScenarioDialog::OnClickBtnNext);
}

void UB2UIScenarioDialog::CloseWidgetDelegate()
{
	OnClickBtnNext();
}

//�� UI�� �׻� �� �ִ� UI�̹Ƿ�, Open�ÿ� BindDocAuto�� �Ҹ��°� �ƴ϶� ��ó�� �����ɶ� �Ҹ����� �Ǿ����� UB2UIManager::PrepareUI()����
void UB2UIScenarioDialog::BindDocAuto()
{
	UnbindDoc();

	auto DocScenarioDialog = UB2UIDocHelper::GetDocScenarioDialog();
	SetDoc(DocScenarioDialog);
	if (DocScenarioDialog)
	{
		UpdateDialogDatas();

		DocScenarioDialog->OnSpeakerPortraitChanged.AddUObject(this, &UB2UIScenarioDialog::OnChangedSpeakerPortrait);
		DocScenarioDialog->OnSpeakerNameChanged.AddUObject(this, &UB2UIScenarioDialog::OnChangedSpeakerName);
		DocScenarioDialog->OnThisDialogShowingTimeChanged.AddUObject(this, &UB2UIScenarioDialog::OnChangedDialogShowingTime);
		DocScenarioDialog->OnDialogTextChanged.AddUObject(this, &UB2UIScenarioDialog::OnChangedDialogText);
		DocScenarioDialog->OnShowNextButtonChanged.AddUObject(this, &UB2UIScenarioDialog::OnChangedShowNextButton);
		DocScenarioDialog->OnbLastBattleUIHiddenChanged.AddUObject(this, &UB2UIScenarioDialog::OnChangedLastBattleUIHidden);
	}
}

void UB2UIScenarioDialog::UnbindDoc()
{
	auto DocScenarioDialog = Cast<UB2UIDocScenarioDialog>(GetDoc());
	if (DocScenarioDialog)
	{
		DocScenarioDialog->OnSpeakerPortraitChanged.RemoveAll(this);
		DocScenarioDialog->OnSpeakerNameChanged.RemoveAll(this);
		DocScenarioDialog->OnThisDialogShowingTimeChanged.RemoveAll(this);
		DocScenarioDialog->OnDialogTextChanged.RemoveAll(this);
		DocScenarioDialog->OnShowNextButtonChanged.RemoveAll(this);
		DocScenarioDialog->OnbLastBattleUIHiddenChanged.RemoveAll(this);
	}

	SetDoc(nullptr);
}

void UB2UIScenarioDialog::ChangeDialog()
{
	ChangeFadeOutDialog();
}

void UB2UIScenarioDialog::ChangeFadeOutDialogComplete()
{
	//���̵� �ƿ��� �Ϸ�Ǹ�, ���ο� �����͸� ä������ FadeIn ��Ų��.
	UpdateDialogDatas();
	ChangeFadeInDialog();
}

void UB2UIScenarioDialog::ChangeFadeInDialogComplete()
{
}

void UB2UIScenarioDialog::UpdateDialogDatas()
{
	auto DocScenarioDialog = Cast<UB2UIDocScenarioDialog>(GetDoc());
	if (DocScenarioDialog)
	{
		SetSpeakerName(DocScenarioDialog->GetSpeakerName());
		SetSpeakerPortrait(DocScenarioDialog->GetSpeakerPortrait());
		SetDialogText(DocScenarioDialog->GetDialogText(), DocScenarioDialog->GetThisDialogShowingTime());
		ShowNextBtn(DocScenarioDialog->GetShowNextButton());

		TempPrevSpeakerName = DocScenarioDialog->GetSpeakerName().ToString();
		
		// ScenarioDialog �� �Ź� ������ �ƴ϶� StayInViewport �δٰ� visibility �� ������ ����.. ������ ���� �� ���� ������ �ʿ�.
		ResetTimeStates();
	}
}

void UB2UIScenarioDialog::OnPause(bool Pause)
{
	bPause = Pause;
	if (bPause)
		ForceHide();
	else
		RestoreFromForceHidden();
}

void UB2UIScenarioDialog::SetSpeakerName(const FText& InText)
{
	if (TB_Speaker.IsValid())
		TB_Speaker->SetText(InText);
}

void UB2UIScenarioDialog::SetSpeakerPortrait(UMaterialInterface* InMaterial)
{
	if (IMG_Speaker.IsValid())
		IMG_Speaker->SetBrushFromMaterial(InMaterial);
}

void UB2UIScenarioDialog::SetDialogText(const FText& InText, float InThisTextShowingTime)
{
	ProgressiveDialogText.SetWholeText(InText.ToString(), InThisTextShowingTime, MaxCharShowInterval, ManualLastRealTime);

	FinalApplyDialogText();
}

void UB2UIScenarioDialog::FinalApplyDialogText()
{
	if (TB_Dialog.IsValid()){
		TB_Dialog->SetText(FText::FromString(ProgressiveDialogText.GetCurrentString()));
	}
}

void UB2UIScenarioDialog::ResetTimeStates()
{
	ManualRealDeltaTime = 0.0f;
	ManualPauseDiffTime = 0.0f;

	UWorld* TheWorld = GetWorld();
	ManualLastRealTime = TheWorld ? TheWorld->GetRealTimeSeconds() : 0.0f;
}

void UB2UIScenarioDialog::ShowDialogBox(bool bShow)
{
	bShownDialogBox = bShow;
	ShowDialogBox_BP(bShow);
}


//====================================================================================
//									Click Handler
//====================================================================================
void UB2UIScenarioDialog::OnClickBtnNext()
{
	if (ProgressiveDialogText.IsStringFilledUp()) // ���ڸ� �� ä�� ���¿����� NextDialog ��.
	{
		NextDialogClass<>::GetInstance().Signal();
	}
	else
	{
		ProgressiveDialogText.ApplyToWholeString();
		FinalApplyDialogText();
	}
}
void UB2UIScenarioDialog::OnClickBtnSkip()
{
	SkipDialogClass<>::GetInstance().Signal();
}


//====================================================================================
//									Doc Delegate
//====================================================================================
void UB2UIScenarioDialog::OnChangedSpeakerPortrait(class UB2UIDocBase* Sender, UMaterialInterface* SpeakerPortrait, UMaterialInterface* PrevSpeakerPortrait)
{
	//���� ����Ŀ�� ���� ����Ŀ�� ���ƾ��� Prtrait�� �ٲ� �� �ִ�(����ǥ�� �ϵ�?) 
	//���� ���� ����Ŀ�� ���� ����Ŀ�� �ٸ��ٸ� ���⼭ Portrait�� �ٲ� �� ����, ChangeDialog()���� ó�� �Ǿ����. (���⶧����!! ChangeDialog�� SpeakerName�� �������� ȣ���)
	auto DocScenarioDialog = Cast<UB2UIDocScenarioDialog>(GetDoc());
	if (DocScenarioDialog)
	{
		if (TempPrevSpeakerName == DocScenarioDialog->GetSpeakerName().ToString())
		{
			SetSpeakerPortrait(SpeakerPortrait);
		}
	}
}

void UB2UIScenarioDialog::OnChangedSpeakerName(class UB2UIDocBase* Sender, FText SpeakerName, FText PrevSpeakerName)
{
	//����Ŀ�� �ٲ��, ���̵�ƿ� ������ ��Ű�� ������ ������ �ϰ������� �����͸� �ٲ۴�. ���⼭ �ٷ� �����ϸ� �ȵ�
	if (SpeakerName.ToString() != PrevSpeakerName.ToString())
	{
		ChangeDialog();
	}
}

void UB2UIScenarioDialog::OnChangedDialogShowingTime(class UB2UIDocBase* Sender, float ShowingTime, float PrevShowingTime)
{
	ProgressiveDialogText.SetTotalShowingTime(ShowingTime);
}

void UB2UIScenarioDialog::OnChangedDialogText(class UB2UIDocBase* Sender, FText DialogText, FText PrevDialogText)
{
	//���� ����Ŀ�� ���� ����Ŀ�� ���ƾ��� �ؽ�Ʈ������ �ٲ� �� �ִ�.
	//���� ���� ����Ŀ�� ���� ����Ŀ�� �ٸ��ٸ� ���⼭ �ؽ�Ʈ ������ �ٲ� �� ����, ChangeDialog()���� ó�� �Ǿ����. (���⶧����!! ChangeDialog�� SpeakerName�� �������� ȣ���)
	auto DocScenarioDialog = Cast<UB2UIDocScenarioDialog>(GetDoc());
	if (DocScenarioDialog)
	{
		if (TempPrevSpeakerName == DocScenarioDialog->GetSpeakerName().ToString())
		{
			SetDialogText(DialogText, DocScenarioDialog->GetThisDialogShowingTime());
		}
	}
}

void UB2UIScenarioDialog::OnChangedShowNextButton(class UB2UIDocBase* Sender, bool ShowNextButton, bool PrevShowNextButton)
{
	ShowNextBtn(ShowNextButton);
}

void UB2UIScenarioDialog::OnChangedLastBattleUIHidden(class UB2UIDocBase* Sender, bool LastBattleUIHidden, bool PrevLastBattleUIHidden)
{
	if (CP_OverallLayout.IsValid())
	{
		// ���� UI �� �������� ������ �⺻ ���̾ƿ����� �� ������. ȭ�� ���̴� ������ �� �� Ȯ���ϱ� ����.
		UCanvasPanelSlot* LayoutSlot = Cast<UCanvasPanelSlot>(CP_OverallLayout->Slot);
		if (LayoutSlot)
		{
			LayoutSlot->SetPosition(LastBattleUIHidden ? 
				(CachedOriginalLayoutPos + FVector2D(0.0f, YOffsetForBattleUIHidden)) : CachedOriginalLayoutPos				
				);
		}
	}
}
