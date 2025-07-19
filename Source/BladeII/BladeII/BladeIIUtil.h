#pragma once

#include "EngineMinimal.h"

class ALevelSequenceActor;
//#if WITH_EDITOR
//#include "BladeIIEditorUtil.h"
//#endif
#include "BladeIILocText.h"
//#include "B2LogManager.h"
#include"Components/Image.h"
#include "BladeII.h"
#include "Engine/StreamableManager.h"
#include "../Common/CommonStruct.h"

FText GetPCClassLocalizedText(EPCClass PCClass);

extern bool OpenBladeIIDLCFrontCommon(class AGameMode* InCurrentGameMode);
bool OpenBladeIILobbyCommon(class AGameMode* InCurrentGameMode);

/** Start the main map of specified StageNum.
* OpenURLOption is the option string goes to Open command. In normal circumstances, most options might be set by StageInfo. */
bool StartBladeIIGameStage(class AGameMode* InCurrentGameMode, int32 InClientStageId, FString OpenURLOption = FString(TEXT("")), EB2GameMode ChangeGameMode = EB2GameMode::Scenario);

/** Client side entry point of PVP game mode. */
bool StartBladeIIPVPGame(class AGameMode* InCurrentGameMode, FString OpenURLOption = FString(TEXT("")));
bool StartBladeIITeamMatchGame(class AGameMode* InCurrentGameMode, FString OpenURLOption = FString(TEXT("")));
bool StartBladeIICounterDungeon(class AGameMode* InCurrentGameMode, int32 CounterStageNum);
bool StartBladeIIHerotowerGame(class AGameMode* InCurrentGameMode, int32 HeroTowerNum);

bool StartBladeIIRaidGame(class AGameMode* InCurrentGameMode, int32 RaidClientStageID, FString OpenURLOption = FString(TEXT("")));
bool StartBladeIIControlGame(class AGameMode* InCurrentGameMode, FString OpenURLOption = FString(TEXT("")));
bool StartBladeIIGuildMatchGame(class AGameMode* InCurrentGameMode, FString OpenURLOption = FString(TEXT("")));

bool StartBladeIIGameTutorial(class AGameMode* InCurrentGameMode, FString OpenURLOption = FString(TEXT("")));

//bool StartBladeIIPVPReplay(class AGameMode* InCurrentGameMode, FModReplayInfo ReplayInfo, FString OpenURLOption = FString(TEXT("")));

bool StartBladeIIDimension(class AGameMode* InCurrentGameMode, int32 MapLoadID, FString OpenURLOption = FString(TEXT("")));

/** Find (or create if not) the stage Info table asset from current BladeIIGameMode object. */
class UB2StageInfo* StaticFindStageInfo();
/** Find (or create if not) the chapter Info table asset from current BladeIIGameMode object. */
class UB2ChapterInfo* StaticFindChapterInfo();
/** Find (or create if not) the item Info table asset from current BladeIIGameMode object. */
class UB2ItemInfo* StaticFindItemInfo();
/** Find (or create if not) the dialog Info table asset from current BladeIIGameMode object. */
class UB2ScenarioDialogInfo* StaticFindDialogInfoTable();
/** Find (or create if not) the dialog sound Info table asset from current BladeIIGameMode object. */
class UB2DialogSoundInfoSet* StaticFindDialogSoundInfoTable();
/** Find (or create if not) the Mob's sensitive Info table asset (which was a part of but now separated from NPCClassInfo) from current BladeIIGameMode object. */
class UB2NPCSensitiveInfo* StaticFindNPCSensitiveInfoTable();
/** Find (or create if not) the Brevet Info table asset from current BladeIIGameMode object. */
class UB2BrevetInfo* StaticFindBrevetInfo();
class UB2ErrorCodeInfo* StaticFindErrorCodeInfo();
/** Find (or create if not) the Quest Info table asset from current BladeIIGameMode object. */
class UB2QuestInfoTable* StaticFindQuestTable();
class UB2QuestNPCSoundInfo* StaticFindQuestNPCSoundTable();
class UB2MissionInfo* StaticFindMissionInfo();
class UB2RewardInfo* StaticFindRewardInfo();
class UB2GoodsTypeInfo* StaticFindGoodsTypeInfoTable();
class UB2GoodsShortcutInfo* StaticFindGoodsShortcutInfo();
class UB2RaidInfoTable* StaticFindRaidInfo();
class UB2TalkInfo* StaticFindTalkInfo();
class UB2DialogSoundInfoSet* StaticFindTalkSoundInfoTable();
class UB2ChattingFilteringInfo* StaticFindChattingFilteringInfo();
class UB2EtherInfo* StaticFindEtherInfoTable();
class UB2SelectiveEtherLottery* StaticFindSelectiveEtherLotteryTable();
class UB2EtherSetOptionInfo* StaticFindEtherSetOptionInfoTable();
class UB2EtherSetInfoTable*	StaticFindEtherSetEffectInfo();
class UB2TotemRefineInfo*	StaticFindTotemRefineInfo();
class UB2ItemSortInfo* StaticFindItemSortInfo();
class UB2CharacterTagDialogueInfo* StaticFindCharacterTagDialogueInfo();
class UB2SeasonEventInfo* StaticFindSeasonEventInfo();
class UB2FairyInfo* StaticFindFairyInfo();
//
////////////////////////////////////////////////////////////////////////////////////////////
//// For StaticFind stuff below, Pass down WorldContextObject whenever possible. for using cached reference.
//////////////////////////////////////////////////////////////////////////////////////////
/** Find the PC class Info box asset containing all PC's class data from config
 * It could be slow without valid WorldContextObject. Pass WorldContextObject whenever possible. */
