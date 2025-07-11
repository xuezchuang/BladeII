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
	
	// ���� �̰ɷ� ���� �����ٰ� Option ���� ���� ���� ��ġ�� ���� ����� �Ŷ�� bCountRelevantOption �� false �� �־�� �� ��.
	// ���������� ���� ��Ȳ���� �ɼǱ��� ����Ѵٰ� �ص� �Ϻ��ϰ� �ϱ�� ����� ��. �׳� �ܼ��ϰ� PrimaryPoint �� üũ�ϴ� ������ ���°� ���� �� �ִ�.
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
	// �������� ���� ������ ��� ������ ����
	//InstanceUID = rhs->id;
	//ItemRefID = rhs->template_id;
	//
	//StarGrade = rhs->grade;
	//Quality = rhs->quality;
	//ConsumingAmount = rhs->amount;

	//AllowedPCClass = (int32)SvrToCliPCClassType(rhs->character_type);

	//InventoryType = SvrToCliItemInvenType(rhs->inventory_type);
	//EquipPlace = SvrToCliItemEquipPlace(rhs->equip_position); // MasterData ���� �ٽ� �����ϴµ�.. ������ ���� equip_position �� ������ �����۸� ���� ���õǾ� �ִ� �͵� ���� ��ư..

	//// ��� ���� �����Ǿ� �ִ��� ����. ������ ǥ�� ����� �ٸ�.
	//// TODO ������ ���� ����
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
	//// ������ �������� ������ ������Ʈ
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
	EquipPlace = EItemEquipPlace::EIEP_End; // MasterData ���� �ٽ� �����ϴµ�.. ������ ���� equip_position �� ������ �����۸� ���� ���õǾ� �ִ� �͵� ���� ��ư..

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

	//������ �����͸� �������� �ٽ� ���� �Ǵ� �׸�
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
//		// RefID �ڸ����� ���� �Ծ��� ���� �������� ����ϰ� �ǹǷ� Ȥ�ö� ����� �ٸ��� ������ üũ�Ѵ�.
//#if WITH_EDITOR // ��.. �����Ϳ����� ����.. ������ ���� �������� Ȥ�ö� �ݳ���� �����Ǵ±�
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
	//	EquipPlace = InMasterData->ItemEquipPlace; // �̹� ������ �Ǿ� �ֳ�..
	//	MaxLevel = InMasterData->MaxLevel + ((SurpassCount > 0) ? SurpassCount : 0); // �ʿ��� ���� MaxLevel Ȯ���� MasterData �� ������ ���� �ݿ�.
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
// FItemRefIDHelper. ItemRefID �ٷ�� ��ƿ�� ��� ����
// ���� ������ RefID ���� �ٷ�� ��ƿ�̳� ��ȭ, ���� �� Ư�� ������ RefID ��
// ItemRefID �� �ڵ忡�� ���� �����ϴ� ���� �ٶ������� ������ 
// DB ���� �ƹ� �Ӽ��̳� Ű�� �ؼ� query �� �� �� ���� Ŭ���̾�Ʈ ������ ���� ����ϰ� �� ���ۿ� ����.

bool FItemRefIDHelper::IsGoodsRefID(int32 InID)
{
	return (InID == GetGoodsID_Gold() || InID == GetGoodsID_Gem() || InID == GetGoodsID_BladePoint() || InID == GetGoodsID_FriendshipPoint() || InID == GetGoodsID_DarkCrystal());
}

int32 FItemRefIDHelper::GetUpgradeStoneIDForUpgradeTarget(const FB2Item& InUpgradeTarget, const EUpgradeded EUpgraded)
{
	// �̹� ���׷��̵� �� ��� -1�� �ؼ� ���� �������� ����� �±޼��� ����� Ȯ���ؾ� �Ǳ� ����.
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
	return ITEM_REF_ID_ENHANCE_PROTECTION_ITEM; // ���� Ÿ�Ժ��� �з��� ����..
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
	//������ �̷��� �ϰ� ������ iteminfo idx�� 2~3��° �ڸ��� ��Ʈ���� �ؼ� �������� ���� �� ����
	return InRefID - ITEM_REF_ID_BOSSPIECE_BEGIN + 1;
}

int32 FItemRefIDHelper::GetBossPieceRefIDFromRelicID(int32 InRelicID)
{ //�� ������ ���������� 1:1 ��Ī�̹Ƿ� , (������ȣ-1)�� ������.
	return ITEM_REF_ID_BOSSPIECE_BEGIN + InRelicID - 1;
}

int32 FItemRefIDHelper::GetWingResourceRefIDByAcqElemType(EWingIngredAcqElemType InAcqElemType)
{
	return InAcqElemType == EWingIngredAcqElemType::WAET_EnhanceIngred ? GetWingEnhanceResourceRefID() :
		(InAcqElemType == EWingIngredAcqElemType::WAET_EvolveIngred ? GetWingEvolveResourceRefID() :
			GetWingPropOptionEnhanceResourceRefID());
}

