// Fill out your copyright notice in the Description page of Project Settings.
#include "B2CostumeDataStore.h"
//#include "BladeII.h"

#include "BladeIIGameImpl.h"

#include "Event.h"


StaticCostumeEquipContainer::StaticCostumeEquipContainer()
{
	CostumeEquipArray.Add(ECostumeEquipPlace::ECEP_Weapon);
	CostumeEquipArray.Add(ECostumeEquipPlace::ECEP_Helmet);
	CostumeEquipArray.Add(ECostumeEquipPlace::ECEP_Body);

	PCClassArray.Add(EPCClass::EPC_Assassin);
	PCClassArray.Add(EPCClass::EPC_Fighter);
	PCClassArray.Add(EPCClass::EPC_Gladiator);
	PCClassArray.Add(EPCClass::EPC_Wizard);
}

FCostumeVisibleCache::FCostumeVisibleCache()
{
	const TArray<EPCClass>& PCArray = FCostumeDataStore::GetCostumeEquipArray().PCClassArray;
	const TArray<ECostumeEquipPlace>& CostumeArray = FCostumeDataStore::GetCostumeEquipArray().CostumeEquipArray;

	for (int i = 0; i < PCArray.Num(); ++i)
	{
		CostumeVisibleMap.Add(PCArray[i]);
		for (int32 j = 0; j < CostumeArray.Num(); ++j)
		{
			CostumeVisibleMap[PCArray[i]].Add(CostumeArray[j], true);
		}
	}
}

void FCostumeVisibleCache::SetCostumeVisible(EPCClass InPCClass, ECostumeEquipPlace InCostumeEquip, bool bVisible)
{
	if (!CostumeVisibleMap.Contains(InPCClass) && CostumeVisibleMap[InPCClass].Contains(InCostumeEquip))
		return;

	bool bChanged = CostumeVisibleMap[InPCClass][InCostumeEquip] != bVisible;

	CostumeVisibleMap[InPCClass][InCostumeEquip] = bVisible;

	if (bChanged)
		OnChagneCostumeVisible.Broadcast(InPCClass, InCostumeEquip, bVisible);
}

bool FCostumeVisibleCache::GetCostumeVisible(EPCClass InPCClass, ECostumeEquipPlace InCostumeEquip)
{
	if (!CostumeVisibleMap.Contains(InPCClass) && CostumeVisibleMap[InPCClass].Contains(InCostumeEquip))
		return true;

	return CostumeVisibleMap[InPCClass][InCostumeEquip];
}

void FCostumeVisibleCache::AllVisibleChange(EPCClass InPCClass, bool bVisible)
{
	for (int32 i = 0; i < FCostumeDataStore::GetCostumeEquipArray().CostumeEquipArray.Num(); ++i)
		SetCostumeVisible(InPCClass, FCostumeDataStore::GetCostumeEquipArray().CostumeEquipArray[i], bVisible);
}

FCostumeDataStore::FCostumeDataStore()
{
	//Initial Value is changed to true at UPT7
	IsOpenCostume = true;

	LocalVisibleCache = std::make_shared<FCostumeVisibleCache>();
	PreviewVisibleCache = std::make_shared<FCostumeVisibleCache>();

	//기본 로컬
	CurrentVisibleCache = LocalVisibleCache;
	bPreview = false;

	//같은 LifeCycle
	LocalVisibleCache->OnChagneCostumeVisible.AddLambda([this](EPCClass InClass, ECostumeEquipPlace EquipPlcae, bool bVisible)
	{
		this->OnChangeCostumeVisible.Broadcast(InClass, EquipPlcae, bVisible);
	});
	PreviewVisibleCache->OnChagneCostumeVisible.AddLambda([this](EPCClass InClass, ECostumeEquipPlace EquipPlcae, bool bVisible)
	{
		this->OnChangeCostumeVisible.Broadcast(InClass, EquipPlcae, bVisible);
	});
}

FCostumeDataStore::~FCostumeDataStore()
{
}

const StaticCostumeEquipContainer & FCostumeDataStore::GetCostumeEquipArray()
{
	static StaticCostumeEquipContainer Container;
	return Container;
}

void FCostumeDataStore::OnStartUpModule()
{
	SubscribeEvents();
}

void FCostumeDataStore::OnShutDownModule()
{
	UnSubscribeEvents();
}

