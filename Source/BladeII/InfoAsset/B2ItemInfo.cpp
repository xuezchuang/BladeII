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

// 窜鉴馆汗 内靛 喉废. 糕滚 捞抚阑 持绢林绢具 秦辑 袍敲复档 救登绊..
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

	// 贸澜俊 酒捞袍 捞抚俊 前龙 立滴荤甫 嘿捞绰 侥捞菌绰单 前龙 立滴荤绰 蝶肺 钎矫窍霸 凳. 趣矫 绢恫 侥栏肺 何劝且瘤 葛福聪 概俺函荐尔 内靛绰 巢败狄.
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
	// 概 FSingleItemInfoData 甫 掘绢棵 锭付促 荤侩窍绰 扒 酒聪绊 扁夯利栏肺绰 咯扁 概 糕滚甸篮 鞘夸且 锭 弊锭弊锭 肺爹茄促.
	// 咯变 GetSomeLazyLoadedAssetCommon 救 静绊 弊成 啊绰 吧肺.

	int32 CheckTAssets = 0; // TAsset 糕滚 眠啊矫 FSingleItemInfoLoadedPtrData 俊档 眠啊 鳖冈瘤 臼霸.
	if (IconMaterial.IsValid() == false && IconMaterial.ToString().Len() > 0)
	{
		InLoadManager.LoadSynchronous(IconMaterial);
	}
	++CheckTAssets; // 汗嘿且 锭 捞巴档 鞍捞 偿偿
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
	checkSlow(FoundLoadedPtrEntry); // 绝栏搁 官焊
	if (FoundLoadedPtrEntry)
	{
		int32 CheckLoadedPtr = 0;
		
		// GC 规瘤侩 肺爹等 饭欺繁胶 某教. 捞率俊 急攫等 TSoftObjectPtr 糕滚甸俊 措秦 FSingleItemInfoLoadedPtrData 俊 亲格阑 持绢林绢具 窃.
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
				FString::Printf(TEXT("[Warning] FSingleItemInfoData 狼 TAsset 糕滚客 FSingleItemInfoLoadedPtrData 狼 肺爹等 俊悸 饭欺繁胶 亲格 荐啊 促福促. 哪腔磐啊 梆 气惯茄促."))
				));
		}
#endif
	}
}