/*================================================================================
�������� RefID �ڸ������� ��� �Ծ࿡ ���� ������ �� ���� �Լ����ε�,
������ �̰� ���⺸�ٴ� FB2Item ��ü �ʵ带 ����ϴ� �� ����.
�׷����� �ᱹ �̷� ����� �ʿ��� ������ �־ ��¿ �� ���� ���.
================================================================================*/
EItemClass FItemRefIDHelper::ExtractItemClassFromRefID(int32 InItemRefID)
{
	//if (InItemRefID == FItemRefIDHelper::GetGoodsID_Gold()) // Ư���� ���.. �ٵ� ��ȭ�� ��� �� ���� ������ �� ��Ÿ ��� �͵��� ���⿡ �־� �־�� �ϴ� �� �ƴϴ�.
	//{
	//	return EItemClass::EIC_Gold;
	//}

	//const int32 TheBiggestCategoryDigit = ExtractNthDigitR<7>(InItemRefID);
	//if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT)	// ���� �����۸� �Ʒ� ��Ģ�� ����
	//{
	//	// 4��° �ڸ�
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
	//if (InItemRefID == FItemRefIDHelper::GetGoodsID_Gold()) // Ư���� ���.. �ٵ� ��ȭ�� ��� �� ���� ������ �� ��Ÿ ��� �͵��� ���⿡ �־� �־�� �ϴ� �� �ƴϴ�.
	//{
	//	return EItemClass::EIC_Gold;
	//}

	//const int32 TheBiggestCategoryDigit = ExtractNthDigitR<7>(InItemRefID);
	//if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT)	// ���� �����۸� �Ʒ� ��Ģ�� ����
	//{
	//	// 4��° �ڸ�
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
	//	// ���� �� ���ڸ��� 8�� ��� �Ҹ�ǰ���� ��� �ʿ��ϱ� ������ ���� �±޼��� ��� ��.
	//	return EItemClass::EIC_UpgradeStone;
	//}

	return EItemClass::EIC_End;
}

EPCClass FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(int32 InItemRefID)
{
	//const int32 TheBiggestCategoryDigit = ExtractNthDigitR<7>(InItemRefID);
	//// ���� ������, ����(Essence), �ڽ�Ƭ �����۸� �Ʒ� ��Ģ�� ����
	//if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT || 
	//	TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_ESSENCE || 
	//	TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_COSTUME ||
	//	TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_SURPASSANVIL
	//	)
	//{
	//	// �ڿ��� 6��° �ڸ�
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
	//// ���� ������, ����(Essence), �ڽ�Ƭ �����۸� �Ʒ� ��Ģ�� ����
	//if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT ||
	//	TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_ESSENCE ||
	//	TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_COSTUME ||
	//	TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_SURPASSANVIL
	//	)
	//{
	//	// �ڿ��� 5��° �ڸ� �����ڸ� ���
	//	int32 Grade = 0;
	//	Grade = (ExtractNthDigitR<ITEM_REF_ID_STAR_TENGRAD_DIGIT_NUMBER>(InItemRefID)) * 10;

	//	// �ڿ��� 3��° �ڸ� �����ڸ� ���
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
	// �� �� ���ڸ�
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
{ // �Ϲ������δ� IsEquipmentItem �� ���

	//const int32 IdLargeDigit = ExtractNthDigitR<7>(InRefID);
	//return IdLargeDigit == ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT;
	return true;
}

bool FItemRefIDHelper::IsEssenceItemByRefID(int32 InRefID) 
{ // �Ϲ������δ� IsEssenceItem �� ���
	return (ExtractNthDigitR<7>(InRefID) == ITEM_REF_ID_LARGEST_DIGIT_ESSENCE); 
}

bool FItemRefIDHelper::IsAnvilItemByRefID(int32 InRefID)
{ // �Ϲ������δ� IsAnvilItem �� ���
	return (ExtractNthDigitR<7>(InRefID) == ITEM_REF_ID_LARGEST_DIGIT_SURPASSANVIL);
}

bool FItemRefIDHelper::IsCostumeItemByRefID(int32 InRefID)
{
	return (ExtractNthDigitR<7>(InRefID) == ITEM_REF_ID_LARGEST_DIGIT_COSTUME);
}

int32 FItemRefIDHelper::GenerateRewardRepresentativeItemID(int32 InRewardGroupID, int32 InType)
{
	// �������� �������� ���� ������ ��ǥ �����ܿ� ���. ItemInfo ���̺� ��ϵ� ��ȣ ü��.
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
//		return FText::FromString(TEXT("")); // �ƹ��͵� ǥ�� ���ϴ� �������� ����ϱ� ���� �� ���� ����.
//	}
//
//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemPrimaryPointType"), true);
//	if (!EnumPtr)
//	{		
//#if WITH_EDITOR && !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("EItemPrimaryPointType Enum ��Ű���� ã�� �� ����. ��ǻ�Ͱ� �� �����Ѵ�.")));
//#endif
//		return FText::FromString(TEXT("Error"));
//	}
//
//	// Enum name �� �ؽ�Ʈ ���̺��� Ű�� ���
//	FString EnumName = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InPointType));
//	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("ENUM_") + EnumName);
	return FText::GetEmpty();
}

FText GetLOCTextOfItemOption(EItemOption InOption, EPCClass InPCClass)
{
//	if (InOption == EItemOption::EIO_End)
//	{
//		return FText::FromString(TEXT("")); // �ƹ��͵� ǥ�� ���ϴ� �������� ����ϱ� ���� �� ���� ����.
//	}
//
//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemOption"), true);
//	if (!EnumPtr)
//	{
//#if WITH_EDITOR && !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("EItemOption Enum ��Ű���� ã�� �� ����. ��ǻ�Ͱ� �� �����Ѵ�.")));
//#endif
//		return FText::FromString(TEXT("Error"));
//	}
//
//	// Enum name �� �ؽ�Ʈ ���̺��� Ű�� ���..
//	FString TextKeyName = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InOption));
//	// ..�ϴµ� ĳ���� ���� ���� �Ϻ� �ɼǵ��� ĳ���� �̸��� ���� �ٿ���.
//	if (IsItemOptionPerPCClass(InOption))
//	{
//		//TextKeyName += TEXT("_") + UB2PCClassInfo::GetTrimmedNameOfPCClassEnum(InPCClass);
//	}
//
//	TextKeyName = TEXT("ENUM_") + TextKeyName; // ���������� ���ξ �� �ٿ��ش�. ��
//	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TextKeyName);
	return FText::GetEmpty();
}

FText GetLOCTextOfItemQuality(int32 ItemQuality)
{
	if (ItemQuality > 0 && ItemQuality <= MAX_ITEM_QUALITY)
	{ 
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString::Printf(TEXT("ItemQualityPrefix_%d"), ItemQuality));
	}
	// ��ȿ�� ǰ�� ������ �ƴ� ��� ������ Ǯ ���ӿ� prefix �� ������� ���ϵ��� ���ۿ� �̰� ������ �ʴ� �� ���� ��. FSingleItemInfoData::GetLocalizedName ����
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
//		return FText::FromString(TEXT("")); // �ƹ��͵� ǥ�� ���ϴ� �������� ����ϱ� ���� �� ���� ����.
//	}
//
//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECharStatApplyType"), true);
//	if (!EnumPtr)
//	{
//#if WITH_EDITOR && !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("ECharStatApplyType Enum ��Ű���� ã�� �� ����. ��ǻ�Ͱ� �� �����Ѵ�.")));
//#endif
//		return FText::FromString(TEXT("Error"));
//	}
//
//	// Enum name �� �ؽ�Ʈ ���̺��� Ű�� ���
//	FString EnumKey = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InPrimaryPointApplyType));
//	FText EnumString =  FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), (TEXT("ENUM_") + EnumKey)), GetLOCTextOfPrimPointType(InCheckPointType));
//	return (EnumString.IsEmpty() == false) ? EnumString : FText::FromString(EnumKey);
	return FText::FromString(TEXT("Error"));
}

