#pragma once
//#include "BladeII.h"
#include "BladeIIDamageType.h"
#include "B2CharacterBuffManager.generated.h"

UCLASS()
class BLADEII_API UB2CharacterBuffManager : public UObject
{
	GENERATED_BODY()
public:
	UB2CharacterBuffManager();
	~UB2CharacterBuffManager();

	void InitBuffManager(class ABladeIICharacter* OwnerCharacter);
	
	class UB2Buff_Base* AddBuff(EBuffType BuffType, float fDuration, float Amount, AActor* InBuffCauser, bool bUseEffect = true, bool bUseTextEffect = true, TArray<float>* PtrAdditionalParams = nullptr);
	class UB2Buff_Base* FindBuff(EBuffType BuffType) const;
	void ClearBuff(EBuffType BuffType);

	bool IsBuffActive(EBuffType BuffType);
	void BuffTick();
	void ClearAllBuffs();
	void ClearDebuffs();

	void CheckCustomTimeDilation();

	void OwnerTakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);
	void AffectToAttacker(ABladeIICharacter* Victim, float ActualDamage, const FDamageInfo& DamageInfo);

	void AddBuffMaterialMark(EBuffType BuffType);
	void RemoveBuffMaterialMark(EBuffType BuffType);

	class UB2Buff_Base* FindOrCreateBuff(EBuffType BuffType);
	template< class T >
	T* CreateBuff(EBuffType BuffType)
	{
		UClass* BuffClass = T::StaticClass();
		AActor* OwnerActor = Cast<AActor>(GetOwnerCharacter());
		//if (BuffClass && OwnerActor)
		//{
		//	static UEnum* ePtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EBuffType"), true);
		//	FString BuffName = FString::Printf(TEXT("%s_%s"), *(ePtr->GetNameStringByIndex(int32(BuffType))), *OwnerActor->GetName());
		//	T* BuffInstance = NewObject<T>(OwnerActor, *BuffName, RF_Transient);

		//	check(BuffInstance);

		//	return BuffInstance;
		//}
		if (BuffClass && OwnerActor)
		{
			// Specify the exact path to the EBuffType enum
			FString EnumPath = "/Game/PathToEnums/EBuffType.EBuffType";  // Update this path to the actual path of your enum
			static UEnum* ePtr = FindObject<UEnum>(nullptr, *EnumPath, true);

			if (!ePtr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Enum EBuffType not found!"));
				return nullptr;
			}

			FString BuffName = FString::Printf(TEXT("%s_%s"), *(ePtr->GetNameStringByIndex(static_cast<int32>(BuffType))), *OwnerActor->GetName());
			T* BuffInstance = NewObject<T>(OwnerActor, FName(*BuffName), RF_Transient);

			check(BuffInstance);  // Ensure BuffInstance is not null

			return BuffInstance;
		}
		
		return nullptr;



	}

	template< class T >
	T* GetBuff(EBuffType BuffType) { return Cast<T>(FindBuff(BuffType)); }

	class ABladeIICharacter* GetOwnerCharacter();
	TArray<UB2Buff_Base*>* GetBuffList();

	void OnPlayerBuffChanged();

	void ResetCrowdControlCount() { CrowdControlCount = 0; }

	bool NotAllowDuplicatedBuff(EBuffType InBuffType);

private:
	UPROPERTY(Transient)
	class ABladeIICharacter* m_pOwner;

	UPROPERTY(Transient)
	TArray<UB2Buff_Base*> m_arBuff;

	float SavedWorldTimeSeconds;

	TArray<EBuffType> m_arBuffMaterialMark;

public:
	// 지속시간 감소에 쓰는 CC카운트 디버프를 강제로 해제할경우 필요하면 0으로 설정따로 해줘야함
	int32 CrowdControlCount;
};
