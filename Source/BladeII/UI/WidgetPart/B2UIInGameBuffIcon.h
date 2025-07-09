// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIDocBindable.h"
#include "CommonStruct.h"
#include "B2UIDocBattle.h"
#include "BladeIIDamageType.h"
#include "B2UIInGameBuffIcon.generated.h"

/** This enum is for creating buff icon using appropriate class. */
/*UENUM()
enum class EInGameBuffIconType
{
	IGBIT_StageBuff,
	IGBIT_ResurrectBuff,
	IGBIT_SkillBuff,
};*/

/*
 * Part for in-game buff display, base for all types of buff. 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIInGameBuffIcon : public UB2UIWidgetBase, public IB2UIDocBindable
{
	GENERATED_BODY()

protected:

	// Put the whole panel size here.
	UPROPERTY(EditAnywhere, Category = "BladeII")
	FVector2D NonScaledSize; 

	TWeakObjectPtr<UImage> IMG_MainIcon;


public:

	UB2UIInGameBuffIcon(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

protected:
	virtual void UpdateWidgets();
	
public:

	FORCEINLINE FVector2D GetNonScaledSize() const { return NonScaledSize; }
};

/*================================================================================
UB2UIInGameBuffIcon_Stage
================================================================================*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIInGameBuffIcon_Stage : public UB2UIInGameBuffIcon
{
	GENERATED_BODY()


protected:

	EStageBuffType MyBuffType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	class UMaterialInterface* MainIconMaterial_IncExp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	class UMaterialInterface* MainIconMaterial_IncGold;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	class UMaterialInterface* MainIconMaterial_AutoSkill;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	class UMaterialInterface* MainIconMaterial_BoostGameSpeed;

public:

	UB2UIInGameBuffIcon_Stage(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	void SetBuffType(EStageBuffType InBuffType);

private:
	virtual void UpdateWidgets() override;

public:
	FORCEINLINE EStageBuffType GetBuffType() const { return MyBuffType; }
};


/*================================================================================
UB2UIInGameBuffIcon_Resurrect
================================================================================*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIInGameBuffIcon_Resurrect : public UB2UIInGameBuffIcon
{
	GENERATED_BODY()

protected:
	EResurrectBuffType MyBuffType;	
	int32	TeamSlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	class UMaterialInterface* MainIconMaterial_Attack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	class UMaterialInterface* MainIconMaterial_Defense;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	class UMaterialInterface* MainIconMaterial_Health;

public:

	UB2UIInGameBuffIcon_Resurrect(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateWidgets() override;

public:
	void BindDocByTeamIndex();
	void UnbindDocByTeamIndex();

	FORCEINLINE EResurrectBuffType GetBuffType() const { return MyBuffType; }
	void SetBuffType(EResurrectBuffType InBuffType);
	void SetResurrectAdditionalInfo(int32 InSlotIndex);

private:
	UFUNCTION()
	void OnChangedResurrectBuffValue_Attack(class UB2UIDocBase* Sender, int32 CurrentValue, int32 PrevValue);
	UFUNCTION()
	void OnChangedResurrectBuffValue_Defense(class UB2UIDocBase* Sender, int32 CurrentValue, int32 PrevValue);
	UFUNCTION()
	void OnChangedResurrectBuffValue_Health(class UB2UIDocBase* Sender, int32 CurrentValue, int32 PrevValue);
	
	void GetBuffData(int32& OutAttackBuffValue, int32& OutDefenseBuffValue, int32& OutHealthBuffValue);
};


/*================================================================================
UB2UIInGameBuffIcon_Skill
================================================================================*/

/** United of EUniqueBuffType, ETeamBuffType, and EStateAttackType for UI display */
UENUM(BlueprintType)
enum class EUIUnitedSkillBuffType : uint8
{
	// For EUniqueBuffType
	EUUSBT_Basic_Hide,
	EUUSBT_Basic_DecreaseDamage,
	EUUSBT_Basic_ReflectDamage,
	EUUSBT_Basic_HealHPByAttack,

	// For ETeamBuffType
	EUUSBT_Team_Shield,
	EUUSBT_Team_IncreaseCriticalRate,
	EUUSBT_Team_AbnormalStateImmune,
	EUUSBT_Team_IncreaseCriticalDamage,

