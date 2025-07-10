// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.


#include "B2CompositeMeshCache.h"
#include "BladeIIGameImpl.h"
#include "BladeIIGameMode.h"
#include "B2LobbyGameMode.h"
#include "BladeIIPlayer.h"
#include "CharacterDataStore.h"
#include "B2TutorialGameMode.h"

FB2GMPCCompositeMeshHash::FB2GMPCCompositeMeshHash(EPCClass InPCClass, const TArray<int32>& InEquipmentId, bool bInWingComposited, const FB2Wing& InWingData)
{
	CharacterClass = InPCClass;
	EquipmentRefIds = InEquipmentId;
	//TArray operator == 이 비교 방식이 메모리 고대로 비교하기 때문에 정렬한번 만 해줌
	EquipmentRefIds.Sort();
	bWingComposited = bInWingComposited;
	WingData = InWingData;
}

FB2GMPCCompositeMeshHash::FB2GMPCCompositeMeshHash(EPCClass InPCClass, const TArray<FB2Item>& InEquipment, bool bInWingComposited, const FB2Wing& InWingData)
{
	CharacterClass = InPCClass;
	for (const FB2Item& ThisItemData : InEquipment)
	{
		EquipmentRefIds.Add(ThisItemData.ItemRefID);
	}
	//TArray operator == 이 비교 방식이 메모리 고대로 비교하기 때문에 정렬한번 만 해줌
	EquipmentRefIds.Sort();
	bWingComposited = bInWingComposited;
	WingData = InWingData;
}

bool FB2GMPCCompositeMeshHash::IsEqual(const FB2GMPCCompositeMeshHash& InOther) const
{
	if (CharacterClass != InOther.CharacterClass || EquipmentRefIds != InOther.EquipmentRefIds) {
		return false;
	}

	// CompositedWing.bShouldBeVisible 대신에 따로 쓰는 이유는 Wing 을 쓰지 않는 상황에서 CompositedWing 데이터 자체가 제대로 들어오지 않았을 가능성을 생각해서임.
	if (bWingComposited != InOther.bWingComposited) {
		return false;
	}

	// Wing 데이터 자체에 대한 비교는 둘 다 보여지는 경우만 수행.
	if (bWingComposited && InOther.bWingComposited)
	{
		// Wing 이 보여지는 상황이라면 이 데이터 필드들도 똑같이 들어가 있어야 한다.
		check(WingData.bShouldBeVisible && InOther.WingData.bShouldBeVisible);

		// 실제 외관을 다르게 할 수 있는 필요한 것들만..
		if (WingData.OwnerPCClass != InOther.WingData.OwnerPCClass ||
			WingData.EvolutionGrade != InOther.WingData.EvolutionGrade
			//CompositedWing.EnhanceLevel != InOther.CompositedWing.EnhanceLevel // EnhanceLevel 도 외관에 영향을 미치지는 않는다. 혹 나중에 어떻게 될지 모르니 적어둘 필요는 있겠지..
			)
		{
			return false;
		}
	}

	return true;
}

