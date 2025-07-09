// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2GoodsShortcutinfo.h"
#include "B2ButtonGoodsShortcutToolTip.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
#include "B2Airport.h"
#include "B2UIGoodsShortcutTooltipPopup.h"
#include "B2ItemInfo.h"


#include "BladeIIGameImpl.h"

//#include "B2UIManager.h"

//Doc정보에 필요 한 것
// ShortcutInfoID (Key)
// Popup Title String 또는 Popup Title String Table ID(Key)
// Shortcut<TArray> String 또는 Shortcut String Table ID(Key)
// Shortcut<TArray> EName (바로가기 이름)
// Condition<TArray> type
// Condition<TArray> amount
// Condition<TArray> Popup String 또는 Popup String Table ID(Key)
// Condition<TArray> icon

UB2ButtonGoodsShortcutToolTip::UB2ButtonGoodsShortcutToolTip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	//, m_isEnough(true)
	//, m_DelayPress(false)
	//, m_DelayClick(false)
{
	DefaultOnClickSoundIndex = -1;

	//ClickMethod = EButtonClickMethod::PreciseClick;
	//TouchMethod = EButtonTouchMethod::PreciseTap;
}

void UB2ButtonGoodsShortcutToolTip::StartFromLobby(class UB2UIManager_Lobby* const InUIManager, class AB2LobbyGameMode* const InGM)
{
	CachedLobbyGM = InGM;
	// 단순히 아이템 정보만 얻어오려면 Inventory 인스턴스 없이 static 으로 얻어올 수 있으나 강화 기능은 인벤토리가 떠 있는 상태에서 동작.
	CachedInventory = InGM->GetLobbyInventory();
	check(CachedInventory);

	m_isEnough = true;

	// 이 시점에 Inventory 의 TargetItemID 는 세팅되어 있어야 함. (UB2LobbyInventory::EnterItemOpMode)
	//if (!CachedInventory->GetItemOPTargetItemData(NativeUpgradeTargetData) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Upgrade)
	//{
		//근데 Button에서 실패하면 어떻게 해야 하지??
		//CloseAndQuitItemOpMode(); // 만에 하나 실패한다면.. 어쩔 수 없지.
	//	return;
	//}
}

void UB2ButtonGoodsShortcutToolTip::SetItemInfo(int32 _ItemInfoidx)
{
	m_ShortcutInfoDocID.Add(_ItemInfoidx);
}

void UB2ButtonGoodsShortcutToolTip::AddGoodsAmountMap(uint32 _type, int32 _amount)
{
	m_GoodsAmountMap.Add(_type, _amount);
}

bool UB2ButtonGoodsShortcutToolTip::IsGoodsShorcutCondition(FName& InFGoodsType) const
{	
	for (auto& shortcutinfo : m_ShortcutInfoDocID)
	{
		//예외처리를 너무 많이 하는거 같지만....일단 안정성을 위해
		FSingleItemInfoData* const singleData = StaticFindItemInfo() ? StaticFindItemInfo()->GetInfoData(shortcutinfo) : nullptr;
		if (!singleData)
			continue;

		const int32 userNeedAmount(m_GoodsAmountMap[shortcutinfo]);
		const int32 userHaveAmount( GetHaveAmount(singleData->FGoodsType) );
		//[@AKI, 170331] 12개의 재화(Good)가 있으니 conditiontype을 만들어서 컨디션에 따라 Doc에서 가져와서 검사 하자.
		if (userNeedAmount > userHaveAmount)
		{
			InFGoodsType = singleData->FGoodsType;
			return true;
		}
	}

	return false;
}

