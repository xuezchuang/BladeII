// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "BladeII.h"
//#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "BladeIICharacter.h"
#include "B2UIDocBattle.generated.h"

USTRUCT()
struct FUISkillBuffDisplayInfo_Base
{
	GENERATED_USTRUCT_BODY()

	FUISkillBuffDisplayInfo_Base()
	{
		LastStartedTime = 0.0f;
		RemainingTime = 0.0f;
		Duration = 0.0f;
	}
	float LastStartedTime;
	float RemainingTime;
	float Duration;
};

//////////////////////////////////////////////////
// Structs to send currently applied in-game skill buffs to UI
USTRUCT()
struct FUISkillBuffDisplayInfo_Basic : public FUISkillBuffDisplayInfo_Base
{
	GENERATED_USTRUCT_BODY()

	FUISkillBuffDisplayInfo_Basic()
		: FUISkillBuffDisplayInfo_Base()
	{
		MyType = EUniqueBuffType::EBT_Normal;
	}
	EUniqueBuffType MyType;
};

USTRUCT()
struct FUISkillBuffDisplayInfo_Team : public FUISkillBuffDisplayInfo_Base
{
	GENERATED_USTRUCT_BODY()

	FUISkillBuffDisplayInfo_Team()
		: FUISkillBuffDisplayInfo_Base()
	{
		MyType = ETeamBuffType::ETBT_Normal;
	}
	ETeamBuffType MyType;
};

USTRUCT()
struct FUISkillBuffDisplayInfo_StateDamage : public FUISkillBuffDisplayInfo_Base
{
	GENERATED_USTRUCT_BODY()

	FUISkillBuffDisplayInfo_StateDamage()
		: FUISkillBuffDisplayInfo_Base()
	{
		MyType = EStateAttackType::EDT_Normal;
	}
	EStateAttackType MyType;
};

USTRUCT()
struct FUISkillBuffDisplayInfo_Invincible : public FUISkillBuffDisplayInfo_Base
{
	GENERATED_USTRUCT_BODY()

	FUISkillBuffDisplayInfo_Invincible()
		: FUISkillBuffDisplayInfo_Base()
	{
		bIsOn = false;
	}
	bool bIsOn;
};

USTRUCT()
struct FUISkillBuffDisplayInfo_Character : public FUISkillBuffDisplayInfo_Base
{
	GENERATED_USTRUCT_BODY()

		FUISkillBuffDisplayInfo_Character()
		: FUISkillBuffDisplayInfo_Base()
	{
		MyType = EBuffType::Buff_None;
	}
	EBuffType MyType;
};
//////////////////////////////////////////////////

// It is not intended to define all QTE state, just sending some QTE state to UI.
UENUM(BlueprintType)
enum class EQTEUIAttackEnableTimingState : uint8
{
	Mount,
	FirstAttack,
	SecondAttack,

	None // Not QTE timing
};

