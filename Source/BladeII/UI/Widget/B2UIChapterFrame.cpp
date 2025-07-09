#include "BladeII.h"
#include "B2UIChapterFrame.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocChapter.h"
#include "B2UIChapter.h"
#include "B2UIChapterStarRewardBox.h"
#include "B2UIMsgPopupReward.h"
#include "Retailer.h"
#include "B2UIMailRewardPopUp.h"
#include "TutorialManager.h"


#include "BladeIIGameImpl.h"

UB2UIChapterFrame::UB2UIChapterFrame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_TempChapter = nullptr;
	ChapterStagePartClass_Holder = nullptr; // ���⼭ ���� ������ ������ �ʴ�.. �׳� Holder
	CurChapterNum = INDEX_NONE;
	CurDifficulty = EStageDifficulty::ESD_None;
	PrefixChapterStarReward = TEXT("UIP_ChapterStarRewardBox");
	StarRewardBoxes.Empty();
	IsOvered10Stages = false;

	CreatedActOpenUI = nullptr;
	CreatedActClearUI = nullptr;
}

void UB2UIChapterFrame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (BTN_StarRewardPreviewAllClose.IsValid() && BTN_StarRewardPreviewAllClose->GetVisibility() == ESlateVisibility::Visible
		&& !IsStarRewardPreviewOpen())
	{ // Ȥ�ö� ��Ʈ ���� ��ư �� �ٸ� ��θ� ���� ���� ���� �ִµ�, �߰��� Delegate �༭ ó���� ���� ������ �� �������� �������� �� ���Ƽ� Tick ����..
		BTN_StarRewardPreviewAllClose->SetVisibility(ESlateVisibility::Hidden);
	}
}
void UB2UIChapterFrame::DestroySelf(class UB2UIManager* InUIManager)
{
	if (CreatedActOpenUI) {
		CreatedActOpenUI->DestroySelf(InUIManager);
		CreatedActOpenUI = nullptr;
	}
	if (CreatedActClearUI) {
		CreatedActClearUI->DestroySelf(InUIManager);
		CreatedActClearUI = nullptr;
	}

	if (UIP_StarRewardPreviewP->IsOpen())
	{
		UIP_StarRewardPreviewP->SetPopupVisible(false);
	}

	Super::DestroySelf(InUIManager);
}

void UB2UIChapterFrame::CacheAssets()
{
	GET_SLOT(UButton, BTN_AdventureLastReached)
	GET_SLOT(UButton, BTN_Next)
	GET_SLOT(UButton, BTN_Prev)
	GET_SLOT(UPanelWidget, CON_Chapter)
	
	GET_SLOT(UImage, IMG_Normal)
	GET_SLOT(UImage, IMG_SelectedNormal)
	GET_SLOT(UImage, IMG_Hard)
	GET_SLOT(UImage, IMG_SelectedHard)
	GET_SLOT(UImage, IMG_VeryHard)
	GET_SLOT(UImage, IMG_SelectedVeryHard)
	GET_SLOT(UImage, IMG_Hell)
	GET_SLOT(UImage, IMG_SelectedHell)

	//�������� ����� �����´�. �ϴ� �˳��ϰ� 6���� ���
	for (int Cnt = 0; Cnt < 6; ++Cnt)
	{
		FString StarRewardAssetStr = PrefixChapterStarReward + FString::FormatAsNumber(Cnt);
		auto StarRewardBoxAsset = GetSlot<UB2UIChapterStarRewardBox>(FName(*StarRewardAssetStr));
		if (StarRewardBoxAsset)
		{
			StarRewardBoxes.AddUnique(StarRewardBoxAsset);
			StarRewardBoxAsset->Init();
			StarRewardBoxAsset->SetRewardIndex(Cnt + 1); // �������� 1, 2, 3 �̷��� ����.
		}
	}
	// ������ ��Ʈ�� �ϳ���. �� ���� ���� Ŭ���ø��� �ش� ������ ���õ� ��.
	GET_SLOT(UB2UIChapterStarRewardPreview, UIP_StarRewardPreviewP)
	if (UIP_StarRewardPreviewP.IsValid())
	{
		UIP_StarRewardPreviewP->Init();
	}

	GET_SLOT(UB2Button, BTN_StarRewardPreviewAllClose)
	if (BTN_StarRewardPreviewAllClose.IsValid()) // ��ü ������ ���� �ִ� �Ŷ� �⺻�����δ� �Է��� �޾Ƽ� �ȵȴ�. �ʿ��� ���� ��
	{
		BTN_StarRewardPreviewAllClose->SetVisibility(ESlateVisibility::Hidden);
	}

	GET_SLOT(UTextBlock, TX_LastDG)
	GET_SLOT(UTextBlock, STB_Easy)
	GET_SLOT(UTextBlock, STB_Normal)
	GET_SLOT(UTextBlock, STB_Hard)
	GET_SLOT(UTextBlock, STB_Hell)

	GET_SLOT(UB2Button, BTN_Normal)
	GET_SLOT(UB2Button, BTN_Hard)
	GET_SLOT(UB2Button, BTN_VeryHard)
	GET_SLOT(UB2Button, BTN_Hell)

	GET_SLOT(UPanelWidget, O_Normal)
	GET_SLOT(UPanelWidget, O_Hard)
	GET_SLOT(UPanelWidget, O_VeryHard)
	GET_SLOT(UPanelWidget, O_Hell)

	BTN_Adventures.Add(EStageDifficulty::ESD_Normal, BTN_Normal);
	BTN_Adventures.Add(EStageDifficulty::ESD_Hard, BTN_Hard);
	BTN_Adventures.Add(EStageDifficulty::ESD_VeryHard, BTN_VeryHard);
	BTN_Adventures.Add(EStageDifficulty::ESD_Hell, BTN_Hell);

	P_Adventures.Add(EStageDifficulty::ESD_Normal, O_Normal);
	P_Adventures.Add(EStageDifficulty::ESD_Hard, O_Hard);
	P_Adventures.Add(EStageDifficulty::ESD_VeryHard, O_VeryHard);
	P_Adventures.Add(EStageDifficulty::ESD_Hell, O_Hell);

	GET_SLOT(UCanvasPanel, X_CP_ActOpenClearUICreatePanel);
}