FText GetLOCTextOfPrimaryPointApplyType(ECharStatApplyType InPrimaryPointApplyType, EItemOption InOption, EPCClass InPCClass)
{
//	if (InPrimaryPointApplyType == ECharStatApplyType::ECSA_End || InOption == EItemOption::EIO_End || InPCClass == EPCClass::EPC_End)
//	{
//		return FText::FromString(TEXT("")); // �ƹ��͵� ǥ�� ���ϴ� �������� ����ϱ� ���� �� ���� ����.
//	}
//
//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECharStatApplyType"), true);
//	if (!EnumPtr)
//	{
//#if WITH_EDITOR && !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("ECharStatApplyType Enum ��Ű���� ã�� �� ����. ��ǻ�Ͱ� �� �����Ѵ�.")));
//#endif
//		return FText::FromString(TEXT("Error"));
//	}
//
//	// Enum name �� �ؽ�Ʈ ���̺��� Ű�� ���
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
	@ItemFactorLevel = ���� Table�� �ִ� ������ ���(1~6��) �� ����ġ - FMD_ItemSingleElem::ItemLevel
	@Quality = ���ϱ�~ �ֻ�� (1 ~ 4)
	@Level = ��ȭ���� 1 ~ 40
*/
float GetPreviewPrimaryPoint(EItemPrimaryPointType PrimaryType, float ItemFactorBase, int32 Quality, int32 FactorLevel)
{
	//FClientDataStore& ClientDataStore = BladeIIGameImpl::GetClientDataStore();

	//// �������� ������ ������ ������� PrimaryPoint ���� ( 0.8 Factor ���ϴ� ���� ����� )
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

	//// ǰ�� �Ѵܰ踶�� 0.2�� ����
	//// ǰ�� ��ȯ ������Ʈ�� ǰ���� ���� �ɷ�ġ�� ���� ��� ���̳��� �ٲ�
	////const float CalculatedItemLevel	 = (0.2f * (Quality - 1)) + ItemLevelForEnhance;
	//const float CalculatedItemLevel = (1 + qualityIncLevel) * ItemLevelForEnhance + 0.2f;

	float ReturnValue = 0.f;
	//if (PrimaryType == EItemPrimaryPointType::EIPP_Attack)
	//{
	//	// 100 + ( 8 * �����۷��� ) + ( 0.2 * �����۷���  * �����۷��� )
	//	float AttackValue = 
	//		(100.f + (8.0f * CalculatedItemLevel) + (0.2f * FMath::Pow(CalculatedItemLevel, 2)));

	//	ReturnValue = AttackValue;
	//}

	//else if (PrimaryType == EItemPrimaryPointType::EIPP_Defense)
	//{
	//	// ���ݷ� * 3 / 4
	//	float AttackValue =
	//		(100.f + (8.f * CalculatedItemLevel) + (0.2f * FMath::Pow(CalculatedItemLevel, 2)));

	//	AttackValue = FMath::FloorToFloat(AttackValue);

	//	ReturnValue = (AttackValue * 3.0f / 4.0f);
	//}
	//else if (PrimaryType == EItemPrimaryPointType::EIPP_Health)
	//{
	//	// (1 + ( 3 * �����۷��� ) + ( 0.05 * �����۷��� * �����۷���)) / 3
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
	���� ���� ����� �ƴ����� �ſ� Ư���� �����. 
	������ RefID �� ���� �Ӽ� ������ �� ��.
	================================================================================*/

	const int32 RefIDPCClassNum = (int32)InPCClass; // �� �Ծ��� �ٲ�ٸ� ĳ���� �Ұ� ȭ�鿡�� ������ �޽����� ������ ���� �״� �˰� �ǰ���..

	for (int32 ECI = 0; ECI < (int32)EItemClass::EIC_End; ++ECI)
	{
		EItemClass CurrItemClass = static_cast<EItemClass>(ECI);
		
		int32 RefIDItemClassNum = -1; // �� �Ծ��� �ٲ� ������ �޽����� ������ ������ �ǰ���..?
		EItemEquipPlace CurrEquipPlace = EItemEquipPlace::EIEP_End;

		// ExtractItemClassFromRefID �� GetItemEquipPlaceFromClass �� �ش��ϴ� �����ε�
		// �װͰ� ��Ī�ϴ��� �Ʒ��� üũ
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

		// �Լ� ���� �� �޽� ���� ����� �ƴ� �� �����Ѵ�. ��Ʈ, �����, ���� ��
		
		}

		if (CurrEquipPlace != EItemEquipPlace::EIEP_End && RefIDItemClassNum >= 0)
		{
			FB2Item NewItemData;
			// RefID �Ծ࿡ ���� �����ε� �̰ͺ��ٴ� �������� �����ִ� �Ӽ����� �켱��. ���⼱ �ʿ��ϴϱ� ��¿ �� ���� ���.
			NewItemData.ItemRefID = 1000000 + RefIDPCClassNum * 100000 + RefIDItemClassNum * 1000 + InStarGrade * 100 + InGroupID;
			// �޽� ���տ����� EquipPlace ������ �� ��. �������� ���� �ִ� �����ʹ� ���ϰ� ä����.
			NewItemData.AllowedPCClass = (int32)InPCClass;
			NewItemData.ItemClass = CurrItemClass;
			NewItemData.EquipPlace = CurrEquipPlace;

			// ������ ���� �Ծ࿡ �´��� üũ.. CheckForItemPropertyRule �� ����ϱ� ���� �� üũ�� ���� �����ʹ� ä���ش�.
			NewItemData.StarGrade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(NewItemData.ItemRefID);
			NewItemData.InventoryType = GetItemInvenTypeFromClass(NewItemData.ItemClass);
			BII_CHECK(CheckForItemPropertyRule(NewItemData));

			OutData.Add(NewItemData);
		}
	}
}

