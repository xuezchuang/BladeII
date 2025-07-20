// Fill out your copyright notice in the Description page of Project Settings.


#include "B2Inventory.h"
//#include "BladeII.h"
#include "B2ItemInfo.h"
#include "B2LobbyGameMode.h"
#include "B2CombatStatEvaluator.h"
#include "B2PCClassInfo.h"

#include "FB2ErrorMessage.h"
#include "BladeIIGameImpl.h"

#include "BladeIIUtil.h"
#include "Math/UnrealMathUtility.h"

float FB2Item::GetPrimaryPoint(bool bCountRelevantOption) const
{
	float RetPoint = FMath::FloorToFloat(PrimaryPoint);
	
	// 父老 捞吧肺 掘绢柯 蔼俊促啊 Option 栏肺 牢茄 刘啊 荐摹绰 蝶肺 绊妨且 芭扼搁 bCountRelevantOption 篮 false 肺 林绢具 且 巴.
	// 角龙利栏肺 咯矾 惑炔俊辑 可记鳖瘤 绊妨茄促绊 秦档 肯寒窍霸 窍扁绰 绢妨匡 巴. 弊成 窜鉴窍霸 PrimaryPoint 父 眉农窍绰 急俊辑 谗绰霸 唱阑 荐 乐促.
	if (bCountRelevantOption)
	{
		const EItemOption RelevantOption = CombatStatEval::GetPrimPointIncOption(PrimaryPointType);
		if (RelevantOption != EItemOption::EIO_End)
		{
			const float OrgValue = FMath::Max(GetEffectiveItemOptionValue(RelevantOption, *this), 0.0f);
			const float FixedValue = FMath::FloorToFloat(OrgValue);
			RetPoint += FixedValue;
		}
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
	float fPreviewCombatValue = FMath::FloorToFloat(PrimaryPoint * 100);
	float fPreviewOptionCombatValue = 0.f;

	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	if (const FMD_ItemSingleElem* ItemInfo = CliDataStore.GetItemMasterDetailInfo(ItemRefID))
	{
		for (auto& ItemOpt : ItemInfo->UniqueOptions)
		{
			fPreviewOptionCombatValue += ItemOpt.RawOptionAmount * CliDataStore.GetOptionWeight(ItemOpt.OptionType);
		}
	}

	this->Power = fPreviewOptionCombatValue + (CliDataStore.GetOptionWeight(ItemPrimaryPointTypeToEItemOptionType(PrimaryPointType)) * (fPreviewCombatValue / 100));
}

FB2Item& FB2Item::operator=(const b2network::B2ItemServerInfoPtr rhs)
{
	 //辑滚俊辑 罐篮 蔼栏肺 糕滚 函荐俊 技泼
	InstanceUID = rhs->id;
	ItemRefID = rhs->template_id;
	
	StarGrade = rhs->grade;
	Quality = rhs->quality;
	ConsumingAmount = rhs->amount;

	AllowedPCClass = (int32)SvrToCliPCClassType(rhs->character_type);

	InventoryType = SvrToCliItemInvenType(rhs->inventory_type);
	EquipPlace = SvrToCliItemEquipPlace(rhs->equip_position); // MasterData 烹秦 促矫 技泼窍绰单.. 捞傈浚 辑滚 equip_position 捞 厘馒等 酒捞袍父 蔼捞 技泼登绢 乐带 巴档 鞍绊 咯瓢..

	// 夸扒 泅犁 厘馒登绢 乐绰瘤 咯何. 辑滚客 钎矫 规过捞 促抚.
	// TODO 橇府悸 沥焊 曼炼
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	bCurrentlyEquipped = CliDataStore.GetLocalCharacterData().GetUserAllItems().IsInCurrentItemPreset(IntToPCClass(AllowedPCClass), InstanceUID);

	Level = rhs->level;
	Exp = rhs->exp;
	SurpassCount = rhs->surpass_count;
	EnhanceLevel = rhs->enhance_level;
	PrimaryPoint = rhs->md_main_attribute_value;

	RandomOptions.Empty();
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
	}

	SealOptions.Empty();
	for (auto SealOptionInfo : rhs->seal_slot_states)
	{
		SealOptions.Add(FSealOption(SealOptionInfo->seal_slot, SealOptionInfo->opened, SvrToCliOptionType(SealOptionInfo->option_id), SealOptionInfo->option_value));
	}

	OpenSealSlotCount = rhs->open_seal_slot_count;
	
	bIsLocked = rhs->locked;
	Power = rhs->power;

	IsNew = rhs->is_new;
	// 付胶磐 单捞磐狼 蔼栏肺 诀单捞飘
	UpdateItemMasterDataOnMD();
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
	EquipPlace = EItemEquipPlace::EIEP_End; // MasterData 烹秦 促矫 技泼窍绰单.. 捞傈浚 辑滚 equip_position 捞 厘馒等 酒捞袍父 蔼捞 技泼登绢 乐带 巴档 鞍绊 咯瓢..

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

	//付胶磐 单捞磐甫 扁霖栏肺 促矫 技泼 登绰 亲格
	//ItemClass, CostumeEquipPlace, AllowedPCClass, InventoryType, StarGrade
	UpdateItemMasterDataOnMD();

	//FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	//bCurrentlyEquipped = CliDataStore.GetLocalCharacterData().GetUserAllItems().IsEquippedCostume(*this);
}

void FB2Item::UpdateItemMasterDataOnMD()
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	const FMD_ItemSingleElem* ItemMDElem = CliDataStore.GetItemMasterDetailInfo(ItemRefID);
	if (ItemMDElem)
	{
		UpdateByItemMasterData(ItemMDElem);

		// RefID 磊府荐俊 蝶弗 痹距阑 捞哩 沥侥栏肺 荤侩窍霸 登骨肺 趣矫扼档 抗惑苞 促福瘤 臼篮瘤 眉农茄促.
#if WITH_EDITOR // 澜.. 俊叼磐俊辑父 窍磊.. 酒流篮 呼靛 滚傈俊辑 趣矫扼档 螺朝鳖毫 捌沥登绰焙
		check(CheckForItemPropertyRule(*this));
#endif
	}
}

void FB2Item::UpdateByItemMasterData(const FMD_ItemSingleElem* InMasterData)
{
	if (InMasterData)
	{
		SetID = InMasterData->SetOptionID;
		MaxStarGrade = InMasterData->MaxGrade;
		ItemClass = InMasterData->ItemClass;
		InventoryType = InMasterData->ItemInvenType;
		EquipPlace = InMasterData->ItemEquipPlace; // 捞固 技泼捞 登绢 乐唱..
		MaxLevel = InMasterData->MaxLevel + ((SurpassCount > 0) ? SurpassCount : 0); // 檬岿肺 牢茄 MaxLevel 犬厘篮 MasterData 浚 绝栏聪 蝶肺 馆康.
		ItemFactorLevel = InMasterData->ItemFactorLevel;
		PrimaryPointType = InMasterData->PrimaryType;

		IntrinsicOptions.Empty();
		for (auto option : InMasterData->UniqueOptions) {
			IntrinsicOptions.Add(option);
		}

		BuyingPrice = InMasterData->BuyPrice;
		SellingPrice = InMasterData->SellPrice;

		ItemRefID = InMasterData->ItemTemplateID;
		StarGrade = InMasterData->ItemGrade;

		AllowedPCClass = PCClassToInt(InMasterData->ItemEquipClass);

		CostumeEquipPlace = InMasterData->CostumeEquipPlace;
	}
}

