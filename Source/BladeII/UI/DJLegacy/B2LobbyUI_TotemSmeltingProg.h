// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DJLegacy/B2LobbyUI_ItemCUSProgBase.h"
#include "B2LobbyUI_TotemSmeltingProg.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2LobbyUI_TotemSmeltingProg : public UB2LobbyUI_ItemCusProgBase
{
	GENERATED_BODY()

protected:
	/** Smelting Complete Particle system */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	class UParticleSystem* SmeltingCompletePS;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	FVector SmeltingCompleteFxScale;
	UPROPERTY(Transient)
	class UParticleSystemComponent* CreatedSmeltingCompleteFx;

public:
	UB2LobbyUI_TotemSmeltingProg(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	void DestroySelf() override;

private:
	void UpdateStaticWidgets();

private:
	const static FName Name_FxMID_SmeltingCompleteBGPanel;
	const static FName Name_FxMID_SmeltingCompleteItemIcon;

protected:
	void SetTotemData_Result(FB2Totem& InTotem);
	class UB2DynItemIcon_ItemOpScene* CreateTotemIcon(UCanvasPanel* ParentCP, const FB2Totem& InItemData, ELobbyInvenItemIconUsage IconUsage);

protected:
	//UFUNCTION(BlueprintCallable, Category = "BladeII")
	virtual void StartShowResultItemIcon() override;

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void StartSmeltingCompleteFx();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void FinishSmeltingCompleteFx();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void StartBGSoundfx();

private:
	void DestroySmeltingCompleteFx();
	virtual void DestroyAllFx() override;

	virtual bool IsSupportedOpMode(ELobbyInvenItemOpMode InMode) const { return (InMode == ELobbyInvenItemOpMode::EIVIOP_TotemSmelting); }

private:
	TWeakObjectPtr<UTextBlock>				TB_SmeltingComplete;
	TWeakObjectPtr<UHorizontalBox>			HB_SmeltingComplete;
};