int32 UB2ButtonGoodsShortcutToolTip::GetHaveAmount(FName FGoodsType) const
{
	if (FGoodsType.Compare(GoodsTypeFName::GOLD) == 0)
	{
		return BladeIIGameImpl::GetClientDataStore().GetGoldAmount();
	}
	else if (FGoodsType.Compare(GoodsTypeFName::GEM) == 0)
	{
		return BladeIIGameImpl::GetClientDataStore().GetGemAmount();
	}
	else if (FGoodsType.Compare(GoodsTypeFName::ACHIEVEMENT_POINT) == 0)
	{
		//이건 부족한 경우가 없다고 기획(박경태)한테 구두로 전달 받음
		return 0;
	}
	else if (FGoodsType.Compare(GoodsTypeFName::SOUL_BLADE) == 0)
	{
		return BladeIIGameImpl::GetClientDataStore().GetBladeCurPoint();
	}
	else if (FGoodsType.Compare(GoodsTypeFName::UPGRADE_STONE_WEAPON) == 0 || FGoodsType.Compare(GoodsTypeFName::ADVANCED_UPGRADE_STONE_WEAPON) == 0 ||
		FGoodsType.Compare(GoodsTypeFName::UPGRADE_STONE_ARMOR) == 0 || FGoodsType.Compare(GoodsTypeFName::ADVANCED_UPGRADE_STONE_ARMOR) == 0 ||
		FGoodsType.Compare(GoodsTypeFName::UPGRADE_STONE_ACCESSORY) == 0 ||FGoodsType.Compare(GoodsTypeFName::ADVANCED_UPGRADE_STONE_ACCESSORY) == 0 )
	{
		//LobbyGM을 가지고 와야지만 얻을 수 있음. StartFromLobby에서 가져옴
		return CachedInventory ? CachedInventory->HowManyUpgradeStoneForCurrentTarget() : -1;
	}
	else if (FGoodsType.Compare(GoodsTypeFName::UPGRADE_WING_ENHANCE_INGREDIENT) == 0 ||
		FGoodsType.Compare(GoodsTypeFName::UPGRADE_WING_EVOLVE_INGREDIENT) == 0)
	{
		// B2Airport::HasEnoughGoldForWingEvolveLocalPlayer()랑 똑같은 로직임.
		FB2Wing CharacterWingData;
		EPCClass InPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
		BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InPCClass, CharacterWingData);
		if (IsItTimeToEvolve(CharacterWingData))
		{
			return UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::GetWingEvolveResourceRefID());
		}
		else
		{
			return UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::GetWingEnhanceResourceRefID());
		}
	}
	else if (FGoodsType.Compare(GoodsTypeFName::UPGRADE_WING_PROP_OPTION_ENHANCE_INGREDIENT) == 0)
	{
		// 대천사의 깃털은 BladeIIGameImpl::GetClientDataStore()여기서 호출하는게 없어서 LobbyInventory에서 가져와서 사용함
		return UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::GetWingPropOptionEnhanceResourceRefID());
	}
	else if (FGoodsType.Compare(GoodsTypeFName::BREVET_STONE) == 0)
	{
		return BladeIIGameImpl::GetClientDataStore().GetRankPromotionPoint();
	}
	else if (FGoodsType.Compare(GoodsTypeFName::ADVANCED_BREVET_STONE) == 0)
	{
		return BladeIIGameImpl::GetClientDataStore().GetAdvancedRankPromotionPoint();
	}
	else if (FGoodsType.Compare(GoodsTypeFName::FRIENDSHIP_POINT) == 0)
	{
		return BladeIIGameImpl::GetClientDataStore().GetSocialPoint();
	}
	else
	{
		return 0;
	}
}

FB2GoodsShortcutIndividualInfo* UB2ButtonGoodsShortcutToolTip::GetGoodsShortcutFromShortcutInfoObject(int32 _GoodsShortcutInfoID)
{
	return StaticFindGoodsShortcutInfo() != NULL ? StaticFindGoodsShortcutInfo()->GetInfoData(_GoodsShortcutInfoID) : nullptr;
}

void UB2ButtonGoodsShortcutToolTip::RegisterExtraStuff(UB2UnitedWidgetBase* InOwnerUnitedWidget)
{
	m_InOwnerUnitedWidget = InOwnerUnitedWidget;

	UB2UnitedWidgetBase* pParentUI(InOwnerUnitedWidget);
	/*
	while (pParentUI)
	{
		m_InOwnerUnitedWidget = pParentUI;

		auto parent = pParentUI->GetParent();
		//pParentUI = pParentUI->GetParent();
		int a = 0;
	}
	*/

	OnClicked.AddUniqueDynamic(this, &UB2ButtonGoodsShortcutToolTip::ExtraOnClick);
	OnPressed.AddUniqueDynamic(this, &UB2ButtonGoodsShortcutToolTip::ExtraOnPress);
	OnReleased.AddUniqueDynamic(this, &UB2ButtonGoodsShortcutToolTip::ExtraOnRelease);
	OnHovered.AddUniqueDynamic(this, &UB2ButtonGoodsShortcutToolTip::ExtraOnHovered);

	OnTouchImpulseRingInst.Init(InOwnerUnitedWidget, OnTouchImpulseRingWidgetName, OnTouchImpulseRingAnimTime, OnTouchImpulseRingParamRange_Begin, OnTouchImpulseRingParamRange_End);
}