void FCostumeDataStore::SetMasterData(const FB2MasterDatas& InMasterDatas)
{

}

void FCostumeDataStore::SetIsOpenCostume(const bool& IsOpen)
{ 
	IsOpenCostume = IsOpen;
}

void FCostumeDataStore::SetCachedCostumePageType(const ECostumePage& eCostumePageType)
{
	CachedCostumePageType = eCostumePageType;
}

void FCostumeDataStore::ChangeVisibleMode(bool InPreview, EPCClass InPCClass)
{
	if (bPreview != InPreview)
	{
		bPreview = InPreview;
		CurrentVisibleCache = bPreview ? PreviewVisibleCache : LocalVisibleCache;
		if(InPreview)
			AllVisibleChange(InPCClass, true);
		OnChangedVisibleMode.Broadcast(bPreview);
	}
}

void FCostumeDataStore::ClearPreview()
{
	PreviewCostumeRefIDList.Empty();
}

void FCostumeDataStore::SetLocalVisibleMode(EPCClass ChangeClass)
{
	ClearPreview();
	ChangeVisibleMode(false, ChangeClass);

	//LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(ChangeClass);
}

void FCostumeDataStore::SetCostumeVisible(EPCClass InPCClass, ECostumeEquipPlace InCostumeEquip, bool bVisible)
{
	CurrentVisibleCache->SetCostumeVisible(InPCClass, InCostumeEquip, bVisible);
}

bool FCostumeDataStore::GetCostumeVisible(EPCClass InPCClass, ECostumeEquipPlace InCostumeEquip)
{
	return CurrentVisibleCache->GetCostumeVisible(InPCClass, InCostumeEquip);
}

void FCostumeDataStore::AllVisibleChange(EPCClass InPCClass, bool bVisible)
{
	CurrentVisibleCache->AllVisibleChange(InPCClass, bVisible);
}

void FCostumeDataStore::SubscribeEvents()
{
	UnSubscribeEvents();

	//Issues.Add(OpenCostumeClass<bool>::GetInstance().Subscribe2(
	//[this](bool IsOpen)
	//{
	//	this->SetIsOpenCostume(IsOpen);
	//}
	//));

	//Issues.Add(SelectCostumePackageClass<int32>::GetInstance().Subscribe2(
	//	[this](int32 ProductID)
	//{
	//	//PreviewMode
	//	this->SetPreviewMode(ProductID);
	//}
	//));

	//Issues.Add(CostumeEquipSlotVisibleClass<bool>::GetInstance().Subscribe2(
	//	[this](bool bInven)
	//{
	//	if (bInven)
	//		this->SetLocalVisibleMode();
	//}
	//));
	//
	//Issues.Add(LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Subscribe2(
	//	[this](EPCClass SelectedPCClass)
	//{
	//	this->SetLocalVisibleMode(SelectedPCClass);
	//}
	//));
}

void FCostumeDataStore::UnSubscribeEvents()
{
	Issues.Empty();
}

void FCostumeDataStore::SetPreviewMode(int32 ProductID)
{
	const FStoreProductData* PackageProductData = BladeIIGameImpl::GetClientDataStore().GetStoreProductData(ProductID);

	if (PackageProductData != nullptr)
	{
		TArray<FPackageProductData> OutList;
		BladeIIGameImpl::GetClientDataStore().GetPackageProductList(PackageProductData->Product.ProductId, OutList);

		ClearPreview();

		for (const FPackageProductData& Data : OutList)
			PreviewCostumeRefIDList.Add(Data.PackageProductId);

		if (PreviewCostumeRefIDList.Num() == 0)
			return;

		EPCClass ItemPCClass = FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(PreviewCostumeRefIDList[0]);
		
		ChangeVisibleMode(true, ItemPCClass);

		LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(ItemPCClass);
		LobbyCharEquippedItemClass<EPCClass, EItemEquipPlace>::GetInstance().Signal(ItemPCClass, GetRandomCostumeAnim());
	}
}

EItemEquipPlace FCostumeDataStore::GetRandomCostumeAnim()
{
	//하나는 신발이라 걍 M
	static TArray<EItemEquipPlace> RandomAnim = { EItemEquipPlace::EIEP_Weapon , EItemEquipPlace::EIEP_BodyArmor};

	return RandomAnim[FMath::RandRange(0, RandomAnim.Num() - 1)];
}
