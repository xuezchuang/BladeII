// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUISwitcher.h"
#include "B2UIManager_Lobby.h"
#include "Event.h"
#include "Retailer.h"
#include "B2UIUpgradeRelicBoard.h"
#include "B2UIValueButton.h"
#include "B2UIRewardEventSet.h"
#include "B2ItemInfo.h"
#include "B2UIUpgradeStatNotify.h"
#include "B2UIMsgPopupSimple.h"
#include "B2UIShortagePopupManager.h"
#include "B2LobbyInventory.h"
#include "B2UIHeader.h"
#include "B2UIDocHeroTower.h"
#include "B2UIDocHelper.h"
#include "B2LobbyUI_RelicManagement.h"
#include "B2AndroidBackManager.h"
#include "B2LobbyUI_RelicMain.h"

#include "BladeIIGameImpl.h"

AB2LobbyGameMode* UB2LobbyUI_RelicManagement::TransientLobbyGM;

UB2LobbyUI_RelicManagement::UB2LobbyUI_RelicManagement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedAirport = NULL;
}

void UB2LobbyUI_RelicManagement::CacheAssets()
{
	Super::CacheAssets();
	CachedAirport = NULL;



	GET_SLOT(UB2UIHeader, UIP_CommonHeader)
	if (UIP_CommonHeader.IsValid())
	{
		UIP_CommonHeader->Init();
		UIP_CommonHeader->SetIsInDJLegacyHeroMgmt(true, EHeroMgmtSubMode::EHMSM_RelicManagement_EXTRAPAGE);
		UIP_CommonHeader->BindDocAuto();
	}

	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UProgressBar, PB_EnhancePoint);
	GET_SLOT(UTextBlock, STB_EnhancePoint);
	GET_SLOT(UTextBlock, STB_EntenHero);
	GET_SLOT(UTextBlock, TB_TotalAttText);
	GET_SLOT(UTextBlock, TB_TotalAttAmount);
	GET_SLOT(UTextBlock, TB_TotalDefText);
	GET_SLOT(UTextBlock, TB_TotalDefAmount);
	GET_SLOT(UTextBlock, TB_TotalHPText);
	GET_SLOT(UTextBlock, TB_TotalHPAmount);
	GET_SLOT(UTextBlock, STB_TotalGradeAtt);
	GET_SLOT(UTextBlock, TB_TotalGradeAtt);
	GET_SLOT(UTextBlock, STB_TotalGradeDef);
	GET_SLOT(UTextBlock, TB_TotalGradeDef);
	GET_SLOT(UTextBlock, STB_TotalGradeHP);
	GET_SLOT(UTextBlock, TB_TotalGradeHP);
	GET_SLOT(UTextBlock, TB_NextAttText);
	GET_SLOT(UTextBlock, TB_NextAttAmount);
	GET_SLOT(UTextBlock, TB_NextDefText);
	GET_SLOT(UTextBlock, TB_NextDefAmount);
	GET_SLOT(UTextBlock, TB_NextHPText);
	GET_SLOT(UTextBlock, TB_NextHPAmount);
	GET_SLOT(UOverlay, X_RelicBoard);
	GET_SLOT(UTextBlock, STB_Upgrade);
	GET_SLOT(UB2Button, BTN_AutoUpgrade);
	GET_SLOT(UTextBlock, STB_AutoUpgrade);
	GET_SLOT(UTextBlock, STB_EnhanceEffect);
	GET_SLOT(UTextBlock, STB_Stuff);
	GET_SLOT(UTextBlock, STB_Percent);
	GET_SLOT(UTextBlock, TB_Percent);
	GET_SLOT(UWidgetSwitcher, BottomInfoSwitcher);
	GET_SLOT(UWidgetSwitcher, UpgradePanelSwitcher);
	GET_SLOT(UTextBlock, TB_RelicName);
	GET_SLOT(UTextBlock, TB_RelicLevel);
	GET_SLOT(UTextBlock, TB_UpgradeIndex);
	GET_SLOT(UB2LobbyUI_RelicStarItem, p_NextGrade);

	StarList.SetNumUninitialized(9);
	GET_SLOT_BYNAME(UWidget, StarList[0], Grade_Star1);
	GET_SLOT_BYNAME(UWidget, StarList[1], Grade_Star2);
	GET_SLOT_BYNAME(UWidget, StarList[2], Grade_Star3);
	GET_SLOT_BYNAME(UWidget, StarList[3], Grade_Star4);
	GET_SLOT_BYNAME(UWidget, StarList[4], Grade_Star5);
	GET_SLOT_BYNAME(UWidget, StarList[5], Grade_Star6);
	GET_SLOT_BYNAME(UWidget, StarList[6], Grade_Star7);
	GET_SLOT_BYNAME(UWidget, StarList[7], Grade_Star8);
	GET_SLOT_BYNAME(UWidget, StarList[8], Grade_Star9);

	GET_SLOT(UTextBlock, STB_EnhanceEfttect);

	GET_SLOT(UB2UIValueButton, BTN_Enchance);
	GET_SLOT(UB2UIValueButton, BTN_Promotion);

	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_StuffTip);
	GET_SLOT(UB2Image, IMG_NeedStuff);
	GET_SLOT(UImage, IMG_TouchBlock);
	
	GET_SLOT(UTextBlock, TB_HaveCount);
	GET_SLOT(UTextBlock, TB_NeedCount);

	CachedWidgetInit();
	CachedWidgetBind();
}

