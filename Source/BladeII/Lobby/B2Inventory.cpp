// Fill out your copyright notice in the Description page of Project Settings.


#include "B2Inventory.h"
//#include "BladeII.h"
//#include "B2ItemInfo.h"
//#include "B2LobbyGameMode.h"
//#include "B2CombatStatEvaluator.h"
#include "B2PCClassInfo.h"

#include "FB2ErrorMessage.h"
//#include "BladeIIGameImpl.h"

#include "BladeIIUtil.h"

float FB2Item::GetPrimaryPoint(bool bCountRelevantOption) const
{
	float RetPoint = FMath::FloorToFloat(PrimaryPoint);
	
	// 만일 이걸로 얻어온 값에다가 Option 으로 인한 증가 수치는 따로 고려할 거라면 bCountRelevantOption 은 false 로 주어야 할 것.
	// 실질적으로 여러 상황에서 옵션까지 고려한다고 해도 완벽하게 하기는 어려울 것. 그냥 단순하게 PrimaryPoint 만 체크하는 선에서 끊는게 나을 수 있다.
	if (bCountRelevantOption)
	{
		//const EItemOption RelevantOption = CombatStatEval::GetPrimPointIncOption(PrimaryPointType);
		//if (RelevantOption != EItemOption::EIO_End)
		//{
		//	const float OrgValue = FMath::Max(GetEffectiveItemOptionValue(RelevantOption, *this), 0.0f);
		//	const float FixedValue = FMath::FloorToFloat(OrgValue);
		//	RetPoint += FixedValue;
		//}
	}

	return RetPoint;
}

void FB2Item::UpdatePreviewItemPrimaryPoint()
{
	const int32 iPreviewItemQuality = 1;
	const float fPreviewPrimaryPoint = GetPreviewPrimaryPoint(PrimaryPointType, ItemFactorLevel, iPreviewItemQuality, Level);

	this->SetPrimaryPoint(fPreviewPrimaryPoint);
}

void FB2Item::UpdatePreviewItemPower()
{
	//float fPreviewCombatValue = FMath::FloorToFloat(PrimaryPoint * 100);
	//float fPreviewOptionCombatValue = 0.f;

	//FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	//if (const FMD_ItemSingleElem* ItemInfo = CliDataStore.GetItemMasterDetailInfo(ItemRefID))
	//{
	//	for (auto& ItemOpt : ItemInfo->UniqueOptions)
	//	{
	//		fPreviewOptionCombatValue += ItemOpt.RawOptionAmount * CliDataStore.GetOptionWeight(ItemOpt.OptionType);
	//	}
	//}

	//this->Power = fPreviewOptionCombatValue + (CliDataStore.GetOptionWeight(ItemPrimaryPointTypeToEItemOptionType(PrimaryPointType)) * (fPreviewCombatValue / 100));
}

FB2Item& FB2Item::operator=(const b2network::B2ItemServerInfoPtr rhs)
{
	// 서버에서 받은 값으로 멤버 변수에 세팅
	//InstanceUID = rhs->id;
	//ItemRefID = rhs->template_id;
	//
	//StarGrade = rhs->grade;
	//Quality = rhs->quality;
	//ConsumingAmount = rhs->amount;

	//AllowedPCClass = (int32)SvrToCliPCClassType(rhs->character_type);

	//InventoryType = SvrToCliItemInvenType(rhs->inventory_type);
	//EquipPlace = SvrToCliItemEquipPlace(rhs->equip_position); // MasterData 통해 다시 세팅하는데.. 이전엔 서버 equip_position 이 장착된 아이템만 값이 세팅되어 있던 것도 같고 여튼..

	//// 요건 현재 장착되어 있는지 여부. 서버와 표시 방법이 다름.
	//// TODO 프리셋 정보 참조
	//FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	//bCurrentlyEquipped = CliDataStore.GetLocalCharacterData().GetUserAllItems().IsInCurrentItemPreset(IntToPCClass(AllowedPCClass), InstanceUID);

	//Level = rhs->level;
	//Exp = rhs->exp;
	//SurpassCount = rhs->surpass_count;
	//EnhanceLevel = rhs->enhance_level;
	//PrimaryPoint = rhs->md_main_attribute_value;

	//RandomOptions.Empty();
	//EItemOption ThisOptionType = SvrToCliOptionType(rhs->random_option_id1);
	//if (ThisOptionType != EItemOption::EIO_End)
	//{
	//	RandomOptions.Add(FItemOption(ThisOptionType, rhs->random_option_value1));
	//}
	//ThisOptionType = SvrToCliOptionType(rhs->random_option_id2);
	//if (ThisOptionType != EItemOption::EIO_End)
	//{
	//	RandomOptions.Add(FItemOption(ThisOptionType, rhs->random_option_value2));
	//}
	//ThisOptionType = SvrToCliOptionType(rhs->random_option_id3);
	//if (ThisOptionType != EItemOption::EIO_End)
	//{
	//	RandomOptions.Add(FItemOption(ThisOptionType, rhs->random_option_value3));
	//}

	//SealOptions.Empty();
	//for (auto SealOptionInfo : rhs->seal_slot_states)
	//{
	//	SealOptions.Add(FSealOption(SealOptionInfo->seal_slot, SealOptionInfo->opened, SvrToCliOptionType(SealOptionInfo->option_id), SealOptionInfo->option_value));
	//}

	//OpenSealSlotCount = rhs->open_seal_slot_count;
	//
	//bIsLocked = rhs->locked;
	//Power = rhs->power;

	//IsNew = rhs->is_new;
	//// 마스터 데이터의 값으로 업데이트
	//UpdateItemMasterDataOnMD();
	return *this;
}

FB2Item::FB2Item(const FMD_ItemSingleElem* ItemMDElem)
	:FB2Item()
{
	if (ItemMDElem)
	{
		UpdateByItemMasterData(ItemMDElem);
	}
}

FB2Item::FB2Item(const b2network::B2AetherServerInfoPtr Ether)
{
	Empty();

	InstanceUID = Ether->id;
	//ItemRefID = rhs->template_id;
	ItemClass = EItemClass::EIC_Ether;
	StarGrade = Ether->grade;
	Quality = Ether->type;
	//ConsumingAmount = rhs->amount;

	AllowedPCClass = (int32)SvrToCliPCClassType(Ether->character_type);

	InventoryType = EItemInvenType::EIIVT_End;
	EquipPlace = EItemEquipPlace::EIEP_End; // MasterData 통해 다시 세팅하는데.. 이전엔 서버 equip_position 이 장착된 아이템만 값이 세팅되어 있던 것도 같고 여튼..

											//FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
											//bCurrentlyEquipped = CliDataStore.GetLocalCharacterData().GetUserAllItems().IsInCurrentItemPreset(IntToPCClass(AllowedPCClass), InstanceUID);

	Level = Ether->tier;
	//Exp = rhs->exp;
	SurpassCount = Ether->slot;
	EnhanceLevel = Ether->level;
	//PrimaryPoint = rhs->md_main_attribute_value;

	/*RandomOptions.Empty();
	EItemOption ThisOptionType = SvrToCliOptionType(rhs->random_option_id1);
	if (ThisOptionType != EItemOption::EIO_End)
	{
	RandomOptions.Add(FItemOption(ThisOptionType, rhs->random_option_value1));
	}
	ThisOptionType = SvrToCliOptionType(rhs->random_option_id2);
	if (ThisOptionType != EItemOption::EIO_End)
	{
	RandomOptions.Add(FItemOption(ThisOptionType, rhs->random_option_value2));
	}
	ThisOptionType = SvrToCliOptionType(rhs->random_option_id3);
	if (ThisOptionType != EItemOption::EIO_End)
	{
	RandomOptions.Add(FItemOption(ThisOptionType, rhs->random_option_value3));
	}*/

	//bIsLocked = rhs->locked;
	//Power = rhs->power;

	//IsNew = rhs->is_new;
}

FB2Item::FB2Item(const b2network::B2CostumeServerInfo& CostumeItemInfo)
{
	Empty();

	ItemRefID			= CostumeItemInfo.template_id;
	InstanceUID			= CostumeItemInfo.id;
	ConsumingAmount		= CostumeItemInfo.amount;
	Quality				= CostumeItemInfo.quality;

	RandomOptions.Empty();
	EItemOption ThisOptionType = SvrToCliOptionType(CostumeItemInfo.random_option_id1);
	if (ThisOptionType != EItemOption::EIO_End)
	{
		RandomOptions.Add(FItemOption(ThisOptionType, CostumeItemInfo.random_option_value1));
	}
	ThisOptionType = SvrToCliOptionType(CostumeItemInfo.random_option_id2);
	if (ThisOptionType != EItemOption::EIO_End)
	{
		RandomOptions.Add(FItemOption(ThisOptionType, CostumeItemInfo.random_option_value2));
	}
	ThisOptionType = SvrToCliOptionType(CostumeItemInfo.random_option_id3);
	if (ThisOptionType != EItemOption::EIO_End)
	{
		RandomOptions.Add(FItemOption(ThisOptionType, CostumeItemInfo.random_option_value3));
	}

	Level				= CostumeItemInfo.level;
	Exp					= CostumeItemInfo.exp;
	EnhanceLevel		= CostumeItemInfo.enhance_level;
	SurpassCount		= CostumeItemInfo.surpass_count;
	bIsLocked			= CostumeItemInfo.locked;
	Power				= CostumeItemInfo.power;
	IsNew				= CostumeItemInfo.is_new;
	PrimaryPoint		= CostumeItemInfo.md_main_attribute_value;

	//마스터 데이터를 기준으로 다시 세팅 되는 항목
	//ItemClass, CostumeEquipPlace, AllowedPCClass, InventoryType, StarGrade
	UpdateItemMasterDataOnMD();

	//FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	//bCurrentlyEquipped = CliDataStore.GetLocalCharacterData().GetUserAllItems().IsEquippedCostume(*this);
}

void FB2Item::UpdateItemMasterDataOnMD()
{
//	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
//	const FMD_ItemSingleElem* ItemMDElem = CliDataStore.GetItemMasterDetailInfo(ItemRefID);
//	if (ItemMDElem)
//	{
//		UpdateByItemMasterData(ItemMDElem);
//
//		// RefID 자리수에 따른 규약을 이젠 정식으로 사용하게 되므로 혹시라도 예상과 다르지 않은지 체크한다.
//#if WITH_EDITOR // 음.. 에디터에서만 하자.. 아직은 빌드 버전에서 혹시라도 쭁날까봐 걱정되는군
//		check(CheckForItemPropertyRule(*this));
//#endif
//	}
}

