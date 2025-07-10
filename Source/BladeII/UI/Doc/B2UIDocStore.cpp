#include "B2UIDocStore.h"


#include "BladeIIGameImpl.h"

UB2UIDocStore::UB2UIDocStore(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DocType = EUIDocType::Store;

	SummonItemMileageReceivedIndex = -1;
	BladePointMileageReceivedIndex = -1;
	GemMileageReceivedIndex = -1;
	GoldMileageReceivedIndex = -1;
	DarkCrystalMileageReceivedIndex = -1;
	PackageMileageReceivedIndex = -1;

	RestrictCountMap.Empty();
	ProductMarkMap.Empty();
	PackageProductMarkMap.Empty();
}

int32 UB2UIDocStore::GetMileagePoint(int32 MileageType)
{
	switch (MileageType)
	{
	case EStoreMileageType::SummonItem:
		return SummonItemMileage;
	case EStoreMileageType::BladePoint:
		return BladePointMileage;
	case EStoreMileageType::Gem:
		return GemMileage;
	case EStoreMileageType::Gold:
		return GoldMileage;
	case EStoreMileageType::DarkCrystal:
		return DarkCrystalMileage;
	case EStoreMileageType::Package:
		return PackageMileage;
	}

	return -1;
}

void UB2UIDocStore::SetMileagePoint(int32 MileageType, int32 ToSetPoint)
{
	switch (MileageType)
	{
	case EStoreMileageType::SummonItem:
		return SetSummonItemMileage(ToSetPoint);
	case EStoreMileageType::BladePoint:
		return SetBladePointMileage(ToSetPoint);
	case EStoreMileageType::Gem:
		return SetGemMileage(ToSetPoint);
	case EStoreMileageType::Gold:
		return SetGoldMileage(ToSetPoint);
	case EStoreMileageType::DarkCrystal:
		return SetDarkCrystalMileage(ToSetPoint);
	case EStoreMileageType::Package:
		return SetPackageMileage(ToSetPoint);
	}
}

int32 UB2UIDocStore::GetMileageReceivedIndex(int32 MileageType)
{
	switch (MileageType)
	{
	case EStoreMileageType::SummonItem:
		return SummonItemMileageReceivedIndex;
	case EStoreMileageType::BladePoint:
		return BladePointMileageReceivedIndex;
	case EStoreMileageType::Gem:
		return GemMileageReceivedIndex;
	case EStoreMileageType::Gold:
		return GoldMileageReceivedIndex;
	case EStoreMileageType::DarkCrystal:
		return DarkCrystalMileageReceivedIndex;
	case EStoreMileageType::Package:
		return PackageMileageReceivedIndex;
	}

	return -1;
}

void UB2UIDocStore::SetMileageReceivedIndex(int32 MileageType, int32 ToSetIndex)
{
	switch (MileageType)
	{
	case EStoreMileageType::SummonItem:
		return SetSummonItemMileageReceivedIndex(ToSetIndex);
	case EStoreMileageType::BladePoint:
		return SetBladePointMileageReceivedIndex(ToSetIndex);
	case EStoreMileageType::Gem:
		return SetGemMileageReceivedIndex(ToSetIndex);
	case EStoreMileageType::Gold:
		return SetGoldMileageReceivedIndex(ToSetIndex);
	case EStoreMileageType::DarkCrystal:
		return SetDarkCrystalMileageReceivedIndex(ToSetIndex);
	case EStoreMileageType::Package:
		return SetPackageMileageReceivedIndex(ToSetIndex);
	}
}

TArray<EUIStoreMileageRewardState> UB2UIDocStore::GetMileageRewardState()
{
	return MileageRewardState;
}

bool UB2UIDocStore::GetEnableMileageRewardState(int32 BoxGrade)
{
	//switch (BoxGrade)
	//{
	//case EStoreMileageBoxType::MileageBoxB:
	//	return (MileageRewardState[BoxGrade] == EUIStoreMileageRewardState::Reached ? true : false);
	//	
	//case EStoreMileageBoxType::MileageBoxA:
	//	return (MileageRewardState[BoxGrade] == EUIStoreMileageRewardState::Reached && 
	//		MileageRewardState[BoxGrade -1] == EUIStoreMileageRewardState::Rewarded ? true : false);

	//case EStoreMileageBoxType::MileageBoxS:
	//	return (MileageRewardState[BoxGrade] == EUIStoreMileageRewardState::Reached &&
	//		MileageRewardState[BoxGrade - 1] == EUIStoreMileageRewardState::Rewarded && 
	//		MileageRewardState[BoxGrade - 2] == EUIStoreMileageRewardState::Rewarded ? true : false);
	//}

	return false;
}

void UB2UIDocStore::SetMileageRewardState(TArray<EUIStoreMileageRewardState> ToSetState)
{
	MileageRewardState.Empty();

	for (auto& Elem : ToSetState)
	{
		MileageRewardState.Add(Elem);
	}
}