void UB2UIChapterFrame::UpdateStaticText()
{
	if (TX_LastDG.IsValid())
	{
		TX_LastDG->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChapterToLastAdventureBtn")));
	}
	if (STB_Easy.IsValid())
	{
		STB_Easy->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_DifficultyEasy")));
	}
	if (STB_Normal.IsValid())
	{
		STB_Normal->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_DifficultyHard")));
	}
	if (STB_Hard.IsValid())
	{
		STB_Hard->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_DifficultyVeryHard")));
	}
	if (STB_Hell.IsValid())
	{
		STB_Hell->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("BattleStageInfo_DifficultyHell")));
	}
}

void UB2UIChapterFrame::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_AdventureLastReached, &UB2UIChapterFrame::OnClickBtnAdventureLastReached)
	BIND_CLICK_FUNC_TO_BTN(BTN_Next, &UB2UIChapterFrame::OnClickBtnNext)
	BIND_CLICK_FUNC_TO_BTN(BTN_Prev, &UB2UIChapterFrame::OnClickBtnPrev)
	BIND_CLICK_FUNC_TO_BTN(BTN_StarRewardPreviewAllClose, &UB2UIChapterFrame::OnClickBtnStarRewardPreviewAllClose)
	BIND_CLICK_FUNC_TO_BTN(BTN_Normal, &UB2UIChapterFrame::OnClickBTN_Normal)
	BIND_CLICK_FUNC_TO_BTN(BTN_Hard , &UB2UIChapterFrame::OnClickBTN_Hard)
	BIND_CLICK_FUNC_TO_BTN(BTN_VeryHard, &UB2UIChapterFrame::OnClickBTN_VeryHard)
	BIND_CLICK_FUNC_TO_BTN(BTN_Hell, &UB2UIChapterFrame::OnClickBTN_Hell)

	for (auto StarRewardBox : StarRewardBoxes)
		StarRewardBox->OnChapterStarRewardBoxClickedDelgate.BindDynamic(this, &UB2UIChapterFrame::OnClickBtnChapterStarRewardBox);
}

void UB2UIChapterFrame::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION_LAMBDA(GetRedDotWidget(TEXT("O_Normal")), [this]() {return this->RedDotCondition_ChapterInternal(EStageDifficulty::ESD_Normal); });
	REGISTER_REDDOT_CONDITION_LAMBDA(GetRedDotWidget(TEXT("O_Hard")), [this]() {return this->RedDotCondition_ChapterInternal(EStageDifficulty::ESD_Hard); });
	REGISTER_REDDOT_CONDITION_LAMBDA(GetRedDotWidget(TEXT("O_VeryHard")), [this]() {return this->RedDotCondition_ChapterInternal(EStageDifficulty::ESD_VeryHard); });
	REGISTER_REDDOT_CONDITION_LAMBDA(GetRedDotWidget(TEXT("O_Hell")), [this]() {return this->RedDotCondition_ChapterInternal(EStageDifficulty::ESD_Hell); });

	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Prev.Get()), &UB2UIChapterFrame::RedDotCondition_PrevChapter);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Next.Get()), &UB2UIChapterFrame::RedDotCondition_NextChapter);
}


void UB2UIChapterFrame::BindDocAuto()
{
	UnbindDoc();

	auto DocChapter = UB2UIDocHelper::GetDocChapter();
	SetDoc(DocChapter);
	if (DocChapter)
	{
		ChangeChapter(DocChapter->GetCurChapterNum(), DocChapter->GetCurStageDifficulty());
		SetChapterTitle(DocChapter->GetCurChapterName());
		SetStarRewards(DocChapter->GetStarRewards());
		SetMaxOpendAdventureDifficulty(DocChapter->GetMaxOpenedStageDifficulty());

		DocChapter->OnCurChapterNumChanged.AddUObject(this, &UB2UIChapterFrame::OnChangedCurChapterNum);
		DocChapter->OnCurChapterNameChanged.AddUObject(this, &UB2UIChapterFrame::OnChangedCurChapterName);
		DocChapter->OnStarRewardsChanged.AddUObject(this, &UB2UIChapterFrame::OnChangedStarRewards);
		DocChapter->OnMaxOpenedStageDifficultyChanged.AddUObject(this, &UB2UIChapterFrame::OnChangedMaxOpenedStageDifficulty);
		DocChapter->OnTutorialFixSwipeChanged.AddUObject(this, &UB2UIChapterFrame::OnChangedTutorialFixSwipe);
	}

	//���� ����Ʈ�� Chapter�� ���� ���´� �����ϰ� ���� ������, ���� �׷�ó���� DocChapter�� �־�� �ɰ�.
}

