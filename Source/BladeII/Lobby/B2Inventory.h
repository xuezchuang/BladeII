// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "UObject/Object.h"
#include "BladeIIPlayer.h"

#include "B2MessageInfo.h"
#include "CommonStruct.h"
//#include "B2ClientDataStore.h"
#include "B2Inventory.generated.h"


const int32 MAX_ITEM_INTRINSIC_OPTION = 4;
const int32 MAX_ITEM_RANDOM_OPTION = 4;
const int32 MAX_ITEM_QUALITY = 5;	//1: 일반, 2: 중급, 3: 상급, 4: 최상급, 5: 특급
const int32 MAX_ITEM_SET_EFFECT = 3;


const int32 ITEM_REF_ID_STAR_GRAD_DIGIT_NUMBER = 3;
const int32 ITEM_REF_ID_STAR_TENGRAD_DIGIT_NUMBER = 5;

/** InPCClass is necessary for some item options which can be applied per-PC class (Refer to IsItemOptionPerPCClass). Otherwise it does nothing. */
FText GetLOCTextOfItemOption(EItemOption InOption, EPCClass InPCClass);
FText GetLOCTextOfPrimPointType(EItemPrimaryPointType InPointType);
FText GetLOCTextOfItemQuality(int32 ItemQuality);
FText GetLOCTextOfSetItemName(int32 GroupID);
FText GetLOCTextOfStageBuffItem(EStageBuffType InStageBuffType);
FText GetLOCTextOfPrimaryPointApplyType(ECharStatApplyType InPrimaryPointApplyType);
FText GetLOCTextOfPrimaryPointApplyType(ECharStatApplyType InPrimaryPointApplyType, EItemPrimaryPointType InCheckPointType);
FText GetLOCTextOfPrimaryPointApplyType(ECharStatApplyType InPrimaryPointApplyType, EItemOption InOption, EPCClass InPCClass);


/** To get EPCClass enum from FB2Item's int type AllowedPCClass. */
EPCClass ItemAllowedPCClassEnum(int32 InPCClassID);

float GetPreviewPrimaryPoint(EItemPrimaryPointType PrimaryType, float ItemFactorBase, int32 Quality, int32 FactorLevel);

/** It returns item list having RefIDs calculated by certain rule. It is for special purpose, so the result data is not fully valid. 
 * It is assumed that only part of FB2Item members are to be used. */
void GetStandardDummyPCPartItemData(EPCClass InPCClass, int32 InStarGrade, TArray<FB2Item>& OutData, int32 InGroupID = 0);

/** Get this item's intrinsic Option of designated type, if has one. OutFoundOption is valid only when it returns true. */
bool GetItemIntrinsicOptionOfType(FB2Item& ItemToCheck, FItemOption& OutFoundOption, EItemOption InSearchOption);
/** Get this item's random Option of designated type, if has one. OutFoundOption is valid only when it returns true. */
bool GetItemRandomOptionOfType(FB2Item& ItemToCheck, FItemOption& OutFoundOption, EItemOption InSearchOption);

bool IsItemOptionPerPCClass(EItemOption InOption);

float GetConvertItemOptionValue(EItemOption InOption, float InOptionValue, bool bIsRawValue);

/** For UI text. If bIsRawValue is false InOptionValue is what you get from GetEffectiveItemOptionValue, or InOptionValue is just the raw value */
FText GetItemOptionValueDisplayText(EItemOption InOption, float InOptionValue, bool bIsRawValue); 
FText GetSkillOptionValueDisplayText(ESkillOption InOption, float InOptionValue);
/** Get accumulated and scaled option value to be effectively applied to equipped character, from an item. 
 * If designated item does not have the option, it will return different base value depend on the option type  */
float GetEffectiveItemOptionValue(EItemOption InOptionType, const FB2Item& ItemToCheckOption, const double InEnhanceEffectFactor = 1.f);

/** Get associated item option type that increases designated InCheckType */

// Make FB2Item instance representing designated amount of gold
void GenerateGoldItem(FB2Item& OutItem, int32 InGoldAmount);

bool IsEquipmentItemByType(const FB2Item& InCheckItem);
// False means consumables or something else.
bool IsEquipmentItem(const FB2Item& InCheckItem);
FORCEINLINE bool IsEssenceItemClass(EItemClass InCheckClass) { return (InCheckClass == EItemClass::EIC_WeaponEssence || InCheckClass == EItemClass::EIC_ProtectionEssence || InCheckClass == EItemClass::EIC_AccessoryEssence); }
bool IsEssenceItem(const FB2Item& InCheckItem);
FORCEINLINE bool IsAnvilItemClass(EItemClass InCheckClass) { return (InCheckClass == EItemClass::EIC_WeaponSurpassAnvil || InCheckClass == EItemClass::EIC_ArmorSurpassAnvil || InCheckClass == EItemClass::EIC_AccessorySurpassAnvil); }
bool IsAnvilItem(const FB2Item& InCheckItem);
bool IsGoldItem(const FB2Item& InCheckItem);
bool IsBossPieceItem(const FB2Item& InCheckItem);
bool IsEtherItem(const FB2Item& InCheckItem);
bool IsMaterialItem(const FB2Item& InCheckItem);
bool IsCostumeItem(const FB2Item& InCheckItem);