bool GetItemIntrinsicOptionOfType(FB2Item& ItemToCheck, FItemOption& OutFoundOption, EItemOption InSearchOption)
{ // InSearchOption �� �ش��ϴ� �� �ִ��� ã�� �뵵
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
{ // InSearchOption �� �ش��ϴ� �� �ִ��� ã�� �뵵
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
		return true; // ���������� �÷��̾� ĳ���� �������� ���еǴ� �͵���.
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
	case EItemOption::EIO_Offense_IncAttack: // �� ��ġ�� ǥ�õǴ� �ֵ�
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
		checkSlow(OptionApplyType == ECombatOptionApplyType::ECOAT_Add_Value); // ���ݱ��� �׷��ٴ� �Ű� �� ������ Ʋ�� ���� �ִ�. �ƴ� ��찡 �߻��ϸ� �����̲� ��ó.
		break;

	case EItemOption::EIO_Health_PerSecRecoverHP:
		checkSlow(OptionApplyType == ECombatOptionApplyType::ECOAT_Add_Value); // ���ݱ��� �׷��ٴ� �Ű� �� ������ Ʋ�� ���� �ִ�. �ƴ� ��찡 �߻��ϸ� �����̲� ��ó.
		break;
		
	case EItemOption::EIO_Skill_DecSkillCooltime: // �ð� ����. �ʴ��� ǥ��
	case EItemOption::EIO_Misc_DecTagCooltime:
		UnitText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TimeUnitSecond"));
		break;

	// ��Ÿ �������� % ��
	default:
		// OptionApplyType �� Percent �Ӽ��� ����ε� Ȥ�ö� �ƴ� ��찡 �߻��ϸ� �����̲� ��ó.
		checkSlow(OptionApplyType == ECombatOptionApplyType::ECOAT_Add_Percent || OptionApplyType == ECombatOptionApplyType::ECOAT_OneMinusMultiply);
		UnitText = FText::FromString(TEXT("%"));
		break;
	}

	// �Ҽ��� ��°�ڸ����� ����ϵ� ���ڸ� 0 �� ��ȿ���ڷ� �� ġ�� ����.
	return FText::Format(FText::FromString(TEXT("{0} {1}")), GetAutoFractionalFormattedText(DisplayValue, 2), UnitText);
}

