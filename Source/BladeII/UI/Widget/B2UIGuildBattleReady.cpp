// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIGuildBattleReady.h"
#include "B2UIManager.h"
#include "B2UIGuildFormation.h"
#include "B2UITMFormationSelectButton.h"
#include "B2UITMCharacterSelectItem.h"

#include "Retailer.h"
#include "B2UIDocHelper.h"
#include "B2UIFormationPopup.h"
#include "B2GuildBuffInfo.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2MessageInfoCommonStruct.h"
#include "B2CombatStatEvaluator.h"



void UB2UIGuildBattleReady::CacheAssets()
{
	Super::CacheAssets();
	

	GET_SLOT(UB2Button, BTN_Upgrade);

	GET_SLOT(UB2UIGuildFormation, UIP_GuildFormation1);


	FGuildEntrySlotDelegate SlotDelegate, SwapDeleate;
	SlotDelegate.BindUObject(this, &UB2UIGuildBattleReady::SelectSlotUpdata);
	SwapDeleate.BindUObject(this, &UB2UIGuildBattleReady::SwapSlotUpdate);
	

	if (UIP_GuildFormation1.IsValid())
	{
		UIP_GuildFormation1->Init();
		UIP_GuildFormation1->SetTeamNumber(1);	// 1팀
		UIP_GuildFormation1->SetSlotDelegate(SlotDelegate);
		UIP_GuildFormation1->SetSwapDelegate(SwapDeleate);
		
	}

	GET_SLOT(UB2UIGuildFormation, UIP_GuildFormation2);
	

	if (UIP_GuildFormation2.IsValid())
	{
		UIP_GuildFormation2->Init();
		UIP_GuildFormation2->SetTeamNumber(2);	// 2팀
		UIP_GuildFormation2->SetSlotDelegate(SlotDelegate);
		UIP_GuildFormation2->SetSwapDelegate(SwapDeleate);
	}


	GET_SLOT(UHorizontalBox, X_MercenaryList);
	GET_SLOT(UTextBlock, TB_HeroExplanation);
}

void UB2UIGuildBattleReady::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Upgrade, &UB2UIGuildBattleReady::OnClickBTN_Upgrade);
}

void UB2UIGuildBattleReady::Init()
{
	Super::Init();

	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	if (!GuildDoc)
		return;
	EntryInfo.Empty();
	TArray<b2network::B2GuildBattleCharacterSlotPtr> CharacterEntry = GuildDoc->GetGuildBattleEntry();

	for (auto El : CharacterEntry)
	{
		FGuildBattleEntryInfo LocalEntryInfo(El);
		auto NPCData = BladeIIGameImpl::GetGuildDataStore().GetGuildMercenaryData(LocalEntryInfo.Entry_code);
		if (NPCData)
		{
			LocalEntryInfo.Level = NPCData->level;
			El->level = NPCData->level;

		}

		EntryInfo.Add(LocalEntryInfo);
	}


	if (TB_HeroExplanation.IsValid())
		TB_HeroExplanation->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleReady_SaveStat")));

	initializationSlot();

}

void UB2UIGuildBattleReady::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}


void UB2UIGuildBattleReady::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);

	UB2UIDocGuild* GuildDoc = UB2UIDocHelper::GetDocGuild();

	/*DeliverySetGuildBattleEntryTiket = DeliverySetGuildBattleEntryClass<FB2ResponseSetGuildBattlePtr>::GetInstance().Subscribe([this, GuildDoc](const FB2ResponseSetGuildBattlePtr& InBattleEntry)
	{
		GuildDoc->SetGuildBattleEntry(InBattleEntry->entry->slots);
	});*/
}

void UB2UIGuildBattleReady::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose(RightNow);

	// jwyim GuildFormation
	//DeliverySetGuildBattleEntryClass<FB2ResponseSetGuildBattlePtr>::GetInstance().Unsubscribe(DeliverySetGuildBattleEntryTiket);
}

void UB2UIGuildBattleReady::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);
	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("Guild_BattleReady"));
}


