// Fill out your copyright notice in the Description page of Project Settings.


#include "B2PCClassInfo.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimBlueprintGeneratedClass.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
//#include "Particles/ParticleSystem.h"
//#include "Components/DecalComponent.h"
//#include "Components/ChainComponent.h"
//#include "PhysicsEngine/RadialForceComponent.h"

#include "B2SkillInfo.h"
#include "B2InGameHUDInfo_Player.h"
#include "BladeIIProjectile.h"
#include "B2AreaDamageActorBase.h"
#include "BladeIIGameMode.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2ItemInfo.h"
#include "B2PCClassInfoBox.h"
#include "SkeletalMeshMerge.h"
#include "Animation/SkeletalMeshActor.h"
#include "B2WingInfo.h"
#include "B2Airport.h"
#include "B2SomeInfo.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"
#include "B2CompositeMeshCache.h"
#include "FB2ErrorMessage.h"
#include "../BladeII/BladeIILocText.h"
#include "../BladeII/BladeIIUtil.h"
#include "B2AssetLoader.h"
#include "B2UIDocHelper.h"
#include "B2GameInstance.h"

#define EQUIPMENTS_EFFECT

UB2InGameOnlyPCClassInfo::UB2InGameOnlyPCClassInfo(const FObjectInitializer& ObjectInitializer)
{
	BaseBPClass = NULL;
	AnimBPOverride = NULL;
	BehaviorTreeOverride = NULL;
	FloorRingPSOverride = NULL;
	ReadyWeaponSkillFXOverride = NULL;

	FloorRingColorMultiplier = 1.f;
}

TSubclassOf<ABladeIIPlayer> UB2InGameOnlyPCClassInfo::GetBaseBPClass(UObject* WorldContextObject)
{
	if (BaseBPClass)
	{
		return BaseBPClass;
	}

	//UB2GameInstance* B2GI = WorldContextObject ? Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)) : nullptr;
	//UB2SomeInfo* SomeInfo = B2GI ? B2GI->GetSomeInfo() : StaticFindSomeInfo();
	return/* SomeInfo ? SomeInfo->GetDefaultPCClassInfoBaseBPClass() :*/ NULL;
}

void ABladeIIPlayer::OverrideByPCClassInfo(class ABladeIIGameMode* InGameMode, UB2PCClassInfo* CustomPCClassInfo, bool bNoOverrideByInGameInfo)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::OverrideByPCClassInfo"));

	check(InGameMode);
//	// 老馆利牢 Player spawn 矫俊绰 GameMode 俊辑 啊廉咳. CustomPCClassInfo 瘤沥篮 漂荐茄 版快
//	UB2PCClassInfo* FinalUseClassInfo = CustomPCClassInfo ? CustomPCClassInfo : InGameMode->GetCurrentPCClassInfo();
//
//#if !WITH_EDITOR
//	check(FinalUseClassInfo); // 喉风橇赴飘 努贰胶肺 抛胶飘窍绰 惑炔俊辑绰 绝阑 荐 乐澜..
//#endif
//	if (!FinalUseClassInfo) {
//		return;
//	}
//
//	// Skill LOD Level Setting
//	switch (InGameMode->GetSkillLODType())
//	{
//	case EB2SkillLODType::ESLT_NONE:
//		SkillLOD = 0;
//		break;
//	case EB2SkillLODType::ESLT_ALL_LOD:
//		SkillLOD = 1;
//		break;
//	default:
//	case EB2SkillLODType::ESLT_ONLY_FOR_SELF:
//		SkillLOD = GetSkillLOD();
//		break;
//	}
//
//	// 肺厚俊急 肺爹 救瞪 局甸. 割割 牢霸烙俊辑父 肺爹窍搁 登绰 局甸篮 咯扁辑 单捞磐甫 掘绢柯促.
//	// 牢霸烙捞扼档 楷免俊父 荤侩瞪 侩档狼 dummy puppet 篮 bNoOverrideByInGameInfo 牢磊甫 林搁 InGameOnlyInfo 甫 荤侩窍瘤 臼阑 巴捞促.
//	// 漂洒 霸烙 努府绢 矫痢俊 汗棱茄 楷免捞 唱坷扁 傈 皋葛府甫 弥措茄 clear 窍绰 单俊 鞘夸且 荐 乐促.
//	UB2InGameOnlyPCClassInfo* InGameOnlyInfo = bNoOverrideByInGameInfo ? nullptr : FinalUseClassInfo->GetInGameOnlyInfo(this);
//
//	// PCClassInfo 肺 override 窍扁 傈 BaseBP override 何磐
//	if (InGameOnlyInfo)
//	{
//		this->OverrideByBaseBP(InGameOnlyInfo->GetBaseBPClass(this));
//	}
//
//	this->CharacterType = UB2PCClassInfo::FromPCClassToCharacterType(FinalUseClassInfo->PCClassEnum);
//
//	// Rename the actor using EPCClass Enum name. -> It could allow duplicated name if we have remote player.. We don't really have to do renaming.
//	/*const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, EPCCLASS_ENUM_PACKAGE_NAME, true);
//	if (EnumPtr)
//	{
//		FString ClassEnumName = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue(FinalUseClassInfo->PCClassEnum));
//		this->Rename(*(TEXT("BladeIIPlayer_") + ClassEnumName));
//	}*/
//
//	UCapsuleComponent* CollisionCapsule = this->GetCapsuleComponent();
//
//	if (CollisionCapsule)
//	{
//		if (FinalUseClassInfo->CapsuleHalfHeightOverride > 0.0f)
//		{
//			CollisionCapsule->SetCapsuleHalfHeight(FinalUseClassInfo->CapsuleHalfHeightOverride);
//		}
//		if (FinalUseClassInfo->CapsuleRadiusOverride > 0.0f)
//		{
//			CollisionCapsule->SetCapsuleRadius(FinalUseClassInfo->CapsuleRadiusOverride);
//		}
//	}
//
//	if (FinalUseClassInfo->DamageNumZOffsetOverride != 0.0f) // - 蔼档 甸绢哎 荐 乐霸..
//	{
//		this->DamageNumZOffset = FinalUseClassInfo->DamageNumZOffsetOverride;
//	}
//
//	USkeletalMeshComponent* MeshComp = GetMesh();
//
//	if (MeshComp)
//	{
//		if (FinalUseClassInfo->BaseMeshAsset)
//		{
//			// AnimBP 官层摹扁 傈俊 SetSkeletalMesh 何磐
//			MeshComp->SetSkeletalMesh(FinalUseClassInfo->BaseMeshAsset); // DefaultPart 甸捞 乐栏骨肺 BaseMeshAsset 捞 弊措肺 蜡瘤登绰 版快绰 颇飘甸阑 merge 窍绰 措脚 component 肺 备己窍绰 版快啊 瞪 巴.
//			if (InGameOnlyInfo && InGameOnlyInfo->AnimBPOverride)
//			{// 捞扒 搬惫 MeshComp->SetAnimInstanceClass 烙. 葛官老 叼滚彪 苞沥狼 蜡魂
//				this->SafeSetAnimBPClass(InGameOnlyInfo->AnimBPOverride);
//			}
//		}
//
//		if (FinalUseClassInfo->MeshScaleOverride > 0.0f)
//		{
//			MeshComp->SetRelativeScale3D(FVector(FinalUseClassInfo->MeshScaleOverride, FinalUseClassInfo->MeshScaleOverride, FinalUseClassInfo->MeshScaleOverride));
//		}
//
//		if (CollisionCapsule)
//		{
//			// Mesh assets have their pivot root at their feet, so lower it by half capsule size to locate it at the center.
//			MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -1.0f * CollisionCapsule->GetScaledCapsuleHalfHeight()));
//		}
//
//		// SkeletalMeshComponent 俊 嘿老 眠啊 哪器惩飘甸. 咯扁辑 嘿捞绰 局甸篮 扁夯利牢 巴甸牢单 PC 狼 版快 捞繁 霸 乐阑瘤 葛福摆促. 厘馒 酒捞袍 殿篮 蝶肺 甸绢哎 巴 (SetupComponentsForParts).
//		for (int32 STI = 0; STI < FinalUseClassInfo->AttachStaticMeshes.Num(); ++STI)
//		{
//			FCHSKCompAttachStaticMeshInfo& ThisInfo = FinalUseClassInfo->AttachStaticMeshes[STI];
//			if (ThisInfo.StaticMeshAsset != NULL)
//			{
//				UStaticMeshComponent* NewAttachComp = NewObject<UStaticMeshComponent>(this, NAME_None, RF_Transient);
//				if (NewAttachComp)
//				{
//					NewAttachComp->bReceivesDecals = false;
//					NewAttachComp->SetStaticMesh(ThisInfo.StaticMeshAsset);
//					NewAttachComp->SetRelativeScale3D(FVector(ThisInfo.AttachScale, ThisInfo.AttachScale, ThisInfo.AttachScale));
//					NewAttachComp->SetMobility(EComponentMobility::Movable); // StaticMesh 甫 Character 俊 嘿捞聪 Movable 肺
//					NewAttachComp->AttachToComponent(MeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisInfo.AttachSocketName);
//					NewAttachComp->RegisterComponent();
//				}
//			}
//		}
//		for (int32 STI = 0; STI < FinalUseClassInfo->AttachSkeletalMeshes.Num(); ++STI)
//		{
//			FCHSKCompAttachSkeletalMeshInfo& ThisInfo = FinalUseClassInfo->AttachSkeletalMeshes[STI];
//			if (ThisInfo.SkeletalMeshAsset != NULL)
//			{
//				USkeletalMeshComponent* NewAttachComp = NewObject<USkeletalMeshComponent>(this, NAME_None, RF_Transient);
//				if (NewAttachComp)
//				{
//					NewAttachComp->bReceivesDecals = false;
//					NewAttachComp->SetSkeletalMesh(ThisInfo.SkeletalMeshAsset);
//					NewAttachComp->SetRelativeScale3D(FVector(ThisInfo.AttachScale, ThisInfo.AttachScale, ThisInfo.AttachScale));
//					NewAttachComp->AttachToComponent(MeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), ThisInfo.AttachSocketName);
//					NewAttachComp->RegisterComponent();
//				}
//			}
//		}
//		for (int32 STI = 0; STI < FinalUseClassInfo->AttachParticleSystems.Num(); ++STI)
//		{
//			FCHSKCompAttachParticleSystemInfo& ThisInfo = FinalUseClassInfo->AttachParticleSystems[STI];
//			ThisInfo.CreatePSCHelper(this, MeshComp);
//		}
//	}
//
//	if (FinalUseClassInfo->HUDInfoClassOverride)
//	{
//		this->HUDInfoClass = FinalUseClassInfo->HUDInfoClassOverride;
//	}
//
//	UCharacterMovementComponent* MovementComp = Cast<UCharacterMovementComponent>(this->GetMovementComponent());
//	if (MovementComp)
//	{
//		if (FinalUseClassInfo->CharMovementEssentialSettings.MaxWalkSpeedOverride > 0.0f)
//		{
//			MovementComp->MaxWalkSpeed = FinalUseClassInfo->CharMovementEssentialSettings.MaxWalkSpeedOverride;
//		}
//		if (FinalUseClassInfo->CharMovementEssentialSettings.MaxAccelerationOverride > 0.0f)
//		{
//			MovementComp->MaxAcceleration = FinalUseClassInfo->CharMovementEssentialSettings.MaxAccelerationOverride;
//		}
//		// InitializeCombatStats 殿俊辑 弥辆利栏肺 可记 殿捞 啊固等 WalkSpeed 甫 促矫 技泼且 巴烙. 蝶扼辑 檬扁蔼 归诀秦 狄.
//		this->CachedBaseMaxWalkSpeed = MovementComp->MaxWalkSpeed;
//	}
//
//	// 傈捧 瓷仿 包访 单捞磐 吝 InfoAsset 技泼俊 狼秦 罐绰 巴甸
//	if (FinalUseClassInfo->CombatAbilityEssentialSettings.MaxArmorOverride > 0.0f)
//	{
//		this->MaxArmor = FinalUseClassInfo->CombatAbilityEssentialSettings.MaxArmorOverride;
//		this->Armor = this->MaxArmor;
//	}
//
//	if (FinalUseClassInfo->TargetingDistanceOverride > 0.0f)
//	{
//		this->TargetingDistance = FinalUseClassInfo->TargetingDistanceOverride;
//	}
//	if (FinalUseClassInfo->MaxDistanceToAllowAutoAttackOverride > 0.0f)
//	{
//		this->MaxDistanceToAllowAutoAttack = FinalUseClassInfo->MaxDistanceToAllowAutoAttackOverride;
//	}
//	if (FinalUseClassInfo->DetectDistanceOverride > 0.0f)
//	{
//		this->DetectDistance = FinalUseClassInfo->DetectDistanceOverride;
//	}
//	if (FinalUseClassInfo->MoveToMobAcceptableRadiusOverride > 0.f)
//	{
//		this->MoveToMobAcceptableRadius = FinalUseClassInfo->MoveToMobAcceptableRadiusOverride;
//	}
//
//	// ETM_End 甫 override condition 栏肺 且 荐档 乐瘤父 editcondition 阑 劝侩秦 焊磊..
//	if (FinalUseClassInfo->bOverride_TargetingMode && FinalUseClassInfo->TargetingModeOverride != ETargetingMode::ETM_End)
//	{
//		this->TargetingMode = FinalUseClassInfo->TargetingModeOverride;
//	}
//
//	if (InGameOnlyInfo)
//	{
//		for (int32 PCI = 0; PCI < InGameOnlyInfo->AdditionalProjectileClasses.Num(); ++PCI)
//		{
//			if (InGameOnlyInfo->AdditionalProjectileClasses[PCI])
//			{
//				this->ProjectileClasses.AddUnique(InGameOnlyInfo->AdditionalProjectileClasses[PCI]);
//			}
//		}
//
//		for (int32 PCI = 0; PCI < InGameOnlyInfo->AdditionalAreaDamageClasses.Num(); ++PCI)
//		{
//			if (InGameOnlyInfo->AdditionalAreaDamageClasses[PCI])
//			{
//				this->AreaDamageClasses.AddUnique(InGameOnlyInfo->AdditionalAreaDamageClasses[PCI]);
//			}
//		}
//
//
//		if (InGameOnlyInfo->DefaultAttackRangeOverride > 0)
//		{
//			DefaultAttackRange = InGameOnlyInfo->DefaultAttackRangeOverride;
//		}
//		if (InGameOnlyInfo->AutoMoveRangeOverride > 0)
//		{
//			AutoMoveRange = InGameOnlyInfo->AutoMoveRangeOverride;
//		}
//	}
//
//	if (InGameOnlyInfo)
//	{
//		FloorRingColor = InGameOnlyInfo->FloorRingColor;
//	}
//
//	if (InGameOnlyInfo)
//	{
//		FloorRingColorMultiplier = FMath::Max(1.f, InGameOnlyInfo->FloorRingColorMultiplier);
//	}
//
//	if (InGameOnlyInfo && InGameOnlyInfo->FloorRingPSOverride)
//	{
//		FloorRingPS = InGameOnlyInfo->FloorRingPSOverride;
//	}
//
//	if (InGameOnlyInfo && InGameOnlyInfo->ReadyWeaponSkillFXOverride)
//	{
//		ReadyWeaponSkillFX = InGameOnlyInfo->ReadyWeaponSkillFXOverride;
//	}
//
//	if (InGameOnlyInfo && InGameOnlyInfo->BehaviorTreeOverride)
//	{
//		SetBehaviorTree(InGameOnlyInfo->BehaviorTreeOverride);
//	}
}

void ABladeIIPlayer::SetUseSectionMergedMesh(bool bUseSectionMerged)
{
	//// AnimNotify 甫 烹秦 捞霸 惯悼捞 登绰 风飘啊 乐栏搁 check 俊 吧副 巴. 弊凡 订 DeferredSetUseSectionMergedMesh 甫 荤侩.

	//// 捞傈俊 呼靛秦 初篮 霸 乐促搁 弊吧 啊廉客辑 镜 巴捞绊 酒聪搁 货肺 呼靛
	//SetupComponentsForParts(bUseSectionMerged); 

	//// 眠啊 惑怕 函荐啊 酒流 绝绰单 酒流 迭洒 鞘夸档 绝瘤父 犬角洒 惑怕 飘发欧阑 且 荐 乐阑瘤 葛福摆扁档 秦辑

	//GetWorldTimerManager().ClearTimer(DeferredSetUseSectionMergedMeshTH); // Deferred 肺 龋免沁阑 荐 乐促.
}
void ABladeIIPlayer::DeferredSetUseSectionMergedMesh(bool bUseSectionMerged)
{
	//FTimerManager& TheTM = GetWorldTimerManager();
	//TheTM.ClearTimer(DeferredSetUseSectionMergedMeshTH);
	//// 矫埃篮 咯扁急 利寸洒 霖促. 父老 溜阿 利侩登绢具 等促芭唱 弥家茄 官肺 促澜 平俊 利侩登绢具 窍绰 版快扼搁 促弗 规过阑 茫酒焊档废.
	//TheTM.SetTimer(DeferredSetUseSectionMergedMeshTH, FTimerDelegate::CreateUObject(this, &ABladeIIPlayer::SetUseSectionMergedMesh, bUseSectionMerged), 0.1f, false);
}

