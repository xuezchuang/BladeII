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

//Doc������ �ʿ� �� ��
// ShortcutInfoID (Key)
// Popup Title String �Ǵ� Popup Title String Table ID(Key)
// Shortcut<TArray> String �Ǵ� Shortcut String Table ID(Key)
// Shortcut<TArray> EName (�ٷΰ��� �̸�)
// Condition<TArray> type
// Condition<TArray> amount
// Condition<TArray> Popup String �Ǵ� Popup String Table ID(Key)
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
	// �ܼ��� ������ ������ �������� Inventory �ν��Ͻ� ���� static ���� ���� �� ������ ��ȭ ����� �κ��丮�� �� �ִ� ���¿��� ����.
	CachedInventory = InGM->GetLobbyInventory();
	check(CachedInventory);

	m_isEnough = true;

	// �� ������ Inventory �� TargetItemID �� ���õǾ� �־�� ��. (UB2LobbyInventory::EnterItemOpMode)
	//if (!CachedInventory->GetItemOPTargetItemData(NativeUpgradeTargetData) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Upgrade)
	//{
		//�ٵ� Button���� �����ϸ� ��� �ؾ� ����??
		//CloseAndQuitItemOpMode(); // ���� �ϳ� �����Ѵٸ�.. ��¿ �� ����.
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
		//����ó���� �ʹ� ���� �ϴ°� ������....�ϴ� �������� ����
		FSingleItemInfoData* const singleData = StaticFindItemInfo() ? StaticFindItemInfo()->GetInfoData(shortcutinfo) : nullptr;
		if (!singleData)
			continue;

		const int32 userNeedAmount(m_GoodsAmountMap[shortcutinfo]);
		const int32 userHaveAmount( GetHaveAmount(singleData->FGoodsType) );
		//[@AKI, 170331] 12���� ��ȭ(Good)�� ������ conditiontype�� ���� ����ǿ� ���� Doc���� �����ͼ� �˻� ����.
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
		//�̰� ������ ��찡 ���ٰ� ��ȹ(�ڰ���)���� ���η� ���� ����
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
		//LobbyGM�� ������ �;����� ���� �� ����. StartFromLobby���� ������
		return CachedInventory ? CachedInventory->HowManyUpgradeStoneForCurrentTarget() : -1;
	}
	else if (FGoodsType.Compare(GoodsTypeFName::UPGRADE_WING_ENHANCE_INGREDIENT) == 0 ||
		FGoodsType.Compare(GoodsTypeFName::UPGRADE_WING_EVOLVE_INGREDIENT) == 0)
	{
		// B2Airport::HasEnoughGoldForWingEvolveLocalPlayer()�� �Ȱ��� ������.
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
		// ��õ���� ������ BladeIIGameImpl::GetClientDataStore()���⼭ ȣ���ϴ°� ��� LobbyInventory���� �����ͼ� �����
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

//�� ��ư�� ���� ���� ��� ���� ���� Press���� �ϰ�
//�� �� �̺�Ʈ�� Clicked���� �ϸ� ��.
void UB2ButtonGoodsShortcutToolTip::ExtraOnPress()
{
	Super::ExtraOnPress();

	if (m_DelayPress)
	{
		//ȣ�� ������ ���⸦ ���� ��� ���ְ� ���� �̺�Ʈ�� �߻��ϰ� �ؾ� ��
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
			//����ó���� �ʹ� ���� �ϴ°� ������....�ϴ� �������� ����
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

	//�׳� ���θ� ���� �Ϸ��°� ��
}

void UB2ButtonGoodsShortcutToolTip::ExtraOnHovered()
{
	//Touch Moved ���⿣ ���ұ�??
	Super::ExtraOnHovered();
}

void UB2ButtonGoodsShortcutToolTip::ExtraOnRelease()
{
	Super::ExtraOnRelease();
}