void UB2UIGuildBattleReady::initializationSlot()
{
	TMap<int32, int32> LocalMergenaryInfo;					//길드용병 개수세기위한 Map

	auto GuildDoc = UB2UIDocHelper::GetDocGuild();

	int32 CombatPower1Team = 0;
	int32 CombatPower2Team = 0;

	for (FGuildBattleEntryInfo El : EntryInfo)
	{

		if (El.Slot_num > UB2UIGuildFormation::GuildFormationOne)
		{
			UIP_GuildFormation2->InitSlotCharacter(El.Slot_num - UB2UIGuildFormation::GuildFormationOne, El.Entry_code, El.Level);
			UIP_GuildFormation2->SetSelectType(El.Slot_num - UB2UIGuildFormation::GuildFormationOne, EGuildEntrySelectType::Normal);

			if (UB2UIGuildFormation::GuildMercenaryBaseID > El.Entry_code)
			{
				CombatPower2Team += GetEntryCombat(El.Entry_code);
				//CombatPower2Team += CombatStatEval::GetPCCombatPower(SvrToCliPCClassType(El.Entry_code));
			}

		}
		else
		{
			UIP_GuildFormation1->InitSlotCharacter(El.Slot_num, El.Entry_code, El.Level);
			UIP_GuildFormation1->SetSelectType(El.Slot_num, EGuildEntrySelectType::Normal);

			if (UB2UIGuildFormation::GuildMercenaryBaseID > El.Entry_code)
			{
				CombatPower1Team += GetEntryCombat(El.Entry_code);
				//CombatPower1Team += CombatStatEval::GetPCCombatPower(SvrToCliPCClassType(El.Entry_code));
			}
		}

		if (El.Entry_code > UB2UIGuildFormation::GuildMercenaryBaseID)
		{
			if (LocalMergenaryInfo.Contains(El.Entry_code))
				LocalMergenaryInfo[El.Entry_code] += 1;
			else
				LocalMergenaryInfo.Add(El.Entry_code, 1);
		}
	}


	UIP_GuildFormation1->SetCombat(CombatPower1Team);
	UIP_GuildFormation2->SetCombat(CombatPower2Team);

	auto MercenaryMasterDataMap = BladeIIGameImpl::GetGuildDataStore().GetGuildMercenaryMasterData().GetMercenariesArr();
	auto MercenaryDataMap = BladeIIGameImpl::GetGuildDataStore().GetGuildAllMercenaryData();


	//역순으로 한번 정리 정렬
	MercenaryDataMap.KeySort([](int32 El1 ,  int32 El2)->bool {
		if (El1 < El2)return false; else return true;
	});

	if(!X_MercenaryList.IsValid())
		return;


	X_MercenaryList->ClearChildren();
	MercenaryListSlotArr.Empty();

	for (auto El : MercenaryDataMap)
	{

		auto MercenaryMasterData = BladeIIGameImpl::GetGuildDataStore().GetGuildMercenaryMasterData().GetMercenaries(El.Value->id);

		check(MercenaryMasterData)

		if(MercenaryMasterData->is_usable == 0)
			continue;

		TSubclassOf<UB2UIGuildFormationMercenarySlot> MercanarySlot = UIP_MercenarySlot.LoadSynchronous();
		UB2UIGuildFormationMercenarySlot* UIPMercanarySlot = CreateWidget<UB2UIGuildFormationMercenarySlot>(this->GetOwningPlayer(), MercanarySlot);
		check(UIPMercanarySlot)

		int32 MinusNumber = 0;

		if (LocalMergenaryInfo.Contains(El.Value->id))
			MinusNumber = LocalMergenaryInfo[El.Value->id];
		
		X_MercenaryList->AddChild(UIPMercanarySlot);

		MercenaryListSlotArr.Add(UIPMercanarySlot);

		FGuildEntrySlotDelegate SlotDelegate, SwapDeleate;
		SlotDelegate.BindUObject(this, &UB2UIGuildBattleReady::SelectSlotUpdata);
		SwapDeleate.BindUObject(this, &UB2UIGuildBattleReady::SwapSlotUpdate);

		UIPMercanarySlot->Init();
		UIPMercanarySlot->SetSlotCharacter(El.Value->id);
		UIPMercanarySlot->SetTextNumber(MercenaryMasterData->max_use_count - MinusNumber);
		UIPMercanarySlot->SetSelectType(EGuildEntrySelectType::Normal);
		UIPMercanarySlot->SetSlotDelegate(SlotDelegate);
		UIPMercanarySlot->SetSwapDelegate(SwapDeleate);

	}
}