bool UB2UIDocStore::HasUnRewardeMileage(int32 MileageType)
{
	/*auto* SClassData = BladeIIGameImpl::GetClientDataStore().GetStoreMileageData().Find(FStoreMileageData::GetHashkey(MileageType, static_cast<uint32>(EUIStoreMileageGrade::SClass)));
	auto* AClassData = BladeIIGameImpl::GetClientDataStore().GetStoreMileageData().Find(FStoreMileageData::GetHashkey(MileageType, static_cast<uint32>(EUIStoreMileageGrade::AClass)));
	auto* BClassData = BladeIIGameImpl::GetClientDataStore().GetStoreMileageData().Find(FStoreMileageData::GetHashkey(MileageType, static_cast<uint32>(EUIStoreMileageGrade::BClass)));

	int32 MaxMileagePoint = SClassData ? SClassData->MaxMileage : 0;

	int32 CurrentMileagePoint = GetMileagePoint(MileageType);
	int32 LastReceivedRewardClassIndex = GetMileageReceivedIndex(MileageType);

	TArray<EUIStoreMileageRewardState> RewardStates;
	RewardStates.Empty(3);

	RewardStates.Add((BClassData->MaxMileage <= CurrentMileagePoint)
		? ((static_cast<int32>(EUIStoreMileageGrade::BClass) <= LastReceivedRewardClassIndex) ? EUIStoreMileageRewardState::Rewarded : EUIStoreMileageRewardState::Reached)
		: EUIStoreMileageRewardState::NotReached);
	RewardStates.Add((AClassData->MaxMileage <= CurrentMileagePoint)
		? ((static_cast<int32>(EUIStoreMileageGrade::AClass) <= LastReceivedRewardClassIndex) ? EUIStoreMileageRewardState::Rewarded : EUIStoreMileageRewardState::Reached)
		: EUIStoreMileageRewardState::NotReached);
	RewardStates.Add((SClassData->MaxMileage <= CurrentMileagePoint)
		? ((static_cast<int32>(EUIStoreMileageGrade::SClass) <= LastReceivedRewardClassIndex) ? EUIStoreMileageRewardState::Rewarded : EUIStoreMileageRewardState::Reached)
		: EUIStoreMileageRewardState::NotReached);

	for (auto RewardState : RewardStates)
	{
		if (RewardState == EUIStoreMileageRewardState::Reached)
			return true;
	}*/

	return false;
}

int32 UB2UIDocStore::GetReceivedRestrictCount(int32 ProductId)
{
	for (auto& Elem : RestrictCountMap)
	{
		if (Elem.Key == ProductId)
			return Elem.Value;
	}
	
	return 0;
}

void UB2UIDocStore::SetReceivedRestrictCount(int32 ProductId, int32 ToSetCount)
{
	RestrictCountMap.Add(ProductId, ToSetCount);
}

int32 UB2UIDocStore::GetProductMark(int32 ProductId)
{
	for (auto& Elem : ProductMarkMap)
	{
		if (Elem.Key == ProductId)
			return Elem.Value;
	}

	return 0;
}

void UB2UIDocStore::SetProductMark(int32 ProductId, int32 ToSetMark)
{
	ProductMarkMap.Add(ProductId, ToSetMark);
}

int32 UB2UIDocStore::GetPackageProductMark(int32 ProductId)
{
	for (auto& Elem : PackageProductMarkMap)
	{
		if (Elem.Key == ProductId)
			return Elem.Value;
	}

	return 0;
}

void UB2UIDocStore::SetProductSalePeriod(int32 ProductId, int64 StartTime, int64 EndTime)
{
	if(StartTime > 0 && EndTime > 0)
		ProductSalePeriodMap.Add(ProductId, FShopProductSaleTime(StartTime, EndTime));
}

bool UB2UIDocStore::GetProductSalePeriod(int32 ProductId, int64& StartTime, int64& EndTime)
{
	for (auto& Elem : ProductSalePeriodMap)
	{
		if (Elem.Key == ProductId)
		{
			StartTime = Elem.Value.StartTime;
			EndTime = Elem.Value.EndTime;
			return true;
		}
	}

	return false;
}

void UB2UIDocStore::SetPackageProductMark(int32 ProductId, int32 ToSetMark)
{
	PackageProductMarkMap.Add(ProductId, ToSetMark);
}

TArray<int32> UB2UIDocStore::GetReceivedPeriodicPackageProductIDs()
{
	TArray<int32> Products;

	for (auto& Elem : PeriodicPackageMap)
	{
		Products.Add(Elem.Key);
	}

	return Products;
}

int64 UB2UIDocStore::GetReceivedPeriodicPackageLeftTime(int32 ProductId)
{
	for (auto& Elem : PeriodicPackageMap)
	{
		if (Elem.Key == ProductId)
			return Elem.Value;
	}

	return 0;
}