USkeletalMesh* ABladeIIPlayer::GetMergedMesh(bool bUseSectionMerged)
{
	USkeletalMesh* ResultMesh = nullptr;
	//SetupComponentsForParts(bUseSectionMerged, &ResultMesh); // Component 技泼 绝捞 搬苞拱父 境 哗柯促.
	return ResultMesh;
}

void ABladeIIPlayer::SetupMeshesMain()
{
//#if WITH_EDITOR
//	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	// 颇飘 炼钦 矫 促矫 PCClassInfo 甫 鞘夸肺 窍骨肺 PCClassEnum 瘤沥 绝捞 喉风橇赴飘 努贰胶肺 抛胶飘窍绰 版快 SetupComponentsForParts 甫 积帆秦具 窃.
//	const bool bDoSetupComponentsForParts = !(B2GM && B2GM->IsDefaultPawnTestBP());
//#else
//	const bool bDoSetupComponentsForParts = true;
//#endif
//	if (bDoSetupComponentsForParts)
//	{
//#if !UE_BUILD_SHIPPING
//		int32 SetupComponentsCount = 0;
//#endif
//		// 冀记 唱床柳 老馆 葛胆阑 荤侩窍绰 纳捞胶牢单 措何盒狼 版快绰 捞霸 鞘夸且 巴捞促.
//		// 父老 冀记 捍钦 等 葛胆阑 荤侩窍绰 葛靛俊辑 楷免鳖瘤 弊吧 荤侩窍绰 霸 倾侩捞 登搁 捞 扁夯 葛胆捞 鞘夸绝绢龙 荐 乐澜.
//		if (!UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(this))
//		{
//#if !UE_BUILD_SHIPPING
//			++SetupComponentsCount;
//#endif
//			this->SetupComponentsForParts(false);
//		}
//		// 冀记 捍钦 等 葛胆阑 荤侩窍绰 纳捞胶. 某腐磐 腹捞 殿厘窍绰 葛靛 侩.
//		// 捞吧 第俊 阂矾辑 冀记 捍钦 等 葛胆阑 荤侩窍妨搁 捞霸 扁夯 技泼登档废 窃.
//		if (UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this))
//		{
//#if !UE_BUILD_SHIPPING
//			++SetupComponentsCount;
//#endif
//			this->SetupComponentsForParts(true);
//		}
//#if !UE_BUILD_SHIPPING
//		check(SetupComponentsCount > 0); /* 寸楷洒 窍唱绰 乐绢具. */
//#endif
//	}
}

void ABladeIIPlayer::SetupComponentsForParts(bool bMergeSections, USkeletalMesh** OutGetBuiltResultOnly)
{
	//// ABladeIIPlayer::OverrideByPCClassInfo 捞饶..
	//if (GetCharacterDataStore())
	//{
	//	EPCClass PCClassEnum = GetCurrentPlayerClass();
	//	CharacterDataStore->GetEquippedItems(PCClassEnum, CachedEquippedItem);
	//	CharacterDataStore->GetEquippedCostumeItems(PCClassEnum, CachedEquippedItem);
	//	bHasWing = CharacterDataStore->GetCharacterWing(PCClassEnum, CachedWingData);
	//	ICharacterDataStore::GetRenderItem(CharacterDataStore, PCClassEnum, CachedEquippedItem);

	//	if (GetGameMode()->GetB2GameModeType() == EB2GameMode::Scenario)
	//	{
	//		UB2UIDocUICondition* DocCon = UB2UIDocHelper::GetDocUICondition();

	//		// 老馆带傈俊 胶配府 胶诺 酒匆锭! 朝俺 碍力肺 馋促.
	//		// CachedWingData 焊搁 备炼眉 汗荤秦辑 静骨肺 蔼 函炼秦档 酒眶霸烙俊 康氢 救固魔.
	//		if(!DocCon->GetbUserSelectedSkipStory())
	//			CachedWingData.bShouldBeVisible = false;
	//	}

	//	// 馆拜带傈篮 朝俺 救焊烙
	//	if (GetGameMode()->GetB2GameModeType() == EB2GameMode::CounterDungeon)
	//		CachedWingData.bShouldBeVisible = false;
	//}

	//// OutGetBuiltResultOnly 甫 valid 茄 器牢磐肺 持绰 版快 角力肺 Component 鳖瘤 技泼登瘤 臼绊 弊成 搬苞拱父 哗 霖促.
	//SetupComponentsForPartsCommon(CachedEquippedItem, GetWingDataPtrIfValid(), bMergeSections, false, OutGetBuiltResultOnly);
}

void ABladeIIPlayer::SetupComponentsForPartsCustomDataStore(ICharacterDataStore* InCustomDataStore, bool bMergeSections)
{ // 老馆利栏肺绰 GetCharacterDataStore 甫 烹秦 掘绢柳 DataStore 甫 荤侩.. 捞扒 漂荐茄 版快甫 困窃.
	//if (InCustomDataStore)
	//{
	//	EPCClass PCClassEnum = GetCurrentPlayerClass(); // 扁夯 努贰胶父篮 沥眉己阑 酪瘤 臼绊..
	//	TArray<FB2Item> FoundEquipment;
	//	InCustomDataStore->GetEquippedItems(PCClassEnum, FoundEquipment);
	//	InCustomDataStore->GetEquippedCostumeItems(PCClassEnum, FoundEquipment);
	//	FB2Wing FoundWingData;
	//	bool bLocalHasWing = InCustomDataStore->GetCharacterWing(PCClassEnum, FoundWingData);
	//	ICharacterDataStore::GetRenderItem(InCustomDataStore, PCClassEnum, FoundEquipment);

	//	// 捞吧 荤侩窍绰 矫痢俊辑 GameEntryID 啊 狼档客绰 促甫 荐 乐栏骨肺 某教秦 初篮 芭 荤侩窍瘤 富绊 ForceUpdate 秦辑 荤侩.
	//	// 窜 composite mesh 某教捞 厘厚 炼钦鳖瘤 眉农绰 窍骨肺 部牢 搬苞拱捞 唱坷瘤绰 臼阑 波绊 己瓷俊父 救 亮篮 康氢阑 固摹绰 芭.
	//	// 咯扁 搬苞档 某教窍妨搁 捞率栏肺 泅犁 炼钦 措惑狼 GameEntryID 甫 逞败临 荐 乐绢具 窃.
	//	SetupComponentsForPartsCommon(FoundEquipment, bLocalHasWing ? &FoundWingData : NULL, bMergeSections, true);
	//}
}

void ABladeIIPlayer::SetupComponentsForPartsCommon(const TArray<FB2Item>& EquippedItems, const FB2Wing* InWingData, bool bMergeSections, bool bForceUpdate, class USkeletalMesh** OutGetBuiltResultOnly)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupComponentsForPartsCommon"));

	//// ABladeIIPlayer::OverrideByPCClassInfo 捞饶..

	//UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	//BII_CHECK(B2GI);

	//UB2PCClassInfoBox* PCInfoBox = B2GI ? B2GI->GetPCClassInfoBox() : StaticFindPCClassInfoBox(this);
	//UB2PCClassInfo* MyInfo = PCInfoBox ? PCInfoBox->GetSingleClassInfo(GetCurrentPlayerClass()) : NULL;
	//BII_CHECK(MyInfo);

	//USkeletalMeshComponent* BaseMeshComp = this->GetBaseMesh();
	//BII_CHECK(BaseMeshComp);

	//if (MyInfo && BaseMeshComp)
	//{ // BaseMeshComp 啊 扁夯 技泼捞 等 巴捞扼搁 MyInfo->BaseMeshAsset 客 BaseMeshComp->SkeletalMesh 啊 酒付档 鞍酒具..
	//	
	//	USkeletalMesh* PrebuiltMeshOrResult = NULL;
	//	UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
	//	if (!bForceUpdate)
	//	{ // 捞傈俊 捞固 呼靛秦 初篮 皋浆啊 乐促搁 弊吧 波郴 敬促.
	//		PrebuiltMeshOrResult = MeshCacheManager ?
	//			MeshCacheManager->GetCachedCompositeMesh(GetCurrentPlayerClass(), this, EquippedItems, (InWingData && InWingData->bShouldBeVisible), InWingData, bMergeSections)
	//			: NULL;
	//	}

	//	// Dummy 侩档肺 spawn 窍绰 版快.. InGameOnlyInfo 甫 肺爹窍瘤 臼酒档 登绰 版快啊 乐澜. 捞霸 耽摹啊 农骨肺 弥利拳 瞒盔俊辑.
	//	UB2InGameOnlyPCClassInfo* InGameOnlyInfo = ShouldLoadInGameOnlyInfoForPartSetup() ? MyInfo->GetInGameOnlyInfo(this) : nullptr;

	//	if (UB2PCClassInfo::SetupSKCompForParts(GetCurrentPlayerClass(), this, BaseMeshComp, PrebuiltMeshOrResult, MyInfo->BaseMeshAsset, MyInfo->DefaultPartsInfo,
	//		EquippedItems, InWingData, (InGameOnlyInfo ? InGameOnlyInfo->AnimBPOverride : NULL), false,
	//		bMergeSections,
	//		(OutGetBuiltResultOnly != nullptr) // 捞 版快 呼靛 搬苞 皋浆甫 观栏肺 哗林扁父 窍绊, 角力 哪器惩飘俊 技泼篮 救 窍绰 葛靛肺 牢侥.
	//		))
	//	{
	//		if (MeshCacheManager && !bForceUpdate) // bForceUpdate 牢 版快 荤侩 格利 惑 泅犁 捞 某腐磐啊 啊柳 EntryID 肺绰 某教秦辑绰 救登绰 巴老 荐 乐促. 某教档 规瘤.
	//		{ // MeshMerging 捞 角青登绰 葛靛俊辑狼 搬苞啊 己傍利捞扼搁 CompositeMeshCache 率俊 某教秦 初澜. 郴何利栏肺绰 角力 诀单捞飘啊 登菌阑 锭俊父 某教捞 瞪 巴烙.
	//			MeshCacheManager->SetCachedCompositeMesh(GetCurrentPlayerClass(), this, PrebuiltMeshOrResult, EquippedItems, (InWingData && InWingData->bShouldBeVisible), InWingData, bMergeSections);
	//		}

	//		if (OutGetBuiltResultOnly)
	//		{ // 漂喊茄 鞘夸俊 狼秦 呼靛等 皋浆甫 寇何肺 哗 林绰 版快.
	//			*OutGetBuiltResultOnly = PrebuiltMeshOrResult;
	//		}
	//	}
	//	SetupLODInfoForInGame(); // 楷免 厘搁捞搁 LODInfo 啊 促矫 技泼瞪 巴.
	//}
}

void ABladeIIPlayer::OverrideByBaseBP(TSubclassOf<class ABladeIICharacter> BPClassToOverride)
{
	//ABladeIIPlayer* BPCDO = BPClassToOverride ? Cast<ABladeIIPlayer>(BPClassToOverride->GetDefaultObject()) : NULL;
	//if (BPCDO)
	//{
	//	// BladeIICharacter 率俊 绝带 窍困 哪器惩飘甸.
	//	
	//	//CopyObjectProperties(this->RightFootShadow, BPCDO->RightFootShadow, true, true);
	//	//CopyObjectProperties(this->LeftFootShadow, BPCDO->LeftFootShadow, true, true);
	//	CopyObjectProperties(this->RightWeapon, BPCDO->RightWeapon, true, true);
	//	CopyObjectProperties(this->LeftWeapon, BPCDO->LeftWeapon, true, true);
	//	CopyObjectProperties(this->LightComponent, BPCDO->LightComponent, true, true);
	//	CopyObjectProperties(this->GetTopDownCameraComponent(), BPCDO->GetTopDownCameraComponent(), true, true);
	//	CopyObjectProperties(this->GetCameraBoom(), BPCDO->GetCameraBoom(), true, true);
	//	CopyObjectProperties(this->FloorRingPSC, BPCDO->FloorRingPSC, true, true);
	//}

	//Super::OverrideByBaseBP(BPClassToOverride);
}

FString ABladeIIPlayer::GetTrimmedNameFromPCClassEnum()
{
	// 弊成 CharacterType 栏肺 荤侩秦档 搬惫 鞍篮 搬苞摆瘤父 PCClassInfo 啊 歹 唱阑 芭 鞍促.
	//EPCClass PCClassEnum = UB2PCClassInfo::FromCharacterTypeToPCClass(this->CharacterType);
	//
	//return UB2PCClassInfo::GetTrimmedNameOfPCClassEnum(PCClassEnum);
	return FString("");
}

void ABladeIIPlayer::GetAllTrimmedNameFromPCClassEnum(TArray<FString>& OutResult)
{
	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, EPCCLASS_ENUM_PACKAGE_NAME, true);
	//BII_CHECK(EnumPtr);

	//if (EnumPtr)
	//{
	//	for (int32 EI = 0; EI < GetMaxPCClassNum(); ++EI)
	//	{
	//		FString ClassEnumName = EnumPtr->GetNameStringByIndex(EI);

	//		// 匙捞怪 逢捞 嘎酒具 登绰单 捞霸 撇绢瘤瘤 臼档废 犬牢窍绰 霸 CheckPCClassEnumNamingConvention
	//		if (ClassEnumName.RemoveFromStart(EPCCLASSENUM_COMMON_PREFIX) == true)
	//		{
	//			OutResult.Add(ClassEnumName);
	//		}
	//	}
	//}
}

FString UB2PCClassInfo::GetTrimmedNameOfPCClassEnum(EPCClass InPCClassEnum)
{
	//if (InPCClassEnum != EPCClass::EPC_End)
	//{
	//	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, EPCCLASS_ENUM_PACKAGE_NAME, true);
	//	BII_CHECK(EnumPtr);

	//	if (EnumPtr)
	//	{
	//		FString ClassEnumName = EnumPtr->GetNameStringByIndex(EnumPtr->GetIndexByValue((int32)InPCClassEnum));

	//		// 匙捞怪 逢捞 嘎酒具 登绰单 捞霸 撇绢瘤瘤 臼档废 犬牢窍绰 霸 CheckPCClassEnumNamingConvention
	//		if (ClassEnumName.RemoveFromStart(EPCCLASSENUM_COMMON_PREFIX) == true)
	//		{
	//			return ClassEnumName;
	//		}
	//	}
	//}

	return TEXT("UnknownPC");  // ECharacterType 率苞 被捞 嘎免 鞘夸啊 乐绰 扒 酒丛.
}

FText UB2PCClassInfo::GetLocalizedName()
{
	return BladeIIGetLOCText(B2LOC_CAT_CHARNAME, LocalizedNameKey.ToString());
}

