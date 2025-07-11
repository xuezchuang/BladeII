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
	//TArray operator == �� �� ����� �޸� ���� ���ϱ� ������ �����ѹ� �� ����
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
	//TArray operator == �� �� ����� �޸� ���� ���ϱ� ������ �����ѹ� �� ����
	EquipmentRefIds.Sort();
	bWingComposited = bInWingComposited;
	WingData = InWingData;
}

bool FB2GMPCCompositeMeshHash::IsEqual(const FB2GMPCCompositeMeshHash& InOther) const
{
	if (CharacterClass != InOther.CharacterClass || EquipmentRefIds != InOther.EquipmentRefIds) {
		return false;
	}

	// CompositedWing.bShouldBeVisible ��ſ� ���� ���� ������ Wing �� ���� �ʴ� ��Ȳ���� CompositedWing ������ ��ü�� ����� ������ �ʾ��� ���ɼ��� �����ؼ���.
	if (bWingComposited != InOther.bWingComposited) {
		return false;
	}

	// Wing ������ ��ü�� ���� �񱳴� �� �� �������� ��츸 ����.
	if (bWingComposited && InOther.bWingComposited)
	{
		// Wing �� �������� ��Ȳ�̶�� �� ������ �ʵ�鵵 �Ȱ��� �� �־�� �Ѵ�.
		check(WingData.bShouldBeVisible && InOther.WingData.bShouldBeVisible);

		// ���� �ܰ��� �ٸ��� �� �� �ִ� �ʿ��� �͵鸸..
		if (WingData.OwnerPCClass != InOther.WingData.OwnerPCClass ||
			WingData.EvolutionGrade != InOther.WingData.EvolutionGrade
			//CompositedWing.EnhanceLevel != InOther.CompositedWing.EnhanceLevel // EnhanceLevel �� �ܰ��� ������ ��ġ���� �ʴ´�. Ȥ ���߿� ��� ���� �𸣴� ����� �ʿ�� �ְ���..
			)
		{
			return false;
		}
	}

	return true;
}

bool FB2GMPCCompositeMeshCacher::IsOutdated(EPCClass InPCClass, const TArray<FB2Item>& InCompositedEquipment, bool bInWingComposited, const FB2Wing* InWingData, bool bForSectionMerged) const
{
	// Wing �� ������ �ʴ� ��Ȳ���� WingData ptr �� null �� ���� �� �ִ�. �� ��� ���̰��� �־��ֹǷ� bWingComposited �÷��׸� ���� �δ� ��.
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
	{ // OldMesh �� �� RootSet �� �ƴϾ��� ���� �ִ�. �׷����� RooSet �� �� ������ ���⼭ ������ �� �⺻������ RooSet ���� ����.
		// ���� ���õǴ� �Ϳ� ���� RootSet ó���� �ܺο��� ��.
		OldMesh->RemoveFromRoot();
	}
}

