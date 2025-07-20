// Fill out your copyright notice in the Description page of Project Settings.
#include "B2ActiveCameraActor.h"
//#include "BladeII.h"
#if WITH_EDITOR
#include "Editor.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Components/TextRenderComponent.h"
#endif

#include "BladeIIPlayer.h"
#include "BladeIIPlayerController.h"
#include "BladeIIGameMode.h"
#include "B2TMGameMode.h"
#include "BladeIIWorldSettings.h"
#include "B2MonsterSpawnPool.h"
#include "GameFramework/SpringArmComponent.h"
#include "BladeIIScalabilityControl.h"

#include "FB2ErrorMessage.h"
#include "Components/BillboardComponent.h"


ABladeIIPlayer* AB2ActiveCameraActor::GetInteractPlayerCharacter()
{ 
	return (CachedInteractingController ? Cast<ABladeIIPlayer>(CachedInteractingController->GetPawn()) : NULL);
}

// Sets default values
AB2ActiveCameraActor::AB2ActiveCameraActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = false;
	NetUpdateFrequency = 10.0f;
	SetReplicatingMovement(false);
	SetCanBeDamaged(false);

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	SetRootComponent(DefaultRootComponent);

#if WITH_EDITOR && WITH_EDITORONLY_DATA
	EditorSpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("ActiveCameraActorSprite"));
	if (!IsRunningCommandlet() && (EditorSpriteComponent != nullptr))
	{
		// Structure to hold one-time initialization
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTexture;
			FName ID_B2ActiveCameraActor;
			FText NAME_B2ActiveCameraActor;
			FConstructorStatics()
				: SpriteTexture(TEXT("/Engine/EditorResources/S_SceneCaptureIcon")) // 迭 墨皋扼 葛剧 酒捞能捞 乐焙.
				, ID_B2ActiveCameraActor(TEXT("B2ActiveCameraActor"))
				, NAME_B2ActiveCameraActor(FText::FromString(TEXT("B2ActiveCameraActorEditorSprite")))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;

		EditorSpriteComponent->Sprite = ConstructorStatics.SpriteTexture.Get();
		EditorSpriteComponent->SpriteInfo.Category = ConstructorStatics.ID_B2ActiveCameraActor;
		EditorSpriteComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_B2ActiveCameraActor;
		EditorSpriteComponent->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		EditorSpriteComponent->bIsScreenSizeScaled = true;
		EditorSpriteComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
#endif

	CameraMovementMode = EActiveCameraMovementMode::EACMM_FollowPlayer;
	CameraTargetingMode = EActiveCameraTargetingMode::EACTM_Player;

	// 惑窜俊 CameraMovementMode 尔 CameraTargetingMode 汲沥 棺 SetEditConditionProperties 蝶扼 利例窍霸
	bDualCameraBlending_Inverse = true;
	bCameraMovementMode_ScaledFollowingPlayer = false;
	bCameraTargetingMode_Mob = false;
	bCameraTargetingMode_Mob_with_PlayerCameraBoom = false;
	bCameraTargetingMode_Fixed = false;
	bCameraMovementTarget_BothPlayer = true;
	bUseActiveCameraComponentMode = false;
	bTriggerMode_ToggleByComponent = false;

	bIsActivated = false;

	CameraPlayerFollowingScale = 0.5f;
	BlendInTime = 1.0f;
	BlendOutTime = 1.0f;
	TransitionInputDisableTime = 0.6f;
	TransitionAutoMoveScale = 1.0f;
	bTransitInInputDisabled = false;
	bTransitOutInputDisabled = false;
	
	bOverrideFOVAngle = false;
	FOV_Override = 90.0f;
	DCBExtraFOV_Override = 90.0f;

	bDualCameraBlending = false;
	DCBExtraCamName = NAME_None;
	DCBFlagName = NAME_None;
	DCBExtraFlagName = NAME_None;
	bRestoreCamera = true;
	CachedDualCameraBlendedYaw = 0.0f;

	bOverrideSpringArmSettings = false;

	bUseCameraBoomRotationClamp = false;

	TriggerMode = EActiveCameraTriggerMode::EACTRG_ActorArea;

	FixedCameraTargetPosition = FVector(0.0f, 0.0f, 0.0f);

	PlayerMoveInputAdjustBoomSyncAlpha = 1.0f;

	ActiveCameraSpringArm = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("ActiveCameraSpringArm"));
	ActiveCameraSpringArm->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	ActiveCameraSpringArm->TargetArmLength = 100.0f;
	ActiveCameraSpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
	ActiveCameraSpringArm->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	ActiveCameraComp = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("ActiveCameraComp"));
	ActiveCameraComp->AttachToComponent(ActiveCameraSpringArm, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	ActiveCameraComp->bAutoActivate = true;
	ActiveCameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	ActiveCameraComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	bUsePlacedActiveCamYawAsInputBase = false;
	LastInteractBeginPlayer = NULL;
	CachedLocalPC = NULL;
	CachedInteractingController = NULL;
	CachedTargetMob = NULL;

	bActivedByAnimation = false;
}

void AB2ActiveCameraActor::PostLoad()
{
	Super::PostLoad();

#if WITH_EDITOR
	SetEditConditionProperties();

	if (bDualCameraBlending && CameraMovementMode == EActiveCameraMovementMode::EACMM_Fixed && !FindExtraCamCompForDualCameraBlending())
	{
#if !PLATFORM_MAC
		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
			FString::Printf(TEXT("[版绊] ActiveCameraActor %s 俊 DualCameraBlending 捞 汲沥登菌栏唱 捞甫 困茄 眠啊 墨皋扼 哪器惩飘啊 力措肺 汲摹 趣篮 汲沥登绢 乐瘤 臼嚼聪促."), *GetName())
			));
#endif
	}
#endif
}

void AB2ActiveCameraActor::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
#if WITH_EDITOR
	if (GIsEditor)
	{
		RefreshEditingComp();
	}
#endif
}

void AB2ActiveCameraActor::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

#if WITH_EDITOR && WITH_EDITORONLY_DATA
	if (GIsEditor && GIsPlayInEditorWorld == false)
	{
		SyncFromEditingCompPlacement(); // 橇府轰 哪器惩飘肺 祈笼茄 困摹 教农
	}
#endif
}

void AB2ActiveCameraActor::BeginDestroy()
{
	Super::BeginDestroy();

	RemoveComponentOverlapCallback(); // 怖 且 鞘夸啊 乐唱..?

#if WITH_EDITOR
	// Need to manually destroy them..?
	DestroyEditingComponents(true);
#endif
}

// Called when the game starts or when spawned
void AB2ActiveCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
	check(ActiveCameraSpringArm && ActiveCameraComp);
	// 俊叼磐俊辑 硅摹茄 扁霖 困摹肺辑.. Fixed 唱 ScaledFollowPlayer 俊辑 荤侩.
	InitialActiveCameraSpringArmTransform = ActiveCameraSpringArm->GetComponentTransform();

	RegisterComponentOverlapCallback();

	if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ManualActive)
		return;

	if (TriggerMode == EActiveCameraTriggerMode::EACTRG_BeginStage)
		ForceBeginActiveCamera();
	else
		// 矫累何磐 敲饭捞绢客 立盟茄 版快 官肺 矫累. 捞 捞饶俊 ABladeIIWorldSettings::BeginPlay 啊 阂赴促搁 ApplyDefaultPlayerCameraSetting 甫 窍瘤 臼阑 巴.
		ManualTestBeginActiveCamera();
}

// Called every frame
void AB2ActiveCameraActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bIsActivated)
	{
		CheckForOutdatedInteraction();
		UpdateActiveCamera(DeltaTime);
	}
}

void AB2ActiveCameraActor::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	if (bDualCameraBlending)
	{		
		// 绊沥 墨皋扼 捞侩
		if (CameraMovementMode == EActiveCameraMovementMode::EACMM_Fixed)
		{
			UCameraComponent* CamA = ActiveCameraComp;
			UCameraComponent* CamB = FindExtraCamCompForDualCameraBlending();

			if (!CamA || !CamB)
			{
				Super::CalcCamera(DeltaTime, OutResult);
			}
			else
			{
				// DualCameraBlending 狼 角龙利牢 扁瓷.

				FMinimalViewInfo ViewInfoA;
				CamA->GetCameraView(DeltaTime, ViewInfoA);
				FMinimalViewInfo ViewInfoB;
				CamB->GetCameraView(DeltaTime, ViewInfoB);

				const float BlendWeight = GetDualCameraBlendingWeight(); // B 肺狼 weight 啊 登摆促.
				// OutResult 俊绰 ViewInfoA 客 ViewInfoB 甫 集澜.
				OutResult = ViewInfoA;
				OutResult.BlendViewInfo(ViewInfoB, BlendWeight);

				CachedDualCameraBlendedYaw = OutResult.Rotation.Yaw; // 捞扒 涝仿 勤甸傅阑 困秦..
			}
		}
	}
	else
	{
		Super::CalcCamera(DeltaTime, OutResult);
	}
}