#if WITH_EDITOR
/** PC Part 狼 mesh merge 矫 扁夯 skeleton 苞 家胶甸狼 skeleton 埃俊 ref pose 啊 倔付唱 促弗瘤 眉农秦辑 版绊 剁框. */
void CheckAndWarnForDifferentRefPose(USkeletalMesh* InBaseSKMesh, const TArray<USkeletalMesh*>& CheckMeshes)
{
	if (!InBaseSKMesh){
		return;
	}

	const FReferenceSkeleton& BaseRefSkeleton = InBaseSKMesh->GetRefSkeleton();
	const TArray<FTransform>& BaseRefBonePose = BaseRefSkeleton.GetRefBonePose();
		
	int32 FoundQuestionableCount = 0;
	const float WarnDifferenceStandard = 0.1f; // 捞巴焊促 阂老摹啊 农搁 版绊 措惑

#if !PLATFORM_MAC
	//FString WarnMessageString = FString::Printf(TEXT("SkeletalMesh 炼钦矫 促澜苞 鞍捞 扁霖 荐摹 %.2f (阑)甫 逞绢啊绰 海捞胶客 颇飘 埃 饭欺繁胶 器令 阂老摹 惯斑"), WarnDifferenceStandard);
	FString WarnMessageString = FString::Printf(TEXT("SkeletalMesh  %.2f"), WarnDifferenceStandard);
#endif

	for (int32 CMI = 0; CMI < CheckMeshes.Num(); ++CMI)
	{
		USkeletalMesh* CurrCheckMesh = CheckMeshes[CMI];
		if (!CurrCheckMesh){
			continue;
		}

		const FReferenceSkeleton& CheckRefSkeleton = CurrCheckMesh->GetRefSkeleton();
		const TArray<FTransform>& CheckRefBonePose = CheckRefSkeleton.GetRefBonePose();

		float TotalDifferenceABS = 0.0f; // 捞锅 Check mesh 俊 措秦 ref pose 客 促弗 例措蔼 歹茄 芭
		int TotalCheckCount = 0;

		for (int32 CBPI = 0; CBPI < CheckRefBonePose.Num(); ++CBPI)
		{
			// 泅 CheckMesh 狼 概 bone 付促 BaseMesh 俊辑 秦寸窍绰 bone 狼 ref pose 甫 茫酒 倔付唱 老摹窍绰瘤 眉农

			const FTransform& CurrCheckPose = CheckRefBonePose[CBPI];
			FName CurrCheckBoneName = CheckRefSkeleton.GetBoneName(CBPI);

			int32 BaseBoneIndex = BaseRefSkeleton.FindBoneIndex(CurrCheckBoneName);
			if (BaseBoneIndex >= 0 && BaseBoneIndex < BaseRefBonePose.Num())
			{
				const FTransform& CurrRefBonePose = BaseRefBonePose[BaseBoneIndex];
				TotalDifferenceABS += (CurrCheckPose.GetTranslation() - CurrRefBonePose.GetTranslation()).Size(); // 老窜 translation 瞒捞父 夯促.
				++TotalCheckCount;
			}
		}

		// 捞锅 CheckMesh 俊 措茄 己利
		if (TotalCheckCount > 0)
		{
			float AvgDifferenceABS = TotalDifferenceABS / (float)TotalCheckCount;
			// 捞扒 弊成 剁快绰 肺弊.
			UE_LOG(LogBladeII, Log, TEXT("Reference pose difference of %s : %f"), *(CurrCheckMesh->GetName()), AvgDifferenceABS);

			if (AvgDifferenceABS > 0.1f) // 漂沥 扁霖 捞惑捞搁 皋矫瘤 冠胶 措惑
			{
				++FoundQuestionableCount;

#if !PLATFORM_MAC
				WarnMessageString += FString::Printf(TEXT("%d : %s, 阂老摹 沥档 : %.4f\n"), FoundQuestionableCount, *CurrCheckMesh->GetName(), AvgDifferenceABS);
#endif

			}
		}
	}

#if !PLATFORM_MAC
	if (GIsEditor)
	{
		if (FoundQuestionableCount > 0)
		{
			WarnMessageString += TEXT("\n困 格废俊 乐歹扼档 狼档茄 局聪皋捞记捞 唱柯促搁 公矫秦档 凳せ");

			//FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMessageString));
			// 购啊 秒钦 规侥俊 巩力啊 乐绰 淀.. 老窜 芭浇妨辑 肺弊肺 措眉.
			UE_LOG(LogBladeII, Log, TEXT("%s"), *WarnMessageString);
		}
	}
#endif
}
#endif

int32 GetPCCompositeMtrlID(EPCPart InPartID)
{
	return (InPartID == EPCPart::EPCPT_End) ? INDEX_NONE : (int32)(InPartID); // UB2PCClassInfo::SetupSKCompForParts 俊辑 merge 矫 阿 犁丰甸阑 PartEnum 俊 蝶扼 沥纺窍骨肺 弊措肺 int 某胶泼
}

EItemEquipPlace GetItemEquipPlaceForPCPart(EPCPart InPCPart)
{
	check((int32)EPCPart::EPCPT_Wing + 1 == (int32)EPCPart::EPCPT_End); // 眠啊 浚飘府 初摹瘤 臼霸

	switch (InPCPart)
	{
	case EPCPart::EPCPT_Face: return EItemEquipPlace::EIEP_End; // Face 绰 PCPart 肺父 粮犁窍绰 巴烙. 秦寸窍绰 EquipPlace 绰 绝澜.
	case EPCPart::EPCPT_Head: return EItemEquipPlace::EIEP_Helmet;
	case EPCPart::EPCPT_Body: return EItemEquipPlace::EIEP_BodyArmor;
	case EPCPart::EPCPT_Leg: return EItemEquipPlace::EIEP_Shoes;
	case EPCPart::EPCPT_Arm: return EItemEquipPlace::EIEP_Glove;
	case EPCPart::EPCPT_Weapon: return EItemEquipPlace::EIEP_Weapon;
	case EPCPart::EPCPT_Robe: return EItemEquipPlace::EIEP_End; // 捞巴档 PCPart 肺父. 郴瘤绰 购啊 嘿菌阑 锭 捞扒 冻绢龙 荐档 乐澜.
	case EPCPart::EPCPT_Wing: return EItemEquipPlace::EIEP_End;
	}
	return EItemEquipPlace::EIEP_End;
}

EPCPart GetPCPartForItemEquipPlace(EItemEquipPlace InEquipPlace)
{
	check((int32)EPCPart::EPCPT_Wing + 1 == (int32)EPCPart::EPCPT_End); // 眠啊 浚飘府 初摹瘤 臼霸
	check((int32)EItemEquipPlace::EIEP_Ring + 1 == (int32)EItemEquipPlace::EIEP_End);

	switch (InEquipPlace)
	{
	case EItemEquipPlace::EIEP_Weapon: return EPCPart::EPCPT_Weapon;
	case EItemEquipPlace::EIEP_Helmet: return EPCPart::EPCPT_Head;
	case EItemEquipPlace::EIEP_BodyArmor: return EPCPart::EPCPT_Body;
	case EItemEquipPlace::EIEP_Glove: return EPCPart::EPCPT_Arm;
	case EItemEquipPlace::EIEP_Shoes: return EPCPart::EPCPT_Leg;
	}
	return EPCPart::EPCPT_End; // 唱赣瘤绰 牢霸烙俊辑 pc 俊 炼钦等 3D 葛胆肺 钎泅登瘤绰 臼绰 巴甸.
}

bool IsCostumeEquipPlace(const ECostumeEquipPlace& InCostumeEquipPlace)
{
	bool IsEquipCostume = false;

	switch (InCostumeEquipPlace)
	{
	case ECostumeEquipPlace::ECEP_Weapon:
	case ECostumeEquipPlace::ECEP_Helmet:
	case ECostumeEquipPlace::ECEP_Body:
		IsEquipCostume = true;
		break;

	default:
		break;
	}

	return IsEquipCostume;
}

EPCPart GetExtraAttachPartEnum(EPCPart InMainPart)
{
	if (InMainPart == EPCPart::EPCPT_Body){
		return EPCPart::EPCPT_Robe; // 漂荐 纳捞胶烙. 喊档 厘馒 浇吩捞 乐绰 扒 酒聪瘤父 蝶肺 嘿看促 睹促档 啊瓷秦具 窍绰.
	}
	return EPCPart::EPCPT_End;
}

bool ShouldRemoveOtherPart(EPCClass InPCClass, EPCPart PartToAdd, EPCPart OtherPartToTest)
{ // PartToAdd 啊 乐阑 锭 OtherPartToTest 甫 力芭秦具 窍绰 版快 true 府畔
	// 扁鸥 促弗 版快 积扁搁 眠啊茄促.
	// Fighter 绰 Robe 啊 荤角惑 促弗 侩档扼 Wing 阑 嘿牢促绊 秦辑 绝局瘤 臼绰促.
	//return (InPCClass != EPCClass::EPC_Fighter && PartToAdd == EPCPart::EPCPT_Wing && OtherPartToTest == EPCPart::EPCPT_Robe);

	return PartToAdd == EPCPart::EPCPT_Wing && OtherPartToTest == EPCPart::EPCPT_Robe;	// 笛篮 悼矫俊 赣瘤 阂啊瓷
}

FName GetWingEmissiveParamName()
{
	return FName(TEXT("WingEmissive"));
}

bool HasWingAuraEmissiveTexture(UMaterialInterface* OrgMaterial)
{ // 漂沥 殿鞭 捞惑 朝俺 material 俊 乐绰 Emissive 咆胶媚 颇扼固磐.. 匙捞怪俊 措茄 眉农啊 鞘夸窃.
	UTexture* Dummy = nullptr;
	if (OrgMaterial && OrgMaterial->GetTextureParameterValue(FName(TEXT("1_auraMask")), Dummy))
		return true;

	return false;
}


struct FLocalPartMeshMtrlInfo
{
	FLocalPartMeshMtrlInfo(){
		PartEnum = EPCPart::EPCPT_End;
		CostumeEquipPlace = ECostumeEquipPlace::ECEP_None;
		MeshAsset = NULL;
		MaterialAsset = NULL;
	}
	EPCPart PartEnum;
	ECostumeEquipPlace CostumeEquipPlace;
	USkeletalMesh* MeshAsset;
	UMaterialInterface* MaterialAsset; // Valid only when overridden by iteminfo
};

void CombineCostumePart(UB2PCMeshSectionMergeInfo* SectionMergeInfo, TArray<FLocalPartMeshMtrlInfo>& InParts)
{
	if (SectionMergeInfo == nullptr)
		return;

	TArray<EPCPart> CombineParts;
	TArray<EPCPart> CurrCombineParts;

	for (const FLocalPartMeshMtrlInfo& Part : InParts)
	{
		SectionMergeInfo->GetReplaceCostumeEquipPlace(Part.CostumeEquipPlace, CurrCombineParts);
		//磊扁 磊脚阑 力寇茄 唱赣瘤 钦磨颇飘 PCPart客 1:1 措览捞搁 滑 鞘夸啊 绝澜
		if (CurrCombineParts.Num() > 0)
			CurrCombineParts.RemoveAt(0);

		if (CurrCombineParts.Num() > 0)
			CombineParts.Append(CurrCombineParts);
	}

	InParts.RemoveAll([&CombineParts](const FLocalPartMeshMtrlInfo& LocalPartInfo)
	{
		return CombineParts.Contains(LocalPartInfo.PartEnum);
	});
}

// 捞抚 啊瘤绊 捞吧肺 积己茄 哪器惩飘牢瘤 魄喊且 芭扼 啊瓷茄 蜡聪农窍霸
#define PCPART_EXTRA_ATTACH_FXCOMP_COMMON_PREFIX TEXT("B2ActPCPartXATCHFx")

/** 厘馒 厘厚 merge 且 锭俊 眠啊肺 PSC 积己秦辑 attach 窍绰单俊 荤侩窍扁 困秦 单捞磐甸阑 葛酒初阑 struct. 
 * FCHSKCompAttachParticleSystemInfo 扁夯 扁瓷俊 粱 漂荐茄 芭 怠嘿咯辑 InfoAsset 捞 酒囱 郴何 贸府 侩档肺 荤侩茄促. */
USTRUCT()
struct FPCPartExtraAttachFxSet : public FCHSKCompAttachParticleSystemInfo
{
	FPCPartExtraAttachFxSet()
		: FCHSKCompAttachParticleSystemInfo()
	{
		LoadedAttachPS = NULL;
		ChangeColor = FLinearColor::Transparent;
	}

	FPCPartExtraAttachFxSet& operator = (FCHSKCompAttachParticleSystemInfo& OtherSuperStruct)
	{
		// FCHSKCompAttachParticleSystemInfo 率俊 急攫等 巴父 啊瘤绊 咯扁波 技泼
		this->AttachSocketName = OtherSuperStruct.AttachSocketName;
		this->AttachScale3D = OtherSuperStruct.AttachScale3D;
		this->LocationOffset = OtherSuperStruct.LocationOffset;
		this->RotationOffset = OtherSuperStruct.RotationOffset;
		this->bIgnoreBoneRotation = OtherSuperStruct.bIgnoreBoneRotation;
		this->LoadedAttachPS = OtherSuperStruct.GetParticleSystemAsset(); // 捞扒 惑困窜 备炼眉俊 汲沥等 TAsset 阑 肺爹秦辑 捞率俊 技泼.
		this->ChangeColor = FLinearColor::Transparent;

		return *this;
	}

	virtual ~FPCPartExtraAttachFxSet() {} // This struct has virtual function so need virtual destructor.

	UPROPERTY(Transient)
	FLinearColor ChangeColor;

	UPROPERTY(Transient)
	UParticleSystem* LoadedAttachPS; // Here we just set already loaded ParticleSystem resource, not the TAsset reference of Super class.

	// GetCompCreateName 绰 惑困 努贰胶焊促绰 林肺 咯扁辑狼 鞘夸俊 狼秦 唱柯 芭. 悼老 家南俊 咯返 嘿绰促芭唱 窍搁 眠啊 侥喊磊甫 持绢具..
	virtual FString GetCompCreateName(AActor* InOwner) const override { return LoadedAttachPS ? FString::Printf(TEXT("%s_%s_%s"), PCPART_EXTRA_ATTACH_FXCOMP_COMMON_PREFIX, (InOwner ? *InOwner->GetName() : TEXT("_")), *AttachSocketName.ToString()) : TEXT(""); }
	FORCEINLINE static bool IsPossibleCreatedCompName(const FString& InNameToCheck) { return InNameToCheck.StartsWith(PCPART_EXTRA_ATTACH_FXCOMP_COMMON_PREFIX); }

	// 惑困 努贰胶狼 TAsset 肺爹 措脚 咯扁 铂 荤侩.
	virtual UParticleSystem* GetParticleSystemAsset() override { return LoadedAttachPS; }
};
// Just a simple code block to reduce duplicated code. 
void PCPartExtraAttachFxSetInfoExtractHelper(TArray<FPCPartExtraAttachFxSet>& OutAddedArray, UParticleSystem* InPS, const FString& InAttachSocketName, FLinearColor InChangeColor = FLinearColor::Transparent)
{ // 捞扒 肯傈 扁夯沥焊父 技泼窍绰 芭. FCHSKCompAttachParticleSystemInfo 技泼俊辑 蜡贰等 扒 捞繁 侥栏肺 窍瘤 臼绰促.
	if (InPS)
	{
		FPCPartExtraAttachFxSet NewSet;
		NewSet.LoadedAttachPS = InPS;
		NewSet.AttachSocketName = FName(*InAttachSocketName);
		NewSet.ChangeColor = InChangeColor;
		OutAddedArray.Add(NewSet);
	}
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
/** 漂沥 喊 己 荐狼 厘厚甫 老褒 馒侩且 荐 乐档废 窍绰 摹飘. 寇包父.. */
int32 gForcedPlayerDummyPartGrade = -1; // 1 ~ 6 阑 林搁 荤侩瞪 巴.

TArray<FB2Item> gCheatCostumeItem;
#endif

// 敲饭捞绢 某腐磐 颇飘 备盒俊 蝶扼 skeletal mesh 甫 备己窍绰 皋牢 窃荐烙. BladeIIPlayer 趣篮 肺厚俊 冠酒持篮 SkeletalMeshActor 笛 促 荤侩窍绰 巴阑 狼档.
bool UB2PCClassInfo::SetupSKCompForParts(EPCClass InPCClass, AActor* SKCompOwner, USkeletalMeshComponent* SKCompToSetup,
	USkeletalMesh*& InOutPrebuiltCompositeMesh,
	USkeletalMesh* InBaseMesh,
	const TArray<FPCPartsAssetInfo>& InDefaultParts,
	const TArray<FB2Item>& InEquippedItems,
	const FB2Wing* InWingData,
	UAnimBlueprintGeneratedClass* AnimBPForInGameChar,
	bool bForceUnloadPartResourceAfterMerge,
	bool bInMergeSections,
	bool bBuildAndGetResultMeshOnly // 哪器惩飘俊 技泼 救窍绊 搬苞 皋浆父 哗 林绰 葛靛.
)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2PCClassInfo::SetupSKCompForParts"));
	B2_SCOPED_TIME_LOG(FString::Printf(TEXT("UB2PCClassInfo::SetupSKCompForParts Owner %s, PCClass %d"), SKCompOwner ? *SKCompOwner->GetName() : TEXT("Unknown"), PCClassToInt(InPCClass)));
	
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // 冀记 捍钦 抛胶飘 敲贰弊
	if (UB2PCMeshSectionMergeInfo::ShouldForceUseSectionMerge()) {
		bInMergeSections = true;
	}
	else if (UB2PCMeshSectionMergeInfo::ShouldForceUnuseSectionMerge()) {
		bInMergeSections = false;
	}
#endif

	check(SKCompOwner && SKCompToSetup && InBaseMesh);
	ABladeIIPlayer* CastedB2Player = Cast<ABladeIIPlayer>(SKCompOwner);
	ASkeletalMeshActor* CastedSKActor = Cast<ASkeletalMeshActor>(SKCompOwner);
	check(CastedB2Player || CastedSKActor); // 牢霸烙 某腐磐芭唱 肺厚俊 冠酒持篮 咀磐芭唱.
	// SKCompToSetup 苞 SKCompOwner 客狼 包拌
	check((CastedB2Player && CastedB2Player->GetBaseMesh() == SKCompToSetup) || (CastedSKActor && CastedSKActor->GetSkeletalMeshComponent() == SKCompToSetup));

	UB2ItemInfo* IteminfoTable = StaticFindItemInfo();
	BII_CHECK(IteminfoTable);

	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(SKCompOwner));
	UB2SomeInfo* SomeInfo = B2GI ? B2GI->GetSomeInfo() : NULL;
	BII_CHECK(SomeInfo);

	UB2WingInfo* WingInfoTable = B2GI ? B2GI->GetWingInfo() : StaticFindWingInfo();
	FClientDataStore& ClientDataStore = BladeIIGameImpl::GetClientDataStore();

	// 捞巴甸篮 炼钦 饶俊 力芭窍妨绊.
	TArray<FSingleItemInfoData*> AllLoadedItemInfo;
	FSingleWingInfoData* LoadedWingInfo = NULL; // 场鳖瘤 NULL 老 荐 乐促.

	UB2PCClassInfoBox* PCInfoBox = B2GI ? B2GI->GetPCClassInfoBox() : StaticFindPCClassInfoBox(SKCompOwner);
	// 漂荐 葛靛俊辑 己瓷 犬焊甫 困秦 section merge 甫 角青窍绰 单俊 鞘夸茄 沥焊甫 淬绊 乐绰 info asset.
	UB2PCMeshSectionMergeInfo* SectionMergeInfo = PCInfoBox ? PCInfoBox->GetMeshSectionMergeInfo() : nullptr;
	if (bInMergeSections)
	{
		check(SectionMergeInfo);
		if (!SectionMergeInfo) {
			return false;
		}
	}
	// SectionMerge 矫俊 茄 冀记 救俊辑 UV 康开阑 促福霸 荤侩窍扁 困茄 沥焊. Section merge 甫 救 且 芭搁 鞘夸绝摆瘤.
	FSkelMeshMergeMeshUVTransforms SectionMergeUVTransform;
	
	// 捞傈俊 merge 等 皋浆啊 林绢脸促搁 咯扁辑 窍绰 老篮 腹瘤 臼酒具 茄促. 窜瘤 捞傈俊 父甸菌带 备炼 殿栏肺 牢秦 傈 苞沥阑 胶诺鳖瘤绰 给窍绊.
	const bool bPrebuiltMeshProvided = (InOutPrebuiltCompositeMesh != nullptr);