class UB2PCClassInfoBox* StaticFindPCClassInfoBox(UObject* WorldContextObject = nullptr);
/** Find the NPC class Info box asset containing all Mobs' class data from config
* It could be slow without valid WorldContextObject. Pass WorldContextObject whenever possible. */
class UB2NPCClassInfoBox* StaticFindMobClassInfoBox(UObject* WorldContextObject = nullptr);
// Same old now..
class UB2CommonSoundInfo* StaticFindCommonSoundInfo(UObject* WorldContextObject = nullptr);
class UB2DialogCharacterInfoSet* StaticFindDialogCharacterInfoTable(UObject* WorldContextObject = nullptr);
class UB2WingInfo* StaticFindWingInfo(UObject* WorldContextObject = nullptr);
class UB2SomeInfo* StaticFindSomeInfo(UObject* WorldContextObject = nullptr);
class UB2ItemMiscInfo* StaticFindItemMiscInfo(UObject* WorldContextObject = nullptr);
class UB2GuildMarkInfo* StaticFindGuildMarkInfo(UObject* WorldContextObject = nullptr);
class UB2LoadingImageInfo* StaticFindLoadingImageInfo(UObject* WorldContextObject = nullptr);
class UB2GuildSkillInfo* StaticFindGuildSkillInfo(UObject* WorldContextObject = nullptr);
class UB2NoAnimSkillInfo* StaticFindNoAnimSkillInfo(UObject* WorldContextObject = nullptr);
class UB2InfluenceSkillInfo* StaticFindInfluenceSkillInfo(UObject* WorldContextObject = nullptr);
class UB2DamageEffectInfo* StaticFindDamageEffectInfo(UObject* WorldContextObject = nullptr);
class UB2BuffModeEffectInfo* StaticFindBuffModeEffectInfo(UObject* WorldContextObject = nullptr);
class UB2SkillAnimInfo* StaticFindSkillAnimInfo(UObject* WorldContextObject = nullptr);
class UB2GuildNPCInfo* StaticFindGuildNPCInfo(UObject* WorldContextObject = nullptr);
class UTutorialControlBotInfos* StaticFindTutorialControlBotInfos();
class UB2CombatConstantInfo* StaticFindCombatConstantInfo(UObject* WorldContextObject = nullptr);
//
#if !UE_BUILD_SHIPPING
/** To track begin/end of code block, usually a function. */
struct FB2ScopedCallTracker
{
	FB2ScopedCallTracker(FString InScopeName);
	FB2ScopedCallTracker(const ANSICHAR* InScopeName);
	~FB2ScopedCallTracker();

	static bool bIsOn; // Allow turn it off at runtime to remove verbose log