FText GetSkillOptionValueDisplayText(ESkillOption InOption, float InOptionValue)
{
	ECombatOptionApplyType OptionApplyType = GetCombatOptionApplyType(InOption);
	// EffectiveOptionValue �� GetEffectiveItemOptionValue ���� ���� �ɷ� ģ��.
	FText TempDisPlayText;
	FText FinalDisplayText;

	// �Ҽ��� ��°�ڸ����� ����ϵ� ���ڸ� 0 �� ��ȿ���ڷ� �� ġ�� ����.
	if (InOptionValue > 0)
		TempDisPlayText = FText::Format(FText::FromString(TEXT("{0} {1}")), FText::FromString("+"), GetAutoFractionalFormattedText(InOptionValue, 2));
	else
		TempDisPlayText = GetAutoFractionalFormattedText(InOptionValue, 2);


	switch (InOption)
	{

	// �ʸ� ���̴� �ֵ�
	case ESkillOption::ESO_DecCoolTime:
		FinalDisplayText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Sconds")), TempDisPlayText);
		break;
	// �ƹ��͵� ���¾ֵ�
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
	// %�� ���̴� �ֵ�
	default:
		FinalDisplayText = FText::Format(FText::FromString(TEXT("{0} {1}")), TempDisPlayText,FText::FromString("%"));
		break;
	}

	return FinalDisplayText;
}

float GetEffectiveItemOptionValue(EItemOption InOptionType, const FB2Item& ItemToCheckOption, const double InEnhanceEffectFactor /*= 1.f*/)
{
	TArray<FOptionValue> OptionRawValues;
	for (const FItemOption& CurrOption : ItemToCheckOption.IntrinsicOptions) // �Ƹ��� ���� ����Ʈ���� ���� Ÿ�� �ɼ��� �ΰ� �̻� �� ��������..?
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
	
	// ��ȭ ������ ��Ʈ ȿ���� ����ϴ� �������� ��
	return GetEffectiveItemOptionValueWithEnhanceSetEffect(InOptionType, OptionRawValues, InEnhanceEffectFactor);
}