void FB2Item::UpdateByItemMasterData(const FMD_ItemSingleElem* InMasterData)
{
	//if (InMasterData)
	//{
	//	SetID = InMasterData->SetOptionID;
	//	MaxStarGrade = InMasterData->MaxGrade;
	//	ItemClass = InMasterData->ItemClass;
	//	InventoryType = InMasterData->ItemInvenType;
	//	EquipPlace = InMasterData->ItemEquipPlace; // 이미 세팅이 되어 있나..
	//	MaxLevel = InMasterData->MaxLevel + ((SurpassCount > 0) ? SurpassCount : 0); // 초월로 인한 MaxLevel 확장은 MasterData 엔 없으니 따로 반영.
	//	ItemFactorLevel = InMasterData->ItemFactorLevel;
	//	PrimaryPointType = InMasterData->PrimaryType;

	//	IntrinsicOptions.Empty();
	//	for (auto option : InMasterData->UniqueOptions) {
	//		IntrinsicOptions.Add(option);
	//	}

	//	BuyingPrice = InMasterData->BuyPrice;
	//	SellingPrice = InMasterData->SellPrice;

	//	ItemRefID = InMasterData->ItemTemplateID;
	//	StarGrade = InMasterData->ItemGrade;

	//	AllowedPCClass = PCClassToInt(InMasterData->ItemEquipClass);

	//	CostumeEquipPlace = InMasterData->CostumeEquipPlace;
	//}
}

//////////////////////////////////////////////////////////////////////
// FItemRefIDHelper. ItemRefID 다루는 유틸들 모두 모음
// 장착 아이템 RefID 룰을 다루는 유틸이나 재화, 지료 등 특수 아이템 RefID 등
// ItemRefID 를 코드에서 직접 참조하는 것이 바람직하지 않지만 
// DB 마냥 아무 속성이나 키로 해서 query 를 할 수 없는 클라이언트 사정상 직접 사용하게 될 수밖에 없음.

bool FItemRefIDHelper::IsGoodsRefID(int32 InID)
{
	return (InID == GetGoodsID_Gold() || InID == GetGoodsID_Gem() || InID == GetGoodsID_BladePoint() || InID == GetGoodsID_FriendshipPoint() || InID == GetGoodsID_DarkCrystal());
}

int32 FItemRefIDHelper::GetUpgradeStoneIDForUpgradeTarget(const FB2Item& InUpgradeTarget, const EUpgradeded EUpgraded)
{
	// 이미 업그레이드 된 경우 -1을 해서 기존 아이템이 사용한 승급석이 어떤건질 확인해야 되기 때문.
	//uint16 StarOffset = (EUpgraded == EUpgradeded::UPGRADE_AFTER ? 1 : 0);
	//bool CheckAdvanced = (InUpgradeTarget.StarGrade - StarOffset >= FItemGradeInfo::MINIMUM_ADVANCED_UPGRADE_STAR_GRADE);

	//switch (InUpgradeTarget.InventoryType)
	//{
	//case EItemInvenType::EIIVT_Weapon:
	//{
	//	if (CheckAdvanced)	return ITEM_REF_ID_ADVANCED_UPGRADE_STONE_WEAPON;
	//	else				return ITEM_REF_ID_UPGRADE_STONE_WEAPON;
	//}
	//case EItemInvenType::EIIVT_Protection:
	//{
	//	if (CheckAdvanced)	return ITEM_REF_ID_ADVANCED_UPGRADE_STONE_ARMOR;
	//	else				return ITEM_REF_ID_UPGRADE_STONE_PROTECTION;
	//}
	//case EItemInvenType::EIIVT_Accessory:
	//{
	//	if (CheckAdvanced)	return ITEM_REF_ID_ADVANCED_UPGRADE_STONE_ACCESSORY;
	//	else				return ITEM_REF_ID_UPGRADE_STONE_ACCESSORY;
	//}
	//}
	return 0;
}
int32 FItemRefIDHelper::GetEnhanceScrollRefId(const FB2Item& InTargetItem)
{
	switch (InTargetItem.InventoryType)
	{
	case EItemInvenType::EIIVT_Weapon: return ITEM_REF_ID_ENHANCE_SCROLL_WEAPON;
	case EItemInvenType::EIIVT_Protection: return ITEM_REF_ID_ENHANCE_SCROLL_PROTECTION;
	case EItemInvenType::EIIVT_Accessory: return ITEM_REF_ID_ENHANCE_SCROLL_ACCESSORY;
	}
	return 0;
}

int32 FItemRefIDHelper::GetBlessEnhanceScrollRefId(const FB2Item& InTargetItem)
{
	switch (InTargetItem.InventoryType)
	{
	case EItemInvenType::EIIVT_Weapon: return ITEM_REF_ID_BLESS_ENHANCE_SCROLL_WEAPON;
	case EItemInvenType::EIIVT_Protection: return ITEM_REF_ID_BLESS_ENHANCE_SCROLL_PROTECTION;
	case EItemInvenType::EIIVT_Accessory: return ITEM_REF_ID_BLESS_ENHANCE_SCROLL_ACCESSORY;
	}
	return 0;
}

int32 FItemRefIDHelper::GetEnhanceProtectionItemRefId()
{
	return ITEM_REF_ID_ENHANCE_PROTECTION_ITEM; // 추후 타입별로 분류될 수도..
}

int32 FItemRefIDHelper::GetGeneralLotteryRefID(int32 InRewardID)
{
	return ITEM_REF_ID_GENERAL_LOTTERY_BEGIN + InRewardID;
}
int32 FItemRefIDHelper::GetSelectiveLotteryRefID(int32 InRewardID)
{
	return ITEM_REF_ID_SELECTIVE_LOTTERY_BEGIN + InRewardID;
}
int32 FItemRefIDHelper::GetSelectiveItemLotteryRefID(int32 InRewardID)
{
	return ITEM_REF_ID_SELECTIVE_ITEM_LOTTERY_BEGIN + InRewardID;
}

int32 FItemRefIDHelper::GetFixedGradeLotteryRefID(int32 InRewardID)
{
	return ITEM_REF_ID_FIXED_GRADE_LOTTERY + InRewardID;
}

int32 FItemRefIDHelper::GetPrefixTotemLotteryRefID(int32 InRewardID)
{
	return TOTEM_REF_ID_PREFIXED_GRADE_LOTTERY_BEGIN + InRewardID;
}


int32 FItemRefIDHelper::GetSelectiveEtherLotteryRefID(int32 InRewardID)
{
	return ITEM_REF_ID_SELECTIVE_ETHER_LOTTERY_BEGIN + InRewardID;
}

int32 FItemRefIDHelper::GetPrefixSelectiveLotteryRefID(int32 InRewardID)
{
	return ITEM_REF_ID_PREFIX_SELECTIVE_LOTTERY_BEGIN + InRewardID;
}

int32 FItemRefIDHelper::GetRewardItemId(ERewardType InRewardType, int32 InRewardID)
{
	switch (InRewardType)
	{
	case ERewardType::Gold: return GetGoodsID_Gold();
	case ERewardType::Gem: return GetGoodsID_Gem();
	case ERewardType::BladePoint: return GetGoodsID_BladePoint();
	case ERewardType::ExperiencePoint: return GetGoodsID_EXP();
	case ERewardType::Hero_Piece: return FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE;
	case ERewardType::Item: return InRewardID;
	case ERewardType::General_Lottery:	return GetGeneralLotteryRefID(InRewardID);
	case ERewardType::Selective_Lottery: return GetSelectiveLotteryRefID(InRewardID);
	case ERewardType::Selective_Item_Lottery: return GetSelectiveItemLotteryRefID(InRewardID);
	case ERewardType::Rank_Promotion_Point:	return FItemRefIDHelper::ITEM_REF_ID_BREVET_STONE;
	case ERewardType::Advanced_Rank_Promotion_Point:	return FItemRefIDHelper::ITEM_REF_ID_ADVANCED_BREVET_STONE;
	case ERewardType::Hot_Time_Gold_Buff_Ticket: return FItemRefIDHelper::ITEM_REF_ID_GOLD_BUFF_TICKET;
	case ERewardType::Hot_Time_Exp_Buff_Ticket: return FItemRefIDHelper::ITEM_REF_ID_EXP_BUFF_TICKET;
	case ERewardType::Stage_Boost_Ticket:	return FItemRefIDHelper::ITEM_REF_ID_STAGE_BOOST;
	case ERewardType::Shop_Ten_Lottery_Ticket:	return FItemRefIDHelper::ITEM_REF_ID_SHOPTENLOTTERY_TICKET;
	case ERewardType::Fame_Point:	return FItemRefIDHelper::ITEM_REF_ID_HONER_POINT;
	case ERewardType::Dark_Crystal: return GetGoodsID_DarkCrystal();
	case ERewardType::Stage_Auto_Clear_Ticket: return GetGoodsID_StageAutoClearTicket();
	case ERewardType::Prefix_Selective_Lottery: return GetPrefixSelectiveLotteryRefID(InRewardID);
	}
	return 0;
}

int32 FItemRefIDHelper::GetProductDataItemId(EStoreItemType InStoreItemType, int32 InProductId)
{
	switch (InStoreItemType)
	{
	case EStoreItemType::BladePoint: return GetGoodsID_BladePoint();
	case EStoreItemType::Gem: return GetGoodsID_Gem();
	case EStoreItemType::Gold: return GetGoodsID_Gold();
	case EStoreItemType::DarkCrystal: return GetGoodsID_DarkCrystal();
	case EStoreItemType::Item: return InProductId;
	case EStoreItemType::GeneralLottery: return GetGeneralLotteryRefID(InProductId);
	case EStoreItemType::SelectiveLottery: return GetSelectiveLotteryRefID(InProductId);
	case EStoreItemType::GoldBuffTicket: return GetGoodsID_GoldBuffTicket();
	case EStoreItemType::ExpBuffTicket: return GetGoodsID_ExpBuffTicket();
	case EStoreItemType::StageBoostTicket: return GetGoodsID_StageBoostTicket();
	case EStoreItemType::ShopTenLotteryTicket: return GetGoodsID_ShopTenLotteryTicket();
	case EStoreItemType::StageAutoClearTicket: return GetGoodsID_StageAutoClearTicket();
	case EStoreItemType::NicknameChangeTicket: return GetGoodsID_NicknameChangeTicket();
	}

	return 0;
}

int32 FItemRefIDHelper::GetRelicIDFromBossPieceRefID(int32 InRefID)
{
	//지금은 이렇게 하고 있지만 iteminfo idx를 2~3번째 자리를 비트연산 해서 가져오게 변경 할 예정
	return InRefID - ITEM_REF_ID_BOSSPIECE_BEGIN + 1;
}

int32 FItemRefIDHelper::GetBossPieceRefIDFromRelicID(int32 InRelicID)
{ //각 유물과 보스조각은 1:1 매칭이므로 , (유물번호-1)을 더해줌.
	return ITEM_REF_ID_BOSSPIECE_BEGIN + InRelicID - 1;
}

int32 FItemRefIDHelper::GetWingResourceRefIDByAcqElemType(EWingIngredAcqElemType InAcqElemType)
{
	return InAcqElemType == EWingIngredAcqElemType::WAET_EnhanceIngred ? GetWingEnhanceResourceRefID() :
		(InAcqElemType == EWingIngredAcqElemType::WAET_EvolveIngred ? GetWingEvolveResourceRefID() :
			GetWingPropOptionEnhanceResourceRefID());
}