void AB2ActiveCameraActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// 泅犁 咀萍宏墨皋扼 荐悼劝己拳等芭搁 坷滚乏 公矫
	ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	if (pGM)
	{
		AB2ActiveCameraActor* pCurAcriveCamActor = pGM->GetCurrentActiveActiveCameraActor();

		if (pCurAcriveCamActor && pCurAcriveCamActor->GetTriggerMode() == EActiveCameraTriggerMode::EACTRG_ManualActive)
			return;
	}

	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(OtherActor);
	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetWorld()->GetFirstPlayerController());

	// 敲饭捞绢 overlap 俊辑 惯悼.
	if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ActorArea && B2PC &&
		B2Player && B2Player == GetWorld()->GetFirstPlayerController()->GetCharacter())
	{
		BeginActiveCamera(B2PC);
	}
}

void AB2ActiveCameraActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	// 泅犁 咀萍宏墨皋扼 荐悼劝己拳等芭搁 坷滚乏 公矫
	ABladeIIGameMode* pGM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	if (pGM)
	{
		AB2ActiveCameraActor* pCurAcriveCamActor = pGM->GetCurrentActiveActiveCameraActor();

		if (pCurAcriveCamActor && pCurAcriveCamActor->GetTriggerMode() == EActiveCameraTriggerMode::EACTRG_ManualActive)
			return;
	}

	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(OtherActor);
	ABladeIIPlayer* B2LocalPlayer = Cast<ABladeIIPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	// 敲饭捞绢啊 栋唱搁 吝窜.
	if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ActorArea &&
		B2Player && B2LocalPlayer && B2Player == B2LocalPlayer)
	{
		EndActiveCamera();
	}
}

void AB2ActiveCameraActor::OnComponentBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(OtherActor);
	ABladeIIPlayer* B2LocalPlayer = Cast<ABladeIIPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetWorld()->GetFirstPlayerController());

	if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ToggleByComponent &&
		B2PC && B2Player && B2LocalPlayer && B2Player == B2LocalPlayer)
	{
		// BeginComponent/EndComponent 吝 绢蠢 巴栏肺 殿废茄 巴俊 overlap 沁绰瘤 咯何俊 蝶扼.
		if (IsCurrentlyActive())
		{
			UShapeComponent* SweepComp = Cast<UShapeComponent>(SweepResult.GetComponent());
			if (IsOneOfEndToggleComp(SweepComp))
			{
				EndActiveCamera();
			}
		}
		else
		{
			UShapeComponent* SweepComp = Cast<UShapeComponent>(SweepResult.GetComponent());
			if (IsOneOfBeginToggleComp(SweepComp))
			{
				BeginActiveCamera(B2PC);
			}
		}
	}
}

void AB2ActiveCameraActor::OnComponentEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(OtherActor);
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	ABladeIIPlayer* B2LocalPlayer = PC ? Cast<ABladeIIPlayer>(PC->GetCharacter()) : nullptr;

	if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ToggleByComponent &&
		B2Player && B2LocalPlayer && B2Player == B2LocalPlayer)
	{
		// 咯扁急 迭洒 贸府 救秦档 瞪 淀.
	}
}

void AB2ActiveCameraActor::RegisterComponentOverlapCallback()
{
	// ShapeComponent 父 Overlap callback 俊 殿废
	TArray<UActorComponent*> AllComps;
	GetComponents(AllComps);
	for (int32 CI = 0; CI < AllComps.Num(); ++CI)
	{
		UShapeComponent* CurrComp = Cast<UShapeComponent>(AllComps[CI]);
		if (CurrComp)
		{
			CurrComp->OnComponentBeginOverlap.AddDynamic(this, &AB2ActiveCameraActor::OnComponentBeginOverlapCallback);
			CurrComp->OnComponentEndOverlap.AddDynamic(this, &AB2ActiveCameraActor::OnComponentEndOverlapCallback);
		}
	}
}
void AB2ActiveCameraActor::RemoveComponentOverlapCallback()
{
	// ShapeComponent 父
	TArray<UActorComponent*> AllComps;
	GetComponents(AllComps);
	for (int32 CI = 0; CI < AllComps.Num(); ++CI)
	{
		UShapeComponent* CurrComp = Cast<UShapeComponent>(AllComps[CI]);
		if (CurrComp)
		{
			CurrComp->OnComponentBeginOverlap.RemoveDynamic(this, &AB2ActiveCameraActor::OnComponentBeginOverlapCallback);
			CurrComp->OnComponentEndOverlap.RemoveDynamic(this, &AB2ActiveCameraActor::OnComponentEndOverlapCallback);
		}
	}
}

bool AB2ActiveCameraActor::ManualTestBeginActiveCamera(float BlendInTimeOverride /*= -1.0f*/)
{
	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ActorArea && B2PC && B2Player && IsOverlappingActor(B2Player))
	{
		BeginActiveCamera(B2PC, BlendInTimeOverride);

		return true;
	}

	// 扁鸥 炼扒捞 眠啊瞪 荐 乐促. TriggerMode 啊 EACTRG_ToggleByComponent 牢 版快俊 措秦? 
	// 窍瘤父 弊扒 瘤加利牢 overlap 捞 酒囱 鉴埃狼 touch 俊 狼秦 toggle 登绰 巴牢单 烙狼狼 矫痢俊 绢痘霸 魄窜窍瘤?

	return false;
}

void AB2ActiveCameraActor::ForceBeginActiveCamera(float BlendInTimeOverride /*= -1.0f*/, AController* InOptionalInteractingController /*= NULL*/, bool bUseConditionalBeginTransitionInputDisable /*= true*/)
{
	// 咯鸥 炼扒 八荤 绝捞 弊成 矫累. 拱沸 漂喊茄 版快俊父 荤侩秦具摆促.
	ABladeIIPlayerController* B2LocalPC = Cast<ABladeIIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (B2LocalPC)
	{
		BeginActiveCamera(B2LocalPC, BlendInTimeOverride, InOptionalInteractingController, bUseConditionalBeginTransitionInputDisable);
	}
}

void AB2ActiveCameraActor::CustomEndActiveCamera()
{
	EndActiveCamera(); // 老窜 喊促甫 扒 绝焙.
}

bool AB2ActiveCameraActor::IsBaseActiveCamera()
{
	return TriggerMode == EActiveCameraTriggerMode::EACTRG_BeginStage;
}

void AB2ActiveCameraActor::BeginActiveCamera(class ABladeIIPlayerController* InLocalPC, float BlendInTimeOverride /*= -1.0f*/, class AController* InOptionalInteractingController /*= NULL*/, bool bUseConditionalBeginTransitionInputDisable /*= true*/)
{
	check(ActiveCameraSpringArm && ActiveCameraComp);
	check(InLocalPC);
		
	// 快急, 扁粮俊 劝己拳 等 巴捞 乐促搁 辆丰. 弊贰具 墨皋扼 喉坊爹捞 救 部烙.
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));

	if (B2GM && !B2GM->IsAllowCameraWorking())
		return;

	if (B2GM && B2GM->GetCurrentActiveActiveCameraActor() && B2GM->GetCurrentActiveActiveCameraActor() != this)
	{
		B2GM->GetCurrentActiveActiveCameraActor()->CustomEndActiveCamera();
	}

	// 咯扁 牢磊甫 烹秦辑 BlendInTime 阑 override 且 荐 乐绰单 措眉肺 楷免 辆丰客 鞍篮 版快 寇何俊辑 流立 利侩 矫档甫 且 锭俊 荤侩且 狼档.
	const float FinalBlendTime = BlendInTimeOverride >= 0.0f ? BlendInTimeOverride : BlendInTime;

	BeginForNewInteractionCommon(InLocalPC, FinalBlendTime, InOptionalInteractingController);

	if(bUseConditionalBeginTransitionInputDisable)
		ConditionalBeginTransitionInputDisable(true);

	bIsActivated = true;

	// GameMode 俊 殿废
	if (B2GM)
	{
		B2GM->SetCurrentActiveActiveCameraActor(this);
	}
}

