#include "B2UITutorial.h"
#include "B2UIManager.h"
#include "TutorialManager.h"
#include "B2UIRewardEventSet.h"
#include "B2LobbyGameMode.h"
#include "B2UIManager_Lobby.h"
#include "B2UILobbyMain.h"
#include "B2UICombineMenu.h"
#include "B2HighlightWidget.h"
#include "B2AndroidBackManager.h"
#if PLATFORM_IOS
#include "IOS/IOSPlatformMisc.h"
#endif

void UB2UITutorial::CacheAssets()
{
	Super::CacheAssets();

	//Tutorial Main Panel
	GET_SLOT(UCanvasPanel, CP_MainTutorial);
	GET_SLOT(UCanvasPanel, CP_TransparencyBlock);

// Tutorial Begin UI Panel
	GET_SLOT(UPanelWidget, P_TutorialBegin);
	GET_SLOT(UB2Image, IMG_TutorialMain);
	GET_SLOT(UTextBlock, TB_TutorialName);
	GET_SLOT(UTextBlock, TB_TutorialDesc);

// Tutorial Step Dialog Panel
	GET_SLOT(UCanvasPanel, CP_Dialog);
	GET_SLOT(UImage, IMG_Speaker);
	GET_SLOT(UB2RichTextBlock, RTB_Dialog);
	GET_SLOT(UButton, BTN_Skip);
	GET_SLOT(UImage, IMG_Arrow);
	GET_SLOT(UImage, IMG_DialogArrow);

	// ------------- Debug -------------------
	GET_SLOT(UTextBlock, TB_Debug1);
	GET_SLOT(UTextBlock, TB_Debug2);
	// ------------- Debug -------------------

// TutorialSkip UI Panel
	GET_SLOT(UPanelWidget, P_TutorialSkip);
	GET_SLOT(UHorizontalBox, HB_SkipRewardItems);

	SkipRewardIcons.AddDefaulted(4);
	GET_SLOT_BYNAME(UB2UIRewardIcon, SkipRewardIcons[0], SkipRewardIcon1);
	GET_SLOT_BYNAME(UB2UIRewardIcon, SkipRewardIcons[1], SkipRewardIcon2);
	GET_SLOT_BYNAME(UB2UIRewardIcon, SkipRewardIcons[2], SkipRewardIcon3);
	GET_SLOT_BYNAME(UB2UIRewardIcon, SkipRewardIcons[3], SkipRewardIcon4);

	for (auto RewardIcon : SkipRewardIcons)
		if (RewardIcon.IsValid()) RewardIcon->Init();

	GET_SLOT(UTextBlock, TB_TutorialSkipWarning);
	GET_SLOT(UTextBlock, TB_SkipPopupTitle);
	
	GET_SLOT(UButton, BTN_SkipConfirm);
	GET_SLOT(UTextBlock, TB_SkipConfirm);

	GET_SLOT(UButton, BTN_SkipCancel);
	GET_SLOT(UTextBlock, TB_SkipCancel);

// ContentOpen Panel
	GET_SLOT(UPanelWidget, P_ContentOpen);
	GET_SLOT(UTextBlock, TB_ContentOpenDesc);
	GET_SLOT(UButton, BTN_GoToLobby);
	GET_SLOT(UTextBlock, TB_GotoLobby);
	GET_SLOT(UButton, BlockContentOpen);

// Tutorial Reward Panel

	GET_SLOT(UPanelWidget, P_TutorialReward);
	GET_SLOT(UHorizontalBox, HB_TutorialRewardItems);

	TutorialRewardIcons.AddDefaulted(4);
	GET_SLOT_BYNAME(UB2UIRewardIcon, TutorialRewardIcons[0], TutorialRewardIcon1);
	GET_SLOT_BYNAME(UB2UIRewardIcon, TutorialRewardIcons[1], TutorialRewardIcon2);
	GET_SLOT_BYNAME(UB2UIRewardIcon, TutorialRewardIcons[2], TutorialRewardIcon3);
	GET_SLOT_BYNAME(UB2UIRewardIcon, TutorialRewardIcons[3], TutorialRewardIcon4);

	for (auto RewardIcon : TutorialRewardIcons)
		if (RewardIcon.IsValid()) RewardIcon->Init();

	GET_SLOT(UTextBlock, TB_TutorialRewardTitle);
	GET_SLOT(UTextBlock, TB_RewardDestination);

	GET_SLOT(UButton, BTN_RewardConfirm);

	GET_SLOT(UCanvasPanel, CP_HilightMainBtn);
	GET_SLOT(UButton, BTN_HilightMain);

	HighlightImage = NewObject<UImage>(this, NAME_None, RF_Transient);
	if(HighlightImage)
	{
		if (UCanvasPanel* RootCanvas = Cast<UCanvasPanel>(GetRootWidget()))
		{
			UCanvasPanelSlot* PanelSlot = RootCanvas->AddChildToCanvas(HighlightImage);
			if (PanelSlot)
			{
				PanelSlot->SetAnchors(FAnchors());
			}
		}

		FSlateBrush CurBrush = HighlightImage->GetBrush();
		CurBrush.Margin = HighlightMargin;
		CurBrush.DrawAs = ESlateBrushDrawType::Box;
		HighlightImage->SetBrush(CurBrush);
	}

	// Debug

	if (TB_Debug1.IsValid()) TB_Debug1->SetVisibility(ESlateVisibility::Collapsed);
	if (TB_Debug2.IsValid()) TB_Debug2->SetVisibility(ESlateVisibility::Collapsed);

	bHighlightRedrawCheck = false;
	bHighlightDialogRedrawCheck = false;
}