bool FB2GMPCCompositeMeshCacher::IsOutdated(EPCClass InPCClass, const TArray<FB2Item>& InCompositedEquipment, bool bInWingComposited, const FB2Wing* InWingData, bool bForSectionMerged) const
{
	// Wing 을 보이지 않는 상황에서 WingData ptr 이 null 로 들어올 수 있다. 이 경우 더미값을 넣어주므로 bWingComposited 플래그를 따로 두는 것.
	return IsOutdated(InPCClass, InCompositedEquipment, bInWingComposited, (InWingData ? *InWingData : FB2Wing()), bForSectionMerged);
}
bool FB2GMPCCompositeMeshCacher::IsOutdated(EPCClass InPCClass, const TArray<FB2Item>& InCompositedEquipment, bool bInWingComposited, const FB2Wing& InWingData, bool bForSectionMerged) const
{
	FB2GMPCCompositeMeshHash NewHash(InPCClass, InCompositedEquipment, bInWingComposited, InWingData);
	return bForSectionMerged ? (this->CompositeMeshHash_SectionMerged != NewHash) : (this->CompositeMeshHash != NewHash);
}
void FB2GMPCCompositeMeshCacher::Set(class USkeletalMesh* InNewMesh, EPCClass InPCClass, const TArray<FB2Item>& InCompositedEquipment, bool bInWingComposited, const FB2Wing* InWingData, bool bForSectionMerged)
{
	Set(InNewMesh, InPCClass, InCompositedEquipment, bInWingComposited, (InWingData ? *InWingData : FB2Wing()), bForSectionMerged);
}
void FB2GMPCCompositeMeshCacher::Set(class USkeletalMesh* InNewMesh, EPCClass InPCClass, const TArray<FB2Item>& InCompositedEquipment, bool bInWingComposited, const FB2Wing& InWingData, bool bForSectionMerged)
{
	USkeletalMesh* OldMesh = nullptr;

	if (bForSectionMerged) {
		CompositeMeshHash_SectionMerged = FB2GMPCCompositeMeshHash(InPCClass, InCompositedEquipment, bInWingComposited, InWingData);
		OldMesh = CreatedCompositeMesh_SectionMerged;
		CreatedCompositeMesh_SectionMerged = InNewMesh;
	}
	else {
		CompositeMeshHash = FB2GMPCCompositeMeshHash(InPCClass, InCompositedEquipment, bInWingComposited, InWingData);
		OldMesh = CreatedCompositeMesh;
		CreatedCompositeMesh = InNewMesh;
	}

	if (OldMesh && OldMesh != InNewMesh)
	{ // OldMesh 가 꼭 RootSet 이 아니었을 수도 있다. 그렇지만 RooSet 일 수 있으니 여기서 나가는 건 기본적으로 RooSet 에서 제거.
		// 새로 세팅되는 것에 대한 RootSet 처리는 외부에서 함.
		OldMesh->RemoveFromRoot();
	}
}

void FB2GMPCCompositeMeshCacher::DiscardMe()
{
	// 여기서 들고 있는 것이 새로 갱신될 수 있도록 하고자 할 때 불러줌. 
	// 여기서 SkeletalMesh 의 파괴는 하지 않는다. 이것을 호출한 후에도 여전히 이번 레벨에서는 실제 갱신되기 전까지는 사용할 수 있어야 한다.
	
	if (CreatedCompositeMesh) {
		CreatedCompositeMesh->RemoveFromRoot();
		CreatedCompositeMesh = nullptr;
	}
	if (CreatedCompositeMesh_SectionMerged) {
		CreatedCompositeMesh_SectionMerged->RemoveFromRoot();
		CreatedCompositeMesh_SectionMerged = nullptr;
	}

	CompositeMeshHash = FB2GMPCCompositeMeshHash();
	CompositeMeshHash_SectionMerged = FB2GMPCCompositeMeshHash();
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
void FB2GMPCCompositeMeshCacher::DevSetMeshesRooSet()
{ // 기본적인 루트에서는 RootSet 여부를 이런 식으로 바꿔주지 않고 외부에서 RootSet 인 채로 옴.
	// 이건 테스트 용 기능
	if (CreatedCompositeMesh) {
		CreatedCompositeMesh->AddToRoot();
	}
	if (CreatedCompositeMesh_SectionMerged) {
		CreatedCompositeMesh_SectionMerged->AddToRoot();
	}
}
#endif

/**
 * ============================================================================================
 * UB2CompositeMeshCache
 * BladeII 플레이어 캐릭터의 장비 장착 상태에 따라 동적으로 조합된 (SkeletalMeshMerge) 메쉬를 관리하여
 * 그 생성을 최소화하기 위한 용도로 구현
 * 매 레벨 transition 사이에도 상태를 유지하여 추가적인 맵 로딩 시간 증가를 없애고자 하였으므로 이 오브젝트는 Rootset 이어야 한다.
 * ============================================================================================
 */

UB2CompositeMeshCache::UB2CompositeMeshCache(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	bAllowLocalPCMeshRootSet = true;
#endif
}

void UB2CompositeMeshCache::SetCachedCompositeMesh(EPCClass InPCClass, class ABladeIIPlayer* InPlayerCharacter, class USkeletalMesh* InMeshToSet,
	const TArray<FB2Item>& InCompositedEquipment, bool bInWingComposited, const FB2Wing* InWingData,
	bool bForSectionMerged)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2CompositeMeshCache::SetCachedCompositeMesh"));

	// InPCClass, InPlayerCharacter 둘 다 제공되면 class enum 은 일치하게
	check(InPCClass == EPCClass::EPC_End || !InPlayerCharacter || InPCClass == InPlayerCharacter->GetCurrentPlayerClass());

	if (InPCClass != EPCClass::EPC_End && (!InPlayerCharacter || InPlayerCharacter->HasValidGameEntryID()))  // 네트워크 모드 등에서 특정 타이밍에 GameEntryID 가 Valid 하지 않을 수 있으며 그럴 때에는 쓰면 안됨.
	{
		TMap<int32, FB2GMPCCompositeMeshCacher>& RelevantCache = GetRelevantCache(InPlayerCharacter);
		const int32 MapKey = GetCachedCompositeMeshMapKey(InPCClass, InPlayerCharacter);
		FB2GMPCCompositeMeshCacher& FoundOrCreatedCache = RelevantCache.FindOrAdd(MapKey);

		// 여긴 Outdated 체크가 필수적인 건 아닐 수 있는데 코드 명확성과 디버깅 편의를 위해서라도 체크해 놓음
		if (FoundOrCreatedCache.IsOutdated(InPCClass, InCompositedEquipment, bInWingComposited, InWingData, bForSectionMerged))
		{
			if (ShouldSearchForLocalPCCache(InPlayerCharacter) && InMeshToSet &&
				IsLocalPCMeshRootSetAllowed())
			{
				// 이 CompositeMeshCache 를 별도로 다루는 핵심적인 이유.. 추가되는 Mesh 가 로컬 캐릭터 용이면 RootSet 에 넣어서 레벨간 이동 중에도 GC 방지.
				InMeshToSet->AddToRoot();
			}

			FoundOrCreatedCache.Set(InMeshToSet, InPCClass, InCompositedEquipment, bInWingComposited, InWingData, bForSectionMerged);
		}
	}
}