//////////////////////////////////////////////////////////////////////
// FItemRefIDHelper. ItemRefID 促风绰 蜡瓶甸 葛滴 葛澜
// 厘馒 酒捞袍 RefID 逢阑 促风绰 蜡瓶捞唱 犁拳, 瘤丰 殿 漂荐 酒捞袍 RefID 殿
// ItemRefID 甫 内靛俊辑 流立 曼炼窍绰 巴捞 官恩流窍瘤 臼瘤父 
// DB 付成 酒公 加己捞唱 虐肺 秦辑 query 甫 且 荐 绝绰 努扼捞攫飘 荤沥惑 流立 荤侩窍霸 瞪 荐观俊 绝澜.

bool FItemRefIDHelper::IsGoodsRefID(int32 InID)
{
	return (InID == GetGoodsID_Gold() || InID == GetGoodsID_Gem() || InID == GetGoodsID_BladePoint() || InID == GetGoodsID_FriendshipPoint() || InID == GetGoodsID_DarkCrystal());
}

int32 FItemRefIDHelper::GetUpgradeStoneIDForUpgradeTarget(const FB2Item& InUpgradeTarget, const EUpgradeded EUpgraded)
{
	// 捞固 诀弊饭捞靛 等 版快 -1阑 秦辑 扁粮 酒捞袍捞 荤侩茄 铰鞭籍捞 绢恫扒龙 犬牢秦具 登扁 锭巩.
	uint16 StarOffset = (EUpgraded == EUpgradeded::UPGRADE_AFTER ? 1 : 0);
	bool CheckAdvanced = (InUpgradeTarget.StarGrade - StarOffset >= FItemGradeInfo::MINIMUM_ADVANCED_UPGRADE_STAR_GRADE);

	switch (InUpgradeTarget.InventoryType)
	{
	case EItemInvenType::EIIVT_Weapon:
	{
		if (CheckAdvanced)	return ITEM_REF_ID_ADVANCED_UPGRADE_STONE_WEAPON;
		else				return ITEM_REF_ID_UPGRADE_STONE_WEAPON;
	}
	case EItemInvenType::EIIVT_Protection:
	{
		if (CheckAdvanced)	return ITEM_REF_ID_ADVANCED_UPGRADE_STONE_ARMOR;
		else				return ITEM_REF_ID_UPGRADE_STONE_PROTECTION;
	}
	case EItemInvenType::EIIVT_Accessory:
	{
		if (CheckAdvanced)	return ITEM_REF_ID_ADVANCED_UPGRADE_STONE_ACCESSORY;
		else				return ITEM_REF_ID_UPGRADE_STONE_ACCESSORY;
	}
	}
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
	return ITEM_REF_ID_ENHANCE_PROTECTION_ITEM; // 眠饶 鸥涝喊肺 盒幅瞪 荐档..
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
	//瘤陛篮 捞犯霸 窍绊 乐瘤父 iteminfo idx甫 2~3锅掳 磊府甫 厚飘楷魂 秦辑 啊廉坷霸 函版 且 抗沥
	return InRefID - ITEM_REF_ID_BOSSPIECE_BEGIN + 1;
}

int32 FItemRefIDHelper::GetBossPieceRefIDFromRelicID(int32 InRelicID)
{ //阿 蜡拱苞 焊胶炼阿篮 1:1 概莫捞骨肺 , (蜡拱锅龋-1)阑 歹秦淋.
	return ITEM_REF_ID_BOSSPIECE_BEGIN + InRelicID - 1;
}

int32 FItemRefIDHelper::GetWingResourceRefIDByAcqElemType(EWingIngredAcqElemType InAcqElemType)
{
	return InAcqElemType == EWingIngredAcqElemType::WAET_EnhanceIngred ? GetWingEnhanceResourceRefID() :
		(InAcqElemType == EWingIngredAcqElemType::WAET_EvolveIngred ? GetWingEvolveResourceRefID() :
			GetWingPropOptionEnhanceResourceRefID());
}

/*================================================================================
酒捞袍狼 RefID 磊复荐俊辑 割啊瘤 痹距俊 蝶弗 沥焊甫 哗 坷绰 窃荐甸牢单,
啊鞭利 捞吧 静扁焊促绰 FB2Item 磊眉 鞘靛甫 荤侩窍绰 霸 亮促.
弊犯瘤父 搬惫 捞繁 荤侩捞 鞘夸茄 镑甸捞 乐绢辑 绢驴 荐 绝捞 荤侩.
================================================================================*/
EItemClass FItemRefIDHelper::ExtractItemClassFromRefID(int32 InItemRefID)
{
	if (InItemRefID == FItemRefIDHelper::GetGoodsID_Gold()) // 漂荐茄 版快.. 辟单 犁拳唱 犁丰 殿 厘馒 酒捞袍 寇 扁鸥 葛电 巴甸阑 咯扁俊 持绢 林绢具 窍绰 扒 酒聪促.
	{
		return EItemClass::EIC_Gold;
	}

	const int32 TheBiggestCategoryDigit = ExtractNthDigitR<7>(InItemRefID);
	if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT)	// 厘馒 酒捞袍父 酒贰 痹蘑捞 嘎澜
	{
		// 4锅掳 磊府
		int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
		switch (FourthDigit)
		{
		case 0: return EItemClass::EIC_Weapon;
		case 1: return EItemClass::EIC_Helmet;
		case 2: return EItemClass::EIC_BodyArmor;
		case 3: return EItemClass::EIC_Glove;
		case 4: return EItemClass::EIC_Shoes;
		case 5: return EItemClass::EIC_Belt;
		case 6: return EItemClass::EIC_Necklace;
		case 7: return EItemClass::EIC_Ring;
		case 8: return EItemClass::EIC_Wing;
		}
	}
	else if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_ESSENCE)
	{
		int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
		switch (FourthDigit)
		{
		case 0: return EItemClass::EIC_WeaponEssence;
		case 1: return EItemClass::EIC_ProtectionEssence;
		case 2: return EItemClass::EIC_AccessoryEssence;
		}
	}
	else if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_COSTUME)
	{
		int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
		switch (FourthDigit)
		{
		case 0: return EItemClass::EIC_CostumeWeapon;
		case 1: return EItemClass::EIC_CostumeHat;
		case 2: return EItemClass::EIC_CostumeArmor;
		}
	}
	else if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_SURPASSANVIL)
	{
		int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
		switch (FourthDigit)
		{
		case 0: return EItemClass::EIC_WeaponSurpassAnvil;
		case 1: return EItemClass::EIC_ArmorSurpassAnvil;
		case 2: return EItemClass::EIC_AccessorySurpassAnvil;
		}
	}

	return EItemClass::EIC_End;
}
EItemClass FItemRefIDHelper::ExtractItemClassFromRefIDForItemForge(int32 InItemRefID)
{
	if (InItemRefID == FItemRefIDHelper::GetGoodsID_Gold()) // 漂荐茄 版快.. 辟单 犁拳唱 犁丰 殿 厘馒 酒捞袍 寇 扁鸥 葛电 巴甸阑 咯扁俊 持绢 林绢具 窍绰 扒 酒聪促.
	{
		return EItemClass::EIC_Gold;
	}

	const int32 TheBiggestCategoryDigit = ExtractNthDigitR<7>(InItemRefID);
	if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT)	// 厘馒 酒捞袍父 酒贰 痹蘑捞 嘎澜
	{
		// 4锅掳 磊府
		int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
		switch (FourthDigit)
		{
		case 0: return EItemClass::EIC_Weapon;
		case 1: return EItemClass::EIC_Helmet;
		case 2: return EItemClass::EIC_BodyArmor;
		case 3: return EItemClass::EIC_Glove;
		case 4: return EItemClass::EIC_Shoes;
		case 5: return EItemClass::EIC_Belt;
		case 6: return EItemClass::EIC_Necklace;
		case 7: return EItemClass::EIC_Ring;
		case 8: return EItemClass::EIC_Wing;
		}
	}
	else if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_ESSENCE)
	{
		int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
		switch (FourthDigit)
		{
		case 0: return EItemClass::EIC_WeaponEssence;
		case 1: return EItemClass::EIC_ProtectionEssence;
		case 2: return EItemClass::EIC_AccessoryEssence;
		}
	}
	else if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_COSTUME)
	{
		int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
		switch (FourthDigit)
		{
		case 0: return EItemClass::EIC_CostumeWeapon;
		case 1: return EItemClass::EIC_CostumeHat;
		case 2: return EItemClass::EIC_CostumeArmor;
		}
	}
	else if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_SURPASSANVIL)
	{
		int32 FourthDigit = ExtractNthDigitR<4>(InItemRefID);
		switch (FourthDigit)
		{
		case 0: return EItemClass::EIC_WeaponSurpassAnvil;
		case 1: return EItemClass::EIC_ArmorSurpassAnvil;
		case 2: return EItemClass::EIC_AccessorySurpassAnvil;
		}
	}
	else if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_Consumable)
	{
		// 歹固 盖 菊磊府啊 8老 版快 家葛前栏肺 免仿 鞘夸窍扁 锭巩俊 烙狼 铰鞭籍阑 免仿 窃.
		return EItemClass::EIC_UpgradeStone;
	}

	return EItemClass::EIC_End;
}