void AB2ActiveCameraActor::BeginForNewInteractionCommon(class ABladeIIPlayerController* InLocalPC, float FinalBlendInTime, class AController* InOptionalInteractingController /*= NULL*/)
{
	check(InLocalPC);
	// InOptionalInteractingController 绰 LocalPC 贸烦 饭骇 肺爹等 郴郴 蜡瘤登绰 局啊 酒聪扼辑 invalid 且 啊瓷己捞 惑寸洒 粮犁窃.
	checkSlow(!InOptionalInteractingController);

	CachedLocalPC = InLocalPC;
	CachedInteractingController = InOptionalInteractingController ? InOptionalInteractingController : CachedLocalPC; // Stage 霸烙 葛靛俊辑绰 CachedLocalPC 客 CachedInteractingController 啊 鞍阑 巴..
	LastInteractBeginPlayer = Cast<ABladeIIPlayer>(CachedInteractingController->GetPawn()); // 捞犯霸 某教秦辑 唱吝俊 眉农父 窍绊 角力 荤侩篮 窍瘤 臼绰促.
	
	if (bDualCameraBlending && CameraMovementMode == EActiveCameraMovementMode::EACMM_Fixed)
	{
		UCameraComponent* CamA = ActiveCameraComp;
		UCameraComponent* CamB = FindExtraCamCompForDualCameraBlending();
		if (CamA && CamB)
		{
			// 弊成 ActiveCameraComp 父 Fixed 肺 荤侩窍绰 芭尔 厚搅且 荐 乐绰单 汲沥 傈崔窍绊 窍促 焊搁 促弗 窍唱啊 穿遏登电瘤 且 荐档 乐绢辑 漂喊茄 夸没捞 酒囱 茄篮 弊成 狄.
			// 捞巴档 ForceFromLastView甫 秦具 力措肺 冈洒匙夸.
			InLocalPC->SetViewTargetWithBlend(this, FinalBlendInTime, EViewTargetBlendFunction::VTBlend_Cubic, 0, false);
			// 扁鸥 Dual Blending 狼 琴缴 累悼篮 CalcCamera 俊辑.
		}
	}
	else
	{
		if (CameraMovementMode == EActiveCameraMovementMode::EACMM_Fixed)
		{
			// CameraTargetingMode 档 Fixed 啊 酒聪扼搁 rotation 篮 官拆 荐 乐促.
			ActiveCameraSpringArm->SetWorldTransform(InitialActiveCameraSpringArmTransform);
		}
		if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Fixed)
		{
			// FixedCameraTargetPosition 篮 relative 肺 秒鞭.
			FTransform FixedTargetWorldTransform = FTransform(FixedCameraTargetPosition) * this->GetTransform();
			SetActiveCameraRotationToTarget(FixedTargetWorldTransform.GetTranslation());
		}

		if (CameraMovementMode == EActiveCameraMovementMode::EACMM_FollowPlayer && CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Player)
		{
			// 扁夯利牢 player top-down 墨皋扼甫 汲沥父 官层辑 荤侩窍绰 葛靛啊 凳. 芭狼 贸澜俊父 技泼.
			SetupForConsistentPlayerTopDownCamera();
		}

		UCameraComponent* FinalCameraComponent = NULL;
		if (IsUsingActiveCameraComponent())
		{
			// 构啊 巩哩瘤 肋 葛福摆绰单 搬惫 ForceFromLastView 甫 秦具 力措肺 冈洒绰焙
			CachedLocalPC->SetViewTargetWithBlend(this, FinalBlendInTime, EViewTargetBlendFunction::VTBlend_Cubic, 0, false);
			FinalCameraComponent = ActiveCameraComp;
		}
		else
		{
			if (LastInteractBeginPlayer)
			{
				// 酒聪扼搁 敲饭捞绢 top-down 墨皋扼甫 荤侩茄促绰 舵捞 登绰单 鞍篮 ViewTarget 埃狼 喉坊爹捞搁 PlayerCameraManager 郴俊辑 漂荐窍霸 贸府秦辑 喉坊爹 且 巴.
				CachedLocalPC->SetViewTargetWithBlend(LastInteractBeginPlayer, FinalBlendInTime, EViewTargetBlendFunction::VTBlend_Cubic);
				
				if (LastInteractBeginPlayer->HasActorBegunPlay())
				{
					FinalCameraComponent = LastInteractBeginPlayer->GetTopDownCameraComponent();
				}
			}
		}

		if (bOverrideFOVAngle && FinalCameraComponent) // 弥辆利栏肺 荤侩登绰 CameraComponent 俊促啊 FOV 阿档 override. EndActiveCamera 俊辑 倒妨初阑 巴.
		{
			FinalCameraComponent->FieldOfView = FOV_Override;
		}

		OverrideSpringArmSettings();
	}
}

void AB2ActiveCameraActor::EndActiveCamera()
{
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	if (B2GM && !B2GM->IsAllowCameraWorking())
		return;
	
	ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
	if (CachedLocalPC && InteractLocalPlayer) // NULL 捞搁 捞固 End 贸府 登菌芭唱 酒丛 购啊 肋给登菌芭唱. 滴锅 阂副 荐绰 乐栏骨肺 吝汗 贸府窍瘤 臼档废.
	{
		if (bRestoreCamera)
		{
			// 敲饭捞绢 扁夯 墨皋扼肺 back
			// Cubic 捞寇狼 可记捞 鞘夸窍促搁 函荐肺 畴免
			if (bOverrideFOVAngle)
			{
				// Beginplay 锭 某教秦 初篮 蔼栏肺 倒酒皑.
				InteractLocalPlayer->GetTopDownCameraComponent()->FieldOfView = InteractLocalPlayer->GetTopDownCamIntialFOV();
			}
			RestoreSpringArmSettings();
			CachedLocalPC->ApplyWorldDefaultCamera(BlendOutTime, EViewTargetBlendFunction::VTBlend_Cubic);
		}

		// GameMode 殿废 秦力
		if (B2GM && B2GM->GetCurrentActiveActiveCameraActor() == this)
		{
			B2GM->SetCurrentActiveActiveCameraActor(NULL);
		}

		ConditionalBeginTransitionInputDisable(false);
	}

	CachedLocalPC = NULL;
	CachedInteractingController = NULL;
	LastInteractBeginPlayer = NULL;
	CachedTargetMob = NULL;
	
	bIsActivated = false;
}

void CopyFromSelectedSettingsToSpringArmComponent(FActiveCameraSpringArmSelectedSettings& FromSettings, USpringArmComponent* ToComponent)
{
	if (ToComponent)
	{
		ToComponent->bEnableCameraLag = FromSettings.bEnableCameraLag;
		ToComponent->bEnableCameraRotationLag = FromSettings.bEnableCameraRotationLag;
		ToComponent->CameraLagSpeed = FromSettings.CameraLagSpeed;
		ToComponent->CameraRotationLagSpeed = FromSettings.CameraRotationLagSpeed;
		ToComponent->CameraLagMaxDistance = FromSettings.CameraLagMaxDistance;
	}
}

void CopyFromSpringArmComponentToSelectedSettings(USpringArmComponent* FromComponent, FActiveCameraSpringArmSelectedSettings& ToSettings)
{
	if (FromComponent)
	{
		ToSettings.bEnableCameraLag = FromComponent->bEnableCameraLag;
		ToSettings.bEnableCameraRotationLag = FromComponent->bEnableCameraRotationLag;
		ToSettings.CameraLagSpeed = FromComponent->CameraLagSpeed;
		ToSettings.CameraRotationLagSpeed = FromComponent->CameraRotationLagSpeed;
		ToSettings.CameraLagMaxDistance = FromComponent->CameraLagMaxDistance;
	}
}

void AB2ActiveCameraActor::OverrideSpringArmSettings()
{
	if (bOverrideSpringArmSettings)
	{
		if (IsUsingActiveCameraComponent())
		{
			CopyFromSelectedSettingsToSpringArmComponent(SpringArmSettings, ActiveCameraSpringArm);
		}
		else if (CachedInteractingController)
		{
			ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
			if (InteractLocalPlayer)
			{// 敲饭捞绢 top-down 墨皋扼狼 版快绰 眠饶 加己蔼甸阑 登倒府扁 困秦 泅犁 蔼阑 某教秦 敌促.
				CopyFromSpringArmComponentToSelectedSettings(InteractLocalPlayer->GetCameraBoom(), PlayerCameraBoomSettingBackup);
				CopyFromSelectedSettingsToSpringArmComponent(SpringArmSettings, InteractLocalPlayer->GetCameraBoom());
			}
		}
	}
}
void AB2ActiveCameraActor::RestoreSpringArmSettings()
{
	ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
	// 敲饭捞绢 某腐磐俊 崔赴 Top-down 墨皋扼父 措惑烙. 咯扁俊辑 焊蜡窍绊 乐绰 ActiveCameraComp 绰 被捞 登倒副 鞘夸啊 绝瘤..
	if (bOverrideSpringArmSettings && IsUsingActiveCameraComponent() == false && InteractLocalPlayer)
	{
		CopyFromSelectedSettingsToSpringArmComponent(PlayerCameraBoomSettingBackup, InteractLocalPlayer->GetCameraBoom());
	}
}