void UB2UITutorial::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Skip, &UB2UITutorial::OnClickBTN_Skip);
	BIND_CLICK_FUNC_TO_BTN(BTN_SkipConfirm, &UB2UITutorial::OnClickBTN_SkipConfirm);
	BIND_CLICK_FUNC_TO_BTN(BTN_SkipCancel, &UB2UITutorial::OnClickBTN_SkipCancel);

	BIND_CLICK_FUNC_TO_BTN(BTN_GoToLobby, &UB2UITutorial::OnClickBTN_GotoLobby);

	BIND_CLICK_FUNC_TO_BTN(BTN_RewardConfirm, &UB2UITutorial::OnClickBTN_RewardConfirm);

	BIND_CLICK_FUNC_TO_BTN(BlockContentOpen, &UB2UITutorial::OnClickBTN_BlockContentOpen);

	BIND_CLICK_FUNC_TO_BTN(BTN_HilightMain, &UB2UITutorial::OnClickBTN_MainHighlight);
}

void UB2UITutorial::Init()
{
	Super::Init();
}

void UB2UITutorial::DestroySelf(UB2UIManager* InUIManager)
{


	Super::DestroySelf(InUIManager);
}

void UB2UITutorial::OnClose(bool RightNow)
{
	const int32 CurrentTutorialID = TutorialManager::GetInstance().GetCurrentTutorialID();
	if (CurrentTutorialID != INDEX_NONE)
	{
		// Scene��ȯ �� Scene�� ��ϵ� Widget �̿ܿ��� �׿������� ��찡 �����Ƿ� (ex - UB2UImanager::CloseAllStandaloneUIs() )
		// Tutorial ���߿��� Close���� ���ϰ� ����
	}

	else
	{
		Super::OnClose(RightNow);
	}
	
}

void UB2UITutorial::OpenTutorialBegin(int32 TutorialID)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2UITutorial::OpenTutorialBegin"));


	if (const FTutorialInfo* TutorialInfo = TutorialManager::GetInstance().GetTutorialInfo(TutorialID))
	{
		if (IMG_TutorialMain.IsValid())
			IMG_TutorialMain->SetBrushFromTexture(GetTutorialIconTexture(TutorialID));

		if (TB_TutorialName.IsValid())
			TB_TutorialName->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Tutorial_OpenTitle")), GetTutorialTitle(TutorialID)));

		if (TB_TutorialDesc.IsValid())
			TB_TutorialDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Tutorial_ContentOpenNotice")));

		OpenTutorialBeginUI(TutorialInfo);
	}
}


void UB2UITutorial::OpenTutorialBeginUI(const FTutorialInfo* TutorialInfo)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2UITutorial::OpenTutorialBeginUI"));

	if (TutorialInfo)
	{
		HideAllTutorialUI();
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		float CloseTime = TutorialOpenCloseTime;

		// ContentOpen�̸� ���~
		if (TutorialInfo->TutorialType == ETutorialType::ContentOpen)
		{
			if (P_TutorialBegin.IsValid())
				P_TutorialBegin->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			OnOpenTutorialBegin();
		}
		else
		{
			CloseTime = 0.05f; // UI�� �ּ��� �ѹ� Draw�� �ð��� �־�� ��.
		}

		TutorialUICloseTimer(CloseTime);
	}	
}


void UB2UITutorial::CloseTutorialBeginUI()
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2UITutorial::CloseTutorialBeginUI"));

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (P_TutorialBegin.IsValid()) P_TutorialBegin->SetVisibility(ESlateVisibility::Hidden);

	if (APlayerController* Controller = GetOwningPlayer())
	{
		FTimerManager& Timer = Controller->GetWorldTimerManager();
		if(Timer.IsTimerActive(CloseTimer))
			Timer.ClearTimer(CloseTimer);
	}

	const int32 CurrentTutorialID = TutorialManager::GetInstance().GetCurrentTutorialID();
	TutorialManager::GetInstance().OnTutorialBeginUIClose(CurrentTutorialID);
}