EPCClass FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(int32 InItemRefID)
{
	const int32 TheBiggestCategoryDigit = ExtractNthDigitR<7>(InItemRefID);
	// 厘馒 酒捞袍, 沥荐(Essence), 内胶片 酒捞袍父 酒贰 痹蘑捞 嘎澜
	if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT || 
		TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_ESSENCE || 
		TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_COSTUME ||
		TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_SURPASSANVIL
		)
	{
		// 第俊辑 6锅掳 磊府
		int32 SixthDigit = ExtractNthDigitR<6>(InItemRefID);
		return ItemAllowedPCClassEnum(SixthDigit);
	}
	else
	{
		return EPCClass::EPC_End;
	}
}

int32 FItemRefIDHelper::ExtractItemStarGradeFromRefID(int32 InItemRefID)
{
	const int32 TheBiggestCategoryDigit = ExtractNthDigitR<7>(InItemRefID);
	// 厘馒 酒捞袍, 沥荐(Essence), 内胶片 酒捞袍父 酒贰 痹蘑捞 嘎澜
	if (TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT ||
		TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_ESSENCE ||
		TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_COSTUME ||
		TheBiggestCategoryDigit == ITEM_REF_ID_LARGEST_DIGIT_SURPASSANVIL
		)
	{
		// 第俊辑 5锅掳 磊府 绞狼磊府 殿鞭
		int32 Grade = 0;
		Grade = (ExtractNthDigitR<ITEM_REF_ID_STAR_TENGRAD_DIGIT_NUMBER>(InItemRefID)) * 10;

		// 第俊辑 3锅掳 磊府 老狼磊府 殿鞭
		Grade += ExtractNthDigitR<ITEM_REF_ID_STAR_GRAD_DIGIT_NUMBER>(InItemRefID);

		return Grade;
	}
	else
	{
		return 0;
	}
}

int32 FItemRefIDHelper::ExtractItemVariationFromRefID(int32 InItemRefID)
{
	// 盖 第 滴磊府
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
	for (int32 Grade = 1; Grade <= FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE; Grade++)
	{
		AllGradeItemIDs.Add(BaseItemID + (Grade * 100));
	}

	return AllGradeItemIDs;
}

bool FItemRefIDHelper::IsEquipmentItemByRefID(int32 InRefID)
{ // 老馆利栏肺绰 IsEquipmentItem 阑 荤侩

	//const int32 IdLargeDigit = ExtractNthDigitR<7>(InRefID);
	//return IdLargeDigit == ITEM_REF_ID_LARGEST_DIGIT_EQUIPMENT;
	return true;
}

bool FItemRefIDHelper::IsEssenceItemByRefID(int32 InRefID) 
{ // 老馆利栏肺绰 IsEssenceItem 阑 荤侩
	return (ExtractNthDigitR<7>(InRefID) == ITEM_REF_ID_LARGEST_DIGIT_ESSENCE); 
}

bool FItemRefIDHelper::IsAnvilItemByRefID(int32 InRefID)
{ // 老馆利栏肺绰 IsAnvilItem 阑 荤侩
	return (ExtractNthDigitR<7>(InRefID) == ITEM_REF_ID_LARGEST_DIGIT_SURPASSANVIL);
}

bool FItemRefIDHelper::IsCostumeItemByRefID(int32 InRefID)
{
	return (ExtractNthDigitR<7>(InRefID) == ITEM_REF_ID_LARGEST_DIGIT_COSTUME);
}

int32 FItemRefIDHelper::GenerateRewardRepresentativeItemID(int32 InRewardGroupID, int32 InType)
{
	// 胶抛捞瘤 沥焊俊辑 焊惑 酒捞袍 措钎 酒捞能俊 荤侩. ItemInfo 抛捞喉俊 殿废等 锅龋 眉拌.
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
	if (InPointType == EItemPrimaryPointType::EIPP_End)
	{
		return FText::FromString(TEXT("")); // 酒公巴档 钎矫 救窍绰 荐窜栏肺 荤侩窍扁 困秦 后 巩厘 府畔.
	}

	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemPrimaryPointType"), true);
	const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/BladeII.EItemPrimaryPointType"));

	if (!EnumPtr)
	{		
#if WITH_EDITOR && !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("EItemPrimaryPointType Enum 菩虐瘤甫 茫阑 荐 绝促. 哪腔磐啊 梆 气惯茄促.")));
#endif
		return FText::FromString(TEXT("Error"));
	}

	// Enum name 阑 咆胶飘 抛捞喉狼 虐肺 荤侩
	FString EnumName = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InPointType));
	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TEXT("ENUM_") + EnumName);
	//return FText::GetEmpty();
}

FText GetLOCTextOfItemOption(EItemOption InOption, EPCClass InPCClass)
{
	if (InOption == EItemOption::EIO_End)
	{
		return FText::FromString(TEXT("")); // 酒公巴档 钎矫 救窍绰 荐窜栏肺 荤侩窍扁 困秦 后 巩厘 府畔.
	}

	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemOption"), true);
	const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/BladeII.EItemOption"));

	if (!EnumPtr)
	{
#if WITH_EDITOR && !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("EItemOption Enum 菩虐瘤甫 茫阑 荐 绝促. 哪腔磐啊 梆 气惯茄促.")));
#endif
		return FText::FromString(TEXT("Error"));
	}

	// Enum name 阑 咆胶飘 抛捞喉狼 虐肺 荤侩..
	FString TextKeyName = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InOption));
	// ..窍绰单 某腐磐 喊肺 啊绰 老何 可记甸篮 某腐磐 捞抚阑 蝶肺 嘿咯淋.
	if (IsItemOptionPerPCClass(InOption))
	{
		//TextKeyName += TEXT("_") + UB2PCClassInfo::GetTrimmedNameOfPCClassEnum(InPCClass);
	}

	TextKeyName = TEXT("ENUM_") + TextKeyName; // 付瘤阜栏肺 立滴绢甫 粱 嘿咯霖促. せ
	return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), TextKeyName);
	//return FText::GetEmpty();
}