/*================================================================================
아이템의 RefID 자릿수에서 몇가지 규약에 따른 정보를 빼 오는 함수들인데,
가급적 이걸 쓰기보다는 FB2Item 자체 필드를 사용하는 게 좋다.
그렇지만 결국 이런 사용이 필요한 곳들이 있어서 어쩔 수 없이 사용.
================================================================================*/
EItemClass FItemRefIDHelper::ExtractItemClassFromRefID(int32 InItemRefID)
{
	//if (InItemRefID == FItemRefIDHelper::GetGoodsID_Gold()) // 특수한 경우.. 근데 재화나 재료 등 장착 아이템 외 기타 모든 것들을 여기에 넣어 주어야 하는 건 아니다.
	//{
	//	return EItemClass::EIC_Gold;
	//}

	//const int32 TheBiggestCategoryDigit = ExtractNthDigitR<7>(InItemRefID);
	//if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT)	// 장착 아이템만 아래 규칙이 맞음
	//{
	//	// 4번째 자리
	//	int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
	//	switch (FourthDigit)
	//	{
	//	case 0: return EItemClass::EIC_Weapon;
	//	case 1: return EItemClass::EIC_Helmet;
	//	case 2: return EItemClass::EIC_BodyArmor;
	//	case 3: return EItemClass::EIC_Glove;
	//	case 4: return EItemClass::EIC_Shoes;
	//	case 5: return EItemClass::EIC_Belt;
	//	case 6: return EItemClass::EIC_Necklace;
	//	case 7: return EItemClass::EIC_Ring;
	//	case 8: return EItemClass::EIC_Wing;
	//	}
	//}
	//else if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_ESSENCE)
	//{
	//	int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
	//	switch (FourthDigit)
	//	{
	//	case 0: return EItemClass::EIC_WeaponEssence;
	//	case 1: return EItemClass::EIC_ProtectionEssence;
	//	case 2: return EItemClass::EIC_AccessoryEssence;
	//	}
	//}
	//else if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_COSTUME)
	//{
	//	int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
	//	switch (FourthDigit)
	//	{
	//	case 0: return EItemClass::EIC_CostumeWeapon;
	//	case 1: return EItemClass::EIC_CostumeHat;
	//	case 2: return EItemClass::EIC_CostumeArmor;
	//	}
	//}
	//else if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_SURPASSANVIL)
	//{
	//	int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
	//	switch (FourthDigit)
	//	{
	//	case 0: return EItemClass::EIC_WeaponSurpassAnvil;
	//	case 1: return EItemClass::EIC_ArmorSurpassAnvil;
	//	case 2: return EItemClass::EIC_AccessorySurpassAnvil;
	//	}
	//}

	return EItemClass::EIC_End;
}
EItemClass FItemRefIDHelper::ExtractItemClassFromRefIDForItemForge(int32 InItemRefID)
{
	//if (InItemRefID == FItemRefIDHelper::GetGoodsID_Gold()) // 특수한 경우.. 근데 재화나 재료 등 장착 아이템 외 기타 모든 것들을 여기에 넣어 주어야 하는 건 아니다.
	//{
	//	return EItemClass::EIC_Gold;
	//}

	//const int32 TheBiggestCategoryDigit = ExtractNthDigitR<7>(InItemRefID);
	//if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT)	// 장착 아이템만 아래 규칙이 맞음
	//{
	//	// 4번째 자리
	//	int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
	//	switch (FourthDigit)
	//	{
	//	case 0: return EItemClass::EIC_Weapon;
	//	case 1: return EItemClass::EIC_Helmet;
	//	case 2: return EItemClass::EIC_BodyArmor;
	//	case 3: return EItemClass::EIC_Glove;
	//	case 4: return EItemClass::EIC_Shoes;
	//	case 5: return EItemClass::EIC_Belt;
	//	case 6: return EItemClass::EIC_Necklace;
	//	case 7: return EItemClass::EIC_Ring;
	//	case 8: return EItemClass::EIC_Wing;
	//	}
	//}
	//else if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_ESSENCE)
	//{
	//	int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
	//	switch (FourthDigit)
	//	{
	//	case 0: return EItemClass::EIC_WeaponEssence;
	//	case 1: return EItemClass::EIC_ProtectionEssence;
	//	case 2: return EItemClass::EIC_AccessoryEssence;
	//	}
	//}
	//else if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_COSTUME)
	//{
	//	int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
	//	switch (FourthDigit)
	//	{
	//	case 0: return EItemClass::EIC_CostumeWeapon;
	//	case 1: return EItemClass::EIC_CostumeHat;
	//	case 2: return EItemClass::EIC_CostumeArmor;
	//	}
	//}
	//else if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_SURPASSANVIL)
	//{
	//	int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
	//	switch (FourthDigit)
	//	{
	//	case 0: return EItemClass::EIC_WeaponSurpassAnvil;
	//	case 1: return EItemClass::EIC_ArmorSurpassAnvil;
	//	case 2: return EItemClass::EIC_AccessorySurpassAnvil;
	//	}
	//}
	//else if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_Consumable)
	//{
	//	// 더미 맨 앞자리가 8일 경우 소모품으로 출력 필요하기 때문에 임의 승급석을 출력 함.
	//	return EItemClass::EIC_UpgradeStone;
	//}

	return EItemClass::EIC_End;
}

EPCClass FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(int32 InItemRefID)
{
	//const int32 TheBiggestCategoryDigit = ExtractNthDigitR<7>(InItemRefID);
	//// 장착 아이템, 정수(Essence), 코스튬 아이템만 아래 규칙이 맞음
	//if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT || 
	//	TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_ESSENCE || 
	//	TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_COSTUME ||
	//	TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_SURPASSANVIL
	//	)
	//{
	//	// 뒤에서 6번째 자리
	//	int32 SixthDigit = ExtractNthDigitR<6>(InItemRefID);
	//	return ItemAllowedPCClassEnum(SixthDigit);
	//}
	//else
	//{
		return EPCClass::EPC_End;
	//}
}

int32 FItemRefIDHelper::ExtractItemStarGradeFromRefID(int32 InItemRefID)
{
	//const int32 TheBiggestCategoryDigit = ExtractNthDigitR<7>(InItemRefID);
	//// 장착 아이템, 정수(Essence), 코스튬 아이템만 아래 규칙이 맞음
	//if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT ||
	//	TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_ESSENCE ||
	//	TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_COSTUME ||
	//	TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_SURPASSANVIL
	//	)
	//{
	//	// 뒤에서 5번째 자리 십의자리 등급
	//	int32 Grade = 0;
	//	Grade = (ExtractNthDigitR<ITEM_REF_ID_STAR_TENGRAD_DIGIT_NUMBER>(InItemRefID)) * 10;

	//	// 뒤에서 3번째 자리 일의자리 등급
	//	Grade += ExtractNthDigitR<ITEM_REF_ID_STAR_GRAD_DIGIT_NUMBER>(InItemRefID);

	//	return Grade;
	//}
	//else
	{
		return 0;
	}
}

int32 FItemRefIDHelper::ExtractItemVariationFromRefID(int32 InItemRefID)
{
	// 맨 뒤 두자리
	int32 Digit_1 = ExtractNthDigitR<1>(InItemRefID);
	int32 Digit_2 = ExtractNthDigitR<2>(InItemRefID);

	return (Digit_2 * 10 + Digit_1);
	return 0;
}

TArray<int32> FItemRefIDHelper::GetAllGradeItemRefIDs(int32 InItemRefID)
{
	int32 Digit_3 = ExtractNthDigitR<3>(InItemRefID);
	int32 BaseItemID = InItemRefID - Digit_3 * 100;

	TArray<int32> AllGradeItemIDs;
	//for (int32 Grade = 1; Grade <= FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE; Grade++)
	//{
	//	AllGradeItemIDs.Add(BaseItemID + (Grade * 100));
	//}

	return AllGradeItemIDs;
}

bool FItemRefIDHelper::IsEquipmentItemByRefID(int32 InRefID)
{ // 일반적으로는 IsEquipmentItem 을 사용

	//const int32 IdLargeDigit = ExtractNthDigitR<7>(InRefID);
	//return IdLargeDigit == ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT;
	return true;
}

bool FItemRefIDHelper::IsEssenceItemByRefID(int32 InRefID) 
{ // 일반적으로는 IsEssenceItem 을 사용
	return (ExtractNthDigitR<7>(InRefID) == ITEM_REF_ID_LARGEST_DIGIT_ESSENCE); 
}

bool FItemRefIDHelper::IsAnvilItemByRefID(int32 InRefID)
{ // 일반적으로는 IsAnvilItem 을 사용
	return (ExtractNthDigitR<7>(InRefID) == ITEM_REF_ID_LARGEST_DIGIT_SURPASSANVIL);
}

bool FItemRefIDHelper::IsCostumeItemByRefID(int32 InRefID)
{
	return (ExtractNthDigitR<7>(InRefID) == ITEM_REF_ID_LARGEST_DIGIT_COSTUME);
}

int32 FItemRefIDHelper::GenerateRewardRepresentativeItemID(int32 InRewardGroupID, int32 InType)
{
	// 스테이지 정보에서 보상 아이템 대표 아이콘에 사용. ItemInfo 테이블에 등록된 번호 체계.
	return ITEM_REF_ID_LARGEST_DIGIT_REWARDS * 1000000 + InRewardGroupID * 1000 + InType;
}

ERewardType FItemRefIDHelper::GetRewardTypeByRefID(int32 InRefID)
{
	switch (InRefID)
	{
	case FItemRefIDHelper::ITEM_REF_ID_GOLD:
		return ERewardType::Gold;

	case FItemRefIDHelper::ITEM_REF_ID_GEM: 
		return ERewardType::Gem;

	case FItemRefIDHelper::ITEM_REF_ID_BLADEPOINT: 
		return ERewardType::BladePoint;

	case FItemRefIDHelper::ITEM_REF_ID_FRIENDSHIP_POINT: 
		return ERewardType::Social_Point;

	case FItemRefIDHelper::ITEM_REF_ID_GENERAL_LOTTERY_BEGIN: 
		return ERewardType::General_Lottery;

	case FItemRefIDHelper::ITEM_REF_ID_SELECTIVE_LOTTERY_BEGIN: 
		return ERewardType::Selective_Lottery;

	case FItemRefIDHelper::ITEM_REF_ID_SELECTIVE_ITEM_LOTTERY_BEGIN: 
		return ERewardType::Selective_Item_Lottery;

	case FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE:
		return ERewardType::Hero_Piece;

	case FItemRefIDHelper::ITEM_REF_ID_BREVET_STONE:
		return ERewardType::Rank_Promotion_Point;

 	case FItemRefIDHelper::ITEM_REF_ID_ADVANCED_BREVET_STONE:
 		return ERewardType::Advanced_Rank_Promotion_Point;

	case FItemRefIDHelper::ITEM_REF_ID_DARK_CRYSTAL:
		return ERewardType::Dark_Crystal;

	case FItemRefIDHelper::ITEM_REF_ID_FIXED_GRADE_LOTTERY:
		return ERewardType::Fixed_Grade_Lottey;

	case FItemRefIDHelper::ITEM_REF_ID_STAGE_BOOST:
		return ERewardType::Stage_Boost_Ticket;

	case FItemRefIDHelper::ITEM_REF_ID_GOLD_BUFF_TICKET:
		return ERewardType::Hot_Time_Gold_Buff_Ticket;

	case FItemRefIDHelper::ITEM_REF_ID_EXP_BUFF_TICKET:
		return ERewardType::Hot_Time_Exp_Buff_Ticket;

	case FItemRefIDHelper::ITEM_REF_ID_SHOPTENLOTTERY_TICKET:
		return ERewardType::Shop_Ten_Lottery_Ticket;

	case FItemRefIDHelper::ITEM_REF_ID_STAGE_AUTO_CLEAR_TICKET:
		return ERewardType::Stage_Auto_Clear_Ticket;
	}

	return ERewardType::Item;
}

