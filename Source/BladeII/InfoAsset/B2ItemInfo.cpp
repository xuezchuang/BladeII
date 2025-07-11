// Fill out your copyright notice in the Description page of Project Settings.

#include "B2ItemInfo.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Materials/Material.h"

#include "B2DropItem.h"
#include "BladeIIGameMode.h"
//#include "B2Inventory.h"
//#include "B2LobbyInventory.h"
//#include "B2SomeInfo.h"
#include "BladeIIUtil.h"
#include "B2GameInstance.h"

#include "FB2ErrorMessage.h"
#include "../BladeII/BladeIILocText.h"
#include "Materials/MaterialInterface.h"

TSubclassOf<class AB2DropItem> FSingleItemInfoData::GetBaseBPClass(UObject* WorldContextObject)
{
	//if (BaseBPClass)
	//{
	//	return BaseBPClass;
	//}

	//UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	//UB2SomeInfo* SomeInfo = B2GI ? B2GI->GetSomeInfo() : StaticFindSomeInfo();
	return /*SomeInfo ? SomeInfo->GetDefaultDropItemBaseBPClass() :*/ NULL;
}

// �ܼ��ݺ� �ڵ� ���. ��� �̸��� �־��־�� �ؼ� ���ø��� �ȵǰ�..
#define SINGLE_ITEM_INFO_DATA_MEMBER_LOADING_BLOCK(ItemInfoObj, MemberVarName, ItemRefID) \
	if (ItemInfoObj && !MemberVarName.IsValid() && MemberVarName.ToString().Len() > 0) \
	{ \
		ItemInfoObj->GetInfoLoadManager().LoadSynchronous(MemberVarName); \
	} \
	FSingleItemInfoLoadedPtrData* MyCachedRefContainer = ItemInfoObj ? ItemInfoObj->GetLoadedPtrForID(ItemRefID) : NULL; \
	checkSlow(MyCachedRefContainer); \
	if (MyCachedRefContainer) \
	{ /* Loaded** members must keep the naming. */ \
		MyCachedRefContainer->Loaded##MemberVarName = MemberVarName.Get(); \
	} \
	return MemberVarName.Get();

UMaterialInterface* FSingleItemInfoData::GetIconMaterial(UB2ItemInfo* InInfoObj)
{
	SINGLE_ITEM_INFO_DATA_MEMBER_LOADING_BLOCK(InInfoObj, IconMaterial, CachedMyRefID)
}

USkeletalMesh* FSingleItemInfoData::GetPCPartMesh(UB2ItemInfo* InInfoObj)
{
	SINGLE_ITEM_INFO_DATA_MEMBER_LOADING_BLOCK(InInfoObj, PCPartMesh, CachedMyRefID)
}

UMaterialInterface* FSingleItemInfoData::GetPCPartMaterial(UB2ItemInfo* InInfoObj)
{
	SINGLE_ITEM_INFO_DATA_MEMBER_LOADING_BLOCK(InInfoObj, PCPartMaterial, CachedMyRefID)
}

USkeletalMesh* FSingleItemInfoData::GetPCPartExtraMesh(UB2ItemInfo* InInfoObj)
{
	SINGLE_ITEM_INFO_DATA_MEMBER_LOADING_BLOCK(InInfoObj, PCPartExtraMesh, CachedMyRefID)
}

UMaterialInterface* FSingleItemInfoData::GetPCPartExtraMaterial(UB2ItemInfo* InInfoObj)
{
	SINGLE_ITEM_INFO_DATA_MEMBER_LOADING_BLOCK(InInfoObj, PCPartExtraMaterial, CachedMyRefID)
}

UParticleSystem* FSingleItemInfoData::GetSixStarWeaponEffect(UB2ItemInfo* InInfoObj)
{
	SINGLE_ITEM_INFO_DATA_MEMBER_LOADING_BLOCK(InInfoObj, SixStarWeaponEffect, CachedMyRefID)
}

UParticleSystem* FSingleItemInfoData::GetSixStarWeaponEffect_R(UB2ItemInfo* InInfoObj)
{
	SINGLE_ITEM_INFO_DATA_MEMBER_LOADING_BLOCK(InInfoObj, SixStarWeaponEffect_R, CachedMyRefID)
}

UParticleSystem* FSingleItemInfoData::GetSixStarWeaponEffect2(UB2ItemInfo* InInfoObj)
{
	SINGLE_ITEM_INFO_DATA_MEMBER_LOADING_BLOCK(InInfoObj, SixStarWeaponEffect2, CachedMyRefID)
}

UParticleSystem* FSingleItemInfoData::GetSixStarWeaponEffect2_R(UB2ItemInfo* InInfoObj)
{
	SINGLE_ITEM_INFO_DATA_MEMBER_LOADING_BLOCK(InInfoObj, SixStarWeaponEffect2_R, CachedMyRefID)
}

UParticleSystem* FSingleItemInfoData::GetDropMainBodyPST(UB2ItemInfo* InInfoObj)
{
	SINGLE_ITEM_INFO_DATA_MEMBER_LOADING_BLOCK(InInfoObj, DropMainBodyPST, CachedMyRefID)
}

UParticleSystem* FSingleItemInfoData::GetDropSuckingPS(UB2ItemInfo* InInfoObj)
{
	SINGLE_ITEM_INFO_DATA_MEMBER_LOADING_BLOCK(InInfoObj, DropSuckingPS, CachedMyRefID)
}

USoundCue* FSingleItemInfoData::GetDropPlaySoundCue(UB2ItemInfo* InInfoObj)
{
	SINGLE_ITEM_INFO_DATA_MEMBER_LOADING_BLOCK(InInfoObj, DropPlaySoundCue, CachedMyRefID)
}

FText FSingleItemInfoData::GetLocalizedName(bool bAttachQualityPrefix, int32 ItemQuality)
{
	static FText ItemNameFormatWithPrefix = FText::FromString(TEXT("{0} {1}"));

	// ó���� ������ �̸��� ǰ�� ���λ縦 ���̴� ���̾��µ� ǰ�� ���λ�� ���� ǥ���ϰ� ��. Ȥ�� � ������ ��Ȱ���� �𸣴� �Ű������� �ڵ�� ���ܵ�.
	FText NameWithoutQuality = BladeIIGetLOCText(B2LOC_CAT_ITEMNAME, LocalizedNameKey.ToString());

	//if (bAttachQualityPrefix && ItemQuality > 0 && ItemQuality <= MAX_ITEM_QUALITY)
	//{
	//	return FText::Format(ItemNameFormatWithPrefix, GetLOCTextOfItemQuality(ItemQuality), NameWithoutQuality);
	//}
	//else
	{
		return NameWithoutQuality;
	}
}

void FSingleItemInfoData::LoadAllTAssets(FStreamableManager& InLoadManager, int32 MyRefID, TMap<int32, FSingleItemInfoLoadedPtrData>& LoadedPtrMap)
{
	// �� FSingleItemInfoData �� ���� ������ ����ϴ� �� �ƴϰ� �⺻�����δ� ���� �� ������� �ʿ��� �� �׶��׶� �ε��Ѵ�.
	// ���� GetSomeLazyLoadedAssetCommon �� ���� �׳� ���� �ɷ�.

	int32 CheckTAssets = 0; // TAsset ��� �߰��� FSingleItemInfoLoadedPtrData ���� �߰� ����� �ʰ�.
	if (IconMaterial.IsValid() == false && IconMaterial.ToString().Len() > 0)
	{
		InLoadManager.LoadSynchronous(IconMaterial);
	}
	++CheckTAssets; // ������ �� �̰͵� ���� ����
	if (PCPartMesh.IsValid() == false && PCPartMesh.ToString().Len() > 0)
	{
		InLoadManager.LoadSynchronous(PCPartMesh);
	}
	++CheckTAssets;
	if (PCPartMaterial.IsValid() == false && PCPartMaterial.ToString().Len() > 0)
	{
		InLoadManager.LoadSynchronous(PCPartMaterial);
	}
	++CheckTAssets;
	if (PCPartExtraMesh.IsValid() == false && PCPartExtraMesh.ToString().Len() > 0)
	{
		InLoadManager.LoadSynchronous(PCPartExtraMesh);
	}
	++CheckTAssets;
	if (PCPartExtraMaterial.IsValid() == false && PCPartExtraMaterial.ToString().Len() > 0)
	{
		InLoadManager.LoadSynchronous(PCPartExtraMaterial);
	}
	++CheckTAssets;
	if (DropMainBodyPST.IsValid() == false && DropMainBodyPST.ToString().Len() > 0)
	{
		InLoadManager.LoadSynchronous(DropMainBodyPST);
	}
	++CheckTAssets;
	if (DropSuckingPS.IsValid() == false && DropSuckingPS.ToString().Len() > 0)
	{
		InLoadManager.LoadSynchronous(DropSuckingPS);
	}
	++CheckTAssets;
	if (DropPlaySoundCue.IsValid() == false && DropPlaySoundCue.ToString().Len() > 0)
	{
		InLoadManager.LoadSynchronous(DropPlaySoundCue);
	}
	++CheckTAssets;
	if (SixStarWeaponEffect.IsValid() == false && SixStarWeaponEffect.ToString().Len() > 0)
	{
		InLoadManager.LoadSynchronous(SixStarWeaponEffect);
	}
	++CheckTAssets;
	if (SixStarWeaponEffect_R.IsValid() == false && SixStarWeaponEffect_R.ToString().Len() > 0)
	{
		InLoadManager.LoadSynchronous(SixStarWeaponEffect_R);
	}
	++CheckTAssets;
	if (SixStarWeaponEffect2.IsValid() == false && SixStarWeaponEffect2.ToString().Len() > 0)
	{
		InLoadManager.LoadSynchronous(SixStarWeaponEffect2);
	}
	++CheckTAssets;
	if (SixStarWeaponEffect2_R.IsValid() == false && SixStarWeaponEffect2_R.ToString().Len() > 0)
	{
		InLoadManager.LoadSynchronous(SixStarWeaponEffect2_R);
	}
	++CheckTAssets;

	FSingleItemInfoLoadedPtrData* FoundLoadedPtrEntry = LoadedPtrMap.Find(MyRefID);
	if (!FoundLoadedPtrEntry)
	{
		FSingleItemInfoLoadedPtrData NewLoadedEntry;
		LoadedPtrMap.Add(MyRefID, NewLoadedEntry);
		FoundLoadedPtrEntry = LoadedPtrMap.Find(MyRefID);
	}
	checkSlow(FoundLoadedPtrEntry); // ������ �ٺ�
	if (FoundLoadedPtrEntry)
	{
		int32 CheckLoadedPtr = 0;
		
		// GC ������ �ε��� ���۷��� ĳ��. ���ʿ� ����� TSoftObjectPtr ����鿡 ���� FSingleItemInfoLoadedPtrData �� �׸��� �־��־�� ��.
		FoundLoadedPtrEntry->LoadedIconMaterial = IconMaterial.Get();
		++CheckLoadedPtr;
		FoundLoadedPtrEntry->LoadedPCPartMesh = PCPartMesh.Get();
		++CheckLoadedPtr;
		FoundLoadedPtrEntry->LoadedPCPartMaterial = PCPartMaterial.Get();
		++CheckLoadedPtr;
		FoundLoadedPtrEntry->LoadedPCPartExtraMesh = PCPartExtraMesh.Get();
		++CheckLoadedPtr;
		FoundLoadedPtrEntry->LoadedPCPartExtraMaterial = PCPartExtraMaterial.Get();
		++CheckLoadedPtr;
		FoundLoadedPtrEntry->LoadedDropMainBodyPST = DropMainBodyPST.Get();
		++CheckLoadedPtr;
		FoundLoadedPtrEntry->LoadedDropSuckingPS = DropSuckingPS.Get();
		++CheckLoadedPtr;
		FoundLoadedPtrEntry->LoadedDropPlaySoundCue = DropPlaySoundCue.Get();
		++CheckLoadedPtr;
		FoundLoadedPtrEntry->LoadedSixStarWeaponEffect = SixStarWeaponEffect.Get();
		++CheckLoadedPtr;
		FoundLoadedPtrEntry->LoadedSixStarWeaponEffect_R = SixStarWeaponEffect_R.Get();
		++CheckLoadedPtr;
		FoundLoadedPtrEntry->LoadedSixStarWeaponEffect2 = SixStarWeaponEffect2.Get();
		++CheckLoadedPtr;
		FoundLoadedPtrEntry->LoadedSixStarWeaponEffect2_R = SixStarWeaponEffect2_R.Get();
		++CheckLoadedPtr;

#if WITH_EDITOR && !PLATFORM_MAC
		if (CheckTAssets != CheckLoadedPtr)
		{
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("[Warning] FSingleItemInfoData �� TAsset ����� FSingleItemInfoLoadedPtrData �� �ε��� ���� ���۷��� �׸� ���� �ٸ���. ��ǻ�Ͱ� �� �����Ѵ�."))
				));
		}