FText GetLOCTextOfItemQuality(int32 ItemQuality)
{
	if (ItemQuality > 0 && ItemQuality <= MAX_ITEM_QUALITY)
	{ 
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString::Printf(TEXT("ItemQualityPrefix_%d"), ItemQuality));
	}
	// 蜡瓤茄 前龙 裹困啊 酒囱 版快 酒捞袍 钱 匙烙俊 prefix 肺 荤侩窍瘤 给窍档废 柳累俊 捞吧 妮窍瘤 臼绰 霸 唱阑 巴. FSingleItemInfoData::GetLocalizedName 曼绊
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
	if (InPrimaryPointApplyType == ECharStatApplyType::ECSA_End)
	{
		return FText::FromString(TEXT("")); // 酒公巴档 钎矫 救窍绰 荐窜栏肺 荤侩窍扁 困秦 后 巩厘 府畔.
	}

	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECharStatApplyType"), true);
	const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/BladeII.ECharStatApplyType"));

	if (!EnumPtr)
	{
#if WITH_EDITOR && !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("ECharStatApplyType Enum 菩虐瘤甫 茫阑 荐 绝促. 哪腔磐啊 梆 气惯茄促.")));
#endif
		return FText::FromString(TEXT("Error"));
	}

	// Enum name 阑 咆胶飘 抛捞喉狼 虐肺 荤侩
	FString EnumKey = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InPrimaryPointApplyType));
	FText EnumString =  FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), (TEXT("ENUM_") + EnumKey)), GetLOCTextOfPrimPointType(InCheckPointType));
	return (EnumString.IsEmpty() == false) ? EnumString : FText::FromString(EnumKey);
	//return FText::FromString(TEXT("Error"));
}

FText GetLOCTextOfPrimaryPointApplyType(ECharStatApplyType InPrimaryPointApplyType, EItemOption InOption, EPCClass InPCClass)
{
	if (InPrimaryPointApplyType == ECharStatApplyType::ECSA_End || InOption == EItemOption::EIO_End || InPCClass == EPCClass::EPC_End)
	{
		return FText::FromString(TEXT("")); // 酒公巴档 钎矫 救窍绰 荐窜栏肺 荤侩窍扁 困秦 后 巩厘 府畔.
	}

	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECharStatApplyType"), true);
	const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/BladeII.ECharStatApplyType"));
	if (!EnumPtr)
	{
#if WITH_EDITOR && !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("ECharStatApplyType Enum 菩虐瘤甫 茫阑 荐 绝促. 哪腔磐啊 梆 气惯茄促.")));
#endif
		return FText::FromString(TEXT("Error"));
	}

	// Enum name 阑 咆胶飘 抛捞喉狼 虐肺 荤侩
	FString EnumKey = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InPrimaryPointApplyType));
	FText EnumString = FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), (TEXT("ENUM_") + EnumKey)), GetLOCTextOfItemOption(InOption, InPCClass));
	return (EnumString.IsEmpty() == false) ? EnumString : FText::FromString(EnumKey);
	//return FText::FromString(TEXT("Error"));
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
	@ItemFactorLevel = 辑滚 Table俊 乐绰 酒捞袍 殿鞭(1~6己) 喊 啊吝摹 - FMD_ItemSingleElem::ItemLevel
	@Quality = 弥窍鞭~ 弥惑鞭 (1 ~ 4)
	@Level = 碍拳饭骇 1 ~ 40
*/
float GetPreviewPrimaryPoint(EItemPrimaryPointType PrimaryType, float ItemFactorBase, int32 Quality, int32 FactorLevel)
{
	FClientDataStore& ClientDataStore = BladeIIGameImpl::GetClientDataStore();

	// 饭骇诀矫 付胶磐 单捞磐 扁馆栏肺 PrimaryPoint 搬沥 ( 0.8 Factor 蚌窍绰 荐侥 荤扼咙 )
	const float PrimaryLevelupFactor = ClientDataStore.GetItemLevelupFactor_PrimaryIncrement(FactorLevel);
	const float ItemLevelForEnhance = ItemFactorBase + PrimaryLevelupFactor;
	float qualityIncLevel=0;

	for (int32 i = 0; i < ClientDataStore.MD_LotteryShopRateQualityInfo.Num(); i++)
	{
		if (
			(PrimaryType == EItemPrimaryPointType::EIPP_Attack && ClientDataStore.MD_LotteryShopRateQualityInfo[i].equipCategory == EEquipCategoryType::WEAPON) ||
			(PrimaryType == EItemPrimaryPointType::EIPP_Defense && ClientDataStore.MD_LotteryShopRateQualityInfo[i].equipCategory == EEquipCategoryType::ARMOR) ||
			(PrimaryType == EItemPrimaryPointType::EIPP_Health && ClientDataStore.MD_LotteryShopRateQualityInfo[i].equipCategory == EEquipCategoryType::ACCESSORY)
			)
		{
			if (Quality == ClientDataStore.MD_LotteryShopRateQualityInfo[i].quality)
				qualityIncLevel = ClientDataStore.MD_LotteryShopRateQualityInfo[i].qualityIncLevel;
		}
	}

	// 前龙 茄窜拌付促 0.2究 刘啊
	// 前龙 函券 诀单捞飘肺 前龙俊 蝶弗 瓷仿摹啊 传俊 剁霸 瞒捞唱霸 官柴
	//const float CalculatedItemLevel	 = (0.2f * (Quality - 1)) + ItemLevelForEnhance;
	const float CalculatedItemLevel = (1 + qualityIncLevel) * ItemLevelForEnhance + 0.2f;

	float ReturnValue = 0.f;
	if (PrimaryType == EItemPrimaryPointType::EIPP_Attack)
	{
		// 100 + ( 8 * 酒捞袍饭骇 ) + ( 0.2 * 酒捞袍饭骇  * 酒捞袍饭骇 )
		float AttackValue = 
			(100.f + (8.0f * CalculatedItemLevel) + (0.2f * FMath::Pow(CalculatedItemLevel, 2)));

		ReturnValue = AttackValue;
	}

	else if (PrimaryType == EItemPrimaryPointType::EIPP_Defense)
	{
		// 傍拜仿 * 3 / 4
		float AttackValue =
			(100.f + (8.f * CalculatedItemLevel) + (0.2f * FMath::Pow(CalculatedItemLevel, 2)));

		AttackValue = FMath::FloorToFloat(AttackValue);

		ReturnValue = (AttackValue * 3.0f / 4.0f);
	}
	else if (PrimaryType == EItemPrimaryPointType::EIPP_Health)
	{
		// (1 + ( 3 * 酒捞袍饭骇 ) + ( 0.05 * 酒捞袍饭骇 * 酒捞袍饭骇)) / 3
		ReturnValue =
			((1.0f + (3.0f * CalculatedItemLevel)) + (0.05f * FMath::Pow(CalculatedItemLevel, 2))) / 3.0f;
	}

	return ReturnValue;
}

