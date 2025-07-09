// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2LobbyUI_RelicMain.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUISwitcher.h"
#include "B2UIManager_Lobby.h"
#include "B2Airport.h"
#include "B2LobbyUI_HeroMgmtCommon.h"
#include "Event.h"
#include "Retailer.h"
#include "B2PCClassInfo.h"
#include "B2UIDocHeroTower.h"
#include "B2UIDocHelper.h"
#include "B2RelicUIP_EnchantInfo.h"
#include "B2UIShortagePopupManager.h"
#include "B2LobbyUI_RelicManagement.h"


#include "BladeIIGameImpl.h"

AB2LobbyGameMode* UB2LobbyUI_RelicMain::TransientLobbyGM;
UB2LobbyUI_RelicMain::UB2LobbyUI_RelicMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedAirport = NULL;
	bRelicMgmtPartAsyncLoadRequested = false;
	CurrentPCClass = EPCClass::EPC_End;
}

void UB2LobbyUI_RelicMain::CacheAssets()
{
	Super::CacheAssets();
	
	CachedAirport = NULL;

	GET_SLOT(UB2LobbyUI_HeroMgmtCommon, UIP_HeroMgmtBase);

	GET_SLOT(UOverlay, OV_RelicMenuEnchant);

	GET_SLOT(UOverlay, OV_CancelEnchantRelic);
	GET_SLOT(UHorizontalBox, HB_EnchantRelic);

	UIP_RelicEnchantInfo.Empty();

	GET_SLOT(UGridPanel, X_RelicItemList);
	GET_SLOT(UTextBlock, TB_AttackIncAmount);
	GET_SLOT(UTextBlock, TB_DefenseIncAmount);
	GET_SLOT(UTextBlock, TB_HealthIncAmount);

	GET_SLOT(UTextBlock, STB_BottomTitle);
	GET_SLOT(UTextBlock, STB_AttackIncLabel);
	GET_SLOT(UTextBlock, STB_DefenseIncLabel);
	GET_SLOT(UTextBlock, STB_HealthIncLabel);

}

void UB2LobbyUI_RelicMain::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	TransientLobbyGM = InGM;

	Super::StartFromLobby(InUIManager, InGM);

	CachedAirport = CachedLobbyGM->GetLobbyAirport();

	CAPTURE_UOBJECT(UB2LobbyUI_RelicMain);
	LobbySetHeroMgmtModePCSelectionTicket = LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbySetHeroMgmtModePCSelection, EPCClass PCClass)
		Capture->OnHeroMgmtPCChanged(PCClass);
		END_CAPTURE_OBJECT()
	);
	
	CreateList();

	if (UIP_HeroMgmtBase.IsValid())
		UIP_HeroMgmtBase->StartFromLobby(InUIManager, InGM); // 이건 LobbyUISwitcher 에서 모르니 직접 해 주어야..

	UpdateStaticText();
	DoMarkRedDot_Relic();
	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_AncientRelic")));
}

void UB2LobbyUI_RelicMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bRelicMgmtPartAsyncLoadRequested && OwnerUIManager)
	{
		// 고대유물 강화 페이지 로딩이 꽤나 걸리므로 (유저가 어물쩡 하는 동안에라도 로딩을 미리 해 놓기 위해) async 로딩 요청을 날린다. 
		// StartFromLobby 에서 하면 flush 가 걸릴 거라 여기서 함.
		// 단, 그 강화 페이지에서는 실제 보스별 데이터(BoardWidget)를 따로 로딩을 하므로 일부만 효과가 있다. 
		OwnerUIManager->ReqAsyncLoadUIPageAsset(ELobbyUIPages::ELUP_RelicManagement);
		bRelicMgmtPartAsyncLoadRequested = true;
	}
}

void UB2LobbyUI_RelicMain::OnHeroMgmtPCChanged(EPCClass InSelectedPCClass)
{
	CurrentPCClass = InSelectedPCClass;
	UpdateList();
	DoMarkRedDot_Relic();
}

void UB2LobbyUI_RelicMain::DestroySelf()
{
	LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Unsubscribe(LobbySetHeroMgmtModePCSelectionTicket);

	if (UIP_HeroMgmtBase.IsValid())
	{
		UIP_HeroMgmtBase->DestroySelf();
	}

	X_RelicItemList->ClearChildren();
	EnchantInfoList.Empty();

	Super::DestroySelf();
}