#if !UE_BUILD_SHIPPING
	extern bool GTest_UseNonCompositedRawPCMesh;
	if (GTest_UseNonCompositedRawPCMesh)
	{ // 扁夯 皋浆 荤侩窍绰 抛胶飘 葛靛
		SKCompToSetup->SetSkeletalMesh(InBaseMesh);
		return false;
	}
#endif
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	/** 漂沥 喊 己 荐狼 厘厚甫 老褒 馒侩且 荐 乐档废 窍绰 摹飘..  
	 摹飘扼辑 弊成 捞 救俊 持绰 巴牢单, 角力 扁瓷俊辑绰 捞 救俊辑 EquippedItems 甫 喊促弗 林狼 绝捞 公累沥 override 秦辑绰 救等促. 
	 老窜 FB2GMPCCompositeMeshCacher 俊辑 厘馒 厘厚俊 蝶扼 积己等 mesh 甫 某教秦辑 静扁 锭巩俊 弊巴何磐 绢边朝 巴烙. */
	if (gForcedPlayerDummyPartGrade >= 1 && gForcedPlayerDummyPartGrade <= FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE)
	{
		TArray<FB2Item> NewDummyPart;
		GetStandardDummyPCPartItemData(InPCClass, gForcedPlayerDummyPartGrade, NewDummyPart);
		TArray<FB2Item>* EquipitemPtrToModify = const_cast<TArray<FB2Item>*>(&InEquippedItems); // 俺惯 曼绊侩 摹飘捞骨肺 促家埃 苞拜茄 规过栏肺..
		if (EquipitemPtrToModify)
		{
			EquipitemPtrToModify->Empty();
			*EquipitemPtrToModify = NewDummyPart;
		}
	}

	if (gCheatCostumeItem.Num() > 0)
	{
		TArray<FB2Item>* EquipitemPtrToModify = const_cast<TArray<FB2Item>*>(&InEquippedItems); // 俺惯 曼绊侩 摹飘捞骨肺 促家埃 苞拜茄 规过栏肺..
		if (EquipitemPtrToModify)
			EquipitemPtrToModify->Append(gCheatCostumeItem);
	}
#endif

	TArray<FPCPartsAssetInfo> DefaultPartsCopy = InDefaultParts; // 老何 夸家啊 档吝俊 力芭瞪 荐档 乐绢辑 汗荤秦辑 敬促.
	TArray<FLocalPartMeshMtrlInfo> AllPartAssets; // 扁夯颇飘俊 泅犁厘馒 酒捞袍 绊妨秦辑 弥辆 府家胶 沥焊 葛篮 巴. 咯扁俊 葛篮 巴甸捞 弥辆 皋浆 炼钦 犁丰肺 荤侩瞪 巴.
	TArray<FPCPartExtraAttachFxSet> AllExtraAttachFx; // 眠啊肺 捞巴档 葛栏霸 瞪 巴.

	TMap< EPCPart, const FB2Item* > ReplaceCostumeItems;
	bool HaveCombineCostume = false;
	
	for (const FB2Item& CurrentItem : InEquippedItems)
	{
		if (CurrentItem.CostumeEquipPlace != ECostumeEquipPlace::ECEP_None)
		{
			TArray<EPCPart> ReplaceParts;
			SectionMergeInfo->GetReplaceCostumeEquipPlace(CurrentItem.CostumeEquipPlace, ReplaceParts);
			//内胶片 茄俺肺 咯矾 何困 钦摹搁 Combine 内胶片 甸绢啊 乐促绊 埃林
			if (ReplaceParts.Num() > 1)
				HaveCombineCostume = true;

			for (EPCPart PCPart : ReplaceParts)
				ReplaceCostumeItems.Add(PCPart, &CurrentItem);
		}
	}

	check(((int32)EPCPart::EPCPT_Face == 0) && ((int32)EPCPart::EPCPT_Wing + 1 == (int32)(EPCPart::EPCPT_End))); // Enum 锅龋啊 葛福霸 官差芭唱 眠啊 亲格 积辨 版快 初摹瘤 臼档废.