USkeletalMesh* UB2CompositeMeshCache::GetCachedCompositeMesh(EPCClass InPCClass, class ABladeIIPlayer* InPlayerCharacter,
	const TArray<FB2Item>& InCompositedEquipment, bool bInWingComposited, const FB2Wing* InWingData,
	bool bForSectionMerged)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2CompositeMeshCache::GetCachedCompositeMesh"));

	// InPCClass, InPlayerCharacter 둘 다 제공되면 class enum 은 일치하게
	check(InPCClass == EPCClass::EPC_End || !InPlayerCharacter || InPCClass == InPlayerCharacter->GetCurrentPlayerClass());

	if (InPCClass != EPCClass::EPC_End && (!InPlayerCharacter || InPlayerCharacter->HasValidGameEntryID()))  // 네트워크 모드 등에서 특정 타이밍에 GameEntryID 가 Valid 하지 않을 수 있으며 그럴 때에는 쓰면 안됨.
	{
		TMap<int32, FB2GMPCCompositeMeshCacher>& RelevantCache = GetRelevantCache(InPlayerCharacter);
		const int32 MapKey = GetCachedCompositeMeshMapKey(InPCClass, InPlayerCharacter);
		FB2GMPCCompositeMeshCacher* FoundCache = RelevantCache.Find(MapKey);
		
		if (FoundCache &&
			// 추가로 Outdated 여부를 체크. 같은 key 값에서라도 장착 장비가 다르고 날개 여부가 다르면 새로 생성하도록 NULL 리턴.
			!FoundCache->IsOutdated(InPCClass, InCompositedEquipment, bInWingComposited, InWingData, bForSectionMerged)
			)
		{
#if !UE_BUILD_SHIPPING
			UE_LOG(LogBladeII, Log, TEXT("Use cached mesh for PCClass %d and OwnerActor %s"), PCClassToInt(InPCClass), InPlayerCharacter ? *InPlayerCharacter->GetName() : TEXT("Unknown"));
#endif
			return FoundCache->Get(bForSectionMerged);
		}
	}
#if !UE_BUILD_SHIPPING
	UE_LOG(LogBladeII, Log, TEXT("Cannot find cached mesh for PCClass %d and OwnerActor %s. A new mesh will be generated.."), PCClassToInt(InPCClass), InPlayerCharacter ? *InPlayerCharacter->GetName() : TEXT("Unknown"));
