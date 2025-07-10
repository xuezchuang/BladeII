// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_UnitySkillWakePopup.h"
#include "B2UIManager.h"

#include "Retailer.h"
#include "Event.h"
#include "B2SkillInfo.h"
#include "B2LobbyInventory.h"
#include "B2ItemInfo.h"
#include "B2LobbyGameMode.h"
#include "B2AndroidBackManager.h"
#include "B2LobbyUI_SetupSkillMain.h"
#include "B2PCClassInfoBox.h"
#include "B2UIHeader.h"

#include "BladeIIGameImpl.h"

void UB2LobbyUI_UnitySkillWakePopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_StuffTip);
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2Button, BTN_GetSoul);

	GET_SLOT(UTextBlock, STB_GetSoul);
	GET_SLOT(UTextBlock, STB_Stuff);
	GET_SLOT(UTextBlock, STB_Percent);
	GET_SLOT(UTextBlock, STB_Title);
	GET_SLOT(UTextBlock, STB_MAX);
	GET_SLOT(UTextBlock, STB_Fail);
	
	GET_SLOT(UTextBlock, TB_SkillName);
	GET_SLOT(UTextBlock, TB_SkillDesc);
	GET_SLOT(UTextBlock, TB_HaveCount);
	GET_SLOT(UTextBlock, TB_NeedCount);
	GET_SLOT(UTextBlock, TB_Percent);

	GET_SLOT(UWidgetSwitcher, SW_BottomInfo);

	GET_SLOT(UB2Image, IMG_NeedStuff);

	GET_SLOT(UImage, IMG_SkillIcon);

	const int32 MaxStep = 5;
	UnityInfinityStones.AddDefaulted(MaxStep);
	GET_SLOT_BYNAME(UB2LobbyUI_SkillStepIcon, UnityInfinityStones[0], Step_1);
	GET_SLOT_BYNAME(UB2LobbyUI_SkillStepIcon, UnityInfinityStones[1], Step_2);
	GET_SLOT_BYNAME(UB2LobbyUI_SkillStepIcon, UnityInfinityStones[2], Step_3);
	GET_SLOT_BYNAME(UB2LobbyUI_SkillStepIcon, UnityInfinityStones[3], Step_4);
	GET_SLOT_BYNAME(UB2LobbyUI_SkillStepIcon, UnityInfinityStones[4], Step_5);

	for (int32 i = 0; i < MaxStep; i++)
	{
		if (UnityInfinityStones[i].IsValid())
		{
			UnityInfinityStones[i]->Init();
			UnityInfinityStones[i]->SetTintColor(i);
			UnityInfinityStones[i]->SetButtonDelegateBindLambda([this, i]() { OnClick_SelectSoul(i); });
		}
	}

}

void UB2LobbyUI_UnitySkillWakePopup::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2LobbyUI_UnitySkillWakePopup::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2LobbyUI_UnitySkillWakePopup::OnClickBTN_Close);
	BIND_CLICK_FUNC_TO_BTN(BTN_GetSoul, &UB2LobbyUI_UnitySkillWakePopup::OnClickBTN_GetSoul);
}

void UB2LobbyUI_UnitySkillWakePopup::UpdateStaticText()
{
	if (STB_GetSoul.IsValid())
	{
		STB_GetSoul->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_SoulGet")));
	}
	if (STB_Stuff.IsValid())
	{
		STB_Stuff->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_Material")));
	}
	if (STB_Percent.IsValid())
	{
		STB_Percent->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_SuccessRate")));
	}
	if (STB_Title.IsValid())
	{
		STB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_AwakenMission")));
	}
	if (TB_SkillDesc.IsValid())
	{
		TB_SkillDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_AwakenMissionInfo")));
	}
	if (STB_MAX.IsValid())
	{
		STB_MAX->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_SoulGetComplete")));
	}
	if (STB_Fail.IsValid())
	{
		STB_Fail->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_SoulGetFailed")));
	}
}

void UB2LobbyUI_UnitySkillWakePopup::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	SubscribeEvents();
	BindDelegates();
	UpdateStaticText();
	PageBlock(false);
}

void UB2LobbyUI_UnitySkillWakePopup::Init(EPCClass pcclass)
{
	CurrentPC = pcclass;
	SetUnityAwakenSkillInfo();
	OnClick_SelectSoul(0);
}