void UB2UITutorial::OpenTutorialSkipNoticeUI(const int32 TutorialID)
{
	HiddenTutorialUI();

	if (HighlightImage) HighlightImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (P_TutorialSkip.IsValid())
	{
		P_TutorialSkip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SetClose(true);
	}

	if (TB_SkipPopupTitle.IsValid())
		TB_SkipPopupTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")));

	if (TB_TutorialSkipWarning.IsValid())
		TB_TutorialSkipWarning->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Tutorial_SkipWarning")));
	
	//Ʃ�丮�� ���� ������ ��� ��� Ȥ�� ���� ���ܵ�
/*
	const int32 CurrentTutorialID = TutorialManager::GetInstance().GetCurrentTutorialID();
	b2network::B2mdTutorialInfoPtr ServerTutorialInfo = TutorialManager::GetInstance().GetServerTutorialInfo(CurrentTutorialID);

	if (ServerTutorialInfo == nullptr)
	{
		return;
	}

	TArray<int32>	arrRewardID;
	TArray<int32>	arrRewardCount;

	if (ServerTutorialInfo->reward_count1 > 0)
	{
		arrRewardID.Push(ServerTutorialInfo->reward_id1);
		arrRewardCount.Push(ServerTutorialInfo->reward_count1);
	}
	if (ServerTutorialInfo->reward_count2 > 0)
	{
		arrRewardID.Push(ServerTutorialInfo->reward_id2);
		arrRewardCount.Push(ServerTutorialInfo->reward_count2);
	}
	if (ServerTutorialInfo->reward_count3 > 0)
	{
		arrRewardID.Push(ServerTutorialInfo->reward_id3);
		arrRewardCount.Push(ServerTutorialInfo->reward_count3);
	}

	const int32 iIconMax = 3;
	for (int i = 0; i < iIconMax; ++i)
	{
		if ((!SkipRewardIcons.IsValidIndex(i)) || (!SkipRewardIcons[i].IsValid()))
		{
			continue;
		}

		if (arrRewardID.IsValidIndex(i) && arrRewardCount.IsValidIndex(i))
		{
			SkipRewardIcons[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SkipRewardIcons[i]->UpdateRewardIcon(arrRewardID[i], arrRewardCount[i]);
		}
		else
		{
			SkipRewardIcons[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
*/
	//Ʃ�丮�� ���� ������ ��� ��� Ȥ�� ���� ���ܵ�

	OnOpenTutorialSkipReward();
}

void UB2UITutorial::CloseTutorialSkipNoticeUI()
{
	if (P_TutorialSkip.IsValid())
	{
		SetClose(false);
		P_TutorialSkip->SetVisibility(ESlateVisibility::Hidden);
	}

	ProcessStepHighlight(CurrentTutorialStep);
}

void UB2UITutorial::ProcessStepHighlight(const FTutorialStep* StepInfo)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2UITutorial::ProcessStepHighlight"));

	HideAllTutorialUI();

	if (StepInfo && (StepInfo->Action == ETutorialAction::Highlight || StepInfo->Action == ETutorialAction::BlockHilight))
	{
		CurrentTutorialStep = StepInfo;

		SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		bHighlightRedrawCheck = true;
		bHighlightDialogRedrawCheck = true;

		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
		B2AndroidBackManager::GetInstance()->AddBackWidget(this);
	}
}

void UB2UITutorial::ClearStepHighlight(const struct FTutorialStep* StepInfo)
{
	HiddenTutorialUI();

	SetVisibileHighlightRect(false);

	bHighlightRedrawCheck = false;
	bHighlightDialogRedrawCheck = false;
}

void UB2UITutorial::TutorialUICloseTimer(const float UICloseSeconds)
{
	if (APlayerController* Controller = GetOwningPlayer())
	{
		FTimerManager& Timer = Controller->GetWorldTimerManager();
		Timer.SetTimer(CloseTimer, this, &UB2UITutorial::CloseTutorialBeginUI, UICloseSeconds, false);
	}
}

