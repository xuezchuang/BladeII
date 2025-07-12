// Fill out your copyright notice in the Description page of Project Settings.
#include "AnimNotify_B2Damage.h"
#include "BladeII.h"
#include "BladeIIDamageType.h"


UAnimNotify_B2Damage::UAnimNotify_B2Damage(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}
#if WITH_EDITOR
void UAnimNotify_B2Damage::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	PostInitProperties();
}
#endif //WITH_EDITOR


void UAnimNotify_B2Damage::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	SerializeBPProperties();
}

void UAnimNotify_B2Damage::PostInitProperties()
{
	Super::PostInitProperties();

	SerializeBPProperties();
}

void UAnimNotify_B2Damage::SerializeBPProperties()
{
	if (UClass* MyClass = GetClass())
	{
		static const FName DamageInfoName = TEXT("B2DamageInfo");
		FStructProperty* DamageProp = Cast<FStructProperty>(MyClass->FindPropertyByName(DamageInfoName));
		if (DamageProp != nullptr)
		{
			DamageInfo = *(DamageProp->ContainerPtrToValuePtr<FDamageInfo>(this));
			DamageHash = DamageInfo.CalculateDamageHash(this);

			if (FDamageInfoContainer::GetContainer().RegisterDamageInfo(DamageHash, DamageInfo))
			{
				FString PathName = GetFullName(nullptr);
				UE_LOG(LogBladeII, Log, TEXT("%s - %d DamageHash Registered"), *PathName, DamageHash);
			}
		}
	}
}