void UB2LobbyUI_UnitySkillWakePopup::BeginDestroy()
{
	//Editor 상황에서 문제가 좀 있어서 여기서도 unsubscribe
	if (CachedLobbyGM)
	{
		UnsubscribeEvents();
	}

	Super::BeginDestroy();
}

void UB2LobbyUI_UnitySkillWakePopup::DestroySelf()
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	UnsubscribeEvents();

	Super::DestroySelf();
}

void UB2LobbyUI_UnitySkillWakePopup::SubscribeEvents()
{
	if (bSubscribed == false)
	{
		UnsubscribeEvents();

		Issues.Add(DeliveryAwakenUnitySkillMissionClass<FB2ResponseAwakenUnitySkillMissionPtr>::GetInstance().Subscribe2(
			[this](const FB2ResponseAwakenUnitySkillMissionPtr& data)
		{
			OnReceiveAwakeSkillInfo(data);
		}
		));
		Issues.Add(DeliveryUnityAwakeFailClass<>::GetInstance().Subscribe2(
			[this]()
		{
			PageBlock(false);
		}
		));

		bSubscribed = true;
	}
}

void UB2LobbyUI_UnitySkillWakePopup::UnsubscribeEvents()
{
	Issues.Empty();

	bSubscribed = false;
}

void UB2LobbyUI_UnitySkillWakePopup::OnClickBTN_Close()
{
	if (bIsPageBlock) return;

	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2LobbyUI_UnitySkillWakePopup::OnClickBTN_GetSoul()
{
	PageBlock(true);
	OnStartGetSoulAnimation_BP();
}

void UB2LobbyUI_UnitySkillWakePopup::OnClick_SelectSoul(int32 nStep)
{
	CurrentStep = nStep;

	for (int32 i = 0; i < 5; i++)
	{
		if (UnityInfinityStones[i].IsValid())
		{
			bool bIsSelect = (nStep == i);

			UnityInfinityStones[i]->SetIsSelect(bIsSelect);
		}
	}

	UpdateAwakeSkillInfo();
}

int32 UB2LobbyUI_UnitySkillWakePopup::GetUnitySkillID(EPCClass InPCClass)
{
	// 로컬케릭터데이터에서 가져오기가 애매..
	// 각성스킬은 딱 특정 스킬이어서 그냥 하드코딩이 나은것같음.

	return (PCClassToInt(InPCClass) + 1) * 100 + 3;
}

void UB2LobbyUI_UnitySkillWakePopup::OnEndGetSoulAnimation_IMP()
{
	data_trader::Retailer::GetInstance().RequestAwakenUnitySkillMission(CliToSvrPCClassType(CurrentPC), CurrentStep + 1);
}

void UB2LobbyUI_UnitySkillWakePopup::OnEndGetSoulResultAnimation_IMP()
{
	FUnitySkillAwakenMissionArray* Missions = BladeIIGameImpl::GetLocalCharacterData().GetUnitySkillAwakenMission(CurrentPC);
	bool bAllGetSoul = true;
	for (auto MissionItem : *Missions)
	{
		if (!MissionItem.bCompleted)
			bAllGetSoul = false;
	}

	if(bAllGetSoul)
		AllClearUnityAwakenMissionClass<>::GetInstance().Signal();

	PageBlock(false);
}

void UB2LobbyUI_UnitySkillWakePopup::OnEndGetSoulResultFailAnimation_IMP()
{
	PageBlock(false);
}

void UB2LobbyUI_UnitySkillWakePopup::SetUnityAwakenSkillInfo()
{
	int32 CurrentUnitySkillIndex = GetUnitySkillID(CurrentPC);
	auto* ClassInfoBox = StaticFindPCClassInfoBox();
	auto* AllSkillInfo = ClassInfoBox ? ClassInfoBox->GetAllSkillInfo() : nullptr;

	FSingleSkillInfo* SkillClientData = AllSkillInfo ? const_cast<FSingleSkillInfo*>(AllSkillInfo->GetSingleInfoOfID(CurrentUnitySkillIndex)) : nullptr;

	if (IMG_SkillIcon.IsValid() && SkillClientData)
	{
		IMG_SkillIcon->SetBrushFromMaterial(Cast<UMaterialInstance>(SkillClientData->ButtonIcon_Normal.LoadSynchronous()));
	}

	if (TB_SkillName.IsValid())
	{
		TB_SkillName->SetText(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::SkillName, CurrentUnitySkillIndex)));
	}
}