#endif
	}
}

void FSingleItemInfoData::UnloadAllTAssets(FStreamableManager& InLoadManager, int32 MyRefID, TMap<int32, FSingleItemInfoLoadedPtrData>& LoadedPtrMap, bool bUnloadEvenRootSet)
{
	if (bUnloadEvenRootSet)
	{
		MarkLoadedEquipPartAtRooSet(false); // Ȥ���� RootSet �̶�� �װ� ���ź��� �ؾ�.
	}

	// bUnloadEvenRootSet �� �ƴ϶�.. ������ ������ ���� unloading �� �� ����. ��Ʈ���̴�.
	LoadedPtrMap.Remove(MyRefID);

	if (IconMaterial.IsValid() && (!IconMaterial->IsRooted() || bUnloadEvenRootSet))
	{
		IconMaterial->RemoveFromRoot();
		InLoadManager.Unload(IconMaterial.ToSoftObjectPath());
	}
	if (PCPartMesh.IsValid() && (!PCPartMesh->IsRooted() || bUnloadEvenRootSet))
	{
		PCPartMesh->RemoveFromRoot();
		InLoadManager.Unload(PCPartMesh.ToSoftObjectPath());
	}
	if (PCPartMaterial.IsValid() && (!PCPartMaterial->IsRooted() || bUnloadEvenRootSet))
	{
		PCPartMaterial->RemoveFromRoot();
		InLoadManager.Unload(PCPartMaterial.ToSoftObjectPath());
	}
	if (PCPartExtraMesh.IsValid() && (!PCPartExtraMesh->IsRooted() || bUnloadEvenRootSet))
	{
		PCPartExtraMesh->RemoveFromRoot();
		InLoadManager.Unload(PCPartExtraMesh.ToSoftObjectPath());
	}
	if (PCPartExtraMaterial.IsValid() && (!PCPartExtraMaterial->IsRooted() || bUnloadEvenRootSet))
	{
		PCPartExtraMaterial->RemoveFromRoot();
		InLoadManager.Unload(PCPartExtraMaterial.ToSoftObjectPath());
	}
	if (DropMainBodyPST.IsValid() && (!DropMainBodyPST->IsRooted() || bUnloadEvenRootSet))
	{
		DropMainBodyPST->RemoveFromRoot();
		InLoadManager.Unload(DropMainBodyPST.ToSoftObjectPath());
	}
	if (DropSuckingPS.IsValid() && (!DropSuckingPS->IsRooted() || bUnloadEvenRootSet))
	{
		DropSuckingPS->RemoveFromRoot();
		InLoadManager.Unload(DropSuckingPS.ToSoftObjectPath());
	}
	if (DropPlaySoundCue.IsValid() && (!DropPlaySoundCue->IsRooted() || bUnloadEvenRootSet))
	{
		DropPlaySoundCue->RemoveFromRoot();
		InLoadManager.Unload(DropPlaySoundCue.ToSoftObjectPath());
	}
	if (SixStarWeaponEffect.IsValid() && (!SixStarWeaponEffect->IsRooted() || bUnloadEvenRootSet))
	{
		SixStarWeaponEffect->RemoveFromRoot();
		InLoadManager.Unload(SixStarWeaponEffect.ToSoftObjectPath());
	}
	if (SixStarWeaponEffect_R.IsValid() && (!SixStarWeaponEffect_R->IsRooted() || bUnloadEvenRootSet))
	{
		SixStarWeaponEffect_R->RemoveFromRoot();
		InLoadManager.Unload(SixStarWeaponEffect_R.ToSoftObjectPath());
	}
	if (SixStarWeaponEffect2.IsValid() && (!SixStarWeaponEffect2->IsRooted() || bUnloadEvenRootSet))
	{
		SixStarWeaponEffect2->RemoveFromRoot();
		InLoadManager.Unload(SixStarWeaponEffect2.ToSoftObjectPath());
	}
	if (SixStarWeaponEffect2_R.IsValid() && (!SixStarWeaponEffect2_R->IsRooted() || bUnloadEvenRootSet))
	{
		SixStarWeaponEffect2_R->RemoveFromRoot();
		InLoadManager.Unload(SixStarWeaponEffect2_R.ToSoftObjectPath());
	}
}

void FSingleItemInfoData::UnloadAllExceptEquipPart(FStreamableManager& InLoadManager, int32 MyRefID, TMap<int32, FSingleItemInfoLoadedPtrData>& LoadedPtrMap)
{
	// PCPart** �� �̿� �����ؼ� �ʿ��� SixStar** ���� ���⼭ �����Ѵ�.
	// ���� �̷� ������ ��ε��� �ٸ� ������ �ʿ������� ���� TAsset �鿡 ���� ��ε��� �����ϴ� �� ���� ��.

	FSingleItemInfoLoadedPtrData* MyCachedRefContainer = LoadedPtrMap.Find(MyRefID);
	if (MyCachedRefContainer) // �� ã���� ���ۿ� �ε��� ���� ���� �ŵ���, ���� �߸��Ǿ�����.
	{
		MyCachedRefContainer->LoadedIconMaterial = NULL;
		MyCachedRefContainer->LoadedDropMainBodyPST = NULL;
		MyCachedRefContainer->LoadedDropSuckingPS = NULL;
		MyCachedRefContainer->LoadedDropPlaySoundCue = NULL;
	}

	if (IconMaterial.IsValid())
	{
		InLoadManager.Unload(IconMaterial.ToSoftObjectPath());
	}
	if (DropMainBodyPST.IsValid())
	{
		InLoadManager.Unload(DropMainBodyPST.ToSoftObjectPath());
	}
	if (DropSuckingPS.IsValid())
	{
		InLoadManager.Unload(DropSuckingPS.ToSoftObjectPath());
	}
	if (DropPlaySoundCue.IsValid())
	{
		InLoadManager.Unload(DropPlaySoundCue.ToSoftObjectPath());
	}
}