	// Limitations to execution time to display specially for warning.
	static float PerfMarkLv1Time;
	static float PerfMarkLv2Time;

private:
	double RecordedStartTime;
	FString ScopeName;
};
#endif
//추후 LogManager로 옮길 예정임
#if B2_PROFILE_LEVEL
#define BLADE2_SCOPE_CYCLE_COUNTER(STATNAME)								\
	BLADE2_DECLARE_STAT(BLADE2_##STATNAME,STATNAME);						\
	static DEFINE_STAT(STATNAME)											\
	FScopeCycleCounter CycleCount_BLADE2_##STATNAME(GET_STATID(STATNAME));

DECLARE_STATS_GROUP(TEXT("BLADE2"), STATGROUP_BLADE2, STATCAT_BLADE2);

#define BLADE2_DECLARE_STAT(Description, StatName)																					\
DECLARE_STAT(TEXT(#Description), StatName, STATGROUP_BLADE2, EStatDataType::ST_int64, true, true, FPlatformMemory::MCR_Invalid);
#else
#define BLADE2_SCOPE_CYCLE_COUNTER(STATNAME)
#endif

#if WITH_EDITOR
#define BII_CHECK(expr) checkSlow((expr))
#else
#define BII_CHECK(expr) check((expr)) 
#endif

/** It can be used for approximately spotting at which place a crash occurs. You may check Begin/End pair of ScopeName.
* B2_SCOPED_TRACK_LOG_L2 is for use multiple track log within the same scope, preventing "declaration shadows a local variable" warning.
* B2_SCOPED_TRACK_LOG_L3, L4 can be added depend on requirement. */
#if !UE_BUILD_SHIPPING
#define B2_SCOPED_TRACK_LOG(ScopeName) FB2ScopedCallTracker _ScopedCallTrackerDummy(ScopeName)
#define B2_SCOPED_TRACK_LOG_L2(ScopeName) FB2ScopedCallTracker _ScopedCallTrackerDummy_L2(ScopeName)
#define B2_SCOPED_TRACK_LOG_AUTO() FB2ScopedCallTracker _ScopedCallTrackerDummy(__FUNCTION__);
#else
#define B2_SCOPED_TRACK_LOG(ScopeName)
#define B2_SCOPED_TRACK_LOG_L2(ScopeName)
#define B2_SCOPED_TRACK_LOG_AUTO()
#endif

#if !UE_BUILD_SHIPPING
/** To simply log execution time of a code block.
* For more sophisticated profiling, use unreal stat system interfaces */
struct FB2ScopedTimeLogger
{
	FB2ScopedTimeLogger(FString InScopeName);
	FB2ScopedTimeLogger(const ANSICHAR* InScopeName);
	~FB2ScopedTimeLogger();

	static bool bIsOn; // Allow turn it off at runtime to remove verbose log

	// Limitations to execution time to display specially for warning.
	static float PerfMarkLv1Time;
	static float PerfMarkLv2Time;

private:
	double RecordedStartTime;
	FString ScopeName;
};
#endif

/** Like B2_SCOPED_TRACK_LOG... */
#if !UE_BUILD_SHIPPING
#define B2_SCOPED_TIME_LOG(ScopeName) FB2ScopedTimeLogger _ScopedTimeLoggerDummy(ScopeName)
#define B2_SCOPED_TIME_LOG_L2(ScopeName) FB2ScopedTimeLogger _ScopedTimeLoggerDummy_L2(ScopeName)
#define B2_SCOPED_TIME_LOG_AUTO() FB2ScopedTimeLogger _ScopedTimeLoggerDummy(__FUNCTION__);
#else
#define B2_SCOPED_TIME_LOG(ScopeName)
#define B2_SCOPED_TIME_LOG_L2(ScopeName)
#define B2_SCOPED_TIME_LOG_AUTO()
#endif

//bit flag
#define TO_FLAG_UINT8(Bit) (1 << static_cast<uint8>(Bit))
//

/** Common templates for StaticFind** which is based on Blueprint asset */
template<class AssetClassT>
AssetClassT* StaticFindBPInfoAssetTempl(const TCHAR* InIniSectionName, const TCHAR* InIniKeyName)
{
	if (GMinimalDLCFrontMode){
		return NULL; // Remove as much as we can for DLC front mode.
	}

	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("StaticFindBPInfoAssetTempl, %s"), InIniKeyName));

#if WITH_EDITOR // In formal game, InfoAsset loading must be with BladeIIGameMode (not with StartupModule).
	checkSlow(CanLoadInfoAssetWhileStartupModule() || ABladeIIGameMode::bDevCheckBladeIIGameModeEnteredOnce);
#endif

	//
	// It can be slow. Cache the result as possible, do NOT use this for like every tick.
	//

	AssetClassT* FoundInfo = NULL;

	FString ClassInfoPath;
	GConfig->GetString(InIniSectionName, InIniKeyName, ClassInfoPath, GGameIni);

	ConstructorHelpers::StripObjectClass(ClassInfoPath, true);
	TSubclassOf<AssetClassT> FoundInfoClass = ConstructorHelpersInternal::FindOrLoadClass(ClassInfoPath, AssetClassT::StaticClass());
	// Be aware that It returns "DefaultObject", not an instance of it.
	FoundInfo = FoundInfoClass ? Cast<AssetClassT>(FoundInfoClass->GetDefaultObject()) : NULL;

	BII_CHECK(FoundInfo);
	return FoundInfo;
}
////////////////////////////////////////////////////////////////////////////////////////////

/** Unload lazy loaded TAsset info assets. It is meant to be called when current level is about to be unloaded.
* It is not only for memory management, but also to ensure safe asset load condition.
* There could be a problem loading TAssets without this kind of unloading. */
void ManualUnloadBeforeNewBeginning(UObject* WorldContextObject);
/** For additional memory saving at stage clear moment when some big fat glorious father mocking effects will be loaded and played soon */
void ManualUnloadOnStageClear(UObject* WorldContextObject);

/** Mainly intended to be used for copying BaseBP settings for PCClassInfo/NPCClassInfo.
* For an actor, it can copies properties of sub objects (components) too. You might want to set bExcludeSubComponents to true to let the actor have its own component.
* For anything else, you may exclude specific properties by ExcludeProperties. */
void CopyObjectProperties(UObject* ObjCopyTo, UObject* ObjCopyFrom, bool bExcludeTransient = true, bool bExcludeSubComponents = true, const TArray<FName>& ExcludeProperties = TArray<FName>());

/** You can put this to some entry place that you don't want to show to team members or public etc. */
void DevShowNotReadyYetMessage(FString Message = FString(TEXT("DevShowNotReadyYet")));
void DevShowError(FString Message);
/** Slightly different version */
void DevShowNotReadyYetPopup();

extern bool isLocalConnectIniFileExist();

/** Guess this can be used somewhat often? */
void PopupGoToShopForGemMessage();
void PopupGoToShopForGoldMessage();
void PopupGoToCounterDungeonForUpgradeStone(int32 _StoneType);
void PopupGoToHeroTowerMessage();
void PopupGoToControlGameModeMessage(int32 _ScrollType);
void PopupGotoShopForDarkCrystalMessage();

/** when you modify this, please note that this information can be saved with instances
* also DefaultEngine.ini [/Script/Engine.CollisionProfile] should match with this list **/
#define COLLISION_PROJECTILE	ECC_GameTraceChannel1

/** See also UCollisionProfile for other profile name define. Some duplicated entries are here, just to not including the file. */
const FName CollisionProfileName_BlockAll = FName(TEXT("BlockAll"));
const FName CollisionProfileName_Trigger = FName(TEXT("Trigger"));
const FName CollisionProfileName_NoCollision = FName(TEXT("NoCollision"));
const FName CollisionProfileName_OverlapAllDynamic = FName(TEXT("OverlapAllDynamic"));
const FName CollisionProfileName_OverlapMostDynamic = FName(TEXT("OverlapMostDynamic"));

/** Those material param names are defined at UI TexAtlas master material. */
const FName MtrlParamName_UIAtlasSampleX(TEXT("AtlasSample_1_X"));
const FName MtrlParamName_UIAtlasSampleY(TEXT("AtlasSample_2_Y"));
const FName MtrlParamName_UIAtlasSampleWidth(TEXT("AtlasSample_3_Width"));
const FName MtrlParamName_UIAtlasSampleHeight(TEXT("AtlasSample_4_Height"));
const FName MtrlParamName_UIAtlasWholeWidth(TEXT("AtlasWholeWidth"));
const FName MtrlParamName_UIAtlasWholeHeight(TEXT("AtlasWholeHeight"));
const FName MtrlParamName_UIAtlasTexture(TEXT("AtlasTexture"));
const FName MtrlParamName_UIAdditionalBlendColor(TEXT("AdditionalBlendColor")); // This parameter is visible once bAdditionalColorBlending switch is On.
const FName MtrlParamName_Color(TEXT("ColorVariControl"));
const FName MtrlStaticParamName_NoAtlas(TEXT("bNoAtlas"));
const FName MtrlParamName_NoAtlasSingleTexture(TEXT("NoAtlasSingleTexture"));

/** Commonly used material parameter name for displaying ring type progress. basically developed for skill cooltime. */
const FName MtrlParamName_CoolTimeRingProgress = FName(TEXT("CooltimeProgress"));

////////////////////////////////////////////////////////////////////////////////
// Those Interfaces below are initially intended for item compose scene to set-up a 3D material same as atlas-sampled UI material.
// Considering they can be used at more than one place, places them here.
/** It create MIDs for NamedMaterialSlots of PSComp's template PS then set them to EmitterMaterials at the same index. */
void SetupNamedMIDForFxComp(class UParticleSystemComponent* InPSComp);

class UMaterialInstanceDynamic* GetFxNamedMID(class UParticleSystemComponent* InPSComp, const FName& MatSlotName);
bool HasFxNamedMID(class UParticleSystemComponent* InPSComp, const FName& MatSlotName);

/** Gets Atlas sampling parameters from designated MIC being used for UI widget element then set the same name of parameter to MIDToSet.
 * It can be used for setting 3D object look the same as some UI widget element. */
void SetupMIDAtlasParamFromUIMIC(class UMaterialInstanceDynamic* MIDToSet, class UMaterialInstanceConstant* RefUIMIC);
void SetupMIDNonAtlasParamFromUIMIC(class UMaterialInstanceDynamic* MIDToSet, class UMaterialInstanceConstant* RefUIMIC);

void SetMIDScalarParam(class UMaterialInstanceDynamic* MIDToSet, const FName& ParamName, const float& ParamValue);
void SetStageResultStar(class UMaterialInstanceDynamic* MIDToSet, const uint8 InStar);

////////////////////////////////////////////////////////////////////////////////
int32 B2ServerRoundDown(double a, float NumDigit);

//////////////////////////////////////////////////
// New UIs are based on this level. Typical DJLegcy-based lobby UIs have z-order of one digit. Must be above them.
#define BII_WIDGET_ZORDER_NEW_UI_MAIN 10
#define BII_WIDGET_ZORDER_NEW_UI_MustTop 1000
//////////////////////////////////////////////////

// Defining In-game widget's z-order.. -> For old DJLegacy, some of them might become obsolete.
#define BII_WIDGET_ZORDER_FLOATINGHPBAR 0
#define BII_WIDGET_ZORDER_FLOATING_DEFAULT 1
// Better have higher z order than other possible widgets based on world coordinate (such as floating HPbar widget)
#define BII_WIDGET_ZORDER_INGAMEHUD 10
// It is above the In-game HUD, for some specially created widgets.
#define BII_WIDGET_ZORDER_SEPARATE_BELOW_UIMAIN 9
#define BII_WIDGET_ZORDER_SEPARATE_ABOVE_UIMAIN 11
#define BII_WIDGET_ZORDER_PRELOADINGSCREEN 20
// Another floating widget.
#define BII_WIDGET_ZORDER_LOBBYONHEADDISPLAY 0
// It need to be almost at the top
#define BII_WIDGET_ZORDER_DEBUG_TEXT_WIDGET 50 

////////////////////////////////////////////////////
// Other miscellaneous utilities. 

/** From raw second to 24h/60m/60s */
void GetTimeFormatFromRawSecond(int32 InRawSecond, int32& OutHour, int32& OutMin, int32& OutSec);

FVector GetGroundHitLocation(const FVector& BeginLocation, AActor* TargetActor);

/** To set the initial z location of spawned character. Almost like editor snap object to ground feature. */
void SnapBladeIICharacterToGround(class ABladeIICharacter* InB2Char);

/** Snapping for SkeletalMeshCompoent */
void SnapSkeletalMeshComponentToGround(class USkeletalMeshComponent* InMeshComp);

/** Get the height offset from SkeletalMeshActor to the BladeIICharacter having the same skeletal mesh. */
float ZOffsetFromMatchingSKActorToBIICharacter(class ABladeIICharacter* InB2Char);

/** Get delta time where the CustomTimeDelation of designated character is ignored, to let it can be applied same to other world objects. */
FORCEINLINE float GetDeltaTimeAsOtherWorldObj(float InRawDeltaTime, class AActor* InCustomDelatedActor)
{
	const float ActorCustomTimeDelation = InCustomDelatedActor ? InCustomDelatedActor->CustomTimeDilation : 1.0f;
	return (ActorCustomTimeDelation != 1.0f && ActorCustomTimeDelation > SMALL_NUMBER) ? (InRawDeltaTime / ActorCustomTimeDelation) : InRawDeltaTime;
}

/** It simply activate (and possibly others if necessary) all attached particle system components to make sure them visible in some circumstances. */
void EnsureActivateStaticallyAttachedPSCs(class AActor* InActor);
void EnsureActivatePSCInternal(class UParticleSystemComponent* InPSC);
/** See if InPSC is to be visible significantly long time for some visibility handling. */
bool IsStaticallyVisiblePSC(class UParticleSystemComponent* InPSC);

bool IsActiveControlTutorial();
bool IsActiveFakeChannelMessage();

// Use UGameplayStatics::SpawnEmitter** instead in most circumstances..
UParticleSystemComponent* CreateParticleSystemHelper(UParticleSystem* EmitterTemplate, UWorld* World, AActor* Actor, bool bAutoDestroy);

UCameraComponent* GetCurrentActiveCamComp(UObject* WorldContextObject);

/** You can go back to normally expected in-game camera in most circumstances by using this. */
void ApplyNormallyExpectedInGameCamera(class ABladeIIGameMode* InB2GM, class ABladeIIPlayerController* InLocalPC, float InBlendInTime);

/** You may optionally append "..." to clamped text by bAppendEllipsisIfClamped. */
FText ClampTextLength(const FText& InSrcText, int32 InMaxLen, bool bAppendEllipsisIfClamped = false);

/** Returns floating point number text with fractional part up to InMaxFracDigit. It removes unnecessary 0. E.g. when InMaxFracDigit is 2, 25.60 -> 25.6 */
FText GetAutoFractionalFormattedText(float InNumber, int32 InMaxFracDigit);
/** Rather for a special case.. but our skill and tag cooltime display use this scheme. */
FText GetAutoFractionalFormattedText_FirstFracCountOnly(float InCountNumber, float InMaxNumber, int32 InMaxFracDigit);

FSlateColor GetPowerColor(const int32 InCurrentPower, const int32 InRecommandPower);
//
//// It assumes UIHeader is created and running.
void SetLobbyUIHeaderTitle(const FText& InTitleText);
FORCEINLINE void SetLobbyUIHeaderTitleByGeneralTextTableKey(const FString& InGeneralTextTableKey)
{
	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, InGeneralTextTableKey));
}

