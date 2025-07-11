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

// 단순반복 코드 블록. 멤버 이름을 넣어주어야 해서 템플릿도 안되고..
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

	// 처음에 아이템 이름에 품질 접두사를 붙이는 식이었는데 품질 접두사는 따로 표시하게 됨. 혹시 어떤 식으로 부활할지 모르니 매개변수랑 코드는 남겨둠.
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
	// 매 FSingleItemInfoData 를 얻어올 때마다 사용하는 건 아니고 기본적으로는 여기 매 멤버들은 필요할 때 그때그때 로딩한다.
	// 여긴 GetSomeLazyLoadedAssetCommon 안 쓰고 그냥 가는 걸로.

	int32 CheckTAssets = 0; // TAsset 멤버 추가시 FSingleItemInfoLoadedPtrData 에도 추가 까먹지 않게.
	if (IconMaterial.IsValid() == false && IconMaterial.ToString().Len() > 0)
	{
		InLoadManager.LoadSynchronous(IconMaterial);
	}
	++CheckTAssets; // 복붙할 때 이것도 같이 낄낄
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
	checkSlow(FoundLoadedPtrEntry); // 없으면 바보
	if (FoundLoadedPtrEntry)
	{
		int32 CheckLoadedPtr = 0;
		
		// GC 방지용 로딩된 레퍼런스 캐싱. 이쪽에 선언된 TSoftObjectPtr 멤버들에 대해 FSingleItemInfoLoadedPtrData 에 항목을 넣어주어야 함.
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
				FString::Printf(TEXT("[Warning] FSingleItemInfoData 의 TAsset 멤버와 FSingleItemInfoLoadedPtrData 의 로딩된 에셋 레퍼런스 항목 수가 다르다. 컴퓨터가 곧 폭발한다."))
				));
		}
#endif
	}
}

