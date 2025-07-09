// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "BladeIICharacter.h"
#include "B2UIDocPVP1on1Rival.generated.h"

/* This purpose is to hold miscellaneous UI-related data. */
UCLASS()
class BLADEII_API UB2UIDocPVP1on1Rival : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	UB2UIDocPVP1on1Rival(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocPVP1on1Rival();

	virtual void Init() override;
	virtual void Destroy() override;

	void UpdateTagMatchData();
	void ClearInvitationData();

	DECLARE_UIDOC_FIELD(int32, ComboNum)

	DECLARE_UIDOC_FIELD(float, ComboExpireProgress) // 0.0 ~ 1.0

	/** Additional critical rate by combo num, not the final critical rate. 0.0 ~ 1.0 */
	DECLARE_UIDOC_FIELD(float, CriticalRateByComboNum)

	DECLARE_UIDOC_FIELD(bool, InAutoPlay)

	/** [@AKI, 170414] The player's Nick Name, currently using by only PVPTagMatch(1on1), if you added to other mode then change this comment*/
	DECLARE_UIDOC_FIELD(FString, UserNickName)
	DECLARE_UIDOC_FIELD(int64, UserAccountID)

	//////////////////////////////////////////////////
	// Friendly Battle
	DECLARE_UIDOC_FIELD(bool, IsFriendshipGame)
	DECLARE_UIDOC_FIELD(bool, IsInviteeReady)
	DECLARE_UIDOC_FIELD(bool, IsInviterReady)
	DECLARE_UIDOC_FIELD(bool, IsFrienshipGameValidSet)
	DECLARE_UIDOC_FIELD(bool, IsInvitee)
	DECLARE_UIDOC_FIELD(bool, InviterForceCancel)

	//////////////////////////////////////////////////
	// Any data that will be necessary both for currently playing character and inactive (tag) character.

	/** If true, current PC class is the main character class selected from lobby, or current PC class is the sub class. */
	DECLARE_UIDOC_FIELD(bool, IsCurPCMain)

	/** Currently active PC class, the main character class if IsCurPCMain is true, or sub. */
	DECLARE_UIDOC_FIELD(int32, CurPCClass)
	DECLARE_UIDOC_FIELD(int32, CurPCLevel)
	DECLARE_UIDOC_FIELD(float, CurrPCHealth)
	DECLARE_UIDOC_FIELD(float, CurrPCMaxHealth)
	DECLARE_UIDOC_FIELD(float, CurrPCShield) // Set by specific skill having ETeamBuffType::ETBT_Shield
	DECLARE_UIDOC_FIELD(bool, CurrPCInCriticalCondition) // This and IsTagPossible can be "Is tag suggested by player condition"


	/** Currently inactive (waiting for tag) PC class, the sub character class if IsCurPCMain is true, or main. */
	DECLARE_UIDOC_FIELD(int32, TagPCClass)
	DECLARE_UIDOC_FIELD(float, TagPCHealth)
	DECLARE_UIDOC_FIELD(float, TagPCMaxHealth)

	//////////////////////////////////////////////////


	//////////////////////////////////////////////////
	// Tag runtime state. In SkillRTManager, tag is one of skill set, but here we take it separate from DocSkill

	DECLARE_UIDOC_FIELD(bool, IsTagPossible)
	DECLARE_UIDOC_FIELD(bool, IsTagDisabledByCooltime)
	DECLARE_UIDOC_FIELD(bool, IsCountingTagCooltime)
	DECLARE_UIDOC_FIELD(float, FinalTagCooltime)
	DECLARE_UIDOC_FIELD(float, RemainingTagCooltime)

	DECLARE_UIDOC_FIELD(bool, IsTagAttackSuggested) // Just for tag attack timing.
	DECLARE_UIDOC_FIELD(bool, IsTagSuggested) // Both for TagAttach and normal tag suggested timing.

	//////////////////////////////////////////////////

	DECLARE_UIDOC_FIELD(bool, bCurrentlyBossAppeared) // True if we currently have a mob spawned from WaveSetting having bBossWave set.


	//////////////////////////////////////////////////
	DECLARE_UIDOC_FIELD(int32, TagMatchGrade)
	DECLARE_UIDOC_FIELD(int32, TagMatchGradeStar)
	DECLARE_UIDOC_FIELD(int32, TagMatchCurrContinuousWins)	
	DECLARE_UIDOC_FIELD(int64, TagRewardTime)
	DECLARE_UIDOC_FIELD(bool, TagTodayReward)
	DECLARE_UIDOC_FIELD(bool, TagLastReward)
	DECLARE_UIDOC_FIELD(int32, TagParticipationCount)
	
	//Tag Ranking Reward

	DECLARE_UIDOC_FIELD(bool, IsTagReward)
	DECLARE_UIDOC_FIELD(b2network::B2RankingRewardPtr,TagReward)

	//RewardBox
	DECLARE_UIDOC_FIELD(b2network::B2DuelRewardBoxStatusPtr, RewardBoxStatus)

	//////////////////////////////////////////////////
	//MatchPoint data

	DECLARE_UIDOC_FIELD(int32, MatchPoint);
	DECLARE_UIDOC_FIELD(int32, NextTimeMatchPointGet);
	DECLARE_UIDOC_FIELD(int32, AdditionalMatchCount);
	DECLARE_UIDOC_FIELD(double, NextChargeSecond);
	DECLARE_UIDOC_FIELD(bool, UsedAdditionalPoint);

	DECLARE_UIDOC_FIELD(bool, CanViewModRewardBoxConfirm)
	

public:
	FModRankerInfoContainer RankerInfoContainer;
private:
	uint32 ChangePvPMatchPointChargeTimeTicket;
	
};