void AB2ActiveCameraActor::CheckForOutdatedInteraction()
{
	ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
	if (InteractLocalPlayer && InteractLocalPlayer != LastInteractBeginPlayer)
	{
		// 捞扒 措帆 绢恫 版快衬搁, ActiveCamera 康开 救俊辑 怕弊甫 沁促芭唱 秦辑 捞吧 飘府芭茄 BladeIIPlayer 啊 Destroy 登绊 货肺 spawn 等 版快.
		// 弊巴档 角龙利栏肺 ActorArea肺 飘府芭 窍绰 版快扼搁 Overlap 皑瘤肺 牢秦 BeginActiveCamera 啊 登菌阑 芭绊 ToggleByComponent 肺 飘府芭 窍绰 版快 咯扁肺 坷霸 瞪 巴.
		// ActorArea 肺 飘府芭 窍绰 版快档 捞吧肺 贸府且 荐 乐促搁 弊霸 歹 亮变 窃.
		BeginForNewInteractionCommon(CachedLocalPC, 0.0f, CachedInteractingController);
	}
}

void AB2ActiveCameraActor::UpdateActiveCamera(float DeltaSeconds)
{
	ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
	if (InteractLocalPlayer && CachedLocalPC)
	{
		bool bFallbackToWorldDefault = false; // 拌魂 档吝 备府搁 捞吧 true 肺 技泼.

		if (IsUsingActiveCameraComponent())
		{
			float ExpectedPlayerInputHandlingYaw = ActiveCameraSpringArm->GetComponentRotation().Yaw; // 老馆利牢 ActiveCameraComponent 荤侩窍绰 惑炔俊辑狼 涝仿 勤甸傅 扁夯绵 蔼.
			
			if (bDualCameraBlending)
			{
				ExpectedPlayerInputHandlingYaw = CachedDualCameraBlendedYaw;
				// 扁鸥 迭洒 咯扁辑 鞘夸茄 扒 绝阑 淀.. 墨皋扼 磊眉绰 傲 绊沥捞扼.
			}
			else
			{
				if (CameraMovementMode == EActiveCameraMovementMode::EACMM_ScaledFollowPlayer)
				{
					FVector BasePosToPlayer = InteractLocalPlayer->GetActorLocation() - InitialActiveCameraSpringArmTransform.GetTranslation();
					float PlayerToBaseDistance = BasePosToPlayer.Size();
					BasePosToPlayer.Normalize();

					// CameraPlayerFollowingScale 父怒 胶纳老秦辑 敲饭捞绢 困摹甫 蝶扼啊档废
					FVector ScaledCameraLocation = InitialActiveCameraSpringArmTransform.GetTranslation() + BasePosToPlayer * CameraPlayerFollowingScale * PlayerToBaseDistance;
					ActiveCameraSpringArm->SetWorldLocation(ScaledCameraLocation);
				}
				// CameraMovementMode 啊 FollowPlayer 捞芭唱 Fixed 搁 咯扁辑 贸府 救 窃.

				if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Player)
				{
					SetActiveCameraRotationToTarget(InteractLocalPlayer->GetActorLocation());
				}
				else if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Mob)
				{
					if (CachedTargetMob == NULL) // Begin 矫痢俊 绝阑 荐档 乐栏骨肺 Update 俊辑 八荤
					{
						CachedTargetMob = GetDesiredCameraTargetWaveMob();
					}
					if (CachedTargetMob && !IsValid(CachedTargetMob))
					{
						SetActiveCameraRotationToTarget(CachedTargetMob->GetActorLocation() + TargetMobCameraAdjustSettings.TargetLocationOffset);
					}
					else
					{
						// 各捞 绝绰 惑炔捞扼搁 扁夯 墨皋扼肺 弃归.
						bFallbackToWorldDefault = true;
					}
				}
				else if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Fixed)
				{
					// FixedCameraTargetPosition 篮 relative 肺 秒鞭.
					FTransform FixedTargetWorldTransform = FTransform(FixedCameraTargetPosition) * this->GetTransform();
					SetActiveCameraRotationToTarget(FixedTargetWorldTransform.GetTranslation());
				}
			}

			// 敲饭捞绢 movement 涝仿 贸府甫 困秦 CameraBoom 阑 咯窍埃 雀傈矫挪促. ABladeIIPlayer::MoveForward/MoveRight 曼绊
			// ActiveCameraComp 狼 Yaw 甫 流户栏肺 措涝窍搁 yaw 啊 鞭拜窍霸 函窍绰 老何 惑炔俊辑 巩力啊 登骨肺 焊埃阑 粱 窍绰单 PlayerMoveInputAdjustBoomSyncAlpha 蔼捞 撤阑 荐废 格钎蔼俊辑 钢绢龙 巴烙.
			// 弊府绊 啊阐篮 阿档啊 荤角惑 函窍瘤 臼绰 惑炔俊辑 镜单绝捞 焊埃阑 窍扁档 秦辑.. 啊瓷窍搁 PlayerMoveInputAdjustBoomSyncAlpha 甫 1.0 栏肺..
			float InterpolatedYaw = 
				bUsePlacedActiveCamYawAsInputBase ? InitialActiveCameraSpringArmTransform.Rotator().Yaw : // 弊贰辑.. yaw 啊 鞭拜窍霸 函窍绰 惑炔俊 措茄 绊腊瘤氓.
				FMath::Lerp(InteractLocalPlayer->
				GetCameraYaw(), ExpectedPlayerInputHandlingYaw, PlayerMoveInputAdjustBoomSyncAlpha);
			InteractLocalPlayer->UpdateCameraYaw(InterpolatedYaw);
		}
		else
		{
			// 敲饭捞绢 top-down 墨皋扼狼 CameraBoom 阑 炼累. BladeIIPlayerController 率俊 备厚秦 敌 牢磐其捞胶甫 烹秦辑.
			// CameraMovementMode 绰 绢瞒乔 FollowPlayer 老 抛绊 (溜, 扒靛副 鞘夸 绝澜) CameraTarget 俊 蝶扼 CameraBoom 狼 rotation 阑 炼累茄促.
			if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Player)
			{
				// 捞 版快绰 芭狼 扁粮 top-down 墨皋扼客 瞒捞啊 绝阑 巴牢单, PlayerTopDownCameraSettings 俊 狼茄 檬扁 技泼 挥?

				// 啪促款 墨皋扼 捞侩茄 掂倔喉坊爹矫
				if (bDualCameraBlending)
				{
					if (CachedLocalPC)
					{
						// PlayerTopDownCameraSettings 狼 阿 夸家喊肺 override 茄 巴父 利侩
						if (PlayerTopDownCameraSettings.bOverrideCameraBoomPitch)
						{
							float fPitchA = PlayerTopDownCameraSettings.CameraBoomPitch;
							float fPitchB = DCBExtraPlayerTopDownCameraSettings.CameraBoomPitch;
							float fBlendedPitch = fPitchA + ((fPitchB - fPitchA) * GetDualCameraBlendingWeight());
							CachedLocalPC->SetCameraBoomPitch(fBlendedPitch);
						}

						if (PlayerTopDownCameraSettings.bOverrideCameraBoomYaw)
						{
							float fYawA = PlayerTopDownCameraSettings.CameraBoomYaw;
							float fYawB = DCBExtraPlayerTopDownCameraSettings.CameraBoomYaw;
							float fBlendedYaw = fYawA + ((fYawB - fYawA) * GetDualCameraBlendingWeight());
							CachedLocalPC->SetCameraBoomYaw(fBlendedYaw);
						}

						if (PlayerTopDownCameraSettings.bOverrideCameraBoomRoll)
						{
							float fRollA = PlayerTopDownCameraSettings.CameraBoomRoll;
							float fRollB = DCBExtraPlayerTopDownCameraSettings.CameraBoomRoll;
							float fBlendedRoll = fRollA + ((fRollB - fRollA) * GetDualCameraBlendingWeight());
							CachedLocalPC->SetCameraBoomRoll(fBlendedRoll);
						}

						if (PlayerTopDownCameraSettings.bOverrideCameraBoomArmLength)
						{
							float fArmLengthA = GetExtraAppliedCameraDistance(PlayerTopDownCameraSettings.CameraBoomArmLength, PlayerTopDownCameraSettings.CameraBoomArmLength_Extra);
							float fArmLengthB = GetExtraAppliedCameraDistance(DCBExtraPlayerTopDownCameraSettings.CameraBoomArmLength, DCBExtraPlayerTopDownCameraSettings.CameraBoomArmLength_Extra);
							float fBlendedArmLength = fArmLengthA + ((fArmLengthB - fArmLengthA) * GetDualCameraBlendingWeight());
							CachedLocalPC->SetCameraBoomArmLength(fBlendedArmLength);
						}

						if (PlayerTopDownCameraSettings.bOverrideCameraTargetFocusOffset)
						{
							FVector OffsetA = PlayerTopDownCameraSettings.CameraTargetFocusOffset;
							FVector OffsetB = DCBExtraPlayerTopDownCameraSettings.CameraTargetFocusOffset;
							FVector Offset = OffsetA + ((OffsetB - OffsetA) * GetDualCameraBlendingWeight());
							CachedLocalPC->SetCameraTargetFocusOffset(Offset);
						}

						if (bOverrideFOVAngle && CachedInteractingController)
						{
							float fFovA = FOV_Override;
							float fFovB = DCBExtraFOV_Override;
							float fBlendedFov = fFovA + ((fFovB - fFovA) * GetDualCameraBlendingWeight());
							ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(CachedInteractingController->GetPawn());

							if (pPlayer)
								pPlayer->GetTopDownCameraComponent()->FieldOfView = fBlendedFov;
						}

						// 墨皋扼 肺抛捞记 炼累
						FRotator CamRotation = InteractLocalPlayer->TopDownCameraComponent->GetRelativeTransform().GetRotation().Rotator();

						if (PlayerTopDownCameraSettings.bOverrideCameraPitch)
						{
							float fPitchA = PlayerTopDownCameraSettings.CameraPitch;
							float fPitchB = DCBExtraPlayerTopDownCameraSettings.CameraPitch;
							float fBlendedPitch = fPitchA + ((fPitchB - fPitchA) * GetDualCameraBlendingWeight());
							CamRotation.Pitch = CamRotation.Pitch + fBlendedPitch;
						}

						if (PlayerTopDownCameraSettings.bOverrideCameraYaw)
						{
							float fYawA = PlayerTopDownCameraSettings.CameraYaw;
							float fYawB = DCBExtraPlayerTopDownCameraSettings.CameraYaw;
							float fBlendedYaw = fYawA + ((fYawB - fYawA) * GetDualCameraBlendingWeight());
							CamRotation.Yaw = CamRotation.Yaw + fBlendedYaw;
						}

						InteractLocalPlayer->TopDownCameraComponent->SetRelativeRotation(CamRotation);
					}					
				}
			}
			else if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Mob)
			{
				if (CachedTargetMob == NULL) // Begin 矫痢俊 绝阑 荐档 乐栏骨肺 Update 俊辑 八荤
				{
					CachedTargetMob = GetDesiredCameraTargetWaveMob();
				}

				if (CachedTargetMob && !IsValid(CachedTargetMob))
				{
					SetPlayerCameraBoomRotationToTarget(
						CachedTargetMob->GetActorLocation() + TargetMobCameraAdjustSettings.TargetLocationOffset,
						// 咯扁浚 俺喊 颇扼固磐俊 措茄 offset 苞 overriding 捞 粱 乐澜.
						TargetMobCameraAdjustSettings.BoomRotOffsetPitch, TargetMobCameraAdjustSettings.BoomRotOffsetYaw,
						(TargetMobCameraAdjustSettings.bOverrideCameraBoomArmLength ? 
							GetExtraAppliedCameraDistance(TargetMobCameraAdjustSettings.CameraBoomArmLength, TargetMobCameraAdjustSettings.CameraBoomArmLength_Extra)
							: 0.0f)
						);

					// 各 媚促焊霸..
					SetPlayerCameraRotationToTarget(
						CachedTargetMob->GetActorLocation() + TargetMobCameraAdjustSettings.TargetLocationOffset,
						TargetMobCameraAdjustSettings.CamRotOffsetPitch, TargetMobCameraAdjustSettings.CamRotOffsetYaw
						);
					
				}
				else
				{
					// 各捞 绝绰 惑炔捞扼搁 扁夯 墨皋扼肺 弃归.
					//bFallbackToWorldDefault = true;
				}
			}
			else if (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Fixed)
			{
				// FixedCameraTargetPosition 篮 relative 肺 秒鞭.
				FTransform FixedTargetWorldTransform = FTransform(FixedCameraTargetPosition) * this->GetTransform();
				SetPlayerCameraBoomRotationToTarget(FixedTargetWorldTransform.GetTranslation());

				// CameraMovementMode 啊 FollowPlayer 搁辑 TargetingMode 啊 Fixed 牢 版快 FixedCameraTargetPosition 困摹俊 敲饭捞绢啊 乐阑 版快 墨皋扼啊 阂救沥窍霸 凳.
				// 酒抗 秦寸 炼钦阑 荤侩窍瘤 臼绰 吧肺 且 荐档 乐瘤父, FixedCameraTargetPosition 阑 肋 技泼秦辑 荤侩秦 杭 荐档.
			}
		}

		if (bFallbackToWorldDefault)
		{
			CachedLocalPC->ApplyWorldDefaultCamera(0.0f);
		}
	}
}