//////////////////////////////////////////////////////////////////////

FText GetLOCTextOfPrimPointType(EItemPrimaryPointType InPointType)
{
//	if (InPointType == EItemPrimaryPointType::EIPP_End)
//	{
//		return FText::FromString(TEXT("")); // 아무것도 표시 안하는 수단으로 사용하기 위해 빈 문장 리턴.
//	}
//
//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemPrimaryPointType"), true);
//	if (!EnumPtr)
//	{		
//#if WITH_EDITOR && !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("EItemPrimaryPointType Enum 패키지를 찾을 수 없다. 컴퓨터가 곧 폭발한다.")));
//#endif
//		return FText::FromString(TEXT("Error"));
//	}
//
//	// Enum name 을 텍스트 테이블의 키로 사용
//	FString EnumName = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InPointType));
//	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("ENUM_") + EnumName);
	return FText::GetEmpty();
}

FText GetLOCTextOfItemOption(EItemOption InOption, EPCClass InPCClass)
{
//	if (InOption == EItemOption::EIO_End)
//	{
//		return FText::FromString(TEXT("")); // 아무것도 표시 안하는 수단으로 사용하기 위해 빈 문장 리턴.
//	}
//
//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemOption"), true);
//	if (!EnumPtr)
//	{
//#if WITH_EDITOR && !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("EItemOption Enum 패키지를 찾을 수 없다. 컴퓨터가 곧 폭발한다.")));
//#endif
//		return FText::FromString(TEXT("Error"));
//	}
//
//	// Enum name 을 텍스트 테이블의 키로 사용..
//	FString TextKeyName = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InOption));
//	// ..하는데 캐릭터 별로 가는 일부 옵션들은 캐릭터 이름을 따로 붙여줌.
//	if (IsItemOptionPerPCClass(InOption))
//	{
//		//TextKeyName += TEXT("_") + UB2PCClassInfo::GetTrimmedNameOfPCClassEnum(InPCClass);
//	}
//
//	TextKeyName = TEXT("ENUM_") + TextKeyName; // 마지막으로 접두어를 좀 붙여준다. ㅋ
//	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TextKeyName);
	return FText::GetEmpty();
}

FText GetLOCTextOfItemQuality(int32 ItemQuality)
{
	if (ItemQuality > 0 && ItemQuality <= MAX_ITEM_QUALITY)
	{ 
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString::Printf(TEXT("ItemQualityPrefix_%d"), ItemQuality));
	}
	// 유효한 품질 범위가 아닌 경우 아이템 풀 네임에 prefix 로 사용하지 못하도록 진작에 이걸 콜하지 않는 게 나을 것. FSingleItemInfoData::GetLocalizedName 참고
	return FText::FromString(TEXT(""));
}

FText GetLOCTextOfSetItemName(int32 GroupID)
{
	static FString SetGroupPrefix(TEXT("SetGroup"));
	return BladeIIGetLOCText(B2LOC_CAT_ITEMNAME, SetGroupPrefix + FString::FromInt(GroupID));
}

FText GetLOCTextOfStageBuffItem(EStageBuffType InStageBuffType)
{
	switch (InStageBuffType)
	{
	case EStageBuffType::SBT_IncreaseExp: return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageBuffItemName_IncExp"));
	case EStageBuffType::SBT_IncreaseGold: return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageBuffItemName_IncGold"));
	case EStageBuffType::SBT_AutoSkill: return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageBuffItemName_AutoSkill"));
	case EStageBuffType::SBT_BoostGameSpeed: return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageBuffItemName_BoostGameSpeed"));
	}
	return FText::FromString(TEXT("Unknown StageBuff"));
}

FText GetLOCTextOfPrimaryPointApplyType(ECharStatApplyType InPrimaryPointApplyType, EItemPrimaryPointType InCheckPointType)
{
//	if (InPrimaryPointApplyType == ECharStatApplyType::ECSA_End)
//	{
//		return FText::FromString(TEXT("")); // 아무것도 표시 안하는 수단으로 사용하기 위해 빈 문장 리턴.
//	}
//
//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECharStatApplyType"), true);
//	if (!EnumPtr)
//	{
//#if WITH_EDITOR && !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("ECharStatApplyType Enum 패키지를 찾을 수 없다. 컴퓨터가 곧 폭발한다.")));
//#endif
//		return FText::FromString(TEXT("Error"));
//	}
//
//	// Enum name 을 텍스트 테이블의 키로 사용
//	FString EnumKey = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InPrimaryPointApplyType));
//	FText EnumString =  FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), (TEXT("ENUM_") + EnumKey)), GetLOCTextOfPrimPointType(InCheckPointType));
//	return (EnumString.IsEmpty() == false) ? EnumString : FText::FromString(EnumKey);
	return FText::FromString(TEXT("Error"));
}

FText GetLOCTextOfPrimaryPointApplyType(ECharStatApplyType InPrimaryPointApplyType, EItemOption InOption, EPCClass InPCClass)
{
//	if (InPrimaryPointApplyType == ECharStatApplyType::ECSA_End || InOption == EItemOption::EIO_End || InPCClass == EPCClass::EPC_End)
//	{
//		return FText::FromString(TEXT("")); // 아무것도 표시 안하는 수단으로 사용하기 위해 빈 문장 리턴.
//	}
//
//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECharStatApplyType"), true);
//	if (!EnumPtr)
//	{
//#if WITH_EDITOR && !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("ECharStatApplyType Enum 패키지를 찾을 수 없다. 컴퓨터가 곧 폭발한다.")));
//#endif
//		return FText::FromString(TEXT("Error"));
//	}
//
//	// Enum name 을 텍스트 테이블의 키로 사용
//	FString EnumKey = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InPrimaryPointApplyType));
//	FText EnumString = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), (TEXT("ENUM_") + EnumKey)), GetLOCTextOfItemOption(InOption, InPCClass));
//	return (EnumString.IsEmpty() == false) ? EnumString : FText::FromString(EnumKey);
	return FText::FromString(TEXT("Error"));
}

FText GetLOCTextOfPrimaryPointApplyType(ECharStatApplyType InPrimaryPointApplyType)
{
	return GetLOCTextOfPrimaryPointApplyType(InPrimaryPointApplyType, EItemPrimaryPointType::EIPP_End);
}

EPCClass ItemAllowedPCClassEnum(int32 InPCClassID)
{
	return IntToPCClass(InPCClassID);
}

/*
	@ItemFactorLevel = 서버 Table에 있는 아이템 등급(1~6성) 별 가중치 - FMD_ItemSingleElem::ItemLevel
	@Quality = 최하급~ 최상급 (1 ~ 4)
	@Level = 강화레벨 1 ~ 40
*/
float GetPreviewPrimaryPoint(EItemPrimaryPointType PrimaryType, float ItemFactorBase, int32 Quality, int32 FactorLevel)
{
	//FClientDataStore& ClientDataStore = BladeIIGameImpl::GetClientDataStore();

	//// 레벨업시 마스터 데이터 기반으로 PrimaryPoint 결정 ( 0.8 Factor 곱하는 수식 사라짐 )
	//const float PrimaryLevelupFactor = ClientDataStore.GetItemLevelupFactor_PrimaryIncrement(FactorLevel);
	//const float ItemLevelForEnhance = ItemFactorBase + PrimaryLevelupFactor;
	//float qualityIncLevel=0;

	//for (int32 i = 0; i < ClientDataStore.MD_LotteryShopRateQualityInfo.Num(); i++)
	//{
	//	if (
	//		(PrimaryType == EItemPrimaryPointType::EIPP_Attack && ClientDataStore.MD_LotteryShopRateQualityInfo[i].equipCategory == EEquipCategoryType::WEAPON) ||
	//		(PrimaryType == EItemPrimaryPointType::EIPP_Defense && ClientDataStore.MD_LotteryShopRateQualityInfo[i].equipCategory == EEquipCategoryType::ARMOR) ||
	//		(PrimaryType == EItemPrimaryPointType::EIPP_Health && ClientDataStore.MD_LotteryShopRateQualityInfo[i].equipCategory == EEquipCategoryType::ACCESSORY)
	//		)
	//	{
	//		if (Quality == ClientDataStore.MD_LotteryShopRateQualityInfo[i].quality)
	//			qualityIncLevel = ClientDataStore.MD_LotteryShopRateQualityInfo[i].qualityIncLevel;
	//	}
	//}

	//// 품질 한단계마다 0.2씩 증가
	//// 품질 변환 업데이트로 품질에 따른 능력치가 눈에 띄게 차이나게 바뀜
	////const float CalculatedItemLevel	 = (0.2f * (Quality - 1)) + ItemLevelForEnhance;
	//const float CalculatedItemLevel = (1 + qualityIncLevel) * ItemLevelForEnhance + 0.2f;

	float ReturnValue = 0.f;
	//if (PrimaryType == EItemPrimaryPointType::EIPP_Attack)
	//{
	//	// 100 + ( 8 * 아이템레벨 ) + ( 0.2 * 아이템레벨  * 아이템레벨 )
	//	float AttackValue = 
	//		(100.f + (8.0f * CalculatedItemLevel) + (0.2f * FMath::Pow(CalculatedItemLevel, 2)));

	//	ReturnValue = AttackValue;
	//}

	//else if (PrimaryType == EItemPrimaryPointType::EIPP_Defense)
	//{
	//	// 공격력 * 3 / 4
	//	float AttackValue =
	//		(100.f + (8.f * CalculatedItemLevel) + (0.2f * FMath::Pow(CalculatedItemLevel, 2)));

	//	AttackValue = FMath::FloorToFloat(AttackValue);

	//	ReturnValue = (AttackValue * 3.0f / 4.0f);
	//}
	//else if (PrimaryType == EItemPrimaryPointType::EIPP_Health)
	//{
	//	// (1 + ( 3 * 아이템레벨 ) + ( 0.05 * 아이템레벨 * 아이템레벨)) / 3
	//	ReturnValue =
	//		((1.0f + (3.0f * CalculatedItemLevel)) + (0.05f * FMath::Pow(CalculatedItemLevel, 2))) / 3.0f;
	//}

	return ReturnValue;
}