//	{
//		B2_SCOPED_TRACK_LOG_L2(TEXT("UB2PCClassInfo::SetupSKCompForParts -- Part Resource Loading"));
//		for (int32 PartIdx = 0; PartIdx < (int32)EPCPart::EPCPT_End; ++PartIdx)
//		{
//			const EPCPart CurrPartEnum = (EPCPart)PartIdx;
//			const FPCPartsAssetInfo* CurrPartDefaultInfo = NULL;
//			for (int32 DPI = 0; DPI < DefaultPartsCopy.Num(); ++DPI)
//			{
//				if (DefaultPartsCopy[DPI].PartEnum == CurrPartEnum)
//				{
//					CurrPartDefaultInfo = &DefaultPartsCopy[DPI];
//					break;
//				}
//			}
//
//			// CurrPartDefaultInfo 啊 NULL 捞芭唱 MeshAsset 捞 NULL 捞绢档 柳青茄促.
//			check(!CurrPartDefaultInfo || CurrPartDefaultInfo->PartEnum != EPCPart::EPCPT_End);
//
//			// 酒贰 笛篮 Equip 捞 等 版快父 蔼捞 甸绢哎 巴烙. 酒聪扼搁 DefaultPartsCopy 俊辑 荤侩.
//			USkeletalMesh* EquippedPartMesh = NULL;
//			UMaterialInterface* EquippedPartMaterial = NULL; // 捞扒 equip 牢 版快扼档 绝阑 荐 乐澜. 鞍篮 皋浆甫 荤侩窍绰 厘厚甫 Material 肺 vary 摹扁 困窃.
//
//			FLocalPartMeshMtrlInfo ExtraAttachInfo; // 捞锅 EquippedPart 俊辑 拱绊 啊绰 眠啊 颇飘啊 乐阑 版快 荤侩瞪 巴.
//			bool IsCostumeOverride = false;
//
//			// CurrPartEnum 俊 秦寸窍绰 厘馒 困摹 ItemInfo 俊 殿废等 巴捞 乐促搁 弊吧 啊廉柯促.
//			EItemEquipPlace RelevantEquipPlace = GetItemEquipPlaceForPCPart(CurrPartEnum);
//			ECostumeEquipPlace CurrCostumeEquipPlace = ECostumeEquipPlace::ECEP_None;
//			
//			if (RelevantEquipPlace != EItemEquipPlace::EIEP_End && IteminfoTable)
//			{
//				const FB2Item* CurrEquipInfo = InEquippedItems.FindByPredicate([RelevantEquipPlace](const FB2Item& item)
//				{
//					return RelevantEquipPlace == item.EquipPlace;
//				});
//
//				const FB2Item** Costume = ReplaceCostumeItems.Find(CurrPartEnum);
//				const FB2Item* CurrCostumeInfo = Costume != nullptr ? *Costume : nullptr;
//				IsCostumeOverride = CurrCostumeInfo != nullptr;
//				if(CurrCostumeInfo != nullptr)
//					CurrCostumeEquipPlace = CurrCostumeInfo->CostumeEquipPlace;
//
//				//Mesh, Material 
//				const FB2Item* ViewItem = CurrCostumeInfo != nullptr ? CurrCostumeInfo : CurrEquipInfo;
//
//				if (ViewItem != nullptr)
//				{
//					FSingleItemInfoData* RelevantInfoData = IteminfoTable->GetInfoData(ViewItem->ItemRefID);
//					AllLoadedItemInfo.Add(RelevantInfoData); // 静绊 抄 淬俊 攫肺爹 窍妨绊 蝶肺 硅凯俊 持绢初澜.
//
//					// 颇飘 府家胶 肺爹篮 角力 mesh merge 甫 秦具 窍绰 版快俊父 角青
//					EquippedPartMesh = (RelevantInfoData && !bPrebuiltMeshProvided) ? RelevantInfoData->GetPCPartMesh(IteminfoTable) : NULL;
//					EquippedPartMaterial = (RelevantInfoData && !bPrebuiltMeshProvided) ? RelevantInfoData->GetPCPartMaterial(IteminfoTable) : NULL;
//
//					//内胶片老订 犬厘阑 救嘿牢促...
//					if (IsCostumeOverride == false)
//					{
//						// 喊档狼 厘馒 浇吩篮 绝绊 促弗 颇飘客 鞍捞 嘿霸登绰 颇飘.
//						EPCPart ExtraAttachPartEnum = GetExtraAttachPartEnum(CurrPartEnum);
//						if (ExtraAttachPartEnum != EPCPart::EPCPT_End && RelevantInfoData)
//						{
//							ExtraAttachInfo.PartEnum = ExtraAttachPartEnum;
//							if (!bPrebuiltMeshProvided) // 颇飘 府家胶 肺爹篮 角力 mesh merge 甫 秦具 窍绰 版快俊父 角青
//							{
//								ExtraAttachInfo.MeshAsset = RelevantInfoData->GetPCPartExtraMesh(IteminfoTable);
//								ExtraAttachInfo.MaterialAsset = RelevantInfoData->GetPCPartExtraMaterial(IteminfoTable);
//							}
//						}
//					}
//
//					// 内胶片 公扁 捞棋飘
//					if (IsCostumeOverride && CurrCostumeInfo != nullptr)
//					{
//						// 漂沥 炼扒阑 父练茄 公扁俊 嘿绰 眠啊 颇萍努 瓤苞. 泅 备炼惑.. 咯扁绰 货肺 merge 窍绰 版快啊 酒聪歹扼档 肺爹阑 茄促.
//						if (CurrCostumeInfo->ItemClass == EItemClass::EIC_CostumeWeapon ||
//							// 炔寸窍霸档 酒捞袍 付胶磐 单捞磐甫 罐瘤 臼篮 惑炔俊辑绰 ItemClass 啊 技泼捞 救登绢 乐促. 蝶扼辑 贸澜俊 6己公扁 捞棋飘啊 救唱坷绰 老捞 乐澜.
//							// InventoryType 苞 RefID 眉农肺 措眉 啊瓷窍档废.
//							(CurrCostumeInfo->InventoryType == EItemInvenType::EIIVT_CostumeWeapon && FItemRefIDHelper::ExtractItemClassFromRefID(CurrCostumeInfo->ItemRefID) == EItemClass::EIC_CostumeWeapon)
//							)
//						{
//
//							AllLoadedItemInfo.Add(RelevantInfoData); // 静绊 抄 淬俊 攫肺爹 窍妨绊 蝶肺 硅凯俊 持绢初澜.
//
//							UParticleSystem* SixStarWeaponEffectPS_L = RelevantInfoData->GetSixStarWeaponEffect(IteminfoTable);
//							UParticleSystem* SixStarWeaponEffectPS_R = RelevantInfoData->GetSixStarWeaponEffect_R(IteminfoTable);
//							if (!SixStarWeaponEffectPS_R) {
//								// _R 滚傈捞 力傍登瘤 臼阑 矫 扁夯 汲沥等 吧肺 谅螟, 快螟 葛滴 技泼. 谅快 悼老茄 屈怕搁 捞犯霸 凳.
//								SixStarWeaponEffectPS_R = SixStarWeaponEffectPS_L;
//							}
//							if (SixStarWeaponEffectPS_L)
//							{
//								// 夸 悸诀俊辑绰 扁夯利牢 沥焊父 啊瘤绊 备己窍绰单.. 趣 眠啊瞪瘤档 葛福摆促.
//								PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffectPS_L, SIXSTAR_WEAPON_EFFECT_SOCKET_L_01);
//							}
//							if (SixStarWeaponEffectPS_R)
//							{
//								PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffectPS_R, SIXSTAR_WEAPON_EFFECT_SOCKET_R_01);
//							}
//
//							UParticleSystem* SixStarWeaponEffect2PS_L = RelevantInfoData->GetSixStarWeaponEffect2(IteminfoTable);
//							UParticleSystem* SixStarWeaponEffect2PS_R = RelevantInfoData->GetSixStarWeaponEffect2_R(IteminfoTable);
//							if (!SixStarWeaponEffect2PS_R) {
//								// _R 滚傈捞 力傍登瘤 臼阑 矫 扁夯 汲沥等 吧肺 谅螟, 快螟 葛滴 技泼. 谅快 悼老茄 屈怕搁 捞犯霸 凳.
//								SixStarWeaponEffect2PS_R = SixStarWeaponEffect2PS_L;
//							}
//
//							if (SixStarWeaponEffect2PS_L)
//							{
//								PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffect2PS_L, SIXSTAR_WEAPON_EFFECT_SOCKET_L_02);
//							}
//							if (SixStarWeaponEffect2PS_R)
//							{
//								PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffect2PS_R, SIXSTAR_WEAPON_EFFECT_SOCKET_R_02);
//							}
//						}
//					}
//				}
//
//				//角力 厘馒等 沥焊啊 乐阑锭父 悼累..
//				if (CurrEquipInfo != nullptr)
//				{
//					// 漂沥 炼扒阑 父练茄 公扁俊 嘿绰 眠啊 颇萍努 瓤苞. 泅 备炼惑.. 咯扁绰 货肺 merge 窍绰 版快啊 酒聪歹扼档 肺爹阑 茄促.
//					if (CurrEquipInfo->ItemClass == EItemClass::EIC_Weapon ||
//						// 炔寸窍霸档 酒捞袍 付胶磐 单捞磐甫 罐瘤 臼篮 惑炔俊辑绰 ItemClass 啊 技泼捞 救登绢 乐促. 蝶扼辑 贸澜俊 6己公扁 捞棋飘啊 救唱坷绰 老捞 乐澜.
//						// InventoryType 苞 RefID 眉农肺 措眉 啊瓷窍档废.
//						(CurrEquipInfo->InventoryType == EItemInvenType::EIIVT_Weapon && FItemRefIDHelper::ExtractItemClassFromRefID(CurrEquipInfo->ItemRefID) == EItemClass::EIC_Weapon)
//						)
//					{
//						if (CurrEquipInfo->StarGrade >= FB2Item::MinimumSurpassStarGrade)
//						{ // 6己 公扁
//							FSingleItemInfoData* RelevantInfoData = IteminfoTable->GetInfoData(CurrEquipInfo->ItemRefID);
//							AllLoadedItemInfo.Add(RelevantInfoData); // 静绊 抄 淬俊 攫肺爹 窍妨绊 蝶肺 硅凯俊 持绢初澜.
//
//							if (RelevantInfoData && !IsCostumeOverride)
//							{
//								{
//									UParticleSystem* SixStarWeaponEffectPS_L = RelevantInfoData->GetSixStarWeaponEffect(IteminfoTable);
//									UParticleSystem* SixStarWeaponEffectPS_R = RelevantInfoData->GetSixStarWeaponEffect_R(IteminfoTable);
//									if (!SixStarWeaponEffectPS_R) {
//										// _R 滚傈捞 力傍登瘤 臼阑 矫 扁夯 汲沥等 吧肺 谅螟, 快螟 葛滴 技泼. 谅快 悼老茄 屈怕搁 捞犯霸 凳.
//										SixStarWeaponEffectPS_R = SixStarWeaponEffectPS_L;
//									}
//									if (SixStarWeaponEffectPS_L)
//									{
//										// 夸 悸诀俊辑绰 扁夯利牢 沥焊父 啊瘤绊 备己窍绰单.. 趣 眠啊瞪瘤档 葛福摆促.
//										PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffectPS_L, SIXSTAR_WEAPON_EFFECT_SOCKET_L_01);
//									}
//									if (SixStarWeaponEffectPS_R)
//									{
//										PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffectPS_R, SIXSTAR_WEAPON_EFFECT_SOCKET_R_01);
//									}
//								}
//								{
//									UParticleSystem* SixStarWeaponEffect2PS_L = RelevantInfoData->GetSixStarWeaponEffect2(IteminfoTable);
//									UParticleSystem* SixStarWeaponEffect2PS_R = RelevantInfoData->GetSixStarWeaponEffect2_R(IteminfoTable);
//									if (!SixStarWeaponEffect2PS_R) {
//										// _R 滚傈捞 力傍登瘤 臼阑 矫 扁夯 汲沥等 吧肺 谅螟, 快螟 葛滴 技泼. 谅快 悼老茄 屈怕搁 捞犯霸 凳.
//										SixStarWeaponEffect2PS_R = SixStarWeaponEffect2PS_L;
//									}
//
//									if (SixStarWeaponEffect2PS_L)
//									{
//										PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffect2PS_L, SIXSTAR_WEAPON_EFFECT_SOCKET_L_02);
//									}
//									if (SixStarWeaponEffect2PS_R)
//									{
//										PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, SixStarWeaponEffect2PS_R, SIXSTAR_WEAPON_EFFECT_SOCKET_R_02);
//									}
//								}
//							}
//						}
//
//						//内胶片苞 惑包绝捞 N碍 捞棋飘绰 厘馒等 公扁俊 蝶扼辑 眠啊秦霖促.
//						// 碍拳俊 蝶弗 公扁 颇萍努 捞棋飘 钎矫
//						if (ClientDataStore.HasEnhanceEffectAuraColor(CurrEquipInfo->EnhanceLevel))
//						{
//							if (SomeInfo)
//							{
//								UParticleSystem* AttachPS = nullptr;
//
//								if (CurrEquipInfo->EnhanceLevel <= SomeInfo->GetUseWeaponExtraEffect_Level40())
//									AttachPS = SomeInfo->GetWeaponExtraEffect_Level30();
//								else if(CurrEquipInfo->EnhanceLevel > SomeInfo->GetUseWeaponExtraEffect_Level40() && CurrEquipInfo->EnhanceLevel <= SomeInfo->GetUseWeaponExtraEffect_Level50())
//								{
//									//某腐磐付促 捞棋飘 促抚
//									if(InPCClass == EPCClass::EPC_Gladiator)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level40_Gl();
//									else if (InPCClass == EPCClass::EPC_Assassin)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level40_As();
//									else if (InPCClass == EPCClass::EPC_Fighter)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level40_Fi();
//									else if (InPCClass == EPCClass::EPC_Wizard)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level40_Wz();
//								}
//								else
//								{
//									//某腐磐付促 捞棋飘 促抚
//									if (InPCClass == EPCClass::EPC_Gladiator)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level50_Gl();
//									else if (InPCClass == EPCClass::EPC_Assassin)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level50_As();
//									else if (InPCClass == EPCClass::EPC_Fighter)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level50_Fi();
//									else if (InPCClass == EPCClass::EPC_Wizard)
//										AttachPS = SomeInfo->GetWeaponExtraEffect_Level50_Wz();
//								}
//							
//
//								if (AttachPS)
//								{
//									FLinearColor EffectColor = ClientDataStore.GetEnhanceEffectAuraColor(CurrEquipInfo->EnhanceLevel);
//									PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, AttachPS, ENHANCED_WEAPON_EFFECT_SOCKET_L, EffectColor);
//									PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, AttachPS, ENHANCED_WEAPON_EFFECT_SOCKET_R, EffectColor);
//								}
//							}
//						}
//					}
//#ifdef EQUIPMENTS_EFFECT 
//					else if (CurrEquipInfo->ItemClass == EItemClass::EIC_BodyArmor
//						|| CurrEquipInfo->ItemClass == EItemClass::EIC_Helmet)
//					{
//						if (CurrEquipInfo->ItemRefID % 10 == 6 ||	// 玫惑
//							CurrEquipInfo->ItemRefID % 10 == 7)		// 康盔
//						{
//							FSingleItemInfoData* RelevantInfoData = IteminfoTable->GetInfoData(CurrEquipInfo->ItemRefID);
//							AllLoadedItemInfo.Add(RelevantInfoData); // 静绊 抄 淬俊 攫肺爹 窍妨绊 蝶肺 硅凯俊 持绢初澜.
//
//							if (RelevantInfoData && !IsCostumeOverride)
//							{
//								UParticleSystem* BodyOrHelmetEffectPS = RelevantInfoData->GetSixStarWeaponEffect(IteminfoTable);
//
//								if (BodyOrHelmetEffectPS)
//								{
//									PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, BodyOrHelmetEffectPS,
//										CurrEquipInfo->ItemClass == EItemClass::EIC_BodyArmor ?
//										BODYAMMOR_EFFECT_SOCKET : HELMET_EFFECT_SOCKET);
//								}
//							}
//						}
//					}
//					else if (CurrEquipInfo->ItemClass == EItemClass::EIC_Glove
//						|| CurrEquipInfo->ItemClass == EItemClass::EIC_Shoes)
//					{
//						if (CurrEquipInfo->ItemRefID % 10 == 6 || // 玫惑
//							CurrEquipInfo->ItemRefID % 10 == 7)	  // 康盔
//						{
//							FSingleItemInfoData* RelevantInfoData = IteminfoTable->GetInfoData(CurrEquipInfo->ItemRefID);
//							AllLoadedItemInfo.Add(RelevantInfoData); // 静绊 抄 淬俊 攫肺爹 窍妨绊 蝶肺 硅凯俊 持绢初澜.
//
//							if (RelevantInfoData && !IsCostumeOverride)
//							{
//
//								UParticleSystem* GloveOrShoesEffectPS_L = RelevantInfoData->GetSixStarWeaponEffect(IteminfoTable);
//								UParticleSystem* GloveOrShoesEffectPS_R = RelevantInfoData->GetSixStarWeaponEffect_R(IteminfoTable);
//								if (!GloveOrShoesEffectPS_R) {
//									GloveOrShoesEffectPS_R = GloveOrShoesEffectPS_L;
//								}
//								if (GloveOrShoesEffectPS_L)
//								{
//									PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, GloveOrShoesEffectPS_L,
//
//										CurrEquipInfo->ItemClass == EItemClass::EIC_Glove ?
//										GLOVE_EFFECT_SOCKET_L_01 : SHOSE_EFFECT_SOCKET_L_01);
//								}
//								if (GloveOrShoesEffectPS_R)
//								{
//									PCPartExtraAttachFxSetInfoExtractHelper(AllExtraAttachFx, GloveOrShoesEffectPS_R,
//
//										CurrEquipInfo->ItemClass == EItemClass::EIC_Glove ?
//										GLOVE_EFFECT_SOCKET_R_01 : SHOSE_EFFECT_SOCKET_R_01);
//								}
//							}
//						}
//					}
//#endif // EQUIPMENTS_EFFECT 
//				}
//			}
//			else if (CurrPartEnum == EPCPart::EPCPT_Wing && InWingData && InWingData->bShouldBeVisible)
//			{// 朝俺绰 促弗 厘厚客绰 喊档肺
//				LoadedWingInfo = WingInfoTable ? WingInfoTable->GetInfoData(*InWingData) : NULL;
//				if (LoadedWingInfo)
//				{
//					if (!bPrebuiltMeshProvided) // 颇飘 府家胶 肺爹篮 角力 mesh merge 甫 秦具 窍绰 版快俊父 角青
//					{
//						EquippedPartMesh = LoadedWingInfo->GetWingMesh();
//						EquippedPartMaterial = LoadedWingInfo->GetMtrlOverride();
//
//#if !UE_BUILD_SHIPPING
//						// 朝俺 赣萍府倔俊 甸绢埃 漂沥 咆胶媚 颇扼固磐 匙捞怪 逢阑 眉农窍绰单 捞霸 6己何磐 乐促..
//						if (InWingData->EvolutionGrade >= 6)
//						{
//							UMaterialInterface* WingMtrlCheck = EquippedPartMaterial;
//							if (!WingMtrlCheck && EquippedPartMesh && EquippedPartMesh->Materials.Num() > 0)
//							{// 荤角 捞 6己 捞惑 朝俺 侩狼 漂荐 赣萍府倔篮 扁夯 技泼捞 酒囱 Override 扼 咯扁肺 坷瘤 臼摆瘤..
//								WingMtrlCheck = EquippedPartMesh->Materials[0].MaterialInterface;
//							}
//							if (WingMtrlCheck)
//							{
//								checkSlow(HasWingAuraEmissiveTexture(EquippedPartMaterial));
//							}
//						}
//#endif
//#if !UE_BUILD_SHIPPING // 锐茄窍霸 朝俺啊 救 父甸绢瘤绰 版快啊 埃趣 乐绰 芭 鞍酒 肺弊甫 谎妨夯促.
//						UE_LOG(LogBladeII, Log, TEXT("WingMesh %s has been added as one of equip part for PCClass %d"), EquippedPartMesh ? *EquippedPartMesh->GetName() : TEXT("InvalidMesh"), PCClassToInt(InPCClass));
//#endif
//					}
//					// 夸扒 酒付 7己 捞惑俊辑父 技泼捞 瞪 巴. 技泼捞 登绢乐绰 版快父
//					for (int32 AEI = 0; AEI < LoadedWingInfo->AttachEffectInfoArray.Num(); ++AEI)
//					{
//						FCHSKCompAttachParticleSystemInfo& ThisAttachEffectInfo = LoadedWingInfo->AttachEffectInfoArray[AEI];
//						UParticleSystem* WingPS = ThisAttachEffectInfo.GetParticleSystemAsset();
//						if (WingPS)
//						{
//							FPCPartExtraAttachFxSet WingAttachPSInfo;
//							WingAttachPSInfo = ThisAttachEffectInfo; // 咯扁 芭摹搁辑 WingAttachPSInfo.LoadedAttachPS 俊 WingPS 啊 技泼瞪 巴.
//							AllExtraAttachFx.Add(WingAttachPSInfo);
//						}
//					}
//				}
//			}
//
//			// 捞锅 颇飘俊辑狼 弥辆利栏肺 荤侩且 府家胶 沥焊. bPrebuiltMeshProvided 狼 版快 角力肺绰 葛滴 null 牢 惑怕老 巴.
//			FLocalPartMeshMtrlInfo ThisPartAssetInfo;
//			ThisPartAssetInfo.PartEnum = CurrPartEnum;
//			ThisPartAssetInfo.MeshAsset = EquippedPartMesh ? EquippedPartMesh : (CurrPartDefaultInfo ? CurrPartDefaultInfo->MeshAsset : NULL);
//			ThisPartAssetInfo.MaterialAsset = EquippedPartMaterial; // Material 篮 叼弃飘肺 沥狼等 巴篮 绝澜.
//			ThisPartAssetInfo.CostumeEquipPlace = CurrCostumeEquipPlace;
//
//			if (ThisPartAssetInfo.MeshAsset) { // 扁夯 府胶飘俊档 绝绊 厘馒 府胶飘俊档 绝栏搁 NULL 老 荐 乐促. 弊繁 版快 弥辆 炼钦 府胶飘俊 器窃矫虐瘤 臼绰促.
//				AllPartAssets.Add(ThisPartAssetInfo);
//			}
//			// 拱沸 咯扁俊 extra attach 啊 眠啊瞪 荐档 乐促.
//			if (ExtraAttachInfo.MeshAsset)
//			{
//				// AllPartAssets 俊 持扁 傈俊, 悼老 PartEnum 狼 扁夯 府胶飘啊 乐促搁 付蛮啊瘤肺 弊扒 哗具瘤.
//				// DefaultParts 俊辑档 哗绊, 捞固 AllPartAssets 俊 甸绢啊 乐阑 荐档 乐栏骨肺 芭扁辑档 哗绊.
//				for (int32 DPI = 0; DPI < DefaultPartsCopy.Num(); ++DPI)
//				{
//					if (DefaultPartsCopy[DPI].PartEnum == ExtraAttachInfo.PartEnum)
//					{
//						DefaultPartsCopy.RemoveAt(DPI);
//						break;
//					}
//				}
//				for (int32 API = 0; API < AllPartAssets.Num(); ++API)
//				{
//					if (AllPartAssets[API].PartEnum == ExtraAttachInfo.PartEnum)
//					{
//						AllPartAssets.RemoveAt(API);
//						break;
//					}
//				}
//				AllPartAssets.Add(ExtraAttachInfo);
//			}
//		}
//	}

	// 鞍捞 粮犁窍搁 救登绰 颇飘甸阑 吧矾晨
	TArray<int32> ToRemovePartIndices;
	for (int32 PIA = 0; PIA < AllPartAssets.Num(); ++PIA)
	{
		const EPCPart PartToAdd = AllPartAssets[PIA].PartEnum;

		for (int32 PIB = 0; PIB < AllPartAssets.Num(); ++PIB)
		{ // 厚背 鉴辑啊 乐绢辑 PIB 甫 PIA+1 何磐 矫累窍瘤绰 给窍绊 鞍篮 版快父 吧矾辰促.
			if (PIA == PIB){
				continue;
			}
			const EPCPart PartToRemoveTest = AllPartAssets[PIB].PartEnum;
			if (ShouldRemoveOtherPart(InPCClass, PartToAdd, PartToRemoveTest)) // 父俊 窍唱.. ShouldRemove 包拌啊 冈捞荤浇贸烦 拱绊拱府绰 侥捞 登绢急 救登摆促..
			{
				ToRemovePartIndices.AddUnique(PIB);
			}
		}
	}
	if (ToRemovePartIndices.Num() > 0)
	{
		ToRemovePartIndices.Sort([](const int32& A, const int32& B) { return A > B; });
		for (int32 ThisRemoveIdx : ToRemovePartIndices) // 第率 牢郸胶何磐 秦辑 力芭
		{
			AllPartAssets.RemoveAt(ThisRemoveIdx);
		}
	}

	//内胶片俊辑 鞍捞 静绰 何困 吧矾晨
	CombineCostumePart(SectionMergeInfo, AllPartAssets);

	if (AllPartAssets.Num() == 0){ // 扁夯颇飘电 厘馒酒捞袍捞电 酒公 技泼 绝澜. 弊措肺 柳青窍搁 MeshMerge 俊辑 角菩 肺弊.
		return false;
	}

	// 夯拜 MeshMerge 肺 逞绢啊扁 傈 material 冀记 ID 甫 犬角洒 窍扁 困秦 府胶飘甫 PartEnum 蝶扼 沥纺. (GetPCCompositeMtrlID)
	struct FLocalPartMeshMtrlInfoSorter
	{
		bool operator()(const FLocalPartMeshMtrlInfo& A, const FLocalPartMeshMtrlInfo& B) const
		{
			return (int32)A.PartEnum < (int32)B.PartEnum;
		}
	};
	AllPartAssets.Sort(FLocalPartMeshMtrlInfoSorter());