void AB2ActiveCameraActor::SetupForConsistentPlayerTopDownCamera()
{
	if (CachedLocalPC)
	{
		// PlayerTopDownCameraSettings 狼 阿 夸家喊肺 override 茄 巴父 利侩
		CachedLocalPC->SetCameraBoomLockedViewTarget(PlayerTopDownCameraSettings.bCameraBoomLockedViewTarget);

		if (PlayerTopDownCameraSettings.bOverrideCameraBoomPitch)
		{
			CachedLocalPC->SetCameraBoomPitch(PlayerTopDownCameraSettings.CameraBoomPitch);
		}

		if (PlayerTopDownCameraSettings.bOverrideCameraBoomYaw)
		{
			CachedLocalPC->SetCameraBoomYaw(PlayerTopDownCameraSettings.CameraBoomYaw);
		}

		if (PlayerTopDownCameraSettings.bOverrideCameraBoomRoll)
		{
			CachedLocalPC->SetCameraBoomRoll(PlayerTopDownCameraSettings.CameraBoomRoll);
		}

		if (PlayerTopDownCameraSettings.bOverrideCameraBoomArmLength)
		{
			CachedLocalPC->SetCameraBoomArmLength(GetExtraAppliedCameraDistance(
				PlayerTopDownCameraSettings.CameraBoomArmLength, PlayerTopDownCameraSettings.CameraBoomArmLength_Extra));
		}

		if (PlayerTopDownCameraSettings.bOverrideCameraTargetFocusOffset)
		{
			CachedLocalPC->SetCameraTargetFocusOffset(PlayerTopDownCameraSettings.CameraTargetFocusOffset);
		}

		
		// 墨皋扼 肺抛捞记 炼累
		if (ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter())
		{
			FRotator CamRotation = InteractLocalPlayer->TopDownCameraComponent->GetRelativeTransform().GetRotation().Rotator();

			if (PlayerTopDownCameraSettings.bOverrideCameraPitch)
				CamRotation.Pitch = CamRotation.Pitch + PlayerTopDownCameraSettings.CameraPitch;

			if (PlayerTopDownCameraSettings.bOverrideCameraYaw)
				CamRotation.Yaw = CamRotation.Yaw + PlayerTopDownCameraSettings.CameraYaw;

			//ActiveCameraSpringArm->SetWorldRotation(CamRotation);
			InteractLocalPlayer->TopDownCameraComponent->SetRelativeRotation(CamRotation);

			// SetViewTargetWithBlend 绰 捞 捞饶俊 蝶肺 阂妨具 窃.
		}
	}
}

bool AB2ActiveCameraActor::IsUsingActiveCameraComponent()
{
	if (CameraMovementMode == EActiveCameraMovementMode::EACMM_FollowPlayer)
	{
		// 老窜, FollowPlayer 葛靛俊辑绰 敲饭捞绢 top-down 墨皋扼甫 荤侩茄促绰 啊沥牢单, 父甸搁辑 官拆 荐档 乐促.
		return false;
	}
	return true;
}

void AB2ActiveCameraActor::SetActiveCameraRotationToTarget(FVector InViewTargetLocation)
{
	FVector ToTarget = (InViewTargetLocation - ActiveCameraSpringArm->GetComponentLocation());
	ToTarget.Normalize();
	// 角力肺绰 SpringArm 阑 炼例.
	ActiveCameraSpringArm->SetWorldRotation(ToTarget.Rotation());
}

void AB2ActiveCameraActor::SetPlayerCameraBoomRotationToTarget(FVector InViewTargetLocation, float RotOffsetPitch /*= 0.0f*/, float RotOffsetYaw /*= 0.0f*/, float ArmLengthOverride /*= 0.0f*/)
{
	ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
	if (InteractLocalPlayer)
	{
		FVector ToTarget = (InViewTargetLocation - InteractLocalPlayer->GetActorLocation());
		ToTarget.Normalize();
		FRotator CamBoomRotation = ToTarget.Rotation();

		// RotOffset 篮 弊成 啊窃
		CamBoomRotation.Pitch = CamBoomRotation.Pitch + RotOffsetPitch;

		// Boom Ptich 惑亲急 窍茄急
		if (bUseCameraBoomRotationClamp)
			CamBoomRotation.Pitch = FMath::Clamp<float>(CamBoomRotation.Pitch, MinCameraBoomRotationClamp, MaxCameraBoomRotationClamp);
		
		InteractLocalPlayer->UpdateCameraPitch(CamBoomRotation.Pitch);
		InteractLocalPlayer->UpdateCameraYaw(CamBoomRotation.Yaw + RotOffsetYaw);
		if (ArmLengthOverride > 0.0f) // ArmLengthOverride 绰 0 焊促 努 锭父
		{
			InteractLocalPlayer->UpdateCameraDistance(ArmLengthOverride);
		}
	}
}