void GenerateGoldItem(FB2Item& OutItem, int32 InGoldAmount)
{
	OutItem.ItemRefID = FItemRefIDHelper::GetGoodsID_Gold();
	OutItem.ItemClass = EItemClass::EIC_Gold;
	OutItem.InventoryType = EItemInvenType::EIIVT_Consumables;
	OutItem.ConsumingAmount = InGoldAmount;
	OutItem.SetPrimaryPoint(InGoldAmount); // ������ PrimaryPoint �� �� �� �־..
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
		// ���� ��ǥ �������� ��� ItemClass �� ���� �˻��ϴ� ���� �ϳ����� ���ڸ��� 9���밡 ����. �̰� ��� ó���ؾ� �ұ�.. �ϴ��� false �� �ɷ�.
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
{// �������� �ٸ� �� �ȵ� ��� ���� �⺻.
	// �ʿ信 ���� B2mdItemInfo �� is_levelupable �� Ŭ��� �����ͼ� �߰� üũ.
	return (IsEquipmentItem(InItem) && !IsItemCompositionAllowed(InItem) && !IsItemUpgradeAllowed(InItem) && !IsItemSurpassAllowed(InItem));
}

bool IsItemCompositionAllowed(const FB2Item& InItem)
{ // �ռ�, �±��� ���� ���ǿ����� ����. �� ��� �� �ְ� ��������. 
  // �ʿ信 ���� B2mdItemInfo �� is_composable �� Ŭ��� �����ͼ� �߰� üũ.
	return (IsEquipmentItem(InItem) && InItem.Level >= InItem.MaxLevel && !IsItemSurpassAllowed(InItem));
}

bool IsItemUpgradeAllowed(const FB2Item& InItem)
{ // �ռ�, �±��� ���� ���ǿ����� ����. �� ��� �� �ְ� ��������
  // �ʿ信 ���� B2mdItemInfo �� is_upgradable �� Ŭ��� �����ͼ� �߰� üũ.
	return (IsEquipmentItem(InItem) && InItem.Level >= InItem.MaxLevel && !IsItemSurpassAllowed(InItem));
}

bool IsItemSurpassAllowed(const FB2Item& InItem)
{ // ���� �������� �ʿ��� ���ȴ�.
  // �ʿ信 ���� B2mdItemInfo �� is_surpassable �� Ŭ��� �����ͼ� �߰� üũ.
	return (IsEquipmentItem(InItem) && InItem.StarGrade >= FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE && InItem.Level >= InItem.MaxLevel);
}

bool IsItemEnhanceAllowed(const FB2Item& InItem)
{
	return (IsEquipmentItem(InItem) && InItem.EnhanceLevel < MAX_ENHANCE_ITEM_LEVEL);
}

// �ʿ��� 6������ 7������ �þ�� ����(�����ε� 8��,9��)
// 6�������� �ռ��� �±� �ʿ��� �ѹ��� �������� ���� ����
// �� ��� ���� üũ (���� 6��)
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

	//// ���� ��ǥ �������� ��� ItemClass �� ���� �˻��ϴ� ���� �ϳ����� ���ڸ��� 9���밡 ����. �̰� ��� ó���ؾ� �ұ�.. �ϴ��� false �� �ɷ�.
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
	//	float IngredFactor2 = CliDataStore.GetItemLevelupFactor_IngredItemEquipPlace(ThisIngred.EquipPlace); // ���� �����۵� EquipPlace �� �ִ�.
	//	float IngredFactor3 = CliDataStore.GetItemLevelupFactor_IngredItemEnhLv(ThisIngred.Level);
	//	float IngredFactor4 = CliDataStore.GetItemLevelupFactor_IngredItemSurpCount(ThisIngred.SurpassCount);
	//	// �� ������ ������ ��Ḷ�� TargetFactor �� ���Ѵ�.
	//	float ThisIngredTotalFactor = TargetFactor * IngredFactor1 * IngredFactor2 * IngredFactor3 * IngredFactor4;

	//	if (IsEssenceItem(ThisIngred)) { // ���� �������� ���� ȿ��.
	//		ThisIngredTotalFactor *= 0.5f;
	//	}

	//	TotalFactor += ThisIngredTotalFactor; // ���������δ� �� ��ῡ ���� factor ��갪���� ����.
	//}

	//// �ϴ� ������ �󸶳� ������ �Ű澲�� �ʰ� ����ġ�� �ܼ��� ���Ѵ�.
	//const float SimpleAddedExp = TargetItem.Exp + TotalFactor;

	//const int32 SimpleAddedExp_Floor = FMath::FloorToInt(SimpleAddedExp);
	////const float SimpleAddedExp_Frac = FMath::Frac(SimpleAddedExp); �̰� �ʿ���� ��.

	//// ���� ����� ���ڸ� ����ŭ.
	//const int32 LevelToIncrease = SimpleAddedExp_Floor / 100;
	//int32 RetLevel = TargetItem.Level + LevelToIncrease; // �� ���������� MaxLevel �� Ŭ�������� ���� ��

	//// ��ü ����ġ ��ºп��� ���ڸ��� ���ϸ� ����.
	//OutNewExp = SimpleAddedExp - ((float)LevelToIncrease * 100.0f); // ���⼭�� ���е� ������ �� �� �ֳ�..

	//if (bClampResultToMaxValue && RetLevel >= TargetItem.MaxLevel)
	//{
	//	RetLevel = TargetItem.MaxLevel;
	//	OutNewExp = 100.0f;
	//}

	//return RetLevel;
	return 0.0;
}

