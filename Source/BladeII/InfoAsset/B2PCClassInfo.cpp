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

//#include "B2SkillInfo.h"
//#include "B2InGameHUDInfo_Player.h"
//#include "BladeIIProjectile.h"
//#include "B2AreaDamageActorBase.h"
#include "BladeIIGameMode.h"
//#include "B2LobbyGameMode.h"
//#include "B2LobbyInventory.h"
//#include "B2ItemInfo.h"
#include "B2PCClassInfoBox.h"
//#include "SkeletalMeshMerge.h"
//#include "Animation/SkeletalMeshActor.h"
#include "B2WingInfo.h"
#include "B2Airport.h"
#include "B2SomeInfo.h"
#include "B2ClientDataStore.h"
#include "BladeIIGameImpl.h"
//#include "B2CompositeMeshCache.h"
#include "FB2ErrorMessage.h"
//#include "B2AssetLoader.h"
//#include "B2UIDocHelper.h"
//#include "B2GameInstance.h"

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
//	// 일반적인 Player spawn 시에는 GameMode 에서 가져옴. CustomPCClassInfo 지정은 특수한 경우
//	UB2PCClassInfo* FinalUseClassInfo = CustomPCClassInfo ? CustomPCClassInfo : InGameMode->GetCurrentPCClassInfo();
//
//#if !WITH_EDITOR
//	check(FinalUseClassInfo); // 블루프린트 클래스로 테스트하는 상황에서는 없을 수 있음..
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
//	// 로비에선 로딩 안될 애들. 몇몇 인게임에서만 로딩하면 되는 애들은 여기서 데이터를 얻어온다.
//	// 인게임이라도 연출에만 사용될 용도의 dummy puppet 은 bNoOverrideByInGameInfo 인자를 주면 InGameOnlyInfo 를 사용하지 않을 것이다.
//	// 특히 게임 클리어 시점에 복잡한 연출이 나오기 전 메모리를 최대한 clear 하는 데에 필요할 수 있다.
//	UB2InGameOnlyPCClassInfo* InGameOnlyInfo = bNoOverrideByInGameInfo ? nullptr : FinalUseClassInfo->GetInGameOnlyInfo(this);
//
//	// PCClassInfo 로 override 하기 전 BaseBP override 부터
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
//	if (FinalUseClassInfo->DamageNumZOffsetOverride != 0.0f) // - 값도 들어갈 수 있게..
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
//			// AnimBP 바꿔치기 전에 SetSkeletalMesh 부터
//			MeshComp->SetSkeletalMesh(FinalUseClassInfo->BaseMeshAsset); // DefaultPart 들이 있으므로 BaseMeshAsset 이 그대로 유지되는 경우는 파트들을 merge 하는 대신 component 로 구성하는 경우가 될 것.
//			if (InGameOnlyInfo && InGameOnlyInfo->AnimBPOverride)
//			{// 이건 결국 MeshComp->SetAnimInstanceClass 임. 모바일 디버깅 과정의 유산
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
//		// SkeletalMeshComponent 에 붙일 추가 컴포넌트들. 여기서 붙이는 애들은 기본적인 것들인데 PC 의 경우 이런 게 있을지 모르겠다. 장착 아이템 등은 따로 들어갈 것 (SetupComponentsForParts).
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
//					NewAttachComp->SetMobility(EComponentMobility::Movable); // StaticMesh 를 Character 에 붙이니 Movable 로
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
//		// InitializeCombatStats 등에서 최종적으로 옵션 등이 가미된 WalkSpeed 를 다시 세팅할 것임. 따라서 초기값 백업해 둠.
//		this->CachedBaseMaxWalkSpeed = MovementComp->MaxWalkSpeed;
//	}
//
//	// 전투 능력 관련 데이터 중 InfoAsset 세팅에 의해 받는 것들
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
//	// ETM_End 를 override condition 으로 할 수도 있지만 editcondition 을 활용해 보자..
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
	// AnimNotify 를 통해 이게 발동이 되는 루트가 있으면 check 에 걸릴 것. 그럴 땐 DeferredSetUseSectionMergedMesh 를 사용.

	// 이전에 빌드해 놓은 게 있다면 그걸 가져와서 쓸 것이고 아니면 새로 빌드
	SetupComponentsForParts(bUseSectionMerged); 

	// 추가 상태 변수가 아직 없는데 아직 딱히 필요도 없지만 확실히 상태 트랙킹을 할 수 있을지 모르겠기도 해서

	GetWorldTimerManager().ClearTimer(DeferredSetUseSectionMergedMeshTH); // Deferred 로 호출했을 수 있다.
}
void ABladeIIPlayer::DeferredSetUseSectionMergedMesh(bool bUseSectionMerged)
{
	FTimerManager& TheTM = GetWorldTimerManager();
	TheTM.ClearTimer(DeferredSetUseSectionMergedMeshTH);
	// 시간은 여기선 적당히 준다. 만일 즉각 적용되어야 된다거나 최소한 바로 다음 틱에 적용되어야 하는 경우라면 다른 방법을 찾아보도록.
	TheTM.SetTimer(DeferredSetUseSectionMergedMeshTH, FTimerDelegate::CreateUObject(this, &ABladeIIPlayer::SetUseSectionMergedMesh, bUseSectionMerged), 0.1f, false);
}

USkeletalMesh* ABladeIIPlayer::GetMergedMesh(bool bUseSectionMerged)
{
	USkeletalMesh* ResultMesh = nullptr;
	SetupComponentsForParts(bUseSectionMerged, &ResultMesh); // Component 세팅 없이 결과물만 쓱 빼온다.
	return ResultMesh;
}

void ABladeIIPlayer::SetupMeshesMain()
{
//#if WITH_EDITOR
//	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
//	// 파트 조합 시 다시 PCClassInfo 를 필요로 하므로 PCClassEnum 지정 없이 블루프린트 클래스로 테스트하는 경우 SetupComponentsForParts 를 생략해야 함.
//	const bool bDoSetupComponentsForParts = !(B2GM && B2GM->IsDefaultPawnTestBP());
//#else
//	const bool bDoSetupComponentsForParts = true;
//#endif
//	if (bDoSetupComponentsForParts)
//	{
//#if !UE_BUILD_SHIPPING
//		int32 SetupComponentsCount = 0;
//#endif
//		// 섹션 나눠진 일반 모델을 사용하는 케이스인데 대부분의 경우는 이게 필요할 것이다.
//		// 만일 섹션 병합 된 모델을 사용하는 모드에서 연출까지 그걸 사용하는 게 허용이 되면 이 기본 모델이 필요없어질 수 있음.
//		if (!UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(this))
//		{
//#if !UE_BUILD_SHIPPING
//			++SetupComponentsCount;
//#endif
//			this->SetupComponentsForParts(false);
//		}
//		// 섹션 병합 된 모델을 사용하는 케이스. 캐릭터 많이 등장하는 모드 용.
//		// 이걸 뒤에 불러서 섹션 병합 된 모델을 사용하려면 이게 기본 세팅되도록 함.
//		if (UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(this))
//		{
//#if !UE_BUILD_SHIPPING
//			++SetupComponentsCount;
//#endif
//			this->SetupComponentsForParts(true);
//		}
//#if !UE_BUILD_SHIPPING
//		check(SetupComponentsCount > 0); /* 당연히 하나는 있어야. */
//#endif
//	}
}

void ABladeIIPlayer::SetupComponentsForParts(bool bMergeSections, USkeletalMesh** OutGetBuiltResultOnly)
{
	//// ABladeIIPlayer::OverrideByPCClassInfo 이후..
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

	//		// 일반던전에 스토리 스킵 아닐때! 날개 강제로 끈다.
	//		// CachedWingData 보면 구조체 복사해서 쓰므로 값 변조해도 아웃게임에 영향 안미침.
	//		if(!DocCon->GetbUserSelectedSkipStory())
	//			CachedWingData.bShouldBeVisible = false;
	//	}

	//	// 반격던전은 날개 안보임
	//	if (GetGameMode()->GetB2GameModeType() == EB2GameMode::CounterDungeon)
	//		CachedWingData.bShouldBeVisible = false;
	//}

	//// OutGetBuiltResultOnly 를 valid 한 포인터로 넣는 경우 실제로 Component 까지 세팅되지 않고 그냥 결과물만 빼 준다.
	//SetupComponentsForPartsCommon(CachedEquippedItem, GetWingDataPtrIfValid(), bMergeSections, false, OutGetBuiltResultOnly);
}

void ABladeIIPlayer::SetupComponentsForPartsCustomDataStore(ICharacterDataStore* InCustomDataStore, bool bMergeSections)
{ // 일반적으로는 GetCharacterDataStore 를 통해 얻어진 DataStore 를 사용.. 이건 특수한 경우를 위함.
	//if (InCustomDataStore)
	//{
	//	EPCClass PCClassEnum = GetCurrentPlayerClass(); // 기본 클래스만은 정체성을 잃지 않고..
	//	TArray<FB2Item> FoundEquipment;
	//	InCustomDataStore->GetEquippedItems(PCClassEnum, FoundEquipment);
	//	InCustomDataStore->GetEquippedCostumeItems(PCClassEnum, FoundEquipment);
	//	FB2Wing FoundWingData;
	//	bool bLocalHasWing = InCustomDataStore->GetCharacterWing(PCClassEnum, FoundWingData);
	//	ICharacterDataStore::GetRenderItem(InCustomDataStore, PCClassEnum, FoundEquipment);

	//	// 이걸 사용하는 시점에서 GameEntryID 가 의도와는 다를 수 있으므로 캐싱해 놓은 거 사용하지 말고 ForceUpdate 해서 사용.
	//	// 단 composite mesh 캐싱이 장비 조합까지 체크는 하므로 꼬인 결과물이 나오지는 않을 꺼고 성능에만 안 좋은 영향을 미치는 거.
	//	// 여기 결과도 캐싱하려면 이쪽으로 현재 조합 대상의 GameEntryID 를 넘겨줄 수 있어야 함.
	//	SetupComponentsForPartsCommon(FoundEquipment, bLocalHasWing ? &FoundWingData : NULL, bMergeSections, true);
	//}
}