void UB2UIGuildBattleReady::SwapSlotUpdate(EGuildEntrySlotType InSlotType, EGuildEntrySelectType InSelectType, int32 InEntryCode, int32 InSlotNumber)
{
	if (InSelectType != EGuildEntrySelectType::SelectPossible)
	{
		initializationSlot();
		return;
	}


	//용병리스트에서 추가하는게 아니고 서로 진형끼리 교환할때
	if (InSlotType == EGuildEntrySlotType::FormationSlot && SelectGuildSlotInfo.SlotType == EGuildEntrySlotType::FormationSlot)
	{

		check(EntryInfo[InSlotNumber - 1].Slot_num == InSlotNumber)
		check(EntryInfo[SelectGuildSlotInfo.SlotNumber - 1].Slot_num == SelectGuildSlotInfo.SlotNumber)

		FGuildBattleEntryInfo TempEntryInfo = EntryInfo[InSlotNumber - 1];

		EntryInfo[InSlotNumber - 1].Entry_code = EntryInfo[SelectGuildSlotInfo.SlotNumber - 1].Entry_code;
		EntryInfo[InSlotNumber - 1].Entry_type = EntryInfo[SelectGuildSlotInfo.SlotNumber - 1].Entry_type;
		EntryInfo[InSlotNumber - 1].Level = EntryInfo[SelectGuildSlotInfo.SlotNumber - 1].Level;

		EntryInfo[SelectGuildSlotInfo.SlotNumber - 1].Entry_code = TempEntryInfo.Entry_code;
		EntryInfo[SelectGuildSlotInfo.SlotNumber - 1].Entry_type = TempEntryInfo.Entry_type;
		EntryInfo[SelectGuildSlotInfo.SlotNumber - 1].Level = TempEntryInfo.Level;
	}
	else
	{

		if (SelectGuildSlotInfo.SlotType == EGuildEntrySlotType::MercenaryList)
		{
			auto MercenaryData = BladeIIGameImpl::GetGuildDataStore().GetGuildMercenaryData(SelectGuildSlotInfo.EntryCode);
			check(MercenaryData)
			EntryInfo[InSlotNumber - 1].Entry_code = SelectGuildSlotInfo.EntryCode;
			EntryInfo[InSlotNumber - 1].Level = MercenaryData->level;
			
		}
		else
		{
			auto MercenaryData = BladeIIGameImpl::GetGuildDataStore().GetGuildMercenaryData(InEntryCode);
			check(MercenaryData)
			EntryInfo[SelectGuildSlotInfo.SlotNumber - 1].Entry_code = InEntryCode;
			EntryInfo[SelectGuildSlotInfo.SlotNumber - 1].Level = MercenaryData->level;

		}
	}



	initializationSlot();
}