void UB2UIChapterFrame::UnbindDoc()
{
	auto DocChapter = UB2UIDocHelper::GetDocChapter();
	if (DocChapter)
	{
		DocChapter->OnCurChapterNumChanged.RemoveAll(this);
		DocChapter->OnCurChapterNameChanged.RemoveAll(this);
		DocChapter->OnStarRewardsChanged.RemoveAll(this);
		DocChapter->OnTutorialFixSwipeChanged.RemoveAll(this);
	}
	SetDoc(nullptr);
}

void UB2UIChapterFrame::ChangeChapterCycle(EUISearchType SearchDir)
{
	// �Ʒ� ChangeChapter �� �ٺ������� �ٸ���. 
	// �Ʒ� ChangeChapter �� �������� ���� ������ �ִ� �� Ȯ�� �� ���Ŀ� ���� ���������� �並 �ٲٴ� �����.
	// ���� �ϴ� ��û���� �ϰ� ��.

	auto DocChapter = Cast<UB2UIDocChapter>(GetDoc());
	if (!DocChapter) return;

	const int32 ChapterNumBackup = CurChapterNum;

	switch (SearchDir)
	{
	case EUISearchType::First:
		CurChapterNum = 1;
		break;
	case EUISearchType::Forward:

		if (ShouldShowCBTVersionContent() && CurChapterNum >= 3)	//CBT 4�� ������ ����
		{
			DevShowNotReadyYetMessage(TEXT("CBT_AreaLimit"));
			break;
		}

		if (CurChapterNum < DocChapter->GetChapterOpenedNum())
		{
			++CurChapterNum;
			PlayNextChapterTransitionSound(ChapterNumBackup, CurChapterNum);
		}
		break;
	case EUISearchType::Reverse:
		if (CurChapterNum > 1)
		{
			CurChapterNum--;
			PlayPrevChapterTransitionSound(ChapterNumBackup, CurChapterNum);
		}
		break;
	case EUISearchType::End: 
		CurChapterNum = DocChapter->GetChapterOpenedNum();
		break; 
	} 

	// �������� ChangeChapter �� �Ҹ�����., �ƴϸ� �̹� �����Ͱ� ������ ChangeChapter �� �Ҹ�����.
	BladeIIGameImpl::GetRedDotManager().MarkAct(CurDifficulty, CurChapterNum);
	BladeIIGameImpl::GetStageDataStore().RequestGetActInfoAndChangeChapter(CurChapterNum, CurDifficulty);
}

void UB2UIChapterFrame::ChangeChapter(int32 TargetChapterNum, EStageDifficulty TargetDifficulty)
{
	auto DocChapter = Cast<UB2UIDocChapter>(GetDoc());
	if (!DocChapter) return;

	//���� �Ұ��� ChapterNum���� üũ
	/*if (TargetChapterNum <= INDEX_NONE || DocChapter->GetChapterMaxNum() < TargetChapterNum) 
		return;*/

	CurChapterNum = TargetChapterNum;
	CurDifficulty = TargetDifficulty;

	// ȣ�� ������ ��ġ�ٰ� ������ �Ҹ� �� �־ �������� CreateChapter �� ����
	if (!m_TempChapter || m_TempChapter->GetChapterNum() != TargetChapterNum || m_TempChapter->GetStageDifficulty() != TargetDifficulty)
	{	
		// é�Ͱ� �ٲ�� ��쵵 UIChapter WidgetClass �� �����ϰ� �� �������� ���� ��� ������ �������ִ� ���� ������ ��ŵ�Ѵ�.
		bool bNeedCreate = true;
		if (m_TempChapter && CheckIfTwoChapterUIsShareClass(m_TempChapter->GetChapterNum(), TargetChapterNum))
		{
			bNeedCreate = false;
		}
		B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("UB2UIChapterFrame::ChangeChapter Internal.. NeedCreate %d"), (int32)bNeedCreate));
		UB2UIChapter* UIChapterToSetup = m_TempChapter;
		if (bNeedCreate)
		{
			UIChapterToSetup = CreateChapter(TargetChapterNum);
			ClearCurChapter();

			if (CON_Chapter.IsValid())
			{
				auto AddedSlot = CON_Chapter->AddChild(UIChapterToSetup);
				auto OverlaySlot = Cast<UOverlaySlot>(AddedSlot);
				if (OverlaySlot)
				{
					OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
					OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				}
			}
		}
		check(UIChapterToSetup);

		UIChapterToSetup->SetChapterNumOnly(CurChapterNum); // �̰� �� �ʿ��� ��Ȳ�� ����..
		UIChapterToSetup->Init();

		UIChapterToSetup->SetChapterNum(TargetChapterNum, TargetDifficulty);
		UIChapterToSetup->SetStageDatas(/*TArray<FStage��¼��> StageDatas*/);

		m_TempChapter = UIChapterToSetup;
	}

	//�¿� �̵���ư Enabledó��
	UpdateSearchBtnEnabled(TargetChapterNum > DocChapter->GetChapterOpenedNum() ? TargetChapterNum : DocChapter->GetChapterOpenedNum());
	OnSelectBtnStageDifficulty(CurDifficulty);

	LobbySetCombatEntryChapterNumClass<int32, EStageDifficulty>::GetInstance().Signal(CurChapterNum, CurDifficulty);
	
	TArray<FUIChapterStarReward> Rewards;
	BladeIIGameImpl::GetStageDataStore().GetActRewardData(CurChapterNum, CurDifficulty, Rewards);
	DocChapter->SetStarRewards(Rewards);

	for (auto* StartRewardBox : StarRewardBoxes)
		StartRewardBox->SetFillImage(TargetDifficulty);

	if (BladeIIGameImpl::GetStageDataStore().IsFirstActClear(CurChapterNum))
	{
		PlayActClear();
		BladeIIGameImpl::GetStageDataStore().SaveClearAct(CurChapterNum);
	}

	else if (BladeIIGameImpl::GetStageDataStore().IsFirstActOpen(CurChapterNum))
	{	
		PlayActOpen();
		BladeIIGameImpl::GetStageDataStore().SaveOpenAct(CurChapterNum);
	}
}