void ABladeIIPlayer::SetupComponentsForPartsCommon(const TArray<FB2Item>& EquippedItems, const FB2Wing* InWingData, bool bMergeSections, bool bForceUpdate, class USkeletalMesh** OutGetBuiltResultOnly)
{
	B2_SCOPED_TRACK_LOG(TEXT("ABladeIIPlayer::SetupComponentsForPartsCommon"));

	//// ABladeIIPlayer::OverrideByPCClassInfo 이후..

	//UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(this));
	//BII_CHECK(B2GI);

	//UB2PCClassInfoBox* PCInfoBox = B2GI ? B2GI->GetPCClassInfoBox() : StaticFindPCClassInfoBox(this);
	//UB2PCClassInfo* MyInfo = PCInfoBox ? PCInfoBox->GetSingleClassInfo(GetCurrentPlayerClass()) : NULL;
	//BII_CHECK(MyInfo);

	//USkeletalMeshComponent* BaseMeshComp = this->GetBaseMesh();
	//BII_CHECK(BaseMeshComp);

	//if (MyInfo && BaseMeshComp)
	//{ // BaseMeshComp 가 기본 세팅이 된 것이라면 MyInfo->BaseMeshAsset 와 BaseMeshComp->SkeletalMesh 가 아마도 같아야..
	//	
	//	USkeletalMesh* PrebuiltMeshOrResult = NULL;
	//	UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
	//	if (!bForceUpdate)
	//	{ // 이전에 이미 빌드해 놓은 메쉬가 있다면 그걸 꺼내 쓴다.
	//		PrebuiltMeshOrResult = MeshCacheManager ?
	//			MeshCacheManager->GetCachedCompositeMesh(GetCurrentPlayerClass(), this, EquippedItems, (InWingData && InWingData->bShouldBeVisible), InWingData, bMergeSections)
	//			: NULL;
	//	}

	//	// Dummy 용도로 spawn 하는 경우.. InGameOnlyInfo 를 로딩하지 않아도 되는 경우가 있음. 이게 덩치가 크므로 최적화 차원에서.
	//	UB2InGameOnlyPCClassInfo* InGameOnlyInfo = ShouldLoadInGameOnlyInfoForPartSetup() ? MyInfo->GetInGameOnlyInfo(this) : nullptr;

	//	if (UB2PCClassInfo::SetupSKCompForParts(GetCurrentPlayerClass(), this, BaseMeshComp, PrebuiltMeshOrResult, MyInfo->BaseMeshAsset, MyInfo->DefaultPartsInfo,
	//		EquippedItems, InWingData, (InGameOnlyInfo ? InGameOnlyInfo->AnimBPOverride : NULL), false,
	//		bMergeSections,
	//		(OutGetBuiltResultOnly != nullptr) // 이 경우 빌드 결과 메쉬를 밖으로 빼주기만 하고, 실제 컴포넌트에 세팅은 안 하는 모드로 인식.
	//		))
	//	{
	//		if (MeshCacheManager && !bForceUpdate) // bForceUpdate 인 경우 사용 목적 상 현재 이 캐릭터가 가진 EntryID 로는 캐싱해서는 안되는 것일 수 있다. 캐싱도 방지.
	//		{ // MeshMerging 이 실행되는 모드에서의 결과가 성공적이라면 CompositeMeshCache 쪽에 캐싱해 놓음. 내부적으로는 실제 업데이트가 되었을 때에만 캐싱이 될 것임.
	//			MeshCacheManager->SetCachedCompositeMesh(GetCurrentPlayerClass(), this, PrebuiltMeshOrResult, EquippedItems, (InWingData && InWingData->bShouldBeVisible), InWingData, bMergeSections);
	//		}

	//		if (OutGetBuiltResultOnly)
	//		{ // 특별한 필요에 의해 빌드된 메쉬를 외부로 빼 주는 경우.
	//			*OutGetBuiltResultOnly = PrebuiltMeshOrResult;
	//		}
	//	}
	//	SetupLODInfoForInGame(); // 연출 장면이면 LODInfo 가 다시 세팅될 것.
	//}
}

void ABladeIIPlayer::OverrideByBaseBP(TSubclassOf<class ABladeIICharacter> BPClassToOverride)
{
	//ABladeIIPlayer* BPCDO = BPClassToOverride ? Cast<ABladeIIPlayer>(BPClassToOverride->GetDefaultObject()) : NULL;
	//if (BPCDO)
	//{
	//	// BladeIICharacter 쪽에 없던 하위 컴포넌트들.
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
	// 그냥 CharacterType 으로 사용해도 결국 같은 결과겠지만 PCClassInfo 가 더 나을 거 같다.
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

	//		// 네이밍 룰이 맞아야 되는데 이게 틀어지지 않도록 확인하는 게 CheckPCClassEnumNamingConvention
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

	//		// 네이밍 룰이 맞아야 되는데 이게 틀어지지 않도록 확인하는 게 CheckPCClassEnumNamingConvention
	//		if (ClassEnumName.RemoveFromStart(EPCCLASSENUM_COMMON_PREFIX) == true)
	//		{
	//			return ClassEnumName;
	//		}
	//	}
	//}

	return TEXT("UnknownPC");  // ECharacterType 쪽과 굳이 맞출 필요가 있는 건 아님.
}

FText UB2PCClassInfo::GetLocalizedName()
{
	return BladeIIGetLOCText(B2LOC_CAT_CHARNAME, LocalizedNameKey.ToString());
}

#if WITH_EDITOR
/** PC Part 의 mesh merge 시 기본 skeleton 과 소스들의 skeleton 간에 ref pose 가 얼마나 다른지 체크해서 경고 띄움. */
void CheckAndWarnForDifferentRefPose(USkeletalMesh* InBaseSKMesh, const TArray<USkeletalMesh*>& CheckMeshes)
{
	if (!InBaseSKMesh){
		return;
	}

	const FReferenceSkeleton& BaseRefSkeleton = InBaseSKMesh->GetRefSkeleton();
	const TArray<FTransform>& BaseRefBonePose = BaseRefSkeleton.GetRefBonePose();
		
	int32 FoundQuestionableCount = 0;
	const float WarnDifferenceStandard = 0.1f; // 이것보다 불일치가 크면 경고 대상

#if !PLATFORM_MAC
	//FString WarnMessageString = FString::Printf(TEXT("SkeletalMesh 조합시 다음과 같이 기준 수치 %.2f (을)를 넘어가는 베이스와 파트 간 레퍼런스 포즈 불일치 발견"), WarnDifferenceStandard);
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

		float TotalDifferenceABS = 0.0f; // 이번 Check mesh 에 대해 ref pose 와 다른 절대값 더한 거
		int TotalCheckCount = 0;

		for (int32 CBPI = 0; CBPI < CheckRefBonePose.Num(); ++CBPI)
		{
			// 현 CheckMesh 의 매 bone 마다 BaseMesh 에서 해당하는 bone 의 ref pose 를 찾아 얼마나 일치하는지 체크

			const FTransform& CurrCheckPose = CheckRefBonePose[CBPI];
			FName CurrCheckBoneName = CheckRefSkeleton.GetBoneName(CBPI);

			int32 BaseBoneIndex = BaseRefSkeleton.FindBoneIndex(CurrCheckBoneName);
			if (BaseBoneIndex >= 0 && BaseBoneIndex < BaseRefBonePose.Num())
			{
				const FTransform& CurrRefBonePose = BaseRefBonePose[BaseBoneIndex];
				TotalDifferenceABS += (CurrCheckPose.GetTranslation() - CurrRefBonePose.GetTranslation()).Size(); // 일단 translation 차이만 본다.
				++TotalCheckCount;
			}
		}

		// 이번 CheckMesh 에 대한 성적
		if (TotalCheckCount > 0)
		{
			float AvgDifferenceABS = TotalDifferenceABS / (float)TotalCheckCount;
			// 이건 그냥 띄우는 로그.
			UE_LOG(LogBladeII, Log, TEXT("Reference pose difference of %s : %f"), *(CurrCheckMesh->GetName()), AvgDifferenceABS);

			if (AvgDifferenceABS > 0.1f) // 특정 기준 이상이면 메시지 박스 대상
			{
				++FoundQuestionableCount;

#if !PLATFORM_MAC
				WarnMessageString += FString::Printf(TEXT("%d : %s, 불일치 정도 : %.4f\n"), FoundQuestionableCount, *CurrCheckMesh->GetName(), AvgDifferenceABS);
#endif

			}
		}
	}

#if !PLATFORM_MAC
	if (GIsEditor)
	{
		if (FoundQuestionableCount > 0)
		{
			WarnMessageString += TEXT("\n위 목록에 있더라도 의도한 애니메이션이 나온다면 무시해도 됨ㅋ");

			//FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(WarnMessageString));
			// 뭔가 취합 방식에 문제가 있는 듯.. 일단 거슬려서 로그로 대체.
			UE_LOG(LogBladeII, Log, TEXT("%s"), *WarnMessageString);
		}
	}
#endif
}
#endif

int32 GetPCCompositeMtrlID(EPCPart InPartID)
{
	return (InPartID == EPCPart::EPCPT_End) ? INDEX_NONE : (int32)(InPartID); // UB2PCClassInfo::SetupSKCompForParts 에서 merge 시 각 재료들을 PartEnum 에 따라 정렬하므로 그대로 int 캐스팅
}

