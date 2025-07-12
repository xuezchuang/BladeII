// Fill out your copyright notice in the Description page of Project Settings.


#include "B2NoDamageZone.h"
#include "BladeIIUtil.h"

// Sets default values
AB2NoDamageZone::AB2NoDamageZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponents = CreateDefaultSubobject<USceneComponent>(TEXT("CollisionComponents"));

	NoDamageFX_Components.Empty();
	NoDamageFX_PS = NULL;
}

// Called when the game starts or when spawned
void AB2NoDamageZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AB2NoDamageZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckOverlap();
}

void AB2NoDamageZone::CheckNoDamageFX(class ABladeIIPlayer* TargetPlayer, bool isOverlap)
{
	//if (isOverlap)
	//{
	//	if (!NoDamageFX_PS)
	//		return;

	//	for (UParticleSystemComponent* pPSC : NoDamageFX_Components)
	//	{
	//		if (pPSC->GetAttachParent() == TargetPlayer->GetRootComponent())
	//			return;
	//	}

	//	UParticleSystemComponent* pPSC = CreateParticleSystemHelper(NoDamageFX_PS, GetWorld(), TargetPlayer, false);
	//	pPSC->RegisterComponent();
	//	pPSC->AttachToComponent(TargetPlayer->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//	pPSC->Activate(true);
	//	NoDamageFX_Components.Add(pPSC);

	//	// 겹치지 않으면. 데미지 받음
	//	TargetPlayer->SetCanBeDamagedForB2(!isOverlap);
	//}
	//else
	//{
	//	UParticleSystemComponent* pRemovePSC = NULL;

	//	for (UParticleSystemComponent* pPSC : NoDamageFX_Components)
	//	{
	//		if (pPSC->GetAttachParent() == TargetPlayer->GetRootComponent())
	//			pRemovePSC = pPSC;
	//	}

	//	if (pRemovePSC)
	//	{
	//		pRemovePSC->Deactivate();
	//		pRemovePSC->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, false));
	//		pRemovePSC->UnregisterComponent();
	//		NoDamageFX_Components.Remove(pRemovePSC);

	//		// 겹치지 않으면. 데미지 받음
	//		TargetPlayer->SetCanBeDamagedForB2(!isOverlap);

	//		// 종료 이펙트는 따로 붙여줌.
	//		if (NoDamageFX_PS_End)
	//		{
	//			// 이렇게 쓰면 플레이후 자동 제거 될듯
	//			UParticleSystemComponent* pEndPSC = UGameplayStatics::SpawnEmitterAttached(NoDamageFX_PS_End, TargetPlayer->GetRootComponent());
	//			if (pEndPSC)
	//			{
	//				//pEndPSC->IsStaticVisible = false;
	//				//pEndPSC->bUseOwnerCustomTimeDilation = false;
	//			}
	//		}
	//	}
	//}
}

void AB2NoDamageZone::CheckOverlap()
{
	//for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
	//{
	//	// 플레이어한테만 적용해봄
	//	ABladeIIPlayer* pPlayer = Cast<ABladeIIPlayer>(*Iterator);

	//	if (pPlayer && pPlayer->IsAlive())
	//	{
	//		bool isOverlap = false;

	//		TArray<USceneComponent*> Comps;

	//		CollisionComponents->GetChildrenComponents(false, Comps);

	//		for (USceneComponent* pCollisionComp : Comps)
	//		{
	//			UShapeComponent* CastedShapeComp = Cast<UShapeComponent>(pCollisionComp);
	//			if (CastedShapeComp)
	//			{
	//				if (CastedShapeComp->IsOverlappingActor(pPlayer))
	//					isOverlap = true;
	//			}
	//		}
	//		
	//		if (pPlayer->IsQTEMounting())
	//			isOverlap = false;

	//		CheckNoDamageFX(pPlayer, isOverlap);
	//	}
	//}	
}