void UB2LobbyUI_RelicMain::UpdateList()
{  
	int32 LastRelicIndex = 0;
	float TotalAtt = 0;
	float TotalDef = 0;
	float TotalHP = 0;
	
	FAncientRelicArray* TotalRelic = BladeIIGameImpl::GetLocalCharacterData().GetRelicInfo(CurrentPCClass);

	if (!TotalRelic) return;
	if (EnchantInfoList.Num() < 1) return;

	for (auto RelicItem : *TotalRelic) // 현재 오픈된 유물의 정보.
	{
		for (int32 i = 0; i < EnchantInfoList.Num(); i++)
		{
			if (RelicItem.nRelicId == EnchantInfoList[i]->GetCurrentRelicIndex())
			{
				EnchantInfoList[i]->UpdateRelicData(RelicItem,CurrentPCClass);
				LastRelicIndex++;

			}
		}

		TArray<float> GradeOptionValue = GLOBALRELICMANAGER.GenerateGradeOptionValue(RelicItem.nRelicId, RelicItem.nRelicGrade);

		TotalAtt += GradeOptionValue[0];
		TotalDef += GradeOptionValue[1];
		TotalHP += GradeOptionValue[2];
	}

	for (int32 i = LastRelicIndex ; i < EnchantInfoList.Num() ; i++) // 오픈되지 않은 유물은 비활성화.
	{
		EnchantInfoList[i]->ClearRelicData();
	}

	// 캐릭별 전체 증가된 공,방,체
	if(TB_AttackIncAmount.IsValid())
		TB_AttackIncAmount->SetText(FText::Format(FText::FromString(TEXT("+{0}")), FText::AsNumber(static_cast<int32>(TotalAtt))));
	if (TB_AttackIncAmount.IsValid())
		TB_DefenseIncAmount->SetText(FText::Format(FText::FromString(TEXT("+{0}")), FText::AsNumber(static_cast<int32>(TotalDef))));
	if (TB_AttackIncAmount.IsValid())
		TB_HealthIncAmount->SetText(FText::Format(FText::FromString(TEXT("+{0}")), FText::AsNumber(static_cast<int32>(TotalHP))));
}

void UB2LobbyUI_RelicMain::CreateList()
{
	X_RelicItemList->ClearChildren();
	EnchantInfoList.Empty();

	int32 RelicNum = GLOBALRELICMANAGER.GetRelicNum();

	for (int32 i = 0; i < RelicNum; i++) // MasterData에있는 전체 유물 갯수만큼 생성
	{
		UB2RelicUIP_EnchantInfo* UIP_RelicEnchantitem = CreateWidget<UB2RelicUIP_EnchantInfo>(GetWorld(), RelicItemSlotClass);
		if (UIP_RelicEnchantitem)
		{
			UGridSlot* GridWidgetSlot = X_RelicItemList->AddChildToGrid(UIP_RelicEnchantitem);

			if (GridWidgetSlot)
			{
				GridWidgetSlot->SetPadding(5.0);
				GridWidgetSlot->SetRow((int32)i / 3);
				GridWidgetSlot->SetColumn((int32)i % 3);
			}

			int32 nRelicID = i + 1;

			UIP_RelicEnchantitem->Init();
			UIP_RelicEnchantitem->InitRelicOptionData(nRelicID, GLOBALRELICMANAGER.getRelicOpenStageID(nRelicID),CurrentPCClass);
			UIP_RelicEnchantitem->ButtonDelegate.BindUObject(this, &UB2LobbyUI_RelicMain::OnOpenRelicPopup);

			EnchantInfoList.Add(UIP_RelicEnchantitem);
		}
	}
}

void UB2LobbyUI_RelicMain::OnOpenRelicPopup(FAncientRelic relicInfo)
{
	UB2UIDocHeroTower* DocHerotower = UB2UIDocHelper::GetDocHeroTower();
	if (DocHerotower)
	{
		DocHerotower->SetRelic_SelectedPCClass(CurrentPCClass);
		DocHerotower->SetRelic_SelectedRelicInfo(relicInfo);
	}

	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_RelicManagement_EXTRAPAGE);
}

void UB2LobbyUI_RelicMain::DoMarkRedDot_Relic()
{
	if(UIP_HeroMgmtBase.IsValid())
		UIP_HeroMgmtBase->DoMarkRedDot();
	
	for (auto ListWidget : EnchantInfoList)
	{
		ListWidget->DoMarkRedDot();
	}
}

void UB2LobbyUI_RelicMain::UpdateStaticText()
{
	if (STB_BottomTitle.IsValid())
		STB_BottomTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_ANCIENTRELIC, TEXT("Relic_IncreasingAbility")));

	if (STB_AttackIncLabel.IsValid())
		STB_AttackIncLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ENUM_EIPP_Attack")));

	if (STB_DefenseIncLabel.IsValid())
		STB_DefenseIncLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ENUM_EIPP_Defense")));

	if (STB_HealthIncLabel.IsValid())
		STB_HealthIncLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ENUM_EIPP_Health")));
}