#endif
	return NULL; // 이게 꼭 문제상황인 건 아니다. 걍 새로 생성하느냐 이전에 생성한 거 쓰느냐 차이.
}

bool UB2CompositeMeshCache::ShouldSearchForLocalPCCache(class ABladeIIPlayer* InPlayerCharacter) const
{
	if (!InPlayerCharacter) 
	{
		// 없으면 기본 로컬 플레이어 용인 걸로 간주하기 위함. 로비 같은 경우. 또 있나?
		
		// !!!!!!!!!!
		// 로비에서 도감이나 타 계정 캐릭터 보기 등 임시적으로 메쉬 조합을 보여주는 경우는 보다 윗단에서 CompositeMeshCache 를 안 쓰게 해야 한다.
		// 이는 매우 중요한 사항으로서 이것이 지켜지지 않으면 애써 이런 시스템을 도입해서 로딩 시간 최적화를 하는 의미가 없어지게 된다.
		// !!!!!!!!!!

		return true; 
	}

	if (Cast<ABladeIITutorialPlayer>(InPlayerCharacter) || Cast<ABladeIITutorialFallbackPuppetPlayer>(InPlayerCharacter)) {
		// FTutorialCharacterData 의 IsLocalCharacterData 가 false 라.. 그쪽을 바꿔도 되긴 하겠지만..
		return true;
	}

	ICharacterDataStore* CDS = InPlayerCharacter->GetCharacterDataStore();
	if (CDS){
		return CDS->IsLocalCharacterData(); // 일반적인 인게임 전투용 캐릭터들은 대부분 여기서 걸러져야할 듯.
	}

	return InPlayerCharacter->IsLocalPlayer(); // 이것만으로 판별할 수 없는게 팀대전 캐릭터 같은 경우 우리편이어도 이게 false 일 꺼라..
}

TMap<int32, FB2GMPCCompositeMeshCacher>& UB2CompositeMeshCache::GetRelevantCache(class ABladeIIPlayer* InPlayerCharacter)
{ // InPlayerCharacter 가 null 인 상황도 간주함. 단, 그건 Local 캐릭터 용.
	return ShouldSearchForLocalPCCache(InPlayerCharacter) ? LocalPCCache : RemotePCCache;
}
TMap<int32, FB2GMPCCompositeMeshCacher>& UB2CompositeMeshCache::GetRelevantCache(ECompositeMeshCacheType InType)
{
	return (InType == ECompositeMeshCacheType::ECMCT_Local) ? LocalPCCache : RemotePCCache;
}

int32 UB2CompositeMeshCache::GetCachedCompositeMeshMapKey(EPCClass InPCClass, class ABladeIIPlayer* InPlayerCharacter) const
{ // GameEntryID 도 기본적으로는 PCClass 를 사용.. PlayerCharacter 가 필요한 상황은 네트웤 게임등 모드에서
	checkSlow(!InPlayerCharacter || InPlayerCharacter->HasValidGameEntryID());
	
	if (InPlayerCharacter)
	{
		// GameEntryID 는 Local 캐릭터가 아닌 경우만 사용.
		return GetCachedCompositeMeshMapKey(InPCClass, 
			ShouldSearchForLocalPCCache(InPlayerCharacter) ? -1 : InPlayerCharacter->GetGameEntryID());
	}
	else
	{
		return GetCachedCompositeMeshMapKey(InPCClass, -1);
	}
}

int32 UB2CompositeMeshCache::GetCachedCompositeMeshMapKey(EPCClass InPCClass, int32 InGameEntryID /*= -1*/) const
{
	// 매우 atomic 하지만.. ㅋ
	return (InGameEntryID >= 0) ? InGameEntryID : PCClassToInt(InPCClass);
}