void FSingleItemInfoData::UnloadOnlyEquipPart(FStreamableManager& InLoadManager, int32 MyRefID, TMap<int32, FSingleItemInfoLoadedPtrData>& LoadedPtrMap, bool bUnloadEvenRootSet)
{
	if (bUnloadEvenRootSet)  // Ȥ���� RootSet �̶�� �װ� ���ź��� ������ �ʿ��� ��� RootSet ���� ������ �� ��ε����� ���ܽ�ų �� �ֵ���.
	{
		MarkLoadedEquipPartAtRooSet(false);
	}

	FSingleItemInfoLoadedPtrData* MyCachedRefContainer = LoadedPtrMap.Find(MyRefID);

	if (PCPartMesh.IsValid() && (!PCPartMesh->IsRooted() || bUnloadEvenRootSet))
	{
		PCPartMesh->RemoveFromRoot();
		if (MyCachedRefContainer) { // �� ã���� ���ۿ� �ε��� ���� ���� �ŵ���, ���� �߸��Ǿ�����.
			MyCachedRefContainer->LoadedPCPartMesh = NULL;
		}
		InLoadManager.Unload(PCPartMesh.ToSoftObjectPath());
	}
	if (PCPartMaterial.IsValid() && (!PCPartMaterial->IsRooted() || bUnloadEvenRootSet))
	{
		PCPartMaterial->RemoveFromRoot();
		if (MyCachedRefContainer) {
			MyCachedRefContainer->LoadedPCPartMaterial = NULL;
		}
		InLoadManager.Unload(PCPartMaterial.ToSoftObjectPath());
	}
	if (PCPartExtraMesh.IsValid() && (!PCPartExtraMesh->IsRooted() || bUnloadEvenRootSet))
	{
		PCPartExtraMesh->RemoveFromRoot();
		if (MyCachedRefContainer) {
			MyCachedRefContainer->LoadedPCPartExtraMesh = NULL;
		}
		InLoadManager.Unload(PCPartExtraMesh.ToSoftObjectPath());
	}
	if (PCPartExtraMaterial.IsValid() && (!PCPartExtraMaterial->IsRooted() || bUnloadEvenRootSet))
	{
		PCPartExtraMaterial->RemoveFromRoot();
		if (MyCachedRefContainer) {
			MyCachedRefContainer->LoadedPCPartExtraMaterial = NULL;
		}
		InLoadManager.Unload(PCPartExtraMaterial.ToSoftObjectPath());
	}
	if (SixStarWeaponEffect.IsValid() && (!SixStarWeaponEffect->IsRooted() || bUnloadEvenRootSet))
	{
		SixStarWeaponEffect->RemoveFromRoot();
		if (MyCachedRefContainer) {
			MyCachedRefContainer->LoadedSixStarWeaponEffect = NULL;
		}
		InLoadManager.Unload(SixStarWeaponEffect.ToSoftObjectPath());
	}
	if (SixStarWeaponEffect_R.IsValid() && (!SixStarWeaponEffect_R->IsRooted() || bUnloadEvenRootSet))
	{
		SixStarWeaponEffect_R->RemoveFromRoot();
		if (MyCachedRefContainer) {
			MyCachedRefContainer->LoadedSixStarWeaponEffect_R = NULL;
		}
		InLoadManager.Unload(SixStarWeaponEffect_R.ToSoftObjectPath());
	}
	if (SixStarWeaponEffect2.IsValid() && (!SixStarWeaponEffect2->IsRooted() || bUnloadEvenRootSet))
	{
		SixStarWeaponEffect2->RemoveFromRoot();
		if (MyCachedRefContainer) {
			MyCachedRefContainer->LoadedSixStarWeaponEffect2 = NULL;
		}
		InLoadManager.Unload(SixStarWeaponEffect2.ToSoftObjectPath());
	}
	if (SixStarWeaponEffect2_R.IsValid() && (!SixStarWeaponEffect2_R->IsRooted() || bUnloadEvenRootSet))
	{
		SixStarWeaponEffect2_R->RemoveFromRoot();
		if (MyCachedRefContainer) {
			MyCachedRefContainer->LoadedSixStarWeaponEffect2_R = NULL;
		}
		InLoadManager.Unload(SixStarWeaponEffect2_R.ToSoftObjectPath());
	}
}

void FSingleItemInfoData::UnloadOnlyEquipPart(UB2ItemInfo* OwnerItemInfoTable, bool bUnloadEvenRootSet)
{ // UnloadOnlyEquipPart �� �ۿ��� ���� �ҷ��� ���� �־..
	if (OwnerItemInfoTable)
	{
		UnloadOnlyEquipPart(OwnerItemInfoTable->InfoLoadManager, CachedMyRefID, OwnerItemInfoTable->LoadedPtrMap, bUnloadEvenRootSet);
	}
}

void FSingleItemInfoData::MarkLoadedEquipPartAtRooSet(bool bRootSet)
{
	// ���⼭ ���� �ε��� ������ �ʴ´�. �ε� �Ǿ� �ִ� �͸� ó��..
	// AddToRoot �� �ϸ� Unload �� ������ ���� ��.
	if (PCPartMesh.IsValid())
	{
		if (bRootSet) {
			PCPartMesh->AddToRoot();
		}
		else {
			PCPartMesh->RemoveFromRoot();
		}
	}
	if (PCPartMaterial.IsValid())
	{
		if (bRootSet) {
			PCPartMaterial->AddToRoot();
		}
		else {
			PCPartMaterial->RemoveFromRoot();
		}
	}
	if (PCPartExtraMesh.IsValid())
	{
		if (bRootSet) {
			PCPartExtraMesh->AddToRoot();
		}
		else {
			PCPartExtraMesh->RemoveFromRoot();
		}
	}
	if (PCPartExtraMaterial.IsValid())
	{
		if (bRootSet) {
			PCPartExtraMaterial->AddToRoot();
		}
		else {
			PCPartExtraMaterial->RemoveFromRoot();
		}
	}
	if (SixStarWeaponEffect.IsValid())
	{
		if (bRootSet) {
			SixStarWeaponEffect->AddToRoot();
		}
		else {
			SixStarWeaponEffect->RemoveFromRoot();
		}
	}
	if (SixStarWeaponEffect_R.IsValid())
	{
		if (bRootSet) {
			SixStarWeaponEffect_R->AddToRoot();
		}
		else {
			SixStarWeaponEffect_R->RemoveFromRoot();
		}
	}
	if (SixStarWeaponEffect2.IsValid())
	{
		if (bRootSet) {
			SixStarWeaponEffect2->AddToRoot();
		}
		else {
			SixStarWeaponEffect2->RemoveFromRoot();
		}
	}
	if (SixStarWeaponEffect2_R.IsValid())
	{
		if (bRootSet) {
			SixStarWeaponEffect2_R->AddToRoot();
		}
		else {
			SixStarWeaponEffect2_R->RemoveFromRoot();
		}
	}
}