EItemEquipPlace GetItemEquipPlaceForPCPart(EPCPart InPCPart)
{
	check((int32)EPCPart::EPCPT_Wing + 1 == (int32)EPCPart::EPCPT_End); // 추가 엔트리 놓치지 않게

	switch (InPCPart)
	{
	case EPCPart::EPCPT_Face: return EItemEquipPlace::EIEP_End; // Face 는 PCPart 로만 존재하는 것임. 해당하는 EquipPlace 는 없음.
	case EPCPart::EPCPT_Head: return EItemEquipPlace::EIEP_Helmet;
	case EPCPart::EPCPT_Body: return EItemEquipPlace::EIEP_BodyArmor;
	case EPCPart::EPCPT_Leg: return EItemEquipPlace::EIEP_Shoes;
	case EPCPart::EPCPT_Arm: return EItemEquipPlace::EIEP_Glove;
	case EPCPart::EPCPT_Weapon: return EItemEquipPlace::EIEP_Weapon;
	case EPCPart::EPCPT_Robe: return EItemEquipPlace::EIEP_End; // 이것도 PCPart 로만. 내지는 뭔가 붙었을 때 이건 떨어질 수도 있음.
	case EPCPart::EPCPT_Wing: return EItemEquipPlace::EIEP_End;
	}
	return EItemEquipPlace::EIEP_End;
}

EPCPart GetPCPartForItemEquipPlace(EItemEquipPlace InEquipPlace)
{
	check((int32)EPCPart::EPCPT_Wing + 1 == (int32)EPCPart::EPCPT_End); // 추가 엔트리 놓치지 않게
	check((int32)EItemEquipPlace::EIEP_Ring + 1 == (int32)EItemEquipPlace::EIEP_End);

	switch (InEquipPlace)
	{
	case EItemEquipPlace::EIEP_Weapon: return EPCPart::EPCPT_Weapon;
	case EItemEquipPlace::EIEP_Helmet: return EPCPart::EPCPT_Head;
	case EItemEquipPlace::EIEP_BodyArmor: return EPCPart::EPCPT_Body;
	case EItemEquipPlace::EIEP_Glove: return EPCPart::EPCPT_Arm;
	case EItemEquipPlace::EIEP_Shoes: return EPCPart::EPCPT_Leg;
	}
	return EPCPart::EPCPT_End; // 나머지는 인게임에서 pc 에 조합된 3D 모델로 표현되지는 않는 것들.
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
		return EPCPart::EPCPT_Robe; // 특수 케이스임. 별도 장착 슬롯이 있는 건 아니지만 따로 붙였다 뗐다도 가능해야 하는.
	}
	return EPCPart::EPCPT_End;
}

bool ShouldRemoveOtherPart(EPCClass InPCClass, EPCPart PartToAdd, EPCPart OtherPartToTest)
{ // PartToAdd 가 있을 때 OtherPartToTest 를 제거해야 하는 경우 true 리턴
	// 기타 다른 경우 생기면 추가한다.
	// Fighter 는 Robe 가 사실상 다른 용도라 Wing 을 붙인다고 해서 없애지 않는다.
	//return (InPCClass != EPCClass::EPC_Fighter && PartToAdd == EPCPart::EPCPT_Wing && OtherPartToTest == EPCPart::EPCPT_Robe);

	return PartToAdd == EPCPart::EPCPT_Wing && OtherPartToTest == EPCPart::EPCPT_Robe;	// 둘은 동시에 머지 불가능
}

FName GetWingEmissiveParamName()
{
	return FName(TEXT("WingEmissive"));
}