/** ����, ����ġ �� �⺻���� ���� ä�������� ���� ��� ��ġ�� �� �� �ִ� �ɼǰ��� ä���ִ� �Լ�. ������ ��. */
static void SetPreviewItemResultOptionValuesInternal(const FB2Item& InOriginItem, FB2Item& InOutPreviewItem)
{ // ��� �ɼǰ��� �ܿ� ���⼭ ����ϴ� �⺻���� ������ ä���� �־�� �Ѵ�. 


	//const float PrimaryPoint = GetPreviewPrimaryPoint(InOutPreviewItem.PrimaryPointType,
	//												  InOutPreviewItem.ItemFactorLevel,
	//												  InOutPreviewItem.Quality,
	//												  InOutPreviewItem.Level);

	//InOutPreviewItem.SetPrimaryPoint(PrimaryPoint);


	//FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	//int32 LevelDistance = (int32)((InOutPreviewItem.Level - InOriginItem.Level + (InOriginItem.Level % 10) ) / 10);
	//// ��Ÿ �ɼǰ���..
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

/** ������ ������ ������ ������ �� ��� �����Ͱ� ��� �Ǵ��� ������ */
void GetCalculateItem_PreviewItemLevelup(const FB2Item& InTargetItem, const TArray<FB2Item>& ItemLevelupIngredients, FB2Item& OutPreviewResult, bool bClampResultToMaxValue)
{
	// Note(����!) : ���� ��� ������ Ŭ�󿡼� UI������ �����ֱ����� �뵵�θ� ����ϰ��ְ�, ���� ���ݷ� ����� �������� �ؼ� �����ְ�����. 
	//OutPreviewResult = InTargetItem;

	//if (ItemLevelupIngredients.Num() == 0)
	//	return;

	//float NewExp = InTargetItem.Exp;
	//OutPreviewResult.Level = GetLevel_PreviewItemLevelup(NewExp, InTargetItem, ItemLevelupIngredients, bClampResultToMaxValue);
	//OutPreviewResult.Exp = NewExp;

	//SetPreviewItemResultOptionValuesInternal(InTargetItem, OutPreviewResult); // �⺻���� �����͸� ���� �� ��� �ɼ� ��ġ���� ������ ����.

	//// ��ȭ�� ���� �ȵ� �⺻ ������� �̾Ƴ��� �� ���� �������� ��ȭ���� �������ش�.
	//int32 BasePrimaryPoint = OutPreviewResult.GetPrimaryPoint();
	//int32 EnhanceLevel = InTargetItem.EnhanceLevel;

	//switch (OutPreviewResult.PrimaryPointType)
	//{
	//	// ���ݷ� += ���ݷ� * ((������� * ��ȭ����) + (����� * ��ȭ���� * ��ȭ����))
	//	case EItemPrimaryPointType::EIPP_Attack:
	//	case EItemPrimaryPointType::EIPP_Defense:
	//	{
	//		BasePrimaryPoint += (BasePrimaryPoint * ((0.02f * EnhanceLevel) + (0.00065f * FMath::Pow(EnhanceLevel, 2))));
	//		OutPreviewResult.SetPrimaryPoint(B2ServerRoundDown(BasePrimaryPoint, 1.f));
	//	}
	//		break;
	//	// ���ݷ� += (���ݷ� * ((������� * ��ȭ����) + (����� * ��ȭ���� * ��ȭ����))) + 1
	//	case EItemPrimaryPointType::EIPP_Health:
	//	{
	//		BasePrimaryPoint += ((BasePrimaryPoint * ((0.02f * EnhanceLevel) + (0.00065f * FMath::Pow(EnhanceLevel, 2)))) + 1);
	//		OutPreviewResult.SetPrimaryPoint(B2ServerRoundDown(BasePrimaryPoint, 1.f));
	//	}
	//		break;
	//}
}

/** Ư�� ��ġ��ŭ ������ �� ��� �����Ͱ� ��� �Ǵ��� ������ */
void GetCalculateItem_PreviewItemLevelup(const FB2Item& InTargetItem, FB2Item& OutPreviewResult, int32 LevelIncreaseAmount /*= 0*/) 
{
	// Note(����!) : ���� ��� ������ Ŭ�󿡼� UI������ �����ֱ����� �뵵�θ� ����ϰ��ְ�, ���� ���ݷ� ����� �������� �ؼ� �����ְ�����. 
	OutPreviewResult = InTargetItem;
	OutPreviewResult.Level += LevelIncreaseAmount;

	SetPreviewItemResultOptionValuesInternal(InTargetItem, OutPreviewResult); // �⺻���� �����͸� ���� �� ��� �ɼ� ��ġ���� ������ ����.
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
	//// ��ȭ�� ���� �ȵ� �⺻ ������� �̾Ƴ��� �� ���� �������� ��ȭ���� �������ش�.
	//int32 BasePrimaryPoint = CalcLevelUpItem.GetPrimaryPoint();
	//int32 EnhanceLevel = InTargetItem.EnhanceLevel + IncreaseEnhanceLevel;

	//switch (CalcLevelUpItem.PrimaryPointType)
	//{
	//	// ���ݷ� += ���ݷ� * ((������� * ��ȭ����) + (����� * ��ȭ���� * ��ȭ����))
	//case EItemPrimaryPointType::EIPP_Attack:
	//case EItemPrimaryPointType::EIPP_Defense:
	//	{
	//		BasePrimaryPoint += (BasePrimaryPoint * ((0.02f * EnhanceLevel) + (0.00065f * FMath::Pow(EnhanceLevel, 2))));
	//		OutPreviewResult.SetPrimaryPoint(B2ServerRoundDown(BasePrimaryPoint, 1.f));
	//	}
	//	break;
	//	// ���ݷ� += (���ݷ� * ((������� * ��ȭ����) + (����� * ��ȭ���� * ��ȭ����))) + 1
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

	//// ��ȭ�� ���� �ȵ� �⺻ ������� �̾Ƴ��� �� ���� �������� ��ȭ���� �������ش�.
	//int32 BasePrimaryPoint = CalcLevelUpItem.GetPrimaryPoint();
	//int32 EnhanceLevel = InTargetItem.EnhanceLevel + IncreaseEnhanceLevel;

	//switch (CalcLevelUpItem.PrimaryPointType)
	//{
	//// ���ݷ� = ���� �⺻ ���ݷ� + ���Ⱝȭ���ݷ�(200+0.5*�⺻ ���ݷ�*��ȭ ����+50*��ȭ ����*��ȭ ����)
	//case EItemPrimaryPointType::EIPP_Attack:
	//{
	//	BasePrimaryPoint += (200 + (0.5 * BasePrimaryPoint * EnhanceLevel) + (50*EnhanceLevel*EnhanceLevel));
	//	OutPreviewResult.SetPrimaryPoint(B2ServerRoundDown(BasePrimaryPoint, 1.f));
	//}
	//break;
	//// ���� =  �� �⺻ ���� + ����ȭ����(400+0.5*�⺻ ����*��ȭ ����+100*��ȭ ����*��ȭ ����)
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
	�������� RefID �ڸ��� ����ó�� Ư�� �Ӽ��� ������ �ٸ� �Ӽ��� �����ϴ� �Լ�.
	FItemRefIDHelper �� Extract** ������ó�� �� �ʿ��� ��츸.
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

	return EItemInvenType::EIIVT_Consumables; // �������� �Ҹ�ǰ����
}

EItemEquipPlace GetItemEquipPlaceFromClass(EItemClass InItemClass)
{
	switch (InItemClass)
	{
		// ���� �ϴ��� ����
	case EItemClass::EIC_Weapon: return EItemEquipPlace::EIEP_Weapon;

	case EItemClass::EIC_Helmet: return EItemEquipPlace::EIEP_Helmet;
	case EItemClass::EIC_BodyArmor: return EItemEquipPlace::EIEP_BodyArmor;
	case EItemClass::EIC_Glove: return EItemEquipPlace::EIEP_Glove;
	case EItemClass::EIC_Shoes: return EItemEquipPlace::EIEP_Shoes;

	case EItemClass::EIC_Belt: return EItemEquipPlace::EIEP_Belt;
	case EItemClass::EIC_Necklace: return EItemEquipPlace::EIEP_Necklace;
	case EItemClass::EIC_Ring: return EItemEquipPlace::EIEP_Ring;
	}

	return EItemEquipPlace::EIEP_End; // �������� ���� �Ұ���
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
	if (FItemRefIDHelper::IsEquipmentItemByRefID(InItemToCheck.ItemRefID)) // ���ڸ� 1 �� �Ϲ����� ���� ������. ���� �� üũ ���
	{
		bool bFoundMisMatch = false;
		bFoundMisMatch |= (InItemToCheck.ItemClass != FItemRefIDHelper::ExtractItemClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.AllowedPCClass != (int32)FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.StarGrade != FItemRefIDHelper::ExtractItemStarGradeFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.InventoryType != GetItemInvenTypeFromClass(InItemToCheck.ItemClass));
		bFoundMisMatch |= (InItemToCheck.EquipPlace != GetItemEquipPlaceFromClass(InItemToCheck.ItemClass));
		
		return !bFoundMisMatch;
	}
	else if (FItemRefIDHelper::IsEssenceItemByRefID(InItemToCheck.ItemRefID)) // ���� �����۵� ����� üũ
	{
		bool bFoundMisMatch = false;
		bFoundMisMatch |= (InItemToCheck.ItemClass != FItemRefIDHelper::ExtractItemClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.AllowedPCClass != (int32)FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.StarGrade != FItemRefIDHelper::ExtractItemStarGradeFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.InventoryType != GetItemInvenTypeFromClass(InItemToCheck.ItemClass));

		return !bFoundMisMatch;
	}
	else if (FItemRefIDHelper::IsCostumeItemByRefID(InItemToCheck.ItemRefID)) // ���ڸ� 3�� �ڽ�Ƭ ������
	{
		bool bFoundMisMatch = false;
		bFoundMisMatch |= (InItemToCheck.ItemClass			!= FItemRefIDHelper::ExtractItemClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.AllowedPCClass		!= (int32)FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.StarGrade			!= FItemRefIDHelper::ExtractItemStarGradeFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.InventoryType		!= GetItemInvenTypeFromClass(InItemToCheck.ItemClass));
		bFoundMisMatch |= (InItemToCheck.CostumeEquipPlace	!= GetItemCostumeEquipPlaceFromClass(InItemToCheck.ItemClass));
		return !bFoundMisMatch;
	}
	else if (FItemRefIDHelper::IsAnvilItemByRefID(InItemToCheck.ItemRefID)) // �ʿ� �����۵� ����� üũ
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
	// RefID �� ������ ������ �־�� ��.
	
	InItemToLoad.StarGrade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(InItemToLoad.ItemRefID);
	InItemToLoad.AllowedPCClass = (int32)FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(InItemToLoad.ItemRefID);

	if (FItemRefIDHelper::IsEquipmentItemByRefID(InItemToLoad.ItemRefID))
	{
		InItemToLoad.ItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(InItemToLoad.ItemRefID);
		InItemToLoad.MaxLevel = 30; // �� ����� ���� ��
		InItemToLoad.InventoryType = GetItemInvenTypeFromClass(InItemToLoad.ItemClass);
		InItemToLoad.EquipPlace = GetItemEquipPlaceFromClass(InItemToLoad.ItemClass);
	}
	else if (FItemRefIDHelper::IsEssenceItemByRefID(InItemToLoad.ItemRefID))
	{ // ���� ������ Class Ȯ�� �� �ڵ�. ������ �� �Լ� ��ü�� ���߿�.
		InItemToLoad.ItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(InItemToLoad.ItemRefID);
		InItemToLoad.InventoryType = GetItemInvenTypeFromClass(InItemToLoad.ItemClass); 
	}
	else if (FItemRefIDHelper::IsAnvilItemByRefID(InItemToLoad.ItemRefID))
	{ // ���� ������ Class Ȯ�� �� �ڵ�. ������ �� �Լ� ��ü�� ���߿�.
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

	return GetMaxPCClassNum(); // ���� ���� ó�� �ǹ���. �̰� �ް� �Ǹ� PCItems �ε����� ���� �ȵ�.
}