/* This purpose is to hold miscellaneous UI-related data */
UCLASS()
class BLADEII_API UB2UIDocBattle : public UB2UIDocBase
{
	GENERATED_BODY()

public:
	static const uint8 MySlotIndex = 0;

public:
	UB2UIDocBattle(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocBattle();

	void ClearBattleDataForStageClear();

	DECLARE_UIDOC_FIELD(int32, ComboNum)

	DECLARE_UIDOC_FIELD(float, ComboExpireProgress) // 0.0 ~ 1.0

	/** Additional critical rate by combo num, not the final critical rate. 0.0 ~ 1.0 */
	DECLARE_UIDOC_FIELD(float, CriticalRateByComboNum)

	DECLARE_UIDOC_FIELD(float, IncCounterDamageCombo)

	DECLARE_UIDOC_FIELD(int32, InAutoPlay)
	DECLARE_UIDOC_FIELD(bool, IsCenematicMode)
	
	DECLARE_UIDOC_FIELD(EQTEUIAttackEnableTimingState, QTEEnableState)

	DECLARE_UIDOC_FIELD(bool, bIsPlayerInQTEState)
	DECLARE_UIDOC_FIELD(bool, bIsQTEMounted)
	DECLARE_UIDOC_FIELD(bool, bIsQTEMountedAndReadyForAttack)
	DECLARE_UIDOC_FIELD(float, MaxQTEMountTime)
	DECLARE_UIDOC_FIELD(float, RemainingQTEMountTime)

	/** [@AKI, 170414] The player's Nick Name, currently using by only PVPTagMatch(1on1), if you added to other mode then change this comment*/
	DECLARE_UIDOC_FIELD(FString, UserNickName)

	//////////////////////////////////////////////////
	// Any data that will be necessary both for currently playing character and inactive (tag) character.

	/** If true, current PC class is the main character class selected from lobby, or current PC class is the sub class. */
	DECLARE_UIDOC_FIELD(bool, IsCurPCMain)

	/** Currently active PC class, the main character class if IsCurPCMain is true, or sub. */
	DECLARE_UIDOC_FIELD(int32, CurPCClass)
	DECLARE_UIDOC_FIELD(float, CurrPCHealth)
	DECLARE_UIDOC_FIELD(float, CurrPCMaxHealth)
	DECLARE_UIDOC_FIELD(float, CurrPCShield) // Set by specific skill having ETeamBuffType::ETBT_Shield
	DECLARE_UIDOC_FIELD(bool, CurrPCInCriticalCondition) // This and IsTagPossible can be "Is tag suggested by player condition"


	/** Currently inactive (waiting for tag) PC class, the sub character class if IsCurPCMain is true, or main. */
	DECLARE_UIDOC_FIELD(int32, TagPCClass)
	DECLARE_UIDOC_FIELD(float, TagPCHealth)
	DECLARE_UIDOC_FIELD(float, TagPCMaxHealth)

	//////////////////////////////////////////////////
	DECLARE_UIDOC_FIELD(int32, SavePCClass)

	// CounterAttack
	DECLARE_UIDOC_FIELD(int32, CounterAttackPoint)
	DECLARE_UIDOC_FIELD(int32, MaxCounterAttackPoint)

	//Tag Ranking Reward

	/*DECLARE_UIDOC_FIELD(bool, IsTagReward)
	DECLARE_UIDOC_FIELD(b2network::B2RankingRewardPtr,TagReward)
	DECLARE_UIDOC_FIELD(TArray<b2network::B2HallOfFamePtr>, TagHallofFame)*/
	

	//////////////////////////////////////////////////
	// Tag runtime state. In SkillRTManager, tag is one of skill set, but here we take it separate from DocSkill

	DECLARE_UIDOC_FIELD(bool, IsTagPossible)
	DECLARE_UIDOC_FIELD(bool, IsTagDisabledByCooltime)
	DECLARE_UIDOC_FIELD(bool, IsCountingTagCooltime)
	DECLARE_UIDOC_FIELD(float, FinalTagCooltime)
	DECLARE_UIDOC_FIELD(float, RemainingTagCooltime)

	DECLARE_UIDOC_FIELD(int32, OffenseSetID)
	DECLARE_UIDOC_FIELD(int32, DefenseSetID)
	DECLARE_UIDOC_FIELD(bool, IsOffenseEther)
	DECLARE_UIDOC_FIELD(bool, IsDefenseEther)
	DECLARE_UIDOC_FIELD(bool, EnableOffenseEther)
	DECLARE_UIDOC_FIELD(bool, EnableDefenseEther)
	DECLARE_UIDOC_FIELD(float, OffenseEtherCoolTime);
	DECLARE_UIDOC_FIELD(float, DefenseEtherCoolTime)
	DECLARE_UIDOC_FIELD(float, OffenseEtherMaxCoolTime);
	DECLARE_UIDOC_FIELD(float, DefenseEtherMaxCoolTime);

	DECLARE_UIDOC_FIELD(float, GuardCoolTime);
	DECLARE_UIDOC_FIELD(bool, HasNotTagDefence);

	DECLARE_UIDOC_FIELD(bool, IsTagAttackSuggested) // Just for tag attack timing.
	DECLARE_UIDOC_FIELD(bool, IsTagSuggested) // Both for TagAttack and normal tag suggested timing.
	DECLARE_UIDOC_FIELD(bool, IsAIMe);
	DECLARE_UIDOC_FIELD(bool, bTagAttackPending) // Tag signal after this has been set is tag attack. Then it must be reset soon.

	DECLARE_UIDOC_FIELD(bool, HasAbnormalBuff)

	//////////////////////////////////////////////////

	DECLARE_UIDOC_FIELD(bool, bCurrentlyBossAppeared) // True if we currently have a mob spawned from WaveSetting having bBossWave set.
	DECLARE_UIDOC_FIELD(float, BossHPCurrent) // Valid only when bCurrentlyBossAppeared is true
	DECLARE_UIDOC_FIELD(float, BossHPMax) // Valid only when bCurrentlyBossAppeared is true.
	DECLARE_UIDOC_FIELD(bool, bBossQTEBreakable) // Valid only when bCurrentlyBossAppeared is true.
	DECLARE_UIDOC_FIELD(float, BossQTEBreakPointCurrent) // Valid only when bCurrentlyBossAppeared is true
	DECLARE_UIDOC_FIELD(float, BossQTEBreakPointMax) // Valid only when bCurrentlyBossAppeared is true.
	DECLARE_UIDOC_FIELD(ENPCClass, BossMobClass) // Valid only when bCurrentlyBossAppeared is true. One of parameter to get relevant NPCClassInfo.
	DECLARE_UIDOC_FIELD(ENPCClassVariation, BossMobClassVariation) // Valid only when bCurrentlyBossAppeared is true. One of parameter to get relevant NPCClassInfo.

	//////////////////////////////////////////////////
	// Resurrection buff.. We need both selected count per buff type and current effective values.

	//DECLARE_UIDOC_FIELD(EResurrectGameModeType, ResurrectGameModeType)

	DECLARE_UIDOC_FIELD(int32, MaxResurrectionChance)
	DECLARE_UIDOC_FIELD(int32, RemainingResurrectionChance)
	DECLARE_UIDOC_FIELD(int32, ResurrectionCost)

	DECLARE_UIDOC_FIELD(int32, SelectedResurrectBuffAttack)
	DECLARE_UIDOC_FIELD(int32, SelectedResurrectBuffDefense)
	DECLARE_UIDOC_FIELD(int32, SelectedResurrectBuffHealth)

	// UI钎矫甫 困秦 利侩吝牢 滚橇绰 评喊肺 啊瘤绊 乐澜. / Index绰 浇吩 鉴辑/ 0(UB2UIDocBattle::MyIndex)锅篮 磊扁磊脚
	DECLARE_UIDOC_FIELD_FourField(int32, CurResurrectBuffAttackCount, 0)
	DECLARE_UIDOC_FIELD_FourField(int32, CurResurrectBuffDefenseCount, 0)
	DECLARE_UIDOC_FIELD_FourField(int32, CurResurrectBuffHealthCount, 0)

	DECLARE_UIDOC_FIELD(bool, NeedResurrectBuffText)

	//////////////////////////////////////////////////
	// Skill buff state

	DECLARE_UIDOC_FIELD(TArray<FUISkillBuffDisplayInfo_Character>, CharacterBuffState)

	//////////////////////////////////////////////////
	// Dimension Buff State
	DECLARE_UIDOC_FIELD(int32, CurDimensionBuffAttackCount)
	DECLARE_UIDOC_FIELD(int32, CurDimensionBuffDefenseCount)
	DECLARE_UIDOC_FIELD(int32, CurDimensionBuffHealthCount)

	//////////////////////////////////////////////////
	// Temporary TagMatch Data located
	DECLARE_UIDOC_FIELD(int32, TagMatchRank)
	DECLARE_UIDOC_FIELD(float, TagMatchRankPercentage)
	DECLARE_UIDOC_FIELD(int32, TagMatchBeforeGrade)
	DECLARE_UIDOC_FIELD(int32, TagMatchGrade)
	DECLARE_UIDOC_FIELD(int32, TagMatchGradeStar)
	DECLARE_UIDOC_FIELD(int32, TagMatchBeforeGradeStar)
	DECLARE_UIDOC_FIELD(int32, TagMatchCurrTopContinuousWins);
	DECLARE_UIDOC_FIELD(int32, TagMatchPrevTopContinuousWins);
	DECLARE_UIDOC_FIELD(int32, TagMatchCurrContinuousWins)
	DECLARE_UIDOC_FIELD(int32, TagMatchPrevContinuousWins)
};