void UB2LobbyUI_UnitySkillWakePopup::SetAwakeMissionInfo(int32 Step)
{
	bool bAbleAwake = false;

	FB2UnitySkillAwakenMissionPtr MissionItem = GLOBALUNITYSKILLMANAGER.GetUnitySkillAwakenMission(CurrentPC, Step + 1);
	if (MissionItem)
	{
		if (StaticFindItemInfo())
		{
			if (TB_HaveCount.IsValid() && TB_NeedCount.IsValid() && IMG_NeedStuff.IsValid() && BTN_StuffTip.IsValid())
			{
				int32 HaveCount = 0;
				int32 NeedCount = 0;

				UMaterialInterface* ItemMat = nullptr;

				// 필요재화가 골드임
				if (MissionItem->req_gold > 0)
				{
					HaveCount = BladeIIGameImpl::GetClientDataStore().GetGoldAmount();
					NeedCount = MissionItem->req_gold;

					ItemMat = StaticFindItemInfo()->GetItemIcon(FItemRefIDHelper::GetGoodsID_Gold());

					BTN_StuffTip->SetItemInfo(FItemRefIDHelper::GetGoodsID_Gold());
				}
				else
				{
					ItemMat = StaticFindItemInfo()->GetItemIcon(MissionItem->req_item_id_1);

					HaveCount = UB2LobbyInventory::GetSharedConsumableAmountOfType(MissionItem->req_item_id_1);
					NeedCount = MissionItem->req_item_count_1;

					BTN_StuffTip->SetItemInfo(MissionItem->req_item_id_1);
				}

				bAbleAwake = (HaveCount >= NeedCount);

				TB_HaveCount->SetText(FText::AsNumber(HaveCount));
				TB_NeedCount->SetText(FText::AsNumber(NeedCount));

				if (ItemMat)
					IMG_NeedStuff->SetBrushFromMaterial(ItemMat); 

				TB_HaveCount->SetColorAndOpacity(bAbleAwake ? FLinearColor::White : UB2UIManager::GetInstance()->TextColor_Decrease);
			}
		}

		if(BTN_GetSoul.IsValid())	
			BTN_GetSoul->SetIsEnabled(bAbleAwake);
	}
}

void UB2LobbyUI_UnitySkillWakePopup::OnReceiveAwakeSkillInfo(const FB2ResponseAwakenUnitySkillMissionPtr& data)
{
	if (data == nullptr) return;

	data_trader::Retailer::GetInstance().RequestUnitySkillMissions(CliToSvrPCClassType(CurrentPC));

	// 골드 갱신
	if(data->current_money < BladeIIGameImpl::GetClientDataStore().GetGoldAmount())
		BladeIIGameImpl::GetClientDataStore().ReplaceUserDataWithDoc(EDocUserDataType::Gold, data->current_money);

	// 아이템 갱신
	if (data->changed_material_items.Num() > 0)
	{
		BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountDecrease(data->changed_material_items[0]->template_id, -1, data->changed_material_items, data->deleted_item_ids);
	}
	else if (data->deleted_item_ids.Num() > 0)
	{
		BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountDecrease(data->deleted_item_ids[0], -1, data->changed_material_items, data->deleted_item_ids);
	}

	// 미션갱신 
	BladeIIGameImpl::GetLocalCharacterData().SetUnitySkillAwakenMission(CurrentPC, data->awaken_mission->mission_id, data->awaken_mission->completed, data->awaken_mission->try_count);
	
	// UI 갱신	
	UpdateAwakeSkillInfo();

	// 애니메이션 출력
	OnStartGetSoulResultAnimation_BP(data->awaken_mission->completed);

	if (CachedLobbyGM->GetLobbyInventory())
	{
		UB2LobbyUI_SetupSkillMain* InventoryUI = CachedLobbyGM ? Cast<UB2LobbyUI_SetupSkillMain>(CachedLobbyGM->DJLegacy_GetCurrLobbyUIPage()) : nullptr;
		if (InventoryUI)
			InventoryUI->DoMarkRedDotAll_Unity();
	}
}

