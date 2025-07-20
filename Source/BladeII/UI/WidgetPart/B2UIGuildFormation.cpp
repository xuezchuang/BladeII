// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIGuildFormation.h"
#include "B2UIManager.h"

#include "B2UIDocGuild.h"
#include "B2UIDocHelper.h"
#include "BladeIIUtil.h"
#include "B2GuildNPCInfo.h"
#include "BladeIIGameImpl.h"

/*
#define GuildFormationOne 4
#define GuildFormationMax 8

#define GuildMercenaryBaseID 10000*/

void UB2UIGuildFormation::CacheAssets()
{
	Super::CacheAssets();

	CharacterSlotArr.SetNum(4);

	GET_SLOT_BYNAME(UB2UIGuildFormationSlot, CharacterSlotArr[0], UIP_CharacterSlot1)
	GET_SLOT_BYNAME(UB2UIGuildFormationSlot, CharacterSlotArr[1], UIP_CharacterSlot2)
	GET_SLOT_BYNAME(UB2UIGuildFormationSlot, CharacterSlotArr[2], UIP_CharacterSlot3)
	GET_SLOT_BYNAME(UB2UIGuildFormationSlot, CharacterSlotArr[3], UIP_CharacterSlot4)

	for (auto El : CharacterSlotArr)
	{
		El->Init();
	}

	GET_SLOT(UTextBlock, TB_Team);

	
	GET_SLOT(UTextBlock ,TB_Combat);
	
}

void UB2UIGuildFormation::BindDelegates()
{
	Super::BindDelegates();

}

void UB2UIGuildFormation::Init()
{
	Super::Init();
}

void UB2UIGuildFormation::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}




void UB2UIGuildFormation::InitSlotCharacter(int32 SlotNumber, int32 InEntryCode , int32 InLv)
{
	int32 SlotNumberRevision = TeamNumber == 2 ? 4 : 0;			//浇吩锅龋 焊沥困秦辑 

	if (CharacterSlotArr.Num() >= SlotNumber && SlotNumber - 1 >= 0)
	{
		CharacterSlotArr[SlotNumber-1]->SetSlotCharacter(SlotNumber + SlotNumberRevision, InEntryCode, InLv);
	}
}

void UB2UIGuildFormation::SetSelectType(int32 InSlotNumber, EGuildEntrySelectType InSelectType)
{


	if (CharacterSlotArr.Num() > InSlotNumber - 1 && InSlotNumber - 1 >= 0)
	{
		CharacterSlotArr[InSlotNumber - 1]->SetSelectType(InSelectType);
	}
}

void UB2UIGuildFormation::SetTeamNumber(int32 InTeamNumber)
{
	TeamNumber = InTeamNumber;

	FString TemaText = InTeamNumber == 1 ? TEXT("Guild_Team_01") : TEXT("Guild_Team_02");

	if (TB_Team.IsValid())
		TB_Team->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TemaText));

}

void UB2UIGuildFormation::SetSlotDelegate(FGuildEntrySlotDelegate InSlotDeleagate)
{
	for (auto El : CharacterSlotArr)
	{
		El->SetSlotDelegate(InSlotDeleagate);
	}
}

void UB2UIGuildFormation::SetSwapDelegate(FGuildEntrySlotDelegate InSlotDeleagate)
{
	for (auto El : CharacterSlotArr)
	{
		El->SetSwapDelegate(InSlotDeleagate);
	}
}

void UB2UIGuildFormation::SetCombat(int32 InCombat)
{
	if (TB_Combat.IsValid())
		TB_Combat->SetText(FText::AsNumber(InCombat));
}

int32 UB2UIGuildFormation::GetSlotCode(int32 InSlotNumber)
{
	if (CharacterSlotArr.Num() > InSlotNumber - 1)
		return CharacterSlotArr[InSlotNumber - 1]->GetSlotCode();

	return 0;
}

void UB2UIGuildFormationSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_Character)
	GET_SLOT(UB2Button, BTN_Select)
	GET_SLOT(UTextBlock, TB_Lv)

	GET_SLOT(UPanelWidget, P_Select);
	GET_SLOT(UPanelWidget, P_SelectPossible);
}

void UB2UIGuildFormationSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIGuildFormationSlot::OnClickSlot);
}

void UB2UIGuildFormationSlot::Init()
{
	Super::Init();
}

