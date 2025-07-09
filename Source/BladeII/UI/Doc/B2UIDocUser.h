// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "B2UIDocUser.generated.h"

/* This purpose is to hold miscellaneous UI-related data. */
UCLASS()
class BLADEII_API UB2UIDocUser : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocUser(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocUser();

	virtual void Init() override;
	virtual void Destroy() override;

	void UpdateUserData();
	void UpdateStaminaData();

	DECLARE_UIDOC_FIELD(int32, UserLevel)
	DECLARE_UIDOC_FIELD(int32, UserExpCur)
	DECLARE_UIDOC_FIELD(int32, UserExpMax)
	DECLARE_UIDOC_FIELD(int32, UserVipLevel)
	DECLARE_UIDOC_FIELD(FText, UserNickName)
	DECLARE_UIDOC_FIELD(UMaterialInterface*, UserPortrait)

	DECLARE_UIDOC_FIELD(FText, UserGuildName)
	DECLARE_UIDOC_FIELD(UMaterialInterface*, UserGuildPortrait)

	DECLARE_UIDOC_FIELD(int32, CombatStartGold) // Valid only in-game, being set at combat (stage) start phase, to be compared to current gold to see how much gained at the end.
	DECLARE_UIDOC_FIELD(int32, Gold) // Current gold amount
	DECLARE_UIDOC_FIELD(int32, Gem) // Current gem amount
	DECLARE_UIDOC_FIELD(int32, StaminaCur) // Current stamina amount
	DECLARE_UIDOC_FIELD(int32, StaminaMax) // Max stamina amount
	DECLARE_UIDOC_FIELD(int32, NextTimeStaminaGet)
	DECLARE_UIDOC_FIELD(int32, RankPromotionPoint)
	DECLARE_UIDOC_FIELD(int32, AdvancedRankPromotionPoint)
	DECLARE_UIDOC_FIELD(int32, SocialPoint)
	DECLARE_UIDOC_FIELD(int32, SocialPointMax)
	DECLARE_UIDOC_FIELD(int32, DarkCrystal)
	DECLARE_UIDOC_FIELD(int32, HeroPiece)
	DECLARE_UIDOC_FIELD(int32, EtherPiece)
	DECLARE_UIDOC_FIELD(int32, FamePoint)
	DECLARE_UIDOC_FIELD(int32, StageAutoClearTicket)
	DECLARE_UIDOC_FIELD(int32, DimensionPiece)

    DECLARE_UIDOC_FIELD(bool, ShowVersion)

private:
	uint32 ChangeUserAccountInfoTicket;
	uint32 ChangeStaminaChargeTimeTicket;
};