//#if WITH_EDITOR && !PLATFORM_MAC
//	// 颇飘 寸 促吝 material 冀记俊 措茄 版绊. MeshMerge 鞍篮 镑俊辑绰 扁瓷利栏肺 啊瓷窍瘤父 泅角利栏肺 牢霸烙俊辑 Material ID 甫 包府窍扁 困秦辑绰 绢妨框.
//	int32 MultiSectionFoundNum = 0;
//	FString MultiSectionPartWarnMsg = TEXT("俺喊 颇飘狼 促吝 冀记篮 泅 矫胶袍俊辑 倾侩登瘤 臼嚼聪促. 促澜 颇飘 俊悸(甸)阑 犬牢秦 林技夸.\n\n");
//	for (const FLocalPartMeshMtrlInfo& ThisAssetInfo : AllPartAssets)
//	{
//		if (ThisAssetInfo.MeshAsset)
//		{
//			FSkeletalMeshResource* ThisMeshResource = ThisAssetInfo.MeshAsset->GetImportedResource();
//			if (ThisMeshResource && ThisMeshResource->LODModels.Num() > 0 && ThisMeshResource->LODModels[0].Sections.Num() > 1)
//			{
//				++MultiSectionFoundNum;
//				MultiSectionPartWarnMsg += FString::Printf(TEXT("%d. %s\n"), MultiSectionFoundNum, *ThisAssetInfo.MeshAsset->GetName());
//			}
//		}
//	}
//	if (MultiSectionFoundNum > 0)
//	{
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(MultiSectionPartWarnMsg));
//	}
//#endif

	bool bResult = false;

	USkeletalMesh* CompositeMesh = InOutPrebuiltCompositeMesh ? InOutPrebuiltCompositeMesh : NewObject<USkeletalMesh>(GetTransientPackage(), NAME_None, RF_Transient);
			
	// 捞扒 Section 捍钦阑 窍绰 版快 盔贰 赣萍府倔狼 颇扼固磐甫 曼绊窍扁 困秦 葛酒滴绰 芭
	TMap<EPCPart, UMaterialInterface*> NonMergedMaterialsBackup;

//	if (!bPrebuiltMeshProvided)
//	{
//		// 夯拜 FSkeletalMeshMerge 扁瓷阑 荤侩窍扁 困茄 胶跑. 角力 mesh merge 甫 窍瘤 臼绰 惑炔俊急 鞘夸绝促. (bPrebuiltMeshProvided)
//
//		TArray<USkeletalMesh*> SourcePartMeshes;
//		TArray<FSkelMeshMergeSectionMapping> ForceSectionMapping;
//		TArray<FSkelMeshMergeMtrlOverrideMapping> SrcMtrlOverrides;
//		
//		// FSkeletalMeshMerge 俊辑 荤侩窍扁 困秦 AllPartAssets 俊 葛酒初篮 沥焊甸阑 茄锅 歹 犁备己茄促.
//		int32 ForceSectionID = 0;
//		for (int32 PAI = 0; PAI < AllPartAssets.Num(); ++PAI)
//		{
//			const FLocalPartMeshMtrlInfo& ThisAssetInfo = AllPartAssets[PAI];
//			check(ThisAssetInfo.MeshAsset); // 官肺 惑窜阑 焊搁 捞霸 NULL 牢 版快绰 绝绢具 窃. 肚茄 酒贰俊 material overriding 俊档 康氢捞 乐澜.
//			SourcePartMeshes.Add(ThisAssetInfo.MeshAsset);
//
//			if (bInMergeSections)
//			{ // 冀记 捍钦 矫俊绰 ForceSectionID 尔 利例茄 material 捞尔 冈咯林搁 MeshMerge 率俊辑 舅酒辑 钦媚龙 巴烙.
//				// 酒囱 版快绰 ForceSectionID 绰 鉴瞒利栏肺 刘啊.
//				ForceSectionID = SectionMergeInfo->GetMergedSectionID(HaveCombineCostume, ThisAssetInfo.PartEnum);
//
//				// 弊府绊 咯矾 冀记阑 窍唱狼 material 救俊 持栏搁辑 鞘夸秦瘤绰 UV 谅钎 offset 沥焊档 积己.
//				TArray<FTransform> ThisPartUVTranform;
//				SectionMergeUVTransform.UVTransformsPerMesh.Add(ThisPartUVTranform);
//				check(SectionMergeUVTransform.UVTransformsPerMesh.Num() == PAI + 1);
//
//				// Merge 茄 饶狼 material parameter 技泼俊 曼绊窍扁 困茄 盔夯 material 某教. 父老 null 捞 甸绢啊搁 角力肺 购啊 肋给等 芭摆瘤.
//				NonMergedMaterialsBackup.Add(ThisAssetInfo.PartEnum,
//					ThisAssetInfo.MaterialAsset ? ThisAssetInfo.MaterialAsset :
//					(ThisAssetInfo.MeshAsset->Materials.Num() > 0 ? ThisAssetInfo.MeshAsset->Materials[0].MaterialInterface : nullptr));
//			}
//
//			FSkeletalMeshResource* ThisMeshResource = ThisAssetInfo.MeshAsset->GetImportedResource();
//			if (ThisMeshResource && ThisMeshResource->LODModels.Num() > 0)
//			{
//				FSkelMeshMergeSectionMapping NewSectionMap;
//				FSkelMeshMergeMtrlOverrideMapping NewMtrlMap; // 拱沸 唱吝俊 section ID 立辟且 鞘夸 绝捞 泅 矫痢俊辑 官肺 material override 甫..
//				for (int32 SI = 0; SI < ThisMeshResource->LODModels[0].Sections.Num(); ++SI) // ForceSectionMapping 篮 LODModel 0 锅 扁霖栏肺 窍搁 凳.
//				{
//					NewSectionMap.SectionIDs.Add(ForceSectionID);
//
//					// 冀记阑 钦摹霸 登搁 颇飘喊 冀记阑 钦媚辑 荤侩窍扁 困秦 霖厚等 喊档狼 material 阑 荤侩茄促. 鞍篮 冀记捞搁 悼老茄 material 阑 荤侩秦具 窃.
//					// MergeSections 俊辑档 咯傈洒 null 老 荐 乐促. 弊繁 版快绰 盔贰 material 阑 静绰 扒单 咯傈洒 疙矫利栏肺 窜老 冀记阑 备己窍绰 颇飘啊 乐栏搁 弊犯霸 瞪 淀
//					UMaterialInterface* MergedSectionMaterial = bInMergeSections ? SectionMergeInfo->GetMergedSectionMaterial(HaveCombineCostume, ThisAssetInfo.PartEnum) : nullptr;
//
//					if (bInMergeSections && SectionMergeUVTransform.UVTransformsPerMesh.IsValidIndex(PAI))
//					{
//						TArray<FTransform>& ThisPartUVTransformInfo = SectionMergeUVTransform.UVTransformsPerMesh[PAI];
//
//						FVector2D PartUVOffset = SectionMergeInfo->GetPartUVOffset(HaveCombineCostume, ThisAssetInfo.PartEnum);
//						FTransform PartUVTransform(
//							FRotator(0.0f, 0.0f, 0.0f),
//							FVector(PartUVOffset.X, PartUVOffset.Y, 0.0f),
//							FVector(0.5f, 0.5f, 1.0f) // 胶纳老篮 蝶肺 汲沥 救 窍绊 扁夯 啊沥栏肺 咯扁辑 持绰促.
//						);
//						// 捞 硅凯篮 阿 夸家啊 Texture 谅钎 牢郸胶俊 秦寸窍绰 巴牢单 咯扁俊 持绢霖 俺荐 捞惑狼 咆胶媚 谅钎甫 静霸 登搁 购啊 捞惑秦龙搏单 冀记 merge 登绰 葛靛甫 夯 饶俊唱 舅霸 抄促搁.. 褥..
//						ThisPartUVTransformInfo.Add(PartUVTransform);
//						ThisPartUVTransformInfo.Add(PartUVTransform); // 弊贰辑 趣 葛福聪 窍唱 歹 持绢林磊. 捞 捞惑 吧副 老篮 绝摆瘤..
//					}
//
//					NewMtrlMap.SectionMatOverrides.Add(
//						(bInMergeSections && MergedSectionMaterial) ? MergedSectionMaterial :
//						ThisAssetInfo.MaterialAsset // 颇飘狼 扁夯 material override. 捞扒 NULL 老 荐档 乐促.
//					);
//
//					if (!bInMergeSections) // 冀记 钦摹绰 巴捞 酒囱 版快狼 鉴瞒利牢 Section ID.
//					{
//						++ForceSectionID;
//					}
//				}
//
//				ForceSectionMapping.Add(NewSectionMap);
//				SrcMtrlOverrides.Add(NewMtrlMap);
//			}
//		}
//		check(!bInMergeSections || (NonMergedMaterialsBackup.Num() == AllPartAssets.Num()));
//		
//
//#if WITH_EDITOR
//		if (GIsEditor)
//		{/** PC Part 狼 mesh merge 矫 扁夯 skeleton 苞 家胶甸狼 skeleton 埃俊 ref pose 啊 倔付唱 促弗瘤 眉农秦辑 版绊 剁框. */
//			CheckAndWarnForDifferentRefPose(InBaseMesh, SourcePartMeshes);
//		}
//#endif
//		{ 
//			B2_SCOPED_TRACK_LOG_L2(TEXT("UB2PCClassInfo::SetupSKCompForParts -- Actual Merge"));
//
//#if !UE_BUILD_SHIPPING
//			extern bool GCheat_PCPartMergeRemoveExtraBoneInfluence;
//#endif
//
//			//create an instance of the FSkeletalMeshMerge utility
//			// EPCPTCM_SeparateMergedComponent 牢 版快 InBaseMesh 狼 胶奶饭沛 阑 荤侩窍瘤 臼绰单 捞 版快 弥辆利栏肺 BaseMesh 哪器惩飘狼 局聪皋捞记阑 荤侩且 巴捞扁 锭巩.
//			FSkeletalMeshMerge MeshMergeUtil(CompositeMesh, SourcePartMeshes, ForceSectionMapping, 0, EMeshBufferAccess::Default
//				, bInMergeSections ? &SectionMergeUVTransform : nullptr
//				, InBaseMesh
//				, &SrcMtrlOverrides
//#if !UE_BUILD_SHIPPING
//				, GCheat_PCPartMergeRemoveExtraBoneInfluence
//#else
//				, false
//#endif
//				);
//
//			if (!MeshMergeUtil.DoMerge()) // 角力 mesh merge 悼累. 瘤陛鳖瘤绰 窜瘤 悸诀捞菌阑 挥 せ
//			{
//				if (CompositeMesh){
//					CompositeMesh->ConditionalBeginDestroy();
//					CompositeMesh = NULL;
//				}
//			}
//		}
//	}

	//if (CompositeMesh) // 某教等 巴捞 林绢脸电, Merge 啊 己傍沁电
	//{
	//	B2_SCOPED_TRACK_LOG_L2(TEXT("UB2PCClassInfo::SetupSKCompForParts -- Post Merge Setup"));

	//	CompositeMesh->PhysicsAsset = InBaseMesh->PhysicsAsset;
	//	InOutPrebuiltCompositeMesh = CompositeMesh; // 货肺 积己捞 登菌带 版快扼搁 InOutPrebuiltCompositeMesh 阑 烹秦 搬苞啊 观栏肺档 傈崔瞪 巴.
	//			
	//	// djsong UE4 MERGE 4.14. 4.14 捞饶何磐 惯积茄 老牢单 merge 窍扁 傈狼 盔夯 皋浆 惑怕俊辑 MID 啊 父甸绢廉 哪器惩飘俊 override 等 惑怕肺 咯扁俊 坷绰 版快啊 积辫.
	//	// 捞犯霸 努府绢甫 刚历 秦 林带瘤, 酒聪搁 CompositeMesh->Materials 啊瘤绊 度鞍捞 override 秦 林带瘤.
	//	if (!bBuildAndGetResultMeshOnly)
	//	{
	//		for (int32 MI = 0; MI < SKCompToSetup->GetNumMaterials(); ++MI)
	//		{
	//			SKCompToSetup->SetMaterial(MI, nullptr);
	//		}
	//		SKCompToSetup->B2ScalarMatParameter.Empty(); // 困俊辑 Material 努府绢 窍绰 芭尔 厚搅茄 捞蜡肺 捞巴档 努府绢 窃.
	//	}

	//	USkeletalMesh* OldSKMeshBackup = SKCompToSetup->SkeletalMesh;
	//	if (!bBuildAndGetResultMeshOnly) // 哪器惩飘俊 皋浆 技泼阑 窍瘤 臼绊 搬苞父 哗坷绰 牢磊.
	//	{
	//		SKCompToSetup->SetSkeletalMesh(CompositeMesh);
	//	}

	//	// 弊府绊 冀记阑 捍钦茄 版快绰 赣萍府倔阑 促福霸 荤侩窍骨肺 颇扼固磐 悸诀阑 秦具 茄促.
	//	if (bInMergeSections && SectionMergeInfo && !bPrebuiltMeshProvided) // 角力 mesh merge 甫 茄 版快父
	//	{
	//		SectionMergeInfo->SetupMergedSectionMaterialParams(HaveCombineCostume, CompositeMesh, NonMergedMaterialsBackup);
	//	}

	//	UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
	//	// 趣矫 葛福摆绢辑 捞傈俊 积己沁带 芭 犬角洒 力芭. (促弗 芭扼搁)
	//	if (OldSKMeshBackup && OldSKMeshBackup != InBaseMesh && OldSKMeshBackup != CompositeMesh 
	//		&& OldSKMeshBackup->HasAnyFlags(RF_Transient) // 积己 矫 RF_Transient 甫 霖促.
	//		&& !(MeshCacheManager && MeshCacheManager->IsOneOfCachedCompositeMesh(OldSKMeshBackup))) // 沥侥栏肺 积己秦辑 某教登绢 乐带 芭扼搁 某矫啊 酒流 盎脚捞 救登菌阑 抛聪 咯辨 烹苞窍瘤 给且 荐档 乐绰单 弊贰档 肺厚俊辑 弊犯瘤 臼篮 版快档 蚕 等促.
	//	{ 
	//		// 咯扁辑 炼钦茄 皋浆 府家胶甫 咯矾 咀磐啊 悼矫俊 坊歹傅俊 荤侩窍搁 捞犯霸 疙矫利栏肺 Destroy 窍绰 扒 巩力甫 老栏懦 荐 乐促.
	//		// 弊繁 版快啊 惯积茄促搁 皋浆 府家胶 货肺 炼钦 矫俊 弊吧 荤侩且 咀磐甸俊 葛滴 诀单捞飘甫 秦 林绰 殿 眠啊利牢 贸府 肚茄 鞘夸窍促.
	//		
	//		// 父老 巩力啊 等促搁 咯扁辑 ConditionalBeginDestroy 朝府绰 内靛绰 力芭窍绰 霸 亮阑 波促. 角力肺 荤侩登瘤 臼绰 mesh 坷宏璃飘扼搁 攫府倔 GC 矫胶袍俊 狼秦 舅酒辑 力芭瞪 巴烙.
	//		//OldSKMeshBackup->ConditionalBeginDestroy(); 酒公贰档 弊蹿 亮酒 焊捞瘤 臼澜.. 被捞 捞凡 鞘夸 绝阑 巴 鞍绊..
	//	}
	//	
	//	// 肺厚 SkeletalMeshActor 甫 困茄 巴捞扼搁 捞扒 NULL 捞绢具.
	//	if (AnimBPForInGameChar && !bBuildAndGetResultMeshOnly)
	//	{
	//		SKCompToSetup->SetAnimInstanceClass(AnimBPForInGameChar);
	//	}

	//	bResult = true;
	//}

	if (!bBuildAndGetResultMeshOnly)
	{
		// 捞力 眠啊 哪器惩飘甸阑 嘿老 扒单 货肺 嘿捞扁 傈俊 捞傈俊 嘿牢 霸 乐促搁 力芭. 捞巴档 SkeletalMesh 父 概锅 货肺 积己窍绰 肺厚俊辑唱 利侩瞪 淀..
		DestroyExtraAttachPartComponents(SKCompOwner, SKCompToSetup);

		for (FPCPartExtraAttachFxSet ExtraAttachFx : AllExtraAttachFx)
		{
			UParticleSystemComponent* AttachFx = ExtraAttachFx.CreatePSCHelper(SKCompOwner, SKCompToSetup);

			if (AttachFx)
			{
				// Note : 眠啊 哪器惩飘狼 祸惑蔼阑 函版秦拎具茄促搁 MtrlParamName_Color(TEXT("ColorVariControl")) 捞棋飘俊 怕弊肺 贸府且巴
				//		: SetColorParameter绰 荤侩窍瘤臼绊 SetVectorParameter甫 流立荤侩茄促.  SetColorParameter狼 版快 RGB甫 (255,255,255,0)鳖瘤观俊 瘤沥窍瘤 给窍骨肺 歹 奴蔼阑 盔且 版快 FLinearColor甫 FVector肺 函券秦辑 持绢林绢具窃.
				if (ExtraAttachFx.ChangeColor != FLinearColor::Transparent)
					AttachFx->SetVectorParameter(MtrlParamName_Color, FVector(ExtraAttachFx.ChangeColor));
			}
		}
	}

	// MeshMerge 甫 窍霸 登搁 盔夯 颇飘客绰 喊档狼 府家胶捞骨肺 盔夯 颇飘绰 朝副 荐 乐促. 
	// 窜, 肺拿 某腐磐 厘馒 颇飘狼 版快 咯矾 惑炔俊辑 肚 鞘夸且 荐 乐绢辑 攫力唱 朝府瘤绰 臼绊.. 咯扁肺 逞绢坷绰 牢磊俊 蝶扼. 措眉肺 老矫利 merge 搁 ForceUnload 肺 逞绢棵 巴.
	if (bForceUnloadPartResourceAfterMerge)
	{
		for (FSingleItemInfoData* ThisLoadedItemInfo : AllLoadedItemInfo)
		{
			if (ThisLoadedItemInfo) {
				// 窜, RootSet 栏肺 瘤沥茄 扒 咯扁辑绰 逞绢啊档废 茄促. (bRemoveFromRoot false)
				// 泅犁 肺拿 某腐磐啊 厘馒茄 颇飘老 荐 乐栏骨肺.
				ThisLoadedItemInfo->UnloadOnlyEquipPart(IteminfoTable, false);
			}
		}
	}

	/*if (LoadedWingInfo) { 娟档 喊档肺 Unload 甫 阂矾林绰 备炼肺 哎鳖...
	LoadedWingInfo->UnloadTAssets(WingInfoTable);
	}*/

	//return bResult;