void GetStandardDummyPCPartItemData(EPCClass InPCClass, int32 InStarGrade, TArray<FB2Item>& OutData, int32 InGroupID)
{
	if (InPCClass == EPCClass::EPC_End || InStarGrade < 1 || InStarGrade > FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE){
		return;
	}

	/*================================================================================
	개발 버전 기능은 아니지만 매우 특수한 기능임. 
	아이템 RefID 에 따른 속성 맵핑이 좀 들어감.
	================================================================================*/

	const int32 RefIDPCClassNum = (int32)InPCClass; // 이 규약이 바뀐다면 캐릭터 소개 화면에서 엉뚱한 메쉬들의 조합이 나올 테니 알게 되겠지..

	for (int32 ECI = 0; ECI < (int32)EItemClass::EIC_End; ++ECI)
	{
		EItemClass CurrItemClass = static_cast<EItemClass>(ECI);
		
		int32 RefIDItemClassNum = -1; // 이 규약이 바뀌어도 엉뚱한 메쉬들의 조합이 나오게 되겠지..?
		EItemEquipPlace CurrEquipPlace = EItemEquipPlace::EIEP_End;

		// ExtractItemClassFromRefID 랑 GetItemEquipPlaceFromClass 에 해당하는 내용인데
		// 그것과 매칭하는지 아래서 체크
		switch (CurrItemClass)
		{
		case EItemClass::EIC_Weapon: 
			RefIDItemClassNum = 0;
			CurrEquipPlace = EItemEquipPlace::EIEP_Weapon; 
			break;
		case EItemClass::EIC_Helmet: 
			RefIDItemClassNum = 1;
			CurrEquipPlace = EItemEquipPlace::EIEP_Helmet; 
			break;
		case EItemClass::EIC_BodyArmor: 
			RefIDItemClassNum = 2;
			CurrEquipPlace = EItemEquipPlace::EIEP_BodyArmor; 
			break;
		case EItemClass::EIC_Glove: 
			RefIDItemClassNum = 3;
			CurrEquipPlace = EItemEquipPlace::EIEP_Glove; 
			break;
		case EItemClass::EIC_Shoes: 
			RefIDItemClassNum = 4;
			CurrEquipPlace = EItemEquipPlace::EIEP_Shoes; 
			break;

		// 함수 목적 상 메쉬 조합 대상이 아닌 건 제외한다. 벨트, 목걸이, 반지 등
		
		}

		if (CurrEquipPlace != EItemEquipPlace::EIEP_End && RefIDItemClassNum >= 0)
		{
			FB2Item NewItemData;
			// RefID 규약에 따른 조합인데 이것보다는 서버에서 보내주는 속성값이 우선임. 여기선 필요하니까 어쩔 수 없이 사용.
			NewItemData.ItemRefID = 1000000 + RefIDPCClassNum * 100000 + RefIDItemClassNum * 1000 + InStarGrade * 100 + InGroupID;
			// 메쉬 조합에서는 EquipPlace 정도면 될 것. 나머지는 여기 있는 데이터니 여하간 채워줌.
			NewItemData.AllowedPCClass = (int32)InPCClass;
			NewItemData.ItemClass = CurrItemClass;
			NewItemData.EquipPlace = CurrEquipPlace;

			// 조합한 것이 규약에 맞는지 체크.. CheckForItemPropertyRule 을 사용하기 전에 걍 체크에 빠진 데이터는 채워준다.
			NewItemData.StarGrade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(NewItemData.ItemRefID);
			NewItemData.InventoryType = GetItemInvenTypeFromClass(NewItemData.ItemClass);
			BII_CHECK(CheckForItemPropertyRule(NewItemData));

			OutData.Add(NewItemData);
		}
	}
}

bool GetItemIntrinsicOptionOfType(FB2Item& ItemToCheck, FItemOption& OutFoundOption, EItemOption InSearchOption)
{ // InSearchOption 에 해당하는 게 있는지 찾는 용도
	for (FItemOption& CurrOption : ItemToCheck.IntrinsicOptions)
	{
		if (CurrOption.OptionType == InSearchOption)
		{
			OutFoundOption = CurrOption;
			return true;
		}
	}
	return false;
}

bool GetItemRandomOptionOfType(FB2Item& ItemToCheck, FItemOption& OutFoundOption, EItemOption InSearchOption)
{ // InSearchOption 에 해당하는 게 있는지 찾는 용도
	for (FItemOption& CurrOption : ItemToCheck.RandomOptions)
	{
		if (CurrOption.OptionType == InSearchOption)
		{
			OutFoundOption = CurrOption;
			return true;
		}
	}
	return false;
}

bool IsItemOptionPerPCClass(EItemOption InOption)
{
	if (InOption == EItemOption::EIO_Skill_IncSkill1Damage ||
		InOption == EItemOption::EIO_Skill_IncSkill2Damage ||
		InOption == EItemOption::EIO_Skill_IncSkill3Damage ||
		InOption == EItemOption::EIO_Skill_IncSkill5Damage){
		return true; // 실질적으로 플레이어 캐릭터 종류별로 구분되는 것들임.
	}
	return false;
}

float GetConvertItemOptionValue(EItemOption InOption, float InOptionValue, bool bIsRawValue)
{
	ECombatOptionApplyType OptionApplyType = GetCombatOptionApplyType(InOption);
	
	float DisplayValue = InOptionValue;
	if (!bIsRawValue)
	{
		switch (OptionApplyType)
		{
		case ECombatOptionApplyType::ECOAT_OneMinusMultiply:
			DisplayValue = (1.0f - InOptionValue) * 100.0f;
			break;
		case ECombatOptionApplyType::ECOAT_Add_Percent:
			DisplayValue = InOptionValue * 100.0f;
			break;
		default:
			DisplayValue = InOptionValue;
			break;
		}
	}

	return DisplayValue;
}

FText GetItemOptionValueDisplayText(EItemOption InOption, float InOptionValue, bool bIsRawValue)
{
	float DisplayValue = GetConvertItemOptionValue(InOption, InOptionValue, bIsRawValue);

	ECombatOptionApplyType OptionApplyType = GetCombatOptionApplyType(InOption);
	FText UnitText;
	switch (InOption)
	{
	case EItemOption::EIO_Offense_IncAttack: // 걍 수치만 표시되는 애들
	case EItemOption::EIO_Health_IncHealth:
	case EItemOption::EIO_Defense_IncDefense:

	case EItemOption::EIO_Tag_Offense_IncAttack:
	case EItemOption::EIO_Tag_Defense_IncDefense:
	case EItemOption::EIO_Tag_Health_IncHealth:

	case EItemOption::EIO_HeroTower_Offense_IncAttack:
	case EItemOption::EIO_HeroTower_Defense_IncDefense:
	case EItemOption::EIO_HeroTower_Health_IncHealth:

	case EItemOption::EIO_Assault_Offense_IncAttack:
	case EItemOption::EIO_Assault_Defense_IncDefense:
	case EItemOption::EIO_Assault_Health_IncHealth:

	case EItemOption::EIO_Raid_Offense_IncAttack:
	case EItemOption::EIO_Raid_Defense_IncDefense:
	case EItemOption::EIO_Raid_Health_IncHealth:

	case EItemOption::EIO_TeamMatch_Offense_IncAttack:
	case EItemOption::EIO_TeamMatch_Defense_IncDefense:
	case EItemOption::EIO_TeamMatch_Health_IncHealth:

	case EItemOption::EIO_Offense_IncAdditionalDamage:
	case EItemOption::EIO_Defense_DecAdditionalDamage:

	case EItemOption::EIO_Guild_Offense_IncAttack:
	case EItemOption::EIO_Guild_Defense_IncDefense:
	case EItemOption::EIO_Guild_Health_IncHealth:

		DisplayValue = (InOptionValue >= 0 ? FMath::CeilToFloat(DisplayValue) : FMath::FloorToFloat(DisplayValue));
		checkSlow(OptionApplyType == ECombatOptionApplyType::ECOAT_Add_Value); // 지금까지 그렇다는 거고 이 가정이 틀릴 수도 있다. 아닌 경우가 발생하면 껀바이껀 대처.
		break;

	case EItemOption::EIO_Health_PerSecRecoverHP:
		checkSlow(OptionApplyType == ECombatOptionApplyType::ECOAT_Add_Value); // 지금까지 그렇다는 거고 이 가정이 틀릴 수도 있다. 아닌 경우가 발생하면 껀바이껀 대처.
		break;
		
	case EItemOption::EIO_Skill_DecSkillCooltime: // 시간 관련. 초단위 표시
	case EItemOption::EIO_Misc_DecTagCooltime:
		UnitText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TimeUnitSecond"));
		break;

	// 기타 나머지는 % 로
	default:
		// OptionApplyType 이 Percent 속성인 경우인데 혹시라도 아닌 경우가 발생하면 껀바이껀 대처.
		checkSlow(OptionApplyType == ECombatOptionApplyType::ECOAT_Add_Percent || OptionApplyType == ECombatOptionApplyType::ECOAT_OneMinusMultiply);
		UnitText = FText::FromString(TEXT("%"));
		break;
	}

	// 소수점 둘째자리까지 허용하되 끝자리 0 은 유효숫자로 안 치고 생략.
	return FText::Format(FText::FromString(TEXT("{0} {1}")), GetAutoFractionalFormattedText(DisplayValue, 2), UnitText);
}