bool HasWingAuraEmissiveTexture(UMaterialInterface* OrgMaterial)
{ // 특정 등급 이상 날개 material 에 있는 Emissive 텍스쳐 파라미터.. 네이밍에 대한 체크가 필요함.
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
		//자기 자신을 제외한 나머지 합칠파트 PCPart와 1:1 대응이면 뺄 필요가 없음
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

// 이름 가지고 이걸로 생성한 컴포넌트인지 판별할 거라 가능한 유니크하게
#define PCPART_EXTRA_ATTACH_FXCOMP_COMMON_PREFIX TEXT("B2ActPCPartXATCHFx")

/** 장착 장비 merge 할 때에 추가로 PSC 생성해서 attach 하는데에 사용하기 위해 데이터들을 모아놓을 struct. 
 * FCHSKCompAttachParticleSystemInfo 기본 기능에 좀 특수한 거 덧붙여서 InfoAsset 이 아닌 내부 처리 용도로 사용한다. */
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
		// FCHSKCompAttachParticleSystemInfo 쪽에 선언된 것만 가지고 여기꺼 세팅
		this->AttachSocketName = OtherSuperStruct.AttachSocketName;
		this->AttachScale3D = OtherSuperStruct.AttachScale3D;
		this->LocationOffset = OtherSuperStruct.LocationOffset;
		this->RotationOffset = OtherSuperStruct.RotationOffset;
		this->bIgnoreBoneRotation = OtherSuperStruct.bIgnoreBoneRotation;
		this->LoadedAttachPS = OtherSuperStruct.GetParticleSystemAsset(); // 이건 상위단 구조체에 설정된 TAsset 을 로딩해서 이쪽에 세팅.
		this->ChangeColor = FLinearColor::Transparent;

		return *this;
	}

	virtual ~FPCPartExtraAttachFxSet() {} // This struct has virtual function so need virtual destructor.

	UPROPERTY(Transient)
	FLinearColor ChangeColor;

	UPROPERTY(Transient)
	UParticleSystem* LoadedAttachPS; // Here we just set already loaded ParticleSystem resource, not the TAsset reference of Super class.

	// GetCompCreateName 는 상위 클래스보다는 주로 여기서의 필요에 의해 나온 거. 동일 소켓에 여럿 붙는다거나 하면 추가 식별자를 넣어야..
	virtual FString GetCompCreateName(AActor* InOwner) const override { return LoadedAttachPS ? FString::Printf(TEXT("%s_%s_%s"), PCPART_EXTRA_ATTACH_FXCOMP_COMMON_PREFIX, (InOwner ? *InOwner->GetName() : TEXT("_")), *AttachSocketName.ToString()) : TEXT(""); }
	FORCEINLINE static bool IsPossibleCreatedCompName(const FString& InNameToCheck) { return InNameToCheck.StartsWith(PCPART_EXTRA_ATTACH_FXCOMP_COMMON_PREFIX); }

	// 상위 클래스의 TAsset 로딩 대신 여기 껄 사용.
	virtual UParticleSystem* GetParticleSystemAsset() override { return LoadedAttachPS; }
};
// Just a simple code block to reduce duplicated code. 
void PCPartExtraAttachFxSetInfoExtractHelper(TArray<FPCPartExtraAttachFxSet>& OutAddedArray, UParticleSystem* InPS, const FString& InAttachSocketName, FLinearColor InChangeColor = FLinearColor::Transparent)
{ // 이건 완전 기본정보만 세팅하는 거. FCHSKCompAttachParticleSystemInfo 세팅에서 유래된 건 이런 식으로 하지 않는다.
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
/** 특정 별 성 수의 장비를 일괄 착용할 수 있도록 하는 치트. 외관만.. */
int32 gForcedPlayerDummyPartGrade = -1; // 1 ~ 6 을 주면 사용될 것.

TArray<FB2Item> gCheatCostumeItem;
#endif

// 플레이어 캐릭터 파트 구분에 따라 skeletal mesh 를 구성하는 메인 함수임. BladeIIPlayer 혹은 로비에 박아넣은 SkeletalMeshActor 둘 다 사용하는 것을 의도.
bool UB2PCClassInfo::SetupSKCompForParts(EPCClass InPCClass, AActor* SKCompOwner, USkeletalMeshComponent* SKCompToSetup,
	USkeletalMesh*& InOutPrebuiltCompositeMesh,
	USkeletalMesh* InBaseMesh,
	const TArray<FPCPartsAssetInfo>& InDefaultParts,
	const TArray<FB2Item>& InEquippedItems,
	const FB2Wing* InWingData,
	UAnimBlueprintGeneratedClass* AnimBPForInGameChar,
	bool bForceUnloadPartResourceAfterMerge,
	bool bInMergeSections,
	bool bBuildAndGetResultMeshOnly // 컴포넌트에 세팅 안하고 결과 메쉬만 빼 주는 모드.
)
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2PCClassInfo::SetupSKCompForParts"));
	B2_SCOPED_TIME_LOG(FString::Printf(TEXT("UB2PCClassInfo::SetupSKCompForParts Owner %s, PCClass %d"), SKCompOwner ? *SKCompOwner->GetName() : TEXT("Unknown"), PCClassToInt(InPCClass)));
//	
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // 섹션 병합 테스트 플래그
//	if (UB2PCMeshSectionMergeInfo::ShouldForceUseSectionMerge()) {
//		bInMergeSections = true;
//	}
//	else if (UB2PCMeshSectionMergeInfo::ShouldForceUnuseSectionMerge()) {
//		bInMergeSections = false;
//	}
//#endif
//
//	check(SKCompOwner && SKCompToSetup && InBaseMesh);
//	ABladeIIPlayer* CastedB2Player = Cast<ABladeIIPlayer>(SKCompOwner);
//	ASkeletalMeshActor* CastedSKActor = Cast<ASkeletalMeshActor>(SKCompOwner);
//	check(CastedB2Player || CastedSKActor); // 인게임 캐릭터거나 로비에 박아넣은 액터거나.
//	// SKCompToSetup 과 SKCompOwner 와의 관계
//	check((CastedB2Player && CastedB2Player->GetBaseMesh() == SKCompToSetup) || (CastedSKActor && CastedSKActor->GetSkeletalMeshComponent() == SKCompToSetup));
//
//	UB2ItemInfo* IteminfoTable = StaticFindItemInfo();
//	BII_CHECK(IteminfoTable);
//
//	UB2GameInstance* B2GI = Cast<UB2GameInstance>(UGameplayStatics::GetGameInstance(SKCompOwner));
//	UB2SomeInfo* SomeInfo = B2GI ? B2GI->GetSomeInfo() : NULL;
//	BII_CHECK(SomeInfo);
//
//	UB2WingInfo* WingInfoTable = B2GI ? B2GI->GetWingInfo() : StaticFindWingInfo();
//	FClientDataStore& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
//
//	// 이것들은 조합 후에 제거하려고.
//	TArray<FSingleItemInfoData*> AllLoadedItemInfo;
//	FSingleWingInfoData* LoadedWingInfo = NULL; // 끝까지 NULL 일 수 있다.
//
//	UB2PCClassInfoBox* PCInfoBox = B2GI ? B2GI->GetPCClassInfoBox() : StaticFindPCClassInfoBox(SKCompOwner);
//	// 특수 모드에서 성능 확보를 위해 section merge 를 실행하는 데에 필요한 정보를 담고 있는 info asset.
//	UB2PCMeshSectionMergeInfo* SectionMergeInfo = PCInfoBox ? PCInfoBox->GetMeshSectionMergeInfo() : nullptr;
//	if (bInMergeSections)
//	{
//		check(SectionMergeInfo);
//		if (!SectionMergeInfo) {
//			return false;
//		}
//	}
//	// SectionMerge 시에 한 섹션 안에서 UV 영역을 다르게 사용하기 위한 정보. Section merge 를 안 할 거면 필요없겠지.
//	FSkelMeshMergeUVTransforms SectionMergeUVTransform;
//	
//	// 이전에 merge 된 메쉬가 주어졌다면 여기서 하는 일은 많지 않아야 한다. 단지 이전에 만들었던 구조 등으로 인해 전 과정을 스킵까지는 못하고.
//	const bool bPrebuiltMeshProvided = (InOutPrebuiltCompositeMesh != nullptr);
//
//#if !UE_BUILD_SHIPPING
//	extern bool GTest_UseNonCompositedRawPCMesh;
//	if (GTest_UseNonCompositedRawPCMesh)
//	{ // 기본 메쉬 사용하는 테스트 모드
//		SKCompToSetup->SetSkeletalMesh(InBaseMesh);
//		return false;
//	}
//#endif
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	/** 특정 별 성 수의 장비를 일괄 착용할 수 있도록 하는 치트..  
//	 치트라서 그냥 이 안에 넣는 것인데, 실제 기능에서는 이 안에서 EquippedItems 를 별다른 주의 없이 무작정 override 해서는 안된다. 
//	 일단 FB2GMPCCompositeMeshCacher 에서 장착 장비에 따라 생성된 mesh 를 캐싱해서 쓰기 때문에 그것부터 어긋날 것임. */
//	if (gForcedPlayerDummyPartGrade >= 1 && gForcedPlayerDummyPartGrade <= FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE)
//	{
//		TArray<FB2Item> NewDummyPart;
//		GetStandardDummyPCPartItemData(InPCClass, gForcedPlayerDummyPartGrade, NewDummyPart);
//		TArray<FB2Item>* EquipitemPtrToModify = const_cast<TArray<FB2Item>*>(&InEquippedItems); // 개발 참고용 치트이므로 다소간 과격한 방법으로..
//		if (EquipitemPtrToModify)
//		{
//			EquipitemPtrToModify->Empty();
//			*EquipitemPtrToModify = NewDummyPart;
//		}
//	}
//
//	if (gCheatCostumeItem.Num() > 0)
//	{
//		TArray<FB2Item>* EquipitemPtrToModify = const_cast<TArray<FB2Item>*>(&InEquippedItems); // 개발 참고용 치트이므로 다소간 과격한 방법으로..
//		if (EquipitemPtrToModify)
//			EquipitemPtrToModify->Append(gCheatCostumeItem);
//	}
//#endif
//
//	TArray<FPCPartsAssetInfo> DefaultPartsCopy = InDefaultParts; // 일부 요소가 도중에 제거될 수도 있어서 복사해서 쓴다.
//	TArray<FLocalPartMeshMtrlInfo> AllPartAssets; // 기본파트에 현재장착 아이템 고려해서 최종 리소스 정보 모은 것. 여기에 모은 것들이 최종 메쉬 조합 재료로 사용될 것.
//	TArray<FPCPartExtraAttachFxSet> AllExtraAttachFx; // 추가로 이것도 모으게 될 것.
//
//	TMap< EPCPart, const FB2Item* > ReplaceCostumeItems;
//	bool HaveCombineCostume = false;
//	
//	for (const FB2Item& CurrentItem : InEquippedItems)
//	{
//		if (CurrentItem.CostumeEquipPlace != ECostumeEquipPlace::ECEP_None)
//		{
//			TArray<EPCPart> ReplaceParts;
//			SectionMergeInfo->GetReplaceCostumeEquipPlace(CurrentItem.CostumeEquipPlace, ReplaceParts);
//			//코스튬 한개로 여러 부위 합치면 Combine 코스튬 들어가 있다고 간주
//			if (ReplaceParts.Num() > 1)
//				HaveCombineCostume = true;
//
//			for (EPCPart PCPart : ReplaceParts)
//				ReplaceCostumeItems.Add(PCPart, &CurrentItem);
//		}
//	}
//
//	check(((int32)EPCPart::EPCPT_Face == 0) && ((int32)EPCPart::EPCPT_Wing + 1 == (int32)(EPCPart::EPCPT_End))); // Enum 번호가 모르게 바뀌거나 추가 항목 생길 경우 놓치지 않도록.
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
//			// CurrPartDefaultInfo 가 NULL 이거나 MeshAsset 이 NULL 이어도 진행한다.
//			check(!CurrPartDefaultInfo || CurrPartDefaultInfo->PartEnum != EPCPart::EPCPT_End);
//
//			// 아래 둘은 Equip 이 된 경우만 값이 들어갈 것임. 아니라면 DefaultPartsCopy 에서 사용.
//			USkeletalMesh* EquippedPartMesh = NULL;
//			UMaterialInterface* EquippedPartMaterial = NULL; // 이건 equip 인 경우라도 없을 수 있음. 같은 메쉬를 사용하는 장비를 Material 로 vary 치기 위함.
//
//			FLocalPartMeshMtrlInfo ExtraAttachInfo; // 이번 EquippedPart 에서 물고 가는 추가 파트가 있을 경우 사용될 것.
//			bool IsCostumeOverride = false;
//
//			// CurrPartEnum 에 해당하는 장착 위치 ItemInfo 에 등록된 것이 있다면 그걸 가져온다.
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
//					AllLoadedItemInfo.Add(RelevantInfoData); // 쓰고 난 담에 언로딩 하려고 따로 배열에 넣어놓음.
//
//					// 파트 리소스 로딩은 실제 mesh merge 를 해야 하는 경우에만 실행
//					EquippedPartMesh = (RelevantInfoData && !bPrebuiltMeshProvided) ? RelevantInfoData->GetPCPartMesh(IteminfoTable) : NULL;
//					EquippedPartMaterial = (RelevantInfoData && !bPrebuiltMeshProvided) ? RelevantInfoData->GetPCPartMaterial(IteminfoTable) : NULL;
//
//					//코스튬일땐 확장을 안붙인다...
//					if (IsCostumeOverride == false)
//					{
//						// 별도의 장착 슬롯은 없고 다른 파트와 같이 붙게되는 파트.
//						EPCPart ExtraAttachPartEnum = GetExtraAttachPartEnum(CurrPartEnum);
//						if (ExtraAttachPartEnum != EPCPart::EPCPT_End && RelevantInfoData)
//						{
//							ExtraAttachInfo.PartEnum = ExtraAttachPartEnum;
//							if (!bPrebuiltMeshProvided) // 파트 리소스 로딩은 실제 mesh merge 를 해야 하는 경우에만 실행
//							{
//								ExtraAttachInfo.MeshAsset = RelevantInfoData->GetPCPartExtraMesh(IteminfoTable);
//								ExtraAttachInfo.MaterialAsset = RelevantInfoData->GetPCPartExtraMaterial(IteminfoTable);
//							}
//						}
//					}
//
//					// 코스튬 무기 이펙트
//					if (IsCostumeOverride && CurrCostumeInfo != nullptr)
//					{
//						// 특정 조건을 만족한 무기에 붙는 추가 파티클 효과. 현 구조상.. 여기는 새로 merge 하는 경우가 아니더라도 로딩을 한다.
//						if (CurrCostumeInfo->ItemClass == EItemClass::EIC_CostumeWeapon ||
//							// 황당하게도 아이템 마스터 데이터를 받지 않은 상황에서는 ItemClass 가 세팅이 안되어 있다. 따라서 처음에 6성무기 이펙트가 안나오는 일이 있음.
//							// InventoryType 과 RefID 체크로 대체 가능하도록.
//							(CurrCostumeInfo->InventoryType == EItemInvenType::EIIVT_CostumeWeapon && FItemRefIDHelper::ExtractItemClassFromRefID(CurrCostumeInfo->ItemRefID) == EItemClass::EIC_CostumeWeapon)
//							)
//						{
//
//							AllLoadedItemInfo.Add(RelevantInfoData); // 쓰고 난 담에 언로딩 하려고 따로 배열에 넣어놓음.
//
//							UParticleSystem* SixStarWeaponEffectPS_L = RelevantInfoData->GetSixStarWeaponEffect(IteminfoTable);
//							UParticleSystem* SixStarWeaponEffectPS_R = RelevantInfoData->GetSixStarWeaponEffect_R(IteminfoTable);
//							if (!SixStarWeaponEffectPS_R) {
//								// _R 버전이 제공되지 않을 시 기본 설정된 걸로 좌측, 우측 모두 세팅. 좌우 동일한 형태면 이렇게 됨.
//								SixStarWeaponEffectPS_R = SixStarWeaponEffectPS_L;
//							}
//							if (SixStarWeaponEffectPS_L)
//							{
//								// 요 셋업에서는 기본적인 정보만 가지고 구성하는데.. 혹 추가될지도 모르겠다.
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
//								// _R 버전이 제공되지 않을 시 기본 설정된 걸로 좌측, 우측 모두 세팅. 좌우 동일한 형태면 이렇게 됨.
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
//				//실제 장착된 정보가 있을때만 동작..
//				if (CurrEquipInfo != nullptr)
//				{
//					// 특정 조건을 만족한 무기에 붙는 추가 파티클 효과. 현 구조상.. 여기는 새로 merge 하는 경우가 아니더라도 로딩을 한다.
//					if (CurrEquipInfo->ItemClass == EItemClass::EIC_Weapon ||
//						// 황당하게도 아이템 마스터 데이터를 받지 않은 상황에서는 ItemClass 가 세팅이 안되어 있다. 따라서 처음에 6성무기 이펙트가 안나오는 일이 있음.
//						// InventoryType 과 RefID 체크로 대체 가능하도록.
//						(CurrEquipInfo->InventoryType == EItemInvenType::EIIVT_Weapon && FItemRefIDHelper::ExtractItemClassFromRefID(CurrEquipInfo->ItemRefID) == EItemClass::EIC_Weapon)
//						)
//					{
//						if (CurrEquipInfo->StarGrade >= FB2Item::MinimumSurpassStarGrade)
//						{ // 6성 무기
//							FSingleItemInfoData* RelevantInfoData = IteminfoTable->GetInfoData(CurrEquipInfo->ItemRefID);
//							AllLoadedItemInfo.Add(RelevantInfoData); // 쓰고 난 담에 언로딩 하려고 따로 배열에 넣어놓음.
//
//							if (RelevantInfoData && !IsCostumeOverride)
//							{
//								{
//									UParticleSystem* SixStarWeaponEffectPS_L = RelevantInfoData->GetSixStarWeaponEffect(IteminfoTable);
//									UParticleSystem* SixStarWeaponEffectPS_R = RelevantInfoData->GetSixStarWeaponEffect_R(IteminfoTable);
//									if (!SixStarWeaponEffectPS_R) {
//										// _R 버전이 제공되지 않을 시 기본 설정된 걸로 좌측, 우측 모두 세팅. 좌우 동일한 형태면 이렇게 됨.
//										SixStarWeaponEffectPS_R = SixStarWeaponEffectPS_L;
//									}
//									if (SixStarWeaponEffectPS_L)
//									{
//										// 요 셋업에서는 기본적인 정보만 가지고 구성하는데.. 혹 추가될지도 모르겠다.
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
//										// _R 버전이 제공되지 않을 시 기본 설정된 걸로 좌측, 우측 모두 세팅. 좌우 동일한 형태면 이렇게 됨.
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
//						//코스튬과 상관없이 N강 이펙트는 장착된 무기에 따라서 추가해준다.
//						// 강화에 따른 무기 파티클 이펙트 표시
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
//									//캐릭터마다 이펙트 다름
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
//									//캐릭터마다 이펙트 다름
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
//						if (CurrEquipInfo->ItemRefID % 10 == 6 ||	// 천상
//							CurrEquipInfo->ItemRefID % 10 == 7)		// 영원
//						{
//							FSingleItemInfoData* RelevantInfoData = IteminfoTable->GetInfoData(CurrEquipInfo->ItemRefID);
//							AllLoadedItemInfo.Add(RelevantInfoData); // 쓰고 난 담에 언로딩 하려고 따로 배열에 넣어놓음.
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
//						if (CurrEquipInfo->ItemRefID % 10 == 6 || // 천상
//							CurrEquipInfo->ItemRefID % 10 == 7)	  // 영원
//						{
//							FSingleItemInfoData* RelevantInfoData = IteminfoTable->GetInfoData(CurrEquipInfo->ItemRefID);
//							AllLoadedItemInfo.Add(RelevantInfoData); // 쓰고 난 담에 언로딩 하려고 따로 배열에 넣어놓음.
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
//			{// 날개는 다른 장비와는 별도로
//				LoadedWingInfo = WingInfoTable ? WingInfoTable->GetInfoData(*InWingData) : NULL;
//				if (LoadedWingInfo)
//				{
//					if (!bPrebuiltMeshProvided) // 파트 리소스 로딩은 실제 mesh merge 를 해야 하는 경우에만 실행
//					{
//						EquippedPartMesh = LoadedWingInfo->GetWingMesh();
//						EquippedPartMaterial = LoadedWingInfo->GetMtrlOverride();
//
//#if !UE_BUILD_SHIPPING
//						// 날개 머티리얼에 들어간 특정 텍스쳐 파라미터 네이밍 룰을 체크하는데 이게 6성부터 있다..
//						if (InWingData->EvolutionGrade >= 6)
//						{
//							UMaterialInterface* WingMtrlCheck = EquippedPartMaterial;
//							if (!WingMtrlCheck && EquippedPartMesh && EquippedPartMesh->Materials.Num() > 0)
//							{// 사실 이 6성 이상 날개 용의 특수 머티리얼은 기본 세팅이 아닌 Override 라 여기로 오지 않겠지..
//								WingMtrlCheck = EquippedPartMesh->Materials[0].MaterialInterface;
//							}
//							if (WingMtrlCheck)
//							{
//								checkSlow(HasWingAuraEmissiveTexture(EquippedPartMaterial));
//							}
//						}
//#endif
//#if !UE_BUILD_SHIPPING // 희한하게 날개가 안 만들어지는 경우가 간혹 있는 거 같아 로그를 뿌려본다.
//						UE_LOG(LogBladeII, Log, TEXT("WingMesh %s has been added as one of equip part for PCClass %d"), EquippedPartMesh ? *EquippedPartMesh->GetName() : TEXT("InvalidMesh"), PCClassToInt(InPCClass));
//#endif
//					}
//					// 요건 아마 7성 이상에서만 세팅이 될 것. 세팅이 되어있는 경우만
//					for (int32 AEI = 0; AEI < LoadedWingInfo->AttachEffectInfoArray.Num(); ++AEI)
//					{
//						FCHSKCompAttachParticleSystemInfo& ThisAttachEffectInfo = LoadedWingInfo->AttachEffectInfoArray[AEI];
//						UParticleSystem* WingPS = ThisAttachEffectInfo.GetParticleSystemAsset();
//						if (WingPS)
//						{
//							FPCPartExtraAttachFxSet WingAttachPSInfo;
//							WingAttachPSInfo = ThisAttachEffectInfo; // 여기 거치면서 WingAttachPSInfo.LoadedAttachPS 에 WingPS 가 세팅될 것.
//							AllExtraAttachFx.Add(WingAttachPSInfo);
//						}
//					}
//				}
//			}
//
//			// 이번 파트에서의 최종적으로 사용할 리소스 정보. bPrebuiltMeshProvided 의 경우 실제로는 모두 null 인 상태일 것.
//			FLocalPartMeshMtrlInfo ThisPartAssetInfo;
//			ThisPartAssetInfo.PartEnum = CurrPartEnum;
//			ThisPartAssetInfo.MeshAsset = EquippedPartMesh ? EquippedPartMesh : (CurrPartDefaultInfo ? CurrPartDefaultInfo->MeshAsset : NULL);
//			ThisPartAssetInfo.MaterialAsset = EquippedPartMaterial; // Material 은 디폴트로 정의된 것은 없음.
//			ThisPartAssetInfo.CostumeEquipPlace = CurrCostumeEquipPlace;
//
//			if (ThisPartAssetInfo.MeshAsset) { // 기본 리스트에도 없고 장착 리스트에도 없으면 NULL 일 수 있다. 그런 경우 최종 조합 리스트에 포함시키지 않는다.
//				AllPartAssets.Add(ThisPartAssetInfo);
//			}
//			// 물론 여기에 extra attach 가 추가될 수도 있다.
//			if (ExtraAttachInfo.MeshAsset)
//			{
//				// AllPartAssets 에 넣기 전에, 동일 PartEnum 의 기본 리스트가 있다면 마찬가지로 그건 빼야지.
//				// DefaultParts 에서도 빼고, 이미 AllPartAssets 에 들어가 있을 수도 있으므로 거기서도 빼고.
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
//
//	// 같이 존재하면 안되는 파트들을 걸러냄
//	TArray<int32> ToRemovePartIndices;
//	for (int32 PIA = 0; PIA < AllPartAssets.Num(); ++PIA)
//	{
//		const EPCPart PartToAdd = AllPartAssets[PIA].PartEnum;
//
//		for (int32 PIB = 0; PIB < AllPartAssets.Num(); ++PIB)
//		{ // 비교 순서가 있어서 PIB 를 PIA+1 부터 시작하지는 못하고 같은 경우만 걸러낸다.
//			if (PIA == PIB){
//				continue;
//			}
//			const EPCPart PartToRemoveTest = AllPartAssets[PIB].PartEnum;
//			if (ShouldRemoveOtherPart(InPCClass, PartToAdd, PartToRemoveTest)) // 만에 하나.. ShouldRemove 관계가 먹이사슬처럼 물고물리는 식이 되어선 안되겠다..
//			{
//				ToRemovePartIndices.AddUnique(PIB);
//			}
//		}
//	}
//	if (ToRemovePartIndices.Num() > 0)
//	{
//		ToRemovePartIndices.Sort([](const int32& A, const int32& B) { return A > B; });
//		for (int32 ThisRemoveIdx : ToRemovePartIndices) // 뒤쪽 인덱스부터 해서 제거
//		{
//			AllPartAssets.RemoveAt(ThisRemoveIdx);
//		}
//	}
//
//	//코스튬에서 같이 쓰는 부위 걸러냄
//	CombineCostumePart(SectionMergeInfo, AllPartAssets);
//
//	if (AllPartAssets.Num() == 0){ // 기본파트든 장착아이템이든 아무 세팅 없음. 그대로 진행하면 MeshMerge 에서 실패 로그.
//		return false;
//	}
//
//	// 본격 MeshMerge 로 넘어가기 전 material 섹션 ID 를 확실히 하기 위해 리스트를 PartEnum 따라 정렬. (GetPCCompositeMtrlID)
//	struct FLocalPartMeshMtrlInfoSorter
//	{
//		bool operator()(const FLocalPartMeshMtrlInfo& A, const FLocalPartMeshMtrlInfo& B) const
//		{
//			return (int32)A.PartEnum < (int32)B.PartEnum;
//		}
//	};
//	AllPartAssets.Sort(FLocalPartMeshMtrlInfoSorter());
//
//#if WITH_EDITOR && !PLATFORM_MAC
//	// 파트 당 다중 material 섹션에 대한 경고. MeshMerge 같은 곳에서는 기능적으로 가능하지만 현실적으로 인게임에서 Material ID 를 관리하기 위해서는 어려움.
//	int32 MultiSectionFoundNum = 0;
//	FString MultiSectionPartWarnMsg = TEXT("개별 파트의 다중 섹션은 현 시스템에서 허용되지 않습니다. 다음 파트 에셋(들)을 확인해 주세요.\n\n");
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
//
//	bool bResult = false;
//
//	USkeletalMesh* CompositeMesh = InOutPrebuiltCompositeMesh ? InOutPrebuiltCompositeMesh : NewObject<USkeletalMesh>(GetTransientPackage(), NAME_None, RF_Transient);
//			
//	// 이건 Section 병합을 하는 경우 원래 머티리얼의 파라미터를 참고하기 위해 모아두는 거
//	TMap<EPCPart, UMaterialInterface*> NonMergedMaterialsBackup;
//
//	if (!bPrebuiltMeshProvided)
//	{
//		// 본격 FSkeletalMeshMerge 기능을 사용하기 위한 스텝. 실제 mesh merge 를 하지 않는 상황에선 필요없다. (bPrebuiltMeshProvided)
//
//		TArray<USkeletalMesh*> SourcePartMeshes;
//		TArray<FSkelMeshMergeSectionMapping> ForceSectionMapping;
//		TArray<FSkelMeshMergeMtrlOverrideMapping> SrcMtrlOverrides;
//		
//		// FSkeletalMeshMerge 에서 사용하기 위해 AllPartAssets 에 모아놓은 정보들을 한번 더 재구성한다.
//		int32 ForceSectionID = 0;
//		for (int32 PAI = 0; PAI < AllPartAssets.Num(); ++PAI)
//		{
//			const FLocalPartMeshMtrlInfo& ThisAssetInfo = AllPartAssets[PAI];
//			check(ThisAssetInfo.MeshAsset); // 바로 상단을 보면 이게 NULL 인 경우는 없어야 함. 또한 아래에 material overriding 에도 영향이 있음.
//			SourcePartMeshes.Add(ThisAssetInfo.MeshAsset);
//
//			if (bInMergeSections)
//			{ // 섹션 병합 시에는 ForceSectionID 랑 적절한 material 이랑 먹여주면 MeshMerge 쪽에서 알아서 합쳐질 것임.
//				// 아닌 경우는 ForceSectionID 는 순차적으로 증가.
//				ForceSectionID = SectionMergeInfo->GetMergedSectionID(HaveCombineCostume, ThisAssetInfo.PartEnum);
//
//				// 그리고 여러 섹션을 하나의 material 안에 넣으면서 필요해지는 UV 좌표 offset 정보도 생성.
//				TArray<FTransform> ThisPartUVTranform;
//				SectionMergeUVTransform.UVTransformsPerMesh.Add(ThisPartUVTranform);
//				check(SectionMergeUVTransform.UVTransformsPerMesh.Num() == PAI + 1);
//
//				// Merge 한 후의 material parameter 세팅에 참고하기 위한 원본 material 캐싱. 만일 null 이 들어가면 실제로 뭔가 잘못된 거겠지.
//				NonMergedMaterialsBackup.Add(ThisAssetInfo.PartEnum,
//					ThisAssetInfo.MaterialAsset ? ThisAssetInfo.MaterialAsset :
//					(ThisAssetInfo.MeshAsset->Materials.Num() > 0 ? ThisAssetInfo.MeshAsset->Materials[0].MaterialInterface : nullptr));
//			}
//
//			FSkeletalMeshResource* ThisMeshResource = ThisAssetInfo.MeshAsset->GetImportedResource();
//			if (ThisMeshResource && ThisMeshResource->LODModels.Num() > 0)
//			{
//				FSkelMeshMergeSectionMapping NewSectionMap;
//				FSkelMeshMergeMtrlOverrideMapping NewMtrlMap; // 물론 나중에 section ID 접근할 필요 없이 현 시점에서 바로 material override 를..
//				for (int32 SI = 0; SI < ThisMeshResource->LODModels[0].Sections.Num(); ++SI) // ForceSectionMapping 은 LODModel 0 번 기준으로 하면 됨.
//				{
//					NewSectionMap.SectionIDs.Add(ForceSectionID);
//
//					// 섹션을 합치게 되면 파트별 섹션을 합쳐서 사용하기 위해 준비된 별도의 material 을 사용한다. 같은 섹션이면 동일한 material 을 사용해야 함.
//					// MergeSections 에서도 여전히 null 일 수 있다. 그런 경우는 원래 material 을 쓰는 건데 여전히 명시적으로 단일 섹션을 구성하는 파트가 있으면 그렇게 될 듯
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
//							FVector(0.5f, 0.5f, 1.0f) // 스케일은 따로 설정 안 하고 기본 가정으로 여기서 넣는다.
//						);
//						// 이 배열은 각 요소가 Texture 좌표 인덱스에 해당하는 것인데 여기에 넣어준 개수 이상의 텍스쳐 좌표를 쓰게 되면 뭔가 이상해질껀데 섹션 merge 되는 모드를 본 후에나 알게 난다면.. 흠..
//						ThisPartUVTransformInfo.Add(PartUVTransform);
//						ThisPartUVTransformInfo.Add(PartUVTransform); // 그래서 혹 모르니 하나 더 넣어주자. 이 이상 걸릴 일은 없겠지..
//					}
//
//					NewMtrlMap.SectionMatOverrides.Add(
//						(bInMergeSections && MergedSectionMaterial) ? MergedSectionMaterial :
//						ThisAssetInfo.MaterialAsset // 파트의 기본 material override. 이건 NULL 일 수도 있다.
//					);
//
//					if (!bInMergeSections) // 섹션 합치는 것이 아닌 경우의 순차적인 Section ID.
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
//		{/** PC Part 의 mesh merge 시 기본 skeleton 과 소스들의 skeleton 간에 ref pose 가 얼마나 다른지 체크해서 경고 띄움. */
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
//			// EPCPTCM_SeparateMergedComponent 인 경우 InBaseMesh 의 스켈레톤 을 사용하지 않는데 이 경우 최종적으로 BaseMesh 컴포넌트의 애니메이션을 사용할 것이기 때문.
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
//			if (!MeshMergeUtil.DoMerge()) // 실제 mesh merge 동작. 지금까지는 단지 셋업이었을 뿐 ㅋ
//			{
//				if (CompositeMesh){
//					CompositeMesh->ConditionalBeginDestroy();
//					CompositeMesh = NULL;
//				}
//			}
//		}
//	}
//
//	if (CompositeMesh) // 캐싱된 것이 주어졌든, Merge 가 성공했든
//	{
//		B2_SCOPED_TRACK_LOG_L2(TEXT("UB2PCClassInfo::SetupSKCompForParts -- Post Merge Setup"));
//
//		CompositeMesh->PhysicsAsset = InBaseMesh->PhysicsAsset;
//		InOutPrebuiltCompositeMesh = CompositeMesh; // 새로 생성이 되었던 경우라면 InOutPrebuiltCompositeMesh 을 통해 결과가 밖으로도 전달될 것.
//				
//		// djsong UE4 MERGE 4.14. 4.14 이후부터 발생한 일인데 merge 하기 전의 원본 메쉬 상태에서 MID 가 만들어져 컴포넌트에 override 된 상태로 여기에 오는 경우가 생김.
//		// 이렇게 클리어를 먼저 해 주던지, 아니면 CompositeMesh->Materials 가지고 똑같이 override 해 주던지.
//		if (!bBuildAndGetResultMeshOnly)
//		{
//			for (int32 MI = 0; MI < SKCompToSetup->GetNumMaterials(); ++MI)
//			{
//				SKCompToSetup->SetMaterial(MI, nullptr);
//			}
//			SKCompToSetup->B2ScalarMatParameter.Empty(); // 위에서 Material 클리어 하는 거랑 비슷한 이유로 이것도 클리어 함.
//		}
//
//		USkeletalMesh* OldSKMeshBackup = SKCompToSetup->SkeletalMesh;
//		if (!bBuildAndGetResultMeshOnly) // 컴포넌트에 메쉬 세팅을 하지 않고 결과만 빼오는 인자.
//		{
//			SKCompToSetup->SetSkeletalMesh(CompositeMesh);
//		}
//
//		// 그리고 섹션을 병합한 경우는 머티리얼을 다르게 사용하므로 파라미터 셋업을 해야 한다.
//		if (bInMergeSections && SectionMergeInfo && !bPrebuiltMeshProvided) // 실제 mesh merge 를 한 경우만
//		{
//			SectionMergeInfo->SetupMergedSectionMaterialParams(HaveCombineCostume, CompositeMesh, NonMergedMaterialsBackup);
//		}
//
//		UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
//		// 혹시 모르겠어서 이전에 생성했던 거 확실히 제거. (다른 거라면)
//		if (OldSKMeshBackup && OldSKMeshBackup != InBaseMesh && OldSKMeshBackup != CompositeMesh 
//			&& OldSKMeshBackup->HasAnyFlags(RF_Transient) // 생성 시 RF_Transient 를 준다.
//			&& !(MeshCacheManager && MeshCacheManager->IsOneOfCachedCompositeMesh(OldSKMeshBackup))) // 정식으로 생성해서 캐싱되어 있던 거라면 캐시가 아직 갱신이 안되었을 테니 여길 통과하지 못할 수도 있는데 그래도 로비에서 그렇지 않은 경우도 꽤 된다.
//		{ 
//			// 여기서 조합한 메쉬 리소스를 여러 액터가 동시에 렌더링에 사용하면 이렇게 명시적으로 Destroy 하는 건 문제를 일으킬 수 있다.
//			// 그런 경우가 발생한다면 메쉬 리소스 새로 조합 시에 그걸 사용할 액터들에 모두 업데이트를 해 주는 등 추가적인 처리 또한 필요하다.
//			
//			// 만일 문제가 된다면 여기서 ConditionalBeginDestroy 날리는 코드는 제거하는 게 좋을 꺼다. 실제로 사용되지 않는 mesh 오브젝트라면 언리얼 GC 시스템에 의해 알아서 제거될 것임.
//			//OldSKMeshBackup->ConditionalBeginDestroy(); 아무래도 그닥 좋아 보이지 않음.. 굳이 이럴 필요 없을 것 같고..
//		}
//		
//		// 로비 SkeletalMeshActor 를 위한 것이라면 이건 NULL 이어야.
//		if (AnimBPForInGameChar && !bBuildAndGetResultMeshOnly)
//		{
//			SKCompToSetup->SetAnimInstanceClass(AnimBPForInGameChar);
//		}
//
//		bResult = true;
//	}
//
//	if (!bBuildAndGetResultMeshOnly)
//	{
//		// 이제 추가 컴포넌트들을 붙일 건데 새로 붙이기 전에 이전에 붙인 게 있다면 제거. 이것도 SkeletalMesh 만 매번 새로 생성하는 로비에서나 적용될 듯..
//		DestroyExtraAttachPartComponents(SKCompOwner, SKCompToSetup);
//
//		for (FPCPartExtraAttachFxSet ExtraAttachFx : AllExtraAttachFx)
//		{
//			UParticleSystemComponent* AttachFx = ExtraAttachFx.CreatePSCHelper(SKCompOwner, SKCompToSetup);
//
//			if (AttachFx)
//			{
//				// Note : 추가 컴포넌트의 색상값을 변경해줘야한다면 MtrlParamName_Color(TEXT("ColorVariControl")) 이펙트에 태그로 처리할것
//				//		: SetColorParameter는 사용하지않고 SetVectorParameter를 직접사용한다.  SetColorParameter의 경우 RGB를 (255,255,255,0)까지밖에 지정하지 못하므로 더 큰값을 원할 경우 FLinearColor를 FVector로 변환해서 넣어주어야함.
//				if (ExtraAttachFx.ChangeColor != FLinearColor::Transparent)
//					AttachFx->SetVectorParameter(MtrlParamName_Color, FVector(ExtraAttachFx.ChangeColor));
//			}
//		}
//	}
//
//	// MeshMerge 를 하게 되면 원본 파트와는 별도의 리소스이므로 원본 파트는 날릴 수 있다. 
//	// 단, 로컬 캐릭터 장착 파트의 경우 여러 상황에서 또 필요할 수 있어서 언제나 날리지는 않고.. 여기로 넘어오는 인자에 따라. 대체로 일시적 merge 면 ForceUnload 로 넘어올 것.
//	if (bForceUnloadPartResourceAfterMerge)
//	{
//		for (FSingleItemInfoData* ThisLoadedItemInfo : AllLoadedItemInfo)
//		{
//			if (ThisLoadedItemInfo) {
//				// 단, RootSet 으로 지정한 건 여기서는 넘어가도록 한다. (bRemoveFromRoot false)
//				// 현재 로컬 캐릭터가 장착한 파트일 수 있으므로.
//				ThisLoadedItemInfo->UnloadOnlyEquipPart(IteminfoTable, false);
//			}
//		}
//	}
//
//	/*if (LoadedWingInfo) { 얘도 별도로 Unload 를 불러주는 구조로 갈까...
//	LoadedWingInfo->UnloadTAssets(WingInfoTable);
//	}*/

	//return bResult;
return true;
}

