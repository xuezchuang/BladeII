#pragma once

#include "B2UIDocBase.h"
#include "B2UIStoreMileage.h"
#include "B2UIDocStore.generated.h"

struct FPackageLevelupMission
{
	FPackageLevelupMission(int32 id, int32 level, EPCClass pcclass, int32 state)
		: Package_Id(id)
		, Package_Level(level)
		, Package_Class(pcclass)
		, Package_State(state)
		{
		}

	explicit FPackageLevelupMission()
		: Package_Id(0)
		, Package_Level(0)
		, Package_Class(EPCClass::EPC_End)
		, Package_State(0)
	{
	}

	int32 Package_Id;
	int32 Package_Level;
	EPCClass Package_Class;
	int32 Package_State;

	FORCEINLINE static int32 GetKey(int32 InPackageID, int32 InLevel) { return ((InPackageID * 1000) + InLevel); }
	FORCEINLINE int32 GetMyKey() { return GetKey(Package_Id, Package_Level); }
};

struct FPackageReachLevelMission
{
	FPackageReachLevelMission(int32 PackageID_, EPCClass PCClass_, int32 Level_, int32 State_, int64 ExpireTime_)
		: PackageID(PackageID_)
		, PCClass(PCClass_)
		, Level(Level_)
		, State(State_)
		, ExpireTime(ExpireTime_)
	{
	}

	explicit FPackageReachLevelMission()
		: PackageID(0)
		, PCClass(EPCClass::EPC_End)
		, Level(0)
		, State(0)
		, ExpireTime(0)
	{
	}

	int32 PackageID;
	EPCClass PCClass;
	int32 Level;
	int32 State;
	int64 ExpireTime;

};

struct FPackageFlatrateBonusState
{
	FPackageFlatrateBonusState(int32 CurrentIndex_, bool WeeklyState_, bool MonthlyState_, bool RewardState_)
		: CurrentIndex(CurrentIndex_)
		, WeeklyState(WeeklyState_)
		, MonthlyState(MonthlyState_)
		, RewardState(RewardState_)
	{
	}

	explicit FPackageFlatrateBonusState()
		: CurrentIndex(0)
		, WeeklyState(false)
		, MonthlyState(false)
		, RewardState(false)
	{
	}

	int32 CurrentIndex;
	bool WeeklyState;
	bool MonthlyState;
	bool RewardState;
};

struct FMagicShopItem
{
	FMagicShopItem(int32 ProductID_, bool IsPurchased_)
		: ProductID(ProductID_)
		, IsPurchased(IsPurchased_)
	{
	}

	explicit FMagicShopItem()
		: ProductID(0)
		, IsPurchased(false)
	{
	}

	int32 ProductID;
	bool IsPurchased;
};

struct FShopProductSaleTime
{
	FShopProductSaleTime(int64 StartTime_, int64 EndTime_)
		: StartTime(StartTime_)
		, EndTime(EndTime_)
	{
	}

	explicit FShopProductSaleTime()
		: StartTime(0)
		, EndTime(0)
	{
	}

	int64 StartTime;
	int64 EndTime;
};

namespace EStoreMileageType
{
	const int32 SummonItem = 0;
	const int32 BladePoint = 1;
	const int32 Gem = 2;
	const int32 Gold = 3;
	const int32 DarkCrystal = 4;
	const int32 Package = 5;
}

namespace EStoreMileageBoxType
{
	const int32 MileageBoxB = 0;
	const int32 MileageBoxA = 1;
	const int32 MileageBoxS = 2;
}

DECLARE_MULTICAST_DELEGATE(FOnMileageChanged);

#define DECLARE_STOREMILEAGE_DOC_FIELD(TYPE,NAME)														\
		public:																							\
		void Set##NAME( TYPE Value )																	\
		{																								\
			TYPE PreValue = NAME;																		\
			NAME = Value;																				\
			OnMileageChanged.Broadcast();																\
		}																								\
		TYPE Get##NAME() const																			\
		{																								\
			return NAME;																				\
		}																								\
		protected:																						\
		TYPE NAME;

DECLARE_MULTICAST_DELEGATE(FOnGeneralShopChanged);

UCLASS()
class BLADEII_API UB2UIDocStore : public UB2UIDocBase
{
	GENERATED_UCLASS_BODY()

	DECLARE_STOREMILEAGE_DOC_FIELD(int32, SelectBoxHashKey);

	DECLARE_STOREMILEAGE_DOC_FIELD(int32, SummonItemMileage);
	DECLARE_STOREMILEAGE_DOC_FIELD(int32, BladePointMileage);
	DECLARE_STOREMILEAGE_DOC_FIELD(int32, GemMileage);
	DECLARE_STOREMILEAGE_DOC_FIELD(int32, GoldMileage);
	DECLARE_STOREMILEAGE_DOC_FIELD(int32, DarkCrystalMileage);
	DECLARE_STOREMILEAGE_DOC_FIELD(int32, PackageMileage);