void FSingleItemInfoData::UnloadAllTAssets(FStreamableManager& InLoadManager, int32 MyRefID, TMap<int32, FSingleItemInfoLoadedPtrData>& LoadedPtrMap, bool bUnloadEvenRootSet)
{
	if (bUnloadEvenRootSet)
	{
		MarkLoadedEquipPartAtRooSet(false); // 혹여나 RootSet 이라면 그거 제거부터 해야.
	}

	// bUnloadEvenRootSet 이 아니라도.. 날려도 어차피 실제 unloading 안 될 것임. 루트셋이니.
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
	// PCPart** 및 이와 관련해서 필요한 SixStar** 등은 여기서 제외한다.
	// 만일 이런 선택적 언로딩이 다른 식으로 필요해지면 개별 TAsset 들에 대한 언로딩을 제공하는 게 좋을 것.

	FSingleItemInfoLoadedPtrData* MyCachedRefContainer = LoadedPtrMap.Find(MyRefID);
	if (MyCachedRefContainer) // 못 찾으면 진작에 로딩을 하지 않은 거든지, 뭔가 잘못되었든지.
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
	if (bUnloadEvenRootSet)  // 혹여나 RootSet 이라면 그거 제거부터 하지만 필요한 경우 RootSet 으로 지정한 건 언로딩에서 제외시킬 수 있도록.
	{
		MarkLoadedEquipPartAtRooSet(false);
	}

	FSingleItemInfoLoadedPtrData* MyCachedRefContainer = LoadedPtrMap.Find(MyRefID);

	if (PCPartMesh.IsValid() && (!PCPartMesh->IsRooted() || bUnloadEvenRootSet))
	{
		PCPartMesh->RemoveFromRoot();
		if (MyCachedRefContainer) { // 못 찾으면 진작에 로딩을 하지 않은 거든지, 뭔가 잘못되었든지.
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
{ // UnloadOnlyEquipPart 을 밖에서 따로 불러줄 일이 있어서..
	if (OwnerItemInfoTable)
	{
		UnloadOnlyEquipPart(OwnerItemInfoTable->InfoLoadManager, CachedMyRefID, OwnerItemInfoTable->LoadedPtrMap, bUnloadEvenRootSet);
	}
}

void FSingleItemInfoData::MarkLoadedEquipPartAtRooSet(bool bRootSet)
{
	// 여기서 따로 로딩을 하지는 않는다. 로딩 되어 있는 것만 처리..
	// AddToRoot 를 하면 Unload 도 먹히지 않을 거.
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
	// TryAsyncLoad 에 필요한 정보들 긁어오기. 순서대로 하나씩.
	// 여기 멤버 TAsset 모두를 넣는 건 아니다. 캐릭터 메쉬 조합 및 부가 효과에 필요한 재료들만.
	// 실제 Async 요청을 보낼 때 단일 SingleItemInfoData 뿐 아니라 여러 RefID 에 대해 긁어모은 FB2AsyncRequestInfo 리스트들을 합쳐서 보내게 될 소지가 높다. 캐릭터 하나 조합하는데 여러 RefID 의 아이템 데이터가 필요하기 때문이다.

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
	// 여기서 캐쉬하고자 하는 대상은 세팅이 된 거라면 로딩이 된 상태여야 한다. (실제로는 모두 세팅된 것이 드물 거라 Get 이 null 을 리턴하는 게 이상한 건 아니다.)
	// Async 로딩과 관련해서 쓰는 것인만큼 flush 요소 없이 단순 Get 만..

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
	//	// 본격 ItemInfo 적용 전에 BaseBP 먼저
	//	OverrideByBaseBP(InSingleInfoOfID->GetBaseBPClass(this));

	//	// 여기서 ItemInfo 에 있는 모든 정보를 사용하지는 않을 것이다. 일부는 UI 에 쓰인다.

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

	//	// 어째 좀 중구난방인 거 같지만 여하간 차근차근 세팅.
	//	// 이것이 불린 이후 AB2DropItem::BeginPlay 가 불리지 않는다면 일부 처리를 수동으로 해야 할 것.

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

	//// 이것이 불리는 시점은 ItemInfo 적용이나 BeginPlay 이전임.
	//// 리소스 레퍼런스 세팅의 경우 세팅 후 실제 사용 시점이 언제인지 확인할 필요가 있음.

	//// 하위 컴포넌트들은 따로 처리해야 한다. this 대상의 CopyObjectProperties 에서 하위 컴포넌트들을 복사하게 하면 컴포넌트 자체를 그대로 가져와 사용하므로 안됨.

	//CopyObjectProperties(this->AudioComp, BPCDO->AudioComp, true, true);
	//CopyObjectProperties(this->MainBodyPS, BPCDO->MainBodyPS, true, true);
	//CopyObjectProperties(this->CollisionComp, BPCDO->CollisionComp, true, true);
	//CopyObjectProperties(this->DropItemMoveComp, BPCDO->DropItemMoveComp, true, true);
	//
	//// 자기 자신은 맨 나중에 처리해 본다.
	//CopyObjectProperties(this, BPCDO, true, true);
}

////////////////////////////////////////////////////////////////////////////////
void FItemInfoEquipPartsAsyncReqCombinedID::SetRefIDs(const TArray<int32>& InRefIDs)
{
	// 이걸 사용하려는 환경에서의 대략적인 제한. 매 요소별로 for 문 돌려가며 체크하는 게 있어서 이게 커지면 성능 문제가 발생할 수 있음.
	// 대략 플레이어 캐릭터 파트 개수만큼 필요한데 만에 하나 예상치 못한 경우에 대비하기 위해..
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
	// MySerialNumber 랑 MyAsyncRequestName 은 비교할 필요가 없다. 
	// 처음부터 서로 다르게 식별하려던 건 RefIDs 뿐.. 
	// RefIDs 구성이 다른데 SerialNumber 가 같으면 문제가 되지만, RefIDs 구성이 같고 SerialNumber 가 다르다고 해서 문제될 건 없다.

	return IsEqualByRefIDs(InOther);
}

bool FItemInfoEquipPartsAsyncReqCombinedID::IsEqualByRefIDs(const FItemInfoEquipPartsAsyncReqCombinedID& InOther) const
{
	if (RefIDs.Num() == InOther.RefIDs.Num())
	{
		bool bHasAnyDifference = false;
		for (int32 IA = 0; IA < RefIDs.Num(); ++IA)
		{
			if (!InOther.RefIDs.Contains(RefIDs[IA])) // 두 배열 개수가 같으므로 한쪽의 모든 요소가 다른 쪽에 포함되어 있다면 똑같은 거.
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
//		// 정해진 하나를 로딩
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
//				FString::Printf(TEXT("[Warning] ItemInfo 데이터 테이블을 찾지 못함. 컴퓨터가 곧 폭발한다."))
//				));
//		}
//
//		if (SetInfoData == NULL)
//		{
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[Warning] ItemSetInfo 데이터 테이블을 찾지 못함. 컴퓨터가 곧 폭발한다."))
//				));
//		}
//
//#endif
//	}
}

//FSingleItemInfoData* UB2ItemInfo::GetInfoData(int32 ItemRefID, bool bFullyLoad)
//{
//	// ItemRefID 를 key 로 사용.
//	FSingleItemInfoData* FoundDataRow = TheData ? TheData->FindRow<FSingleItemInfoData>(FName(*FString::FromInt(ItemRefID)), TEXT("")) : NULL;
//	if (FoundDataRow == NULL)
//	{
//#if WITH_EDITOR && !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//			FString::Printf(TEXT("ItemInfo 테이블에서 RefID %d 의 항목을 찾지 못함. 컴퓨터가 곧 폭발한다."), ItemRefID)
//			));
//#endif
//		return NULL;
//	}
//
//	FoundDataRow->CachedMyRefID = ItemRefID; // RefID 캐싱해 놓으면 SingleInfo 의 개별 데이터 로딩 시 사용하게 될 거.
//
//	if (bFullyLoad
//#if WITH_EDITOR
//		|| GIsEditor
//#endif
//		)
//	{
//		// 굳이 LoadAllTAssets 를 하지 않더라도 실제 사용할 때에는 개별 레퍼런스들을 로딩하게 될 것. LoadAllTAssets 으로는 쓸데없는 메모리 소모의 소지가 있다.
//		FoundDataRow->LoadAllTAssets(InfoLoadManager, ItemRefID, LoadedPtrMap);
//	}
//
//	return FoundDataRow;
//}

int32 UB2ItemInfo::GetRandomItemRefID()
{
	return 1000100; //리소스 제작이 되어 있지 않기 때문에 첫 row의 아이템만 내뱉는 것으로 임시 사용
	/*
	uint32 NumItems = TheData ? TheData->RowMap.Num() : 0;

	if (NumItems > 0)
	{
		int32 RandIndex = FMath::FRandRange(0.0f, (float)NumItems - 1.5f); //골드 제외

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
//		GetInfoData(InRefIDList[RI], true); // FullyLoad 를 true 로 주고 부르면 다 로드됨. 이걸 대략 몇가지 범주별로 나눠서 Preload 하는 방식이 필요할 수도.
//	}
//#if !UE_BUILD_SHIPPING
//	if (InRefIDList.Num() > 0){
//		UE_LOG(LogBladeII, Log, TEXT("ItemInfo pre-load for %d assets"), InRefIDList.Num());
//	}
//#endif	
}

void UB2ItemInfo::UnloadAll()
{
	check(0); // 음 잠깐.. 이거 일단 쓰면 안될 것 같음. UnloadAllExceptCurrentLocalEquipPartData 로 대신..

	// DataTable 의 모든 row 를 iterate 하는 방법인듯.
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
		LoadedPtrMap.Empty(); // 확인 사살
	}	

	ResetAsyncLoadReqInfo();
}

void UB2ItemInfo::UnloadAllExceptCurrentLocalEquipPartData()
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2ItemInfo::UnloadAllExceptCurrentLocalEquipPartData"));

//	// 특정 부분에서 현재 로컬 캐릭터들의 장착 장비 파트랑 관련된 일부에 필요한 데이터 이외에 모든 것을 날리기 위함.
//	// 예를 들어 도감이나 인벤토리 나갈 시 사용.
//	// .. 그리고 결국은 일반적인 UnloadAll 상황을 이걸로 대체해야 할 듯.. 로컬 캐릭터 장착 파트들을 남겨두는 게 좀 중요해져서.
//
//#if WITH_EDITOR
//	if (GIsEditor) {
//		return;
//	}
//#endif
//
//	TMap<int32, bool> AllEquippedRefIDs; // Value 는 의미없고 걍 Key 검색만 할 거.
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
//			AllEquippedRefIDs.Add(ThisEquipped.ItemRefID, true); // Key 값만 있음 됨.
//		}
//	}
//	
//	if (TheData)
//	{
//		// RefID 간에 중복해서 사용되는 파트들이 있어서 루트셋 처리에 주의가 필요하다..
//		// 이 정도면 리소스 로딩에 필적하는 상당한 수준인데.. ㅡㅡ
//
//		// 1차 패스에서 모두 RootSet 에서 제거
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
//		// 2차 패스에서 현재 장착인 것만 RootSet 에 추가
//		for (TMap<int32, bool>::TIterator RefIDIt(AllEquippedRefIDs); RefIDIt; ++RefIDIt)
//		{
//			int32 ThisRefID = RefIDIt.Key();
//			FSingleItemInfoData* FoundDataRow = GetInfoData(ThisRefID, false);
//			if (FoundDataRow) {
//				FoundDataRow->MarkLoadedEquipPartAtRooSet(true);
//			}
//		}
//		// 마지막으로 RootSet 인 거 제외한 Unload
//		TArray<FName> RowKeys = TheData->GetRowNames();
//		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
//		{
//			FName& ThisRowKey = RowKeys[RI];
//			FSingleItemInfoData* FoundDataRow = TheData->FindRow<FSingleItemInfoData>(ThisRowKey, TEXT(""));
//			if (FoundDataRow)
//			{ // GetAllRows 로 한거랑 순서가 안 맞으려나...
//				const int32 ThisRowNumber = FCString::Atoi(*ThisRowKey.ToString());
//				FoundDataRow->UnloadAllTAssets(InfoLoadManager, ThisRowNumber, LoadedPtrMap, false);
//			}
//		}
//	}
}

FSingleItemInfoLoadedPtrData* UB2ItemInfo::GetLoadedPtrForID(int32 InItemID)
{
	FSingleItemInfoLoadedPtrData* FoundLoadedPtrEntry = LoadedPtrMap.Find(InItemID);
	if (!FoundLoadedPtrEntry && InItemID > 0) // 없으면 새로 생성.
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
//	// PostSpawnInitialize 나 BeginPlay 를 따로 떼고 그 전에 ItemInfo 를 적용하기 위함.
//	// bDeferPostSpawnInitialize 만 내부적으로 세팅하는 PCClassInfo/NPCClassInfo 와는 달리 bDeferConstruction 도 내부적으로 세팅하는데 FinishSpawning 이전에 특별히 할 것이 없다고 판단되기 때문.
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
//	// PCClassInfo 나 NPCClassInfo 의 경우와 마찬가지로 이게 제대로 먹히려면 SpawnParameters 에서 bDeferConstruction 이 세팅되어 있어야 할 것. 그래야 BeginPlay 가 이 이후 불리게 됨.
//	// 기본 클래스는 정해 놓고 사용. (DropItem_DefaultBaseBPClass)
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
//	// SignificantData 의 기타 나머지는 ItemRefID 를 기반으로 서버에서 가져오는데 개발버전 클라이언트에서는 아래와 같이 테스트 데이터를 넣어줄 수 있음.
////#if !UE_BUILD_SHIPPING 
//	//DevOnlyLoadItemClientTestData(SpawnedActor->SignificantData);
////#endif
//	
//
//	// ItemInfo 적용 후 bDeferConstruction 및 bDeferPostSpawnInitialize 에 따른 처리
//	if (SpawnedActor)
//	{
//		if(!FMath::IsNearlyZero(StayTime))
//		{
//			SpawnedActor->SetQTEStayTime(StayTime);
//		}
//
//		SpawnWorld->ManualPostSpawnInitialize(SpawnedActor, SpawnTransform, FinalSpawnParam);
//
//		// FinishSpawning 에서 BeginPlay 가 불림. 만일 아이템 spawn 상황에 따라 이걸 나중에 처리해야 하는 경우가 생긴다면 상단 bDeferConstruction 세팅하는 부분과 함께 이걸 밖으로 뺄 것.
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

	//// DataTable 의 모든 row 를 iterate 하는 방법인듯.
	//TArray<FName> RowKeys = TheData->GetRowNames();
	//
	//const float TotalProgRange = FMath::Max(0.0f, InEndProgress - InStartProgress);
	//// InfoAsset 에 등록되는 애들이 얼마나 되는지에 따라 적절히 정하는 거. 몇번마다 한번씩 업데이트 할 지.
	//const int32 ProgUpdateInterval = 10;
	//const int32 TotalProgUpdateCount = RowKeys.Num() / ProgUpdateInterval;
	//const float SingleProgIncAmount = TotalProgRange / (float)TotalProgUpdateCount;

	//float CurrProg = InStartProgress;
	//
	//for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
	//{
	//	GetInfoData(FCString::Atoi(*RowKeys[RI].ToString()), true); // 안에서 다시 FName 이 되니 낭비인데 어차피 에디터 기능이니..

	//	// Splash progress 업데이트. 아래 SetInfoData 는 얼마 안될 걸로 치고..
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
	//		GetSetInfoData(FCString::Atoi(*SetRowKeys[RI].ToString())); // 안에서 다시 FName 이 되니 낭비인데 어차피 에디터 기능이니..
	//	}
	//}


	//// 이걸 사용하려면 BladeIIGameImpl::StartupModule 에서 하게 될 텐데 그러면 이것의 인스턴스 위치가 바뀌게 될 것..
}

void UB2ItemInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{ 
	//
	// DataTable 에디트 시에는 이게 안먹히나..
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
			//FoundDataRow->LoadLocalizedName(); 이젠 좃도 쓸모없군.
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
//	// PendingEquipPartsAsyncLoadReqList 에서 완료된 엔트리를 식별하고 리스트에서 제거.
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
//	// 로딩된 에셋들을 캐싱
//	for (int32 LoadedRefID : JustLoadedRefIDs)
//	{
//		// 여기서 FullyLoad 를 주면 캐싱이 알아서 되긴 하지만
//		// 이 Async 로딩은 단일 Info 에 등록된 모든 걸 로딩하려는 게 아니므로 역시나 FullyLoad 없이 그냥.
//		// 게다가 거기서 FullyLoad 하는 건 async flush 요소가 있어서 async 로딩과 연관해서 쓰기 적절치 않다.
//		FSingleItemInfoData* ThisLoadedInfo = GetInfoData(LoadedRefID, false); 
//		if (ThisLoadedInfo)
//		{
//			ThisLoadedInfo->CacheOnEquipPartsAsyncLoadComplete(LoadedRefID, LoadedPtrMap);
//		}
//	}
//
//	// 완료된 Request 이름을 담아서 이벤트 날림.
//	if (OnEquipPartsAyncLoadComplete.IsBound())
//	{
//		OnEquipPartsAyncLoadComplete.Broadcast(CompleteRequest);
//	}
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (!bFoundMatchingCombinedID)
//	{
//		// 이런 일이 있을 수도? 똑같은 RefID 조합으로 연달아 로딩 요청을 보낼 경우 둘 다에 대해 각각 콜백이 온다면 이런 일이 발생할 텐데 실제로 그런 경우에는 한번만 온다.
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
	//{ // 로딩 없이 그냥 InfoData 껍데기만
	//	FSingleItemInfoData* ThisInfoData = GetInfoData(ThisRefID, false);
	//	if (ThisInfoData)
	//	{
	//		ThisInfoData->GetEquipPartsAsyncLoadReqInfo(TotalReqInfo); // 각 InfoData 에서 긁어온 거 누적시킴. 다 모음
	//	}
	//}


	////
	//// GetAsyncReqCombinedIDofPartsList 를 먼저 체크해 보고 이미 pending 이면 return false 해 볼까?
	//// 연달아 들어온 중복 요청 무시 차원..
	////FItemInfoEquipPartsAsyncReqCombinedID AsyncReqCombinedID; 
	////if (GetAsyncReqCombinedIDofPartsList(InRefIDList, AsyncReqCombinedID, false))
	////{
	////	return false;
	////}
	////


	//FItemInfoEquipPartsAsyncReqCombinedID AsyncReqCombinedID; // Async 로딩 요청이랑 완료 이후 식별에 필요한 정보.
	//if (GetAsyncReqCombinedIDofPartsList(InRefIDList, AsyncReqCombinedID, true)) // 여기가 유일하게 기존에 없으면 새로 만들어야 할 곳이 될 듯.
	//{
	//	check(AsyncReqCombinedID.IsValid());
	//	TryAsyncLoad(AsyncReqCombinedID.MyAsyncRequestName, TotalReqInfo, bShowBlockingUI);
	//	OutRequestedName = AsyncReqCombinedID.MyAsyncRequestName; // 받은 쪽에서 필요하면 이걸 가지고 완료 이벤트가 왔을 때 식별 가능.
	//	return true;
	//}
	//
	return false;
}

// 단순 Get 으로서만 쓰려던 게 아닌 Generate 의 의미가 좀 있다. const 성격이어야 하는 건 아님. 
bool UB2ItemInfo::GetAsyncReqCombinedIDofPartsList(const TArray<int32>& InRefIDList,
	FItemInfoEquipPartsAsyncReqCombinedID& OutNewOrGeneratedID, bool bGenerateNewOnMiss)
{
	FItemInfoEquipPartsAsyncReqCombinedID LocalTempIDStruct;
	LocalTempIDStruct.SetRefIDs(InRefIDList);

	int32 FoundIndexFromPending = PendingEquipPartsAsyncLoadReqList.Find(LocalTempIDStruct);
	if (FoundIndexFromPending != INDEX_NONE)
	{ // 현재 async 로딩 중인 거..
		OutNewOrGeneratedID = PendingEquipPartsAsyncLoadReqList[FoundIndexFromPending];
		return true;
	}

	if (bGenerateNewOnMiss)
	{
		// 의미없이 계속 다른 조합으로 Get 하다가 SerialNumber 가 마구 증가하는 일이 없도록.. (그리고 PendingEquipPartsAsyncLoadReqList 도 증가..)
		// 없는 경우 확실히 새로 만들고자 할 때만 bGenerateNewOnMiss 를 지정.

		// RefID 리스트 조합으로 unique 하게 식별 가능한 문자열을 곧이곧대로 만드려면 뭔가 쉽지 않을 테니 일련번호를 부여한다
		LocalTempIDStruct.MySerialNumber = EquipPartsAsyncLoadReqCounter;
		++EquipPartsAsyncLoadReqCounter; // 여기가 증가하는 유일한 지점이 되어야 한다.
		// TryAsyncLoad 에 RequestName 으로 쓸 거
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
		return ConsumablesIconBGMtrl.LoadSynchronous(); // 특별히 레퍼런스를 유지해야 할 목적이 있는 거 아님 그냥 LoadSynchronous 쓰면 된다. 이전에 메모리 관리가 잘 안되어서 RefHolder 라는 것에 캐싱을 했던 것임.
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
				AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedItemIconBGMtrlPerGrade_Simple); // 로딩된 에셋 캐싱. 인덱스 똑같이 맞춰서
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
				AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedItemIconBGMtrlPerGrade); // 로딩된 에셋 캐싱. 인덱스 똑같이 맞춰서
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
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedEquipCategoryIcon); // 로딩된 에셋 캐싱. 인덱스 똑같이 맞춰서
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
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedEquipIconBGMtrlPerGrade); // 로딩된 에셋 캐싱. 인덱스 똑같이 맞춰서
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
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedItemQualityBGMtrl); // 로딩된 에셋 캐싱. 인덱스 똑같이 맞춰서
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
			AddOrUpdateMtrlRefHolderCache(EtherTier, LoadedAsset, AllLoadedRefHolder.LoadedEtherTierBGMtrl); // 로딩된 에셋 캐싱. 인덱스 똑같이 맞춰서
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
			AddOrUpdateMtrlRefHolderCache(EtherTier, LoadedAsset, AllLoadedRefHolder.LoadedEtherTierNameBGMtrl); // 로딩된 에셋 캐싱. 인덱스 똑같이 맞춰서
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
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedStarNumberMtrl); // 로딩된 에셋 캐싱. 인덱스 똑같이 맞춰서
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
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedEtherPosBGMtrl); // 로딩된 에셋 캐싱. 인덱스 똑같이 맞춰서
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
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedCostumeIconBGMtrlPerGrade); // 로딩된 에셋 캐싱. 인덱스 똑같이 맞춰서
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
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedTotemOptionIndexIcon); // 로딩된 에셋 캐싱. 인덱스 똑같이 맞춰서
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
{ // 걍 여기서 쓰이는 단순한 반복 코드
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
	// 의도치 않게 저장된 레퍼런스를 날려준다.
	CleanupOnPreSave();
}
#endif