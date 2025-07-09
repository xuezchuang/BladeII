#include "BladeII.h"
#include "B2UIMainCounterAttackDG_Slot.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2UIDocHelper.h"
#include "B2RichTextBlock.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIMainCounterAttackDG_Slot::Init()
{
	Super::Init();

	if (IsHardCoding)
	{
		if (HB_CloseItemInfo.IsValid())
		{
			HB_CloseItemInfo->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (HB_OpenItemInfo.IsValid())
		{
			HB_OpenItemInfo->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (HB_CloseItemInfo_Hard.IsValid())
		{
			HB_CloseItemInfo_Hard->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (HB_OpenItemInfo_Hard.IsValid())
		{
			HB_OpenItemInfo_Hard->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else
	{
		if (HB_CloseItemInfo.IsValid())
		{
			HB_CloseItemInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (HB_OpenItemInfo.IsValid())
		{
			HB_OpenItemInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (HB_CloseItemInfo_Hard.IsValid())
		{
			HB_CloseItemInfo_Hard->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (HB_OpenItemInfo_Hard.IsValid())
		{
			HB_OpenItemInfo_Hard->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIMainCounterAttackDG_Slot::CacheAssets()
{
	GET_SLOT(UPanelWidget, HB_CloseItemInfo);
	GET_SLOT(UPanelWidget, HB_CloseItemInfo_Hard);
	GET_SLOT(UPanelWidget, HB_OpenItemInfo);
	GET_SLOT(UPanelWidget, HB_OpenItemInfo_Hard);
	GET_SLOT(UTextBlock,		TB_OpenDungeonTitle)
	GET_SLOT(UTextBlock,		TB_CloseDungeonTitle)
	GET_SLOT(UTextBlock,		TB_PossibilityTime)
	GET_SLOT(UTextBlock,		TB_ClosePossibilityTime)
	GET_SLOT(UTextBlock,		TB_OpenRewardItem)
	GET_SLOT(UTextBlock,		TB_CloseRewardItem)
	GET_SLOT(UB2RichTextBlock,	TB_PossibilityCount)
	GET_SLOT(UButton,			BTN_Select)
	GET_SLOT(UImage,			IMG_BG)
	GET_SLOT(UImage,			IMG_BG2)
	GET_SLOT(UImage,			IMG_Item)
	GET_SLOT(UImage,			IMG_Item2)
}

void UB2UIMainCounterAttackDG_Slot::UpdateStaticText()
{

}

void UB2UIMainCounterAttackDG_Slot::BindDelegates()
{
	if (BTN_Select.IsValid())
		BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIMainCounterAttackDG_Slot::OnClickSelect)
}

void UB2UIMainCounterAttackDG_Slot::SetNowDay(int32 SlotType)
{
	auto* CounterDoc = UB2UIDocHelper::GetDocCounterAttack();

	if (!CounterDoc)
		return; 

	IsOpened = false;

	auto OpenedDungeonType = CounterDoc->GetOpenedDungeonType();

	for (auto& Ele : OpenedDungeonType)
	{
		if(SlotType != Ele)
			continue;

		IsOpened = true;

		if (TB_PossibilityCount.IsValid())
			TB_PossibilityCount->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterAttack_DailyEnter")), FText::FromString(FString::FromInt(CounterDoc->GetLeftTicket())), FText::FromString(FString::FromInt(BladeIIGameImpl::GetClientDataStore().GetMaxCounterDungeonTicketCount()))));

		CounterDoc->SetDungeonType(SlotType);
	}

	DivisionLockEvent(IsOpened);
}

void UB2UIMainCounterAttackDG_Slot::SetDayType(int32 type , FString DungeonName)
{
	auto* PeriodData = BladeIIGameImpl::GetClientDataStore().GetCounterDungeonPeriodData(type);
	auto CounterDungeonData = BladeIIGameImpl::GetClientDataStore().GetCounterDungeonData();

	if (!PeriodData)
		return;

	TB_CloseDungeonTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, DungeonName));
	TB_OpenDungeonTitle->SetText(TB_CloseDungeonTitle->GetText());

	for (auto& Ele : CounterDungeonData)
	{
		if (Ele.DungeonType == type)
		{
			if (!Ele.RewardId.Num())
				return;

			TB_OpenRewardItem->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_SquareBracket")), BladeIIGameImpl::GetClientDataStore().GetRewardName(Ele.RewardId[0].Id)));
			TB_CloseRewardItem->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_SquareBracket")), BladeIIGameImpl::GetClientDataStore().GetRewardName(Ele.RewardId[0].Id)));

			IMG_Item->SetBrushFromMaterial(BladeIIGameImpl::GetClientDataStore().GetRewardIconMaterial(Ele.RewardId[0].Id));
			IMG_Item2->SetBrushFromMaterial(BladeIIGameImpl::GetClientDataStore().GetRewardIconMaterial(Ele.RewardId[0].Id));
			break;
		}
	}

	if (BGTexture.Num())
	{
		IMG_BG->SetBrushFromMaterial(BGTexture[type - 1]);
		IMG_BG2->SetBrushFromMaterial(BGTexture[type - 1]);
	}	

	TArray<EUICounterDayType>	DayTypeArr;

	for (auto& Ele : *PeriodData)
	{
		DayTypeArr.Add(static_cast<EUICounterDayType>(Ele.DayOfWeek));
		/*BeginHour = Ele.BeginHour;		//시간표시 필요없어서 삭제
		BeginMin = Ele.BeginMin;
		EndHour = Ele.EndHour;
		EndMin = Ele.EndMin;*/

		AllowedCharClass = Ele.CharacterType;
	}

	if (DayTypeArr.Num() == 2)
	{
		FString Daystr1 = DayDivision(DayTypeArr[0]);
		FString Daystr2 = DayDivision(DayTypeArr[1]);
		TB_PossibilityTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterAttack_SlotTitle1")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, *Daystr1),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, *Daystr2)));
	}
	else if (DayTypeArr.Num() == 1)
	{
		FString Daystr1 = DayDivision(DayTypeArr[0]);
		DayDivision(DayTypeArr[0]);
		TB_PossibilityTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterAttack_SlotTitle2")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, *Daystr1)));
	}

	TB_ClosePossibilityTime->SetText(TB_PossibilityTime->GetText());

	OpenType = type;
	SetNowDay(type);

}

const FString UB2UIMainCounterAttackDG_Slot::DayDivision(EUICounterDayType Day)
{
	switch (Day)
	{
	case EUICounterDayType::Monday:
		return  FString::Printf(TEXT("CounterAttack_Monday"));
	case EUICounterDayType::Tuesday:
		return  FString::Printf(TEXT("CounterAttack_Tuesday"));
	case EUICounterDayType::Wednesday:
		return  FString::Printf(TEXT("CounterAttack_Wedneday"));
	case EUICounterDayType::Thursday:
		return  FString::Printf(TEXT("CounterAttack_Thursday"));
	case EUICounterDayType::Friday:
		return  FString::Printf(TEXT("CounterAttack_Friday"));
	case EUICounterDayType::Saturday:
		return  FString::Printf(TEXT("CounterAttack_Saturday"));
	case EUICounterDayType::Sunday:
		return  FString::Printf(TEXT("CounterAttack_Sunday"));
	default:
		return FString::Printf(TEXT(""));
	}
}

FText UB2UIMainCounterAttackDG_Slot::GetTimeText(int32 beginHour, int32 beginMin, int32 endHour, int32 endMin)
{
	return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterDungeon_Time")), FText::FromString(FString::FromInt(beginHour)), FText::FromString(FString::FromInt(beginMin))
		, FText::FromString(FString::FromInt(endHour)), FText::FromString(FString::FromInt(endMin)));
}

void UB2UIMainCounterAttackDG_Slot::OnClickSelect()
{
	if (!IsOpened)
		return;

	auto* DocCounterAttack = UB2UIDocHelper::GetDocCounterAttack();
	auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocCounterAttack && DocBattle)
	{
		DocCounterAttack->SetOpenedDungeonSlot(OpenType);
	}

	UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::CounterAttackDGSelect);
}