FText GetSkillOptionValueDisplayText(ESkillOption InOption, float InOptionValue)
{
	ECombatOptionApplyType OptionApplyType = GetCombatOptionApplyType(InOption);
	// EffectiveOptionValue 는 GetEffectiveItemOptionValue 에서 얻은 걸로 친다.
	FText TempDisPlayText;
	FText FinalDisplayText;

	// 소수점 둘째자리까지 허용하되 끝자리 0 은 유효숫자로 안 치고 생략.
	if (InOptionValue > 0)
		TempDisPlayText = FText::Format(FText::FromString(TEXT("{0} {1}")), FText::FromString("+"), GetAutoFractionalFormattedText(InOptionValue, 2));
	else
		TempDisPlayText = GetAutoFractionalFormattedText(InOptionValue, 2);


	switch (InOption)
	{

	// 초를 붙이는 애들
	case ESkillOption::ESO_DecCoolTime:
		FinalDisplayText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Sconds")), TempDisPlayText);
		break;
	// 아무것도 없는애들
	case ESkillOption::ESO_IncMotion:
	case ESkillOption::ESO_DecCoolTimeOtherSkills:
	case ESkillOption::ESO_BuffTime:
	case ESkillOption::ESO_IncMaxNumCharacterBuff:
	case ESkillOption::ESO_HealHPByCountAttack:
	case ESkillOption::ESO_IncAttackDamageToGladiator:
	case ESkillOption::ESO_IncAttackDamageToAssassin:
	case ESkillOption::ESO_IncAttackDamageToWizard:
	case ESkillOption::ESO_IncAttackDamageToFighter:
	case ESkillOption::ESO_IncMaxHP:
	case ESkillOption::ESO_IncResistAbnormal:
	case ESkillOption::ESO_DecReceiveDamageByGladiator:
	case ESkillOption::ESO_DecReceiveDamageByAssassin:
	case ESkillOption::ESO_DecReceiveDamageByWizard:
	case ESkillOption::ESO_DecReceiveDamageByFighter:
	case ESkillOption::ESO_EnableTagOnKnockBack:
	case ESkillOption::ESO_EnableTagOnBigBounce:
		FinalDisplayText = FText::FromString("");
		break;
	// %를 붙이는 애들
	default:
		FinalDisplayText = FText::Format(FText::FromString(TEXT("{0} {1}")), TempDisPlayText,FText::FromString("%"));
		break;
	}

	return FinalDisplayText;
}

float GetEffectiveItemOptionValue(EItemOption InOptionType, const FB2Item& ItemToCheckOption, const double InEnhanceEffectFactor /*= 1.f*/)
{
	TArray<FOptionValue> OptionRawValues;
	for (const FItemOption& CurrOption : ItemToCheckOption.IntrinsicOptions) // 아마도 단일 리스트에서 같은 타입 옵션이 두개 이상 안 나오겠지..?
	{
		if (CurrOption.OptionType == InOptionType){
			FOptionValue OptionValue = FOptionValue(CurrOption.RawOptionAmount);
			OptionRawValues.Add(OptionValue);
		}
	}
	for (const FItemOption& CurrOption : ItemToCheckOption.RandomOptions)
	{
		if (CurrOption.OptionType == InOptionType){
			FOptionValue OptionValue = FOptionValue(CurrOption.RawOptionAmount);
			OptionRawValues.Add(OptionValue);
		}
	}
	
	// 강화 아이템 세트 효과를 고려하는 버전으로 콜
	return GetEffectiveItemOptionValueWithEnhanceSetEffect(InOptionType, OptionRawValues, InEnhanceEffectFactor);
}

void GenerateGoldItem(FB2Item& OutItem, int32 InGoldAmount)
{
	OutItem.ItemRefID = FItemRefIDHelper::GetGoodsID_Gold();
	OutItem.ItemClass = EItemClass::EIC_Gold;
	OutItem.InventoryType = EItemInvenType::EIIVT_Consumables;
	OutItem.ConsumingAmount = InGoldAmount;
	OutItem.SetPrimaryPoint(InGoldAmount); // 예전에 PrimaryPoint 로 쓴 게 있어서..
}

bool IsEquipmentItemByType(const FB2Item& InCheckItem)
{
	return (InCheckItem.ItemClass == EItemClass::EIC_Weapon ||
		InCheckItem.ItemClass == EItemClass::EIC_Helmet ||
		InCheckItem.ItemClass == EItemClass::EIC_BodyArmor ||
		InCheckItem.ItemClass == EItemClass::EIC_Glove ||
		InCheckItem.ItemClass == EItemClass::EIC_Shoes ||
		InCheckItem.ItemClass == EItemClass::EIC_Belt ||
		InCheckItem.ItemClass == EItemClass::EIC_Necklace ||
		InCheckItem.ItemClass == EItemClass::EIC_Ring
		);
}

bool IsEquipmentItem(const FB2Item& InCheckItem)
{
	const bool bRetVal = IsEquipmentItemByType(InCheckItem)
		// 보상 대표 아이템의 경우 ItemClass 는 위에 검사하는 것중 하나지만 앞자리는 9번대가 나옴. 이걸 어떻게 처러해야 할까.. 일단은 false 인 걸로.
		&& FItemRefIDHelper::IsEquipmentItemByRefID(InCheckItem.ItemRefID);

	return bRetVal;
}

bool IsEssenceItem(const FB2Item& InCheckItem)
{
	const bool bRetVal = IsEssenceItemClass(InCheckItem.ItemClass);
#if !UE_BUILD_SHIPPING
	if (bRetVal){
		checkSlow(FItemRefIDHelper::IsEssenceItemByRefID(InCheckItem.ItemRefID));
	}
#endif
	return bRetVal;
}

bool IsAnvilItem(const FB2Item& InCheckItem)
{
	const bool bRetVal = IsAnvilItemClass(InCheckItem.ItemClass);
#if !UE_BUILD_SHIPPING
	if (bRetVal) {
		checkSlow(FItemRefIDHelper::IsAnvilItemByRefID(InCheckItem.ItemRefID));
	}
#endif
	return bRetVal;
}

bool IsGoldItem(const FB2Item& InCheckItem)
{
	return InCheckItem.ItemRefID == FItemRefIDHelper::GetGoodsID_Gold();
}

bool IsBossPieceItem(const FB2Item& InCheckItem)
{	
	return InCheckItem.ItemClass == EItemClass::EIC_BossPiece;
}

bool IsEtherItem(const FB2Item& InCheckItem)
{
	return InCheckItem.ItemClass == EItemClass::EIC_Ether;
}

bool IsMaterialItem(const FB2Item& InCheckItem)
{
	return InCheckItem.InventoryType == EItemInvenType::EIIVT_Consumables && InCheckItem.ItemClass != EItemClass::EIC_BossPiece;
}

bool IsCostumeItem(const FB2Item& InCheckItem)
{
	return InCheckItem.InventoryType == EItemInvenType::EIIVT_CostumeWeapon || InCheckItem.InventoryType == EItemInvenType::EIIVT_CostumeArmor;
}

bool IsItemLevelupAllowed(const FB2Item& InItem)
{// 레벨업은 다른 게 안될 경우 가장 기본.
	// 필요에 따라 B2mdItemInfo 의 is_levelupable 을 클라로 가져와서 추가 체크.
	return (IsEquipmentItem(InItem) && !IsItemCompositionAllowed(InItem) && !IsItemUpgradeAllowed(InItem) && !IsItemSurpassAllowed(InItem));
}

bool IsItemCompositionAllowed(const FB2Item& InItem)
{ // 합성, 승급은 동일 조건에서의 선택. 각 등급 별 최고 레벨에서. 
  // 필요에 따라 B2mdItemInfo 의 is_composable 을 클라로 가져와서 추가 체크.
	return (IsEquipmentItem(InItem) && InItem.Level >= InItem.MaxLevel && !IsItemSurpassAllowed(InItem));
}

bool IsItemUpgradeAllowed(const FB2Item& InItem)
{ // 합성, 승급은 동일 조건에서의 선택. 각 등급 별 최고 레벨에서
  // 필요에 따라 B2mdItemInfo 의 is_upgradable 을 클라로 가져와서 추가 체크.
	return (IsEquipmentItem(InItem) && InItem.Level >= InItem.MaxLevel && !IsItemSurpassAllowed(InItem));
}

bool IsItemSurpassAllowed(const FB2Item& InItem)
{ // 최종 레벨에서 초월이 허용된다.
  // 필요에 따라 B2mdItemInfo 의 is_surpassable 을 클라로 가져와서 추가 체크.
	return (IsEquipmentItem(InItem) && InItem.StarGrade >= FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE && InItem.Level >= InItem.MaxLevel);
}

bool IsItemEnhanceAllowed(const FB2Item& InItem)
{
	return (IsEquipmentItem(InItem) && InItem.EnhanceLevel < MAX_ENHANCE_ITEM_LEVEL);
}

// 초월이 6성에서 7성으로 늘어남에 따라(앞으로도 8성,9성)
// 6성에서는 합성과 승급 초월이 한번에 보여지는 일이 생김
// 그 등급 구간 체크 (현재 6성)
bool IsItemAllowedCompositeAndSurpass(const FB2Item& InItem)
{
	return (IsEquipmentItem(InItem) && InItem.Level >= 30 &&
		InItem.StarGrade >= FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE && InItem.StarGrade < FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE);
}

bool IsItemSetEffectAvailable(const FB2Item& InItem)
{
	if (InItem.InventoryType == EItemInvenType::EIIVT_Accessory || InItem.InventoryType == EItemInvenType::EIIVT_Protection)
		return true;

	if (InItem.InventoryType == EItemInvenType::EIIVT_CostumeWeapon || InItem.InventoryType == EItemInvenType::EIIVT_CostumeArmor)
		return true;

	return false;
}

bool IsItemSafeEnhanceLevel(const FB2Item & InItem)
{
	bool IsSafe = false;

	if (InItem.EnhanceLevel != 0)
	{
		if (InItem.EnhanceLevel == 10 ||
			InItem.EnhanceLevel == 15 ||
			InItem.EnhanceLevel == 20 ||
			InItem.EnhanceLevel == 25 ||
			InItem.EnhanceLevel == 30 ||
			InItem.EnhanceLevel == 40 ||
			InItem.EnhanceLevel == 50 ||
			InItem.EnhanceLevel == 60)
		{
			IsSafe = true;
		}
	}

	return IsSafe;
}

bool IsItemDecompostionAllowed(const FB2Item& InCheckItem)
{
	//FItemDismantleInfo DismantleInfo;
	//FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	//bool bEnableDismantle = CliDataStore.GetItemDismantleInfoData(InCheckItem.InventoryType, InCheckItem.StarGrade, DismantleInfo);

	//// 보상 대표 아이템의 경우 ItemClass 는 위에 검사하는 것중 하나지만 앞자리는 9번대가 나옴. 이걸 어떻게 처러해야 할까.. 일단은 false 인 걸로.
	//const bool bRetVal = IsEquipmentItemByType(InCheckItem) && 
	//	FItemRefIDHelper::IsEquipmentItemByRefID(InCheckItem.ItemRefID) && 
	//	bEnableDismantle;

	//return bRetVal;
	return true;
}

