// Fill out your copyright notice in the Description page of Project Settings.

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
		UIP_HeroMgmtBase->StartFromLobby(InUIManager, InGM); // 捞扒 LobbyUISwitcher 俊辑 葛福聪 流立 秦 林绢具..

	UpdateStaticText();
	DoMarkRedDot_Relic();
	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_AncientRelic")));
}

void UB2LobbyUI_RelicMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bRelicMgmtPartAsyncLoadRequested && OwnerUIManager)
	{
		// 绊措蜡拱 碍拳 其捞瘤 肺爹捞 蚕唱 吧府骨肺 (蜡历啊 绢拱履 窍绰 悼救俊扼档 肺爹阑 固府 秦 初扁 困秦) async 肺爹 夸没阑 朝赴促. 
		// StartFromLobby 俊辑 窍搁 flush 啊 吧副 芭扼 咯扁辑 窃.
		// 窜, 弊 碍拳 其捞瘤俊辑绰 角力 焊胶喊 单捞磐(BoardWidget)甫 蝶肺 肺爹阑 窍骨肺 老何父 瓤苞啊 乐促. 
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

	for (auto RelicItem : *TotalRelic) // 泅犁 坷锹等 蜡拱狼 沥焊.
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

	for (int32 i = LastRelicIndex ; i < EnchantInfoList.Num() ; i++) // 坷锹登瘤 臼篮 蜡拱篮 厚劝己拳.
	{
		EnchantInfoList[i]->ClearRelicData();
	}

	// 某腐喊 傈眉 刘啊等 傍,规,眉
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

	for (int32 i = 0; i < RelicNum; i++) // MasterData俊乐绰 傈眉 蜡拱 肮荐父怒 积己
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