	DECLARE_STOREMILEAGE_DOC_FIELD(int32, SummonItemMileageReceivedIndex);
	DECLARE_STOREMILEAGE_DOC_FIELD(int32, BladePointMileageReceivedIndex);
	DECLARE_STOREMILEAGE_DOC_FIELD(int32, GemMileageReceivedIndex);
	DECLARE_STOREMILEAGE_DOC_FIELD(int32, GoldMileageReceivedIndex);
	DECLARE_STOREMILEAGE_DOC_FIELD(int32, DarkCrystalMileageReceivedIndex);
	DECLARE_STOREMILEAGE_DOC_FIELD(int32, PackageMileageReceivedIndex);

	DECLARE_UIDOC_FIELD(int32, StoreTabToOpen);
	DECLARE_UIDOC_FIELD(int32, StoreGoodsTabToOpen);
	DECLARE_UIDOC_FIELD(ESummonItemTabMenu, SummonItemTabToOpen);

	DECLARE_UIDOC_FIELD(int32, PackageTabToOpen);

	DECLARE_UIDOC_FIELD(int32, CurrentSummonItemSlotHashKey);
	DECLARE_UIDOC_FIELD(EPCClass, CurrentSummonItemSelectedPCClass);

	DECLARE_UIDOC_FIELD(FString, LastLotteryKey);

	// Magic Store
	DECLARE_UIDOC_FIELD(int32, RenewalCount);
	DECLARE_UIDOC_FIELD(int64, NextFreeRenewalTime);
	DECLARE_UIDOC_FIELD(bool, IsFreeRenewal);

private:

	int32												CurrentMileageType;

	TArray<EUIStoreMileageRewardState>					MileageRewardState;

	TMap<int32, int32>									RestrictCountMap;
	TMap<int32, int32>									ProductMarkMap;
	TMap<int32, FShopProductSaleTime>					ProductSalePeriodMap;
	TMap<int32, int32>									PackageProductMarkMap;

	TMap<int32, int64>									PeriodicPackageMap;
	TMap<int32, FPackageLevelupMission>					LevelupPackageMissionMap;
	TMap<int32, FPackageReachLevelMission>				ReachLevelMissionMap;
	FPackageFlatrateBonusState							PackageFlatrateBonusState;

	TArray<FMagicShopItem>								MagicShopItemList;
	
public:

	void SetCurrentMileageType(int32 ToSet) { CurrentMileageType = ToSet; }
	int32 GetCurrentMileageType() { return CurrentMileageType; }

	int32 GetMileagePoint(int32 MileageType);
	void SetMileagePoint(int32 MileageType, int32 ToSetPoint);

	int32 GetMileageReceivedIndex(int32 MileageType);
	void SetMileageReceivedIndex(int32 MileageType, int32 ToSetIndex);

	TArray<EUIStoreMileageRewardState> GetMileageRewardState();
	bool GetEnableMileageRewardState(int32 BoxGrade);
	void SetMileageRewardState(TArray<EUIStoreMileageRewardState> ToSetState);

	bool HasUnRewardeMileage(int32 MileageType);

	FOnMileageChanged OnMileageChanged;

	int32 GetReceivedRestrictCount(int32 ProductId);
	void SetReceivedRestrictCount(int32 ProductId, int32 ToSetCount);

	int32 GetProductMark(int32 ProductId);
	void SetProductMark(int32 ProductId, int32 ToSetMark);

	bool GetProductSalePeriod(int32 ProductId, int64& StartTime,int64& EndTime);
	void SetProductSalePeriod(int32 ProductId, int64 StartTime, int64 EndTime);

	int32 GetPackageProductMark(int32 ProductId);
	void SetPackageProductMark(int32 ProductId, int32 ToSetMark);

	TArray<int32> GetReceivedPeriodicPackageProductIDs();
	int64 GetReceivedPeriodicPackageLeftTime(int32 ProductId);
	bool HasReceivedPeriodicPackage();
	bool IsPeriodicPackage(int32 ProductId);
	bool IsPeriodicSale(int32 ProductId);
	void SetReceivedPeriodicPackageLeftTime(int32 ProductId, int64 ToSetPeriod);

	FPackageLevelupMission* GetReceivedLevelupPacakgeMission(int32 ProductId, int32 Level);
	void SetReceivedLevelupPacakgeMissions(int32 ProductId, FB2LevelUpPackageMissionPtr missionPtr);

	bool GetReddotPackageLevelMissionByPCClass(EPCClass PCClass);
	bool GetReddotPackageLevelUpMissionByPackageID(int32 PacakgeID);

	FPackageReachLevelMission* GetReceivedReachLevelPacakgeMissions(int32 ProductId);
	void SetReceivedReachLevelPacakgeMissions(int32 ProductId, FB2ReachLevelPackageMissionPtr MissionPtr);

	FPackageFlatrateBonusState GetReceivedFlatratePackageBonusState();
	void SetReceivedFlatratePackageBonusState(FB2FlatrateBonusStatePtr FlatrateBonusStatePtr);

	// Magic Store
	void ResetMagicShopList();
	TArray<FMagicShopItem> GetMagicShopItemList();
	void SetMagicShopList(FB2ResponseMagicShopPtr MagicShopPtr);

	FOnGeneralShopChanged OnGeneralShopChanged;
};
	