void UB2LobbyUI_RelicManagement::CachedWidgetInit()
{
	if (BTN_Enchance.IsValid())
		BTN_Enchance->Init();
	if (BTN_Promotion.IsValid())
		BTN_Promotion->Init();

	if (UIP_CommonHeader.IsValid())
	{
		UIP_CommonHeader->Init();
		UIP_CommonHeader->SetIsInDJLegacyHeroMgmt(true, EHeroMgmtSubMode::EHMSM_RelicManagement_EXTRAPAGE); 
		UIP_CommonHeader->BindDocAuto();
	}

	if (BTN_StuffTip.IsValid())
		BTN_StuffTip->SetItemInfo(FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE);
}

void UB2LobbyUI_RelicManagement::CachedWidgetBind()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2LobbyUI_RelicManagement::OnClickBTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_AutoUpgrade, &UB2LobbyUI_RelicManagement::OnClickBTN_AutoUpgrade);

	if (BTN_Enchance.IsValid())
		BTN_Enchance->ValueButtonDelegateBindLambda([this]() {RequestEnchanceRelic(); });

	if (BTN_Promotion.IsValid())
		BTN_Promotion->ValueButtonDelegateBindLambda([this]() {RequestPromotionRelic(); });
}

void UB2LobbyUI_RelicManagement::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	TransientLobbyGM = InGM;

	Super::StartFromLobby(InUIManager, InGM);

	CachedAirport = CachedLobbyGM->GetLobbyAirport();

	if (UIP_CommonHeader.IsValid())
		UIP_CommonHeader->SetIsInDJLegacyHeroMgmt(true, EHeroMgmtSubMode::EHMSM_RelicManagement_EXTRAPAGE);
	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_AncientRelic")));

	SetStaticText();
	bIsAuto = false;
	bIsAni = false;

	OnCreateRelicBoard();

	B2AndroidBackManager::GetInstance()->AddBackWidget(this);
}

void UB2LobbyUI_RelicManagement::DestroySelf()
{
	Super::DestroySelf();

	if (UIP_CommonHeader.IsValid())
	{
		UIP_CommonHeader->UnbindDoc();
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
		UIP_CommonHeader->DestroySelf(NULL); // 여기선 UIManager 와 상관없이 사용을 하게 되므로 NULL 을 전달.
	}
}

void UB2LobbyUI_RelicManagement::OnClickBTN_Close()
{
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_RelicManagement_EXTRAPAGE);
}

void UB2LobbyUI_RelicManagement::OnClickBTN_AutoUpgrade()
{
	UB2UIDocHeroTower* DocHerotower = UB2UIDocHelper::GetDocHeroTower();
	if (!DocHerotower) return;
	
	int32 MaxLevel = GLOBALRELICMANAGER.GetRelicLevelMax();

	if (bIsAuto || (DocHerotower->GetRelic_SelectedRelicInfo().nRelicLevel == MaxLevel))
	{
		bIsAuto = false;
	}
	else
	{
		bIsAuto = true;

		if (!bIsAni)
			RequestEnchanceRelic();
	}

	CheckButtonEnable();
}

void UB2LobbyUI_RelicManagement::OnCreateRelicBoard()
{
	UB2UIDocHeroTower* DocHerotower = UB2UIDocHelper::GetDocHeroTower();
	if (!DocHerotower) return;

	const int32 BoardWidgetIndex = DocHerotower->GetRelic_SelectedRelicInfo().nRelicId - 1;
	if (!BoardWidgetArray.IsValidIndex(BoardWidgetIndex)) return;
	if (!X_RelicBoard.IsValid()) return;

	X_RelicBoard->ClearChildren();

	// TSoftObjectPtr 의 단순 LoadSynchronous 는 Async flush 를 일으킬 수 있지만 이걸 사용할 타이밍에 async flush 를 걱정할 필요는 없을 것 같으므로 그냥 사용.
	TSubclassOf<UB2UIUpgradeRelicBoard> LoadedWidgetClass = BoardWidgetArray[BoardWidgetIndex].LoadSynchronous();
	UB2UIUpgradeRelicBoard* pUI = LoadedWidgetClass ? CreateWidget<UB2UIUpgradeRelicBoard>(GetWorld(), LoadedWidgetClass) : nullptr;

	if (pUI && X_RelicBoard.IsValid())
	{
		pUI->Init();
		pUI->SetParentClass(this);
		pUI->SetBoardInfo(DocHerotower->GetRelic_SelectedRelicInfo().nRelicLevel, DocHerotower->GetRelic_SelectedRelicInfo().nRelicId, DocHerotower->GetRelic_SelectedRelicInfo().nRelicGrade);
			
		X_RelicBoard->AddChild(pUI);

		auto OverlaySlot = Cast<UOverlaySlot>(pUI->Slot);
		if (OverlaySlot)
		{
			OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		}
	}

	UpdateRelicInfo();
}