bool UB2UIDocStore::HasReceivedPeriodicPackage()
{
	return PeriodicPackageMap.Num() > 0;
}

bool UB2UIDocStore::IsPeriodicPackage(int32 ProductId)
{
	return PeriodicPackageMap.Contains(ProductId);
}

bool UB2UIDocStore::IsPeriodicSale(int32 ProductId)
{
	return ProductSalePeriodMap.Contains(ProductId);
}

void UB2UIDocStore::SetReceivedPeriodicPackageLeftTime(int32 ProductId, int64 ToSetPeriod)
{
	PeriodicPackageMap.Add(ProductId, ToSetPeriod);
}

FPackageLevelupMission* UB2UIDocStore::GetReceivedLevelupPacakgeMission(int32 ProductId, int32 Level)
{
	int32 mykey = (ProductId * 1000) + Level;

	return LevelupPackageMissionMap.Find(mykey);
}

bool UB2UIDocStore::GetReddotPackageLevelMissionByPCClass(EPCClass PCClass)
{
	for (auto Each : LevelupPackageMissionMap)
	{
		if (Each.Value.Package_Class == PCClass && Each.Value.Package_State == static_cast<int32>(b2network::B2MissionStateType::COMPLETED))
		{
			return true;
		}
	}

	for (auto Each : ReachLevelMissionMap)
	{
		if (Each.Value.PCClass == PCClass && Each.Value.State == static_cast<int32>(b2network::B2MissionStateType::COMPLETED))
		{
			return true;
		}
	}

	return false;
}

bool UB2UIDocStore::GetReddotPackageLevelUpMissionByPackageID(int32 PacakgeID)
{
	for (auto Each : LevelupPackageMissionMap)
	{
		if (Each.Value.Package_Id == PacakgeID && Each.Value.Package_State == static_cast<int32>(b2network::B2MissionStateType::COMPLETED))
		{
			return true;
		}
	}

	return false;
}

void UB2UIDocStore::SetReceivedLevelupPacakgeMissions(int32 ProductId, FB2LevelUpPackageMissionPtr missionPtr)
{
	if (missionPtr->package_id == 0) return;
	
	FPackageLevelupMission newMission;
	newMission.Package_Id = missionPtr->package_id;
	newMission.Package_Level = missionPtr->level;
	newMission.Package_Class = SvrToCliPCClassType(missionPtr->character_type);
	newMission.Package_State = missionPtr->state;

	LevelupPackageMissionMap.Add(newMission.GetMyKey(), newMission);
}

FPackageReachLevelMission* UB2UIDocStore::GetReceivedReachLevelPacakgeMissions(int32 ProductId)
{
	return ReachLevelMissionMap.Find(ProductId);
}

void UB2UIDocStore::SetReceivedReachLevelPacakgeMissions(int32 ProductId, FB2ReachLevelPackageMissionPtr MissionPtr)
{
	if (MissionPtr->package_id == 0) return;

	FPackageReachLevelMission ReachLevelMission;
	ReachLevelMission.PackageID = MissionPtr->package_id;
	ReachLevelMission.PCClass = SvrToCliPCClassType(MissionPtr->character_type); 
	ReachLevelMission.Level = MissionPtr->level;
	ReachLevelMission.State = MissionPtr->state;
	ReachLevelMission.ExpireTime = MissionPtr->expire_time;

	ReachLevelMissionMap.Add(ProductId, ReachLevelMission);
}

FPackageFlatrateBonusState UB2UIDocStore::GetReceivedFlatratePackageBonusState()
{
	return PackageFlatrateBonusState;
}

void UB2UIDocStore::SetReceivedFlatratePackageBonusState(FB2FlatrateBonusStatePtr FlatrateBonusStatePtr)
{
	PackageFlatrateBonusState.CurrentIndex = FlatrateBonusStatePtr->current_index;
	PackageFlatrateBonusState.WeeklyState = FlatrateBonusStatePtr->weekly_state;
	PackageFlatrateBonusState.MonthlyState = FlatrateBonusStatePtr->monthly_state;
	PackageFlatrateBonusState.RewardState = FlatrateBonusStatePtr->reward_state;
}

void UB2UIDocStore::ResetMagicShopList()
{
	MagicShopItemList.Empty();
}

TArray<FMagicShopItem> UB2UIDocStore::GetMagicShopItemList()
{
	return MagicShopItemList;
}

void UB2UIDocStore::SetMagicShopList(FB2ResponseMagicShopPtr MagicShopPtr)
{
	if (MagicShopPtr->id == 0)
		return;

	FMagicShopItem MagicShopItem;
	MagicShopItem.ProductID = MagicShopPtr->id;
	MagicShopItem.IsPurchased = MagicShopPtr->is_purchased;

	MagicShopItemList.Add(MagicShopItem);
}
