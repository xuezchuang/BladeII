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

	// Pause 惑怕老 荐 乐栏骨肺 流立 RealTime 栏肺 墨款飘茄促.
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
		// bPause 惑炔俊辑绰 ManualLastRealTime 阑 蜡瘤矫虐绰 芭..
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

//捞 UI绰 亲惑 栋 乐绰 UI捞骨肺, Open矫俊 BindDocAuto啊 阂府绰霸 酒聪扼 俩贸澜 积己瞪锭 阂府档废 登绢乐澜 UB2UIManager::PrepareUI()曼绊
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
	//其捞靛 酒眶捞 肯丰登搁, 货肺款 单捞磐甫 盲况初绊 FadeIn 矫挪促.
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
		
		// ScenarioDialog 绰 概锅 积己捞 酒聪扼 StayInViewport 肺促啊 visibility 甫 力绢且 波扼.. 单捞磐 盎脚 矫 惑怕 府悸捞 鞘夸.
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
	if (ProgressiveDialogText.IsStringFilledUp()) // 臂磊甫 促 盲款 惑怕俊辑绰 NextDialog 肺.
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
	//抗傈 胶乔目客 瘤陛 胶乔目啊 鞍酒具瘤 Prtrait甫 官曹 荐 乐促(皑沥钎泅 老淀?) 
	//父踞 抗傈 胶乔目客 瘤陛 胶乔目啊 促福促搁 咯扁辑 Portrait甫 官曹 荐 绝绊, ChangeDialog()俊辑 贸府 登绢具窃. (楷免锭巩烙!! ChangeDialog绰 SpeakerName狼 函版栏肺 龋免凳)
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
	//胶乔目啊 官差搁, 其捞靛酒眶 楷免阑 矫虐绊 楷免捞 场唱搁 老褒利栏肺 单捞磐甫 官槽促. 咯扁辑 官肺 技泼窍搁 救凳
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
	//抗傈 胶乔目客 瘤陛 胶乔目啊 鞍酒具瘤 咆胶飘郴侩阑 官曹 荐 乐促.
	//父踞 抗傈 胶乔目客 瘤陛 胶乔目啊 促福促搁 咯扁辑 咆胶飘 郴侩阑 官曹 荐 绝绊, ChangeDialog()俊辑 贸府 登绢具窃. (楷免锭巩烙!! ChangeDialog绰 SpeakerName狼 函版栏肺 龋免凳)
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
		// 傈捧 UI 啊 见绢乐阑 锭俊绰 扁夯 饭捞酒眶俊辑 粱 郴赴促. 拳搁 焊捞绰 康开阑 粱 歹 犬厘窍扁 困秦.
		UCanvasPanelSlot* LayoutSlot = Cast<UCanvasPanelSlot>(CP_OverallLayout->Slot);
		if (LayoutSlot)
		{
			LayoutSlot->SetPosition(LastBattleUIHidden ? 
				(CachedOriginalLayoutPos + FVector2D(0.0f, YOffsetForBattleUIHidden)) : CachedOriginalLayoutPos				
				);
		}
	}
}