void FSingleItemInfoData::UnloadAllTAssets(FStreamableManager& InLoadManager, int32 MyRefID, TMap<int32, FSingleItemInfoLoadedPtrData>& LoadedPtrMap, bool bUnloadEvenRootSet)
{
	if (bUnloadEvenRootSet)
	{
		MarkLoadedEquipPartAtRooSet(false); // 趣咯唱 RootSet 捞扼搁 弊芭 力芭何磐 秦具.
	}

	// bUnloadEvenRootSet 捞 酒聪扼档.. 朝妨档 绢瞒乔 角力 unloading 救 瞪 巴烙. 风飘悸捞聪.
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
	// PCPart** 棺 捞客 包访秦辑 鞘夸茄 SixStar** 殿篮 咯扁辑 力寇茄促.
	// 父老 捞繁 急琶利 攫肺爹捞 促弗 侥栏肺 鞘夸秦瘤搁 俺喊 TAsset 甸俊 措茄 攫肺爹阑 力傍窍绰 霸 亮阑 巴.

	FSingleItemInfoLoadedPtrData* MyCachedRefContainer = LoadedPtrMap.Find(MyRefID);
	if (MyCachedRefContainer) // 给 茫栏搁 柳累俊 肺爹阑 窍瘤 臼篮 芭电瘤, 购啊 肋给登菌电瘤.
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
	if (bUnloadEvenRootSet)  // 趣咯唱 RootSet 捞扼搁 弊芭 力芭何磐 窍瘤父 鞘夸茄 版快 RootSet 栏肺 瘤沥茄 扒 攫肺爹俊辑 力寇矫懦 荐 乐档废.
	{
		MarkLoadedEquipPartAtRooSet(false);
	}

	FSingleItemInfoLoadedPtrData* MyCachedRefContainer = LoadedPtrMap.Find(MyRefID);

	if (PCPartMesh.IsValid() && (!PCPartMesh->IsRooted() || bUnloadEvenRootSet))
	{
		PCPartMesh->RemoveFromRoot();
		if (MyCachedRefContainer) { // 给 茫栏搁 柳累俊 肺爹阑 窍瘤 臼篮 芭电瘤, 购啊 肋给登菌电瘤.
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
{ // UnloadOnlyEquipPart 阑 观俊辑 蝶肺 阂矾临 老捞 乐绢辑..
	if (OwnerItemInfoTable)
	{
		UnloadOnlyEquipPart(OwnerItemInfoTable->InfoLoadManager, CachedMyRefID, OwnerItemInfoTable->LoadedPtrMap, bUnloadEvenRootSet);
	}
}

void FSingleItemInfoData::MarkLoadedEquipPartAtRooSet(bool bRootSet)
{
	// 咯扁辑 蝶肺 肺爹阑 窍瘤绰 臼绰促. 肺爹 登绢 乐绰 巴父 贸府..
	// AddToRoot 甫 窍搁 Unload 档 冈洒瘤 臼阑 芭.
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
	// TryAsyncLoad 俊 鞘夸茄 沥焊甸 避绢坷扁. 鉴辑措肺 窍唱究.
	// 咯扁 糕滚 TAsset 葛滴甫 持绰 扒 酒聪促. 某腐磐 皋浆 炼钦 棺 何啊 瓤苞俊 鞘夸茄 犁丰甸父.
	// 角力 Async 夸没阑 焊尘 锭 窜老 SingleItemInfoData 挥 酒聪扼 咯矾 RefID 俊 措秦 避绢葛篮 FB2AsyncRequestInfo 府胶飘甸阑 钦媚辑 焊郴霸 瞪 家瘤啊 臭促. 某腐磐 窍唱 炼钦窍绰单 咯矾 RefID 狼 酒捞袍 单捞磐啊 鞘夸窍扁 锭巩捞促.

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
	// 咯扁辑 某浆窍绊磊 窍绰 措惑篮 技泼捞 等 芭扼搁 肺爹捞 等 惑怕咯具 茄促. (角力肺绰 葛滴 技泼等 巴捞 靛拱 芭扼 Get 捞 null 阑 府畔窍绰 霸 捞惑茄 扒 酒聪促.)
	// Async 肺爹苞 包访秦辑 静绰 巴牢父怒 flush 夸家 绝捞 窜鉴 Get 父..

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
	//	// 夯拜 ItemInfo 利侩 傈俊 BaseBP 刚历
	//	OverrideByBaseBP(InSingleInfoOfID->GetBaseBPClass(this));

	//	// 咯扁辑 ItemInfo 俊 乐绰 葛电 沥焊甫 荤侩窍瘤绰 臼阑 巴捞促. 老何绰 UI 俊 静牢促.

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

	//	// 绢掳 粱 吝备抄规牢 芭 鞍瘤父 咯窍埃 瞒辟瞒辟 技泼.
	//	// 捞巴捞 阂赴 捞饶 AB2DropItem::BeginPlay 啊 阂府瘤 臼绰促搁 老何 贸府甫 荐悼栏肺 秦具 且 巴.

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

	//// 捞巴捞 阂府绰 矫痢篮 ItemInfo 利侩捞唱 BeginPlay 捞傈烙.
	//// 府家胶 饭欺繁胶 技泼狼 版快 技泼 饶 角力 荤侩 矫痢捞 攫力牢瘤 犬牢且 鞘夸啊 乐澜.

	//// 窍困 哪器惩飘甸篮 蝶肺 贸府秦具 茄促. this 措惑狼 CopyObjectProperties 俊辑 窍困 哪器惩飘甸阑 汗荤窍霸 窍搁 哪器惩飘 磊眉甫 弊措肺 啊廉客 荤侩窍骨肺 救凳.

	//CopyObjectProperties(this->AudioComp, BPCDO->AudioComp, true, true);
	//CopyObjectProperties(this->MainBodyPS, BPCDO->MainBodyPS, true, true);
	//CopyObjectProperties(this->CollisionComp, BPCDO->CollisionComp, true, true);
	//CopyObjectProperties(this->DropItemMoveComp, BPCDO->DropItemMoveComp, true, true);
	//
	//// 磊扁 磊脚篮 盖 唱吝俊 贸府秦 夯促.
	//CopyObjectProperties(this, BPCDO, true, true);
}

////////////////////////////////////////////////////////////////////////////////
void FItemInfoEquipPartsAsyncReqCombinedID::SetRefIDs(const TArray<int32>& InRefIDs)
{
	// 捞吧 荤侩窍妨绰 券版俊辑狼 措帆利牢 力茄. 概 夸家喊肺 for 巩 倒妨啊哥 眉农窍绰 霸 乐绢辑 捞霸 目瘤搁 己瓷 巩力啊 惯积且 荐 乐澜.
	// 措帆 敲饭捞绢 某腐磐 颇飘 俺荐父怒 鞘夸茄单 父俊 窍唱 抗惑摹 给茄 版快俊 措厚窍扁 困秦..
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
	// MySerialNumber 尔 MyAsyncRequestName 篮 厚背且 鞘夸啊 绝促. 
	// 贸澜何磐 辑肺 促福霸 侥喊窍妨带 扒 RefIDs 挥.. 
	// RefIDs 备己捞 促弗单 SerialNumber 啊 鞍栏搁 巩力啊 登瘤父, RefIDs 备己捞 鞍绊 SerialNumber 啊 促福促绊 秦辑 巩力瞪 扒 绝促.

	return IsEqualByRefIDs(InOther);
}

bool FItemInfoEquipPartsAsyncReqCombinedID::IsEqualByRefIDs(const FItemInfoEquipPartsAsyncReqCombinedID& InOther) const
{
	if (RefIDs.Num() == InOther.RefIDs.Num())
	{
		bool bHasAnyDifference = false;
		for (int32 IA = 0; IA < RefIDs.Num(); ++IA)
		{
			if (!InOther.RefIDs.Contains(RefIDs[IA])) // 滴 硅凯 俺荐啊 鞍栏骨肺 茄率狼 葛电 夸家啊 促弗 率俊 器窃登绢 乐促搁 度鞍篮 芭.
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
//		// 沥秦柳 窍唱甫 肺爹
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
//				FString::Printf(TEXT("[Warning] ItemInfo 单捞磐 抛捞喉阑 茫瘤 给窃. 哪腔磐啊 梆 气惯茄促."))
//				));
//		}
//
//		if (SetInfoData == NULL)
//		{
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
//				FString::Printf(TEXT("[Warning] ItemSetInfo 单捞磐 抛捞喉阑 茫瘤 给窃. 哪腔磐啊 梆 气惯茄促."))
//				));
//		}
//
//#endif
//	}
}

FSingleItemInfoData* UB2ItemInfo::GetInfoData(int32 ItemRefID, bool bFullyLoad)
{
	// ItemRefID 甫 key 肺 荤侩.
	FSingleItemInfoData* FoundDataRow = TheData ? TheData->FindRow<FSingleItemInfoData>(FName(*FString::FromInt(ItemRefID)), TEXT("")) : NULL;
	if (FoundDataRow == NULL)
	{
#if WITH_EDITOR && !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("ItemInfo 抛捞喉俊辑 RefID %d 狼 亲格阑 茫瘤 给窃. 哪腔磐啊 梆 气惯茄促."), ItemRefID)
			));
#endif
		return NULL;
	}

	FoundDataRow->CachedMyRefID = ItemRefID; // RefID 某教秦 初栏搁 SingleInfo 狼 俺喊 单捞磐 肺爹 矫 荤侩窍霸 瞪 芭.

	if (bFullyLoad
#if WITH_EDITOR
		|| GIsEditor
#endif
		)
	{
		// 被捞 LoadAllTAssets 甫 窍瘤 臼歹扼档 角力 荤侩且 锭俊绰 俺喊 饭欺繁胶甸阑 肺爹窍霸 瞪 巴. LoadAllTAssets 栏肺绰 镜单绝绰 皋葛府 家葛狼 家瘤啊 乐促.
		FoundDataRow->LoadAllTAssets(InfoLoadManager, ItemRefID, LoadedPtrMap);
	}

	return FoundDataRow;
}

int32 UB2ItemInfo::GetRandomItemRefID()
{
	return 1000100; //府家胶 力累捞 登绢 乐瘤 臼扁 锭巩俊 霉 row狼 酒捞袍父 郴柜绰 巴栏肺 烙矫 荤侩
	/*
	uint32 NumItems = TheData ? TheData->RowMap.Num() : 0;

	if (NumItems > 0)
	{
		int32 RandIndex = FMath::FRandRange(0.0f, (float)NumItems - 1.5f); //榜靛 力寇

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

FItemSetInfoData* UB2ItemInfo::GetSetInfoData(int32 SetUniqueKey)
{
	const FName* DataTableKey = SetUniqueKeyMap.Find(SetUniqueKey);
	if (SetInfoData && DataTableKey)
	{
		return SetInfoData->FindRow<FItemSetInfoData>(*DataTableKey, TEXT(""));
	}

	return nullptr;
}

FItemSetInfoData* UB2ItemInfo::GetSetInfoData(int32 GroupID, EPCClass PCClass, int32 Grade)
{
	const int32 SetUniqueKey = GetSetItemUniqueKey(GroupID, PCClass, Grade);
	return GetSetInfoData(SetUniqueKey);
}

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
//		GetInfoData(InRefIDList[RI], true); // FullyLoad 甫 true 肺 林绊 何福搁 促 肺靛凳. 捞吧 措帆 割啊瘤 裹林喊肺 唱床辑 Preload 窍绰 规侥捞 鞘夸且 荐档.
//	}
//#if !UE_BUILD_SHIPPING
//	if (InRefIDList.Num() > 0){
//		UE_LOG(LogBladeII, Log, TEXT("ItemInfo pre-load for %d assets"), InRefIDList.Num());
//	}
//#endif	
}

void UB2ItemInfo::UnloadAll()
{
	check(0); // 澜 泪瘪.. 捞芭 老窜 静搁 救瞪 巴 鞍澜. UnloadAllExceptCurrentLocalEquipPartData 肺 措脚..

	// DataTable 狼 葛电 row 甫 iterate 窍绰 规过牢淀.
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
		LoadedPtrMap.Empty(); // 犬牢 荤混
	}	

	ResetAsyncLoadReqInfo();
}

void UB2ItemInfo::UnloadAllExceptCurrentLocalEquipPartData()
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2ItemInfo::UnloadAllExceptCurrentLocalEquipPartData"));

//	// 漂沥 何盒俊辑 泅犁 肺拿 某腐磐甸狼 厘馒 厘厚 颇飘尔 包访等 老何俊 鞘夸茄 单捞磐 捞寇俊 葛电 巴阑 朝府扁 困窃.
//	// 抗甫 甸绢 档皑捞唱 牢亥配府 唱哎 矫 荤侩.
//	// .. 弊府绊 搬惫篮 老馆利牢 UnloadAll 惑炔阑 捞吧肺 措眉秦具 且 淀.. 肺拿 某腐磐 厘馒 颇飘甸阑 巢败滴绰 霸 粱 吝夸秦廉辑.
//
//#if WITH_EDITOR
//	if (GIsEditor) {
//		return;
//	}
//#endif
//
//	TMap<int32, bool> AllEquippedRefIDs; // Value 绰 狼固绝绊 傲 Key 八祸父 且 芭.
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
//			AllEquippedRefIDs.Add(ThisEquipped.ItemRefID, true); // Key 蔼父 乐澜 凳.
//		}
//	}
//	
//	if (TheData)
//	{
//		// RefID 埃俊 吝汗秦辑 荤侩登绰 颇飘甸捞 乐绢辑 风飘悸 贸府俊 林狼啊 鞘夸窍促..
//		// 捞 沥档搁 府家胶 肺爹俊 鞘利窍绰 惑寸茄 荐霖牢单.. ぱぱ
//
//		// 1瞒 菩胶俊辑 葛滴 RootSet 俊辑 力芭
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
//		// 2瞒 菩胶俊辑 泅犁 厘馒牢 巴父 RootSet 俊 眠啊
//		for (TMap<int32, bool>::TIterator RefIDIt(AllEquippedRefIDs); RefIDIt; ++RefIDIt)
//		{
//			int32 ThisRefID = RefIDIt.Key();
//			FSingleItemInfoData* FoundDataRow = GetInfoData(ThisRefID, false);
//			if (FoundDataRow) {
//				FoundDataRow->MarkLoadedEquipPartAtRooSet(true);
//			}
//		}
//		// 付瘤阜栏肺 RootSet 牢 芭 力寇茄 Unload
//		TArray<FName> RowKeys = TheData->GetRowNames();
//		for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
//		{
//			FName& ThisRowKey = RowKeys[RI];
//			FSingleItemInfoData* FoundDataRow = TheData->FindRow<FSingleItemInfoData>(ThisRowKey, TEXT(""));
//			if (FoundDataRow)
//			{ // GetAllRows 肺 茄芭尔 鉴辑啊 救 嘎栏妨唱...
//				const int32 ThisRowNumber = FCString::Atoi(*ThisRowKey.ToString());
//				FoundDataRow->UnloadAllTAssets(InfoLoadManager, ThisRowNumber, LoadedPtrMap, false);
//			}
//		}
//	}
}

FSingleItemInfoLoadedPtrData* UB2ItemInfo::GetLoadedPtrForID(int32 InItemID)
{
	FSingleItemInfoLoadedPtrData* FoundLoadedPtrEntry = LoadedPtrMap.Find(InItemID);
	if (!FoundLoadedPtrEntry && InItemID > 0) // 绝栏搁 货肺 积己.
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
//	// PostSpawnInitialize 唱 BeginPlay 甫 蝶肺 都绊 弊 傈俊 ItemInfo 甫 利侩窍扁 困窃.
//	// bDeferPostSpawnInitialize 父 郴何利栏肺 技泼窍绰 PCClassInfo/NPCClassInfo 客绰 崔府 bDeferConstruction 档 郴何利栏肺 技泼窍绰单 FinishSpawning 捞傈俊 漂喊洒 且 巴捞 绝促绊 魄窜登扁 锭巩.
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
//	// PCClassInfo 唱 NPCClassInfo 狼 版快客 付蛮啊瘤肺 捞霸 力措肺 冈洒妨搁 SpawnParameters 俊辑 bDeferConstruction 捞 技泼登绢 乐绢具 且 巴. 弊贰具 BeginPlay 啊 捞 捞饶 阂府霸 凳.
//	// 扁夯 努贰胶绰 沥秦 初绊 荤侩. (DropItem_DefaultBaseBPClass)
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
//	// SignificantData 狼 扁鸥 唱赣瘤绰 ItemRefID 甫 扁馆栏肺 辑滚俊辑 啊廉坷绰单 俺惯滚傈 努扼捞攫飘俊辑绰 酒贰客 鞍捞 抛胶飘 单捞磐甫 持绢临 荐 乐澜.
////#if !UE_BUILD_SHIPPING 
//	//DevOnlyLoadItemClientTestData(SpawnedActor->SignificantData);
////#endif
//	
//
//	// ItemInfo 利侩 饶 bDeferConstruction 棺 bDeferPostSpawnInitialize 俊 蝶弗 贸府
//	if (SpawnedActor)
//	{
//		if(!FMath::IsNearlyZero(StayTime))
//		{
//			SpawnedActor->SetQTEStayTime(StayTime);
//		}
//
//		SpawnWorld->ManualPostSpawnInitialize(SpawnedActor, SpawnTransform, FinalSpawnParam);
//
//		// FinishSpawning 俊辑 BeginPlay 啊 阂覆. 父老 酒捞袍 spawn 惑炔俊 蝶扼 捞吧 唱吝俊 贸府秦具 窍绰 版快啊 积变促搁 惑窜 bDeferConstruction 技泼窍绰 何盒苞 窃膊 捞吧 观栏肺 滑 巴.
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

	//// DataTable 狼 葛电 row 甫 iterate 窍绰 规过牢淀.
	//TArray<FName> RowKeys = TheData->GetRowNames();
	//
	//const float TotalProgRange = FMath::Max(0.0f, InEndProgress - InStartProgress);
	//// InfoAsset 俊 殿废登绰 局甸捞 倔付唱 登绰瘤俊 蝶扼 利例洒 沥窍绰 芭. 割锅付促 茄锅究 诀单捞飘 且 瘤.
	//const int32 ProgUpdateInterval = 10;
	//const int32 TotalProgUpdateCount = RowKeys.Num() / ProgUpdateInterval;
	//const float SingleProgIncAmount = TotalProgRange / (float)TotalProgUpdateCount;

	//float CurrProg = InStartProgress;
	//
	//for (int32 RI = 0; RI < RowKeys.Num(); ++RI)
	//{
	//	GetInfoData(FCString::Atoi(*RowKeys[RI].ToString()), true); // 救俊辑 促矫 FName 捞 登聪 扯厚牢单 绢瞒乔 俊叼磐 扁瓷捞聪..

	//	// Splash progress 诀单捞飘. 酒贰 SetInfoData 绰 倔付 救瞪 吧肺 摹绊..
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
	//		GetSetInfoData(FCString::Atoi(*SetRowKeys[RI].ToString())); // 救俊辑 促矫 FName 捞 登聪 扯厚牢单 绢瞒乔 俊叼磐 扁瓷捞聪..
	//	}
	//}


	//// 捞吧 荤侩窍妨搁 BladeIIGameImpl::StartupModule 俊辑 窍霸 瞪 刨单 弊矾搁 捞巴狼 牢胶畔胶 困摹啊 官差霸 瞪 巴..
}

void UB2ItemInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{ 
	//
	// DataTable 俊叼飘 矫俊绰 捞霸 救冈洒唱..
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
			//FoundDataRow->LoadLocalizedName(); 捞哩 两档 镜葛绝焙.
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
//	// PendingEquipPartsAsyncLoadReqList 俊辑 肯丰等 浚飘府甫 侥喊窍绊 府胶飘俊辑 力芭.
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
//	// 肺爹等 俊悸甸阑 某教
//	for (int32 LoadedRefID : JustLoadedRefIDs)
//	{
//		// 咯扁辑 FullyLoad 甫 林搁 某教捞 舅酒辑 登变 窍瘤父
//		// 捞 Async 肺爹篮 窜老 Info 俊 殿废等 葛电 吧 肺爹窍妨绰 霸 酒聪骨肺 开矫唱 FullyLoad 绝捞 弊成.
//		// 霸促啊 芭扁辑 FullyLoad 窍绰 扒 async flush 夸家啊 乐绢辑 async 肺爹苞 楷包秦辑 静扁 利例摹 臼促.
//		FSingleItemInfoData* ThisLoadedInfo = GetInfoData(LoadedRefID, false); 
//		if (ThisLoadedInfo)
//		{
//			ThisLoadedInfo->CacheOnEquipPartsAsyncLoadComplete(LoadedRefID, LoadedPtrMap);
//		}
//	}
//
//	// 肯丰等 Request 捞抚阑 淬酒辑 捞亥飘 朝覆.
//	if (OnEquipPartsAyncLoadComplete.IsBound())
//	{
//		OnEquipPartsAyncLoadComplete.Broadcast(CompleteRequest);
//	}
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	if (!bFoundMatchingCombinedID)
//	{
//		// 捞繁 老捞 乐阑 荐档? 度鞍篮 RefID 炼钦栏肺 楷崔酒 肺爹 夸没阑 焊尘 版快 笛 促俊 措秦 阿阿 妮归捞 柯促搁 捞繁 老捞 惯积且 刨单 角力肺 弊繁 版快俊绰 茄锅父 柯促.
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
	//{ // 肺爹 绝捞 弊成 InfoData 伯单扁父
	//	FSingleItemInfoData* ThisInfoData = GetInfoData(ThisRefID, false);
	//	if (ThisInfoData)
	//	{
	//		ThisInfoData->GetEquipPartsAsyncLoadReqInfo(TotalReqInfo); // 阿 InfoData 俊辑 避绢柯 芭 穿利矫糯. 促 葛澜
	//	}
	//}


	////
	//// GetAsyncReqCombinedIDofPartsList 甫 刚历 眉农秦 焊绊 捞固 pending 捞搁 return false 秦 杭鳖?
	//// 楷崔酒 甸绢柯 吝汗 夸没 公矫 瞒盔..
	////FItemInfoEquipPartsAsyncReqCombinedID AsyncReqCombinedID; 
	////if (GetAsyncReqCombinedIDofPartsList(InRefIDList, AsyncReqCombinedID, false))
	////{
	////	return false;
	////}
	////


	//FItemInfoEquipPartsAsyncReqCombinedID AsyncReqCombinedID; // Async 肺爹 夸没捞尔 肯丰 捞饶 侥喊俊 鞘夸茄 沥焊.
	//if (GetAsyncReqCombinedIDofPartsList(InRefIDList, AsyncReqCombinedID, true)) // 咯扁啊 蜡老窍霸 扁粮俊 绝栏搁 货肺 父甸绢具 且 镑捞 瞪 淀.
	//{
	//	check(AsyncReqCombinedID.IsValid());
	//	TryAsyncLoad(AsyncReqCombinedID.MyAsyncRequestName, TotalReqInfo, bShowBlockingUI);
	//	OutRequestedName = AsyncReqCombinedID.MyAsyncRequestName; // 罐篮 率俊辑 鞘夸窍搁 捞吧 啊瘤绊 肯丰 捞亥飘啊 吭阑 锭 侥喊 啊瓷.
	//	return true;
	//}
	//
	return false;
}

// 窜鉴 Get 栏肺辑父 静妨带 霸 酒囱 Generate 狼 狼固啊 粱 乐促. const 己拜捞绢具 窍绰 扒 酒丛. 
bool UB2ItemInfo::GetAsyncReqCombinedIDofPartsList(const TArray<int32>& InRefIDList,
	FItemInfoEquipPartsAsyncReqCombinedID& OutNewOrGeneratedID, bool bGenerateNewOnMiss)
{
	FItemInfoEquipPartsAsyncReqCombinedID LocalTempIDStruct;
	LocalTempIDStruct.SetRefIDs(InRefIDList);

	int32 FoundIndexFromPending = PendingEquipPartsAsyncLoadReqList.Find(LocalTempIDStruct);
	if (FoundIndexFromPending != INDEX_NONE)
	{ // 泅犁 async 肺爹 吝牢 芭..
		OutNewOrGeneratedID = PendingEquipPartsAsyncLoadReqList[FoundIndexFromPending];
		return true;
	}

	if (bGenerateNewOnMiss)
	{
		// 狼固绝捞 拌加 促弗 炼钦栏肺 Get 窍促啊 SerialNumber 啊 付备 刘啊窍绰 老捞 绝档废.. (弊府绊 PendingEquipPartsAsyncLoadReqList 档 刘啊..)
		// 绝绰 版快 犬角洒 货肺 父甸绊磊 且 锭父 bGenerateNewOnMiss 甫 瘤沥.

		// RefID 府胶飘 炼钦栏肺 unique 窍霸 侥喊 啊瓷茄 巩磊凯阑 梆捞梆措肺 父靛妨搁 购啊 奖瘤 臼阑 抛聪 老访锅龋甫 何咯茄促
		LocalTempIDStruct.MySerialNumber = EquipPartsAsyncLoadReqCounter;
		++EquipPartsAsyncLoadReqCounter; // 咯扁啊 刘啊窍绰 蜡老茄 瘤痢捞 登绢具 茄促.
		// TryAsyncLoad 俊 RequestName 栏肺 镜 芭
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
		return ConsumablesIconBGMtrl.LoadSynchronous(); // 漂喊洒 饭欺繁胶甫 蜡瘤秦具 且 格利捞 乐绰 芭 酒丛 弊成 LoadSynchronous 静搁 等促. 捞傈俊 皋葛府 包府啊 肋 救登绢辑 RefHolder 扼绰 巴俊 某教阑 沁带 巴烙.
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
				AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedItemIconBGMtrlPerGrade_Simple); // 肺爹等 俊悸 某教. 牢郸胶 度鞍捞 嘎苗辑
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
				AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedItemIconBGMtrlPerGrade); // 肺爹等 俊悸 某教. 牢郸胶 度鞍捞 嘎苗辑
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
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedEquipCategoryIcon); // 肺爹等 俊悸 某教. 牢郸胶 度鞍捞 嘎苗辑
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
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedEquipIconBGMtrlPerGrade); // 肺爹等 俊悸 某教. 牢郸胶 度鞍捞 嘎苗辑
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
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedItemQualityBGMtrl); // 肺爹等 俊悸 某教. 牢郸胶 度鞍捞 嘎苗辑
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
			AddOrUpdateMtrlRefHolderCache(EtherTier, LoadedAsset, AllLoadedRefHolder.LoadedEtherTierBGMtrl); // 肺爹等 俊悸 某教. 牢郸胶 度鞍捞 嘎苗辑
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
			AddOrUpdateMtrlRefHolderCache(EtherTier, LoadedAsset, AllLoadedRefHolder.LoadedEtherTierNameBGMtrl); // 肺爹等 俊悸 某教. 牢郸胶 度鞍捞 嘎苗辑
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
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedStarNumberMtrl); // 肺爹等 俊悸 某教. 牢郸胶 度鞍捞 嘎苗辑
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
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedEtherPosBGMtrl); // 肺爹等 俊悸 某教. 牢郸胶 度鞍捞 嘎苗辑
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
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedCostumeIconBGMtrlPerGrade); // 肺爹等 俊悸 某教. 牢郸胶 度鞍捞 嘎苗辑
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
			AddOrUpdateMtrlRefHolderCache(ElemIndex, LoadedAsset, AllLoadedRefHolder.LoadedTotemOptionIndexIcon); // 肺爹等 俊悸 某教. 牢郸胶 度鞍捞 嘎苗辑
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
{ // 傲 咯扁辑 静捞绰 窜鉴茄 馆汗 内靛
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
	// 狼档摹 臼霸 历厘等 饭欺繁胶甫 朝妨霖促.
	CleanupOnPreSave();
}
#endif