void FSingleItemInfoData::GetEquipPartsAsyncLoadReqInfo(TArray<FB2AsyncRequestInfo>& OutRequestInfoList)
{
	// TryAsyncLoad �� �ʿ��� ������ �ܾ����. ������� �ϳ���.
	// ���� ��� TAsset ��θ� �ִ� �� �ƴϴ�. ĳ���� �޽� ���� �� �ΰ� ȿ���� �ʿ��� ���鸸.
	// ���� Async ��û�� ���� �� ���� SingleItemInfoData �� �ƴ϶� ���� RefID �� ���� �ܾ���� FB2AsyncRequestInfo ����Ʈ���� ���ļ� ������ �� ������ ����. ĳ���� �ϳ� �����ϴµ� ���� RefID �� ������ �����Ͱ� �ʿ��ϱ� �����̴�.

	//if (!PCPartMesh.IsNull()) {
	//	OutRequestInfoList.Add(FB2AsyncRequestInfo(GetAsyncReqKey_PCPartMesh(), PCPartMesh));
	//}
	//if (!PCPartMaterial.IsNull()) {
	//	OutRequestInfoList.Add(FB2AsyncRequestInfo(GetAsyncReqKey_PCPartMaterial(), PCPartMaterial));
	//}
	//if (!PCPartExtraMesh.IsNull()) {
	//	OutRequestInfoList.Add(FB2AsyncRequestInfo(GetAsyncReqKey_PCPartExtraMesh(), PCPartExtraMesh));
	//}
	//if (!PCPartExtraMaterial.IsNull()) {
	//	OutRequestInfoList.Add(FB2AsyncRequestInfo(GetAsyncReqKey_PCPartExtraMaterial(), PCPartExtraMaterial));
	//}
	//if (!SixStarWeaponEffect.IsNull()) {
	//	OutRequestInfoList.Add(FB2AsyncRequestInfo(GetAsyncReqKey_SixStarWeaponEffect(), SixStarWeaponEffect));
	//}
	//if (!SixStarWeaponEffect_R.IsNull()) {
	//	OutRequestInfoList.Add(FB2AsyncRequestInfo(GetAsyncReqKey_SixStarWeaponEffect_R(), SixStarWeaponEffect_R));
	//}
	//if (!SixStarWeaponEffect2.IsNull()) {
	//	OutRequestInfoList.Add(FB2AsyncRequestInfo(GetAsyncReqKey_SixStarWeaponEffect2(), SixStarWeaponEffect2));
	//}
	//if (!SixStarWeaponEffect2_R.IsNull()) {
	//	OutRequestInfoList.Add(FB2AsyncRequestInfo(GetAsyncReqKey_SixStarWeaponEffect2_R(), SixStarWeaponEffect2_R));
	//}
}
void FSingleItemInfoData::CacheOnEquipPartsAsyncLoadComplete(int32 MyRefID, TMap<int32, FSingleItemInfoLoadedPtrData>& LoadedPtrMap)
{
	// ���⼭ ĳ���ϰ��� �ϴ� ����� ������ �� �Ŷ�� �ε��� �� ���¿��� �Ѵ�. (�����δ� ��� ���õ� ���� �幰 �Ŷ� Get �� null �� �����ϴ� �� �̻��� �� �ƴϴ�.)
	// Async �ε��� �����ؼ� ���� ���θ�ŭ flush ��� ���� �ܼ� Get ��..

	FSingleItemInfoLoadedPtrData& FoundOrLoadedPtrEntry = LoadedPtrMap.FindOrAdd(MyRefID);

	FoundOrLoadedPtrEntry.LoadedPCPartMesh = PCPartMesh.Get();
	FoundOrLoadedPtrEntry.LoadedPCPartMaterial = PCPartMaterial.Get();
	FoundOrLoadedPtrEntry.LoadedPCPartExtraMesh = PCPartExtraMesh.Get();
	FoundOrLoadedPtrEntry.LoadedPCPartExtraMaterial = PCPartExtraMaterial.Get();
	FoundOrLoadedPtrEntry.LoadedSixStarWeaponEffect = SixStarWeaponEffect.Get();
	FoundOrLoadedPtrEntry.LoadedSixStarWeaponEffect_R = SixStarWeaponEffect_R.Get();
	FoundOrLoadedPtrEntry.LoadedSixStarWeaponEffect2 = SixStarWeaponEffect2.Get();
	FoundOrLoadedPtrEntry.LoadedSixStarWeaponEffect2_R = SixStarWeaponEffect2_R.Get();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void AB2DropItem::OverrideByItemInfo(UB2ItemInfo* WholeInfoTable, FSingleItemInfoData* InSingleInfoOfID)
{
	check(WholeInfoTable && InSingleInfoOfID);

	//if (WholeInfoTable && InSingleInfoOfID)
	//{
	//	// ���� ItemInfo ���� ���� BaseBP ����
	//	OverrideByBaseBP(InSingleInfoOfID->GetBaseBPClass(this));

	//	// ���⼭ ItemInfo �� �ִ� ��� ������ ��������� ���� ���̴�. �Ϻδ� UI �� ���δ�.

	//	this->SpawnLocationOffsetZ += InSingleInfoOfID->Offset_SpawnLocationZ;
	//	this->StayTime += InSingleInfoOfID->Offset_StayTime;
	//	this->SpawnMinRandomDistance += InSingleInfoOfID->Offset_SpawnMinDistance;
	//	this->SpawnMaxRandomDistance += InSingleInfoOfID->Offset_SpawnMaxDistance;
	//	this->SpawningTime += InSingleInfoOfID->Offset_SpawningTime;

	//	if (InSingleInfoOfID->PlaySoundDelayOverride > 0.0f)
	//	{
	//		this->PlaySoundDelay = InSingleInfoOfID->PlaySoundDelayOverride;
	//	}

	//	if (InSingleInfoOfID->PostSuckLifeSpanOverride > 0.0f)
	//	{
	//		this->PostSuckLifeSpan = InSingleInfoOfID->PostSuckLifeSpanOverride;
	//	}

	//	// ��° �� �߱������� �� ������ ���ϰ� �������� ����.
	//	// �̰��� �Ҹ� ���� AB2DropItem::BeginPlay �� �Ҹ��� �ʴ´ٸ� �Ϻ� ó���� �������� �ؾ� �� ��.

	//	UParticleSystem* DropMainBodyPST = InSingleInfoOfID->GetDropMainBodyPST(WholeInfoTable);
	//	if (this->MainBodyPS && DropMainBodyPST)
	//	{
	//		this->MainBodyPS->SetTemplate(DropMainBodyPST);
	//	}

	//	UParticleSystem* DropSuckingPS = InSingleInfoOfID->GetDropSuckingPS(WholeInfoTable);
	//	if (DropSuckingPS)
	//	{
	//		this->SuckingPS = DropSuckingPS;
	//	}

	//	USoundCue* DropPlaySound = InSingleInfoOfID->GetDropPlaySoundCue(WholeInfoTable);
	//	if (this->AudioComp && DropPlaySound)
	//	{
	//		this->AudioComp->SetSound(DropPlaySound);
	//	}

	//	this->GetCollisionComp()->SetSphereRadius(this->GetCollisionComp()->GetUnscaledSphereRadius() + InSingleInfoOfID->Offset_CollisionRadius);
	//}
}

void AB2DropItem::OverrideByBaseBP(TSubclassOf<AB2DropItem> BPClassToOverride)
{
	AB2DropItem* BPCDO = BPClassToOverride ? Cast<AB2DropItem>(BPClassToOverride->GetDefaultObject()) : NULL;
	if (!BPCDO)
		return;

	//// �̰��� �Ҹ��� ������ ItemInfo �����̳� BeginPlay ������.
	//// ���ҽ� ���۷��� ������ ��� ���� �� ���� ��� ������ �������� Ȯ���� �ʿ䰡 ����.

	//// ���� ������Ʈ���� ���� ó���ؾ� �Ѵ�. this ����� CopyObjectProperties ���� ���� ������Ʈ���� �����ϰ� �ϸ� ������Ʈ ��ü�� �״�� ������ ����ϹǷ� �ȵ�.

	//CopyObjectProperties(this->AudioComp, BPCDO->AudioComp, true, true);
	//CopyObjectProperties(this->MainBodyPS, BPCDO->MainBodyPS, true, true);
	//CopyObjectProperties(this->CollisionComp, BPCDO->CollisionComp, true, true);
	//CopyObjectProperties(this->DropItemMoveComp, BPCDO->DropItemMoveComp, true, true);
	//
	//// �ڱ� �ڽ��� �� ���߿� ó���� ����.
	//CopyObjectProperties(this, BPCDO, true, true);
}

////////////////////////////////////////////////////////////////////////////////
void FItemInfoEquipPartsAsyncReqCombinedID::SetRefIDs(const TArray<int32>& InRefIDs)
{
	// �̰� ����Ϸ��� ȯ�濡���� �뷫���� ����. �� ��Һ��� for �� �������� üũ�ϴ� �� �־ �̰� Ŀ���� ���� ������ �߻��� �� ����.
	// �뷫 �÷��̾� ĳ���� ��Ʈ ������ŭ �ʿ��ѵ� ���� �ϳ� ����ġ ���� ��쿡 ����ϱ� ����..
	check(InRefIDs.Num() <= 10);

	RefIDs.Empty();
	RefIDs = InRefIDs;
}
const FItemInfoEquipPartsAsyncReqCombinedID& FItemInfoEquipPartsAsyncReqCombinedID::operator=(const FItemInfoEquipPartsAsyncReqCombinedID& InOther)
{
	SetRefIDs(InOther.RefIDs);
	MySerialNumber = InOther.MySerialNumber;
	MyAsyncRequestName = InOther.MyAsyncRequestName;
	return *this;
}
bool FItemInfoEquipPartsAsyncReqCombinedID::operator==(const FItemInfoEquipPartsAsyncReqCombinedID& InOther) const
{
	// MySerialNumber �� MyAsyncRequestName �� ���� �ʿ䰡 ����. 
	// ó������ ���� �ٸ��� �ĺ��Ϸ��� �� RefIDs ��.. 
	// RefIDs ������ �ٸ��� SerialNumber �� ������ ������ ������, RefIDs ������ ���� SerialNumber �� �ٸ��ٰ� �ؼ� ������ �� ����.

	return IsEqualByRefIDs(InOther);
}

bool FItemInfoEquipPartsAsyncReqCombinedID::IsEqualByRefIDs(const FItemInfoEquipPartsAsyncReqCombinedID& InOther) const
{
	if (RefIDs.Num() == InOther.RefIDs.Num())
	{
		bool bHasAnyDifference = false;
		for (int32 IA = 0; IA < RefIDs.Num(); ++IA)
		{
			if (!InOther.RefIDs.Contains(RefIDs[IA])) // �� �迭 ������ �����Ƿ� ������ ��� ��Ұ� �ٸ� �ʿ� ���ԵǾ� �ִٸ� �Ȱ��� ��.
			{
				bHasAnyDifference = true;
				break;
			}
		}
		return !bHasAnyDifference;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

UB2ItemInfo::UB2ItemInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
//	EquipPartsAsyncLoadReqCounter = 0;
//
//	if (HasAnyFlags(RF_ClassDefaultObject) == false)
//	{
//		// ������ �ϳ��� �ε�
//		FString ItemDataTablePath;
//		GConfig->GetString(TEXT("/Script/BladeII.B2Item"), TEXT("ItemInfoDataTable"), ItemDataTablePath, GGameIni);
//
//		TheData = LoadObject<UDataTable>(NULL, *ItemDataTablePath);
//
//
//		// Set Info Table
//		FString ItemSetDataTablePath;
//		GConfig->GetString(TEXT("/Script/BladeII.B2Item"), TEXT("ItemSetInfoDataTable"), ItemSetDataTablePath, GGameIni);
//
//		SetInfoData = LoadObject<UDataTable>(NULL, *ItemSetDataTablePath);
//		GenerateSetKeyList();
//
//#if WITH_EDITOR && !PLATFORM_MAC
//		if (TheData == NULL)
//		{
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[Warning] ItemInfo ������ ���̺��� ã�� ����. ��ǻ�Ͱ� �� �����Ѵ�."))
//				));
//		}
//
//		if (SetInfoData == NULL)
//		{
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[Warning] ItemSetInfo ������ ���̺��� ã�� ����. ��ǻ�Ͱ� �� �����Ѵ�."))
//				));
//		}
//
//#endif
//	}
}

//FSingleItemInfoData* UB2ItemInfo::GetInfoData(int32 ItemRefID, bool bFullyLoad)
//{
//	// ItemRefID �� key �� ���.
//	FSingleItemInfoData* FoundDataRow = TheData ? TheData->FindRow<FSingleItemInfoData>(FName(*FString::FromInt(ItemRefID)), TEXT("")) : NULL;
//	if (FoundDataRow == NULL)
//	{
//#if WITH_EDITOR && !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//			FString::Printf(TEXT("ItemInfo ���̺��� RefID %d �� �׸��� ã�� ����. ��ǻ�Ͱ� �� �����Ѵ�."), ItemRefID)
//			));
//#endif
//		return NULL;
//	}
//
//	FoundDataRow->CachedMyRefID = ItemRefID; // RefID ĳ���� ������ SingleInfo �� ���� ������ �ε� �� ����ϰ� �� ��.
//
//	if (bFullyLoad
//#if WITH_EDITOR
//		|| GIsEditor
//#endif
//		)
//	{
//		// ���� LoadAllTAssets �� ���� �ʴ��� ���� ����� ������ ���� ���۷������� �ε��ϰ� �� ��. LoadAllTAssets ���δ� �������� �޸� �Ҹ��� ������ �ִ�.
//		FoundDataRow->LoadAllTAssets(InfoLoadManager, ItemRefID, LoadedPtrMap);
//	}
//
//	return FoundDataRow;
//}

int32 UB2ItemInfo::GetRandomItemRefID()
{
	return 1000100; //���ҽ� ������ �Ǿ� ���� �ʱ� ������ ù row�� �����۸� ����� ������ �ӽ� ���
	/*
	uint32 NumItems = TheData ? TheData->RowMap.Num() : 0;

	if (NumItems > 0)
	{
		int32 RandIndex = FMath::FRandRange(0.0f, (float)NumItems - 1.5f); //��� ����

		int32 Idx = 0;
		for (auto it = TheData->RowMap.CreateConstIterator(); it; ++it, ++Idx)
		{
			if (Idx == RandIndex)
			{
				int32 RandItemRefID;
				LexicalConversion::FromString(RandItemRefID, *(it.Key().ToString()));
				return RandItemRefID;
			}			
		}		
	}

	return INDEX_NONE;
	*/
}

//FItemSetInfoData* UB2ItemInfo::GetSetInfoData(int32 SetUniqueKey)
//{
//	const FName* DataTableKey = SetUniqueKeyMap.Find(SetUniqueKey);
//	if (SetInfoData && DataTableKey)
//	{
//		return SetInfoData->FindRow<FItemSetInfoData>(*DataTableKey, TEXT(""));
//	}
//
//	return nullptr;
//}
//
//FItemSetInfoData* UB2ItemInfo::GetSetInfoData(int32 GroupID, EPCClass PCClass, int32 Grade)
//{
//	const int32 SetUniqueKey = GetSetItemUniqueKey(GroupID, PCClass, Grade);
//	return GetSetInfoData(SetUniqueKey);
//}
//
//void UB2ItemInfo::GenerateSetKeyList()
//{
//	if (SetInfoData)
//	{
//		const TArray<FName>& RowKeys = SetInfoData->GetRowNames();
//		for (const FName& RowKey : RowKeys)
//		{
//			FItemSetInfoData* FoundDataRow = SetInfoData ? SetInfoData->FindRow<FItemSetInfoData>(RowKey, TEXT("")) : nullptr;
//			if (FoundDataRow)
//			{
//				const int32 SetUniqueKey = GetSetItemUniqueKey(FoundDataRow->GroupID, FoundDataRow->CharacterClass, FoundDataRow->Grade);
//				SetUniqueKeyMap.Add(SetUniqueKey, RowKey);
//			}
//		}
//	}
//}

void UB2ItemInfo::PreloadClassAssets(const TArray<int32>& InRefIDList)
{
//	for (int32 RI = 0; RI < InRefIDList.Num(); ++RI)
//	{
//		GetInfoData(InRefIDList[RI], true); // FullyLoad �� true �� �ְ� �θ��� �� �ε��. �̰� �뷫 ��� ���ֺ��� ������ Preload �ϴ� ����� �ʿ��� ����.
//	}
//#if !UE_BUILD_SHIPPING
//	if (InRefIDList.Num() > 0){
//		UE_LOG(LogBladeII, Log, TEXT("ItemInfo pre-load for %d assets"), InRefIDList.Num());
//	}
//#endif	
}

void UB2ItemInfo::UnloadAll()
{
	check(0); // �� ���.. �̰� �ϴ� ���� �ȵ� �� ����. UnloadAllExceptCurrentLocalEquipPartData �� ���..

	// DataTable �� ��� row �� iterate �ϴ� ����ε�.
	if (TheData)
	{
		TArray<FName> RowKeys = TheData->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			FSingleItemInfoData* FoundDataRow = TheData->FindRow<FSingleItemInfoData>(RowKeys[RI], TEXT(""));
			if (FoundDataRow)
			{
				int32 ThisRowNumber = FCString::Atoi(*RowKeys[RI].ToString());
				FoundDataRow->UnloadAllTAssets(InfoLoadManager, ThisRowNumber, LoadedPtrMap, true);
			}
		}
		LoadedPtrMap.Empty(); // Ȯ�� ���
	}	

	ResetAsyncLoadReqInfo();
}

void UB2ItemInfo::UnloadAllExceptCurrentLocalEquipPartData()
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2ItemInfo::UnloadAllExceptCurrentLocalEquipPartData"));

//	// Ư�� �κп��� ���� ���� ĳ���͵��� ���� ��� ��Ʈ�� ���õ� �Ϻο� �ʿ��� ������ �̿ܿ� ��� ���� ������ ����.
//	// ���� ��� �����̳� �κ��丮 ���� �� ���.
//	// .. �׸��� �ᱹ�� �Ϲ����� UnloadAll ��Ȳ�� �̰ɷ� ��ü�ؾ� �� ��.. ���� ĳ���� ���� ��Ʈ���� ���ܵδ� �� �� �߿�������.
//
//#if WITH_EDITOR
//	if (GIsEditor) {
//		return;
//	}
//#endif
//
//	TMap<int32, bool> AllEquippedRefIDs; // Value �� �ǹ̾��� �� Key �˻��� �� ��.
//
//	for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//	{
//		EPCClass CastedPCClass = IntToPCClass(PCI);
//
//		TArray<FB2Item> ThisEquippedList;
//		UB2LobbyInventory::GetAllEquippedItem(ThisEquippedList, CastedPCClass);
//
//		for (FB2Item& ThisEquipped : ThisEquippedList)
//		{
//			AllEquippedRefIDs.Add(ThisEquipped.ItemRefID, true); // Key ���� ���� ��.
//		}
//	}
//	
//	if (TheData)
//	{
//		// RefID ���� �ߺ��ؼ� ���Ǵ� ��Ʈ���� �־ ��Ʈ�� ó���� ���ǰ� �ʿ��ϴ�..
//		// �� ������ ���ҽ� �ε��� �����ϴ� ����� �����ε�.. �Ѥ�
//
//		// 1�� �н����� ��� RootSet ���� ����
//		TArray<FSingleItemInfoData*> AllRows;
//		TheData->GetAllRows<FSingleItemInfoData>(TEXT(""), AllRows);
//		for (int32 RI = 0; RI < AllRows.Num(); ++RI)
//		{
//			FSingleItemInfoData* FoundDataRow = AllRows[RI];
//			if (FoundDataRow)
//			{
//				FoundDataRow->MarkLoadedEquipPartAtRooSet(false);
//			}
//		}
//		// 2�� �н����� ���� ������ �͸� RootSet �� �߰�
//		for (TMap<int32, bool>::TIterator RefIDIt(AllEquippedRefIDs); RefIDIt; ++RefIDIt)
//		{
//			int32 ThisRefID = RefIDIt.Key();
//			FSingleItemInfoData* FoundDataRow = GetInfoData(ThisRefID, false);
//			if (FoundDataRow) {
//				FoundDataRow->MarkLoadedEquipPartAtRooSet(true);
//			}
//		}
//		// ���������� RootSet �� �� ������ Unload
//		TArray<FName> RowKeys = TheData->GetRowNames();
//		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
//		{
//			FName& ThisRowKey = RowKeys[RI];
//			FSingleItemInfoData* FoundDataRow = TheData->FindRow<FSingleItemInfoData>(ThisRowKey, TEXT(""));
//			if (FoundDataRow)
//			{ // GetAllRows �� �ѰŶ� ������ �� ��������...
//				const int32 ThisRowNumber = FCString::Atoi(*ThisRowKey.ToString());
//				FoundDataRow->UnloadAllTAssets(InfoLoadManager, ThisRowNumber, LoadedPtrMap, false);
//			}
//		}
//	}
}

