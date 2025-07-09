// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BladeIIUserWidget.h"
#include "BladeIIPlayer.h"
#include "B2UIManager_Lobby.h"
#include "B2FloatingLobbyOnHeadDisplay.generated.h"

/**
 * LIke B2FloatingWidget, but it is for Lobby scene.
 */
UCLASS()
class BLADEII_API UB2FloatingLobbyOnHeadDisplay : public UBladeIIUserWidget
{
	GENERATED_BODY()

protected:

	ELobbyOnHeadDisplayType MasterType;

	/** Owner character that this widget will floating around. */
	UPROPERTY(Transient, BlueprintReadOnly)
	class ASkeletalMeshActor* OwnerSKActor;
	
	EPCClass OwnerPCClass;

	/** Location offset from owner SkeletalMeshActor in world space, for LobbyMain scene. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FVector LocOffsetFromOwner_LobbyMain;

	/** Location offset from owner SkeletalMeshActor in world space, for CharacterObserve scene.
	 * This is to be smaller than LocOffsetFromOwner_LobbyMain, because this is for zoomed in scene. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FVector LocOffsetFromOwner_CharObserve;

	/** Set this value if want to adjust based on on screen size, or adjust for widget origin. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	FVector2D FinalScreenOffset;
	
	uint32 bWidgetProjectedPosBeingSet : 1;

	TWeakObjectPtr<UTextBlock> TB_LevelAndOrName;
	TWeakObjectPtr<UB2Button> BTN_SetNickName;
	TWeakObjectPtr<UTextBlock> TB_Rank;
	TWeakObjectPtr<UImage> IMG_RankMark;

public:

	UB2FloatingLobbyOnHeadDisplay(const FObjectInitializer& ObjectInitializer);
	
	virtual void CacheAssets() override;

	void SetMasterType(ELobbyOnHeadDisplayType InType);

	void UpdateOnMasterTypeChange();

	/** Set the owner and do some initialization if necessary. */
	virtual void SetOwnerInfo(EPCClass InOwnerClass, class ASkeletalMeshActor* InOwner);

	UFUNCTION()
	void OnClickSetNickNameBtn();

	/** Update its floating screen position to follow owner character. */
	virtual void UpdatePosition();

	void DestroySelf() override;
};