return true;
}

void UB2PCClassInfo::DestroyExtraAttachPartComponents(AActor* SKCompOwner, USkeletalMeshComponent* SKCompToSetup)
{	
	//if (SKCompOwner && SKCompToSetup)
	//{
	//	// 老窜 泅犁绰 FPCPartExtraAttachFxSet 俊 狼茄 ParticleSystemComponent 甸 挥. 眠啊登搁 厚搅窍霸 犬厘
	//	TArray<UActorComponent*> AllPSC = SKCompOwner->GetComponentsByClass(UParticleSystemComponent::StaticClass());
	//	for (int32 PSCI = 0; PSCI < AllPSC.Num(); ++PSCI)
	//	{
	//		UParticleSystemComponent* ThisPSC = Cast<UParticleSystemComponent>(AllPSC[PSCI]);
	//		// 府家胶 袍敲复捞尔 Attach 等 家南档 眉农且 荐 乐绰单 被捞 弊凡 鞘夸绝霸 捞抚阑 概快 蜡聪农窍霸 林绰 吧肺..
	//		if (ThisPSC	&& ThisPSC->GetAttachParent() == SKCompToSetup && FPCPartExtraAttachFxSet::IsPossibleCreatedCompName(ThisPSC->GetName()))
	//		{
	//			ThisPSC->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
	//			ThisPSC->ConditionalBeginDestroy();
	//		}
	//	}
	//}
}

void UB2PCClassInfo::SyncLoadInGameOnlyAssets()
{
	// 咯扁辑 肺爹窍瘤 臼绰 咯鸥 老馆 hard ptr 甸篮 PCClassInfoBox 俊辑 捞霸 肺爹瞪 锭 鞍捞 扁夯栏肺 肺爹等促.
	// 牢霸烙(傈捧)俊辑父 荤侩窍绰 局甸(吝 啊瓷茄 局甸)父 捞繁 侥栏肺 蝶肺 备己秦辑 肺厚俊辑狼 皋葛府甫 例距.

	//GetSomeLazyLoadedClassAssetCommon<UB2InGameOnlyPCClassInfo>(InfoLoadManager, InGameOnlyAssetPtr, &LoadedInGameOnlyAssetPtr, TEXT("PCClassInfo"));

	ConditionalSetLoadedInGameOnlyInfoRootSet();
}

void UB2PCClassInfo::UnloadInGameOnlyAssets()
{
	//// ManagedPCClassUnload 俊 蝶扼 RootSet 老 荐 乐促.
	//if (InGameOnlyAssetPtr.IsValid()){
	//	InGameOnlyAssetPtr->RemoveFromRoot();
	//}
	//if (LoadedInGameOnlyAssetPtr) {
	//	LoadedInGameOnlyAssetPtr->RemoveFromRoot();
	//}

	//UnloadSomeLazyLoadedClassAssetCommon<UB2InGameOnlyPCClassInfo>(InfoLoadManager, InGameOnlyAssetPtr, &LoadedInGameOnlyAssetPtr);

	//if (InGameOnlyAssetAsyncLoadHandle.IsValid()) {
	//	InGameOnlyAssetAsyncLoadHandle->ReleaseHandle();
	//}
}

UB2PCClassInfo::UB2PCClassInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bInGameOnlyAssetAsyncLoadRequestPending = false;

	PCClassEnum = EPCClass::EPC_End;
	
	BaseMeshAsset = NULL;

	LoadedInGameOnlyAssetPtr = NULL;
	//HUDInfoClassOverride = NULL;

	MeshScaleOverride = 0.0f;
	CapsuleHalfHeightOverride = 0.0f;
	CapsuleRadiusOverride = 0.0f;
	DamageNumZOffsetOverride = 0.0f;

	TargetingDistanceOverride = 0.0f;
	MaxDistanceToAllowAutoAttackOverride = 0.0f;
	DetectDistanceOverride = 0.0f;

	bOverride_TargetingMode = false;
	//TargetingModeOverride = ETargetingMode::ETM_Normal;
}

UB2InGameOnlyPCClassInfo* UB2PCClassInfo::GetInGameOnlyInfo(UObject* WorldContextObject)
{
//#if !UE_BUILD_SHIPPING
//	// 咯扁辑 WorldContextObject 绰 霸烙葛靛 眉农侩捞促. NULL 阑 焊郴拎档 累悼篮 窍摆瘤父 啊鞭利 弊矾瘤 富磊.
//	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
//	// 肺厚俊辑 敬促绊 秦辑 累悼 救窍绰 扒 酒聪促. 芭狼 镜葛绝绰 惑寸茄 剧狼 皋葛府客 肺爹 矫埃 殿阑 皑荐秦具 茄促.
//	// 父老 肺厚俊辑 捞吧 荤侩秦具 窍绰 夸备荤亲捞 坷搁 沥富 柳瘤窍霸 绊刮秦 毫具 茄促. 概快 腹篮 巴甸阑 器扁秦具 且 荐 乐促.
//	// 郴瘤绰 沥富 炼缴炼缴 秦 啊搁辑 茄锅俊 努贰胶 窍唱究 厚悼扁 肺爹秦啊哥 荤侩且 荐 乐促搁 秦 杭 荐档.
//	checkSlow(!LobbyGM);
//#endif
//
//	// 捞傈俊 AsyncLoad 夸没阑 朝啡绰单 酒流 肯丰 救 等 惑炔.
//	if (InGameOnlyAssetAsyncLoadHandle.IsValid() && !InGameOnlyAssetPtr.IsValid())
//	{
//		InGameOnlyAssetAsyncLoadHandle->WaitUntilComplete(GENERAL_TASSET_ASYNC_LOAD_TIMEOUT); // Timeout 篮 利寸洒.. 逞绢啊绰 惑炔篮 阜厘捞妨唱
//	}	
//	if(!InGameOnlyAssetPtr.IsValid())
//	{
//		SyncLoadInGameOnlyAssets();
//	}
//	BII_CHECK(InGameOnlyAssetPtr.IsValid()); /* 咯扁鳖瘤 客辑档 invalid 搁 阜厘 */
//	if (InGameOnlyAssetPtr.IsValid())
//	{
//		ConditionalSetLoadedInGameOnlyInfoRootSet();
//
//		LoadedInGameOnlyAssetPtr = InGameOnlyAssetPtr.Get(); // SyncLoadInGameOnlyAssets 俊急 扁夯 某教秦林绰单 Async 肺爹捞 乐绢辑.. 初摹搁 救凳.
//		return Cast<UB2InGameOnlyPCClassInfo>(LoadedInGameOnlyAssetPtr->GetDefaultObject());
//	}
	return NULL;
}

bool UB2PCClassInfo::IsInGameOnlyInfoAtRootSet()
{
	//return (InGameOnlyAssetPtr.IsValid() && InGameOnlyAssetPtr->IsRooted());
	return  true;
}

void UB2PCClassInfo::ReqAsyncLoad(UObject* WorldContextObject, TAsyncLoadPriority InPriority)
{
	// AsyncLoad 肺 牢茄 角力 肺爹 矫埃 窜绵 眉农, 郴瘤绰 捞惑 悼累 矫 抛胶飘. 奖俏呼靛辑档 抛胶飘啊 鞘夸且 荐 乐绢辑 !UE_BUILD_SHIPPING 栏肺 救 窃.
	if (!BladeIIGameImpl::bAllowInfoAssetAsyncLoading) { 
		SyncLoadInGameOnlyAssets();
		return;
	}

	if (!InGameOnlyAssetPtr.IsValid()) // ManagedPCClassUnload 俊 蝶扼 RootSet 老 荐 乐绰单 咯瓢 肺爹捞 登绢 乐促搁 夸没 积帆
	{
		TArray<FSoftObjectPath> ThisReqList;
		ThisReqList.Add(InGameOnlyAssetPtr.ToSoftObjectPath());
		// 咯扁辑 this 甫 WorldContextObject 肺 持绢林绊 酵扁档 茄单 寇何俊辑 利例茄 Async Callback 阑 冈捞扁啊 浇陆 鳖促肺况 焊捞扁档..
		InGameOnlyAssetAsyncLoadHandle = InfoLoadManager.RequestAsyncLoad(ThisReqList, FStreamableDelegate::CreateUObject(this, &UB2PCClassInfo::InGameOnlyAssetAsyncLoadCB), InPriority);
		bInGameOnlyAssetAsyncLoadRequestPending = true; // 夸 敲贰弊绰 迭洒 镜葛啊 绝阑 荐 乐促.. 酒流 肋 隔扼辑
	}
}

void UB2PCClassInfo::InGameOnlyAssetAsyncLoadCB() // 皋牢静饭靛俊辑 阂覆. 芭狼 犬牢 瞒盔栏肺父. 角力 肺爹俊 措茄 waiting 鞍篮 扒 FStreamableHandle 肺.
{
	// 捞 扁夯 啊沥捞 撇府搁 攫府倔 StreamableManager 悼累阑 肋给 颇厩茄 巴捞芭唱 攫府倔 郴何 滚弊芭唱
	/*checkf(InGameOnlyAssetPtr.IsValid() && InGameOnlyAssetAsyncLoadHandle.IsValid() && InGameOnlyAssetAsyncLoadHandle->HasLoadCompleted(),
		TEXT("Wrong assumption or abnormal behavior of PCClassInfo async loading. Call DJSong."));
	bInGameOnlyAssetAsyncLoadRequestPending = false;

	LoadedInGameOnlyAssetPtr = InGameOnlyAssetPtr.Get();

	ConditionalSetLoadedInGameOnlyInfoRootSet();

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (UB2AssetLoader::bEnableDevLog)
	{
		BII_SCREEN_LOG(FString::Printf(TEXT("[PCClassInfo] AsyncLoad request of %s completed."), *InGameOnlyAssetPtr.ToString()),
			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 8.0f);
	}
#endif
#if !UE_BUILD_SHIPPING
	UE_LOG(LogBladeII, Log, TEXT("AsyncLoad request of %s completed"), *InGameOnlyAssetPtr.ToString());
#endif*/
}

void UB2PCClassInfo::ConditionalSetLoadedInGameOnlyInfoRootSet()
{
	//// 饭骇 郴副 锭 unload 救 且 波搁 犬角洒 RootSet 俊 冠酒狄.
	//if (LoadedInGameOnlyAssetPtr && !UB2PCClassInfoBox::ShouldUnloadAllPCClassInfoOnLevelTransition())
	//{
	//	LoadedInGameOnlyAssetPtr->AddToRoot();
	//}
}

TSubclassOf<ABladeIIPlayer> UB2PCClassInfo::GetBaseBPClass(UObject* WorldContextObject)
{
	//// 咯扁急 InGameOnlyAsset 阑 肺爹窍瘤 臼绰促. 肺爹登瘤 臼篮 惑炔俊辑 捞吧 荤侩且 版快啊 积扁搁 犁绊..
	//if (InGameOnlyAssetPtr.IsValid())
	//{
	//	UB2InGameOnlyPCClassInfo* InGameOnlyAssetObj = Cast<UB2InGameOnlyPCClassInfo>(InGameOnlyAssetPtr.Get()->GetDefaultObject());
	//	return InGameOnlyAssetObj ? InGameOnlyAssetObj->GetBaseBPClass(WorldContextObject) : NULL;
	//}

	return NULL;
}

void UB2PCClassInfo::PostLoad()
{
	Super::PostLoad();
}

ECharacterType UB2PCClassInfo::FromPCClassToCharacterType(EPCClass InPCClassEnum)
{
	// 捞犯霸 甘俏秦辑 CharacterType 阑 技泼窍绰 措脚 BladeIIPlayer 率俊 EPCClass 糕滚甫 持绢初阑 荐档..
	switch (InPCClassEnum)
	{
	case EPCClass::EPC_Assassin: return ECharacterType::ECT_Assassin;
	case EPCClass::EPC_Gladiator : return ECharacterType::ECT_Gladiator;
	case EPCClass::EPC_Wizard: return ECharacterType::ECT_Wizard;
	case EPCClass::EPC_Fighter: return ECharacterType::ECT_Fighter;
	}

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

	return ECharacterType::ECT_UnknownPC;
}

EPCClass UB2PCClassInfo::FromCharacterTypeToPCClass(ECharacterType InChTypeEnum)
{
	switch (InChTypeEnum)
	{
	case ECharacterType::ECT_Assassin: return EPCClass::EPC_Assassin;
	case ECharacterType::ECT_Gladiator: return EPCClass::EPC_Gladiator;
	case ECharacterType::ECT_Wizard: return EPCClass::EPC_Wizard;
	case ECharacterType::ECT_Fighter: return EPCClass::EPC_Fighter;
	}

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

	return EPCClass::EPC_End;
}

#if WITH_EDITOR
void UB2PCClassInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

}

bool UB2PCClassInfo::CheckPCClassEnumNamingConvention()
{
	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, EPCCLASS_ENUM_PACKAGE_NAME, true);
	//BII_CHECK(EnumPtr);

	//if(EnumPtr == NULL)
	//{
	//	return false;
	//}

	//for(int32 EI = 0; EI < GetMaxPCClassNum(); ++EI)
	//{
	//	FString ClassEnumName = EnumPtr->GetNameStringByIndex(EI);

	//	// Prefix 犬牢. 芭狼 ABladeIIPlayer::GetTrimmedNameFromPCClassEnum 悼累阑 困秦..
	//	if(ClassEnumName.StartsWith(EPCCLASSENUM_COMMON_PREFIX) == false)
	//	{
	//		return false;
	//	}
	//}

	return true;
}

#endif

// TAsset lazy-loading 阑 荤侩窍绰 Blueprint InfoAsset 狼 坷悼累 巩力甫 沥侥栏肺 秦搬窍扁 傈鳖瘤狼 烙矫 东龙.
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2PCClassInfo::CleanupOnPreSave()
{
	LoadedInGameOnlyAssetPtr = NULL;
}
void UB2PCClassInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// 狼档摹 臼霸 历厘等 饭欺繁胶甫 朝妨霖促.
	CleanupOnPreSave();
}
#endif