FSingleItemInfoLoadedPtrData* UB2ItemInfo::GetLoadedPtrForID(int32 InItemID)
{
	FSingleItemInfoLoadedPtrData* FoundLoadedPtrEntry = LoadedPtrMap.Find(InItemID);
	if (!FoundLoadedPtrEntry && InItemID > 0) // ������ ���� ����.
	{
		FSingleItemInfoLoadedPtrData NewLoadedEntry;
		LoadedPtrMap.Add(InItemID, NewLoadedEntry);
		FoundLoadedPtrEntry = LoadedPtrMap.Find(InItemID);
	}
	return FoundLoadedPtrEntry;
}

AB2DropItem* UB2ItemInfo::SpawnSingleDropItem(class UWorld* SpawnWorld, int32 ItemRefID, const FTransform& SpawnTransform, const FActorSpawnParameters& SpawnParameters, float StayTime)
{
	FActorSpawnParameters FinalSpawnParam = SpawnParameters;
//	// PostSpawnInitialize �� BeginPlay �� ���� ���� �� ���� ItemInfo �� �����ϱ� ����.
//	// bDeferPostSpawnInitialize �� ���������� �����ϴ� PCClassInfo/NPCClassInfo �ʹ� �޸� bDeferConstruction �� ���������� �����ϴµ� FinishSpawning ������ Ư���� �� ���� ���ٰ� �ǴܵǱ� ����.
//	FinalSpawnParam.bDeferConstruction = true;
//	FinalSpawnParam.bDeferPostSpawnInitialize = true;
//
//	BII_CHECK(SpawnWorld);
//	if (SpawnWorld == NULL)
//	{
		return NULL;
//	}
//
//	FSingleItemInfoData* ThisInfo = GetInfoData(ItemRefID);
//	BII_CHECK(ThisInfo);
//	if (ThisInfo == NULL)
//	{
//		return NULL;
//	}
//
//	// PCClassInfo �� NPCClassInfo �� ���� ���������� �̰� ����� �������� SpawnParameters ���� bDeferConstruction �� ���õǾ� �־�� �� ��. �׷��� BeginPlay �� �� ���� �Ҹ��� ��.
//	// �⺻ Ŭ������ ���� ���� ���. (DropItem_DefaultBaseBPClass)
//	AB2DropItem* SpawnedActor = Cast<AB2DropItem>(SpawnWorld->SpawnActor(AB2DropItem::StaticClass(), &SpawnTransform, FinalSpawnParam));
//	BII_CHECK(SpawnedActor && SpawnedActor->IsValidObj());
//	if (SpawnedActor == NULL)
//	{
//		return NULL;
//	}
//
//	SpawnedActor->OverrideByItemInfo(this, ThisInfo);
//	SpawnedActor->SignificantData.ItemRefID = ItemRefID; 
//
//	// SignificantData �� ��Ÿ �������� ItemRefID �� ������� �������� �������µ� ���߹��� Ŭ���̾�Ʈ������ �Ʒ��� ���� �׽�Ʈ �����͸� �־��� �� ����.
////#if !UE_BUILD_SHIPPING 
//	//DevOnlyLoadItemClientTestData(SpawnedActor->SignificantData);
////#endif
//	
//
//	// ItemInfo ���� �� bDeferConstruction �� bDeferPostSpawnInitialize �� ���� ó��
//	if (SpawnedActor)
//	{
//		if(!FMath::IsNearlyZero(StayTime))
//		{
//			SpawnedActor->SetQTEStayTime(StayTime);
//		}
//
//		SpawnWorld->ManualPostSpawnInitialize(SpawnedActor, SpawnTransform, FinalSpawnParam);
//
//		// FinishSpawning ���� BeginPlay �� �Ҹ�. ���� ������ spawn ��Ȳ�� ���� �̰� ���߿� ó���ؾ� �ϴ� ��찡 ����ٸ� ��� bDeferConstruction �����ϴ� �κа� �Բ� �̰� ������ �� ��.
//		SpawnedActor->FinishSpawning(SpawnTransform);
//	}
//
//	return SpawnedActor;
}