void UB2PCClassInfo::DestroyExtraAttachPartComponents(AActor* SKCompOwner, USkeletalMeshComponent* SKCompToSetup)
{	
	//if (SKCompOwner && SKCompToSetup)
	//{
	//	// 일단 현재는 FPCPartExtraAttachFxSet 에 의한 ParticleSystemComponent 들 뿐. 추가되면 비슷하게 확장
	//	TArray<UActorComponent*> AllPSC = SKCompOwner->GetComponentsByClass(UParticleSystemComponent::StaticClass());
	//	for (int32 PSCI = 0; PSCI < AllPSC.Num(); ++PSCI)
	//	{
	//		UParticleSystemComponent* ThisPSC = Cast<UParticleSystemComponent>(AllPSC[PSCI]);
	//		// 리소스 템플릿이랑 Attach 된 소켓도 체크할 수 있는데 굳이 그럴 필요없게 이름을 매우 유니크하게 주는 걸로..
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
	// 여기서 로딩하지 않는 여타 일반 hard ptr 들은 PCClassInfoBox 에서 이게 로딩될 때 같이 기본으로 로딩된다.
	// 인게임(전투)에서만 사용하는 애들(중 가능한 애들)만 이런 식으로 따로 구성해서 로비에서의 메모리를 절약.

	//GetSomeLazyLoadedClassAssetCommon<UB2InGameOnlyPCClassInfo>(InfoLoadManager, InGameOnlyAssetPtr, &LoadedInGameOnlyAssetPtr, TEXT("PCClassInfo"));

	ConditionalSetLoadedInGameOnlyInfoRootSet();
}

