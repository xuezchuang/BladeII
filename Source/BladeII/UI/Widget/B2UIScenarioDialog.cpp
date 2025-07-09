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

	// Pause 상태일 수 있으므로 직접 RealTime 으로 카운트한다.
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
		// bPause 상황에서는 ManualLastRealTime 을 유지시키는 거..
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

//이 UI는 항상 떠 있는 UI이므로, Open시에 BindDocAuto가 불리는게 아니라 젤처음 생성될때 불리도록 되어있음 UB2UIManager::PrepareUI()참고
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
	//페이드 아웃이 완료되면, 새로운 데이터를 채워놓고 FadeIn 시킨다.
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
		
		// ScenarioDialog 는 매번 생성이 아니라 StayInViewport 로다가 visibility 를 제어할 꺼라.. 데이터 갱신 시 상태 리셋이 필요.
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
	if (ProgressiveDialogText.IsStringFilledUp()) // 글자를 다 채운 상태에서는 NextDialog 로.
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
	//예전 스피커와 지금 스피커가 같아야지 Prtrait를 바꿀 수 있다(감정표현 일듯?) 
	//만얀 예전 스피커와 지금 스피커가 다르다면 여기서 Portrait를 바꿀 수 없고, ChangeDialog()에서 처리 되어야함. (연출때문임!! ChangeDialog는 SpeakerName의 변경으로 호출됨)
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
	//스피커가 바뀌면, 페이드아웃 연출을 시키고 연출이 끝나면 일괄적으로 데이터를 바꾼다. 여기서 바로 세팅하면 안됨
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
	//예전 스피커와 지금 스피커가 같아야지 텍스트내용을 바꿀 수 있다.
	//만얀 예전 스피커와 지금 스피커가 다르다면 여기서 텍스트 내용을 바꿀 수 없고, ChangeDialog()에서 처리 되어야함. (연출때문임!! ChangeDialog는 SpeakerName의 변경으로 호출됨)
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
		// 전투 UI 가 숨어있을 때에는 기본 레이아웃에서 좀 내린다. 화면 보이는 영역을 좀 더 확장하기 위해.
		UCanvasPanelSlot* LayoutSlot = Cast<UCanvasPanelSlot>(CP_OverallLayout->Slot);
		if (LayoutSlot)
		{
			LayoutSlot->SetPosition(LastBattleUIHidden ? 
				(CachedOriginalLayoutPos + FVector2D(0.0f, YOffsetForBattleUIHidden)) : CachedOriginalLayoutPos				
				);
		}
	}
}