void FB2GMPCCompositeMeshCacher::DiscardMe()
{
	// ���⼭ ��� �ִ� ���� ���� ���ŵ� �� �ֵ��� �ϰ��� �� �� �ҷ���. 
	// ���⼭ SkeletalMesh �� �ı��� ���� �ʴ´�. �̰��� ȣ���� �Ŀ��� ������ �̹� ���������� ���� ���ŵǱ� �������� ����� �� �־�� �Ѵ�.
	
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
{ // �⺻���� ��Ʈ������ RootSet ���θ� �̷� ������ �ٲ����� �ʰ� �ܺο��� RootSet �� ä�� ��.
	// �̰� �׽�Ʈ �� ���
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
 * BladeII �÷��̾� ĳ������ ��� ���� ���¿� ���� �������� ���յ� (SkeletalMeshMerge) �޽��� �����Ͽ�
 * �� ������ �ּ�ȭ�ϱ� ���� �뵵�� ����
 * �� ���� transition ���̿��� ���¸� �����Ͽ� �߰����� �� �ε� �ð� ������ ���ְ��� �Ͽ����Ƿ� �� ������Ʈ�� Rootset �̾�� �Ѵ�.
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

	// InPCClass, InPlayerCharacter �� �� �����Ǹ� class enum �� ��ġ�ϰ�
	check(InPCClass == EPCClass::EPC_End || !InPlayerCharacter || InPCClass == InPlayerCharacter->GetCurrentPlayerClass());

	if (InPCClass != EPCClass::EPC_End && (!InPlayerCharacter || InPlayerCharacter->HasValidGameEntryID()))  // ��Ʈ��ũ ��� ��� Ư�� Ÿ�ֿ̹� GameEntryID �� Valid ���� ���� �� ������ �׷� ������ ���� �ȵ�.
	{
		TMap<int32, FB2GMPCCompositeMeshCacher>& RelevantCache = GetRelevantCache(InPlayerCharacter);
		const int32 MapKey = GetCachedCompositeMeshMapKey(InPCClass, InPlayerCharacter);
		FB2GMPCCompositeMeshCacher& FoundOrCreatedCache = RelevantCache.FindOrAdd(MapKey);

		// ���� Outdated üũ�� �ʼ����� �� �ƴ� �� �ִµ� �ڵ� ��Ȯ���� ����� ���Ǹ� ���ؼ��� üũ�� ����
		if (FoundOrCreatedCache.IsOutdated(InPCClass, InCompositedEquipment, bInWingComposited, InWingData, bForSectionMerged))
		{
			if (ShouldSearchForLocalPCCache(InPlayerCharacter) && InMeshToSet &&
				IsLocalPCMeshRootSetAllowed())
			{
				// �� CompositeMeshCache �� ������ �ٷ�� �ٽ����� ����.. �߰��Ǵ� Mesh �� ���� ĳ���� ���̸� RootSet �� �־ ������ �̵� �߿��� GC ����.
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

	// InPCClass, InPlayerCharacter �� �� �����Ǹ� class enum �� ��ġ�ϰ�
	check(InPCClass == EPCClass::EPC_End || !InPlayerCharacter || InPCClass == InPlayerCharacter->GetCurrentPlayerClass());

	if (InPCClass != EPCClass::EPC_End && (!InPlayerCharacter || InPlayerCharacter->HasValidGameEntryID()))  // ��Ʈ��ũ ��� ��� Ư�� Ÿ�ֿ̹� GameEntryID �� Valid ���� ���� �� ������ �׷� ������ ���� �ȵ�.
	{
		TMap<int32, FB2GMPCCompositeMeshCacher>& RelevantCache = GetRelevantCache(InPlayerCharacter);
		const int32 MapKey = GetCachedCompositeMeshMapKey(InPCClass, InPlayerCharacter);
		FB2GMPCCompositeMeshCacher* FoundCache = RelevantCache.Find(MapKey);
		
		if (FoundCache &&
			// �߰��� Outdated ���θ� üũ. ���� key �������� ���� ��� �ٸ��� ���� ���ΰ� �ٸ��� ���� �����ϵ��� NULL ����.
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
	return NULL; // �̰� �� ������Ȳ�� �� �ƴϴ�. �� ���� �����ϴ��� ������ ������ �� ������ ����.
}

bool UB2CompositeMeshCache::ShouldSearchForLocalPCCache(class ABladeIIPlayer* InPlayerCharacter) const
{
	if (!InPlayerCharacter) 
	{
		// ������ �⺻ ���� �÷��̾� ���� �ɷ� �����ϱ� ����. �κ� ���� ���. �� �ֳ�?
		
		// !!!!!!!!!!
		// �κ񿡼� �����̳� Ÿ ���� ĳ���� ���� �� �ӽ������� �޽� ������ �����ִ� ���� ���� ���ܿ��� CompositeMeshCache �� �� ���� �ؾ� �Ѵ�.
		// �̴� �ſ� �߿��� �������μ� �̰��� �������� ������ �ֽ� �̷� �ý����� �����ؼ� �ε� �ð� ����ȭ�� �ϴ� �ǹ̰� �������� �ȴ�.
		// !!!!!!!!!!

		return true; 
	}

	if (Cast<ABladeIITutorialPlayer>(InPlayerCharacter) || Cast<ABladeIITutorialFallbackPuppetPlayer>(InPlayerCharacter)) {
		// FTutorialCharacterData �� IsLocalCharacterData �� false ��.. ������ �ٲ㵵 �Ǳ� �ϰ�����..
		return true;
	}

	ICharacterDataStore* CDS = InPlayerCharacter->GetCharacterDataStore();
	if (CDS){
		return CDS->IsLocalCharacterData(); // �Ϲ����� �ΰ��� ������ ĳ���͵��� ��κ� ���⼭ �ɷ������� ��.
	}

	return InPlayerCharacter->IsLocalPlayer(); // �̰͸����� �Ǻ��� �� ���°� ������ ĳ���� ���� ��� �츮���̾ �̰� false �� ����..
}

TMap<int32, FB2GMPCCompositeMeshCacher>& UB2CompositeMeshCache::GetRelevantCache(class ABladeIIPlayer* InPlayerCharacter)
{ // InPlayerCharacter �� null �� ��Ȳ�� ������. ��, �װ� Local ĳ���� ��.
	return ShouldSearchForLocalPCCache(InPlayerCharacter) ? LocalPCCache : RemotePCCache;
}
TMap<int32, FB2GMPCCompositeMeshCacher>& UB2CompositeMeshCache::GetRelevantCache(ECompositeMeshCacheType InType)
{
	return (InType == ECompositeMeshCacheType::ECMCT_Local) ? LocalPCCache : RemotePCCache;
}

int32 UB2CompositeMeshCache::GetCachedCompositeMeshMapKey(EPCClass InPCClass, class ABladeIIPlayer* InPlayerCharacter) const
{ // GameEntryID �� �⺻�����δ� PCClass �� ���.. PlayerCharacter �� �ʿ��� ��Ȳ�� ��Ʈ�p ���ӵ� ��忡��
	checkSlow(!InPlayerCharacter || InPlayerCharacter->HasValidGameEntryID());
	
	if (InPlayerCharacter)
	{
		// GameEntryID �� Local ĳ���Ͱ� �ƴ� ��츸 ���.
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
	// �ſ� atomic ������.. ��
	return (InGameEntryID >= 0) ? InGameEntryID : PCClassToInt(InPCClass);
}

bool UB2CompositeMeshCache::IsOneOfCachedCompositeMesh(class USkeletalMesh* InTestMesh) const
{
	// Mesh ���տ� ���� ��� ���� ���� ������ ���Ϸ��� �� �ƴϰ� USkeletalMesh ������Ʈ ��ü üũ����.
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
	// Remote ĳ���͵��� �޽��� �Ϲ������� ���� ��ȯ ���̿� ��� ���� �ʿ䰡 ���µ� Ȥ�ö� �κ񿡼� ��Ī�س��� �޽� ������ �Ŀ� ������ �ε��ؼ� �ٽ� ���ٰų� �ϴ� �ʿ䰡 �������.
	// �׷��Ա����� �ʿ� ���� �� ������.. �ϰ� �Ǹ� �����ؼ�..
	DiscardCaches(ECompositeMeshCacheType::ECMCT_Remote);

	if (!IsLocalPCMeshRootSetAllowed())
	{
		// �̰� ��� �Ϻ�Ǹ� ����� �ʴ� �� ��ȹ��. IsLocalPCMeshRootSetAllowed �� false �� �� �׽�Ʈ�ų�..
		DiscardCaches(ECompositeMeshCacheType::ECMCT_Local);
	}
}

void UB2CompositeMeshCache::DiscardCaches(ECompositeMeshCacheType DiscardCacheType)
{
	// ĳ���� ����� ������ mesh ���� �̺�Ʈ�� ���� �� ������ ���� �����ϰ��� ���� ����.
	// ĳ���� SkeletalMesh �� ���⼭ ���� �ı��ϴ� �� �ƴ�.
	// ���ŵǱ� �������� ������ ���� ��.

	TMap<int32, FB2GMPCCompositeMeshCacher>& DesiredCache = GetRelevantCache(DiscardCacheType);

	for (TMap<int32, FB2GMPCCompositeMeshCacher>::TIterator MeshMapIt(DesiredCache); MeshMapIt; ++MeshMapIt)
	{
		FB2GMPCCompositeMeshCacher& ThisMap = MeshMapIt.Value();
		ThisMap.DiscardMe(); // RootSet ���� ���ŵ� ��.
	}

	DesiredCache.Empty();
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
void UB2CompositeMeshCache::DevResetCompositMeshCache() // �޽� ���� ���� �̸����� �׽�Ʈ�ϴ� ġƮ���� �ʿ�..
{
	DiscardCaches(ECompositeMeshCacheType::ECMCT_Remote);
	DiscardCaches(ECompositeMeshCacheType::ECMCT_Local);
}
// ���� ��İ� �� �׽�Ʈ�� ���� ����
void UB2CompositeMeshCache::SetAllowLocalPCMeshRootSet(bool bSet)
{
	bAllowLocalPCMeshRootSet = bSet;

	if (!IsLocalPCMeshRootSetAllowed())
	{ // ����� ���ٸ� �����ϰ� ĳ���� ����.
		DiscardCaches(ECompositeMeshCacheType::ECMCT_Remote);
		DiscardCaches(ECompositeMeshCacheType::ECMCT_Local);
	}
	else
	{
		// ����� �Ѵ� ��� ���� ĳ���� ĳ���� ��Ʈ�¿� �߰��� ��.
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