void UB2ButtonGoodsShortcutToolTip::UnregisterExtraStuff(UB2UnitedWidgetBase* InOwnerUnitedWidget)
{
	OnClicked.RemoveDynamic(this, &UB2ButtonGoodsShortcutToolTip::ExtraOnClick);
	OnPressed.RemoveDynamic(this, &UB2ButtonGoodsShortcutToolTip::ExtraOnPress);
	OnReleased.RemoveDynamic(this, &UB2ButtonGoodsShortcutToolTip::ExtraOnRelease);
	OnHovered.RemoveDynamic(this, &UB2ButtonGoodsShortcutToolTip::ExtraOnHovered);
	OnTouchImpulseRingInst.Clear();
}

void UB2ButtonGoodsShortcutToolTip::ClickedBroadCast()
{
	m_ReserveClicked.Broadcast();
}

void UB2ButtonGoodsShortcutToolTip::PressedBroadCast()
{
	m_ReservePressed.Broadcast();
}

void UB2ButtonGoodsShortcutToolTip::RegisterDelayClickEvent()
{
	m_ReserveClicked.AddUniqueDynamic(this, &UB2ButtonGoodsShortcutToolTip::ExtraOnClick);
	m_ReserveClicked = OnClicked;
	StopButtonClicked();
}

void UB2ButtonGoodsShortcutToolTip::RegisterDelayPressEvent()
{
	m_ReservePressed.Clear();
	m_ReservePressed.AddUniqueDynamic(this, &UB2ButtonGoodsShortcutToolTip::ExtraOnPress);
	m_ReservePressed = OnPressed;
}

void UB2ButtonGoodsShortcutToolTip::StopButtonClicked()
{
	OnClicked.Clear();
}

void UB2ButtonGoodsShortcutToolTip::StopButtonPressed()
{
	OnPressed.Clear();
}

//이 버튼을 사용시 서버 통신 같은 것은 Press에서 하고
//그 후 이벤트는 Clicked에서 하면 됨.
void UB2ButtonGoodsShortcutToolTip::ExtraOnPress()
{
	Super::ExtraOnPress();

	if (m_DelayPress)
	{
		//호출 순서상 여기를 먼저 등록 해주고 다음 이벤트를 발생하게 해야 함
		RegisterDelayClickEvent();
		RegisterDelayPressEvent();

		StopButtonClicked();
		StopButtonPressed();
		return;
	}
}

void UB2ButtonGoodsShortcutToolTip::ExtraOnClick()
{
	Super::ExtraOnClick();

	if (m_DelayClick)
	{
		RegisterDelayClickEvent();
		return;
	}

	FName InFGoodsType = GoodsTypeFName::None;
	if (IsGoodsShorcutCondition(InFGoodsType))
	{
		uint32 isShowShorcutIteminfoIdx = 0;
		for (auto& shortcutinfo : m_ShortcutInfoDocID)
		{
			//예외처리를 너무 많이 하는거 같지만....일단 안정성을 위해
			FSingleItemInfoData* const singleData = StaticFindItemInfo() ? StaticFindItemInfo()->GetInfoData(shortcutinfo) : nullptr;
			if (!singleData)
				continue;

			if (InFGoodsType.Compare(singleData->FGoodsType) == 0)
			{
				isShowShorcutIteminfoIdx = shortcutinfo;
				break;
			}
		}
		//*
		UB2UIManager* const UIMgrInst = UB2UIManager::GetInstance();
		if (UIMgrInst)
		{
			auto* pUI = UIMgrInst->OpenUI<UB2UIGoodsShortcutTooltipPopup>(UIFName::GoodsShortcutTooltipPopup);
			pUI->SetItemInfo(isShowShorcutIteminfoIdx, EGoodsButtonType::EGoodsButtonType_ShortageShortcut);
			m_isEnough = false;
		}
		StopButtonPressed();
		StopButtonClicked();

		UnregisterExtraStuff(m_InOwnerUnitedWidget);
		RegisterExtraStuff(m_InOwnerUnitedWidget);
	}

	//그냥 놔두면 원래 하려는거 함
}

void UB2ButtonGoodsShortcutToolTip::ExtraOnHovered()
{
	//Touch Moved 여기엔 뭐할까??
	Super::ExtraOnHovered();
}

void UB2ButtonGoodsShortcutToolTip::ExtraOnRelease()
{
	Super::ExtraOnRelease();
}