void GetStandardDummyPCPartItemData(EPCClass InPCClass, int32 InStarGrade, TArray<FB2Item>& OutData, int32 InGroupID)
{
	if (InPCClass == EPCClass::EPC_End || InStarGrade < 1 || InStarGrade > FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE){
		return;
	}

	/*================================================================================
	俺惯 滚傈 扁瓷篮 酒聪瘤父 概快 漂荐茄 扁瓷烙. 
	酒捞袍 RefID 俊 蝶弗 加己 甘俏捞 粱 甸绢皑.
	================================================================================*/

	const int32 RefIDPCClassNum = (int32)InPCClass; // 捞 痹距捞 官诧促搁 某腐磐 家俺 拳搁俊辑 钧蹲茄 皋浆甸狼 炼钦捞 唱棵 抛聪 舅霸 登摆瘤..

	for (int32 ECI = 0; ECI < (int32)EItemClass::EIC_End; ++ECI)
	{
		EItemClass CurrItemClass = static_cast<EItemClass>(ECI);
		
		int32 RefIDItemClassNum = -1; // 捞 痹距捞 官差绢档 钧蹲茄 皋浆甸狼 炼钦捞 唱坷霸 登摆瘤..?
		EItemEquipPlace CurrEquipPlace = EItemEquipPlace::EIEP_End;

		// ExtractItemClassFromRefID 尔 GetItemEquipPlaceFromClass 俊 秦寸窍绰 郴侩牢单
		// 弊巴苞 概莫窍绰瘤 酒贰辑 眉农
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

		// 窃荐 格利 惑 皋浆 炼钦 措惑捞 酒囱 扒 力寇茄促. 骇飘, 格吧捞, 馆瘤 殿
		
		}

		if (CurrEquipPlace != EItemEquipPlace::EIEP_End && RefIDItemClassNum >= 0)
		{
			FB2Item NewItemData;
			// RefID 痹距俊 蝶弗 炼钦牢单 捞巴焊促绰 辑滚俊辑 焊郴林绰 加己蔼捞 快急烙. 咯扁急 鞘夸窍聪鳖 绢驴 荐 绝捞 荤侩.
			NewItemData.ItemRefID = 1000000 + RefIDPCClassNum * 100000 + RefIDItemClassNum * 1000 + InStarGrade * 100 + InGroupID;
			// 皋浆 炼钦俊辑绰 EquipPlace 沥档搁 瞪 巴. 唱赣瘤绰 咯扁 乐绰 单捞磐聪 咯窍埃 盲况淋.
			NewItemData.AllowedPCClass = (int32)InPCClass;
			NewItemData.ItemClass = CurrItemClass;
			NewItemData.EquipPlace = CurrEquipPlace;

			// 炼钦茄 巴捞 痹距俊 嘎绰瘤 眉农.. CheckForItemPropertyRule 阑 荤侩窍扁 傈俊 傲 眉农俊 狐柳 单捞磐绰 盲况霖促.
			NewItemData.StarGrade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(NewItemData.ItemRefID);
			NewItemData.InventoryType = GetItemInvenTypeFromClass(NewItemData.ItemClass);
			BII_CHECK(CheckForItemPropertyRule(NewItemData));

			OutData.Add(NewItemData);
		}
	}
}

bool GetItemIntrinsicOptionOfType(FB2Item& ItemToCheck, FItemOption& OutFoundOption, EItemOption InSearchOption)
{ // InSearchOption 俊 秦寸窍绰 霸 乐绰瘤 茫绰 侩档
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
{ // InSearchOption 俊 秦寸窍绰 霸 乐绰瘤 茫绰 侩档
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
		return true; // 角龙利栏肺 敲饭捞绢 某腐磐 辆幅喊肺 备盒登绰 巴甸烙.
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
	case EItemOption::EIO_Offense_IncAttack: // 傲 荐摹父 钎矫登绰 局甸
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
		checkSlow(OptionApplyType == ECombatOptionApplyType::ECOAT_Add_Value); // 瘤陛鳖瘤 弊犯促绰 芭绊 捞 啊沥捞 撇副 荐档 乐促. 酒囱 版快啊 惯积窍搁 搏官捞搏 措贸.
		break;

	case EItemOption::EIO_Health_PerSecRecoverHP:
		checkSlow(OptionApplyType == ECombatOptionApplyType::ECOAT_Add_Value); // 瘤陛鳖瘤 弊犯促绰 芭绊 捞 啊沥捞 撇副 荐档 乐促. 酒囱 版快啊 惯积窍搁 搏官捞搏 措贸.
		break;
		
	case EItemOption::EIO_Skill_DecSkillCooltime: // 矫埃 包访. 檬窜困 钎矫
	case EItemOption::EIO_Misc_DecTagCooltime:
		UnitText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("TimeUnitSecond"));
		break;

	// 扁鸥 唱赣瘤绰 % 肺
	default:
		// OptionApplyType 捞 Percent 加己牢 版快牢单 趣矫扼档 酒囱 版快啊 惯积窍搁 搏官捞搏 措贸.
		checkSlow(OptionApplyType == ECombatOptionApplyType::ECOAT_Add_Percent || OptionApplyType == ECombatOptionApplyType::ECOAT_OneMinusMultiply);
		UnitText = FText::FromString(TEXT("%"));
		break;
	}

	// 家荐痢 笛掳磊府鳖瘤 倾侩窍登 场磊府 0 篮 蜡瓤箭磊肺 救 摹绊 积帆.
	return FText::Format(FText::FromString(TEXT("{0} {1}")), GetAutoFractionalFormattedText(DisplayValue, 2), UnitText);
}

FText GetSkillOptionValueDisplayText(ESkillOption InOption, float InOptionValue)
{
	ECombatOptionApplyType OptionApplyType = GetCombatOptionApplyType(InOption);
	// EffectiveOptionValue 绰 GetEffectiveItemOptionValue 俊辑 掘篮 吧肺 模促.
	FText TempDisPlayText;
	FText FinalDisplayText;

	// 家荐痢 笛掳磊府鳖瘤 倾侩窍登 场磊府 0 篮 蜡瓤箭磊肺 救 摹绊 积帆.
	if (InOptionValue > 0)
		TempDisPlayText = FText::Format(FText::FromString(TEXT("{0} {1}")), FText::FromString("+"), GetAutoFractionalFormattedText(InOptionValue, 2));
	else
		TempDisPlayText = GetAutoFractionalFormattedText(InOptionValue, 2);


	switch (InOption)
	{

	// 檬甫 嘿捞绰 局甸
	case ESkillOption::ESO_DecCoolTime:
		FinalDisplayText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Sconds")), TempDisPlayText);
		break;
	// 酒公巴档 绝绰局甸
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
	// %甫 嘿捞绰 局甸
	default:
		FinalDisplayText = FText::Format(FText::FromString(TEXT("{0} {1}")), TempDisPlayText,FText::FromString("%"));
		break;
	}

	return FinalDisplayText;
}

float GetEffectiveItemOptionValue(EItemOption InOptionType, const FB2Item& ItemToCheckOption, const double InEnhanceEffectFactor /*= 1.f*/)
{
	TArray<FOptionValue> OptionRawValues;
	for (const FItemOption& CurrOption : ItemToCheckOption.IntrinsicOptions) // 酒付档 窜老 府胶飘俊辑 鞍篮 鸥涝 可记捞 滴俺 捞惑 救 唱坷摆瘤..?
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
	
	// 碍拳 酒捞袍 技飘 瓤苞甫 绊妨窍绰 滚傈栏肺 妮
	return GetEffectiveItemOptionValueWithEnhanceSetEffect(InOptionType, OptionRawValues, InEnhanceEffectFactor);
}