bool UB2CompositeMeshCache::IsOneOfCachedCompositeMesh(class USkeletalMesh* InTestMesh) const
{
	// Mesh 조합에 사용된 장비나 날개 등을 가지고 비교하려는 게 아니고 USkeletalMesh 오브젝트 자체 체크용임.
	if (InTestMesh)
	{
		for (TMap<int32, FB2GMPCCompositeMeshCacher>::TConstIterator MeshMapIt(LocalPCCache); MeshMapIt; ++MeshMapIt)
		{
			const FB2GMPCCompositeMeshCacher& ThisMap = MeshMapIt.Value();
			if (InTestMesh == ThisMap.Get(true) || InTestMesh == ThisMap.Get(false)) {
				return true;
			}
		}
		for (TMap<int32, FB2GMPCCompositeMeshCacher>::TConstIterator MeshMapIt(RemotePCCache); MeshMapIt; ++MeshMapIt)
		{
			const FB2GMPCCompositeMeshCacher& ThisMap = MeshMapIt.Value();
			if (InTestMesh == ThisMap.Get(true) || InTestMesh == ThisMap.Get(false)) {
				return true;
			}
		}
	}
	return false;
}

void UB2CompositeMeshCache::OnPreLoadMap(ABladeIIGameMode* InGameModeAboutToShutdown)
{
	// Remote 캐릭터들의 메쉬는 일반적으로 레벨 전환 사이에 들고 있을 필요가 없는데 혹시라도 로비에서 매칭해놓고 메쉬 빌드한 후에 전투맵 로딩해서 다시 쓴다거나 하는 필요가 생길수도.
	// 그렇게까지는 필요 없을 것 같지만.. 하게 되면 조심해서..
	DiscardCaches(ECompositeMeshCacheType::ECMCT_Remote);

	if (!IsLocalPCMeshRootSetAllowed())
	{
		// 이건 기능 완비되면 비우지 않는 게 계획임. IsLocalPCMeshRootSetAllowed 가 false 인 건 테스트거나..
		DiscardCaches(ECompositeMeshCacheType::ECMCT_Local);
	}
}

void UB2CompositeMeshCache::DiscardCaches(ECompositeMeshCacheType DiscardCacheType)
{
	// 캐쉬를 비워서 다음에 mesh 생성 이벤트가 있을 때 실제로 새로 생성하고자 함이 목적.
	// 캐쉬한 SkeletalMesh 를 여기서 직접 파괴하는 건 아님.
	// 갱신되기 전까지는 여전히 사용될 것.

	TMap<int32, FB2GMPCCompositeMeshCacher>& DesiredCache = GetRelevantCache(DiscardCacheType);

	for (TMap<int32, FB2GMPCCompositeMeshCacher>::TIterator MeshMapIt(DesiredCache); MeshMapIt; ++MeshMapIt)
	{
		FB2GMPCCompositeMeshCacher& ThisMap = MeshMapIt.Value();
		ThisMap.DiscardMe(); // RootSet 에서 제거될 것.
	}

	DesiredCache.Empty();
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
void UB2CompositeMeshCache::DevResetCompositMeshCache() // 메쉬 조합 상태 이리저리 테스트하는 치트에서 필요..
{
	DiscardCaches(ECompositeMeshCacheType::ECMCT_Remote);
	DiscardCaches(ECompositeMeshCacheType::ECMCT_Local);
}
// 이전 방식과 비교 테스트를 위해 제공
void UB2CompositeMeshCache::SetAllowLocalPCMeshRootSet(bool bSet)
{
	bAllowLocalPCMeshRootSet = bSet;

	if (!IsLocalPCMeshRootSetAllowed())
	{ // 기능을 껐다면 심플하게 캐쉬를 리셋.
		DiscardCaches(ECompositeMeshCacheType::ECMCT_Remote);
		DiscardCaches(ECompositeMeshCacheType::ECMCT_Local);
	}
	else
	{
		// 기능을 켜는 경우 로컬 캐릭터 캐쉬만 루트셋에 추가해 줌.
		for (TMap<int32, FB2GMPCCompositeMeshCacher>::TIterator MeshMapIt(LocalPCCache); MeshMapIt; ++MeshMapIt)
		{
			FB2GMPCCompositeMeshCacher& ThisMap = MeshMapIt.Value();
			ThisMap.DevSetMeshesRooSet();
		}
	}
}
#endif
bool UB2CompositeMeshCache::IsLocalPCMeshRootSetAllowed() const
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	return bAllowLocalPCMeshRootSet;
#else
	return true;
#endif
}

UB2CompositeMeshCache* StaticFindCompositeMeshCacher()
{
	BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	BII_CHECK(B2GameModule && B2GameModule->GetCompositeMeshCacher());
	return B2GameModule ? B2GameModule->GetCompositeMeshCacher() : NULL;
}