	// For EStateAttackType
	EUUSBT_StateDamage_Freezing,
	EUUSBT_StateDamage_Stun,
	EUUSBT_StateDamage_Burning,
	EUUSBT_StateDamage_Poison,

	EUUSBT_Misc_Unbeatable,

	EUUSBT_Character_Restraint,

	EUUSBT_End
};

EUIUnitedSkillBuffType UnitedSkillBT_from_CharacterBT(EBuffType InType);

USTRUCT()
struct FUISkillBuffDisplayInfo_United : public FUISkillBuffDisplayInfo_Base // Final data set for UI widget.. See what do we have at B2UIDocBattle
{
	GENERATED_USTRUCT_BODY()

	FUISkillBuffDisplayInfo_United()
		: FUISkillBuffDisplayInfo_Base()
	{
		MyType = EUIUnitedSkillBuffType::EUUSBT_End;
	}
	
	EUIUnitedSkillBuffType MyType;	
};

USTRUCT()
struct FSkillBuffTypeIconMtrlMapping
{
	GENERATED_USTRUCT_BODY()

	FSkillBuffTypeIconMtrlMapping()
	{
		BuffType = EUIUnitedSkillBuffType::EUUSBT_End;
		IconMtrl = NULL;
	}
	UPROPERTY(EditAnywhere)
	EUIUnitedSkillBuffType BuffType;
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* IconMtrl;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIInGameBuffIcon_Skill : public UB2UIInGameBuffIcon
{
	GENERATED_BODY()
	
	EUIUnitedSkillBuffType BuffType;


protected:

	/** Set icon material for each buff type through this one. */
	UPROPERTY(EditAnywhere)
	TArray<FSkillBuffTypeIconMtrlMapping> IconMtrlMapping;

	TWeakObjectPtr<UTextBlock> TB_RemainingTime;
	
public:

	UB2UIInGameBuffIcon_Skill(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

	void SetBuffType(EUIUnitedSkillBuffType InType); // Like direct set.
	// They all set EUIUnitedSkillBuffType
	void SetAsBasicBuffType(EUniqueBuffType InType);
	void SetAsTeamBuffType(ETeamBuffType InType);
	void SetAsStateDamageBuffType(EStateAttackType InType);

	FORCEINLINE EUIUnitedSkillBuffType GetBuffType() { return BuffType; }

	void SetRemainigTime(float InTime);

private:
	virtual void UpdateWidgets() override;
};


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIInGameBuffIcon_GuildBuff : public UB2UIInGameBuffIcon
{
	GENERATED_BODY()


protected:

	EItemOption GuildBuffType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		class UMaterialInterface* MainIconMaterial_Attack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		class UMaterialInterface* MainIconMaterial_Defense;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		class UMaterialInterface* MainIconMaterial_HP;

public:

	UB2UIInGameBuffIcon_GuildBuff(const FObjectInitializer& ObjectInitializer);

	void SetBuffType(EItemOption InBuffType);

private:
	virtual void UpdateWidgets() override;

public:
	FORCEINLINE EItemOption GetBuffType() const { return GuildBuffType; }
};

/*================================================================================
UB2UIInGameBuffIcon_Resurrect
================================================================================*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIInGameBuffIcon_DimensionTower : public UB2UIInGameBuffIcon
{
	GENERATED_BODY()

protected:
	EResurrectBuffType MyBuffType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		class UMaterialInterface* MainIconMaterial_Attack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		class UMaterialInterface* MainIconMaterial_Defense;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		class UMaterialInterface* MainIconMaterial_Health;

public:

	UB2UIInGameBuffIcon_DimensionTower(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

private:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateWidgets() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;

public:
	FORCEINLINE EResurrectBuffType GetBuffType() const { return MyBuffType; }
	void SetBuffType(EResurrectBuffType InBuffType);

private:
	UFUNCTION()
		void OnChangedDimensionBuffValue_Attack(class UB2UIDocBase* Sender, int32 CurrentValue, int32 PrevValue);
	UFUNCTION()
		void OnChangedDimensionBuffValue_Defense(class UB2UIDocBase* Sender, int32 CurrentValue, int32 PrevValue);
	UFUNCTION()
		void OnChangedDimensionBuffValue_Health(class UB2UIDocBase* Sender, int32 CurrentValue, int32 PrevValue);
};