void GenerateGoldItem(FB2Item& OutItem, int32 InGoldAmount)
{
	OutItem.ItemRefID = FItemRefIDHelper::GetGoodsID_Gold();
	OutItem.ItemClass = EItemClass::EIC_Gold;
	OutItem.InventoryType = EItemInvenType::EIIVT_Consumables;
	OutItem.ConsumingAmount = InGoldAmount;
	OutItem.SetPrimaryPoint(InGoldAmount); // 抗傈俊 PrimaryPoint 肺 敬 霸 乐绢辑..
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
		// 焊惑 措钎 酒捞袍狼 版快 ItemClass 绰 困俊 八荤窍绰 巴吝 窍唱瘤父 菊磊府绰 9锅措啊 唱咳. 捞吧 绢痘霸 贸矾秦具 且鳖.. 老窜篮 false 牢 吧肺.
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
{// 饭骇诀篮 促弗 霸 救瞪 版快 啊厘 扁夯.
	// 鞘夸俊 蝶扼 B2mdItemInfo 狼 is_levelupable 阑 努扼肺 啊廉客辑 眠啊 眉农.
	return (IsEquipmentItem(InItem) && !IsItemCompositionAllowed(InItem) && !IsItemUpgradeAllowed(InItem) && !IsItemSurpassAllowed(InItem));
}

bool IsItemCompositionAllowed(const FB2Item& InItem)
{ // 钦己, 铰鞭篮 悼老 炼扒俊辑狼 急琶. 阿 殿鞭 喊 弥绊 饭骇俊辑. 
  // 鞘夸俊 蝶扼 B2mdItemInfo 狼 is_composable 阑 努扼肺 啊廉客辑 眠啊 眉农.
	return (IsEquipmentItem(InItem) && InItem.Level >= InItem.MaxLevel && !IsItemSurpassAllowed(InItem));
}

bool IsItemUpgradeAllowed(const FB2Item& InItem)
{ // 钦己, 铰鞭篮 悼老 炼扒俊辑狼 急琶. 阿 殿鞭 喊 弥绊 饭骇俊辑
  // 鞘夸俊 蝶扼 B2mdItemInfo 狼 is_upgradable 阑 努扼肺 啊廉客辑 眠啊 眉农.
	return (IsEquipmentItem(InItem) && InItem.Level >= InItem.MaxLevel && !IsItemSurpassAllowed(InItem));
}

bool IsItemSurpassAllowed(const FB2Item& InItem)
{ // 弥辆 饭骇俊辑 檬岿捞 倾侩等促.
  // 鞘夸俊 蝶扼 B2mdItemInfo 狼 is_surpassable 阑 努扼肺 啊廉客辑 眠啊 眉农.
	return (IsEquipmentItem(InItem) && InItem.StarGrade >= FItemGradeInfo::MINIMUM_SURPASS_STAR_GRADE && InItem.Level >= InItem.MaxLevel);
}

bool IsItemEnhanceAllowed(const FB2Item& InItem)
{
	return (IsEquipmentItem(InItem) && InItem.EnhanceLevel < MAX_ENHANCE_ITEM_LEVEL);
}

// 檬岿捞 6己俊辑 7己栏肺 疵绢巢俊 蝶扼(菊栏肺档 8己,9己)
// 6己俊辑绰 钦己苞 铰鞭 檬岿捞 茄锅俊 焊咯瘤绰 老捞 积辫
// 弊 殿鞭 备埃 眉农 (泅犁 6己)
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
	FItemDismantleInfo DismantleInfo;
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	bool bEnableDismantle = CliDataStore.GetItemDismantleInfoData(InCheckItem.InventoryType, InCheckItem.StarGrade, DismantleInfo);

	// 焊惑 措钎 酒捞袍狼 版快 ItemClass 绰 困俊 八荤窍绰 巴吝 窍唱瘤父 菊磊府绰 9锅措啊 唱咳. 捞吧 绢痘霸 贸矾秦具 且鳖.. 老窜篮 false 牢 吧肺.
	const bool bRetVal = IsEquipmentItemByType(InCheckItem) && 
		FItemRefIDHelper::IsEquipmentItemByRefID(InCheckItem.ItemRefID) && 
		bEnableDismantle;

	return bRetVal;
}

int32 GetLevel_PreviewItemLevelup(float& OutNewExp, const FB2Item& TargetItem, const TArray<FB2Item>& ItemLevelupIngredients, bool bClampResultToMaxValue)
{
	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();

	float TotalFactor = 0.0f;

	float TargetFactor = CliDataStore.GetItemLevelupFactor_TargetItemGrade(TargetItem.StarGrade);
	
	for (int32 II = 0; II < ItemLevelupIngredients.Num(); ++II)
	{
		const FB2Item& ThisIngred = ItemLevelupIngredients[II];

		float IngredFactor1 = CliDataStore.GetItemLevelupFactor_IngredItemGrade(ThisIngred.StarGrade);
		float IngredFactor2 = CliDataStore.GetItemLevelupFactor_IngredItemEquipPlace(ThisIngred.EquipPlace); // 沥荐 酒捞袍档 EquipPlace 啊 乐促.
		float IngredFactor3 = CliDataStore.GetItemLevelupFactor_IngredItemEnhLv(ThisIngred.Level);
		float IngredFactor4 = CliDataStore.GetItemLevelupFactor_IngredItemSurpCount(ThisIngred.SurpassCount);
		// 概 酒捞袍 饭骇诀 犁丰付促 TargetFactor 档 蚌茄促.
		float ThisIngredTotalFactor = TargetFactor * IngredFactor1 * IngredFactor2 * IngredFactor3 * IngredFactor4;

		if (IsEssenceItem(ThisIngred)) { // 沥荐 酒捞袍篮 例馆 瓤啦.
			ThisIngredTotalFactor *= 0.5f;
		}

		TotalFactor += ThisIngredTotalFactor; // 弥辆利栏肺绰 概 犁丰俊 措茄 factor 拌魂蔼甸阑 歹窃.
	}

	// 老窜 饭骇捞 倔付唱 坷甫瘤 脚版静瘤 臼绊 版氰摹俊 窜鉴洒 歹茄促.
	const float SimpleAddedExp = TargetItem.Exp + TotalFactor;

	const int32 SimpleAddedExp_Floor = FMath::FloorToInt(SimpleAddedExp);
	//const float SimpleAddedExp_Frac = FMath::Frac(SimpleAddedExp); 捞扒 鞘夸绝阑 淀.

	// 饭骇 惑铰篮 菊磊府 荐父怒.
	const int32 LevelToIncrease = SimpleAddedExp_Floor / 100;
	int32 RetLevel = TargetItem.Level + LevelToIncrease; // 捞 矫痢俊辑绰 MaxLevel 肺 努伐橇登瘤 臼篮 蔼

	// 傈眉 版氰摹 惑铰盒俊辑 滴磊复荐 捞窍父 巢辫.
	OutNewExp = SimpleAddedExp - ((float)LevelToIncrease * 100.0f); // 咯扁辑档 沥剐档 巩力啊 瞪 荐 乐唱..

	if (bClampResultToMaxValue && RetLevel >= TargetItem.MaxLevel)
	{
		RetLevel = TargetItem.MaxLevel;
		OutNewExp = 100.0f;
	}

	return RetLevel;
}

/** 饭骇, 版氰摹 殿 扁夯利牢 蔼甸 盲况持栏搁 角力 搬苞 荐摹扼 且 荐 乐绰 可记蔼甸 盲况持绰 窃荐. 橇府轰 侩. */
static void SetPreviewItemResultOptionValuesInternal(const FB2Item& InOriginItem, FB2Item& InOutPreviewItem)
{ // 搬苞 可记蔼甸 寇俊 咯扁辑 荤侩窍绰 扁夯利牢 蔼甸篮 盲况廉 乐绢具 茄促. 


	const float PrimaryPoint = GetPreviewPrimaryPoint(InOutPreviewItem.PrimaryPointType,
													  InOutPreviewItem.ItemFactorLevel,
													  InOutPreviewItem.Quality,
													  InOutPreviewItem.Level);

	InOutPreviewItem.SetPrimaryPoint(PrimaryPoint);


	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();
	int32 LevelDistance = (int32)((InOutPreviewItem.Level - InOriginItem.Level + (InOriginItem.Level % 10) ) / 10);
	// 扁鸥 可记蔼甸..
	for (int32 IOI = 0; IOI < InOutPreviewItem.IntrinsicOptions.Num(); ++IOI)
	{
		FItemOption& ThisOption = InOutPreviewItem.IntrinsicOptions[IOI];
		ThisOption.RawOptionAmount = ThisOption.RawOptionAmount + CliDataStore.GetItemLevelupFactor_PerOptionInc(ThisOption.OptionType) * (float)(LevelDistance);
	}
	for (int32 IOI = 0; IOI < InOutPreviewItem.RandomOptions.Num(); ++IOI)
	{
		FItemOption& ThisOption = InOutPreviewItem.RandomOptions[IOI];
		ThisOption.RawOptionAmount = ThisOption.RawOptionAmount + CliDataStore.GetItemLevelupFactor_PerOptionInc(ThisOption.OptionType) * (float)(LevelDistance);
	}
}

