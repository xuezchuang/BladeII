#pragma once
//#include "BladeII.h"
#include "CommonStruct.h"
#include "EngineMinimal.h"
#include "B2UIMsgPopupBase.h"
#include "../DataStore/B2ClientDataStore.h"

#include "B2UIUtil.generated.h"

UCLASS(Config = Game)
class BLADEII_API UB2UIUtilAssetContainer : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(config)
		FSoftObjectPath GuestWarningPopupAsset;
};

//Unreal에서 UClass로 선언된 얘들은 Namespace를 사용 할 수 없다....
class B2UIUtil
{
public:
	static bool IsLimitedBuySummonSlot(int32 SummonItemSlotHashKey);
	static EItemInvenType GetItemEquipPlaceToItemInventoryType(EItemEquipPlace EquipPlace);

	static TWeakObjectPtr<class UB2UIMsgPopupInventoryFull> OpenInventoryFullPopup(EPCClass PCClass, const FMsgPopupOnClick& OverrideMoveInven = 0, bool ShowExtension = true);
	static int32 DifferenceItemPrimaryPoint(const FB2Item& Origin, const FB2Item& Comparer);

	static bool HasLevelUpSkillPoint(int32 InSkillID, int32 InSkillLevel);

	//1 Day, Hour 2 Hour, Min 3 Min, Second 4 Second
	static FText TwoStepTimeSpanToString(FTimespan InTimeSpan);

	//Open Guest Warning Popup
	static class UB2UIMsgPopupGuestWarning* OpenGuestWarningPopup(class UB2UnitedWidgetBase* OwnerUserWidget);

	static bool CheckPayableStoreProduct(const FStoreProductData* CheckProductData);
	static void OpenCostTypeErrorPopup(EStoreItemCost InCostType);

	static int32 GetGuildBuffSortValue(EItemOption InItemOption);
};

USTRUCT()
struct FB2ClassPortraitImage
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		EPCClass CharClass;
	UPROPERTY(EditAnywhere)
		class UMaterialInterface* Portrait;

	static UMaterialInterface* GetCharImage(const TArray<FB2ClassPortraitImage>& ImageInfos, EPCClass PCClass);
};

class B2SlotSliderLogic
{
public:
	B2SlotSliderLogic();

	void SetData(int32 InCurrentSlotCount, int32 InSlotCountPerSingleExtend, int32 InMaxAllowedSlotCount);

	int32 SliderValueToExtendCount(float SliderValue);
	float ExtendCountToSliderValue(int32 ExtendCount);
	int32 GetRealExtendCount(float SliderValue);

	int32 GetSlotCount(int32 ExtendCount);
	
	FORCEINLINE float GetSliderStepSize() { return SliderStepSize; }
	FORCEINLINE int32 GetMaxExtendCount() { return MaxExtendCount; }
	FORCEINLINE int32 GetMaxAllowedSlotCount() { return MaxAllowedSlotCount; }
	FORCEINLINE int32 GetCurrentSlotCount() { return CurrentSlotCount; }
	FORCEINLINE bool IsAlreadyMaxSlotCount() { return MaxAllowedSlotCount <= CurrentSlotCount; }
private:
	FORCEINLINE int32 DomainExtendCount(int32 ExtendCount) { return FMath::Clamp(ExtendCount, 0, MaxExtendCount);}

	int32	CurrentSlotCount;

	int32	SliderMinSlotCount;
	int32	MaxAllowedSlotCount;

	int32	SlotCountPerSingleExtend;
	int32	MaxExtendCount;

	float	SliderStepSize;
};