UB2UIUpgradeRelicBoard* UB2LobbyUI_RelicManagement::GetCurrentBoardWidget()
{
	if (!X_RelicBoard.IsValid()) return nullptr;
	if (!X_RelicBoard->GetChildAt(0)) return nullptr;

	UB2UIUpgradeRelicBoard* pUI = Cast<UB2UIUpgradeRelicBoard>(X_RelicBoard->GetChildAt(0));

	return pUI;
}

void UB2LobbyUI_RelicManagement::RequestEnchanceRelic()
{
	UB2UIDocHeroTower* DocHerotower = UB2UIDocHelper::GetDocHeroTower();
	if (!DocHerotower) return;

	const FMD_AncientRelicLevelElem* LevelInfo = GLOBALRELICMANAGER.getRelicLevelInfo(DocHerotower->GetRelic_SelectedRelicInfo().nRelicId, DocHerotower->GetRelic_SelectedRelicInfo().nRelicLevel + 1); // 4, 3, 40;

	if (LevelInfo)
	{
		int32 nHaveCount = UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE);

		if (BladeIIGameImpl::GetClientDataStore().GetGoldAmount() < LevelInfo->nEnhance_Gold) // 골드부족
		{
			bIsAuto = false;
			bIsAni = false;
			CheckButtonEnable();
			ShortageMGR->PopupGoToShopForGoldMessage();
			return;
		}
		else if (nHaveCount < LevelInfo->nNeed_HeroPiece)
		{
			bIsAuto = false;
			bIsAni = false;
			CheckButtonEnable();
			ShortageMGR->CheckContentsShortageShortcut(FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE);
			return;
		}
	}

	bIsAni = true;
	CheckButtonEnable();

	data_trader::Retailer::GetInstance().RequestEnhanceRelic(CliToSvrPCClassType(DocHerotower->GetRelic_SelectedPCClass()), DocHerotower->GetRelic_SelectedRelicInfo().nRelicId);
}

void UB2LobbyUI_RelicManagement::RequestPromotionRelic()
{
	UB2UIDocHeroTower* DocHerotower = UB2UIDocHelper::GetDocHeroTower();
	if (!DocHerotower) return;

	const FMD_AncientRelicGradeElem* GradeInfo = GLOBALRELICMANAGER.getRelicGradeInfo(DocHerotower->GetRelic_SelectedRelicInfo().nRelicId, DocHerotower->GetRelic_SelectedRelicInfo().nRelicGrade + 1);
	if (GradeInfo)
	{
		int32 nHaveCount = UB2LobbyInventory::GetSharedConsumableAmountOfType(GradeInfo->nPromotion_BossPieceid);

		if (BladeIIGameImpl::GetClientDataStore().GetGoldAmount() < GradeInfo->nPromotion_Gold)
		{
			bIsAuto = false;
			bIsAni = false;
			CheckButtonEnable();
			ShortageMGR->PopupGoToShopForGoldMessage(); // 골드부족
			return;
		}
		else if (nHaveCount < GradeInfo->nPromotion_BossPiece)
		{
			bIsAuto = false;
			bIsAni = false;
			CheckButtonEnable();
			ShortageMGR->CheckContentsShortageShortcut(GradeInfo->nPromotion_BossPieceid);
			return;
		}
	}

	bIsAni = true;
	CheckButtonEnable();

	data_trader::Retailer::GetInstance().RequestPromotionRelic(CliToSvrPCClassType(DocHerotower->GetRelic_SelectedPCClass()), DocHerotower->GetRelic_SelectedRelicInfo().nRelicId);
}
void UB2LobbyUI_RelicManagement::OnResponseRelicLevelUp(int32 nLevel)
{
	UB2UIDocHeroTower* DocHerotower = UB2UIDocHelper::GetDocHeroTower();
	if (DocHerotower)
	{
		if (DocHerotower->GetRelic_SelectedRelicInfo().nRelicLevel == nLevel)
		{
			PlayFailAnimation_LevelUp();
			return;
		}
		else
		{
			bIsAni = true;
			auto UpdateRelicInfo = DocHerotower->GetRelic_SelectedRelicInfo();
			UpdateRelicInfo.nRelicLevel = nLevel;
			DocHerotower->SetRelic_SelectedRelicInfo(UpdateRelicInfo);

			if (GetCurrentBoardWidget())
				GetCurrentBoardWidget()->OnPlayAnimation_Levelup(DocHerotower->GetRelic_SelectedRelicInfo().nRelicLevel);

			data_trader::Retailer::GetInstance().RequestGetAccountRelic(CliToSvrPCClassType(DocHerotower->GetRelic_SelectedPCClass()));
		}
	}
}