/** 
 * Gather all (but not probably all, most..) particle systems which can be played by designated skeletal mesh component, 
 * it is intended to find PS from particle playing anim notifies from all animations that the skeletal mesh component can play.
 * For special use..
 */
void TryGatherAllBoundPSFromSKComp(class USkeletalMeshComponent* InSKComp, TMap<FName, UParticleSystem*>& OutAllFound);
/** Part of TryGatherAllBoundPSFromSKComp, and will be used independently */
void TryGatherAllBoundPSFromAnimSeq(class UAnimSequenceBase* InAnimSeq, TMap<FName, UParticleSystem*>& OutAllFound);
/** Returns UParticleSystem resource if InNotifyEvent is about particle system use. */
class UParticleSystem* CheckNotifyEventForParticleSystemUse(FAnimNotifyEvent& InNotifyEvent);

/** Display text to screen-sized DebugTextWidget */
void DrawDebugText(float InStartX, float InStartY, const FString& InText, const FLinearColor& InFontColor, int32 InFontSize = 12, bool bDrawShadow = true, const FLinearColor& InShadowColor = FLinearColor::Black);

#if WITH_BII_ON_SCREEN_DEBUG_TEXT
/** Draw text for a duration, you just call it once, and it will be drawn for designated time and disappear.
 * Use BII_SCREEN_LOG instead of direct use. */
