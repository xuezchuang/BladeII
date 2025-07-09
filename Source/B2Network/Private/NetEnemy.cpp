// Fill out your copyright notice in the Description page of Project Settings.


#include "NetEnemy.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
//#include "CaiYunCharacter.h"
//#include "Components/DecalComponent.h"

// Sets default values
ANetEnemy::ANetEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