void UB2UITutorial::SetTutorialDialog(const FTutorialStep* StepInfo, const FVector2D& DrawPosition)
{
	if (CP_Dialog.IsValid())
	{
		SetClose(true);
		CP_Dialog->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (UCanvasPanelSlot* DialogPanel = Cast<UCanvasPanelSlot>(CP_Dialog->Slot))
		{
			DialogPanel->SetPosition(DrawPosition);
			DialogPanel->SynchronizeProperties();
		}

		if (bVisibleSkipButton)
		{
			//���̾�α�â�� Ȱ��ȭ �ɶ� ��ŵ��ư Ȱ��ȭ
			UB2UITutorial::OnVisibleAndSetLOCTextSkipBtn();
		}
		else
		{
			if (BTN_Skip.IsValid())
			{
				BTN_Skip->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}

	if (RTB_Dialog.IsValid())
	{
		RTB_Dialog->SetText(GetTutorialStepDialog(StepInfo->TutorialID, StepInfo->TutorialStep));
	}

	if (IMG_Speaker.IsValid())
	{
		IMG_Speaker->SetBrushFromMaterial(GetTutorialSpeakerMaterial(StepInfo->TutorialID));
	}

	if (StepInfo->Action == ETutorialAction::BlockHilight)
	{
		if (IMG_DialogArrow.IsValid())
		{
			UB2UITutorial::OnVisibleDialogArrow();
		}
	}
}


void UB2UITutorial::ProcessStepHighlight_Material(const struct FTutorialStep* StepInfo)
{
	if (HighlightImage)
	{
		//���̶���Ʈ ���ۺ� �ٸ��� ����(��ȹ)
		if (StepInfo->Action == ETutorialAction::BlockHilight)
			SetHighlightMaterial(HighlightImage, ArrayHighlightMaterial[1]);
		
		else
			SetHighlightMaterial(HighlightImage, ArrayHighlightMaterial[0]);
	}
}

void UB2UITutorial::SetVisibileHighlightRect(bool bVisible)
{
	ESlateVisibility RectVisibility = bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	for (UButton* Button : HighlightRects)
	{
		if (Button)
			Button->SetVisibility(RectVisibility);
	}
}

void UB2UITutorial::ProcessStepHighlight_Arrow(const struct FTutorialStep* StepInfo, const FVector2D& TopLeft, const FVector2D& DrawSize)
{
	if (IMG_Arrow.IsValid() && (StepInfo->Action == ETutorialAction::Highlight || StepInfo->Action == ETutorialAction::BlockHilight))
	{
		static const float space = 10.f;

		const FVector2D BtmRight = TopLeft + DrawSize;

		const float MidX = (TopLeft.X + BtmRight.X) / 2.f;
		const float MidY = (TopLeft.Y + BtmRight.Y) / 2.f;
		
		float posX(0), posY(0);
		if (UCanvasPanelSlot* ArrowSlot = Cast<UCanvasPanelSlot>(IMG_Arrow->Slot))
		{
			switch (StepInfo->ArrowDirection)
			{
			case ETutorialArrow::Left:
				ArrowSlot->SetAlignment(FVector2D(1.0f, 0.5f));
				posX = TopLeft.X - space;
				posY = MidY;
				break;

			case ETutorialArrow::Right:
				ArrowSlot->SetAlignment(FVector2D(0.0f, 0.5f));
				posX = BtmRight.X + space;
				posY = MidY;
				break;

			case ETutorialArrow::Top:
				ArrowSlot->SetAlignment(FVector2D(0.5f, 1.0f));
				posX = MidX;
				posY = TopLeft.Y - space;
				break;

			case ETutorialArrow::Bottom:
				posX = MidX;
				posY = BtmRight.Y + space;
				break;
			}

			int32 ArrowIndex = static_cast<int32>(StepInfo->ArrowDirection) - 1;
			UMaterialInterface* ArrowMtrl = GetArrowMaterial(ArrowIndex);

			// 1. Set Anchor ( must be same HighlightWidget Anchor ) and position
			ArrowSlot->SetPosition(FVector2D(posX, posY));
			ArrowSlot->SetZOrder(1);
			ArrowSlot->SynchronizeProperties();

			// 2. Texture
			IMG_Arrow->SetBrushFromMaterial(ArrowMtrl);

			// 3. Visible
			IMG_Arrow->SetVisibility(ArrowMtrl ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

			//���̶���Ʈ ���� bForceRedraw�ɼ� ���� ȭ��ǥ �ִϸ��̼� �ߺ� ��� ����
			if (UWidgetAnimation* ArrowAnim = GetAnimation(TEXT("ANIM_Arrow")))
			{
				if (IsAnimationPlaying(ArrowAnim) == false)
					OnVisibleHighlightArrow(true);
			}
		}
	}
}

void UB2UITutorial::SetHighlightMaterial(UImage* ImageWidget, UMaterialInterface* BrushMtrl)
{
	if (ImageWidget != nullptr)
	{
		ImageWidget->SetBrushFromMaterial(BrushMtrl);
		ImageWidget->SetVisibility(BrushMtrl != nullptr ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

		ImageWidget->SetColorAndOpacity(FLinearColor::White);
	}
}

UMaterialInterface* UB2UITutorial::GetArrowMaterial(int32 ArrowIndex)
{
	return ArrowMaterials.IsValidIndex(ArrowIndex) ? ArrowMaterials[ArrowIndex] : nullptr;
}

FName UB2UITutorial::GetCurrentDJLegacyUIName()
{
	if (AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		if (UB2UIManager_Lobby* LobbyUIManager = LobbyGM->DJLegacy_GetLobbyUIManager())
		{
			UB2UnitedWidgetBase* CurrDJLegacyUI = LobbyUIManager->GetCurrActiveUI();
			return LobbyUIManager->GetDJLegacyUIName(CurrDJLegacyUI);
		}
	}

	return FName(TEXT("None"));
}

bool UB2UITutorial::GlobalTutorialProcessingCheck_BP()
{
	return TutorialManager::GetInstance().HasAnyTutorial();
}

//�� ���� �ִ� ����� Ʃ�丮�� ui����� ��� ������...����(17.10.20)�� ProcessingCheck�� �ƿ� ��� ����
void UB2UITutorial::GlobalOpenAnimCheckTutorialVisible_BP(bool bOpenAnimPlaying)
{
	UB2UITutorial* TutorialUI = UB2UIManager::GetInstance()->GetUI<UB2UITutorial>(UIFName::Tutorial);

	if(TutorialUI == nullptr)
	{ return; }

	if (bOpenAnimPlaying == true)
	{
		TutorialUI->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TutorialUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UB2UITutorial::SetDebugInfo(const struct FTutorialStep* StepInfo)
{
	//if (TB_Debug1.IsValid() && TB_Debug2.IsValid())
	//{
	//	FString Requirement(TEXT("None"));

	//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETutorialRequire"), true);
	//	if(EnumPtr)
	//		Requirement = EnumPtr->GetNameStringByIndex(static_cast<int32>(StepInfo->Requirement));

	//	FString Debug1 = FString::Printf(TEXT("%d_%d / %s / %s"),
	//		StepInfo->TutorialID, StepInfo->TutorialStep, *Requirement, *StepInfo->RequireName.ToString());

	//	TB_Debug1->SetText(FText::FromString(Debug1));


	//	const UEnum* EnumPtr2 = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETutorialAction"), true);
	//	if (EnumPtr2)
	//	{
	//		FString Action = EnumPtr2->GetNameStringByIndex(static_cast<int32>(StepInfo->Action));
	//		FString Debug2 = FString::Printf(TEXT("Action : %s"), *Action);
	//		TB_Debug2->SetText(FText::FromString(Debug2));
	//	}
	//}
}

void UB2UITutorial::HideAllTutorialUI()
{
	ClearStepHighlight(CurrentTutorialStep);

	HiddenTutorialUI();
}

void UB2UITutorial::HiddenTutorialUI()
{
	SetClose(false);

	if (P_TutorialBegin.IsValid())	P_TutorialBegin->SetVisibility(ESlateVisibility::Hidden);
	if (CP_Dialog.IsValid())		CP_Dialog->SetVisibility(ESlateVisibility::Hidden);
	if (P_TutorialSkip.IsValid())	P_TutorialSkip->SetVisibility(ESlateVisibility::Hidden);
	if (P_ContentOpen.IsValid())	P_ContentOpen->SetVisibility(ESlateVisibility::Hidden);
	if (P_TutorialReward.IsValid()) P_TutorialReward->SetVisibility(ESlateVisibility::Hidden);
	if (IMG_Arrow.IsValid())		IMG_Arrow->SetVisibility(ESlateVisibility::Hidden);
	if (BTN_Skip.IsValid())			BTN_Skip->SetVisibility(ESlateVisibility::Hidden);
	if (IMG_DialogArrow.IsValid())	IMG_DialogArrow->SetVisibility(ESlateVisibility::Hidden);
	if (CP_HilightMainBtn.IsValid())	CP_HilightMainBtn->SetVisibility(ESlateVisibility::Hidden);

	if (HighlightImage)
		HighlightImage->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UITutorial::OpenDialogContentsOpenUI()
{
	HideAllTutorialUI();
	if (P_ContentOpen.IsValid()) P_ContentOpen->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SetClose(true);
	if (TB_ContentOpenDesc.IsValid()) 
		TB_ContentOpenDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Tutorial_ContentOpenDesc")));

	if (TB_GotoLobby.IsValid())
		TB_GotoLobby->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Tutorial_ContentOpenGoToLobby")));

	OnShowContentOpenUI();
}

void UB2UITutorial::CloseDialogContentsOpenUI()
{
	if (P_ContentOpen.IsValid()) P_ContentOpen->SetVisibility(ESlateVisibility::Hidden);
	SetClose(false);
}

void UB2UITutorial::OpenTutorialRewardUI(const int32 TutorialID, const TArray<b2network::B2RewardPtr>& ServerRewards)
{
	HideAllTutorialUI();

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (P_TutorialReward.IsValid()) 
		P_TutorialReward->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if(TB_TutorialRewardTitle.IsValid())
		TB_TutorialRewardTitle->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_TUTORIAL, TEXT("TutorialClaerNotice")), GetTutorialTitle(TutorialID)));
/*
	if (TB_RewardDestination.IsValid())
		TB_RewardDestination->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Tutorial_RewardDestination")));

	Ʃ�丮�� ���� ������ ��±�� Ȥ�� ���� ���ܳ�

		const int32 iIconMax = 3;
	for (int32 i = 0; i < iIconMax; ++i)
	{
		if (!TutorialRewardIcons.IsValidIndex(i))
		{
			continue;
		}

		if (ServerRewards.IsValidIndex(i))
		{
			TutorialRewardIcons[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TutorialRewardIcons[i]->UpdateRewardIcon(ServerRewards[i]);
		}
		else
		{
			TutorialRewardIcons[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
*/
	//Ʃ�丮�� ���� ������ ��±�� Ȥ�� ���� ���ܳ�

	OnOpenTutorialReward();
}

void UB2UITutorial::CloseTutorialRewardUI()
{
	const int32 CurrentTutorialID = TutorialManager::GetInstance().GetCurrentTutorialID();
	TutorialManager::GetInstance().OnTutorialRewardUIClose(CurrentTutorialID);
	UB2UIManager::GetInstance()->CloseUI(UIFName::Tutorial);
}

void UB2UITutorial::OnAddedHighlightRectButton(UButton* AddedHighlightRectButton)
{
	if (AddedHighlightRectButton)
	{
		AddedHighlightRectButton->OnClicked.RemoveDynamic(this, &UB2UITutorial::OnClickBTN_Highlight);
		AddedHighlightRectButton->OnClicked.AddDynamic(this, &UB2UITutorial::OnClickBTN_Highlight);
	}
}

void UB2UITutorial::ActiveTransparencyBlock(bool bCheck)
{
	if (bCheck == true)
	{
		if (CP_MainTutorial.IsValid())
			CP_MainTutorial->SetVisibility(ESlateVisibility::Hidden);

		if (CP_TransparencyBlock.IsValid())
			CP_TransparencyBlock->SetVisibility(ESlateVisibility::Visible);

		ClearStepHighlight(CurrentTutorialStep);
	}
	else
	{
		if (CP_MainTutorial.IsValid())
			CP_MainTutorial->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (CP_TransparencyBlock.IsValid())
			CP_TransparencyBlock->SetVisibility(ESlateVisibility::Collapsed);

		ProcessStepHighlight(CurrentTutorialStep);
	}
}

void UB2UITutorial::CloseWidgetDelegate()
{
	if (P_ContentOpen.IsValid() && P_ContentOpen->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		OnClickBTN_GotoLobby();
	}
	else if (CP_Dialog.IsValid() && CP_Dialog->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		if (BTN_Skip.IsValid())
		{
			if (BTN_Skip->GetVisibility() == ESlateVisibility::Visible)
			{
				OnClickBTN_Skip();
			}
			else
			{
				OnClickBTN_Highlight();
			}
		}
	}
	else if (P_TutorialSkip.IsValid() && P_TutorialSkip->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		OnClickBTN_SkipCancel();
	}
}

void UB2UITutorial::OnClickBTN_Skip()
{
	const int32 CurrentTutorialID = TutorialManager::GetInstance().GetCurrentTutorialID();
	OpenTutorialSkipNoticeUI(CurrentTutorialID);
}

void UB2UITutorial::OnClickBTN_Highlight()
{
	TutorialManager::GetInstance().OnClickHighlightRect();
}

void UB2UITutorial::OnClickBTN_SkipConfirm()
{
	TutorialManager::GetInstance().SkipCurrentTutorial();
}

void UB2UITutorial::OnClickBTN_SkipCancel()
{
	CloseTutorialSkipNoticeUI();
}

void UB2UITutorial::OnClickBTN_GotoLobby()
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	GoToVillageClass<>::GetInstance().Signal();
	TutorialManager::GetInstance().CloseLobbyMainCombineMenu();
}

void UB2UITutorial::OnClickBTN_RewardConfirm()
{
	if (IsPlayingRewardAnimation())
		return;

	CloseTutorialRewardUI();
}

void UB2UITutorial::OnClickBTN_BlockContentOpen()
{
	//[M22]Ʃ�丮�� �������� ��ġ ���� Ȯ��
	//[M24]Ʃ�丮�� �������� ��ġ ���� ���...������� ��� ���ܵ�
	////�κ�� �̵� ��ư ������ Ȯ�� ��Ű�� ���� ���
	//if (TB_GotoLobby->GetVisibility() != ESlateVisibility::Hidden)
	//{
	//	GoToVillageClass<>::GetInstance().Signal();
	//	TutorialManager::GetInstance().CloseLobbyMainCombineMenu();
	//}
}

void UB2UITutorial::OnClickBTN_MainHighlight()
{
	TutorialManager::GetInstance().OnClickHighlightRect();
}

void UB2UITutorial::ActiveHighlightRect(const FVector2D& TopLeft, const FVector2D& DrawSize)
{
	if (UCanvasPanel* RootPanel = Cast<UCanvasPanel>(GetRootWidget()))
	{
		int32 ViewportX(1280), ViewportY(720);
		if (UCanvasPanel* Canvas = Cast<UCanvasPanel>(GetRootWidget()))
		{
			FVector2D CanvasSize = Canvas->GetCanvasWidget()->GetCachedGeometry().Size;
			ViewportX = CanvasSize.X;
			ViewportY = CanvasSize.Y;

			//��Ȥ ������Ʈ�� ĳ���� �ȵ� ��찡 �־ ��ƽ �ٽ� ����ϵ���
			if (ViewportX <= 0 && ViewportY <= 0)
			{
				bHighlightRedrawCheck = true;
				return;
			}
		}

		SetVisibileHighlightRect(true);

		float Left = TopLeft.X;
		float Right = Left + DrawSize.X;
		float Top = TopLeft.Y;
		float Bottom = Top + DrawSize.Y;

		const FVector2D AnchorBegin = FVector2D(0, 0);
		if (UCanvasPanelSlot* RECT0 = AddHighlightPart(RootPanel, 0))
		{
			/*
			O----------------------------
			|		|			|		|
			|	R	|			|		|
			|	E	|-----------|		|
			|	C	| Highlight	|		|
			|	T	|-----------|		|
			|	0	|			|		|
			--------O--------------------
			*/

			const FVector2D LeftTop(AnchorBegin.X, AnchorBegin.Y);
			const FVector2D RightBtm(Left, AnchorBegin.Y + ViewportY);
			const FVector2D RectSize = RightBtm - LeftTop;
			
			RECT0->SetPosition(LeftTop);
			RECT0->SetSize(RectSize);
			RECT0->SynchronizeProperties();
		}
		if (UCanvasPanelSlot* RECT1 = AddHighlightPart(RootPanel, 1))
		{
			/*
			--------O--------------------
			|		|   RECT1	|		|
			|		|			|		|
			|		|-----------O		|
			|		| Highlight	|		|
			|		|-----------|		|
			|		|			|		|
			-----------------------------
			*/

			const FVector2D LeftTop(Left, AnchorBegin.Y);
			const FVector2D RightBtm(Right, Top);
			const FVector2D RectSize = RightBtm - LeftTop;

			RECT1->SetPosition(LeftTop);
			RECT1->SetSize(RectSize);
			RECT1->SynchronizeProperties();
		}
		if (UCanvasPanelSlot* RECT2 = AddHighlightPart(RootPanel, 2))
		{
			/*
			-----------------------------
			|		|			|		|
			|		|			|		|
			|		|-----------|		|
			|		| Highlight	|		|
			|		O-----------|		|
			|		|	RECT2	|		|
			--------------------O--------
			*/

			const FVector2D LeftTop(Left, Bottom);
			const FVector2D RightBtm(Right, AnchorBegin.Y + ViewportY);
			const FVector2D RectSize = RightBtm - LeftTop;

			RECT2->SetPosition(LeftTop);
			RECT2->SetSize(RectSize);
			RECT2->SynchronizeProperties();
		}

		if (UCanvasPanelSlot* RECT3 = AddHighlightPart(RootPanel, 3))
		{
			/*
			--------------------O--------
			|		|			|		|
			|		|			|	R	|
			|		|-----------|	E	|
			|		| Highlight	|	C	|
			|		|-----------|	T	|
			|		|			|	3	|
			----------------------------O
			*/

			const FVector2D LeftTop(Right, AnchorBegin.Y);
			const FVector2D RightBtm(AnchorBegin.X + ViewportX, AnchorBegin.Y + ViewportY);
			const FVector2D RectSize = RightBtm - LeftTop;

			RECT3->SetPosition(LeftTop);
			RECT3->SetSize(RectSize);
			RECT3->SynchronizeProperties();
		}

		if (HighlightImage)
		{
			if (UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(HighlightImage->Slot))
			{
				PanelSlot->SetPosition(TopLeft);
				PanelSlot->SetSize(DrawSize);
				PanelSlot->SynchronizeProperties();
				PanelSlot->SetZOrder(iHighlightZOder);
			}
		}

		//���� Ʃ�丮���� ��� ���̶���Ʈ ��ġ�� �Ѿ���� ��ư �ǳ� Ȱ��ȭ 
		if (CurrentTutorialStep && (CurrentTutorialStep->Action == ETutorialAction::BlockHilight))
		{
			if (UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(CP_HilightMainBtn->Slot))
			{
				CP_HilightMainBtn->SetVisibility(ESlateVisibility::Visible);
				PanelSlot->SetPosition(TopLeft);
				PanelSlot->SetSize(DrawSize);
				PanelSlot->SynchronizeProperties();
				PanelSlot->SetZOrder(iHighlightZOder);
				PanelSlot->SetAnchors(FAnchors());
			}
		}
	}
}

int32 UB2UITutorial::GetUIWidgetTuTorialID()
{
	if (CurrentTutorialStep)
		return CurrentTutorialStep->TutorialID;

	return -1;
}

int32 UB2UITutorial::GetUIWidgetTuTorialStep()
{
	if (CurrentTutorialStep)
		return CurrentTutorialStep->TutorialStep;

	return -1;
}

#if PLATFORM_IOS
bool IsCurrentDeviceIphonePlus()
{
	FPlatformMisc::EIOSDevice Device = FPlatformMisc::GetIOSDeviceType();
	if (   Device == FPlatformMisc::IOS_IPhone6Plus 
		|| Device == FPlatformMisc::IOS_IPhone6SPlus 
		|| Device == FPlatformMisc::IOS_IPhone7Plus
		|| Device == FPlatformMisc::IOS_IPhone8Plus)
		return true;
	return false;
}

float GetIOSPlatformScale()
{
	float TempDrawScale = 1.04f;
	if (IsCurrentDeviceIphonePlus() == true)
		TempDrawScale = 1.13f;
	
	return TempDrawScale;
}

#endif

void UB2UITutorial::TickHighlight(const FVector2D& fv2Position, const FVector2D& fv2Size)
{
	//�ſ� Ư���� ��Ȳ ���׸� Ʃ�丮�� Ư�� ��Ȳ���� Ʃ�丮�� ���� ����� ���� ���� �Է��� ���� �ִ� ����
	if (CP_TransparencyBlock.IsValid() && CP_TransparencyBlock->GetVisibility() == ESlateVisibility::Visible)
	{
		ClearStepHighlight(CurrentTutorialStep);
		return;
	}

	ActiveHighlightRect(fv2Position, fv2Size);
	ProcessStepHighlight_Arrow(CurrentTutorialStep, fv2Position, fv2Size);
	ProcessStepHighlight_Material(CurrentTutorialStep);
}

void UB2UITutorial::TickHighlightDialog(const FVector2D& fv2Position)
{
	SetTutorialDialog(CurrentTutorialStep, fv2Position);
}

bool UB2UITutorial::IsPlayingRewardAnimation()
{
	if(UWidgetAnimation* RewardAnim = GetAnimation(TEXT("ANIM_TutorialReward_FX")))
	{
		if (IsAnimationPlaying(RewardAnim))
			return true;
	}
	return false;
}

UCanvasPanelSlot* UB2UITutorial::AddHighlightPart(UCanvasPanel* RootCanvasPanel, int32 QuadrantIdx)
{
	BLADE2_SCOPE_CYCLE_COUNTER(UB2UITutorial_AddHighlightPart);
	if (RootCanvasPanel)
	{
		static const int32 QuadrantCount = 4;	// 1 ~ 4�и�
		static FButtonStyle HighlightRectStyle;

		if (HighlightRects.Num() == 0)
		{
			HilightPanels.AddDefaulted(QuadrantCount);
			HighlightRects.AddDefaulted(QuadrantCount);

			FSlateBrush SlateBrush;
			SlateBrush.DrawAs = ESlateBrushDrawType::Image;
			SlateBrush.TintColor = FSlateColor(HighlightRectColor);
			HighlightRectStyle.SetHovered(SlateBrush);
			HighlightRectStyle.SetNormal(SlateBrush);
			HighlightRectStyle.SetPressed(SlateBrush);
		}

		if (HighlightRects.IsValidIndex(QuadrantIdx) && HilightPanels.IsValidIndex(QuadrantIdx))
		{
			UButton* RectPartsButton = HighlightRects[QuadrantIdx];
			if (RectPartsButton == nullptr)
			{
				RectPartsButton = NewObject<UB2Button>(this, NAME_None, RF_Transient);
				HighlightRects[QuadrantIdx] = RectPartsButton;

				check(RectPartsButton != nullptr);
				OnAddedHighlightRectButton(RectPartsButton);

				UCanvasPanelSlot * PanelSlot = RootCanvasPanel->AddChildToCanvas(RectPartsButton);
				PanelSlot->SetAnchors(FAnchors(0, 0));
				HilightPanels[QuadrantIdx] = PanelSlot;
			}

			RectPartsButton->SetVisibility(ESlateVisibility::Visible);
			RectPartsButton->SetStyle(HighlightRectStyle);

			RectPartsButton->SetRenderScale(FVector2D(1.f, 1.f));

			return HilightPanels[QuadrantIdx];
		}
	}

	return nullptr;
}