void DrawTimedDebugText(const FString& InText, const FLinearColor& InFontColor, int32 InFontSize = 12, float InDisplayTime = 2.0f, bool bDrawShadow = true, const FLinearColor& InShadowColor = FLinearColor::Black);
#define BII_SCREEN_LOG(LogText, InFontColor, InFontSize, InDisplayTime) DrawTimedDebugText(LogText, InFontColor, InFontSize, InDisplayTime)
#else
#define BII_SCREEN_LOG(LogText, InFontColor, InFontSize, InDisplayTime)
#endif

// Actual mobile device or mobile preview..
bool IsUsingMobileRendering(UWorld* InWorld);
// For use modulated shadow unless it is WholeScene shadow, in mobile renderer.
FORCEINLINE bool ShouldUsePerObjectModulatedShadow(UWorld* InWorld) { return IsUsingMobileRendering(InWorld); }
void TurnOffMeshComponentsDyamicShadowForModulated(AActor* InOwnerActor);

void ForceSendReallocateRenderTargetsSignal();

////////////////////////////////////////////////////

const int32 MAX_SKILL_INPUT = 6; //active 3 + 6StarSkill 1 + Tag 1 + Counter 1
const int32 MAX_CLASS_SKILL_INPUT = 3;
const int32 MAX_ACTIVE_SKILL_INPUT = 4;
const int32 WEAPON_6STAR_SKILL_INDEX = 3;
const int32 TAG_SKILL_INDEX = 4; // Manage tag as a special detachment of skill set.
const int32 COUNTER_SKILL_INDEX = 5; // Manage counter as a special detachment of skill set.

const int32 MAX_COUNTER_ATTACK_POINT = 8;

namespace AutoBattleType
{
	const int32 NONE = 0;
	const int32 Attack = 1;
	const int32 Skill = Attack << 1;
	const int32 NotTag = Attack << 2;
	const int32 MAX = INT_MAX;
};

int32 UpdateAutoBattleType(int32 InAutoBattleType);

////////////////////////////////////////////////////

const int32 TUTORIAL_INCREASE_MOTION = 2;

////////////////////////////////////////////////////
FORCEINLINE float GetTimeToDecideInputClickOrTab() { return 0.3f; }

const uint32 CONNECTION_STATE_CONNECTING			=	10;
const uint32 CONNECTION_STATE_CONNECTED				=	20;
const uint32 CONNECTION_STATE_SERVICENOTAVAILABLE	=	30;

namespace Utilities
{
	struct FBlendHelper
	{
		/** worker function for APlayerController::SmoothTargetViewRotation() */
		static float BlendRotation(float DeltaTime, float BlendC, float NewC)
		{
			if (FMath::Abs(BlendC - NewC) > 180.f)
			{
				if (BlendC > NewC)
				{
					NewC += 360.f;
				}
				else
				{
					BlendC += 360.f;
				}
			}

			if (FMath::Abs(BlendC - NewC) > 22.57f)
			{
				BlendC = NewC;
			}
			else
			{
				BlendC = BlendC + (NewC - BlendC) * FMath::Min(1.f, 24.f * DeltaTime);
			}

			return FRotator::ClampAxis(BlendC);
		}

		static float BlendRotationWithSpeed(float DeltaTime, float BlendC, float NewC, float RotSpeed)
		{
			// Assume RotSpeed 0 as infinity
			if (RotSpeed == 0.f)
			{
				return FRotator::ClampAxis(NewC);
			}

			if (FMath::Abs(BlendC - NewC) > 180.f)
			{
				if (BlendC > NewC)
				{
					NewC += 360.f;
				}
				else
				{
					BlendC += 360.f;
				}
			}

			float RotDelta = RotSpeed * DeltaTime;
			if (FMath::Abs(BlendC - NewC) <= RotDelta)
			{
				BlendC = NewC;
			}
			else
			{
				BlendC = BlendC + FMath::Sign(NewC - BlendC) * RotDelta;
			}

			return FRotator::ClampAxis(BlendC);
		}
	};