void UB2PCClassInfo::UnloadInGameOnlyAssets()
{
	//// ManagedPCClassUnload 에 따라 RootSet 일 수 있다.
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
	TargetingModeOverride = ETargetingMode::ETM_Normal;
}

UB2InGameOnlyPCClassInfo* UB2PCClassInfo::GetInGameOnlyInfo(UObject* WorldContextObject)
{
//#if !UE_BUILD_SHIPPING
//	// 여기서 WorldContextObject 는 게임모드 체크용이다. NULL 을 보내줘도 작동은 하겠지만 가급적 그러지 말자.
//	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
//	// 로비에서 쓴다고 해서 작동 안하는 건 아니다. 거의 쓸모없는 상당한 양의 메모리와 로딩 시간 등을 감수해야 한다.
//	// 만일 로비에서 이걸 사용해야 하는 요구사항이 오면 정말 진지하게 고민해 봐야 한다. 매우 많은 것들을 포기해야 할 수 있다.
//	// 내지는 정말 조심조심 해 가면서 한번에 클래스 하나씩 비동기 로딩해가며 사용할 수 있다면 해 볼 수도.
//	checkSlow(!LobbyGM);
//#endif
//
//	// 이전에 AsyncLoad 요청을 날렸는데 아직 완료 안 된 상황.
//	if (InGameOnlyAssetAsyncLoadHandle.IsValid() && !InGameOnlyAssetPtr.IsValid())
//	{
//		InGameOnlyAssetAsyncLoadHandle->WaitUntilComplete(GENERAL_TASSET_ASYNC_LOAD_TIMEOUT); // Timeout 은 적당히.. 넘어가는 상황은 막장이려나
//	}	
//	if(!InGameOnlyAssetPtr.IsValid())
//	{
//		SyncLoadInGameOnlyAssets();
//	}
//	BII_CHECK(InGameOnlyAssetPtr.IsValid()); /* 여기까지 와서도 invalid 면 막장 */
//	if (InGameOnlyAssetPtr.IsValid())
//	{
//		ConditionalSetLoadedInGameOnlyInfoRootSet();
//
//		LoadedInGameOnlyAssetPtr = InGameOnlyAssetPtr.Get(); // SyncLoadInGameOnlyAssets 에선 기본 캐싱해주는데 Async 로딩이 있어서.. 놓치면 안됨.
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
	//// AsyncLoad 로 인한 실제 로딩 시간 단축 체크, 내지는 이상 동작 시 테스트. 쉽핑빌드서도 테스트가 필요할 수 있어서 !UE_BUILD_SHIPPING 으로 안 함.
	//if (!BladeIIGameImpl::bAllowInfoAssetAsyncLoading) { 
	//	SyncLoadInGameOnlyAssets();
	//	return;
	//}

	//if (!InGameOnlyAssetPtr.IsValid()) // ManagedPCClassUnload 에 따라 RootSet 일 수 있는데 여튼 로딩이 되어 있다면 요청 생략
	//{
	//	TArray<FSoftObjectPath> ThisReqList;
	//	ThisReqList.Add(InGameOnlyAssetPtr.GetSoftObjectPath());
	//	// 여기서 this 를 WorldContextObject 로 넣어주고 싶기도 한데 외부에서 적절한 Async Callback 을 먹이기가 슬쩍 까다로워 보이기도..
	//	InGameOnlyAssetAsyncLoadHandle = InfoLoadManager.RequestAsyncLoad(ThisReqList, FStreamableDelegate::CreateUObject(this, &UB2PCClassInfo::InGameOnlyAssetAsyncLoadCB), InPriority);
	//	bInGameOnlyAssetAsyncLoadRequestPending = true; // 요 플래그는 딱히 쓸모가 없을 수 있다.. 아직 잘 몰라서
	//}
}

