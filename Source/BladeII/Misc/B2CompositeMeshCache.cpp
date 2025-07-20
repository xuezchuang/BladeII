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
	//TArray operator == 捞 厚背 规侥捞 皋葛府 绊措肺 厚背窍扁 锭巩俊 沥纺茄锅 父 秦淋
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
	//TArray operator == 捞 厚背 规侥捞 皋葛府 绊措肺 厚背窍扁 锭巩俊 沥纺茄锅 父 秦淋
	EquipmentRefIds.Sort();
	bWingComposited = bInWingComposited;
	WingData = InWingData;
}

bool FB2GMPCCompositeMeshHash::IsEqual(const FB2GMPCCompositeMeshHash& InOther) const
{
	if (CharacterClass != InOther.CharacterClass || EquipmentRefIds != InOther.EquipmentRefIds) {
		return false;
	}

	// CompositedWing.bShouldBeVisible 措脚俊 蝶肺 静绰 捞蜡绰 Wing 阑 静瘤 臼绰 惑炔俊辑 CompositedWing 单捞磐 磊眉啊 力措肺 甸绢坷瘤 臼疽阑 啊瓷己阑 积阿秦辑烙.
	if (bWingComposited != InOther.bWingComposited) {
		return false;
	}

	// Wing 单捞磐 磊眉俊 措茄 厚背绰 笛 促 焊咯瘤绰 版快父 荐青.
	if (bWingComposited && InOther.bWingComposited)
	{
		// Wing 捞 焊咯瘤绰 惑炔捞扼搁 捞 单捞磐 鞘靛甸档 度鞍捞 甸绢啊 乐绢具 茄促.
		check(WingData.bShouldBeVisible && InOther.WingData.bShouldBeVisible);

		// 角力 寇包阑 促福霸 且 荐 乐绰 鞘夸茄 巴甸父..
		if (WingData.OwnerPCClass != InOther.WingData.OwnerPCClass ||
			WingData.EvolutionGrade != InOther.WingData.EvolutionGrade
			//CompositedWing.EnhanceLevel != InOther.CompositedWing.EnhanceLevel // EnhanceLevel 档 寇包俊 康氢阑 固摹瘤绰 臼绰促. 趣 唱吝俊 绢痘霸 瞪瘤 葛福聪 利绢笛 鞘夸绰 乐摆瘤..
			)
		{
			return false;
		}
	}

	return true;
}

bool FB2GMPCCompositeMeshCacher::IsOutdated(EPCClass InPCClass, const TArray<FB2Item>& InCompositedEquipment, bool bInWingComposited, const FB2Wing* InWingData, bool bForSectionMerged) const
{
	// Wing 阑 焊捞瘤 臼绰 惑炔俊辑 WingData ptr 捞 null 肺 甸绢棵 荐 乐促. 捞 版快 歹固蔼阑 持绢林骨肺 bWingComposited 敲贰弊甫 蝶肺 滴绰 巴.
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
	{ // OldMesh 啊 怖 RootSet 捞 酒聪菌阑 荐档 乐促. 弊犯瘤父 RooSet 老 荐 乐栏聪 咯扁辑 唱啊绰 扒 扁夯利栏肺 RooSet 俊辑 力芭.
		// 货肺 技泼登绰 巴俊 措茄 RootSet 贸府绰 寇何俊辑 窃.
		OldMesh->RemoveFromRoot();
	}
}