void AB2ActiveCameraActor::SetPlayerCameraRotationToTarget(FVector InViewTargetLocation, float RotOffsetPitch /*= 0.0f*/, float RotOffsetYaw /*= 0.0f*/)
{
	ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();
	if (InteractLocalPlayer)
	{
		if (InteractLocalPlayer->TopDownCameraComponent)
		{
			FVector ToTarget = (InViewTargetLocation - InteractLocalPlayer->TopDownCameraComponent->GetComponentLocation());
			ToTarget.Normalize();
			FRotator CamBoomRotation = ToTarget.Rotation();

			CamBoomRotation.Pitch = CamBoomRotation.Pitch + RotOffsetPitch;
			CamBoomRotation.Yaw = CamBoomRotation.Yaw + RotOffsetYaw;

			InteractLocalPlayer->TopDownCameraComponent->SetWorldRotation(CamBoomRotation);
		}		
	}
}

ABladeIICharacter* AB2ActiveCameraActor::GetDesiredCameraTargetWaveMob()
{
	ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(this));
	AB2MonsterSpawnPool* TargetMobSP = B2GM ? B2GM->GetActiveSpawnPool() : NULL; // 饭骇 矫累 矫 沥秦咙.

	if (TargetMobSP)
	{
		int32 WaveNum = 0;
		int32 MobIndex = 0;
		ParseForWaveMobSelectKeyword(TargetMobSelectKeyword, WaveNum, MobIndex); // 巩磊凯肺 瘤沥茄 可记俊辑 锅龋甸 哗咳.

		TArray<ABladeIICharacter*> InterestedWaveMobs;
		TargetMobSP->GetWaveMobList(WaveNum, InterestedWaveMobs, false); // 磷绢啊绰 赤籍档 器窃秦辑 啊廉坷瘤父 咯扁辑 龋免窍绰 鸥捞怪俊 弊霸 瞪鳖..?

		for (int32 WMI = 0; WMI < InterestedWaveMobs.Num(); ++WMI)
		{
			ABladeIICharacter* CurrMob = InterestedWaveMobs[WMI];
			if (CurrMob)
			{
				if (CurrMob->GetSpawnPoolWaveObjIndex() == MobIndex) // 荤角惑 Wave 啊 矫累等 惑炔捞 酒聪搁 茫绰 牢郸胶狼 mob 捞 绝阑 荐档 乐促.
				{
					return CurrMob;
				}
			}
		}
	}
	return NULL;
}

void AB2ActiveCameraActor::ConditionalBeginTransitionInputDisable(bool bTransitIn)
{
	ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();

	// 墨皋扼 况农 函悼俊 蝶扼 敲饭捞绢 捞悼 涝仿俊 去急捞 棵 荐 乐栏骨肺 老沥 矫埃 悼救 涝仿阑 阜绰促.
	// 敲饭捞绢 Top-down 墨皋扼甫 荤侩窍绰 葛靛俊急 鞘夸绝阑 淀.. 沁绰单 movement, target 葛滴 player 啊 酒囱 捞惑篮 鞘夸且 淀.
	// 掂倔墨皋扼 葛靛俊辑绰 荤侩
	if ((bDualCameraBlending || CameraMovementMode != EActiveCameraMovementMode::EACMM_FollowPlayer || CameraTargetingMode != EActiveCameraTargetingMode::EACTM_Player) &&
		TransitionInputDisableTime > 0.0f && CachedLocalPC && InteractLocalPlayer)
	{
		if (bTransitIn)
		{
			CachedLocalPC->bDisableMoveInputForActiveCameraTransition = true;
			bTransitInInputDisabled = true;
			GetWorldTimerManager().SetTimer(TransitInTimeTimerHandle, this, &AB2ActiveCameraActor::TransitInTimerCB, TransitionInputDisableTime, false);

			if (TransitionAutoMoveScale > 0.0f)
			{
				// 涝仿捞 厚劝己拳 等 悼救 泅犁 困摹俊辑 距埃 菊栏肺 捞悼矫挪促. 版拌俊辑 粱 钢绢瘤档废 せ
								
				const float MoveDistance = InteractLocalPlayer->GetDefaultHalfHeight() * 2.0f * TransitionAutoMoveScale;
				const FVector MoveTargetLocation = InteractLocalPlayer->GetActorLocation() + (InteractLocalPlayer->GetVelocity().GetSafeNormal() * MoveDistance);
				
				//UNavigationSystem::SimpleMoveToLocation(CachedLocalPC, MoveTargetLocation);
			}
		}
		else
		{
			// 趣矫扼档 TransitIn 档吝捞扼搁 吝窜
			if (bTransitInInputDisabled)
			{
				GetWorldTimerManager().ClearTimer(TransitInTimeTimerHandle);
				bTransitInInputDisabled = false;
			}

			CachedLocalPC->bDisableMoveInputForActiveCameraTransition = true;
			bTransitOutInputDisabled = true;
			GetWorldTimerManager().SetTimer(TransitOutTimeTimerHandle, this, &AB2ActiveCameraActor::TransitOutTimerCB, TransitionInputDisableTime, false);
		}
	}
}

void AB2ActiveCameraActor::TransitInTimerCB()
{
	GetWorldTimerManager().ClearTimer(TransitInTimeTimerHandle);
	// CachedLocalPC 啊 绝歹扼档 咯窍埃 bDisableMoveInput 篮 贸府秦具..
	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (B2PC)
	{
		B2PC->bDisableMoveInputForActiveCameraTransition = false;
	}
	bTransitInInputDisabled = false;
}

void AB2ActiveCameraActor::TransitOutTimerCB()
{
	GetWorldTimerManager().ClearTimer(TransitOutTimeTimerHandle);
	// CachedLocalPC 啊 绝歹扼档 咯窍埃 bDisableMoveInput 篮 贸府秦具..
	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (B2PC)
	{
		B2PC->bDisableMoveInputForActiveCameraTransition = false;
	}
	bTransitOutInputDisabled = false;
}

bool AB2ActiveCameraActor::IsOneOfBeginToggleComp(class UShapeComponent* InTestComp)
{
	if (InTestComp == NULL)
	{
		return false;
	}

	for (int32 BCI = 0; BCI < ToggleMode_BeginComponents.Num(); ++BCI)
	{
		if (InTestComp->GetName() == ToggleMode_BeginComponents[BCI].ToString())
		{
			return true;
		}
	}

	return false;
}

bool AB2ActiveCameraActor::IsOneOfEndToggleComp(class UShapeComponent* InTestComp)
{
	if (InTestComp == NULL)
	{
		return false;
	}

	for (int32 BCI = 0; BCI < ToggleMode_EndComponents.Num(); ++BCI)
	{
		if (InTestComp->GetName() == ToggleMode_EndComponents[BCI].ToString())
		{
			return true;
		}
	}

	return false;
}

UCameraComponent* AB2ActiveCameraActor::FindExtraCamCompForDualCameraBlending()
{
	TInlineComponentArray<UCameraComponent*> AllCameras;
	GetComponents<UCameraComponent>(AllCameras);

	for (UCameraComponent* ThisCam : AllCameras)
	{
		if (ThisCam && ThisCam->GetFName() == DCBExtraCamName)
		{
			return ThisCam;
		}
	}
	return NULL;
}

USceneComponent* AB2ActiveCameraActor::FindSceneComponentForDualCameraBlending(FName CompName)
{
	TInlineComponentArray<USceneComponent*> AllComponents;
	GetComponents<USceneComponent>(AllComponents);

	for (USceneComponent* ThisComp : AllComponents)
	{
		if (ThisComp && ThisComp->GetFName() == CompName)
		{
			return ThisComp;
		}
	}
	return NULL;
}