/** 瘤沥茄 犁丰甸阑 啊瘤绊 饭骇诀 矫 搬苞 单捞磐啊 绢痘霸 登绰瘤 橇府轰 */
void GetCalculateItem_PreviewItemLevelup(const FB2Item& InTargetItem, const TArray<FB2Item>& ItemLevelupIngredients, FB2Item& OutPreviewResult, bool bClampResultToMaxValue)
{
	// Note(林狼!) : 泅犁 拌魂 傍侥篮 努扼俊辑 UI惑栏肺 焊咯林扁困茄 侩档肺父 荤侩窍绊乐绊, 角力 傍拜仿 拌魂篮 辑滚俊辑 秦辑 郴妨林绊乐澜. 
	OutPreviewResult = InTargetItem;

	if (ItemLevelupIngredients.Num() == 0)
		return;

	float NewExp = InTargetItem.Exp;
	OutPreviewResult.Level = GetLevel_PreviewItemLevelup(NewExp, InTargetItem, ItemLevelupIngredients, bClampResultToMaxValue);
	OutPreviewResult.Exp = NewExp;

	SetPreviewItemResultOptionValuesInternal(InTargetItem, OutPreviewResult); // 扁夯利牢 单捞磐甫 持篮 饶 搬苞 可记 荐摹甸阑 魂免秦 辰促.

	// 碍拳啊 利侩 救等 扁夯 傍规仿阑 惶酒郴绊 弊 蔼阑 扁霖栏肺 碍拳蔼阑 刘啊秦霖促.
	int32 BasePrimaryPoint = OutPreviewResult.GetPrimaryPoint();
	int32 EnhanceLevel = InTargetItem.EnhanceLevel;

	switch (OutPreviewResult.PrimaryPointType)
	{
		// 傍拜仿 += 傍拜仿 * ((殿瞒拌荐 * 碍拳饭骇) + (殿厚拌荐 * 碍拳饭骇 * 碍拳饭骇))
		case EItemPrimaryPointType::EIPP_Attack:
		case EItemPrimaryPointType::EIPP_Defense:
		{
			BasePrimaryPoint += (BasePrimaryPoint * ((0.02f * EnhanceLevel) + (0.00065f * FMath::Pow(static_cast<float>(EnhanceLevel), 2))));
			OutPreviewResult.SetPrimaryPoint(B2ServerRoundDown(BasePrimaryPoint, 1.f));
		}
			break;
		// 傍拜仿 += (傍拜仿 * ((殿瞒拌荐 * 碍拳饭骇) + (殿厚拌荐 * 碍拳饭骇 * 碍拳饭骇))) + 1
		case EItemPrimaryPointType::EIPP_Health:
		{
			BasePrimaryPoint += ((BasePrimaryPoint * ((0.02f * EnhanceLevel) + (0.00065f * FMath::Pow(static_cast<float>(EnhanceLevel), 2)))) + 1);
			OutPreviewResult.SetPrimaryPoint(B2ServerRoundDown(BasePrimaryPoint, 1.f));
		}
			break;
	}
}

/** 漂沥 荐摹父怒 饭骇诀 矫 搬苞 单捞磐啊 绢痘霸 登绰瘤 橇府轰 */
void GetCalculateItem_PreviewItemLevelup(const FB2Item& InTargetItem, FB2Item& OutPreviewResult, int32 LevelIncreaseAmount /*= 0*/) 
{
	// Note(林狼!) : 泅犁 拌魂 傍侥篮 努扼俊辑 UI惑栏肺 焊咯林扁困茄 侩档肺父 荤侩窍绊乐绊, 角力 傍拜仿 拌魂篮 辑滚俊辑 秦辑 郴妨林绊乐澜. 
	OutPreviewResult = InTargetItem;
	OutPreviewResult.Level += LevelIncreaseAmount;

	SetPreviewItemResultOptionValuesInternal(InTargetItem, OutPreviewResult); // 扁夯利牢 单捞磐甫 持篮 饶 搬苞 可记 荐摹甸阑 魂免秦 辰促.
}

int GetTotalCost_PreviewItemLevelup(const FB2Item& InTargetItem, const TArray<FB2Item>& ItemLevelupIngredients)
{
	int32 LevelUpCost = 0;
	FClientDataStore& DataStore = BladeIIGameImpl::GetClientDataStore();
	for (const FB2Item& Each : ItemLevelupIngredients)
	{
		LevelUpCost += DataStore.GetItemLevelUpCost(Each.InventoryType, Each.StarGrade, Each.Level);
	}
	return LevelUpCost;
}

void GetCalculateItem_PreviewItemEnhance(const FB2Item& InTargetItem, FB2Item& OutPreviewResult, int32 IncreaseEnhanceLevel /*= 0*/, float manualFactor)
{
	OutPreviewResult = InTargetItem;
	if (manualFactor != 0)
	{
		OutPreviewResult.ItemFactorLevel = manualFactor;
	}
	FB2Item CalcLevelUpItem;
	GetCalculateItem_PreviewItemLevelup(OutPreviewResult, CalcLevelUpItem);
	
	// 碍拳啊 利侩 救等 扁夯 傍规仿阑 惶酒郴绊 弊 蔼阑 扁霖栏肺 碍拳蔼阑 刘啊秦霖促.
	int32 BasePrimaryPoint = CalcLevelUpItem.GetPrimaryPoint();
	int32 EnhanceLevel = InTargetItem.EnhanceLevel + IncreaseEnhanceLevel;

	switch (CalcLevelUpItem.PrimaryPointType)
	{
		// 傍拜仿 += 傍拜仿 * ((殿瞒拌荐 * 碍拳饭骇) + (殿厚拌荐 * 碍拳饭骇 * 碍拳饭骇))
	case EItemPrimaryPointType::EIPP_Attack:
	case EItemPrimaryPointType::EIPP_Defense:
		{
			BasePrimaryPoint += (BasePrimaryPoint * ((0.02f * EnhanceLevel) + (0.00065f * FMath::Pow(static_cast<float>(EnhanceLevel), 2))));
			OutPreviewResult.SetPrimaryPoint(B2ServerRoundDown(BasePrimaryPoint, 1.f));
		}
		break;
		// 傍拜仿 += (傍拜仿 * ((殿瞒拌荐 * 碍拳饭骇) + (殿厚拌荐 * 碍拳饭骇 * 碍拳饭骇))) + 1
	case EItemPrimaryPointType::EIPP_Health:
		{
			BasePrimaryPoint += ((BasePrimaryPoint * ((0.02f * EnhanceLevel) + (0.00065f * FMath::Pow(static_cast<float>(EnhanceLevel), 2)))) + 1);
			OutPreviewResult.SetPrimaryPoint(B2ServerRoundDown(BasePrimaryPoint, 1.f));
		}
		break;
	}
}