void UB2LobbyUI_RelicManagement::OnResponseRelicGradeUp(int32 nGrade)
{
	UB2UIDocHeroTower* DocHerotower = UB2UIDocHelper::GetDocHeroTower();
	if (DocHerotower)
	{
		if (DocHerotower->GetRelic_SelectedRelicInfo().nRelicGrade == nGrade)
		{
			if (GetCurrentBoardWidget())
				GetCurrentBoardWidget()->OnPlayAnimation_Upgrade(true);
			return;
		}
		else
		{
			auto UpdateRelicInfo = DocHerotower->GetRelic_SelectedRelicInfo();
			UpdateRelicInfo.nRelicLevel = 0;
			UpdateRelicInfo.nRelicGrade = nGrade;
			DocHerotower->SetRelic_SelectedRelicInfo(UpdateRelicInfo);

			bIsAni = true;

			if (GetCurrentBoardWidget())
				GetCurrentBoardWidget()->OnPlayAnimation_Upgrade(false);

			data_trader::Retailer::GetInstance().RequestGetAccountRelic(CliToSvrPCClassType(DocHerotower->GetRelic_SelectedPCClass()));
		}
	}
}

void UB2LobbyUI_RelicManagement::UpdateRelicInfo()
{
	int32 nRelicId = 0;
	int32 nRelicLevel = 0;
	int32 nRelicGrade = 0;

	UB2UIDocHeroTower* DocHerotower = UB2UIDocHelper::GetDocHeroTower();
	if (DocHerotower)
	{
		nRelicId = DocHerotower->GetRelic_SelectedRelicInfo().nRelicId;
		nRelicLevel = DocHerotower->GetRelic_SelectedRelicInfo().nRelicLevel;
		nRelicGrade = DocHerotower->GetRelic_SelectedRelicInfo().nRelicGrade;
	}

	int32 MaxLevel = GLOBALRELICMANAGER.GetRelicLevelMax();
	int32 MaxGrade = GLOBALRELICMANAGER.GetRelicGradeMax();

	bool IsMax = (nRelicLevel == MaxLevel && nRelicGrade == MaxGrade);
	bool IsGradeUp = (nRelicLevel == MaxLevel);

	TArray<float> LevelOptionValue = GLOBALRELICMANAGER.GenerateLevelOptionValue(nRelicId, nRelicGrade, nRelicLevel);
	TArray<float> GradeOptionValue = GLOBALRELICMANAGER.GenerateGradeOptionValue(nRelicId, nRelicGrade);
	
	const FMD_AncientRelicLevelElem* LevelUpInfo = GLOBALRELICMANAGER.getRelicLevelInfo(nRelicId, nRelicLevel + 1 > MaxLevel ? nRelicLevel = MaxLevel : nRelicLevel + 1);
	const FMD_AncientRelicLevelElem* PrevLevelUpInfo = GLOBALRELICMANAGER.getRelicLevelInfo(nRelicId, nRelicLevel);

	const FMD_AncientRelicGradeElem* GradeUpInfo = GLOBALRELICMANAGER.getRelicGradeInfo(nRelicId, nRelicGrade + 1 > MaxGrade ? nRelicGrade = MaxGrade : nRelicGrade + 1);
	const FMD_AncientRelicGradeElem* PrevGradeUpInfo = GLOBALRELICMANAGER.getRelicGradeInfo(nRelicId, nRelicGrade);

	CheckButtonEnable();

	if (STB_AutoUpgrade.IsValid())
		STB_AutoUpgrade->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), bIsAuto ? TEXT("LobbyInvenText_AutoEnhancementCancel") : TEXT("LobbyInvenText_AutoEnhancement")));

	if (BottomInfoSwitcher.IsValid())
		BottomInfoSwitcher->SetActiveWidgetIndex(IsMax);

	if (UpgradePanelSwitcher.IsValid())
		UpgradePanelSwitcher->SetActiveWidgetIndex(IsGradeUp);

	if (STB_AutoUpgrade.IsValid())
		STB_AutoUpgrade->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), bIsAuto ? TEXT("LobbyInvenText_AutoEnhancementCancel") : TEXT("LobbyInvenText_AutoEnhancement")));

	float fPercent = float(nRelicLevel) / float(MaxLevel);
	if (PB_EnhancePoint.IsValid())
		PB_EnhancePoint->SetPercent(fPercent);

	if (STB_EnhancePoint.IsValid())
	{
		FNumberFormattingOptions Options;
		Options.SetMinimumFractionalDigits(1);
		FText PointText = FText::Format(FText::FromString(TEXT("{0}%")), FText::AsNumber(fPercent * 100, &Options));

		STB_EnhancePoint->SetText(PointText);
	}

	if (BTN_Enchance.IsValid())
	{
		bool bOkGold = (BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= LevelUpInfo->nEnhance_Gold);
		BTN_Enchance->SetAmount(FText::AsNumber(LevelUpInfo->nEnhance_Gold), !bOkGold);
	}

	if (BTN_Promotion.IsValid())
	{
		bool bOkGold = (BladeIIGameImpl::GetClientDataStore().GetGoldAmount() >= GradeUpInfo->nPromotion_Gold);
		BTN_Promotion->SetAmount(FText::AsNumber(GradeUpInfo->nPromotion_Gold), !bOkGold);
	}

	if (TB_UpgradeIndex.IsValid())
	{
		TB_UpgradeIndex->SetText(FText::AsNumber(nRelicLevel + 1));
	}

	if (TB_RelicName.IsValid())
		TB_RelicName->SetText(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, "Relic_Type" + FString::FromInt(nRelicId)));

	// 현재 등급표시
	SetVisibleCurrentGrade(nRelicGrade);

	if (TB_RelicLevel.IsValid())
		TB_RelicLevel->SetText(FText::FromString("Lv." + FString::FromInt(nRelicLevel)));

	/// 누적 증가 능력치 세팅
	if (TB_TotalGradeAtt.IsValid())
		TB_TotalGradeAtt->SetText(FText::FromString("+" + FString::FromInt(GradeOptionValue.IsValidIndex(0) ? GradeOptionValue[0] : 0.0f)));
	if (TB_TotalGradeDef.IsValid())
		TB_TotalGradeDef->SetText(FText::FromString("+" + FString::FromInt(GradeOptionValue.IsValidIndex(1) ? GradeOptionValue[1] : 0.0f)));
	if (TB_TotalGradeHP.IsValid())
		TB_TotalGradeHP->SetText(FText::FromString("+" + FString::FromInt(GradeOptionValue.IsValidIndex(2) ? GradeOptionValue[2] : 0.0f)));


	// 총 증가 능력치 텍스트
	if (TB_TotalAttText.IsValid())
		TB_TotalAttText->SetText(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, FString::Printf(TEXT("Option_%d_%d"), nRelicId, 1)));
	if (TB_TotalDefText.IsValid())
		TB_TotalDefText->SetText(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, FString::Printf(TEXT("Option_%d_%d"), nRelicId, 2)));
	if (TB_TotalHPText.IsValid())
		TB_TotalHPText->SetText(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, FString::Printf(TEXT("Option_%d_%d"), nRelicId, 3)));
	if (TB_TotalAttAmount.IsValid())
		TB_TotalAttAmount->SetText(FText::FromString("+" + FString::FromInt(static_cast<int32>(LevelOptionValue.IsValidIndex(0) ? LevelOptionValue[0] : 0.0f))));
	if (TB_TotalDefAmount.IsValid())
		TB_TotalDefAmount->SetText(FText::FromString("+" + FString::FromInt(static_cast<int32>(LevelOptionValue.IsValidIndex(1) ? LevelOptionValue[1] : 0.0f))));
	if (TB_TotalHPAmount.IsValid())
		TB_TotalHPAmount->SetText(FText::FromString("+" + FString::FromInt(static_cast<int32>(LevelOptionValue.IsValidIndex(2) ? LevelOptionValue[2] : 0.0f))));
	
	if (!IsMax)
	{
		// 등급업패널의 다음등급 표시
		if (p_NextGrade.IsValid())
		{
			int32 NextGrade = nRelicGrade + 1;
			p_NextGrade->SetStarGrade(NextGrade);
		}

		// 증가 예상 능력치 텍스트
		if (TB_NextAttText.IsValid())
		{
			if (IsGradeUp)
				TB_NextAttText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ENUM_EIO_Offense_IncAttack")));
			else
				TB_NextAttText->SetText(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, FString::Printf(TEXT("Option_%d_%d"), nRelicId, 1)));
		}
		if (TB_NextDefText.IsValid())
		{
			if (IsGradeUp)
				TB_NextDefText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ENUM_EIPP_Defense")));
			else
				TB_NextDefText->SetText(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, FString::Printf(TEXT("Option_%d_%d"), nRelicId, 2)));
		}
		if (TB_NextHPText.IsValid())
		{
			if (IsGradeUp)
				TB_NextHPText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ENUM_EIPP_Health")));
			else
				TB_NextHPText->SetText(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, FString::Printf(TEXT("Option_%d_%d"), nRelicId, 3)));
		}
		if (TB_NextAttAmount.IsValid())
		{
			int32 GradeUpAmount = GradeUpInfo->nOption_Attack.RawOptionAmount - PrevGradeUpInfo->nOption_Attack.RawOptionAmount;
			int32 LevelUpAmount = LevelUpInfo->nModeOption_Attack.RawOptionAmount - PrevLevelUpInfo->nModeOption_Attack.RawOptionAmount;

			if (IsGradeUp)
				TB_NextAttAmount->SetText(FText::FromString("+" + FString::FromInt(GradeUpAmount)));
			else
				TB_NextAttAmount->SetText(FText::FromString("+" + FString::FromInt(LevelUpAmount)));
		}
		if (TB_NextDefAmount.IsValid())
		{
			int32 GradeUpAmount = GradeUpInfo->nOption_Defence.RawOptionAmount - PrevGradeUpInfo->nOption_Defence.RawOptionAmount;
			int32 LevelUpAmount = LevelUpInfo->nModeOption_Defence.RawOptionAmount - PrevLevelUpInfo->nModeOption_Defence.RawOptionAmount;

			if (IsGradeUp)
				TB_NextDefAmount->SetText(FText::FromString("+" + FString::FromInt(GradeUpAmount)));
			else
				TB_NextDefAmount->SetText(FText::FromString("+" + FString::FromInt(LevelUpAmount)));
		}
		if (TB_NextHPAmount.IsValid())
		{
			int32 GradeUpAmount = GradeUpInfo->nOption_Health.RawOptionAmount - PrevGradeUpInfo->nOption_Health.RawOptionAmount;
			int32 LevelUpAmount = LevelUpInfo->nModeOption_Health.RawOptionAmount - PrevLevelUpInfo->nModeOption_Health.RawOptionAmount;

			if (IsGradeUp)
				TB_NextHPAmount->SetText(FText::FromString("+" + FString::FromInt(GradeUpAmount)));
			else
				TB_NextHPAmount->SetText(FText::FromString("+" + FString::FromInt(LevelUpAmount)));
		}

		if (TB_Percent.IsValid())
		{
			if (IsGradeUp)
				TB_Percent->SetText(FText::FromString(FString::FromInt(GradeUpInfo->nPromotion_Rate / 100) + "%"));
			else
				TB_Percent->SetText(FText::FromString(FString::FromInt(LevelUpInfo->nEnhance_Rate / 100) + "%"));
		}

	
		// 재료아이템 정보 세팅
		int32 nHaveCount = 0;
		int32 nNeedCount = 0;
		int32 ItemRefID = 0;

		if (StaticFindItemInfo())
		{
			if (IsGradeUp)
			{
				ItemRefID = GradeUpInfo->nPromotion_BossPieceid;
				nHaveCount = UB2LobbyInventory::GetSharedConsumableAmountOfType(ItemRefID);
				nNeedCount = GradeUpInfo->nPromotion_BossPiece;
			}
			else
			{
				ItemRefID = FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE;
				nHaveCount = UB2LobbyInventory::GetSharedConsumableAmountOfType(ItemRefID);
				nNeedCount = LevelUpInfo->nNeed_HeroPiece;
			}


			if (IMG_NeedStuff.IsValid())
			{
				UMaterialInterface* ItemMat = StaticFindItemInfo()->GetItemIcon(ItemRefID);

				if (ItemMat)
					IMG_NeedStuff->SetBrushFromMaterial(ItemMat);
			}

			if (BTN_StuffTip.IsValid())
				BTN_StuffTip->SetItemInfo(ItemRefID);
		}

		if (TB_HaveCount.IsValid())
		{
			TB_HaveCount->SetText(FText::AsNumber(nHaveCount));

			if (nHaveCount < nNeedCount)
				TB_HaveCount->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
			else
				TB_HaveCount->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
		}

		if (TB_NeedCount.IsValid())
			TB_NeedCount->SetText(FText::AsNumber(nNeedCount));
	}
}