int32 GetLevel_PreviewItemLevelup(float& OutNewExp, const FB2Item& TargetItem, const TArray<FB2Item>& ItemLevelupIngredients, bool bClampResultToMaxValue)
{
	//FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();

	//float TotalFactor = 0.0f;

	//float TargetFactor = CliDataStore.GetItemLevelupFactor_TargetItemGrade(TargetItem.StarGrade);
	//
	//for (int32 II = 0; II < ItemLevelupIngredients.Num(); ++II)
	//{
	//	const FB2Item& ThisIngred = ItemLevelupIngredients[II];

	//	float IngredFactor1 = CliDataStore.GetItemLevelupFactor_IngredItemGrade(ThisIngred.StarGrade);
	//	float IngredFactor2 = CliDataStore.GetItemLevelupFactor_IngredItemEquipPlace(ThisIngred.EquipPlace); // 정수 아이템도 EquipPlace 가 있다.
	//	float IngredFactor3 = CliDataStore.GetItemLevelupFactor_IngredItemEnhLv(ThisIngred.Level);
	//	float IngredFactor4 = CliDataStore.GetItemLevelupFactor_IngredItemSurpCount(ThisIngred.SurpassCount);
	//	// 매 아이템 레벨업 재료마다 TargetFactor 도 곱한다.
	//	float ThisIngredTotalFactor = TargetFactor * IngredFactor1 * IngredFactor2 * IngredFactor3 * IngredFactor4;

	//	if (IsEssenceItem(ThisIngred)) { // 정수 아이템은 절반 효율.
	//		ThisIngredTotalFactor *= 0.5f;
	//	}

	//	TotalFactor += ThisIngredTotalFactor; // 최종적으로는 매 재료에 대한 factor 계산값들을 더함.
	//}

	//// 일단 레벨이 얼마나 오를지 신경쓰지 않고 경험치에 단순히 더한다.
	//const float SimpleAddedExp = TargetItem.Exp + TotalFactor;

	//const int32 SimpleAddedExp_Floor = FMath::FloorToInt(SimpleAddedExp);
	////const float SimpleAddedExp_Frac = FMath::Frac(SimpleAddedExp); 이건 필요없을 듯.

	//// 레벨 상승은 앞자리 수만큼.
	//const int32 LevelToIncrease = SimpleAddedExp_Floor / 100;
	//int32 RetLevel = TargetItem.Level + LevelToIncrease; // 이 시점에서는 MaxLevel 로 클램프되지 않은 값

	//// 전체 경험치 상승분에서 두자릿수 이하만 남김.
	//OutNewExp = SimpleAddedExp - ((float)LevelToIncrease * 100.0f); // 여기서도 정밀도 문제가 될 수 있나..

	//if (bClampResultToMaxValue && RetLevel >= TargetItem.MaxLevel)
	//{
	//	RetLevel = TargetItem.MaxLevel;
	//	OutNewExp = 100.0f;
	//}

	//return RetLevel;
	return 0.0;
}

/** 레벨, 경험치 등 기본적인 값들 채워넣으면 실제 결과 수치라 할 수 있는 옵션값들 채워넣는 함수. 프리뷰 용. */
static void SetPreviewItemResultOptionValuesInternal(const FB2Item& InOriginItem, FB2Item& InOutPreviewItem)
{ // 결과 옵션값들 외에 여기서 사용하는 기본적인 값들은 채워져 있어야 한다. 


	//const float PrimaryPoint = GetPreviewPrimaryPoint(InOutPreviewItem.PrimaryPointType,
	//												  InOutPreviewItem.ItemFactorLevel,
	//												  InOutPreviewItem.Quality,
	//												  InOutPreviewItem.Level);

	//InOutPreviewItem.SetPrimaryPoint(PrimaryPoint);


	//FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	//int32 LevelDistance = (int32)((InOutPreviewItem.Level - InOriginItem.Level + (InOriginItem.Level % 10) ) / 10);
	//// 기타 옵션값들..
	//for (int32 IOI = 0; IOI < InOutPreviewItem.IntrinsicOptions.Num(); ++IOI)
	//{
	//	FItemOption& ThisOption = InOutPreviewItem.IntrinsicOptions[IOI];
	//	ThisOption.RawOptionAmount = ThisOption.RawOptionAmount + CliDataStore.GetItemLevelupFactor_PerOptionInc(ThisOption.OptionType) * (float)(LevelDistance);
	//}
	//for (int32 IOI = 0; IOI < InOutPreviewItem.RandomOptions.Num(); ++IOI)
	//{
	//	FItemOption& ThisOption = InOutPreviewItem.RandomOptions[IOI];
	//	ThisOption.RawOptionAmount = ThisOption.RawOptionAmount + CliDataStore.GetItemLevelupFactor_PerOptionInc(ThisOption.OptionType) * (float)(LevelDistance);
	//}
}

/** 지정한 재료들을 가지고 레벨업 시 결과 데이터가 어떻게 되는지 프리뷰 */
void GetCalculateItem_PreviewItemLevelup(const FB2Item& InTargetItem, const TArray<FB2Item>& ItemLevelupIngredients, FB2Item& OutPreviewResult, bool bClampResultToMaxValue)
{
	// Note(주의!) : 현재 계산 공식은 클라에서 UI상으로 보여주기위한 용도로만 사용하고있고, 실제 공격력 계산은 서버에서 해서 내려주고있음. 
	//OutPreviewResult = InTargetItem;

	//if (ItemLevelupIngredients.Num() == 0)
	//	return;

	//float NewExp = InTargetItem.Exp;
	//OutPreviewResult.Level = GetLevel_PreviewItemLevelup(NewExp, InTargetItem, ItemLevelupIngredients, bClampResultToMaxValue);
	//OutPreviewResult.Exp = NewExp;

	//SetPreviewItemResultOptionValuesInternal(InTargetItem, OutPreviewResult); // 기본적인 데이터를 넣은 후 결과 옵션 수치들을 산출해 낸다.

	//// 강화가 적용 안된 기본 공방력을 뽑아내고 그 값을 기준으로 강화값을 증가해준다.
	//int32 BasePrimaryPoint = OutPreviewResult.GetPrimaryPoint();
	//int32 EnhanceLevel = InTargetItem.EnhanceLevel;

	//switch (OutPreviewResult.PrimaryPointType)
	//{
	//	// 공격력 += 공격력 * ((등차계수 * 강화레벨) + (등비계수 * 강화레벨 * 강화레벨))
	//	case EItemPrimaryPointType::EIPP_Attack:
	//	case EItemPrimaryPointType::EIPP_Defense:
	//	{
	//		BasePrimaryPoint += (BasePrimaryPoint * ((0.02f * EnhanceLevel) + (0.00065f * FMath::Pow(EnhanceLevel, 2))));
	//		OutPreviewResult.SetPrimaryPoint(B2ServerRoundDown(BasePrimaryPoint, 1.f));
	//	}
	//		break;
	//	// 공격력 += (공격력 * ((등차계수 * 강화레벨) + (등비계수 * 강화레벨 * 강화레벨))) + 1
	//	case EItemPrimaryPointType::EIPP_Health:
	//	{
	//		BasePrimaryPoint += ((BasePrimaryPoint * ((0.02f * EnhanceLevel) + (0.00065f * FMath::Pow(EnhanceLevel, 2)))) + 1);
	//		OutPreviewResult.SetPrimaryPoint(B2ServerRoundDown(BasePrimaryPoint, 1.f));
	//	}
	//		break;
	//}
}

/** 특정 수치만큼 레벨업 시 결과 데이터가 어떻게 되는지 프리뷰 */
void GetCalculateItem_PreviewItemLevelup(const FB2Item& InTargetItem, FB2Item& OutPreviewResult, int32 LevelIncreaseAmount /*= 0*/) 
{
	// Note(주의!) : 현재 계산 공식은 클라에서 UI상으로 보여주기위한 용도로만 사용하고있고, 실제 공격력 계산은 서버에서 해서 내려주고있음. 
	OutPreviewResult = InTargetItem;
	OutPreviewResult.Level += LevelIncreaseAmount;

	SetPreviewItemResultOptionValuesInternal(InTargetItem, OutPreviewResult); // 기본적인 데이터를 넣은 후 결과 옵션 수치들을 산출해 낸다.
}

int GetTotalCost_PreviewItemLevelup(const FB2Item& InTargetItem, const TArray<FB2Item>& ItemLevelupIngredients)
{
	int32 LevelUpCost = 0;
	//FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	//for (const FB2Item &Each : ItemLevelupIngredients)
	//{
	//	LevelUpCost += DataStore.GetItemLevelUpCost(Each.InventoryType, Each.StarGrade, Each.Level);
	//}
	return LevelUpCost;
}

void GetCalculateItem_PreviewItemEnhance(const FB2Item& InTargetItem, FB2Item& OutPreviewResult, int32 IncreaseEnhanceLevel /*= 0*/, float manualFactor)
{
	//OutPreviewResult = InTargetItem;
	//if (manualFactor != 0)
	//{
	//	OutPreviewResult.ItemFactorLevel = manualFactor;
	//}
	//FB2Item CalcLevelUpItem;
	//GetCalculateItem_PreviewItemLevelup(OutPreviewResult, CalcLevelUpItem);
	//
	//// 강화가 적용 안된 기본 공방력을 뽑아내고 그 값을 기준으로 강화값을 증가해준다.
	//int32 BasePrimaryPoint = CalcLevelUpItem.GetPrimaryPoint();
	//int32 EnhanceLevel = InTargetItem.EnhanceLevel + IncreaseEnhanceLevel;

	//switch (CalcLevelUpItem.PrimaryPointType)
	//{
	//	// 공격력 += 공격력 * ((등차계수 * 강화레벨) + (등비계수 * 강화레벨 * 강화레벨))
	//case EItemPrimaryPointType::EIPP_Attack:
	//case EItemPrimaryPointType::EIPP_Defense:
	//	{
	//		BasePrimaryPoint += (BasePrimaryPoint * ((0.02f * EnhanceLevel) + (0.00065f * FMath::Pow(EnhanceLevel, 2))));
	//		OutPreviewResult.SetPrimaryPoint(B2ServerRoundDown(BasePrimaryPoint, 1.f));
	//	}
	//	break;
	//	// 공격력 += (공격력 * ((등차계수 * 강화레벨) + (등비계수 * 강화레벨 * 강화레벨))) + 1
	//case EItemPrimaryPointType::EIPP_Health:
	//	{
	//		BasePrimaryPoint += ((BasePrimaryPoint * ((0.02f * EnhanceLevel) + (0.00065f * FMath::Pow(EnhanceLevel, 2)))) + 1);
	//		OutPreviewResult.SetPrimaryPoint(B2ServerRoundDown(BasePrimaryPoint, 1.f));
	//	}
	//	break;
	//}
}