bool UB2UIChapterFrame::CheckIfTwoChapterUIsShareClass(int32 CheckChapterNum1, int32 CheckChapterNum2) const
{
	// �� é�� ��ȣ�� ��ϵ� ChapterWidgetClasses �� ���� ������ �Ǻ�. ������ ���� �������� �ʵ��� �ϱ� ����.
	const TSoftClassPtr<class UB2UIChapter>* ChapterSetup1 = ChapterWidgetClasses.Find(CheckChapterNum1);
	const TSoftClassPtr<class UB2UIChapter>* ChapterSetup2 = ChapterWidgetClasses.Find(CheckChapterNum2);
	if (ChapterSetup1 && ChapterSetup2)
	{
		return (ChapterSetup1->GetAssetName() == ChapterSetup2->GetAssetName());
	}
	return false;
}

void UB2UIChapterFrame::SetChapterDynamicSetupReady()
{
	m_TempChapter->bExternalDynamicSetupReady = true;

	if (UB2UIDocChapter* DocChapter = UB2UIDocHelper::GetDocChapter())
	{
		// Tutorial0-1 ��Ȳ���� Swipe�ȵǰԲ�
		if (DocChapter->GetTutorialFixSwipe())
			m_TempChapter->bExternalDynamicSetupReady = false;
	}
}

UB2UIChapter* UB2UIChapterFrame::CreateChapter(int32 TargetChaperNum)
{
	TSubclassOf<UB2UIChapter> ChapterClass = GetChapterWidgetClass(TargetChaperNum);
	auto ChapterAsset = Cast<UB2UIChapter>(CreateWidget<UB2UIChapter>(GetWorld(), ChapterClass));
	if (ChapterAsset)
	{
		return ChapterAsset;
	}
	return nullptr;
	
}

UB2UIChapter* UB2UIChapterFrame::GetCurChapter()
{
	if (!CON_Chapter.IsValid())
		return NULL;

	for (int32 Cnt = 0; Cnt < CON_Chapter->GetChildrenCount(); ++Cnt)
	{
		auto UIChapter = Cast<UB2UIChapter>(CON_Chapter->GetChildAt(Cnt));
		if (UIChapter)
		{
			return UIChapter;
		}
	}

	return NULL;
}

TSubclassOf<UB2UIChapter> UB2UIChapterFrame::GetChapterWidgetClass(int32 TargetChaperNum)
{
	// KeepLoadedChapterWidgetClasses �� Ȧ���ϰ� �ִ� �Ŷ�� ���⼭ ���� �ε��� �Ͼ���� ���� ��..
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("UB2UIChapterFrame::GetChapterWidgetClass %d"), TargetChaperNum));

	TSoftClassPtr<class UB2UIChapter>* FoundElem = ChapterWidgetClasses.Find(TargetChaperNum);
	return FoundElem ? (*FoundElem).LoadSynchronous() : nullptr;
}

void UB2UIChapterFrame::ClearCurChapter()
{
	if (!CON_Chapter.IsValid())return;

	for (int32 Cnt = 0; Cnt < CON_Chapter->GetChildrenCount(); ++Cnt)
	{
		auto Chapter = Cast<UB2UIChapter>(CON_Chapter->GetChildAt(Cnt));
		if (Chapter)
		{
			Chapter->DestroySelf(UB2UIManager::GetInstance());
		}
	}
}