void UB2UIGuildFormationSlot::SetSlotCharacter(int32 InSlotNumber,int32 InEntryCode, int32 InLv)
{
	InEntryCode >= 10000 ? GuildEntryType = b2network::B2GuildBattleEntryType::MERCENARY : GuildEntryType = b2network::B2GuildBattleEntryType::CHARACTER;
	SlotNumber = InSlotNumber;
	
	GuildEntryCode = InEntryCode;

	LineHidden();
	if (GuildEntryType == b2network::B2GuildBattleEntryType::CHARACTER)
	{
		IMG_Character->SetBrushFromMaterial(CharacterIMG[PCClassToInt(SvrToCliPCClassType(InEntryCode))]);
		LineSetting(InEntryCode);
	}
	else if (GuildEntryType == b2network::B2GuildBattleEntryType::MERCENARY)
	{
		auto NPCInfo = StaticFindGuildNPCInfo();

		check(NPCInfo)

		IMG_Character->SetBrushFromMaterial(NPCInfo->GetGuildNPCInfoOfID(InEntryCode)->NPCSlotCircleIcon.LoadSynchronous());
	}

	if (TB_Lv.IsValid())
		TB_Lv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(InLv)));
}

void UB2UIGuildFormationSlot::SetTextTeamNumber(int32 InTeamNumber, int32 InSlotNumber)
{

}

void UB2UIGuildFormationSlot::SetSelectType(EGuildEntrySelectType InSelectType)
{
	SelectType = InSelectType;

	IMG_Character->SetColorAndOpacity(CharacterIMGColor[0]);
	TB_Lv->SetColorAndOpacity(CharacterIMGColor[0]);

	P_Select->SetVisibility(ESlateVisibility::Hidden);
	P_SelectPossible->SetVisibility(ESlateVisibility::Hidden);

	if (SelectType == EGuildEntrySelectType::Normal)
	{

	}
	else if (SelectType == EGuildEntrySelectType::Select)
	{
		P_Select->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if (SelectType == EGuildEntrySelectType::SelectImpossible)
	{
		IMG_Character->SetColorAndOpacity(CharacterIMGColor[1]);
		TB_Lv->SetColorAndOpacity(CharacterIMGColor[1]);
	}
	else if (SelectType == EGuildEntrySelectType::SelectPossible)
	{
		SetSelectAnimation();

		P_SelectPossible->SetVisibility(ESlateVisibility::HitTestInvisible);
	}


}

void UB2UIGuildFormationSlot::OnClickSlot()
{
	if (SelectType == EGuildEntrySelectType::Normal)			//努腐沁阑锭 老馆惑怕搁 努腐茄 惑怕肺 官层霖促
	{
		SelectType = EGuildEntrySelectType::Select;
		SelectPlaySound();
		SlotDelegate.ExecuteIfBound(EGuildEntrySlotType::FormationSlot, SelectType, GuildEntryCode, SlotNumber);					//浇吩甸 盎脚秦霖促
		return;
	}
	else if(SelectType == EGuildEntrySelectType::SelectImpossible || SelectType == EGuildEntrySelectType::Select)	//努腐捞 阂啊瓷茄惑怕唱, 捞固努腐茄惑怕俊辑 度鞍篮 浇吩俊 甸绢坷搁 促矫 Normal惑怕肺 官层霖促
		SelectType = EGuildEntrySelectType::Normal;
	else if (SelectType == EGuildEntrySelectType::SelectPossible)
	{
		ChangePlaySound();
	
	}
	
	//浇吩 官层林扁
	SwapDelegate.ExecuteIfBound(EGuildEntrySlotType::FormationSlot, SelectType, GuildEntryCode, SlotNumber);
}

void UB2UIGuildFormationSlot::SetSlotDelegate(FGuildEntrySlotDelegate InSlotDeleagate)
{
	SlotDelegate = InSlotDeleagate;
}

void UB2UIGuildFormationSlot::SetSwapDelegate(FGuildEntrySlotDelegate InSlotDeleagate)
{
	SwapDelegate = InSlotDeleagate;
}

void UB2UIGuildFormationMercenarySlot::CacheAssets()
{
	Super::CacheAssets();


	GET_SLOT(UB2Button, BTN_Select);
	GET_SLOT(UImage, IMG_Character);
	GET_SLOT(UTextBlock, TB_Lv);
	GET_SLOT(UTextBlock, TB_Number);

	GET_SLOT(UPanelWidget, P_Select);
	GET_SLOT(UPanelWidget, P_SelectPossible);

}

void UB2UIGuildFormationMercenarySlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIGuildFormationMercenarySlot::OnClickSlot);
}