	struct FConnectionState
	{
		enum class ESessionCloseReason: uint8
		{
			ESessionCloseReason_None,
			ESessionCloseReason_UpdateProtocolEvent,
			ESessionCloseReason_ClosedNotify,
		};
	public:
		static FConnectionState& GetInstance();

		bool IsConnected() const
		{
			return ConnectionState == CONNECTION_STATE_CONNECTED;
		}

		void SetState(uint32 state)
		{
			ConnectionState = state;
		}

		uint32 GetState() const
		{
			return ConnectionState;
		}

		void SetSessionCloseReason(ESessionCloseReason _SessionCloseReason)
		{
			SessionCloseReason = _SessionCloseReason;
		}
		ESessionCloseReason GetSessionCloseReason()
		{
			return SessionCloseReason;
		}

		void Initialize();
		void Uninitialize();

	private:
		FConnectionState() : ConnectionState(CONNECTION_STATE_SERVICENOTAVAILABLE)
		{
		}

		~FConnectionState()
		{
		}

		FConnectionState(FConnectionState const&)				= delete;
		FConnectionState& operator =(FConnectionState const&)	= delete;

		uint32	ConnectionState;
		ESessionCloseReason SessionCloseReason;//ServiceNotAvailableEvent가 발생 전에 Disconnect 이유가 있을때 값을 넣어줌(ex>중복 접속)
	};

	FORCEINLINE	bool IsNearlyEqual(FVector const& lhs, FVector const& rhs)
	{
		return FMath::IsNearlyEqual(lhs.X, rhs.X) && FMath::IsNearlyEqual(lhs.Y, rhs.Y) && FMath::IsNearlyEqual(lhs.Z, rhs.Z);
	}

	FORCEINLINE	bool IsNearlyEqual(FRotator const& lhs, FRotator const& rhs)
	{
		return FMath::IsNearlyEqual(lhs.Pitch, rhs.Pitch) && FMath::IsNearlyEqual(lhs.Yaw, rhs.Yaw) && FMath::IsNearlyEqual(lhs.Roll, rhs.Roll);
	}
}

template <int32 N>
struct RaiseTen
{
	enum
	{
		value = 10 * RaiseTen<N - 1>::value
	};
};

template <>
struct RaiseTen<0>
{
	enum
	{
		value = 1
	};
};

// Extract N'th digit counted "from rear". Unreal might provide this kind of utility?
template <int32 N>
int32 ExtractNthDigitR(int32 InNumber)
{
	return ((InNumber % RaiseTen<N>::value) / RaiseTen<N - 1>::value);
}

template <int32 N>
int32 AddNthDigitR(int32 InNumber, int32 InAdd)
{
	return InNumber + (InAdd * RaiseTen<N - 1>::value);
}

// For any case that you cannot use ExtractNthDigitR due to dynamic digit number parameter.
FORCEINLINE int32 ExtractNthDigitR_Dynamic(int32 InNumber, int32 NFromRear) // The least NFromRear is 1, not 0.
{
	const int32 LowerValue = FMath::RoundToInt(FMath::Pow(10.0f, (float)(NFromRear - 1))); // 1, 10, 100...
	const int32 UpperValue = FMath::RoundToInt(FMath::Pow(10.0f, (float)(NFromRear))); // 10, 100, 1000...
	
	return (InNumber % UpperValue) / LowerValue;
}

EB2GameMode GetB2GameModeType(class UObject* WorldContextObject);

class ABladeIIGameMode* GetBladeIIGameMode(UObject* WorldContextObect);

void SetInvenQuickSlot(bool bSet);
bool GetInvenQuickSlot();

void SetVibration(bool bSet);
bool IsVibration();

void SetViewVibBtn(bool bSet);
bool IsViewVibBtn();

void SetViewPromotion(bool bSet);
bool IsViewPromotion();
void SetPurchasesInit(bool bSet);
bool IsInitPurchases();

void SetDevModeContent(bool IsDevMode);
bool IsDevModeContent();
void SetClearNewTutorial(bool IsClear);
bool IsClearNewTutorial();
void SetTrueOpenPopupAppStoreReview();
bool IsOpenPopupAppStoreReview();
bool ShouldShowGDCVersionContent(); // [GDC] Feb 2017 temporary.
bool ShouldShowCBTVersionContent();	// [CBT] 2018 3~ 
bool ChangeGuildBattleId(int32 SeasonID, int32 turnID);

// Game settings mainly controlled by UISetting.
void SaveGameSetting_Graphics(int32 InValue);
void SaveGameSetting_FrameLimit(int32 InValue);
void SaveGameSetting_Resolution(int32 InValue);
void SaveGameSetting_BGMVolume(float InValue);
void SaveGameSetting_EffectVolume(float InValue);
void SaveGameSetting_Vibration(int32 InValue);
void SaveGameSetting_KeepScreenOn(int32 InValue);
void SaveGameSetting_GamePush(int32 InValue);
void SaveGameSetting_kakaoDayPush(int32 InValue);
void SaveGameSetting_KakaoNightPush(int32 InValue);
void SaveGameSetting_SoundLocType(int32 InValue);

bool LoadGameSetting_Graphics(int32& OutValue); // return false if not found
/** If no saved EB2GraphicsLevel detected, set OutValue as suggested value based on current engine scalability value.
 * It returns false if user saved EB2GraphicsLevel is not found */
bool LoadGameSetting_Graphics_OrByDefault(int32& OutValue);
bool LoadGameSetting_FrameLimit(int32& OutValue);
bool LoadGameSetting_FrameLimit_OrByDefault(int32& OutValue);
bool LoadGameSetting_Resolution(int32& OutValue);
bool LoadGameSetting_Resolution_OrByDefault(int32& OutValue);