void UB2PCClassInfo::InGameOnlyAssetAsyncLoadCB() // 메인쓰레드에서 불림. 거의 확인 차원으로만. 실제 로딩에 대한 waiting 같은 건 FStreamableHandle 로.
{
	// 이 기본 가정이 틀리면 언리얼 StreamableManager 동작을 잘못 파악한 것이거나 언리얼 내부 버그거나
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
	//// 레벨 내릴 때 unload 안 할 꺼면 확실히 RootSet 에 박아둠.
	//if (LoadedInGameOnlyAssetPtr && !UB2PCClassInfoBox::ShouldUnloadAllPCClassInfoOnLevelTransition())
	//{
	//	LoadedInGameOnlyAssetPtr->AddToRoot();
	//}
}

TSubclassOf<ABladeIIPlayer> UB2PCClassInfo::GetBaseBPClass(UObject* WorldContextObject)
{
	//// 여기선 InGameOnlyAsset 을 로딩하지 않는다. 로딩되지 않은 상황에서 이걸 사용할 경우가 생기면 재고..
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
	// 이렇게 맵핑해서 CharacterType 을 세팅하는 대신 BladeIIPlayer 쪽에 EPCClass 멤버를 넣어놓을 수도..
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

	//	// Prefix 확인. 거의 ABladeIIPlayer::GetTrimmedNameFromPCClassEnum 동작을 위해..
	//	if(ClassEnumName.StartsWith(EPCCLASSENUM_COMMON_PREFIX) == false)
	//	{
	//		return false;
	//	}
	//}

	return true;
}