float AB2ActiveCameraActor::GetDualCameraBlendingWeight()
{
	UCameraComponent* CamA = ActiveCameraComp;
	UCameraComponent* CamB = FindExtraCamCompForDualCameraBlending();
	ABladeIIPlayer* InteractLocalPlayer = GetInteractPlayerCharacter();

	if (CameraMovementMode == EActiveCameraMovementMode::EACMM_Fixed && (!CamA || !CamB))
		return 0.0f;

	if (InteractLocalPlayer && IsValid(InteractLocalPlayer))
	{
		USceneComponent* FlagA = NULL;
		USceneComponent* FlagB = NULL;

		if (DCBFlagName != NAME_None)
			FlagA = FindSceneComponentForDualCameraBlending(DCBFlagName);

		if (DCBExtraFlagName != NAME_None)
			FlagB = FindSceneComponentForDualCameraBlending(DCBExtraFlagName);
		
		//焊埃 矫累 扁霖痢
		FVector vPointA;
		//焊埃 辆丰 扁霖痢
		FVector vPointB;

		// 敲贰弊 汲沥登菌栏搁 弊困摹肺 窍绊 酒聪搁 墨皋扼困摹 捞侩窃
		if (FlagA)
			vPointA = FlagA->GetComponentLocation();
		else
			vPointA = CamA->GetComponentLocation();
				
		if (FlagB)
			vPointB = FlagB->GetComponentLocation();
		else
			vPointB = CamB->GetComponentLocation();

		// 何磊楷胶矾况辑 纳腐磐 困摹蔼阑 A, B 滴 瘤痢荤捞肺 焊沥茄促(郴利 捞侩).
		FVector vGoalPos = vPointB - vPointA;
		FVector vCurPos = InteractLocalPlayer->GetActorLocation() - vPointA;

		// 焊沥等 泅犁困摹 氦磐 荤捞令
		float fCurPosSize = FVector::DotProduct(vGoalPos.GetSafeNormal(KINDA_SMALL_NUMBER), vCurPos);
				
		float fWeight = fCurPosSize / vGoalPos.Size();

		if (fWeight < KINDA_SMALL_NUMBER) // 如茄 汲沥俊辑 唱鸥朝 荐 乐绰 惑炔篮 酒聪摆瘤父..
			return 0.0f;

		if (fWeight >= 1.0f) // 趣矫 1逞绰霸 鞘夸窍搁 弊锭 力芭
			return 1.0f;

		return fWeight;
	}

	return 0.0f;
}

void AB2ActiveCameraActor::ChangeCameraOwnerPlayer(ABladeIIPlayer* targetPlayer, float OverrideBlendingTime)
{	
	ABladeIIPlayerController* B2PC = Cast<ABladeIIPlayerController>(GetWorld()->GetFirstPlayerController());

	if (B2PC && targetPlayer && targetPlayer->IsAlive())
	{
		ABladeIIWorldSettings* B2WS = GetWorld() ? Cast<ABladeIIWorldSettings>(GetWorld()->GetWorldSettings()) : NULL;

		if (B2WS)
		{
			B2WS->ApplyDefaultPlayerCameraSettingForPlayerPawn(targetPlayer);

			if (PlayerTopDownCameraSettings.bOverrideCameraBoomPitch)
				targetPlayer->UpdateCameraPitch(PlayerTopDownCameraSettings.CameraBoomPitch);

			if (PlayerTopDownCameraSettings.bOverrideCameraBoomYaw)
				targetPlayer->UpdateCameraYaw(PlayerTopDownCameraSettings.CameraBoomYaw);

			if (PlayerTopDownCameraSettings.bOverrideCameraBoomRoll)
				targetPlayer->UpdateCameraRoll(PlayerTopDownCameraSettings.CameraBoomRoll);

			if (PlayerTopDownCameraSettings.bOverrideCameraBoomArmLength)
				targetPlayer->UpdateCameraDistance(GetExtraAppliedCameraDistance(
					PlayerTopDownCameraSettings.CameraBoomArmLength, PlayerTopDownCameraSettings.CameraBoomArmLength_Extra));

			if (PlayerTopDownCameraSettings.bOverrideCameraTargetFocusOffset)
				targetPlayer->UpdateCameraTargetFocusOffset(PlayerTopDownCameraSettings.CameraTargetFocusOffset);
		}

		//劝己拳 茄锅 秦淋
		ForceBeginActiveCamera();
		float LastBlendingTime = OverrideBlendingTime >= 0.0f ? OverrideBlendingTime : BlendInTime;
		B2PC->SetViewTargetWithBlend(targetPlayer, LastBlendingTime, EViewTargetBlendFunction::VTBlend_Cubic, 0, true);
	}
}

#if WITH_EDITOR
void AB2ActiveCameraActor::RefreshEditingComp(bool bRegenerateStaticComp /*= true*/)
{
	DestroyEditingComponents(bRegenerateStaticComp);

	if(CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Fixed)
	{
		UStaticMesh* EditingCompMesh = LoadObject<UStaticMesh>(NULL, TEXT("/Engine/BasicShapes/Cone.Cone"));
		// TEXT("/Engine/BasicShapes/Cone.Cone")
		if(EditingCompMesh)
		{
			UStaticMeshComponent* EditingComp = NULL;

			// 货肺 积己窍绰 可记捞 酒聪扼搁 扁粮 芭 荤侩
			if(TargetPositionEditingComp.Num() == 0)
			{
				EditingComp = NewObject<UStaticMeshComponent>(this, FName(TEXT("TargetPositionEditingComp")), RF_Transient);
				if(EditingComp)
				{
					TargetPositionEditingComp.Add(EditingComp);
				}
			}
			else
			{
				EditingComp = TargetPositionEditingComp[0];
			}

			if(EditingComp)
			{
				EditingComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false)); // EditingComp 狼 RelativeLocation 阑 荤侩且 巴捞骨肺 馆靛矫 root 俊 attach
				EditingComp->RegisterComponent();
				EditingComp->SetStaticMesh(EditingCompMesh);
				EditingComp->SetVisibility(true);
				// 绢瞒乔 霸烙俊急 积扁瘤档 臼阑 局甸捞瘤父 
				EditingComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				EditingComp->SetHiddenInGame(true);
				EditingComp->SetMaterial(0, LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial")));
				
				// Movement, Target 葛滴 Fixed 捞搁 被捞 蝶肺 CameraTargetPosition 阑 临 芭 绝捞 ActiveCameraComp 狼 硅摹等 困摹 棺 雀傈蔼阑 弊措肺 荤侩窍搁 登摆瘤..
				if(CameraMovementMode == EActiveCameraMovementMode::EACMM_Fixed)
				{					
					EditingComp->SetWorldLocation(ActiveCameraSpringArm->GetComponentTransform().TransformPosition(FVector(300.0f, 0.0f, 0.0f)));
					FixedCameraTargetPosition = EditingComp->GetRelativeLocation();
				}
				else
				{
					EditingComp->SetRelativeLocation(FixedCameraTargetPosition); // FixedCameraTargetPosition 篮 relative 秒鞭
				}
			}
		}
	}

	// ShapeComponent 甸俊 措茄 救郴咆胶飘甸 积己. ToggleMode_BeginComponents, ToggleMode_EndComponents 俊 殿废茄 巴甸篮 漂喊洒 眠啊 沥焊..
	TArray<UActorComponent*> ShapeComps;
	GetComponents(ShapeComps);
	for (int32 SCI = 0; SCI < ShapeComps.Num(); ++SCI)
	{
		UShapeComponent* CurrShapeComp = Cast<UShapeComponent>(ShapeComps[SCI]);
		if (CurrShapeComp == NULL)
		{
			continue;
		}

		UTextRenderComponent* NewInfoTextComp = NewObject<UTextRenderComponent>(this, *FString::Printf(TEXT("ShapeCompInfoText_%d"), SCI), RF_Transient);
		if (NewInfoTextComp == NULL)
		{
			continue;
		}

		NewInfoTextComp->AttachToComponent(CurrShapeComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		NewInfoTextComp->RegisterComponent();
		NewInfoTextComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NewInfoTextComp->SetVisibility(true);
		NewInfoTextComp->SetHiddenInGame(true);
		NewInfoTextComp->SetHorizontalAlignment(EHTA_Center);
		NewInfoTextComp->SetWorldSize(30.0f);
		NewInfoTextComp->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
		NewInfoTextComp->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // 困摹绰 利寸洒 嘎苗霖促
		
		const bool bIsOneOfBeginToggleComp = IsOneOfBeginToggleComp(CurrShapeComp);
		const bool bIsOneOfEndToggleComp = IsOneOfEndToggleComp(CurrShapeComp);

		// Component 侩档 蝶扼 促福霸 钎泅且 沥焊甸.
		FColor CompTextColor(255, 0, 0);
		FString CompInfoStr = CurrShapeComp->GetName() + TEXT("\n");

		if (TriggerMode == EActiveCameraTriggerMode::EACTRG_ToggleByComponent)
		{
			if (bIsOneOfBeginToggleComp && bIsOneOfEndToggleComp)
			{
				CompInfoStr += TEXT("WRONG Setting!!"); // Begin/End 笛 促 悼矫俊 技泼等 扒 肋给烙.
				CompTextColor = FColor(255, 0, 127);
			}
			else
			{
				if (bIsOneOfBeginToggleComp)
				{
					CompInfoStr += TEXT("Begin Toggle");
					CompTextColor = FColor(255, 0, 0);
				}
				if (bIsOneOfEndToggleComp)
				{
					CompInfoStr += TEXT("End Toggle");
					CompTextColor = FColor(0, 0, 255);
				}
			}
		}
		else
		{
			CompInfoStr += TEXT("ActiveCamera Area");
			CompTextColor = FColor(0, 255, 127);
		}
				
		
		NewInfoTextComp->SetTextRenderColor(CompTextColor);
		NewInfoTextComp->SetText(FText::FromString(CompInfoStr));
		
		ShapeCompInfoText.Add(NewInfoTextComp);
	}

}