void UB2UIChapterFrame::SetChapterTitle(const FText& InText)
{
	//Ÿ��Ʋ�ؽ�Ʈó��. (DocUICondition�� UI���ο��� �����ϰ� Set�� �� �� �ִ� Doc�̴�. ������ Doc���� View�� UI�� Set�ϸ� �ȵ�)
	auto DocUICondition = UB2UIDocHelper::GetDocUICondition();
	if (DocUICondition)
	{
		DocUICondition->SetHeaderTitle(InText);	//"�� 1 ��  é���̸�" ���� �������� �ٲ��ٰ�
	}
}

void UB2UIChapterFrame::SetEnabledNextBtn(const bool bEnabled)
{
	if (BTN_Next.IsValid()){
		BTN_Next->SetIsEnabled(bEnabled);
	}
}

void UB2UIChapterFrame::SetEnabledPrevBtn(const bool bEnabled)
{
	if (BTN_Prev.IsValid())
		BTN_Prev->SetIsEnabled(bEnabled);
}

void UB2UIChapterFrame::SetVisibilityPrevNextButton(bool InVisibility)
{
	if (BTN_Next.IsValid() && BTN_Prev.IsValid())
	{
		BTN_Next->SetVisibility(InVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		BTN_Prev->SetVisibility(InVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UB2UIChapterFrame::SetStarRewards(const TArray<FUIChapterStarReward>& StarRewards)
{
	int32 StarCurNum = 0;
	int32 StartMaxNum = 0;
	int32 PrevGoalStarNum = 0;
	bool bPrevGoalReached = false;

	for (int AssetCnt = 0; AssetCnt < StarRewardBoxes.Num(); ++AssetCnt)
	{
		UB2UIChapterStarRewardBox* StarRewardAsset = StarRewardBoxes[AssetCnt];
		if (StarRewardAsset)
		{
			int32 RewardDataLength = StarRewards.Num();
			if (RewardDataLength > AssetCnt)
			{
				StarRewardAsset->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				auto StarRewardData = StarRewards[AssetCnt];
				StarRewardAsset->SetRewarded(StarRewardData.bAlreadyRewarded);
				StarRewardAsset->SetMyPreviousRewarded(AssetCnt == 0 ? true : StarRewards[AssetCnt - 1].bAlreadyRewarded); // 0 ���� ���� �� ���� ���� ������ true.

				StartMaxNum = StarRewardData.StarMaxNum;
				StarCurNum = StarRewardData.StarCurNum;

				StartMaxNum -= PrevGoalStarNum;

				bool bCurGoalReached = false;
				if (StarRewardData.StarMaxNum <= StarCurNum) 
					bCurGoalReached = true;

				if (bCurGoalReached)
				{
					StarCurNum = StarRewardData.StarMaxNum;
					StarRewardAsset->ShowProgressText(false);
				}
				else
				{
					if (bPrevGoalReached || AssetCnt == 0)
					{
						StarCurNum -= PrevGoalStarNum;
						StarRewardAsset->ShowProgressText(true);
					}
					else
					{
						StarCurNum = 0;
						StarRewardAsset->ShowProgressText(false);
					}
				}

				//���� ���α׷��� ���������� max�� ����
				StarRewardAsset->SetStarMaxNum(StartMaxNum);

				//���� ���α׷��� ���������� cur�� ����
				StarRewardAsset->SetStarCurNum(StarCurNum);

				//���� ���α׷��� ���������� cur,max���� ����������, �׿ʹ� ������ �ؽ�Ʈ ǥ�ô� ��� ������ ���������� �ؽ�Ʈ�� ǥ�����־�� �Ѵٰ� �Ѵ�.
				StarRewardAsset->SetForceProgressText(StarRewardData.StarCurNum, StarRewardData.StarMaxNum);
				
				PrevGoalStarNum = StarRewardData.StarMaxNum;
				bPrevGoalReached = bCurGoalReached;
			}
			else
				StarRewardAsset->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			StarRewardAsset->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	DoMarkRedDot();
}

void UB2UIChapterFrame::SetVisibilityBtnAdventure(EStageDifficulty InStageDifficulty, const bool bEnabled)
{
	if (P_Adventures.Contains(InStageDifficulty) == false)
		return;

	if (P_Adventures[InStageDifficulty].IsValid() == false)
		return;

	P_Adventures[InStageDifficulty]->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UB2UIChapterFrame::SetMaxOpendAdventureDifficulty(EStageDifficulty InMaxStageDifficulty)
{
	for (auto AdventurePanel : P_Adventures)
	{
		bool IsVisible = (InMaxStageDifficulty == EStageDifficulty::ESD_Normal) ? false : ((int32)AdventurePanel.Key <= (int32)InMaxStageDifficulty);
		
		//[180712_YJ] Ʃ�丮���� ������� ����.
		if ((int32)AdventurePanel.Key > BladeIIGameImpl::GetClientDataStore().GetLimitOpenStageDifficulty())
			IsVisible = false;
			
		SetVisibilityBtnAdventure(AdventurePanel.Key, IsVisible);
	}
}

bool UB2UIChapterFrame::HasTutorialChangeChapter1Check(EStageDifficulty eStageDifficulty)
{
	if (TutorialManager::GetInstance().HasAnyTutorial())
	{
		CurChapterNum = 1;
		CurDifficulty = eStageDifficulty;
		BladeIIGameImpl::GetStageDataStore().RequestGetActInfoAndChangeChapter(CurChapterNum, CurDifficulty);
		return true;
	}
	else
	{
		return false;
	}	
}

void UB2UIChapterFrame::UpdateSearchBtnEnabled(int32 ChapterOpenedNum)
{
	//�ϳ��ۿ� �ȿ������� �¿� ����
	SetVisibilityPrevNextButton(ChapterOpenedNum <= 5 ? true : false);


	if (ChapterOpenedNum <= 1)
	{
		SetEnabledNextBtn(false);
		SetEnabledPrevBtn(false);
		return;
	}

	if (CurChapterNum == 1)
	{
		SetEnabledNextBtn(true);
		SetEnabledPrevBtn(false);
	}
	else if (ShouldShowCBTVersionContent() && CurChapterNum == 3)	//[CBT])
	{
		SetEnabledNextBtn(true);
		SetEnabledPrevBtn(true);
	}
	else if (CurChapterNum == ChapterOpenedNum)
	{
		SetEnabledNextBtn(false);
		SetEnabledPrevBtn(true);
	}
	else if (CurChapterNum > 1 && CurChapterNum < ChapterOpenedNum)
	{
		SetEnabledNextBtn(true);
		SetEnabledPrevBtn(true);
	}
}

void UB2UIChapterFrame::OnSelectBtnStageDifficulty(EStageDifficulty InDifficulty)
{
	if (IMG_Normal.IsValid())
		IMG_Normal->SetVisibility(InDifficulty == EStageDifficulty::ESD_Normal ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	if (IMG_SelectedNormal.IsValid())
		IMG_SelectedNormal->SetVisibility(InDifficulty == EStageDifficulty::ESD_Normal ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_Hard.IsValid())
		IMG_Hard->SetVisibility(InDifficulty == EStageDifficulty::ESD_Hard ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	if (IMG_SelectedHard.IsValid())
		IMG_SelectedHard->SetVisibility(InDifficulty == EStageDifficulty::ESD_Hard ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_VeryHard.IsValid())
		IMG_VeryHard->SetVisibility(InDifficulty == EStageDifficulty::ESD_VeryHard ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	if (IMG_SelectedVeryHard.IsValid())
		IMG_SelectedVeryHard->SetVisibility(InDifficulty == EStageDifficulty::ESD_VeryHard ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (IMG_Hell.IsValid())
		IMG_Hell->SetVisibility(InDifficulty == EStageDifficulty::ESD_Hell ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
	if (IMG_SelectedHell.IsValid())
		IMG_SelectedHell->SetVisibility(InDifficulty == EStageDifficulty::ESD_Hell ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

UB2UIWidget* UB2UIChapterFrame::CreateDynamicActFxUIP(UClass* InCreateWidgetClass, UCanvasPanel* CreateParentPanel)
{
	if (InCreateWidgetClass && CreateParentPanel)
	{
		UB2UIWidget* NewCreated = Cast<UB2UIWidget>(DynCreateInCanvasPanelFull(InCreateWidgetClass, this, CreateParentPanel));
		if (NewCreated)
		{
			NewCreated->Init();
			return NewCreated;
		}
	}
	return nullptr;
}

void UB2UIChapterFrame::PlayActOpen()
{
	if (!CreatedActOpenUI) {
		CreatedActOpenUI = Cast<UB2UIActOpen>(CreateDynamicActFxUIP(UIPActOpenClass.LoadSynchronous(), X_CP_ActOpenClearUICreatePanel.Get()));
	}
	if (CreatedActOpenUI)
	{
		CreatedActOpenUI->SetVisibility(ESlateVisibility::Visible);
		CreatedActOpenUI->SetTextData(CurChapterNum);
		CreatedActOpenUI->PlayOpenAnim();
	}
}
void UB2UIChapterFrame::PlayActClear()
{
	if (!CreatedActClearUI) {
		CreatedActClearUI = Cast<UB2UIActClear>(CreateDynamicActFxUIP(UIPActClearClass.LoadSynchronous(), X_CP_ActOpenClearUICreatePanel.Get()));
	}
	if (CreatedActClearUI)
	{
		CreatedActClearUI->SetVisibility(ESlateVisibility::Visible);
		CreatedActClearUI->SetTextData(CurChapterNum);
		CreatedActClearUI->PlayClearAnim();
	}
}

bool UB2UIChapterFrame::IsStarRewardPreviewOpen()
{
	return (UIP_StarRewardPreviewP.IsValid() && UIP_StarRewardPreviewP->IsOpen());
}


UB2UIChapterStarRewardBox* UB2UIChapterFrame::GetStarRewardBoxOfRewardIndex(int32 InRewardIndex)
{
	for (UB2UIChapterStarRewardBox* ThisBox : StarRewardBoxes)
	{
		if (ThisBox && ThisBox->GetRewardIndex() == InRewardIndex)
		{
			return ThisBox;
		}
	}
	return NULL;
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UIChapterFrame::OnClickBtnAdventureLastReached()
{
	//���������� ������ �������� �ٷΰ���
	GoGameStageInfoLastReachedFromLobbyClass<>::GetInstance().Signal();
}


void UB2UIChapterFrame::OnClickBtnNext()
{
	ChangeChapterCycle(EUISearchType::Forward);
}

void UB2UIChapterFrame::OnClickBtnPrev()
{
	ChangeChapterCycle(EUISearchType::Reverse);
}

void UB2UIChapterFrame::OnClickBtnChapterStarRewardBox(UB2UIChapterStarRewardBox* ClickedReward)
{	//������ �ڽ��� ������. 
	// ���° ���������� �����ʿ��� ó����. ���ʿ��� �̹��� ���� ��û ���� ��ư�� enable
	UB2UIDocChapter* DocChapter = UB2UIDocHelper::GetDocChapter();
	if (ClickedReward && ClickedReward->GetGoalReachedState() != EUIChapterStarRewardState::Rewarded && UIP_StarRewardPreviewP.IsValid() && DocChapter)
	{
		int32 PreviewRewardGold = 0, PreviewRewardGem = 0;
		TArray<b2network::B2RewardPtr> OutRewards;
		BladeIIGameImpl::GetStageDataStore().GetActRewardPreviewData(CurChapterNum, CurDifficulty, ClickedReward->GetRewardIndex(), OutRewards);

		const TArray<FUIChapterStarReward>& StarRewards = DocChapter->GetStarRewards();
		const int32 CurStarNum = (StarRewards.Num() > 0) ? StarRewards[0].StarCurNum : 0;

		//UB2UIChapterStarRewardBox* PrevIndexRewardBox = GetStarRewardBoxOfRewardIndex(ClickedReward->GetRewardIndex() - 1);
		const bool bCanGetRewardForThisIndex = (ClickedReward->GetGoalReachedState() == EUIChapterStarRewardState::Reached); /* &&
			// ���� �ε����� RewardBox �� �ִٸ� ������ ���� ���¿����� �̹� �ε����� RewardBox ������ �����ϵ��� ��.
			(PrevIndexRewardBox || PrevIndexRewardBox->GetGoalReachedState() == EUIChapterStarRewardState::Rewarded));*/


		UIP_StarRewardPreviewP->ShowMe(bCanGetRewardForThisIndex, CurChapterNum, CurDifficulty, ClickedReward->GetRewardIndex(), CurStarNum,
			ClickedReward->GetStarMaxNum(), OutRewards);
	
		if (BTN_StarRewardPreviewAllClose.IsValid())
		{ // �˾��Ǹ� ��ü ��ũ���� ���� �ִ� ���� ��ư�� Ȱ��ȭ ��.. ���� ���� ���� �ִ� �ǵ� ���� �־ ���� �� ���� ��?
			BTN_StarRewardPreviewAllClose->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UB2UIChapterFrame::OnClickBtnStarRewardPreviewAllClose()
{// ��Ʈ �ȿ��� ����� �ִµ�.. �ϴ� �̰͵� ���ܵ���.
	if (UIP_StarRewardPreviewP.IsValid())
	{
		UIP_StarRewardPreviewP->CloseMe();
	}
	if (BTN_StarRewardPreviewAllClose.IsValid())
	{ // �̰� ����� ������ �ٽ� �ڷ� �������� �ٸ� ��ư�� ������ �� �ֵ���.
		BTN_StarRewardPreviewAllClose->SetVisibility(ESlateVisibility::Hidden);
	}
}

//====================================================================================
//									Doc Delegate
//====================================================================================
void UB2UIChapterFrame::OnChangedCurChapterNum(class UB2UIDocBase* Sender, int32 CurrChapterNum, int32 PrevCurrChapterNum)
{
	if (CurrChapterNum != PrevCurrChapterNum)
	{
		auto DocChapter = Cast<UB2UIDocChapter>(UB2UIDocHelper::GetDocChapter());
		if (DocChapter != nullptr)
		{
			BladeIIGameImpl::GetStageDataStore().RequestGetActInfoAndChangeChapter(CurrChapterNum, DocChapter->GetCurStageDifficulty());
		}
	}		
}

void UB2UIChapterFrame::OnChangedCurChapterName(class UB2UIDocBase* Sender, FText CurChapterName, FText PrevCurChapterName)
{
	SetChapterTitle(CurChapterName);
}

void UB2UIChapterFrame::OnChangedStarRewards(class UB2UIDocBase* Sender, TArray<FUIChapterStarReward> StarRewards, TArray<FUIChapterStarReward> PrevStarRewards)
{
	SetStarRewards(StarRewards);
}

void UB2UIChapterFrame::OnChangedMaxOpenedStageDifficulty(class UB2UIDocBase* Sender, EStageDifficulty CurrentDifficulty, EStageDifficulty PrevDifficulty)
{
	SetMaxOpendAdventureDifficulty(CurrentDifficulty);

	DoMarkRedDot();
}

void UB2UIChapterFrame::OnChangedTutorialFixSwipe(class UB2UIDocBase* Sender, bool CurFixSwipe, bool PrevFixSwipe)
{
	m_TempChapter->bExternalDynamicSetupReady = !CurFixSwipe;
}

void UB2UIChapterFrame::OnClickBTN_Normal()
{
	auto DocChapter = Cast<UB2UIDocChapter>(GetDoc());
	if (DocChapter == nullptr)
		return;

	if (CurDifficulty == EStageDifficulty::ESD_Normal)
		return;

	OnSelectBtnStageDifficulty(EStageDifficulty::ESD_Normal);
	BladeIIGameImpl::GetStageDataStore().RequestGetActInfoAndChangeChapter(DocChapter->GetCurChapterNum(), EStageDifficulty::ESD_Normal);
}

void UB2UIChapterFrame::OnClickBTN_Hard()
{
	auto DocChapter = Cast<UB2UIDocChapter>(GetDoc());
	if (DocChapter == nullptr)
		return;

	if (CurDifficulty == EStageDifficulty::ESD_Hard)
		return;

	OnSelectBtnStageDifficulty(EStageDifficulty::ESD_Hard);

	if (HasTutorialChangeChapter1Check(EStageDifficulty::ESD_Hard) == false)
	{
		int32 SelectChapter = BladeIIGameImpl::GetStageDataStore().GetMaxReachedActNumber(EStageDifficulty::ESD_Hard);
		BladeIIGameImpl::GetStageDataStore().RequestGetActInfoAndChangeChapter(SelectChapter, EStageDifficulty::ESD_Hard);
	}
}

void UB2UIChapterFrame::OnClickBTN_VeryHard()
{
	auto DocChapter = Cast<UB2UIDocChapter>(GetDoc());
	if (DocChapter == nullptr)
		return;

	if (CurDifficulty == EStageDifficulty::ESD_VeryHard)
		return;

	OnSelectBtnStageDifficulty(EStageDifficulty::ESD_VeryHard);

	if (HasTutorialChangeChapter1Check(EStageDifficulty::ESD_VeryHard) == false)
	{
		int32 SelectChapter = BladeIIGameImpl::GetStageDataStore().GetMaxReachedActNumber(EStageDifficulty::ESD_VeryHard);
		BladeIIGameImpl::GetStageDataStore().RequestGetActInfoAndChangeChapter(SelectChapter, EStageDifficulty::ESD_VeryHard);
	}
}

void UB2UIChapterFrame::OnClickBTN_Hell()
{
	auto DocChapter = Cast<UB2UIDocChapter>(GetDoc());
	if (DocChapter == nullptr)
		return;

	if (CurDifficulty == EStageDifficulty::ESD_Hell)
		return;

	OnSelectBtnStageDifficulty(EStageDifficulty::ESD_Hell);

	if (HasTutorialChangeChapter1Check(EStageDifficulty::ESD_Hell) == false)
	{
		int32 SelectChapter = BladeIIGameImpl::GetStageDataStore().GetMaxReachedActNumber(EStageDifficulty::ESD_Hell);
		BladeIIGameImpl::GetStageDataStore().RequestGetActInfoAndChangeChapter(SelectChapter, EStageDifficulty::ESD_Hell);
	}
}

bool UB2UIChapterFrame::RedDotCondition_ChapterInternal(EStageDifficulty Difficulty)
{
	auto DocChapter = Cast<UB2UIDocChapter>(GetDoc());
	if (DocChapter == nullptr)
		return false;

	if (P_Adventures.Contains(Difficulty) && P_Adventures[Difficulty].IsValid())
	{
		if (P_Adventures[Difficulty]->GetVisibility() == ESlateVisibility::Visible)
			return BladeIIGameImpl::GetStageDataStore().HasAnyRewardToReceive(DocChapter->GetCurChapterNum(), Difficulty);
	}

	return false;
}

bool UB2UIChapterFrame::RedDotCondition_PrevChapter()
{
	bool bHasRewardPrevChapters = false;
	for (int32 Act = CurChapterNum - 1; Act > 0; Act--)
	{
		if (BladeIIGameImpl::GetStageDataStore().HasAnyRewardToReceive(Act, CurDifficulty))
		{
			bHasRewardPrevChapters = true;
			break;
		}
			
	}
	return bHasRewardPrevChapters || BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_NewAct(CurDifficulty, CurChapterNum - 1);
}

bool UB2UIChapterFrame::RedDotCondition_NextChapter()
{
	bool bHasRewardNextChapters = false;

	if (UB2UIDocChapter* DocChapter = UB2UIDocHelper::GetDocChapter())
	{
		const int32 MaxActID = DocChapter->GetChapterMaxNum();

		for (int32 Act = CurChapterNum + 1; Act <= MaxActID; Act++)
		{
			if (BladeIIGameImpl::GetStageDataStore().HasAnyRewardToReceive(Act, CurDifficulty))
			{
				bHasRewardNextChapters = true;
				break;
			}
		}
	}

	return bHasRewardNextChapters || BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_NewAct(CurDifficulty, CurChapterNum + 1);
}