/**
 * ======================================================================================================
 * For dynamically merge player character mesh sections
 * 漂荐 葛靛 己瓷 犬焊甫 困茄 敲饭捞绢 某腐磐 mesh section 捍钦 侩 InfoAsset
 * ======================================================================================================
 */

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
int32 UB2PCMeshSectionMergeInfo::SectionMergeTestFlag = 0; // 1捞搁 公炼扒 荤侩 2搁 公炼扒 掺扁, 唱赣瘤绰 霸烙 扁瓷俊 蝶扼
bool UB2PCMeshSectionMergeInfo::ShouldForceUseSectionMerge()
{
	return SectionMergeTestFlag == 1;
}
bool UB2PCMeshSectionMergeInfo::ShouldForceUnuseSectionMerge()
{
	return SectionMergeTestFlag == 2;
}
#endif

/** 冀记 捍钦 皋浆 葛胆 荤侩 包访茄 scalability 函荐. 老何 霸烙葛靛俊辑父 荤侩登绊, 捞巴苞 包拌绝捞 公炼扒 冀记 捍钦 葛胆阑 荤侩窍绰 葛靛档 乐促.
 * 啊鞭利 1 捞 臭篮 荤剧 侩捞 登档废 SectionMergedMeshUse 客 馆措 狼固肺. */
static TAutoConsoleVariable<int32> CVarB2GameSectionPreservedMeshUse(
	TEXT("r.B2GameSectionPreservedMeshUse"),
	1,
	TEXT("If 0, Some game-mode will use section-merged mesh model to save render thread time.\n"),
	ECVF_Scalability 
);

bool UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(UObject* WorldContextObject)
{
	//ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	//IGameRule* GameRuleObj = B2GM ? B2GM->GetGameRule() : nullptr;
	//return GameRuleObj ? 
	//	// 霸烙葛靛 瞒盔俊辑 冀记 捍钦 葛胆阑 碍力窍绰 版快档 乐绊, 可记俊 蝶扼 荤侩窍绰 版快档 乐澜.
	//	(GameRuleObj->UseSectionMergedPCModel() || (GameRuleObj->ConditionalUseSectionMergedPCModel() && (CVarB2GameSectionPreservedMeshUse.GetValueOnGameThread() == 0)))
	//	: false;
	return false;
}
bool UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(UObject* WorldContextObject)
{
	// 楷免俊辑 冀记 捍钦等 吧 荤侩且瘤 咯何牢单 寸厘 荤侩窍瘤绰 臼歹扼档 趣矫扼档 唱吝俊 扁瓷阑 持霸 瞪 锭 绊媚具 窍绰 何盒甸 茫扁 奖档废
//	return (
//		false // 备泅阑 窍霸 登搁 酒付档 荤侩磊啊 急琶茄 可记 汲沥捞 瞪 淀..
//#if 0
//		&& ShouldUseSectionMergeByGameContext(WorldContextObject) // 备泅 甸绢艾阑 矫 捞 炼扒篮 扁夯栏肺 眉农 鞘夸.
//#endif
//		);
	return false;
}

UB2PCMeshSectionMergeInfo::UB2PCMeshSectionMergeInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2PCMeshSectionMergeInfo::PostLoad()
{
	Super::PostLoad();
#if WITH_EDITOR
	if (GIsEditor)
	{
		CheckDataIntegrity();
	}
#endif
}

const FPCMeshSectionMergeGroupData* UB2PCMeshSectionMergeInfo::GetMergeSectionData(bool InHaveCombineCostume, EPCPart InPartEnum) const
{
	const TArray<FPCMeshSectionMergeGroupData>* SelectMergeSctionData = InHaveCombineCostume ? &CombineCostumeMergedSectionData : &MergedSectionData;

	for (const FPCMeshSectionMergeGroupData& MergeGroup : *SelectMergeSctionData)
	{
		for (const FMergeSectionParameter& ParamInfo : MergeGroup.MergeMtrlParameters)
		{
			if (ParamInfo.TargetPart == InPartEnum)
				return &MergeGroup;
		}
	}

	return nullptr;
}

UMaterialInterface* UB2PCMeshSectionMergeInfo::GetMergedSectionMaterial(bool InHaveCombineCostume, EPCPart InPartEnum) const
{
	if(const FPCMeshSectionMergeGroupData* SectionData = GetMergeSectionData(InHaveCombineCostume, InPartEnum))
	{
		return SectionData->SectionMaterial;
	}
	
	return nullptr;
}

const FMergeSectionParameter* UB2PCMeshSectionMergeInfo::GetParamterReplaceInfo(bool InHaveCombineCostume, EPCPart InPartEnum) const
{
	if (const FPCMeshSectionMergeGroupData* SectionData = GetMergeSectionData(InHaveCombineCostume, InPartEnum))
	{
		for (const FMergeSectionParameter& ParamInfo : SectionData->MergeMtrlParameters)
		{
			if (ParamInfo.TargetPart == InPartEnum)
				return &ParamInfo;
		}
	}

	return nullptr;
}

int32 UB2PCMeshSectionMergeInfo::GetMergedSectionID(bool InHaveCombineCostume, EPCPart InPartEnum) const
{
	const FPCMeshSectionMergeGroupData* PartSectionData = GetMergeSectionData(InHaveCombineCostume, InPartEnum);
	return PartSectionData ? PartSectionData->SectionID : 0;
}

FVector2D UB2PCMeshSectionMergeInfo::GetPartUVOffset(bool InHaveCombineCostume, EPCPart InPartEnum) const
{ // 货肺 捍钦等 冀记 救俊辑狼 磊扁 康开 茫扁
	if(const FMergeSectionParameter* Parameter = GetParamterReplaceInfo(InHaveCombineCostume, InPartEnum))
	{
		return Parameter->UVOffset;
	}

	return FVector2D(0.0f, 0.0f);
}

void UB2PCMeshSectionMergeInfo::SetupMergedSectionMaterialParams(bool InHaveCombineCostume, USkeletalMesh* InSectionMergedMesh, TMap<EPCPart, UMaterialInterface*>& OriginalNonMergedMaterials)
{
	//if (!InSectionMergedMesh) {
	//	return;
	//}
	//
	//// SkeletalMesh 狼 Material 阑 悼利栏肺 积己茄 MIC 肺 官层模促.
	//// 咯扁 甸绢柯 SkeletalMesh 档 悼利栏肺 积己茄 坷宏璃飘咯具 秦辑 官层媚档 凳.
	//check(InSectionMergedMesh->HasAnyFlags(RF_Transient));

	//// 扁夯利栏肺 merge 等 冀记阑 勤甸傅窍扁 困秦 漂喊洒 力累等 material 捞 技泼登绢 乐绢具 窍绰单
	//// 悼老茄 备己狼 material 阑 啊瘤绊 概 牢胶畔胶付促 parameter 甫 促福霸 镜 巴捞骨肺 MIC 甫 货肺 积己茄促.
	//for (int32 MI = 0; MI < InSectionMergedMesh->Materials.Num(); ++MI)
	//{
	//	UMaterialInterface* ParentMaterial = InSectionMergedMesh->Materials[MI].MaterialInterface;
	//	UMaterialInstanceConstant* ParentMIC = Cast<UMaterialInstanceConstant>(ParentMaterial);
	//	if (ParentMIC && ParentMIC->HasAnyFlags(RF_Transient)) 
	//	{
	//		// 捞 版快 捞固 捞傈俊 咯扁辑 transient 肺 积己秦辑 技泼茄 MIC 牢 巴烙. 悼老 炼扒俊辑 merge 等 皋浆绰 GameMode 俊 某教秦 初绊 静骨肺 咯扁肺 坷霸 瞪 荐 乐促.
	//		// 捞率 弥利拳甫 芭摹霸 登搁.. 捞固 茄锅 积己秦 初篮 霸 乐栏搁 捞镑栏肺 犁瞒 坷芭唱 且 鞘夸 绝捞 弊措肺 荤侩且 荐 乐档废 窍绰 霸 亮阑 巴.
	//	}
	//	else
	//	{
	//		// MIC 甫 dynamic create 肺 荤侩.. RF_Transient 甫 持绢霖促. MaterialInstanceConstant 率狼 Set~~TransientObjOnly 甫 曼绊.
	//		// 漂喊茄 荤侩牢 祭牢单 MIC 绰 扁夯利栏肺 Editor 俊辑 积己秦辑 荤侩窍绰 巴捞促. 弊矾唱 SkeletalMeshMerge 何磐啊 捞固 弊繁 逢阑 柄绰 扒单 给且 芭 构 乐澜?
	//		UMaterialInstanceConstant* NewMIC = NewObject<UMaterialInstanceConstant>(InSectionMergedMesh, NAME_None, RF_Transient);
	//		if (NewMIC)
	//		{
	//			// 扁粮俊 技泼登绢 乐带 material 阑 parent 肺 茄 MIC 甫 积己秦辑 荤侩.
	//			NewMIC->SetParentTransientObjOnly(ParentMaterial, false); // MIC 甫 dynamic 窍霸 静搁辑 盔贰 MIC 俊绰 绝带 牢磐其捞胶 墩绢初澜
	//			FSkeletalMaterial& OldMaterialSetup = InSectionMergedMesh->Materials[MI];
	//			FSkeletalMaterial NewMaterialSetup(NewMIC, true);
	//			NewMaterialSetup.UVChannelData = OldMaterialSetup.UVChannelData;
	//			InSectionMergedMesh->Materials[MI] = NewMaterialSetup;
	//		}
	//	}
	//}

	//// Merge 救等 盔夯 material 窜困肺 倒搁辑 磊脚俊 秦寸窍绰 merge 等 颇飘狼 section 俊 技泼等 MIC 甫 茫酒 material parameter 甫 傈崔
	//for(TMap<EPCPart, UMaterialInterface*>::TIterator OriginalMtrlIt(OriginalNonMergedMaterials); OriginalMtrlIt; ++OriginalMtrlIt)
	//{
	//	const EPCPart ThisPartEnum = OriginalMtrlIt.Key();
	//	UMaterialInstance* ThisPartOriginalMI = Cast<UMaterialInstance>(OriginalMtrlIt.Value());

	//	// 泅犁 MeshMerge 扁瓷捞 盔窍绰 SectionID 措肺 甘俏捞 力措肺 救登绊 乐绢辑 Mesh 狼 Materials 俊 牢郸胶 持绢辑 啊廉坷搁 盔窍绰 material 阑 嫩 麦绢辑 啊廉棵 荐 绝澜
	//	// 弊吧 绊摹扁 傈鳖瘤绰 何葛 赣萍府倔阑 眉农窍绰 侥栏肺 秦辑 盔窍绰 泅犁 颇飘狼 MIC 甫 啊廉柯促.		
	//	UMaterialInterface* SupposedParentMtrl = GetMergedSectionMaterial(InHaveCombineCostume, ThisPartEnum);
	//	UMaterialInstanceConstant* MergedMIC = nullptr;
	//	// 捞霸 泅犁肺辑绰 救冈腮促绰 芭. MeshMerge 扁瓷捞 粱 部看澜.
	//	//const int32 MergedSectionID = GetMergedSectionID(ThisPartEnum);
	//	//UMaterialInstanceConstant* MergedMIC = Cast<UMaterialInstanceConstant>(InSectionMergedMesh->Materials[MergedSectionID].MaterialInterface);
	//	for (int32 MI = 0; MI < InSectionMergedMesh->Materials.Num(); ++MI)
	//	{
	//		UMaterialInstanceConstant* ThisMIC = Cast<UMaterialInstanceConstant>(InSectionMergedMesh->Materials[MI].MaterialInterface);
	//		if (ThisMIC && ThisMIC->Parent == SupposedParentMtrl) {
	//			MergedMIC = ThisMIC;
	//			break;
	//		}
	//	}

	//	const FMergeSectionParameter* MergeParameter = GetParamterReplaceInfo(InHaveCombineCostume, ThisPartEnum);

	//	if (ThisPartOriginalMI && MergedMIC && MergeParameter)
	//	{
	//		// Set Scalar Parameters
	//		for (const FMaterialParamterReplace& ScalarInfo : MergeParameter->ScalarParams)
	//		{
	//			float ParamValue = 1.f;
	//			const FName OrgParamName = ScalarInfo.OriginalMtrlParam;

	//			if (ThisPartOriginalMI->GetScalarParameterValue(OrgParamName, ParamValue))
	//			{
	//				const FName& ReplaceParamName = ScalarInfo.ReplaceMtrlParam;
	//				MergedMIC->SetScalarParameterValueTransientObjOnly(ReplaceParamName, ParamValue);
	//			}
	//		}

	//		// Set Vector Parameters
	//		for (const FMaterialParamterReplace& VectorInfo : MergeParameter->VectorParams)
	//		{
	//			FLinearColor ParamValue(1.f, 1.f, 1.f, 1.f);
	//			const FName OrgParamName = VectorInfo.OriginalMtrlParam;

	//			if (ThisPartOriginalMI->GetVectorParameterValue(OrgParamName, ParamValue))
	//			{
	//				const FName& ReplaceParamName = VectorInfo.ReplaceMtrlParam;
	//				MergedMIC->SetVectorParameterValueTransientObjOnly(ReplaceParamName, ParamValue);
	//			}
	//		}

	//		// Set Texture Parameters
	//		for (const FMaterialParamterReplace& TextureInfo : MergeParameter->TextureParams)
	//		{
	//			UTexture* ParamValue = nullptr;
	//			const FName OrgParamName = TextureInfo.OriginalMtrlParam;
	//			const FName& ReplaceParamName = TextureInfo.ReplaceMtrlParam;

	//			if (ThisPartOriginalMI->GetTextureParameterValue(OrgParamName, ParamValue))
	//			{
	//				MergedMIC->SetTextureParameterValueTransientObjOnly(ReplaceParamName, ParamValue);
	//			}
	//			else
	//			{
	//				MergedMIC->SetTextureParameterValueTransientObjOnly(ReplaceParamName, nullptr);
	//			}
	//		}

	//		if (ThisPartEnum == EPCPart::EPCPT_Wing && HasWingAuraEmissiveTexture(ThisPartOriginalMI))
	//		{
	//			const FName EmissiveFlag = GetWingEmissiveParamName();
	//			MergedMIC->SetScalarParameterValueTransientObjOnly(EmissiveFlag, 1.f);
	//		}
	//	}
	//}
}

const FCostumeReplaceData * UB2PCMeshSectionMergeInfo::GetCostumeReplaceData(ECostumeEquipPlace InCostumeEquipPlace) const
{
	for (const FCostumeReplaceData& data : CostumeReplaceData)
	{
		if (data.CostumeEquipPlace == InCostumeEquipPlace)
			return &data;
	}
	return nullptr;
}

void UB2PCMeshSectionMergeInfo::GetReplaceCostumeEquipPlace(ECostumeEquipPlace InCostumeEquipPlace, TArray<EPCPart>& OutPCPartReplace)
{
	OutPCPartReplace.SetNum(0);
		
	const FCostumeReplaceData* ReplaceData = GetCostumeReplaceData(InCostumeEquipPlace);
	if (ReplaceData == nullptr)
		return;
	
	OutPCPartReplace.Append(ReplaceData->CombineCostumePCPart);
}

#if WITH_EDITOR
void UB2PCMeshSectionMergeInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	CheckDataIntegrity();
}

void UB2PCMeshSectionMergeInfo::CheckDataIntegrity()
{
	// MergedSectionData 祈笼 矫 肋给瞪 荐 乐绰 何盒甸 眉农.
	int32 ContainingPartConflictFound = 0;
	int32 ExistParts = 0;

	TArray<EPCPart> ValidParts;

	for (const FPCMeshSectionMergeGroupData& MergeGroup : MergedSectionData)
	{
		for (const FMergeSectionParameter& SectionParam : MergeGroup.MergeMtrlParameters)
		{
			ValidParts.AddUnique(SectionParam.TargetPart);
		}
	}

	const int32 PartsMax = static_cast<int32>(EPCPart::EPCPT_End);
	for (int32 PartsIndex = 0; PartsIndex < PartsMax; PartsIndex++)
	{
		EPCPart TargetPart = static_cast<EPCPart>(PartsIndex);
		if (ValidParts.Contains(TargetPart))
		{
			ExistParts++;
		}
		else
		{
			ContainingPartConflictFound++;
		}
	}


	int32 NonExistParts = PartsMax - ExistParts;
	if (ContainingPartConflictFound > 0 || NonExistParts > 0)
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("[PCMeshSectionMergeInfo] 肋给等 技泼捞 惯斑登菌嚼聪促.\r\n辑肺 促弗 弊缝 埃俊 ContainingParts 吝汗 : %d 俺\r\n弊缝 郴俊辑 ContainingParts 肺 汲沥登瘤 臼篮 Part 俊 措茄 技泼 : %d 俺"),
				ContainingPartConflictFound, NonExistParts)
		));
#endif

	}
}
#endif