bool LoadGameSetting_BGMVolume(float& OutValue);
bool LoadGameSetting_EffectVolume(float& OutValue);
bool LoadGameSetting_Vibration(int32& OutValue);
bool LoadGameSetting_KeepScreenOn(int32& OutValue);
bool LoadGameSetting_GamePush(int32& OutValue);
bool LoadGameSetting_KakaoDayPush(int32& OutValue);
bool LoadGameSetting_KakaoNightPush(int32& OutValue);
bool LoadGameSetting_SoundLocType(int32& OutValue);

void ReserveResolutionLevelForNextAppStart(EB2ResolutionLevel InReserveLevel);
EB2ResolutionLevel LoadReservedResolutionLevelOfPrevAppRun();
FORCEINLINE void ClearResolutionLevelReserve() { ReserveResolutionLevelForNextAppStart(EB2ResolutionLevel::End); }

void InitializeAllGameSettingData(UObject* WorldContextObject, bool bExtraContentScaleFallback = false);

void SaveGameSetting_AccessTerm(bool InValue);
bool IsAccessTerm();

void SaveGameData_DLCDownload(bool bSuccess);
bool IsDLCDownload();

// 친구 초대 정보 로컬 저장
void SaveGameData_KakaoInvitedFriends(const TArray<FString> &SaveList);
bool LoadGameData_KakaoInvitedFriends(TArray<FString> &OutList);

void SetPopupOpen(bool bSet);
bool IsPopupUIOpen();

//b2network::B2ContentsMode
bool CheckContentsModeState(int32 ContentsModeType);

void GlobalSetGameVolumeCommon(const FName& SoundClassName, float InVolume);
FORCEINLINE void GlobalSetGameBGMVolume(float InVolume)
{ // We got uniquely defined naming here. 
	GlobalSetGameVolumeCommon(FName(TEXT("Main_BGM_Class")), InVolume);
}
FORCEINLINE void GlobalSetGameEffectVolume(float InVolume)
{// We got uniquely defined naming here. 
	GlobalSetGameVolumeCommon(FName(TEXT("Main_SFX_Class")), InVolume);
}

/** It is only necessary until UImage is provided with some interface like SetBrushFromPaperSprite. */
void SetImageBrushFromPaperSprite(UImage* InImageToSetBrush, class UPaperSprite* InSpriteToSet);
/** Some workaround needed because we can't get MID from Image widget with PaperSprite. */
void SetBlendColorForPaperSpriteBoundImage(UImage* InImageWithPaperSprite, const FLinearColor InBlendColor);

class ASkeletalMeshActor* SpawnPuppetActor(FString TrackName, ALevelSequenceActor * MatineeActor, EPCClass CharClass, const class ICharacterDataStore* DataStore, class UAnimationAsset* ToPlayAnimation);

/** Replacing deprecated static UNavigationSystem::GetRandomReachablePointInRadius.. */
FVector GetRandomReachablePointInRadiusWithCurrNavSys(UWorld* InWorld, const FVector& Origin, float Radius, ANavigationData* NavData = NULL, TSubclassOf<UNavigationQueryFilter> FilterClass = NULL);
//
//////////////////////////////////////////////////////////////////////////////////
// Common internal functionality for specific way of TSoftObjectPtr/TSoftClassPtr use.
template<class TAssetClass>
TAssetClass* GetSomeLazyLoadedAssetCommon(FStreamableManager& InLoadManager, TSoftObjectPtr<TAssetClass>& InAssetToLoad, TAssetClass** LoadedPtrCache, const FString& InfoAssetNamePrefix = TEXT("Unknown InfoAsset"))
{
	checkSlow(LoadedPtrCache); // Must provide caching ptr for use this template. There are some exceptional cases, but then don't have to use this util in that case.
	const bool bAssetSpecified = (InAssetToLoad.ToString().Len() > 0); // Will do some check only when asset is specified.
	if (InAssetToLoad.IsValid() == false && bAssetSpecified)
	{
		InLoadManager.LoadSynchronous(InAssetToLoad.ToSoftObjectPath());
	}

	if (!LoadedPtrCache)
	{ // Just provided for some case, but do NOT resort in this path.
		return InAssetToLoad.Get();
	}

	*LoadedPtrCache = InAssetToLoad.IsValid() ? InAssetToLoad.Get() : NULL;
	//checkSlow(!bAssetSpecified || *LoadedPtrCache);
#if WITH_EDITOR
	if (bAssetSpecified && !(*LoadedPtrCache))
	{ // Better not put Unicode at header.
		EditorPopupTAssetBlowupWarning(InfoAssetNamePrefix, InAssetToLoad.ToString());
	}
#endif
	return *LoadedPtrCache;
}
template<class TAssetClass>
void UnloadSomeLazyLoadedAssetCommon(FStreamableManager& InLoadManager, TSoftObjectPtr<TAssetClass>& InAssetToUnLoad, TAssetClass** LoadedPtrCache)
{
	checkSlow(LoadedPtrCache); // Must provide caching ptr for use this template
	if (InAssetToUnLoad.IsValid())
	{
		if (LoadedPtrCache){
			*LoadedPtrCache = NULL;
		}
		InLoadManager.Unload(InAssetToUnLoad.ToSoftObjectPath());
	}
}
// Common template for TSoftClassPtr
template<class TAssetClass>
TSubclassOf<TAssetClass> GetSomeLazyLoadedClassAssetCommon(FStreamableManager& InLoadManager, TSoftClassPtr<TAssetClass>& InAssetClassToLoad, TSubclassOf<TAssetClass>* LoadedPtrCache, const FString& InfoAssetNamePrefix = TEXT("Unknown InfoAsset"))
{
	checkSlow(LoadedPtrCache); // Must provide caching ptr for use this template. There are some exceptional cases, but then don't have to use this util in that case.
	const bool bAssetSpecified = (InAssetClassToLoad.ToString().Len() > 0); // Will do some check only when asset is specified.
	if (InAssetClassToLoad.IsValid() == false && bAssetSpecified)
	{
		InLoadManager.LoadSynchronous(InAssetClassToLoad.ToSoftObjectPath());
	}
	
	if (!LoadedPtrCache)
	{ // Just provided for some case, but do NOT resort in this path.
		return InAssetClassToLoad.Get();
	}

	*LoadedPtrCache = InAssetClassToLoad.IsValid() ? InAssetClassToLoad.Get() : NULL;
	checkSlow(!bAssetSpecified || *LoadedPtrCache);
#if WITH_EDITOR
	if (bAssetSpecified && !(*LoadedPtrCache))
	{ // Better not put Unicode at header.
		EditorPopupTAssetBlowupWarning(InfoAssetNamePrefix, InAssetClassToLoad.ToString());
	}
#endif
	return *LoadedPtrCache;
}
template<class TAssetClass>
void UnloadSomeLazyLoadedClassAssetCommon(FStreamableManager& InLoadManager, TSoftClassPtr<TAssetClass>& InAssetClassToUnLoad, TSubclassOf<TAssetClass>* LoadedPtrCache)
{
	checkSlow(LoadedPtrCache); // Must provide caching ptr for use this template
	if (InAssetClassToUnLoad.IsValid())
	{
		if (LoadedPtrCache){
			*LoadedPtrCache = NULL;
		}
		InLoadManager.Unload(InAssetClassToUnLoad.ToSoftObjectPath());
	}
}
const int32 GENERAL_TASSET_ASYNC_LOAD_TIMEOUT = 60.0f;
#if WITH_EDITOR
void EditorPopupTAssetBlowupWarning(const FString& InfoAssetName, const FString& WarningAssetName);
#endif