void UB2UIGuildFormationMercenarySlot::Init()
{
	Super::Init();

}

void UB2UIGuildFormationMercenarySlot::SetSlotCharacter(int32 InEntryCode)
{


	auto NPCInfo = StaticFindGuildNPCInfo();

	GuildEntryCode = InEntryCode;

	check(NPCInfo)

	IMG_Character->SetBrushFromMaterial(NPCInfo->GetGuildNPCInfoOfID(InEntryCode)->NPCSlotIcon.LoadSynchronous());

	auto MercenaryData = BladeIIGameImpl::GetGuildDataStore().GetGuildMercenaryData(InEntryCode);

	check(MercenaryData)

	if (TB_Lv.IsValid())
		TB_Lv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(MercenaryData->level)));
	

}

void UB2UIGuildFormationMercenarySlot::SetTextNumber(int32 InNumber)
{
	MercenaryCount = InNumber;

	if (TB_Number.IsValid())
		TB_Number->SetText(FText::FromString(FString::Printf(TEXT("x%d"), InNumber)));

	if(BTN_Select.IsValid())
		BTN_Select->SetColorAndOpacity(InNumber > 0 ? CharacterIMGColor[0] : CharacterIMGColor[1]);
	
}

void UB2UIGuildFormationMercenarySlot::SetSlotDelegate(FGuildEntrySlotDelegate InSlotDeleagate)
{
	SlotDelegate = InSlotDeleagate;
}

void UB2UIGuildFormationMercenarySlot::SetSwapDelegate(FGuildEntrySlotDelegate InSlotDeleagate)
{
	SwapDelegate = InSlotDeleagate;
}

void UB2UIGuildFormationMercenarySlot::SetSelectType(EGuildEntrySelectType InSelectType)
{
	SelectType = InSelectType;

	BTN_Select->SetColorAndOpacity(CharacterIMGColor[0]);

	P_Select->SetVisibility(ESlateVisibility::Hidden);
	P_SelectPossible->SetVisibility(ESlateVisibility::Hidden);

	if (MercenaryCount == 0)
	{
		BTN_Select->SetColorAndOpacity(CharacterIMGColor[1]);
		SelectType = EGuildEntrySelectType::SelectImpossible;
	}

	if (SelectType == EGuildEntrySelectType::Normal)
	{

	}
	else if (SelectType == EGuildEntrySelectType::Select)
	{
		//SlotDelegate.ExecuteIfBound(EGuildEntrySlotType::FormationSlot, SelectType , GuildEntryCode);
		P_Select->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if (SelectType == EGuildEntrySelectType::SelectImpossible)
	{
		BTN_Select->SetColorAndOpacity(CharacterIMGColor[1]);
	}
	else if (SelectType == EGuildEntrySelectType::SelectPossible)
	{
		P_SelectPossible->SetVisibility(ESlateVisibility::HitTestInvisible);
	}


}

void UB2UIGuildFormationMercenarySlot::OnClickSlot()
{
	/*if (MercenaryCount == 0)
		return;*/

	if (SelectType == EGuildEntrySelectType::Normal)			//努腐沁阑锭 老馆惑怕搁 努腐茄 惑怕肺 官层霖促
	{
		SelectType = EGuildEntrySelectType::Select;
		SlotDelegate.ExecuteIfBound(EGuildEntrySlotType::MercenaryList, SelectType, GuildEntryCode, 0);					//浇吩甸 盎脚秦霖促
		SelectPlaySound();
		return;
	}
	else if (SelectType == EGuildEntrySelectType::SelectImpossible || SelectType == EGuildEntrySelectType::Select)	//努腐捞 阂啊瓷茄惑怕唱, 捞固努腐茄惑怕俊辑 度鞍篮 浇吩俊 甸绢坷搁 促矫 Normal惑怕肺 官层霖促
		SelectType = EGuildEntrySelectType::Normal;
	else if (SelectType == EGuildEntrySelectType::SelectPossible)
	{
		ChangePlaySound();
	}

	//浇吩 官层林扁
	SwapDelegate.ExecuteIfBound(EGuildEntrySlotType::MercenaryList, SelectType, GuildEntryCode, 0);				
}