void UB2LobbyUI_UnitySkillWakePopup::UpdateAwakeSkillInfo()
{
	FUnitySkillAwakenMissionArray* Missions = BladeIIGameImpl::GetLocalCharacterData().GetUnitySkillAwakenMission(CurrentPC);
	FB2UnitySkillAwakenMissionPtr CurrentMission = GLOBALUNITYSKILLMANAGER.GetUnitySkillAwakenMission(CurrentPC, CurrentStep + 1);

	for (auto MissionItem : *Missions)
	{
		int32 Index = MissionItem.nMissionID - 1;

		if (UnityInfinityStones.IsValidIndex(Index))
		{
			UnityInfinityStones[Index]->SetIsGet(MissionItem.bCompleted);
		}

		if (Index == CurrentStep)
		{
			if (TB_Percent.IsValid())
			{
				int32 Rate = (CurrentMission->success_rate + (MissionItem.ntry_count * CurrentMission->success_rate_add)) / 100;
				FString RateText = FString::FromInt(Rate) + TEXT("%");
				TB_Percent->SetText(FText::FromString(RateText));
			}

			if (SW_BottomInfo.IsValid())
			{
				SW_BottomInfo->SetActiveWidgetIndex(MissionItem.bCompleted);
			}
		}
	}
	SetAwakeMissionInfo(CurrentStep);
}

void UB2LobbyUI_UnitySkillWakePopup::PageBlock(bool bIsBlock)
{
	bIsPageBlock = bIsBlock;

	if (GetRootWidget())
		GetRootWidget()->SetVisibility(bIsBlock ? ESlateVisibility::HitTestInvisible : ESlateVisibility::SelfHitTestInvisible);

	if (CachedLobbyGM->GetLobbyInventory())
	{
		UB2LobbyUI_SetupSkillMain* InventoryUI = CachedLobbyGM ? Cast<UB2LobbyUI_SetupSkillMain>(CachedLobbyGM->DJLegacy_GetCurrLobbyUIPage()) : nullptr;
		if(InventoryUI)
			InventoryUI->SetVisibility(bIsBlock ? ESlateVisibility::HitTestInvisible : ESlateVisibility::SelfHitTestInvisible);
	}

	UB2UIHeader* HeaderUI = UB2UIManager::GetInstance()->GetUI<UB2UIHeader>(UIFName::Header);
	if (HeaderUI)
		HeaderUI->SetIsEnabled(!bIsBlock);
}

///////////////////////////////////////////////////////
// Step Icon
///////////////////////////////////////////////////////

void UB2LobbyUI_SkillStepIcon::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UWidgetSwitcher, WS_Switch);
	GET_SLOT(UImage, IMG_Select);
	GET_SLOT(UImage, IMG_On);
	GET_SLOT(UButton, BTN_Select);

	BindDelegate();

}

void UB2LobbyUI_SkillStepIcon::Init()
{
	bIsGet = false;
	bIsSelect = false;

	SetState(false, false);
}

void UB2LobbyUI_SkillStepIcon::BindDelegate()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2LobbyUI_SkillStepIcon::OnClick_Select);
}

void UB2LobbyUI_SkillStepIcon::SetState(bool bSelect, bool bGet)
{
	bIsGet = bGet;
	bIsSelect = bSelect;

	if (WS_Switch.IsValid())
		WS_Switch->SetActiveWidgetIndex(bGet);

	if (IMG_Select.IsValid())
		IMG_Select->SetVisibility(bSelect ?  ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2LobbyUI_SkillStepIcon::GetState(bool& bSelect, bool& bGet)
{
	bSelect = bIsSelect;
	bGet = bIsGet;
}

void UB2LobbyUI_SkillStepIcon::SetIsGet(bool bGet)
{
	bIsGet = bGet;

	if (WS_Switch.IsValid())
		WS_Switch->SetActiveWidgetIndex(bGet);
}

void UB2LobbyUI_SkillStepIcon::SetIsSelect(bool bSelect)
{
	bIsSelect = bSelect;

	if(IMG_Select.IsValid())
		IMG_Select->SetVisibility(bSelect ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2LobbyUI_SkillStepIcon::SetTintColor(int32 nNumber)
{
	if (IMG_On.IsValid() && TintColors.IsValidIndex(nNumber))
	{
		IMG_On->SetColorAndOpacity(TintColors[nNumber]);
	}
}

void UB2LobbyUI_SkillStepIcon::OnClick_Select()
{
	ButtonDelegate.ExecuteIfBound();
}

void UB2LobbyUI_SkillStepIcon::DestroySelf()
{
	Super::DestroySelf();
}