void GetCalculateCostumeItem_PreviewItemEnhance(const FB2Item& InTargetItem, FB2Item& OutPreviewResult, int32 IncreaseEnhanceLevel /*= 0*/)
{
	//OutPreviewResult = InTargetItem;

	//FB2Item CalcLevelUpItem;
	//GetCalculateItem_PreviewItemLevelup(InTargetItem, CalcLevelUpItem);

	//// 碍拳啊 利侩 救等 扁夯 傍规仿阑 惶酒郴绊 弊 蔼阑 扁霖栏肺 碍拳蔼阑 刘啊秦霖促.
	//int32 BasePrimaryPoint = CalcLevelUpItem.GetPrimaryPoint();
	//int32 EnhanceLevel = InTargetItem.EnhanceLevel + IncreaseEnhanceLevel;

	//switch (CalcLevelUpItem.PrimaryPointType)
	//{
	//// 傍拜仿 = 公扁 扁夯 傍拜仿 + 公扁碍拳傍拜仿(200+0.5*扁夯 傍拜仿*碍拳 饭骇+50*碍拳 饭骇*碍拳 饭骇)
	//case EItemPrimaryPointType::EIPP_Attack:
	//{
	//	BasePrimaryPoint += (200 + (0.5 * BasePrimaryPoint * EnhanceLevel) + (50*EnhanceLevel*EnhanceLevel));
	//	OutPreviewResult.SetPrimaryPoint(B2ServerRoundDown(BasePrimaryPoint, 1.f));
	//}
	//break;
	//// 规绢仿 =  规绢备 扁夯 规绢仿 + 规绢备碍拳规绢仿(400+0.5*扁夯 规绢仿*碍拳 饭骇+100*碍拳 饭骇*碍拳 饭骇)
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
	酒捞袍狼 RefID 磊复荐 沥焊贸烦 漂沥 加己阑 啊瘤绊 促弗 加己阑 蜡眠窍绰 窃荐.
	FItemRefIDHelper 狼 Extract** 揪府令贸烦 怖 鞘夸茄 版快父.
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

	return EItemInvenType::EIIVT_Consumables; // 唱赣瘤绰 家葛前栏肺
}

EItemEquipPlace GetItemEquipPlaceFromClass(EItemClass InItemClass)
{
	switch (InItemClass)
	{
		// 芭狼 老措老 甘俏
	case EItemClass::EIC_Weapon: return EItemEquipPlace::EIEP_Weapon;

	case EItemClass::EIC_Helmet: return EItemEquipPlace::EIEP_Helmet;
	case EItemClass::EIC_BodyArmor: return EItemEquipPlace::EIEP_BodyArmor;
	case EItemClass::EIC_Glove: return EItemEquipPlace::EIEP_Glove;
	case EItemClass::EIC_Shoes: return EItemEquipPlace::EIEP_Shoes;

	case EItemClass::EIC_Belt: return EItemEquipPlace::EIEP_Belt;
	case EItemClass::EIC_Necklace: return EItemEquipPlace::EIEP_Necklace;
	case EItemClass::EIC_Ring: return EItemEquipPlace::EIEP_Ring;
	}

	return EItemEquipPlace::EIEP_End; // 唱赣瘤绰 厘馒 阂啊瓷
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
	if (FItemRefIDHelper::IsEquipmentItemByRefID(InItemToCheck.ItemRefID)) // 菊磊府 1 捞 老馆利牢 厘馒 酒捞袍. 泅犁 逢 眉农 措惑
	{
		bool bFoundMisMatch = false;
		bFoundMisMatch |= (InItemToCheck.ItemClass != FItemRefIDHelper::ExtractItemClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.AllowedPCClass != (int32)FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.StarGrade != FItemRefIDHelper::ExtractItemStarGradeFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.InventoryType != GetItemInvenTypeFromClass(InItemToCheck.ItemClass));
		bFoundMisMatch |= (InItemToCheck.EquipPlace != GetItemEquipPlaceFromClass(InItemToCheck.ItemClass));
		
		return !bFoundMisMatch;
	}
	else if (FItemRefIDHelper::IsEssenceItemByRefID(InItemToCheck.ItemRefID)) // 沥荐 酒捞袍档 厚搅茄 眉农
	{
		bool bFoundMisMatch = false;
		bFoundMisMatch |= (InItemToCheck.ItemClass != FItemRefIDHelper::ExtractItemClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.AllowedPCClass != (int32)FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.StarGrade != FItemRefIDHelper::ExtractItemStarGradeFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.InventoryType != GetItemInvenTypeFromClass(InItemToCheck.ItemClass));

		return !bFoundMisMatch;
	}
	else if (FItemRefIDHelper::IsCostumeItemByRefID(InItemToCheck.ItemRefID)) // 菊磊府 3牢 内胶片 酒捞袍
	{
		bool bFoundMisMatch = false;
		bFoundMisMatch |= (InItemToCheck.ItemClass			!= FItemRefIDHelper::ExtractItemClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.AllowedPCClass		!= (int32)FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.StarGrade			!= FItemRefIDHelper::ExtractItemStarGradeFromRefID(InItemToCheck.ItemRefID));
		bFoundMisMatch |= (InItemToCheck.InventoryType		!= GetItemInvenTypeFromClass(InItemToCheck.ItemClass));
		bFoundMisMatch |= (InItemToCheck.CostumeEquipPlace	!= GetItemCostumeEquipPlaceFromClass(InItemToCheck.ItemClass));
		return !bFoundMisMatch;
	}
	else if (FItemRefIDHelper::IsAnvilItemByRefID(InItemToCheck.ItemRefID)) // 檬岿 酒捞袍档 厚搅茄 眉农
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
	// RefID 绰 荤傈俊 沥秦廉 乐绢具 窃.
	
	InItemToLoad.StarGrade = FItemRefIDHelper::ExtractItemStarGradeFromRefID(InItemToLoad.ItemRefID);
	InItemToLoad.AllowedPCClass = (int32)FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(InItemToLoad.ItemRefID);

	if (FItemRefIDHelper::IsEquipmentItemByRefID(InItemToLoad.ItemRefID))
	{
		InItemToLoad.ItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(InItemToLoad.ItemRefID);
		InItemToLoad.MaxLevel = 30; // 傲 面盒洒 臭霸 淋
		InItemToLoad.InventoryType = GetItemInvenTypeFromClass(InItemToLoad.ItemClass);
		InItemToLoad.EquipPlace = GetItemEquipPlaceFromClass(InItemToLoad.ItemClass);
	}
	else if (FItemRefIDHelper::IsEssenceItemByRefID(InItemToLoad.ItemRefID))
	{ // 沥荐 酒捞袍 Class 犬沥 傈 内靛. 绢瞒乔 捞 窃荐 磊眉啊 俺惯侩.
		InItemToLoad.ItemClass = FItemRefIDHelper::ExtractItemClassFromRefID(InItemToLoad.ItemRefID);
		InItemToLoad.InventoryType = GetItemInvenTypeFromClass(InItemToLoad.ItemClass); 
	}
	else if (FItemRefIDHelper::IsAnvilItemByRefID(InItemToLoad.ItemRefID))
	{ // 沥荐 酒捞袍 Class 犬沥 傈 内靛. 绢瞒乔 捞 窃荐 磊眉啊 俺惯侩.
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

	return GetMaxPCClassNum(); // 拱沸 抗寇 贸府 狼固烙. 捞吧 罐霸 登搁 PCItems 牢郸胶肺 静搁 救凳.
}