void FB2GMPCCompositeMeshCacher::DiscardMe()
{
	// 咯扁辑 甸绊 乐绰 巴捞 货肺 盎脚瞪 荐 乐档废 窍绊磊 且 锭 阂矾淋. 
	// 咯扁辑 SkeletalMesh 狼 颇鲍绰 窍瘤 臼绰促. 捞巴阑 龋免茄 饶俊档 咯傈洒 捞锅 饭骇俊辑绰 角力 盎脚登扁 傈鳖瘤绰 荤侩且 荐 乐绢具 茄促.
	
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
{ // 扁夯利牢 风飘俊辑绰 RootSet 咯何甫 捞繁 侥栏肺 官层林瘤 臼绊 寇何俊辑 RootSet 牢 盲肺 咳.
	// 捞扒 抛胶飘 侩 扁瓷
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
 * BladeII 敲饭捞绢 某腐磐狼 厘厚 厘馒 惑怕俊 蝶扼 悼利栏肺 炼钦等 (SkeletalMeshMerge) 皋浆甫 包府窍咯
 * 弊 积己阑 弥家拳窍扁 困茄 侩档肺 备泅
 * 概 饭骇 transition 荤捞俊档 惑怕甫 蜡瘤窍咯 眠啊利牢 甘 肺爹 矫埃 刘啊甫 绝局绊磊 窍看栏骨肺 捞 坷宏璃飘绰 Rootset 捞绢具 茄促.
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

	// InPCClass, InPlayerCharacter 笛 促 力傍登搁 class enum 篮 老摹窍霸
	check(InPCClass == EPCClass::EPC_End || !InPlayerCharacter || InPCClass == InPlayerCharacter->GetCurrentPlayerClass());

	if (InPCClass != EPCClass::EPC_End && (!InPlayerCharacter || InPlayerCharacter->HasValidGameEntryID()))  // 匙飘况农 葛靛 殿俊辑 漂沥 鸥捞怪俊 GameEntryID 啊 Valid 窍瘤 臼阑 荐 乐栏哥 弊凡 锭俊绰 静搁 救凳.
	{
		TMap<int32, FB2GMPCCompositeMeshCacher>& RelevantCache = GetRelevantCache(InPlayerCharacter);
		const int32 MapKey = GetCachedCompositeMeshMapKey(InPCClass, InPlayerCharacter);
		FB2GMPCCompositeMeshCacher& FoundOrCreatedCache = RelevantCache.FindOrAdd(MapKey);

		// 咯变 Outdated 眉农啊 鞘荐利牢 扒 酒匆 荐 乐绰单 内靛 疙犬己苞 叼滚彪 祈狼甫 困秦辑扼档 眉农秦 初澜
		if (FoundOrCreatedCache.IsOutdated(InPCClass, InCompositedEquipment, bInWingComposited, InWingData, bForSectionMerged))
		{
			if (ShouldSearchForLocalPCCache(InPlayerCharacter) && InMeshToSet &&
				IsLocalPCMeshRootSetAllowed())
			{
				// 捞 CompositeMeshCache 甫 喊档肺 促风绰 琴缴利牢 捞蜡.. 眠啊登绰 Mesh 啊 肺拿 某腐磐 侩捞搁 RootSet 俊 持绢辑 饭骇埃 捞悼 吝俊档 GC 规瘤.
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

	// InPCClass, InPlayerCharacter 笛 促 力傍登搁 class enum 篮 老摹窍霸
	check(InPCClass == EPCClass::EPC_End || !InPlayerCharacter || InPCClass == InPlayerCharacter->GetCurrentPlayerClass());

	if (InPCClass != EPCClass::EPC_End && (!InPlayerCharacter || InPlayerCharacter->HasValidGameEntryID()))  // 匙飘况农 葛靛 殿俊辑 漂沥 鸥捞怪俊 GameEntryID 啊 Valid 窍瘤 臼阑 荐 乐栏哥 弊凡 锭俊绰 静搁 救凳.
	{
		TMap<int32, FB2GMPCCompositeMeshCacher>& RelevantCache = GetRelevantCache(InPlayerCharacter);
		const int32 MapKey = GetCachedCompositeMeshMapKey(InPCClass, InPlayerCharacter);
		FB2GMPCCompositeMeshCacher* FoundCache = RelevantCache.Find(MapKey);
		
		if (FoundCache &&
			// 眠啊肺 Outdated 咯何甫 眉农. 鞍篮 key 蔼俊辑扼档 厘馒 厘厚啊 促福绊 朝俺 咯何啊 促福搁 货肺 积己窍档废 NULL 府畔.
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
	return NULL; // 捞霸 怖 巩力惑炔牢 扒 酒聪促. 傲 货肺 积己窍蠢衬 捞傈俊 积己茄 芭 静蠢衬 瞒捞.
}

bool UB2CompositeMeshCache::ShouldSearchForLocalPCCache(class ABladeIIPlayer* InPlayerCharacter) const
{
	//if (!InPlayerCharacter)
	//{
	//	// 绝栏搁 扁夯 肺拿 敲饭捞绢 侩牢 吧肺 埃林窍扁 困窃. 肺厚 鞍篮 版快. 肚 乐唱?

	//	// !!!!!!!!!!
	//	// 肺厚俊辑 档皑捞唱 鸥 拌沥 某腐磐 焊扁 殿 烙矫利栏肺 皋浆 炼钦阑 焊咯林绰 版快绰 焊促 拉窜俊辑 CompositeMeshCache 甫 救 静霸 秦具 茄促.
	//	// 捞绰 概快 吝夸茄 荤亲栏肺辑 捞巴捞 瘤难瘤瘤 臼栏搁 局结 捞繁 矫胶袍阑 档涝秦辑 肺爹 矫埃 弥利拳甫 窍绰 狼固啊 绝绢瘤霸 等促.
	//	// !!!!!!!!!!

	//	return true;
	//}

	//if (Cast<ABladeIITutorialPlayer>(InPlayerCharacter) || Cast<ABladeIITutorialFallbackPuppetPlayer>(InPlayerCharacter))
	//{
	//	// FTutorialCharacterData 狼 IsLocalCharacterData 啊 false 扼.. 弊率阑 官层档 登变 窍摆瘤父..
	//	return true;
	//}

	//ICharacterDataStore* CDS = InPlayerCharacter->GetCharacterDataStore();
	//if (CDS)
	//{
	//	return CDS->IsLocalCharacterData(); // 老馆利牢 牢霸烙 傈捧侩 某腐磐甸篮 措何盒 咯扁辑 吧矾廉具且 淀.
	//}

	return InPlayerCharacter->IsLocalPlayer(); // 捞巴父栏肺 魄喊且 荐 绝绰霸 评措傈 某腐磐 鞍篮 版快 快府祈捞绢档 捞霸 false 老 波扼..
}

TMap<int32, FB2GMPCCompositeMeshCacher>& UB2CompositeMeshCache::GetRelevantCache(class ABladeIIPlayer* InPlayerCharacter)
{ // InPlayerCharacter 啊 null 牢 惑炔档 埃林窃. 窜, 弊扒 Local 某腐磐 侩.
	return ShouldSearchForLocalPCCache(InPlayerCharacter) ? LocalPCCache : RemotePCCache;
}
TMap<int32, FB2GMPCCompositeMeshCacher>& UB2CompositeMeshCache::GetRelevantCache(ECompositeMeshCacheType InType)
{
	return (InType == ECompositeMeshCacheType::ECMCT_Local) ? LocalPCCache : RemotePCCache;
}

int32 UB2CompositeMeshCache::GetCachedCompositeMeshMapKey(EPCClass InPCClass, class ABladeIIPlayer* InPlayerCharacter) const
{ // GameEntryID 档 扁夯利栏肺绰 PCClass 甫 荤侩.. PlayerCharacter 啊 鞘夸茄 惑炔篮 匙飘焢 霸烙殿 葛靛俊辑
	checkSlow(!InPlayerCharacter || InPlayerCharacter->HasValidGameEntryID());
	
	if (InPlayerCharacter)
	{
		// GameEntryID 绰 Local 某腐磐啊 酒囱 版快父 荤侩.
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
	// 概快 atomic 窍瘤父.. せ
	return (InGameEntryID >= 0) ? InGameEntryID : PCClassToInt(InPCClass);
}

bool UB2CompositeMeshCache::IsOneOfCachedCompositeMesh(class USkeletalMesh* InTestMesh) const
{
	// Mesh 炼钦俊 荤侩等 厘厚唱 朝俺 殿阑 啊瘤绊 厚背窍妨绰 霸 酒聪绊 USkeletalMesh 坷宏璃飘 磊眉 眉农侩烙.
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
	// Remote 某腐磐甸狼 皋浆绰 老馆利栏肺 饭骇 傈券 荤捞俊 甸绊 乐阑 鞘夸啊 绝绰单 趣矫扼档 肺厚俊辑 概莫秦初绊 皋浆 呼靛茄 饶俊 傈捧甘 肺爹秦辑 促矫 敬促芭唱 窍绰 鞘夸啊 积辨荐档.
	// 弊犯霸鳖瘤绰 鞘夸 绝阑 巴 鞍瘤父.. 窍霸 登搁 炼缴秦辑..
	DiscardCaches(ECompositeMeshCacheType::ECMCT_Remote);

	if (!IsLocalPCMeshRootSetAllowed())
	{
		// 捞扒 扁瓷 肯厚登搁 厚快瘤 臼绰 霸 拌裙烙. IsLocalPCMeshRootSetAllowed 啊 false 牢 扒 抛胶飘芭唱..
		DiscardCaches(ECompositeMeshCacheType::ECMCT_Local);
	}
}

void UB2CompositeMeshCache::DiscardCaches(ECompositeMeshCacheType DiscardCacheType)
{
	// 某浆甫 厚况辑 促澜俊 mesh 积己 捞亥飘啊 乐阑 锭 角力肺 货肺 积己窍绊磊 窃捞 格利.
	// 某浆茄 SkeletalMesh 甫 咯扁辑 流立 颇鲍窍绰 扒 酒丛.
	// 盎脚登扁 傈鳖瘤绰 咯傈洒 荤侩瞪 巴.

	TMap<int32, FB2GMPCCompositeMeshCacher>& DesiredCache = GetRelevantCache(DiscardCacheType);

	for (TMap<int32, FB2GMPCCompositeMeshCacher>::TIterator MeshMapIt(DesiredCache); MeshMapIt; ++MeshMapIt)
	{
		FB2GMPCCompositeMeshCacher& ThisMap = MeshMapIt.Value();
		ThisMap.DiscardMe(); // RootSet 俊辑 力芭瞪 巴.
	}

	DesiredCache.Empty();
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
void UB2CompositeMeshCache::DevResetCompositMeshCache() // 皋浆 炼钦 惑怕 捞府历府 抛胶飘窍绰 摹飘俊辑 鞘夸..
{
	DiscardCaches(ECompositeMeshCacheType::ECMCT_Remote);
	DiscardCaches(ECompositeMeshCacheType::ECMCT_Local);
}
// 捞傈 规侥苞 厚背 抛胶飘甫 困秦 力傍
void UB2CompositeMeshCache::SetAllowLocalPCMeshRootSet(bool bSet)
{
	bAllowLocalPCMeshRootSet = bSet;

	if (!IsLocalPCMeshRootSetAllowed())
	{ // 扁瓷阑 舶促搁 缴敲窍霸 某浆甫 府悸.
		DiscardCaches(ECompositeMeshCacheType::ECMCT_Remote);
		DiscardCaches(ECompositeMeshCacheType::ECMCT_Local);
	}
	else
	{
		// 扁瓷阑 难绰 版快 肺拿 某腐磐 某浆父 风飘悸俊 眠啊秦 淋.
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
	//BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
	//BII_CHECK(B2GameModule && B2GameModule->GetCompositeMeshCacher());
	//return B2GameModule ? B2GameModule->GetCompositeMeshCacher() : NULL;
	return NULL;
}
