// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "B2UIWidget.h"
#include "B2UIInGameBuffIcon.h"
#include "B2UITeamMatchBattleSlot.generated.h"

DECLARE_DELEGATE_TwoParams(FB2UITeamMatchBattleSlot_OnChangedHealthAndShieldCommon, bool, int32)
DECLARE_DELEGATE_TwoParams(FB2UITeamMatchBattleSlot_OnClickBtnCharacter, bool, int32)

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UITeamMatchBattleSlot : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	UB2UITeamMatchBattleSlot(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetDamagePercent(float DamagePercent);
	void SetData(EPCClass CharClass, int32 Lv, bool bInMyTeam, int32 InSlotIdx, int8 FormationNum = 0);
	void SetHpAndShield(float CurHPValue, float MaxHPValue, float ShieldValue);
	void SetCamera(bool bSet);
	void SetDeath(bool bDeath);
	void SetContributionValue(int32 Value);
	void CreateResurrectBuffIcon(EResurrectBuffType InResurrectBuffType, int32 InTeamIndex);
	void DestroyAllBuffIcons_SoR();

	int32 GetCurrentPCClass() const;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION()
	void OnClickBtnCharacter();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayCameraEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayDeathEvent();

	void OnChangedHealthAndShieldCommon();

	virtual void OnClickImplement();

public:
	UFUNCTION()
	void OnChangedCharName(class UB2UIDocBase* Sender, FText CurrName, FText PrevName);
	UFUNCTION()
	void OnChanagedHealth(class UB2UIDocBase* Sender, float CurrHealth, float PrevHealth);
	UFUNCTION()
	void OnChanagedShield(class UB2UIDocBase* Sender, float CurrShield, float PrevShield);
	UFUNCTION()
	void OnChanagedPCClass(class UB2UIDocBase* Sender, EPCClass CurPCClass, EPCClass PrePCClass);
	UFUNCTION()
	void OnChanagedPCLevel(class UB2UIDocBase* Sender, int32 CurPCLevel, int32 PrePCLevel);
	UFUNCTION()
	void OnChanagedContribution(class UB2UIDocBase* Sender, int32 CurContribution, int32 PreContribution);

private:
	UB2UIInGameBuffIcon_Resurrect* CreateBuffIconCommon(TSubclassOf<UB2UIInGameBuffIcon> InCreateClass);

protected:
	TWeakObjectPtr<UB2Button> BTN_Character;
	TWeakObjectPtr<UImage> IMG_CharacterImage;
	TWeakObjectPtr<UTextBlock> TB_Lv;
	TWeakObjectPtr<UTextBlock> TB_Job;
	TWeakObjectPtr<UTextBlock> TB_DamageContribution;
	TWeakObjectPtr<UProgressBar> PB_HP;
	TWeakObjectPtr<UProgressBar> PB_Shield;
	TWeakObjectPtr<UImage> IMG_Death;
	TWeakObjectPtr<UImage> IMG_Camera;
	TWeakObjectPtr<UImage> IMG_Circle_Camera;
	TWeakObjectPtr<UImage> IMG_Circle_Death;
	TWeakObjectPtr<UTextBlock> TB_Contribution;
	TWeakObjectPtr<UTextBlock> TBX_FomationNum;
	TWeakObjectPtr<UPanelWidget> X_FormationNumber;
	int32 SlotIdx;
	bool bMyTeam;
	bool OldDeathstate;
	FText UserName;

	TArray<UB2UIInGameBuffIcon*> AllBuffIcons_SoR;
	TWeakObjectPtr<UHorizontalBox> HB_StageResurrectBuffIconSet;

	int32 PCClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	TArray<UMaterialInterface*>	CharacterTexture;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	TSubclassOf<UB2UIInGameBuffIcon> BuffIconClass_Resurrect;

	// Put the whole panel size here.
	UPROPERTY(EditAnywhere, Category = "BladeII")
	FVector2D BuffIconSize;

	FB2UITeamMatchBattleSlot_OnChangedHealthAndShieldCommon FOnChangedHealthAndShieldCommon;
	FB2UITeamMatchBattleSlot_OnClickBtnCharacter			FOnClickCharacter;
};