void AB2ActiveCameraActor::SyncFromEditingCompPlacement()
{
	if(CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Fixed)
	{
		if(TargetPositionEditingComp.Num() > 0 && TargetPositionEditingComp[0])
		{
			FixedCameraTargetPosition = TargetPositionEditingComp[0]->GetRelativeLocation();
		}
	}
}

void AB2ActiveCameraActor::DestroyEditingComponents(bool bDestroyStaticCompToo)
{
	if(bDestroyStaticCompToo)
	{
		for(int32 CI = 0; CI < TargetPositionEditingComp.Num(); ++CI)
		{
			if(TargetPositionEditingComp[CI])
			{
				TargetPositionEditingComp[CI]->DestroyComponent();
			}
		}
		TargetPositionEditingComp.Empty();
	}

	for (int32 SCI = 0; SCI < ShapeCompInfoText.Num(); ++SCI)
	{
		if (ShapeCompInfoText[SCI])
		{
			ShapeCompInfoText[SCI]->DestroyComponent();
		}
	}
	ShapeCompInfoText.Empty();

	// 扁鸥 哪器惩飘啊 趣矫 眠啊啊 登搁..
}

void AB2ActiveCameraActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName Name_ToggleMode_BeginComponents = GET_MEMBER_NAME_CHECKED(AB2ActiveCameraActor, ToggleMode_BeginComponents);
	const FName Name_ToggleMode_EndComponents = GET_MEMBER_NAME_CHECKED(AB2ActiveCameraActor, ToggleMode_EndComponents);

	FProperty* PropertyThatChanged = PropertyChangedEvent.Property;
	FName PropertyName = PropertyThatChanged != NULL ? PropertyThatChanged->GetFName() : NAME_None;

	PlayerMoveInputAdjustBoomSyncAlpha = FMath::Clamp(PlayerMoveInputAdjustBoomSyncAlpha, 0.0f, 1.0f);
	CameraPlayerFollowingScale = FMath::Clamp(CameraPlayerFollowingScale, 0.0f, 1.0f);

	if (PropertyName == Name_ToggleMode_BeginComponents || PropertyName == Name_ToggleMode_EndComponents)
	{
		// 剧促府 陛瘤 ぇぇ
		bool bCompNameExistForBothBeginAndEnd = false;
		for (int32 BCI = 0; BCI < ToggleMode_BeginComponents.Num(); ++BCI)
		{
			for (int32 ECI = 0; ECI < ToggleMode_EndComponents.Num(); ++ECI)
			{
				if (ToggleMode_BeginComponents[BCI] == ToggleMode_EndComponents[ECI])
				{
					bCompNameExistForBothBeginAndEnd = true;
					break;
				}
			}
		}


		if (bCompNameExistForBothBeginAndEnd)
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("[版绊] ToggleMode_BeginComponents 客 ToggleMode_EndComponents 俊 悼矫俊 殿废等 哪器惩飘啊 乐嚼聪促."))
				));
#endif
		}


		TArray<UActorComponent*> AllShapeComps;
		GetComponents(AllShapeComps);
		// 角力肺 绝绰 哪器惩飘 捞抚阑 持菌绰瘤 八荤.
		for (int32 BCI = 0; BCI < ToggleMode_BeginComponents.Num(); ++BCI)
		{
			if (ToggleMode_BeginComponents[BCI] == NAME_None) // 酒流 技泼窍瘤 臼篮 扒 菩胶.
			{
				continue;
			}

			bool bNameNotExist = true;
			for (int32 SCI = 0; SCI < AllShapeComps.Num(); ++SCI)
			{
				if (AllShapeComps[SCI]->GetName() == ToggleMode_BeginComponents[BCI].ToString())
				{
					bNameNotExist = false;
					break;
				}
			}
			if (bNameNotExist == true)
			{
#if !PLATFORM_MAC
				FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
					FString::Printf(TEXT("[版绊] %s (捞)扼绰 捞抚狼 Shape (Collision) Component 啊 绝嚼聪促."), *(ToggleMode_BeginComponents[BCI].ToString()))
					));
#endif
			}
		}

		for (int32 BCI = 0; BCI < ToggleMode_EndComponents.Num(); ++BCI)
		{
			if (ToggleMode_EndComponents[BCI] == NAME_None) // 酒流 技泼窍瘤 臼篮 扒 菩胶.
			{
				continue;
			}

			bool bNameNotExist = true;
			for (int32 SCI = 0; SCI < AllShapeComps.Num(); ++SCI)
			{
				if (AllShapeComps[SCI]->GetName() == ToggleMode_EndComponents[BCI].ToString())
				{
					bNameNotExist = false;
					break;
				}
			}
			if (bNameNotExist == true)
			{
#if !PLATFORM_MAC
				FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
					FString::Printf(TEXT("[版绊] %s (捞)扼绰 捞抚狼 Shape (Collision) Component 啊 绝嚼聪促."), *(ToggleMode_EndComponents[BCI].ToString()))
					));
#endif
			}
		}
	}

	SetEditConditionProperties();

	RefreshEditingComp();
}

void AB2ActiveCameraActor::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (bFinished)
	{
		// ActiveCameraComp 措脚 SpringArm 阑 力绢秦具 窃.
		// 弊贰档 ActiveCameraComp 啊 努腐窍扁俊绰 亮栏聪 ActiveCameraComp 俊叼飘啊 SpringArm 俊叼飘肺 登档废 蔼阑 嘎苗林档废 窍磊.
		if(ActiveCameraComp->GetRelativeLocation() != FVector(0.0f,0.0f,0.0f))
		{
			// 沥侥栏肺绰 SocketOffset 苞 TargetOffset 档 嘎苗林绢具 窍瘤父 被捞 弊犯霸鳖瘤..
			FTransform SupposedSpringArmTransform = FTransform(FVector(ActiveCameraSpringArm->TargetArmLength, 0.0f, 0.0f)) * ActiveCameraComp->GetComponentTransform();
			ActiveCameraSpringArm->SetWorldLocation(SupposedSpringArmTransform.GetTranslation());
			ActiveCameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		}
		if(ActiveCameraComp->GetRelativeRotation() != FRotator(0.0f,0.0f,0.0f))
		{
			ActiveCameraSpringArm->SetWorldRotation(ActiveCameraComp->GetComponentRotation());
			ActiveCameraComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		}

		// 困摹 教农 饶 Refresh
		SyncFromEditingCompPlacement();
		// Regenerate 绝捞 困摹父 教农窍绰 可记栏肺 call. 己瓷 锭巩捞 酒聪扼 捞犯霸 窍瘤 臼栏搁 概 捞悼付促 急琶 器目胶甫 酪绢滚妨辑 粮唱 阂祈窃.
		RefreshEditingComp(false);
	}
}

void AB2ActiveCameraActor::SetEditConditionProperties()
{
	bDualCameraBlending_Inverse = !bDualCameraBlending;
	bCameraMovementMode_ScaledFollowingPlayer = (CameraMovementMode == EActiveCameraMovementMode::EACMM_ScaledFollowPlayer && !bDualCameraBlending);
	bCameraTargetingMode_Mob = (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Mob && !bDualCameraBlending);
	bCameraTargetingMode_Mob_with_PlayerCameraBoom = (CameraMovementMode == EActiveCameraMovementMode::EACMM_FollowPlayer && CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Mob && !bDualCameraBlending);
	bCameraTargetingMode_Fixed = (CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Fixed && !bDualCameraBlending);
	bCameraMovementTarget_BothPlayer = (CameraMovementMode == EActiveCameraMovementMode::EACMM_FollowPlayer && CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Player);
	bUseActiveCameraComponentMode = IsUsingActiveCameraComponent();
	bTriggerMode_ToggleByComponent = (TriggerMode == EActiveCameraTriggerMode::EACTRG_ToggleByComponent);
	bUseDCBExtraPlayerTopDownCameraSettings = (CameraMovementMode == EActiveCameraMovementMode::EACMM_FollowPlayer&& CameraTargetingMode == EActiveCameraTargetingMode::EACTM_Player && bDualCameraBlending);
	bUseDCBExtraFOV_Override = bUseDCBExtraPlayerTopDownCameraSettings && bOverrideFOVAngle;
}
#endif