#if WITH_EDITOR
void UB2ItemInfo::EditorLoadAll(SetB2ContentLoadingProgressSplashFnPtr InSplashProgFnPtr, float InStartProgress, float InEndProgress)
{
	//if (TheData == NULL)
	//{
	//	return;
	//}

	//// DataTable �� ��� row �� iterate �ϴ� ����ε�.
	//TArray<FName> RowKeys = TheData->GetRowNames();
	//
	//const float TotalProgRange = FMath::Max(0.0f, InEndProgress - InStartProgress);
	//// InfoAsset �� ��ϵǴ� �ֵ��� �󸶳� �Ǵ����� ���� ������ ���ϴ� ��. ������� �ѹ��� ������Ʈ �� ��.
	//const int32 ProgUpdateInterval = 10;
	//const int32 TotalProgUpdateCount = RowKeys.Num() / ProgUpdateInterval;
	//const float SingleProgIncAmount = TotalProgRange / (float)TotalProgUpdateCount;

	//float CurrProg = InStartProgress;
	//
	//for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
	//{
	//	GetInfoData(FCString::Atoi(*RowKeys[RI].ToString()), true); // �ȿ��� �ٽ� FName �� �Ǵ� �����ε� ������ ������ ����̴�..

	//	// Splash progress ������Ʈ. �Ʒ� SetInfoData �� �� �ȵ� �ɷ� ġ��..
	//	if (InSplashProgFnPtr && (RI % ProgUpdateInterval == ProgUpdateInterval - 1))
	//	{
	//		CurrProg = FMath::Min(InEndProgress, CurrProg + SingleProgIncAmount);
	//		InSplashProgFnPtr(CurrProg);
	//	}
	//}

	//if (SetInfoData)
	//{
	//	TArray<FName> SetRowKeys = SetInfoData->GetRowNames();
	//	for (int32 RI = 0; RI < SetRowKeys.Num(); ++RI)
	//	{
	//		GetSetInfoData(FCString::Atoi(*SetRowKeys[RI].ToString())); // �ȿ��� �ٽ� FName �� �Ǵ� �����ε� ������ ������ ����̴�..
	//	}
	//}


	//// �̰� ����Ϸ��� BladeIIGameImpl::StartupModule ���� �ϰ� �� �ٵ� �׷��� �̰��� �ν��Ͻ� ��ġ�� �ٲ�� �� ��..
}

void UB2ItemInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{ 
	//
	// DataTable ����Ʈ �ÿ��� �̰� �ȸ�����..
	//

	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName Name_LocalizedNameKey = GET_MEMBER_NAME_CHECKED(FSingleItemInfoData, LocalizedNameKey);

	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	if (PropertyName == Name_LocalizedNameKey)
	{
		TArray<FName> RowKeys = TheData->GetRowNames();
		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
		{
			FSingleItemInfoData* FoundDataRow = TheData->FindRow<FSingleItemInfoData>(RowKeys[RI], TEXT(""));
			//FoundDataRow->LoadLocalizedName(); ���� ���� �������.
		}
	}
}
#endif

UMaterialInterface* UB2ItemInfo::GetItemIcon(int32 ItemRefId)
{
	//auto* ItemData = GetInfoData(ItemRefId);
	//if (ItemData == nullptr)
		return nullptr;

	//return ItemData->GetIconMaterial(this);
}

class UMaterialInterface* UB2ItemInfo::GetSetItemIcon(int32 SetUniqueKey)
{
	//FItemSetInfoData* SetInfo = GetSetInfoData(SetUniqueKey);
	//if (SetInfo == nullptr)
		return nullptr;

	//return SetInfo->IconMaterial.LoadSynchronous();
}

FText UB2ItemInfo::GetItemName(int32 ItemRefId, bool bAttachQualityPrefix, int32 ItemQuality)
{
	//auto* ItemData = GetInfoData(ItemRefId);
	//if (ItemData == nullptr)
		return FText::FromString(TEXT("Unknown Item"));

	//return ItemData->GetLocalizedName(bAttachQualityPrefix, ItemQuality);
}

//FText UB2ItemInfo::GetSetItemName(int32 SetUniqueKey)
//{
//	const int32 SetGroupID = GetSetGroupIDFromUniqueKey(SetUniqueKey);
//	return GetLOCTextOfSetItemName(SetGroupID);
//}

void UB2ItemInfo::GetItemIconAndName(int32 ItemRefId, class UMaterialInstance*& ItemIcon, FText& ItemName)
{
	//ItemIcon = nullptr;

	//UB2ItemInfo* AllItemInfo = StaticFindItemInfo();

	//if (AllItemInfo)
	//{
	//	ItemIcon = Cast<UMaterialInstance>(AllItemInfo->GetItemIcon(ItemRefId));
	//	ItemName = AllItemInfo->GetItemName(ItemRefId);
	//}
}

void UB2ItemInfo::OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FB2AsyncRequestInfo>& CompleteRequestList)
{
//	TArray<int32> JustLoadedRefIDs;
//
//	// PendingEquipPartsAsyncLoadReqList ���� �Ϸ�� ��Ʈ���� �ĺ��ϰ� ����Ʈ���� ����.
//	bool bFoundMatchingCombinedID = false;
//	for (int32 RI = 0; RI < PendingEquipPartsAsyncLoadReqList.Num(); ++RI)
//	{
//		FItemInfoEquipPartsAsyncReqCombinedID& ThisID = PendingEquipPartsAsyncLoadReqList[RI];
//		if (ThisID.MyAsyncRequestName == CompleteRequest)
//		{
//			JustLoadedRefIDs = ThisID.GetRefIDs();
//			PendingEquipPartsAsyncLoadReqList.RemoveAt(RI);
//			bFoundMatchingCombinedID = true;
//			break;
//		}
//	}
//
//	// �ε��� ���µ��� ĳ��
//	for (int32 LoadedRefID : JustLoadedRefIDs)
//	{
//		// ���⼭ FullyLoad �� �ָ� ĳ���� �˾Ƽ� �Ǳ� ������
//		// �� Async �ε��� ���� Info �� ��ϵ� ��� �� �ε��Ϸ��� �� �ƴϹǷ� ���ó� FullyLoad ���� �׳�.
//		// �Դٰ� �ű⼭ FullyLoad �ϴ� �� async flush ��Ұ� �־ async �ε��� �����ؼ� ���� ����ġ �ʴ�.
//		FSingleItemInfoData* ThisLoadedInfo = GetInfoData(LoadedRefID, false); 
//		if (ThisLoadedInfo)
//		{
//			ThisLoadedInfo->CacheOnEquipPartsAsyncLoadComplete(LoadedRefID, LoadedPtrMap);
//		}
//	}
//
//	// �Ϸ�� Request �̸��� ��Ƽ� �̺�Ʈ ����.
//	if (OnEquipPartsAyncLoadComplete.IsBound())
//	{
//		OnEquipPartsAyncLoadComplete.Broadcast(CompleteRequest);
//	}
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (!bFoundMatchingCombinedID)
//	{
//		// �̷� ���� ���� ����? �Ȱ��� RefID �������� ���޾� �ε� ��û�� ���� ��� �� �ٿ� ���� ���� �ݹ��� �´ٸ� �̷� ���� �߻��� �ٵ� ������ �׷� ��쿡�� �ѹ��� �´�.
//		FString AssetKeyString;
//		for (const FB2AsyncRequestInfo& ThisReqInfo : CompleteRequestList)
//		{
//			AssetKeyString += FString::Printf(TEXT("%d,"), ThisReqInfo.AssetKey);
//		}
//		BII_SCREEN_LOG(FString::Printf(TEXT("[UB2ItemInfo] No matching CombinedID OnAsyncLoadComplete.. ReqName %s, AssetKeys: %s"), *CompleteRequest, *AssetKeyString),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 5.0f);
//	}
//#endif
}

bool UB2ItemInfo::TryAsyncLoadOfEquipParts(const TArray<int32>& InRefIDList, FString& OutRequestedName, bool bShowBlockingUI)
{
	//TArray<FB2AsyncRequestInfo> TotalReqInfo;

	//for (int32 ThisRefID : InRefIDList)
	//{ // �ε� ���� �׳� InfoData �����⸸
	//	FSingleItemInfoData* ThisInfoData = GetInfoData(ThisRefID, false);
	//	if (ThisInfoData)
	//	{
	//		ThisInfoData->GetEquipPartsAsyncLoadReqInfo(TotalReqInfo); // �� InfoData ���� �ܾ�� �� ������Ŵ. �� ����
	//	}
	//}


	////
	//// GetAsyncReqCombinedIDofPartsList �� ���� üũ�� ���� �̹� pending �̸� return false �� ����?
	//// ���޾� ���� �ߺ� ��û ���� ����..
	////FItemInfoEquipPartsAsyncReqCombinedID AsyncReqCombinedID; 
	////if (GetAsyncReqCombinedIDofPartsList(InRefIDList, AsyncReqCombinedID, false))
	////{
	////	return false;
	////}
	////


	//FItemInfoEquipPartsAsyncReqCombinedID AsyncReqCombinedID; // Async �ε� ��û�̶� �Ϸ� ���� �ĺ��� �ʿ��� ����.
	//if (GetAsyncReqCombinedIDofPartsList(InRefIDList, AsyncReqCombinedID, true)) // ���Ⱑ �����ϰ� ������ ������ ���� ������ �� ���� �� ��.
	//{
	//	check(AsyncReqCombinedID.IsValid());
	//	TryAsyncLoad(AsyncReqCombinedID.MyAsyncRequestName, TotalReqInfo, bShowBlockingUI);
	//	OutRequestedName = AsyncReqCombinedID.MyAsyncRequestName; // ���� �ʿ��� �ʿ��ϸ� �̰� ������ �Ϸ� �̺�Ʈ�� ���� �� �ĺ� ����.
	//	return true;
	//}
	//
	return false;
}

// �ܼ� Get ���μ��� ������ �� �ƴ� Generate �� �ǹ̰� �� �ִ�. const �����̾�� �ϴ� �� �ƴ�. 
bool UB2ItemInfo::GetAsyncReqCombinedIDofPartsList(const TArray<int32>& InRefIDList,
	FItemInfoEquipPartsAsyncReqCombinedID& OutNewOrGeneratedID, bool bGenerateNewOnMiss)
{
	FItemInfoEquipPartsAsyncReqCombinedID LocalTempIDStruct;
	LocalTempIDStruct.SetRefIDs(InRefIDList);

	int32 FoundIndexFromPending = PendingEquipPartsAsyncLoadReqList.Find(LocalTempIDStruct);
	if (FoundIndexFromPending != INDEX_NONE)
	{ // ���� async �ε� ���� ��..
		OutNewOrGeneratedID = PendingEquipPartsAsyncLoadReqList[FoundIndexFromPending];
		return true;
	}

	if (bGenerateNewOnMiss)
	{
		// �ǹ̾��� ��� �ٸ� �������� Get �ϴٰ� SerialNumber �� ���� �����ϴ� ���� ������.. (�׸��� PendingEquipPartsAsyncLoadReqList �� ����..)
		// ���� ��� Ȯ���� ���� ������� �� ���� bGenerateNewOnMiss �� ����.

		// RefID ����Ʈ �������� unique �ϰ� �ĺ� ������ ���ڿ��� ���̰��� ������� ���� ���� ���� �״� �Ϸù�ȣ�� �ο��Ѵ�
		LocalTempIDStruct.MySerialNumber = EquipPartsAsyncLoadReqCounter;
		++EquipPartsAsyncLoadReqCounter; // ���Ⱑ �����ϴ� ������ ������ �Ǿ�� �Ѵ�.
		// TryAsyncLoad �� RequestName ���� �� ��
		LocalTempIDStruct.MyAsyncRequestName = FString::Printf(TEXT("ItemInfoEquipPartBunch_%d"), LocalTempIDStruct.MySerialNumber);
		OutNewOrGeneratedID = LocalTempIDStruct;
		PendingEquipPartsAsyncLoadReqList.Add(LocalTempIDStruct);

		return true;
	}

	return false;
}
void UB2ItemInfo::ResetAsyncLoadReqInfo()
{
	PendingEquipPartsAsyncLoadReqList.Empty();
	EquipPartsAsyncLoadReqCounter = 0;
}