void GetCalculateCostumeItem_PreviewItemEnhance(const FB2Item& InTargetItem, FB2Item& OutPreviewResult, int32 IncreaseEnhanceLevel /*= 0*/)
{
	//OutPreviewResult = InTargetItem;

	//FB2Item CalcLevelUpItem;
	//GetCalculateItem_PreviewItemLevelup(InTargetItem, CalcLevelUpItem);

	//// 강화가 적용 안된 기본 공방력을 뽑아내고 그 값을 기준으로 강화값을 증가해준다.
	//int32 BasePrimaryPoint = CalcLevelUpItem.GetPrimaryPoint();
	//int32 EnhanceLevel = InTargetItem.EnhanceLevel + IncreaseEnhanceLevel;

	//switch (CalcLevelUpItem.PrimaryPointType)
	//{
	//// 공격력 = 무기 기본 공격력 + 무기강화공격력(200+0.5*기본 공격력*강화 레벨+50*강화 레벨*강화 레벨)
	//case EItemPrimaryPointType::EIPP_Attack:
	//{
	//	BasePrimaryPoint += (200 + (0.5 * BasePrimaryPoint * EnhanceLevel) + (50*EnhanceLevel*EnhanceLevel));
	//	OutPreviewResult.SetPrimaryPoint(B2ServerRoundDown(BasePrimaryPoint, 1.f));
	//}
	//break;
	//// 방어력 =  방어구 기본 방어력 + 방어구강화방어력(400+0.5*기본 방어력*강화 레벨+100*강화 레벨*강화 레벨)
	//case EItemPrimaryPointType::EIPP_Defense:
	//{
	//	BasePrimaryPoint += (400 + (0.5 * BasePrimaryPoint * EnhanceLevel) + (100 * EnhanceLevel*EnhanceLevel));
	//	OutPreviewResult.SetPrimaryPoint(B2ServerRoundDown(BasePrimaryPoint, 1.f));
	//}
	//break;
	//case EItemPrimaryPointType::EIPP_Health:
	//break;
	//}
}


/*================================================================================
	아이템의 RefID 자릿수 정보처럼 특정 속성을 가지고 다른 속성을 유추하는 함수.
	FItemRefIDHelper 의 Extract** 씨리즈처럼 꼭 필요한 경우만.
================================================================================*/

EItemInvenType GetItemInvenTypeFromClass(EItemClass InItemClass)
{
	if (InItemClass == EItemClass::EIC_End)
	{
		return EItemInvenType::EIIVT_End;
	}

	switch (InItemClass)
	{
	case EItemClass::EIC_Weapon:
	case EItemClass::EIC_WeaponEssence:
	case EItemClass::EIC_WeaponSurpassAnvil:
		return EItemInvenType::EIIVT_Weapon;

	case EItemClass::EIC_Helmet:
	case EItemClass::EIC_BodyArmor:
	case EItemClass::EIC_Glove:
	case EItemClass::EIC_Shoes:
	case EItemClass::EIC_ProtectionEssence:
	case EItemClass::EIC_ArmorSurpassAnvil:
		return EItemInvenType::EIIVT_Protection;

	case EItemClass::EIC_Belt:
	case EItemClass::EIC_Necklace:
	case EItemClass::EIC_Ring:
	case EItemClass::EIC_AccessoryEssence:
	case EItemClass::EIC_AccessorySurpassAnvil:
		return EItemInvenType::EIIVT_Accessory;

	case EItemClass::EIC_CostumeWeapon:
		return EItemInvenType::EIIVT_CostumeWeapon;

	case EItemClass::EIC_CostumeHat:
	case EItemClass::EIC_CostumeArmor:
		return EItemInvenType::EIIVT_CostumeArmor;
	}

	return EItemInvenType::EIIVT_Consumables; // 나머지는 소모품으로
}

EItemEquipPlace GetItemEquipPlaceFromClass(EItemClass InItemClass)
{
	switch (InItemClass)
	{
		// 거의 일대일 맵핑
	case EItemClass::EIC_Weapon: return EItemEquipPlace::EIEP_Weapon;

	case EItemClass::EIC_Helmet: return EItemEquipPlace::EIEP_Helmet;
	case EItemClass::EIC_BodyArmor: return EItemEquipPlace::EIEP_BodyArmor;
	case EItemClass::EIC_Glove: return EItemEquipPlace::EIEP_Glove;
	case EItemClass::EIC_Shoes: return EItemEquipPlace::EIEP_Shoes;

	case EItemClass::EIC_Belt: return EItemEquipPlace::EIEP_Belt;
	case EItemClass::EIC_Necklace: return EItemEquipPlace::EIEP_Necklace;
	case EItemClass::EIC_Ring: return EItemEquipPlace::EIEP_Ring;
	}

	return EItemEquipPlace::EIEP_End; // 나머지는 장착 불가능
}

ECostumeEquipPlace GetItemCostumeEquipPlaceFromClass(EItemClass InItemClass)
{
	switch (InItemClass)
	{
	case EItemClass::EIC_CostumeWeapon: return ECostumeEquipPlace::ECEP_Weapon;
	case EItemClass::EIC_CostumeHat:	return ECostumeEquipPlace::ECEP_Helmet;
	case EItemClass::EIC_CostumeArmor:	return ECostumeEquipPlace::ECEP_Body;
	}

	return ECostumeEquipPlace::ECEP_None;
}

bool CheckForItemPropertyRule(const FB2Item& InItemToCheck)
{
	if (FItemRefIDHelper::IsEquipmentItemByRefID(InItemToCheck.ItemRefID)) // 앞자리 1 이 일반적인 장착 아이템. 현재 룰 체크 대상
	{
		bool bFoundMisMatch = false;
		bFoundMisMatch |= (InItemToCheck.ItemClass != FItemRefIDHelper::ExtractItemClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.AllowedPCClass != (int32)FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.StarGrade != FItemRefIDHelper::ExtractItemStarGradeFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.InventoryType != GetItemInvenTypeFromClass(InItemToCheck.ItemClass));
		bFoundMisMatch |= (InItemToCheck.EquipPlace != GetItemEquipPlaceFromClass(InItemToCheck.ItemClass));
		
		return !bFoundMisMatch;
	}
	else if (FItemRefIDHelper::IsEssenceItemByRefID(InItemToCheck.ItemRefID)) // 정수 아이템도 비슷한 체크
	{
		bool bFoundMisMatch = false;
		bFoundMisMatch |= (InItemToCheck.ItemClass != FItemRefIDHelper::ExtractItemClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.AllowedPCClass != (int32)FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.StarGrade != FItemRefIDHelper::ExtractItemStarGradeFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.InventoryType != GetItemInvenTypeFromClass(InItemToCheck.ItemClass));

		return !bFoundMisMatch;
	}
	else if (FItemRefIDHelper::IsCostumeItemByRefID(InItemToCheck.ItemRefID)) // 앞자리 3인 코스튬 아이템
	{
		bool bFoundMisMatch = false;
		bFoundMisMatch |= (InItemToCheck.ItemClass			!= FItemRefIDHelper::ExtractItemClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.AllowedPCClass		!= (int32)FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.StarGrade			!= FItemRefIDHelper::ExtractItemStarGradeFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.InventoryType		!= GetItemInvenTypeFromClass(InItemToCheck.ItemClass));
		bFoundMisMatch |= (InItemToCheck.CostumeEquipPlace	!= GetItemCostumeEquipPlaceFromClass(InItemToCheck.ItemClass));
		return !bFoundMisMatch;
	}
	else if (FItemRefIDHelper::IsAnvilItemByRefID(InItemToCheck.ItemRefID)) // 초월 아이템도 비슷한 체크
	{
		bool bFoundMisMatch = false;
		bFoundMisMatch |= (InItemToCheck.ItemClass != FItemRefIDHelper::ExtractItemClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.AllowedPCClass != (int32)FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.StarGrade != FItemRefIDHelper::ExtractItemStarGradeFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.InventoryType != GetItemInvenTypeFromClass(InItemToCheck.ItemClass));

		return !bFoundMisMatch;
	}

	return true;
}


/*
	SetUniqueKey

	|     GroupId     | | Class | | Grade |
	0000 0000 0000 0011 0000 0001 0000 0001
*/
int32 GetSetItemUniqueKey(int32 SetGroupID, EPCClass CharacterClass, int32 Grade)
{
	return (SetGroupID << 16) + (PCClassToInt(CharacterClass) << 8) + Grade;;
}

int32 GetSetItemUniqueKey(int32 SetGroupID, int32 ItemTemplateID)
{
	EPCClass CharacterClass = FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(ItemTemplateID);
	int32 Grade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(ItemTemplateID);

	return GetSetItemUniqueKey(SetGroupID, CharacterClass, Grade);
}

int32 GetSetGroupIDFromUniqueKey(int32 SetUniqueKey)
{
	return SetUniqueKey >> 16;
}

int32 GetSetGradeFromUniqueKey(int32 SetUniqueKey)
{
	return 0xff & SetUniqueKey;
}

EPCClass GetSetCharacterClassFromUniqueKey(int32 SetUniqueKey)
{
	return IntToPCClass((0xff00 & SetUniqueKey) >> 8);
}

EItemInvenType GetSetOptionInvenType(int32 SetOptionID)
{
	int32 InvType = ExtractNthDigitR<3>(SetOptionID);
	if (InvType == 1)
		return EItemInvenType::EIIVT_Protection;
	else if (InvType == 5 || InvType == 6)
		return EItemInvenType::EIIVT_Accessory;

	return EItemInvenType::EIIVT_End;
}

//================================================================================

#if !UE_BUILD_SHIPPING
void DevOnlyLoadItemClientTestData(FB2Item& InItemToLoad)
{
	// RefID 는 사전에 정해져 있어야 함.
	
	InItemToLoad.StarGrade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(InItemToLoad.ItemRefID);
	InItemToLoad.AllowedPCClass = (int32)FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(InItemToLoad.ItemRefID);

	if (FItemRefIDHelper::IsEquipmentItemByRefID(InItemToLoad.ItemRefID))
	{
		InItemToLoad.ItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(InItemToLoad.ItemRefID);
		InItemToLoad.MaxLevel = 30; // 걍 충분히 높게 줌
		InItemToLoad.InventoryType = GetItemInvenTypeFromClass(InItemToLoad.ItemClass);
		InItemToLoad.EquipPlace = GetItemEquipPlaceFromClass(InItemToLoad.ItemClass);
	}
	else if (FItemRefIDHelper::IsEssenceItemByRefID(InItemToLoad.ItemRefID))
	{ // 정수 아이템 Class 확정 전 코드. 어차피 이 함수 자체가 개발용.
		InItemToLoad.ItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(InItemToLoad.ItemRefID);
		InItemToLoad.InventoryType = GetItemInvenTypeFromClass(InItemToLoad.ItemClass); 
	}
	else if (FItemRefIDHelper::IsAnvilItemByRefID(InItemToLoad.ItemRefID))
	{ // 정수 아이템 Class 확정 전 코드. 어차피 이 함수 자체가 개발용.
		InItemToLoad.ItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(InItemToLoad.ItemRefID);
		InItemToLoad.InventoryType = GetItemInvenTypeFromClass(InItemToLoad.ItemClass);
	}

	InItemToLoad.BuyingPrice = InItemToLoad.StarGrade * 1000 + FMath::RandRange(0, 100);
	InItemToLoad.SellingPrice = InItemToLoad.BuyingPrice;
}
#endif

////////////////////////////////////////////////////////////////////////////////

UB2Inventory::UB2Inventory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

int32 UB2Inventory::GetInvenPCIndexFromPCClass(EPCClass InPCClass)
{
	if (InPCClass != EPCClass::EPC_End)
	{
		return PCClassToInt(InPCClass);
	}

	return GetMaxPCClassNum(); // 물론 예외 처리 의미임. 이걸 받게 되면 PCItems 인덱스로 쓰면 안됨.
}