#endif

// TAsset lazy-loading 을 사용하는 Blueprint InfoAsset 의 오동작 문제를 정식으로 해결하기 전까지의 임시 땜질.
#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP
void UB2PCClassInfo::CleanupOnPreSave()
{
	LoadedInGameOnlyAssetPtr = NULL;
}
void UB2PCClassInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// 의도치 않게 저장된 레퍼런스를 날려준다.
	CleanupOnPreSave();
}
#endif


/**
 * ======================================================================================================
 * For dynamically merge player character mesh sections
 * 특수 모드 성능 확보를 위한 플레이어 캐릭터 mesh section 병합 용 InfoAsset
 * ======================================================================================================
 */

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
int32 UB2PCMeshSectionMergeInfo::SectionMergeTestFlag = 0; // 1이면 무조건 사용 2면 무조건 끄기, 나머지는 게임 기능에 따라
bool UB2PCMeshSectionMergeInfo::ShouldForceUseSectionMerge()
{
	return SectionMergeTestFlag == 1;
}
bool UB2PCMeshSectionMergeInfo::ShouldForceUnuseSectionMerge()
{
	return SectionMergeTestFlag == 2;
}
#endif

/** 섹션 병합 메쉬 모델 사용 관련한 scalability 변수. 일부 게임모드에서만 사용되고, 이것과 관계없이 무조건 섹션 병합 모델을 사용하는 모드도 있다.
 * 가급적 1 이 높은 사양 용이 되도록 SectionMergedMeshUse 와 반대 의미로. */
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
	//	// 게임모드 차원에서 섹션 병합 모델을 강제하는 경우도 있고, 옵션에 따라 사용하는 경우도 있음.
	//	(GameRuleObj->UseSectionMergedPCModel() || (GameRuleObj->ConditionalUseSectionMergedPCModel() && (CVarB2GameSectionPreservedMeshUse.GetValueOnGameThread() == 0)))
	//	: false;
	return false;
}
bool UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeForStageEvent(UObject* WorldContextObject)
{
	// 연출에서 섹션 병합된 걸 사용할지 여부인데 당장 사용하지는 않더라도 혹시라도 나중에 기능을 넣게 될 때 고쳐야 하는 부분들 찾기 쉽도록
//	return (
//		false // 구현을 하게 되면 아마도 사용자가 선택한 옵션 설정이 될 듯..
//#if 0
//		&& ShouldUseSectionMergeByGameContext(WorldContextObject) // 구현 들어갔을 시 이 조건은 기본으로 체크 필요.
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
{ // 새로 병합된 섹션 안에서의 자기 영역 찾기
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
	//// SkeletalMesh 의 Material 을 동적으로 생성한 MIC 로 바꿔친다.
	//// 여기 들어온 SkeletalMesh 도 동적으로 생성한 오브젝트여야 해서 바꿔쳐도 됨.
	//check(InSectionMergedMesh->HasAnyFlags(RF_Transient));

	//// 기본적으로 merge 된 섹션을 핸들링하기 위해 특별히 제작된 material 이 세팅되어 있어야 하는데
	//// 동일한 구성의 material 을 가지고 매 인스턴스마다 parameter 를 다르게 쓸 것이므로 MIC 를 새로 생성한다.
	//for (int32 MI = 0; MI < InSectionMergedMesh->Materials.Num(); ++MI)
	//{
	//	UMaterialInterface* ParentMaterial = InSectionMergedMesh->Materials[MI].MaterialInterface;
	//	UMaterialInstanceConstant* ParentMIC = Cast<UMaterialInstanceConstant>(ParentMaterial);
	//	if (ParentMIC && ParentMIC->HasAnyFlags(RF_Transient)) 
	//	{
	//		// 이 경우 이미 이전에 여기서 transient 로 생성해서 세팅한 MIC 인 것임. 동일 조건에서 merge 된 메쉬는 GameMode 에 캐싱해 놓고 쓰므로 여기로 오게 될 수 있다.
	//		// 이쪽 최적화를 거치게 되면.. 이미 한번 생성해 놓은 게 있으면 이곳으로 재차 오거나 할 필요 없이 그대로 사용할 수 있도록 하는 게 좋을 것.
	//	}
	//	else
	//	{
	//		// MIC 를 dynamic create 로 사용.. RF_Transient 를 넣어준다. MaterialInstanceConstant 쪽의 Set~~TransientObjOnly 를 참고.
	//		// 특별한 사용인 셈인데 MIC 는 기본적으로 Editor 에서 생성해서 사용하는 것이다. 그러나 SkeletalMeshMerge 부터가 이미 그런 룰을 깨는 건데 못할 거 뭐 있음?
	//		UMaterialInstanceConstant* NewMIC = NewObject<UMaterialInstanceConstant>(InSectionMergedMesh, NAME_None, RF_Transient);
	//		if (NewMIC)
	//		{
	//			// 기존에 세팅되어 있던 material 을 parent 로 한 MIC 를 생성해서 사용.
	//			NewMIC->SetParentTransientObjOnly(ParentMaterial, false); // MIC 를 dynamic 하게 쓰면서 원래 MIC 에는 없던 인터페이스 뚫어놓음
	//			FSkeletalMaterial& OldMaterialSetup = InSectionMergedMesh->Materials[MI];
	//			FSkeletalMaterial NewMaterialSetup(NewMIC, true);
	//			NewMaterialSetup.UVChannelData = OldMaterialSetup.UVChannelData;
	//			InSectionMergedMesh->Materials[MI] = NewMaterialSetup;
	//		}
	//	}
	//}

	//// Merge 안된 원본 material 단위로 돌면서 자신에 해당하는 merge 된 파트의 section 에 세팅된 MIC 를 찾아 material parameter 를 전달
	//for(TMap<EPCPart, UMaterialInterface*>::TIterator OriginalMtrlIt(OriginalNonMergedMaterials); OriginalMtrlIt; ++OriginalMtrlIt)
	//{
	//	const EPCPart ThisPartEnum = OriginalMtrlIt.Key();
	//	UMaterialInstance* ThisPartOriginalMI = Cast<UMaterialInstance>(OriginalMtrlIt.Value());

	//	// 현재 MeshMerge 기능이 원하는 SectionID 대로 맵핑이 제대로 안되고 있어서 Mesh 의 Materials 에 인덱스 넣어서 가져오면 원하는 material 을 콕 찝어서 가져올 수 없음
	//	// 그걸 고치기 전까지는 부모 머티리얼을 체크하는 식으로 해서 원하는 현재 파트의 MIC 를 가져온다.		
	//	UMaterialInterface* SupposedParentMtrl = GetMergedSectionMaterial(InHaveCombineCostume, ThisPartEnum);
	//	UMaterialInstanceConstant* MergedMIC = nullptr;
	//	// 이게 현재로서는 안먹힌다는 거. MeshMerge 기능이 좀 꼬였음.
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
	// MergedSectionData 편집 시 잘못될 수 있는 부분들 체크.
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
			FString::Printf(TEXT("[PCMeshSectionMergeInfo] 잘못된 세팅이 발견되었습니다.\r\n서로 다른 그룹 간에 ContainingParts 중복 : %d 개\r\n그룹 내에서 ContainingParts 로 설정되지 않은 Part 에 대한 세팅 : %d 개"),
				ContainingPartConflictFound, NonExistParts)
		));
#endif

	}
}
#endif