////////////////////////////////////////////////////////////////////////////////
// B2ItemMisInfo

UB2ItemMiscInfo::UB2ItemMiscInfo(const FObjectInitializer& ObjectInitializer)
{

}

UMaterialInterface* UB2ItemMiscInfo::GetItemIconBGMtrl(int32 InStarGrade, bool bSimpleBG, bool bForConsumables)
{
	if (bForConsumables) {
		return ConsumablesIconBGMtrl.LoadSynchronous(); // Ư���� ���۷����� �����ؾ� �� ������ �ִ� �� �ƴ� �׳� LoadSynchronous ���� �ȴ�. ������ �޸� ������ �� �ȵǾ RefHolder ��� �Ϳ� ĳ���� �ߴ� ����.
	}

	int32 ElemIndex = InStarGrade - 1;
	if (bSimpleBG)
	{
		if (ItemIconBGMtrlPerGrade_Simple.IsValidIndex(ElemIndex))
		{
			UMaterialInterface* LoadedAsset = NULL;
			GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, ItemIconBGMtrlPerGrade_Simple[ElemIndex], &LoadedAsset, TEXT("ItemMiscInfo"));
			if (LoadedAsset)
			{ 
				AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedItemIconBGMtrlPerGrade_Simple); // �ε��� ���� ĳ��. �ε��� �Ȱ��� ���缭
				return LoadedAsset;
			}
		}
	}
	else
	{
		if (ItemIconBGMtrlPerGrade.IsValidIndex(ElemIndex))
		{
			UMaterialInterface* LoadedAsset = NULL;
			GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, ItemIconBGMtrlPerGrade[ElemIndex], &LoadedAsset, TEXT("ItemMiscInfo"));
			if (LoadedAsset)
			{
				AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedItemIconBGMtrlPerGrade); // �ε��� ���� ĳ��. �ε��� �Ȱ��� ���缭
				return LoadedAsset;
			}
		}
	}
	return NULL;
}
void UB2ItemMiscInfo::UnloadItemIconBGMtrl(int32 InStarGrade, bool bSimpleBG)
{
	int32 ElemIndex = InStarGrade - 1;
	if (bSimpleBG)
	{
		if (ItemIconBGMtrlPerGrade_Simple.IsValidIndex(ElemIndex))
		{
			AllLoadedRefHolder.LoadedItemIconBGMtrlPerGrade_Simple.Remove(ElemIndex);
			UMaterialInterface* DummyRef = NULL;
			UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, ItemIconBGMtrlPerGrade_Simple[ElemIndex], &DummyRef);
		}
	}
	else
	{
		if (ItemIconBGMtrlPerGrade.IsValidIndex(ElemIndex))
		{
			AllLoadedRefHolder.LoadedItemIconBGMtrlPerGrade.Remove(ElemIndex);
			UMaterialInterface* DummyRef = NULL;
			UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, ItemIconBGMtrlPerGrade[ElemIndex], &DummyRef);
		}
	}
}

class UMaterialInterface* UB2ItemMiscInfo::GetEquipCategoryIcon(EEquipCategoryType InEquipCategory)
{
	if (InEquipCategory == EEquipCategoryType::End)
		return nullptr;

	int32 ElemIndex = static_cast<int32>(InEquipCategory) - 1;
	
	if (EquipCategoryIcon.IsValidIndex(ElemIndex))
	{
		UMaterialInterface* LoadedAsset = NULL;
		GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, EquipCategoryIcon[ElemIndex], &LoadedAsset, TEXT("ItemMiscInfo"));
		if (LoadedAsset)
		{
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedEquipCategoryIcon); // �ε��� ���� ĳ��. �ε��� �Ȱ��� ���缭
			return LoadedAsset;
		}
	}
	return NULL;
}

void UB2ItemMiscInfo::UnloadEquipCategoryIcon(EEquipCategoryType InEquipCategory)
{
	if (InEquipCategory == EEquipCategoryType::End)
		return;

	int32 ElemIndex = static_cast<int32>(InEquipCategory) - 1;
	if (EquipCategoryIcon.IsValidIndex(ElemIndex))
	{
		AllLoadedRefHolder.LoadedEquipCategoryIcon.Remove(ElemIndex);
		UMaterialInterface* DummyRef = NULL;
		UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, EquipCategoryIcon[ElemIndex], &DummyRef);
	}
}

FText UB2ItemMiscInfo::GetEquipCategoryName(EEquipCategoryType InEquipCategory)
{
	switch (InEquipCategory)
	{
	case EEquipCategoryType::WEAPON:
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_WeaponTab"));
	case EEquipCategoryType::ARMOR:
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Protection"));
	case EEquipCategoryType::ACCESSORY:
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_Accessory"));
	}

	return FText();
}

UMaterialInterface* UB2ItemMiscInfo::GetEquipIconBGMtrl(int32 InStarGrade)
{
	int32 ElemIndex = InStarGrade - 1;
	if (EquipIconBGMtrlPerGrade.IsValidIndex(ElemIndex))
	{
		UMaterialInterface* LoadedAsset = NULL;
		GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, EquipIconBGMtrlPerGrade[ElemIndex], &LoadedAsset, TEXT("ItemMiscInfo"));

		if (LoadedAsset)
		{
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedEquipIconBGMtrlPerGrade); // �ε��� ���� ĳ��. �ε��� �Ȱ��� ���缭
			return LoadedAsset;
		}
	}
	return NULL;
}
void UB2ItemMiscInfo::UnloadEquipIconBGMtrl(int32 InStarGrade)
{
	int32 ElemIndex = InStarGrade - 1;
	if (EquipIconBGMtrlPerGrade.IsValidIndex(ElemIndex))
	{
		AllLoadedRefHolder.LoadedEquipIconBGMtrlPerGrade.Remove(ElemIndex);
		UMaterialInterface* DummyRef = NULL;
		UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, EquipIconBGMtrlPerGrade[ElemIndex], &DummyRef);
	}
}

FSlateColor UB2ItemMiscInfo::GetItemNameTextColor(int32 InStarGrade)
{
	int32 ElemIndex = InStarGrade - 1;
	if (ItemNameTextColorPerGrade.IsValidIndex(ElemIndex)) {
		return ItemNameTextColorPerGrade[ElemIndex];
	}
	return FSlateColor(FColor::White);
}

UMaterialInterface* UB2ItemMiscInfo::GetItemStarGradeImage(bool bSurpassed)
{
	UMaterialInterface* LoadedAsset = NULL;
	if (bSurpassed)
	{
		LoadedAsset = GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, ItemStarGradeImage_Surpassed, &AllLoadedRefHolder.LoadedItemStarGradeImage_Surpassed, TEXT("ItemMiscInfo"));
	}
	else
	{
		LoadedAsset = GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, ItemStarGradeImage_Normal, &AllLoadedRefHolder.LoadedItemStarGradeImage_Normal, TEXT("ItemMiscInfo"));
	}

	return LoadedAsset;
}
void UB2ItemMiscInfo::UnloadItemStarGradeImage(bool bSurpassed)
{
	if (bSurpassed)
	{
		UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, ItemStarGradeImage_Surpassed, &AllLoadedRefHolder.LoadedItemStarGradeImage_Surpassed);
	}
	else
	{
		UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, ItemStarGradeImage_Normal, &AllLoadedRefHolder.LoadedItemStarGradeImage_Normal);
	}
}

UMaterialInterface* UB2ItemMiscInfo::GetItemQualityBGMtrl(int32 InQuality)
{
	int32 ElemIndex = InQuality - 1;
	if (ItemQualityBGMtrl.IsValidIndex(ElemIndex))
	{
		UMaterialInterface* LoadedAsset = NULL;
		GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, ItemQualityBGMtrl[ElemIndex], &LoadedAsset, TEXT("ItemMiscInfo"));

		if (LoadedAsset)
		{
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedItemQualityBGMtrl); // �ε��� ���� ĳ��. �ε��� �Ȱ��� ���缭
			return LoadedAsset;
		}
	}
	return NULL;
}
void UB2ItemMiscInfo::UnloadItemQualityBGMtrl(int32 InQuality)
{
	int32 ElemIndex = InQuality - 1;
	if (ItemQualityBGMtrl.IsValidIndex(ElemIndex))
	{
		AllLoadedRefHolder.LoadedItemQualityBGMtrl.Remove(ElemIndex);
		UMaterialInterface* DummyRef = NULL;
		UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, ItemQualityBGMtrl[ElemIndex], &DummyRef);
	}
}

class UMaterialInterface* UB2ItemMiscInfo::GetEtherTierBGMtrl(int32 EtherTier)
{
	if (EtherTierBGMtrl.Contains(EtherTier))
	{
		UMaterialInterface* LoadedAsset = NULL;
		GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, EtherTierBGMtrl[EtherTier], &LoadedAsset, TEXT("ItemMiscInfo"));

		if (LoadedAsset)
		{
			AddOrUpdateMtrlRefHolderCache(EtherTier, LoadedAsset, AllLoadedRefHolder.LoadedEtherTierBGMtrl); // �ε��� ���� ĳ��. �ε��� �Ȱ��� ���缭
			return LoadedAsset;
		}
	}
	return NULL;
}

void UB2ItemMiscInfo::UnloadEtherTierBGMtrl(int32 EtherTier)
{
	if (EtherTierBGMtrl.Contains(EtherTier))
	{
		AllLoadedRefHolder.LoadedEtherTierBGMtrl.Remove(EtherTier);
		UMaterialInterface* DummyRef = NULL;
		UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, EtherTierBGMtrl[EtherTier], &DummyRef);
	}
}