/** Check if Item Levelup of InItem is allowed according to InItem's state. */
bool IsItemLevelupAllowed(const FB2Item& InItem);
/** Check if Composition of InItem is allowed according to InItem's state. */
bool IsItemCompositionAllowed(const FB2Item& InItem);
/** Check if Upgrade of InItem is allowed according to InItem's state. */
bool IsItemUpgradeAllowed(const FB2Item& InItem);
/** Check if Surpass of InItem is allowed according to InItem's state. */
bool IsItemSurpassAllowed(const FB2Item& InItem);
/** Check if Item Enhance of InItem is allowed according to InItem's state. */
bool IsItemEnhanceAllowed(const FB2Item& InItem);

bool IsItemAllowedCompositeAndSurpass(const FB2Item& InItem);

bool IsItemSetEffectAvailable(const FB2Item& InItem);

bool IsItemSafeEnhanceLevel(const FB2Item& InItem);

bool IsItemDecompostionAllowed(const FB2Item& InItem);

/** Calculate item's levelup level (and new exp considering level-up, to ref parameter) given target and ingredients data.
 * This is "preview", because the actual levelup operation is done at server, but there's some case that client itself need to calculate it. */
int32 GetLevel_PreviewItemLevelup(float& OutNewExp, const FB2Item& TargetItem, const TArray<FB2Item>& ItemLevelupIngredients, bool bClampResultToMaxValue = false);

/** It returns item data expected after levelup (InTargetItem + ItemLevelupIngredients), through OutPreviewResult,
 * It also include the result of GetLevel_PreviewItemLevelup. */
void GetCalculateItem_PreviewItemLevelup(const FB2Item& InTargetItem, const TArray<FB2Item>& ItemLevelupIngredients, FB2Item& OutPreviewResult, bool bClampResultToMaxValue = false);
void GetCalculateItem_PreviewItemLevelup(const FB2Item& InTargetItem, FB2Item& OutPreviewResult, int32 IncreaseLevel = 0);
/** Simply returns expected levelup cost. */
int32 GetTotalCost_PreviewItemLevelup(const FB2Item& InTargetItem, const TArray<FB2Item>& ItemLevelupIngredients);

void GetCalculateItem_PreviewItemEnhance(const FB2Item& InTargetItem, FB2Item& OutPreviewResult, int32 IncreaseEnhanceLevel = 0, float manualFactor = 0);
void GetCalculateCostumeItem_PreviewItemEnhance(const FB2Item& InTargetItem, FB2Item& OutPreviewResult, int32 IncreaseEnhanceLevel = 0);

// Provide similar kind of utility like ExtractItem**. Do not use them to much, only when necessary.
EItemInvenType GetItemInvenTypeFromClass(EItemClass InItemClass);
EItemEquipPlace GetItemEquipPlaceFromClass(EItemClass InItemClass);
ECostumeEquipPlace GetItemCostumeEquipPlaceFromClass(EItemClass InItemClass);

// Check if rules by ItemRefID utilities are valid.
bool CheckForItemPropertyRule(const FB2Item& InItemToCheck); 


#if !UE_BUILD_SHIPPING
/** Fill up some of data based on ItemRefID (and some on random stuff..) */
void DevOnlyLoadItemClientTestData(FB2Item& InItemToLoad);
#endif

int32 GetSetItemUniqueKey(int32 SetGroupID, EPCClass CharacterClass, int32 Grade);
int32 GetSetItemUniqueKey(int32 SetGroupID, int32 ItemTemplateID);
int32 GetSetGroupIDFromUniqueKey(int32 SetUniqueKey);
int32 GetSetGradeFromUniqueKey(int32 SetUniqueKey);
EPCClass GetSetCharacterClassFromUniqueKey(int32 SetUniqueKey);
EItemInvenType GetSetOptionInvenType(int32 SetOptionID);


/** A struct to be temporarily used to handle adding acquired items to inventory. */
USTRUCT()
struct FInvenPostCombatItems
{
	GENERATED_USTRUCT_BODY()

	FInvenPostCombatItems(){}

	TArray<FB2Item> Items;
};

/**
 * An inventory class base for in-game and lobby, could be just a dummy to provide a coding place for B2Item..
 */
UCLASS()
class BLADEII_API UB2Inventory : public UObject
{
	GENERATED_BODY()

public:

	UB2Inventory(const FObjectInitializer& ObjectInitializer);


	/** Conversion between EPCClass enum to int index.. to be used to some array */
	static int32 GetInvenPCIndexFromPCClass(EPCClass InPCClass);
};