void UB2UIGuildBattleReady::SelectSlotUpdata(EGuildEntrySlotType InSlotType, EGuildEntrySelectType InSelectType, int32 InEntryCode, int32 InSlotNumber)
{
	FGuildSlotInfo LocalSlotInfo(InSlotType, InSelectType, InEntryCode, InSlotNumber);

	SelectGuildSlotInfo = LocalSlotInfo;


	if (InSelectType == EGuildEntrySelectType::Select)				//새로운선택을 했을경우(화살표 표시가 떠야하는상황)
	{
		for (int32 i = 1; i <= UB2UIGuildFormation::GuildFormationMax; ++i)
		{

			bool isSameTeam = (i <= UB2UIGuildFormation::GuildFormationOne) == (InSlotNumber <= UB2UIGuildFormation::GuildFormationOne) ? true : false;		//현재포문하고 선택된 슬롯하고 같은팀인지 아닌지

			if (InSlotNumber == 0)				//slotnumber가 0이면 용병리스트에서 오는거기때문에 무조건 다른팀으로 인식하게해주자
				isSameTeam = false;
			

			UB2UIGuildFormation * Formation = i <= UB2UIGuildFormation::GuildFormationOne ? UIP_GuildFormation1.Get() : UIP_GuildFormation2.Get();
			int32 iSlotNumber = i <= UB2UIGuildFormation::GuildFormationOne ? i : i - 4;

			bool IsEntryMercenary = InEntryCode > UB2UIGuildFormation::GuildMercenaryBaseID;
			bool IsSlotMercenary = Formation->GetSlotCode(iSlotNumber) > UB2UIGuildFormation::GuildMercenaryBaseID;

			if(i == InSlotNumber)
				Formation->SetSelectType(iSlotNumber, EGuildEntrySelectType::Select);
			else if (IsEntryMercenary == IsSlotMercenary)										//같은 용병이거나 캐릭터면 스왑가능
			{
				Formation->SetSelectType(iSlotNumber, EGuildEntrySelectType::SelectPossible);
			}
			else if (isSameTeam)																//같은팀일경우도 스왑가능
			{
				Formation->SetSelectType(iSlotNumber, EGuildEntrySelectType::SelectPossible);
			}
			else if (InSlotType == EGuildEntrySlotType::MercenaryList && IsSlotMercenary)		//현재 돌고있는 슬롯과 선택된게 용병 리스트라면 스왑가능
			{
				Formation->SetSelectType(iSlotNumber, EGuildEntrySelectType::SelectPossible);
			}
			else
				Formation->SetSelectType(iSlotNumber, EGuildEntrySelectType::SelectImpossible);			//나머지는 다 안됨
		}

		//용병리스트부분 업데이트
	
		for (auto El : MercenaryListSlotArr)
		{
			if (El->GetGuildEntryCode() == InEntryCode && InSlotType == EGuildEntrySlotType::MercenaryList)	//용병리스트에서 오고 용병코드가 같다면
				El->SetSelectType(EGuildEntrySelectType::Select);
			else if (InSlotType == EGuildEntrySlotType::FormationSlot && InEntryCode > UB2UIGuildFormation::GuildMercenaryBaseID)
				El->SetSelectType(EGuildEntrySelectType::SelectPossible);
			else
				El->SetSelectType(EGuildEntrySelectType::SelectImpossible);

		}
	}
}

int32 UB2UIGuildBattleReady::GetEntryCombat(int32 EntryCode)
{
	auto GuildDoc = UB2UIDocHelper::GetDocGuild();


	if (!GuildDoc)
		return 0;

	for (auto El : GuildDoc->GetCharacterPower())
	{
		
		if (El->character_type == EntryCode)
			return El->total_power;
	}

	return 0;
}

void UB2UIGuildBattleReady::OnClickBTN_Upgrade()
{
	
	data_trader::Retailer::GetInstance().RequestSetGuildBattleEntry(
		EntryInfo[0].Entry_code,
		EntryInfo[1].Entry_code,
		EntryInfo[2].Entry_code,
		EntryInfo[3].Entry_code,
		EntryInfo[4].Entry_code,
		EntryInfo[5].Entry_code,
		EntryInfo[6].Entry_code,
		EntryInfo[7].Entry_code
		);
	
}



bool UB2UIGuildBattleReady::OnBackButtonProc()
{
	/*bool Check = CheckChangeFormation();
	if (Check == true)
	{*/
		UB2UIManager::GetInstance()->OpenMsgPopup
		(
			EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_NotSaveChanges")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::YesOrNo,
			FMsgPopupOnClick::CreateLambda([this]() {
			LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::GuildMain); }),
			0
		);
	//}
	return true;
}

bool UB2UIGuildBattleReady::CheckChangeFormation()
{
	UB2UIDocGuild* GuildDoc = UB2UIDocHelper::GetDocGuild();
	bool Change = false;
	auto EntryArr = GuildDoc->GetGuildBattleEntry();


	for (int32 i = 0; i < UB2UIGuildFormation::GuildFormationMax; ++i)
	{
		if(EntryArr[i]->slot_num == EntryInfo[i].Slot_num)
		{
			if (EntryArr[i]->entry_code != EntryInfo[i].Entry_code)
				return true;

			if (EntryArr[i]->entry_type != EntryInfo[i].Entry_type)
				return true;
		}
	}

	return Change;
}