void UB2LobbyUI_RelicManagement::SetStaticText()
{
	if (STB_TotalGradeAtt.IsValid())
		STB_TotalGradeAtt->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ENUM_EIO_Offense_IncAttack")));
	if (STB_TotalGradeDef.IsValid())
		STB_TotalGradeDef->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ENUM_EIO_Defense_IncDefense")));
	if (STB_TotalGradeHP.IsValid())
		STB_TotalGradeHP->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ENUM_EIO_Health_IncHealth")));
	if (STB_EntenHero.IsValid())
		STB_EntenHero->SetText(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, TEXT("Relic_IncreasingAbility")));
	if (STB_Upgrade.IsValid())
		STB_Upgrade->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Effect")));
	if (STB_EnhanceEffect.IsValid())
		STB_EnhanceEffect->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_UpgradeEffect")));
	if (STB_Stuff.IsValid())
		STB_Stuff->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Material")));
	if (STB_Percent.IsValid())
		STB_Percent->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Successrate")));
	if (STB_AutoUpgrade.IsValid())
		STB_AutoUpgrade->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_AutoEnhancement")));
	if (STB_EnhanceEfttect.IsValid())
		STB_EnhanceEfttect->SetText(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, TEXT("Relic_EnhanceEffect")));

	if (BTN_Enchance.IsValid())
		BTN_Enchance->SetButtonName(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Enhance")));
	if (BTN_Promotion.IsValid())
		BTN_Promotion->SetButtonName(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Upgrade")));
}

void UB2LobbyUI_RelicManagement::PlayFailAnimation_LevelUp()
{
	UB2UIDocHeroTower* DocHerotower = UB2UIDocHelper::GetDocHeroTower();
	if (DocHerotower)
	{
		if (GetCurrentBoardWidget())
			GetCurrentBoardWidget()->OnPlayFailAnimation_Levelup(DocHerotower->GetRelic_SelectedRelicInfo().nRelicLevel + 1);
	}
}

void UB2LobbyUI_RelicManagement::ShowFailPopup(bool bIsLevelUp)
{
	OnPlayFailPopup_IMP(bIsLevelUp);
}

void UB2LobbyUI_RelicManagement::OnEndFailPopupAnimation_BP()
{
	UB2UIDocHeroTower* DocHerotower = UB2UIDocHelper::GetDocHeroTower();
	if (DocHerotower)
	{
		int32 MaxLevel = GLOBALRELICMANAGER.GetRelicLevelMax();
		if (bIsAuto && DocHerotower->GetRelic_SelectedRelicInfo().nRelicLevel != MaxLevel)
		{
			RequestEnchanceRelic();
		}
		else
			bIsAuto = false;

		bIsAni = false;
		UpdateRelicInfo();
	}
}

void UB2LobbyUI_RelicManagement::OnEndUpgradeFail_BP()
{
	ShowFailPopup(false);
}

void UB2LobbyUI_RelicManagement::OnPlaySucessAnimation(bool bIsLevelUp)
{
	UB2UIDocHeroTower* DocHerotower = UB2UIDocHelper::GetDocHeroTower();
	if (DocHerotower)
	{
		if (bIsLevelUp)
			OnPlayLevelUpAnimation_IMP();
		else
			OnPlayUpgradeAnimation_IMP(DocHerotower->GetRelic_SelectedRelicInfo().nRelicGrade);
	}
}

void UB2LobbyUI_RelicManagement::OnEndAnimation_BP(bool bIsLevelUp)
{

	UB2UIDocHeroTower* DocHerotower = UB2UIDocHelper::GetDocHeroTower();
	if (!DocHerotower) return;

	UB2UIUpgradeStatNotify* UpgradeStatNotifyUI = UB2UIManager::GetInstance()->OpenUI<UB2UIUpgradeStatNotify>(UIFName::UpgradeStatNotify);
	if (UpgradeStatNotifyUI != nullptr)
	{
		UpgradeStatNotifyUI->ClearList();
		if (bIsLevelUp)
		{
			const FMD_AncientRelicLevelElem* PrevLevelInfo = GLOBALRELICMANAGER.getRelicLevelInfo(DocHerotower->GetRelic_SelectedRelicInfo().nRelicId, DocHerotower->GetRelic_SelectedRelicInfo().nRelicLevel - 1); 
			const FMD_AncientRelicLevelElem* LevelInfo = GLOBALRELICMANAGER.getRelicLevelInfo(DocHerotower->GetRelic_SelectedRelicInfo().nRelicId, DocHerotower->GetRelic_SelectedRelicInfo().nRelicLevel); 
			if (!LevelInfo) return;
			
			int32 nAttack = LevelInfo->nModeOption_Attack.RawOptionAmount - PrevLevelInfo->nModeOption_Attack.RawOptionAmount;
			int32 nDefense = LevelInfo->nModeOption_Defence.RawOptionAmount - PrevLevelInfo->nModeOption_Defence.RawOptionAmount;
			int32 nHealth = LevelInfo->nModeOption_Health.RawOptionAmount - PrevLevelInfo->nModeOption_Health.RawOptionAmount;
			
			UpgradeStatNotifyUI->AddListItem(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, FString::Printf(TEXT("Option_%d_%d"), DocHerotower->GetRelic_SelectedRelicInfo().nRelicId, 1)), FText::AsNumber(nAttack), true);
			UpgradeStatNotifyUI->AddListItem(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, FString::Printf(TEXT("Option_%d_%d"), DocHerotower->GetRelic_SelectedRelicInfo().nRelicId, 2)), FText::AsNumber(nDefense), true);
			UpgradeStatNotifyUI->AddListItem(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, FString::Printf(TEXT("Option_%d_%d"), DocHerotower->GetRelic_SelectedRelicInfo().nRelicId, 3)), FText::AsNumber(nHealth), true);
		}
		else
		{
			const FMD_AncientRelicGradeElem* GradeInfo = GLOBALRELICMANAGER.getRelicGradeInfo(DocHerotower->GetRelic_SelectedRelicInfo().nRelicId, DocHerotower->GetRelic_SelectedRelicInfo().nRelicGrade);
			const FMD_AncientRelicGradeElem* PrevGradeInfo = GLOBALRELICMANAGER.getRelicGradeInfo(DocHerotower->GetRelic_SelectedRelicInfo().nRelicId, DocHerotower->GetRelic_SelectedRelicInfo().nRelicGrade-1);
			
			if (!GradeInfo || !PrevGradeInfo) return;

			int32 nAttack = GradeInfo->nOption_Attack.RawOptionAmount - PrevGradeInfo->nOption_Attack.RawOptionAmount;
			int32 nDefense = GradeInfo->nOption_Defence.RawOptionAmount - PrevGradeInfo->nOption_Defence.RawOptionAmount;
			int32 nHealth = GradeInfo->nOption_Health.RawOptionAmount - PrevGradeInfo->nOption_Health.RawOptionAmount;

			UpgradeStatNotifyUI->AddListItem(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ENUM_EIO_Offense_IncAttack")), FText::AsNumber(nAttack > 0 ? nAttack : 0), true);
			UpgradeStatNotifyUI->AddListItem(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ENUM_EIPP_Defense")), FText::AsNumber(nDefense > 0 ? nDefense : 0), true);
			UpgradeStatNotifyUI->AddListItem(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ENUM_EIPP_Health")), FText::AsNumber(nHealth > 0 ? nHealth : 0), true);
			UpgradeStatNotifyUI->SetTitleText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_UpgradeSucces")));

			UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(DocHerotower->GetRelic_SelectedPCClass(), true);
		}
	}
	
	if (GetCurrentBoardWidget())
		GetCurrentBoardWidget()->SetBoardInfo(DocHerotower->GetRelic_SelectedRelicInfo().nRelicLevel, DocHerotower->GetRelic_SelectedRelicInfo().nRelicId, DocHerotower->GetRelic_SelectedRelicInfo().nRelicGrade);

	int32 MaxLevel = GLOBALRELICMANAGER.GetRelicLevelMax();

	if (bIsAuto && DocHerotower->GetRelic_SelectedRelicInfo().nRelicLevel != MaxLevel)
		RequestEnchanceRelic();
	else
		bIsAuto = false;

	bIsAni = false;;
	UpdateRelicInfo();
}

void UB2LobbyUI_RelicManagement::CheckButtonEnable()
{
	bool bIsAble = (!bIsAni && !bIsAuto);
	if (BTN_Enchance.IsValid())
		BTN_Enchance->SetIsEnabled(bIsAble);
	if (BTN_Promotion.IsValid())
		BTN_Promotion->SetIsEnabled(bIsAble);
	if (BTN_Close.IsValid())
		BTN_Close->SetIsEnabled(bIsAble);
	
	if (bIsAni == true && bIsAuto == false)
	{//강화시 백버튼 막음
		SetClose(false);
	}
	else
	{
		SetClose(true);
	}

	SetEnablePage(bIsAble);
}

void UB2LobbyUI_RelicManagement::SetVisibleCurrentGrade(int32 nGrade)
{

	for (int32 i = 0; i < StarList.Num(); i++)
	{
		if (nGrade > 6)
		{
			if (i == nGrade - 1)
				StarList[i]->SetVisibility(ESlateVisibility::Visible);
			else
				StarList[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			if (i <= nGrade - 1)
				StarList[i]->SetVisibility(ESlateVisibility::Visible);
			else
				StarList[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2LobbyUI_RelicManagement::SetEnablePage(bool bEnable)
{
	if(IMG_TouchBlock.IsValid())
		IMG_TouchBlock->SetVisibility(bEnable ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UB2LobbyUI_RelicManagement::CloseWidgetDelegate()
{
	if (bIsAuto == true)
	{
		OnClickBTN_AutoUpgrade();
	} 
	else if(UIP_CommonHeader.IsValid())
	{
		UIP_CommonHeader->CloseWidgetDelegate();
	}
}