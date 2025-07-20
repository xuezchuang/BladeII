// Fill out your copyright notice in the Description page of Project Settings.

#include "B2ButtonGoodsShortcutToolTip.h"
#include "B2GoodsShortcutinfo.h"

#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
#include "B2Airport.h"
#include "B2UIGoodsShortcutTooltipPopup.h"
#include "B2ItemInfo.h"


#include "BladeIIGameImpl.h"

//#include "B2UIManager.h"

//Doc沥焊俊 鞘夸 茄 巴
// ShortcutInfoID (Key)
// Popup Title String 肚绰 Popup Title String Table ID(Key)
// Shortcut<TArray> String 肚绰 Shortcut String Table ID(Key)
// Shortcut<TArray> EName (官肺啊扁 捞抚)
// Condition<TArray> type
// Condition<TArray> amount
// Condition<TArray> Popup String 肚绰 Popup String Table ID(Key)
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
	// 窜鉴洒 酒捞袍 沥焊父 掘绢坷妨搁 Inventory 牢胶畔胶 绝捞 static 栏肺 掘绢棵 荐 乐栏唱 碍拳 扁瓷篮 牢亥配府啊 栋 乐绰 惑怕俊辑 悼累.
	CachedInventory = InGM->GetLobbyInventory();
	check(CachedInventory);

	m_isEnough = true;

	// 捞 矫痢俊 Inventory 狼 TargetItemID 绰 技泼登绢 乐绢具 窃. (UB2LobbyInventory::EnterItemOpMode)
	//if (!CachedInventory->GetItemOPTargetItemData(NativeUpgradeTargetData) || CachedInventory->GetItemOpMode() != ELobbyInvenItemOpMode::EIVIOP_Upgrade)
	//{
		//辟单 Button俊辑 角菩窍搁 绢痘霸 秦具 窍瘤??
		//CloseAndQuitItemOpMode(); // 父俊 窍唱 角菩茄促搁.. 绢驴 荐 绝瘤.
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
	//for (auto& shortcutinfo : m_ShortcutInfoDocID)
	//{
	//	//抗寇贸府甫 呈公 腹捞 窍绰芭 鞍瘤父....老窜 救沥己阑 困秦
	//	FSingleItemInfoData* const singleData = StaticFindItemInfo() ? StaticFindItemInfo()->GetInfoData(shortcutinfo) : nullptr;
	//	if (!singleData)
	//		continue;

	//	const int32 userNeedAmount(m_GoodsAmountMap[shortcutinfo]);
	//	const int32 userHaveAmount( GetHaveAmount(singleData->FGoodsType) );
	//	//[@AKI, 170331] 12俺狼 犁拳(Good)啊 乐栏聪 conditiontype阑 父甸绢辑 牧叼记俊 蝶扼 Doc俊辑 啊廉客辑 八荤 窍磊.
	//	if (userNeedAmount > userHaveAmount)
	//	{
	//		InFGoodsType = singleData->FGoodsType;
	//		return true;
	//	}
	//}

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
		//捞扒 何练茄 版快啊 绝促绊 扁裙(冠版怕)茄抛 备滴肺 傈崔 罐澜
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
		//LobbyGM阑 啊瘤绊 客具瘤父 掘阑 荐 乐澜. StartFromLobby俊辑 啊廉咳
		return CachedInventory ? CachedInventory->HowManyUpgradeStoneForCurrentTarget() : -1;
	}
	else if (FGoodsType.Compare(GoodsTypeFName::UPGRADE_WING_ENHANCE_INGREDIENT) == 0 ||
		FGoodsType.Compare(GoodsTypeFName::UPGRADE_WING_EVOLVE_INGREDIENT) == 0)
	{
		// B2Airport::HasEnoughGoldForWingEvolveLocalPlayer()尔 度鞍篮 肺流烙.
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
		// 措玫荤狼 标判篮 BladeIIGameImpl::GetClientDataStore()咯扁辑 龋免窍绰霸 绝绢辑 LobbyInventory俊辑 啊廉客辑 荤侩窃
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

//捞 滚瓢阑 荤侩矫 辑滚 烹脚 鞍篮 巴篮 Press俊辑 窍绊
//弊 饶 捞亥飘绰 Clicked俊辑 窍搁 凳.
void UB2ButtonGoodsShortcutToolTip::ExtraOnPress()
{
	Super::ExtraOnPress();

	if (m_DelayPress)
	{
		//龋免 鉴辑惑 咯扁甫 刚历 殿废 秦林绊 促澜 捞亥飘甫 惯积窍霸 秦具 窃
		RegisterDelayClickEvent();
		RegisterDelayPressEvent();

		StopButtonClicked();
		StopButtonPressed();
		return;
	}
}

void UB2ButtonGoodsShortcutToolTip::ExtraOnClick()
{
	//Super::ExtraOnClick();

	//if (m_DelayClick)
	//{
	//	RegisterDelayClickEvent();
	//	return;
	//}

	//FName InFGoodsType = GoodsTypeFName::None;
	//if (IsGoodsShorcutCondition(InFGoodsType))
	//{
	//	uint32 isShowShorcutIteminfoIdx = 0;
	//	for (auto& shortcutinfo : m_ShortcutInfoDocID)
	//	{
	//		//抗寇贸府甫 呈公 腹捞 窍绰芭 鞍瘤父....老窜 救沥己阑 困秦
	//		FSingleItemInfoData* const singleData = StaticFindItemInfo() ? StaticFindItemInfo()->GetInfoData(shortcutinfo) : nullptr;
	//		if (!singleData)
	//			continue;

	//		if (InFGoodsType.Compare(singleData->FGoodsType) == 0)
	//		{
	//			isShowShorcutIteminfoIdx = shortcutinfo;
	//			break;
	//		}
	//	}
	//	//*
	//	UB2UIManager* const UIMgrInst = UB2UIManager::GetInstance();
	//	if (UIMgrInst)
	//	{
	//		auto* pUI = UIMgrInst->OpenUI<UB2UIGoodsShortcutTooltipPopup>(UIFName::GoodsShortcutTooltipPopup);
	//		pUI->SetItemInfo(isShowShorcutIteminfoIdx, EGoodsButtonType::EGoodsButtonType_ShortageShortcut);
	//		m_isEnough = false;
	//	}
	//	StopButtonPressed();
	//	StopButtonClicked();

	//	UnregisterExtraStuff(m_InOwnerUnitedWidget);
	//	RegisterExtraStuff(m_InOwnerUnitedWidget);
	//}

	////弊成 出滴搁 盔贰 窍妨绰芭 窃
}

void UB2ButtonGoodsShortcutToolTip::ExtraOnHovered()
{
	//Touch Moved 咯扁浚 构且鳖??
	Super::ExtraOnHovered();
}

void UB2ButtonGoodsShortcutToolTip::ExtraOnRelease()
{
	Super::ExtraOnRelease();
}