class UMaterialInterface* UB2ItemMiscInfo::GetEtherTierNameBGMtrl(int32 EtherTier)
{
	if (EtherTierNameBGMtrl.Contains(EtherTier))
	{
		UMaterialInterface* LoadedAsset = NULL;
		GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, EtherTierNameBGMtrl[EtherTier], &LoadedAsset, TEXT("ItemMiscInfo"));

		if (LoadedAsset)
		{
			AddOrUpdateMtrlRefHolderCache(EtherTier, LoadedAsset, AllLoadedRefHolder.LoadedEtherTierNameBGMtrl); // �ε��� ���� ĳ��. �ε��� �Ȱ��� ���缭
			return LoadedAsset;
		}
	}
	return NULL;
}

void UB2ItemMiscInfo::UnloadEtherTierNameBGMtrl(int32 EtherTier)
{
	if (EtherTierNameBGMtrl.Contains(EtherTier))
	{
		AllLoadedRefHolder.LoadedEtherTierNameBGMtrl.Remove(EtherTier);
		UMaterialInterface* DummyRef = NULL;
		UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, EtherTierNameBGMtrl[EtherTier], &DummyRef);
	}
}

FSlateColor UB2ItemMiscInfo::GetEtherTierTextColor(int32 EtherTier)
{
	FSlateColor Result = FSlateColor(FColor::White);
	if (EtherTierTextColor.Contains(EtherTier))
	{
		Result = EtherTierTextColor[EtherTier];
	}
	return Result;
}

class UMaterialInterface* UB2ItemMiscInfo::GetStarNumberMtrl(int32 ElemIndex)
{
	if (StarNumberMtrl.Contains(ElemIndex))
	{
		UMaterialInterface* LoadedAsset = NULL;
		GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, StarNumberMtrl[ElemIndex], &LoadedAsset, TEXT("ItemMiscInfo"));

		if (LoadedAsset)
		{
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedStarNumberMtrl); // �ε��� ���� ĳ��. �ε��� �Ȱ��� ���缭
			return LoadedAsset;
		}
	}
	return NULL;
}

void UB2ItemMiscInfo::UnloadStarNumberMtrl(int32 ElemIndex)
{
	if (StarNumberMtrl.Contains(ElemIndex))
	{
		AllLoadedRefHolder.LoadedStarNumberMtrl.Remove(ElemIndex);
		UMaterialInterface* DummyRef = NULL;
		UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, StarNumberMtrl[ElemIndex], &DummyRef);
	}
}

class UMaterialInterface* UB2ItemMiscInfo::GetEtherPosBGMtrl(int32 ElemIndex)
{
	if (EtherPosBGMtrl.Contains(ElemIndex))
	{
		UMaterialInterface* LoadedAsset = NULL;
		GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, EtherPosBGMtrl[ElemIndex], &LoadedAsset, TEXT("ItemMiscInfo"));

		if (LoadedAsset)
		{
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedEtherPosBGMtrl); // �ε��� ���� ĳ��. �ε��� �Ȱ��� ���缭
			return LoadedAsset;
		}
	}
	return NULL;
}

void UB2ItemMiscInfo::UnloadEtherPosBGMtrl(int32 ElemIndex)
{
	if (EtherPosBGMtrl.Contains(ElemIndex))
	{
		AllLoadedRefHolder.LoadedEtherPosBGMtrl.Remove(ElemIndex);
		UMaterialInterface* DummyRef = NULL;
		UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, EtherPosBGMtrl[ElemIndex], &DummyRef);
	}
}

UMaterialInterface* UB2ItemMiscInfo::GetCostumeIconBGMtrl(int32 InStarGrade)
{
	int32 ElemIndex = InStarGrade - 1;
	if (CostumeIconBGMtrlPerGrade.IsValidIndex(ElemIndex))
	{
		UMaterialInterface* LoadedAsset = NULL;
		GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, CostumeIconBGMtrlPerGrade[ElemIndex], &LoadedAsset, TEXT("ItemMiscInfo"));

		if (LoadedAsset)
		{
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedCostumeIconBGMtrlPerGrade); // �ε��� ���� ĳ��. �ε��� �Ȱ��� ���缭
			return LoadedAsset;
		}
	}
	return NULL;
}
void UB2ItemMiscInfo::UnloadCostumeIconBGMtrl(int32 InStarGrade)
{
	int32 ElemIndex = InStarGrade - 1;
	if (CostumeIconBGMtrlPerGrade.IsValidIndex(ElemIndex))
	{
		AllLoadedRefHolder.LoadedCostumeIconBGMtrlPerGrade.Remove(ElemIndex);
		UMaterialInterface* DummyRef = NULL;
		UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, CostumeIconBGMtrlPerGrade[ElemIndex], &DummyRef);
	}
}

UMaterialInterface* UB2ItemMiscInfo::GetTotemIconNotSmelted()
{
	UMaterialInterface* LoadedAsset = NULL;

	LoadedAsset = GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, TotemIcon_NotSmelted, &AllLoadedRefHolder.LoadedTotemIcon_NotSmelted, TEXT("ItemMiscInfo"));

	return LoadedAsset;
}
void UB2ItemMiscInfo::UnloadTotemIconNotSmelted()
{
	UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, TotemIcon_NotSmelted, &AllLoadedRefHolder.LoadedTotemIcon_NotSmelted);
}

class UMaterialInterface* UB2ItemMiscInfo::GetTotemOptionIndexIcon(int32 InIndex)
{
	int32 ElemIndex = InIndex - 1;
	if (TotemOptionIndexIcon.IsValidIndex(ElemIndex))
	{
		UMaterialInterface* LoadedAsset = NULL;
		GetSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, TotemOptionIndexIcon[ElemIndex], &LoadedAsset, TEXT("ItemMiscInfo"));

		if (LoadedAsset)
		{
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedTotemOptionIndexIcon); // �ε��� ���� ĳ��. �ε��� �Ȱ��� ���缭
			return LoadedAsset;
		}
	}
	return NULL;
}
void UB2ItemMiscInfo::UnloadTotemOptionIndexIcon(int32 InIndex)
{
	int32 ElemIndex = InIndex - 1;
	if (TotemOptionIndexIcon.IsValidIndex(ElemIndex))
	{
		AllLoadedRefHolder.LoadedTotemOptionIndexIcon.Remove(ElemIndex);
		UMaterialInterface* DummyRef = NULL;
		UnloadSomeLazyLoadedAssetCommon<UMaterialInterface>(InfoLoadManager, TotemOptionIndexIcon[ElemIndex], &DummyRef);
	}
}

void UB2ItemMiscInfo::AddOrUpdateMtrlRefHolderCache(int32 EntryIndex, UMaterialInterface* MtrlToCache, TMap<int32, class UMaterialInterface*>& HolderMap)
{ // �� ���⼭ ���̴� �ܼ��� �ݺ� �ڵ�
	if (MtrlToCache)
	{
		UMaterialInterface** FoundCachedAssetPtr = HolderMap.Find(EntryIndex);
		if (!FoundCachedAssetPtr) { 
			HolderMap.Add(EntryIndex, MtrlToCache);
		}
		else {
			*FoundCachedAssetPtr = MtrlToCache;
		}
	}
}

void UB2ItemMiscInfo::UnloadAll()
{
	//AllLoadedRefHolder.ClearAll();

	//for (int32 GI = 1; GI <= FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE; ++GI)
	//{
	//	UnloadItemIconBGMtrl(GI, true);
	//	UnloadItemIconBGMtrl(GI, false);
	//	UnloadEquipIconBGMtrl(GI);
	//	UnloadCostumeIconBGMtrl(GI);
	//}
	//for (int32 QI = 1; QI <= MAX_ITEM_QUALITY; ++QI)
	//{
	//	UnloadItemQualityBGMtrl(QI);
	//}
	//UnloadItemStarGradeImage(true);
	//UnloadItemStarGradeImage(false);

	//UnloadTotemIconNotSmelted();

	//for (int32 i = 1; i <= MAX_TOTEM_SUB_OPTION; ++i)
	//{
	//	UnloadTotemOptionIndexIcon(i);
	//}

	//for (int32 i = 1; i < (int32)EEquipCategoryType::End; ++i)
	//	UnloadEquipCategoryIcon((EEquipCategoryType)i);

	//for (TPair<int32, TSoftObjectPtr<UMaterialInterface>>& Elem : EtherPosBGMtrl)
	//{
	//	UnloadEtherPosBGMtrl(Elem.Key);
	//}

	//for (TPair<int32, TSoftObjectPtr<UMaterialInterface>>& Elem : EtherTierBGMtrl)
	//{
	//	UnloadEtherTierBGMtrl(Elem.Key);
	//}

	//for (TPair<int32, TSoftObjectPtr<UMaterialInterface>>& Elem : EtherTierNameBGMtrl)
	//{
	//	UnloadEtherTierNameBGMtrl(Elem.Key);
	//}
}

#if WITH_EDITOR
//void UB2ItemMiscInfo::EditorLoadAll()
//{
//	for (int32 GI = 1; GI <= FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE; ++GI)
//	{
//		GetItemIconBGMtrl(GI, true, false);
//		GetItemIconBGMtrl(GI, false, false);
//		GetEquipIconBGMtrl(GI);
//		GetCostumeIconBGMtrl(GI);
//	}
//	for (int32 QI = 1; QI <= MAX_ITEM_QUALITY; ++QI)
//	{
//		GetItemQualityBGMtrl(QI);
//	}
//	GetItemStarGradeImage(true);
//	GetItemStarGradeImage(false);
//
//	GetTotemIconNotSmelted();
//
//	for (int32 i = 1; i <= MAX_TOTEM_SUB_OPTION; ++i)
//	{
//		GetTotemOptionIndexIcon(i);
//	}
//
//	for (int32 i = 1; i < (int32)EEquipCategoryType::End; ++i)
//		GetEquipCategoryIcon((EEquipCategoryType)i);
//}
#endif

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2ItemMiscInfo::CleanupOnPreSave()
{
	AllLoadedRefHolder.ClearAll();
}
void UB2ItemMiscInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	// �ǵ�ġ �ʰ� ����� ���۷����� �����ش�.
	CleanupOnPreSave();
}
#endif