#if !UE_BUILD_SHIPPING
void CheckResetAssumeNoWorldLoadingCondition(UObject* WorldContextObject);
#endif
#if BII_STATS
// 
// A struct to accumulate specific types of numbers and get some statistics from collected numbers.
template<typename ValueType>
struct FSimpleStatisticRecorder
{
	FSimpleStatisticRecorder(){}
	static const int32 DataCollectLimit = 100000;
private:
	TArray<ValueType> GatheredData;
public:
	FORCEINLINE void Reset() { GatheredData.Empty(); }
	FORCEINLINE int32 GetRecordedNum() const { return GatheredData.Num(); }

	void AddValue(ValueType InValue) 
	{ 
		if (GatheredData.Num() < DataCollectLimit) {
			GatheredData.Add(InValue);
		}
	}

	FORCEINLINE ValueType GetTotalAverage() { return GetRangedAverage(true, 1.0f); }
	/** Get average only from values in range of specified Proportion (0.0 ~ 1.0). Either counted from highest or lowest */
	ValueType GetRangedAverage(bool bFromHighRank, float InProportion)
	{
		if (GatheredData.Num() == 0) {
			return 0;
		}

		if (bFromHighRank)
		{
			GatheredData.Sort( // Descending order
				[](const ValueType& A, const ValueType& B)
				{
					return A > B;
				});			
		}
		else
		{
			GatheredData.Sort( // Ascending order
				[](const ValueType& A, const ValueType& B)
				{
					return A < B;
				});
		}

		// Not quite precise if GatheredData size is small
		const int32 RequiredElemNum = static_cast<int32>(FMath::Clamp(InProportion, 0.0f, 1.0f) * (float)GatheredData.Num());

		// From the first element. GatheredData is sorted in desired manner.
		ValueType TheSum = 0;
		for (int32 SI = 0; SI < RequiredElemNum; ++SI)
		{
			TheSum += GatheredData[SI];
		}
		// Use the most precise format for division..
		return static_cast<ValueType>((double)TheSum / (double)(RequiredElemNum));
	}

	float GetVariance()
	{
		if (GatheredData.Num() == 0) {
			return 0;
		}
		const ValueType AverageValue = GetTotalAverage();

		double SomeOfSquaredDeviation = 0.0; // Intermediate values can be very large, so might be safer to use double..

		for (ValueType ThisValue : GatheredData)
		{
			SomeOfSquaredDeviation += FMath::Square((double)ThisValue - (double)AverageValue);
		}
		return SomeOfSquaredDeviation / (double)GatheredData.Num();
	}
	float GetStandardDeviation()
	{
		return FMath::Sqrt(GetVariance());
	}
};
#endif

bool CreateSoundCueConcatenator(class USoundCue *OutSound, class USoundWave *SoundWaveA, class USoundWave *SoundWaveB);

///////////////////////////////Time////////////////////////
FDateTime GetServerTimeToDateTime(int64 ServerTime);

//Copy DateTimeStructCustomization::ConvertTime;
FDateTime ConvertTime(const FDateTime& InDate, int32 InTimezone, int32 OutTimezone);

//Copy DateTimeStructCustomization::ConvertShortTimezone;
int32 ConvertShortTimezone(int32 ShortTimezone);

FDateTime ConvertUTCToLocalTime(const FDateTime& InDate);

struct FItemGradeInfo
{
	static CONSTEXPR int32 MINIMUM_SURPASS_STAR_GRADE = 6;
	static CONSTEXPR int32 MINIMUM_ADVANCED_SURPASS_STAR_GRADE = 11;	// 11성 이상은 11성 이상의 장비들만을 초월 할 수 있다.
	static CONSTEXPR int32 MINIMUM_ADVANCED_UPGRADE_STAR_GRADE = 10;	// 상위 승급석이 필요로 하는 장비 성
	static CONSTEXPR int32 MAX_NORMAL_ITEM_STAR_GRADE = 12;

	static FText GetItemGradeColor(uint32 ItemGrade);

	FItemGradeInfo() = delete;
	FItemGradeInfo(const FItemGradeInfo& InFItemGradeInfo) = delete;
};
//////////////////////////////////////////////////////////////////////////////////
