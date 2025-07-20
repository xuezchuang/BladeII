// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "Event.h"
#include "B2UIDocChapter.h"
#include "EventSubsystem.h"
#include "Components/Button.h"
#include "Engine/DataTable.h"
#include "Sound/SoundCue.h"
#include "BladeIICharacter.h"
#include "TutorialManager.generated.h"

#define MAX_VISIBLE_CONTENTOPEN_COUNT	2

UENUM()
enum class ETutorialType : uint8
{
	Default,
	ContentOpen,

	Max
};

UENUM()
enum class ETutorialRequire : uint8 
{
	None,
	ClickButton,
	ClickHighlight,
	OpenUIComplete,
	HardCoding
};

UENUM()
enum class ETutorialAction : uint8
{
	None,
	TutorialBegin,
	Highlight,
	BlockHilight,
	TutorialClear,
	Dialog
};

UENUM()
enum class ETutorialStepState : uint8
{
	WaitForRequirement,
	Passable
};

UENUM()
enum class ETutorialArrow : uint8 
{
	None,
	Left,
	Right,
	Top,
	Bottom
};

enum class ENeedToCachingResponse : uint8
{
	None,
	NeedCahcing,
	Cahced,
};

#define TutorialID_GDC 0
#define TutorialID_FirstStageClear 100
#define TutorialID_PlayerSkill 200
#define TutorialID_Store 300
#define TutorialID_Occupy 400
#define TutorialID_ItemEnhance 500
#define TutorialID_ItemLevelup 600
#define TutorialID_PvPOneAndOne 700
#define TutorialID_PlayerPromote 800
#define TutorialID_Relic 900
#define TutorialID_HeroTower 1000
#define TutorialID_Raid 1100
#define TutorialID_CounterDungeon 1200
#define TutorialID_Guild 1300
#define TutorialID_Wing 1400
#define TutorialID_ExpertDungon 1500
#define TutorialID_Ether 1600
#define TutorialID_PvPTeam 1700
#define TutorialID_Fairy 1800
#define TutorialID_Seal 1900
//#define TutorialID_NightmareDungeon 1800

USTRUCT()
struct FTutorialStep : public FTableRowBase
{
	GENERATED_BODY()
	FTutorialStep()
	{
		ViewSkipButton = true;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETutorialRequire Requirement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName RequireName; /* could be a UIName or Button WidgetName*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETutorialAction Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETutorialArrow ArrowDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USoundCue> SoundAssetPtr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool ViewSkipButton;

	FName StepKey;

	int32 TutorialID;
	int32 TutorialStep;

	void SetStepBasicInfoFromKey(const FName& InStepKey);
};

USTRUCT()
struct FTutorialInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TutorialID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETutorialType TutorialType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RequireLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RequireOpenStage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UTexture2D> TutorialTexture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UMaterialInterface> SpeakerMaterial;

	TArray<FTutorialStep>	StepList;

	UTexture2D* GetTutorialIconTexture() { return TutorialTexture.IsValid() ? TutorialTexture.LoadSynchronous() : nullptr; }
};

USTRUCT(BlueprintType)
struct FTutorialControlBotItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ItemTemplateId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ItemEquipPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ItemBaseStat;
};


USTRUCT(BlueprintType)
struct FTutorialControlBotInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString BotName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPCClass BotPCClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BotLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BotSkillId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTutorialControlBotItemInfo>	BotEquipItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETeamType BotTeamType;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UTutorialControlBotInfos : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTutorialControlBotInfo> TutorialControlBotInfos;
};

UCLASS()
class BLADEII_API UB2TutorialInfo : public UObject
{
	GENERATED_BODY()

public:
	UB2TutorialInfo(const FObjectInitializer& ObjectInitializer);

	const FTutorialInfo*	GetTutorialInfo(int32 TutorialID);
	const FTutorialStep*	GetTutorialStep(int32 TutorialID, int32 TutorialStep);

	void GenerateTutorialSteps();

	FORCEINLINE FName GetTutorialInfoName(int32 TutorialID) 
	{
		return FName(*FString::Printf(TEXT("Tutorial%d"), TutorialID));
	}

	FORCEINLINE FName GetTutorialStepName(int32 TutorialID, int32 TutorialStep)
	{
		return FName(*FString::Printf(TEXT("TutorialStep%d_%d"), TutorialID, TutorialStep));
	}

	void InitTutorialInfo();

	void LoadTutorialInfoTable();
	void LoadTutorialStepTable();
	void LoadTutorialControlBotInfos();

	const TArray<FTutorialControlBotInfo>* GetTutorialControlBotInfos();

private:

	bool bGeneratedStepList;

	UPROPERTY()
	UDataTable* TutorialInfoTable;

	UPROPERTY()
	UDataTable* TutorialStepTable;

	UPROPERTY()
	UTutorialControlBotInfos* TutorialControlBotInfos;
};

DECLARE_DELEGATE_RetVal_OneParam(bool, FStepExceptionFunc, const FTutorialStep*);
DECLARE_DELEGATE_RetVal_OneParam(bool, FTutorialExceptionFunc, const FTutorialInfo*);

#define DECLARE_TUTORIAL_DOC_FIELD(TYPE,NAME) DECLARE_DOC_FIELD(class TutorialManager*, TYPE,NAME)

#define DECLARESTEP_REQUIRE_EXCEPTION(Id, Step)	bool ExceptionRequire_Tutorial_##Id##Step( const FTutorialStep* StepInfo)
#define DEFINESTEP_REQUIRE_EXCEPTION(Id, Step)	bool TutorialManager::ExceptionRequire_Tutorial_##Id##Step( const FTutorialStep* StepInfo)

#define REGISTERSTEP_REQUIRE_EXCEPTION(Id, Step)												\
	{																									\
		FStepExceptionFunc ExceptionFunc;																\
		ExceptionFunc.BindRaw(this, &TutorialManager::ExceptionRequire_Tutorial_##Id##Step);	\
		FName StepKey(*GetTutorialStepKey(Id, Step));											\
		StepExceptionRequireExecs.Add(StepKey, ExceptionFunc);											\
	}																					

#define DECLARESTEP_ACTION_EXCEPTION(Id, Step)	bool ExceptionAction_Tutorial_##Id##Step(const FTutorialStep* StepInfo)
#define DEFINESTEP_ACTION_EXCEPTION(Id, Step)	bool TutorialManager::ExceptionAction_Tutorial_##Id##Step( const FTutorialStep* StepInfo)

#define REGISTERSTEP_ACTION_EXCEPTION(Id, Step)													\
	{																									\
		FStepExceptionFunc ExceptionFunc;																\
		ExceptionFunc.BindRaw(this, &TutorialManager::ExceptionAction_Tutorial_##Id##Step);	\
		FName StepKey(*GetTutorialStepKey(Id, Step));											\
		StepExceptionActionExecs.Add(StepKey, ExceptionFunc);											\
	}																					

#define DECLARETUTORIAL_END_EXCEPTION(Id)	bool ExceptionTutorial_##Id_End(const FTutorialInfo* TutorialInfo)
#define DEFINETUTORIAL_END_EXCEPTION(Id)	bool TutorialManager::ExceptionTutorial_##Id_End(const FTutorialInfo* TutorialInfo)	

#define REGISTERTUTORIAL_END_EXCEPTION(Id)														\
	{																							\
		FTutorialExceptionFunc ExceptionFunc;													\
		ExceptionFunc.BindRaw(this, &TutorialManager::ExceptionTutorial_##Id_End);				\
		FName TutorialKey(*GetTutorialKey(Id));													\
		TutorialExceptionEndExecs.Add(TutorialKey, ExceptionFunc);								\
	}																					

class BLADEII_API TutorialManager
{
public:
	TutorialManager();
	~TutorialManager();

	static TutorialManager&	GetInstance();
	void Init();

	void CachingTableInit();

	// Test
	void SetTestTutorial(int32 TestTutorialID);
	void SetTestTutorialAvailable(bool bAvailable) { bTestTutorialAvailable = bAvailable; }
	bool bTestTutorialAvailable;
	void RefreshTutorialTestUI();
	// Test

	static bool IsLockTutorial(const int32 TutorialID, const int32 TutorialStep);
	static bool IsCurrentTutorial(const int32 TutorialID, const int32 TutorialStep);
	static bool IsCurrentTutorial(const int32 TutorialID);
	static bool IsFinishTutorial(const int32 TutorialID);
	static void OpenLockContentPopup(const int32 TutorialID, const int32 TutorialStep);
	bool CheckOpenLockContentPopup(const int32 LockTutorialID);
	
	FORCEINLINE void SetTutorialAvailable(bool bAvailable) { bTutorialAvailable = bAvailable; }
	FORCEINLINE bool IsTutorialAvailable() const { return bTutorialAvailable; }

	bool IsValidTutorialSet();

	void SetTutorialMasterData(const TArray<b2network::B2mdTutorialInfoPtr>& TutorialMasterDatas);

	void SubscribeEvents();
	void UnsubscribeEvents();
	bool HasAnyTutorial();
	void ExternalProcessTutorial();

	const FTutorialInfo* GetTutorialInfo(const int32 TutorialID); 
	const FTutorialStep* GetTutorialStepInfo(const int32 TutorialID, const int32 TutorialStep);

	const FTutorialInfo* GetCurrentTutorialInfo();
	const FTutorialStep* GetCurrentStepInfo();

	const FTutorialStep* GetPrevStepInfo();
	ETutorialAction GetPrevStepAction();

	b2network::B2mdTutorialInfoPtr GetServerTutorialInfo(int32 TutorialID);

	int32 GetPrevTutorialID(int32 TutorialID);
	int32 GetNextTutorialID(int32 TutorialID);
	const FTutorialInfo* GetNextTutorialInfo();

	int32 GetFirstTutorialID();
	
	const FTutorialInfo* GetProcessableNextTutorial();

	void SkipCurrentTutorial();

	TArray<int32> GetLobbyDisplayTutorials();

	void OnTutorialBeginUIClose(int32 TutorialID);
	void OnTutorialRewardUIClose(int32 TutorialID);
	void OnClickHighlightRect();

	void OpenDialogContentsOpen();

	FText GetTutorialConditionText(int32 TutorialID);

	const TArray<FTutorialControlBotInfo>* GetTutorialControlBotInfos();

// Server Request
	void RequestGetTutorial();
	void RequestUpdateTutorial();
	void RequestRewardTutorial();
	void RequestSkipTutorial();

	bool bWaitForCompleteReward;

	ENeedToCachingResponse eNeedToCachingResponse;
	FB2ResponseGetTutorialProgressPtr CachedResponse;

	void RefreshContentLockUI();
	void OnAttendanceUIClose();

	void ForceBeginTutorial(int32 TutorialID, bool bFinish);

	// Skip New Tutorial
	FORCEINLINE void SetSkipNewTutorial(bool bIsSkip) { bIsSkipNewTutorial = bIsSkip; }
	FORCEINLINE bool IsSkipNewTutorial() const { return bIsSkipNewTutorial; }

private:

	void OnTutorialBegin(int32 TutorialID);
	void OnTutorialEnd(int32 TutorialID);
	void OnTutorialClear(int32 TutorialID);

	void OnTutorialStepBegin(const FTutorialStep* BeginStepInfo);
	void OnTutorialStepClear(const FTutorialStep* ClearStepInfo);

	void ProcessStep();
	void ProcessTutorialAction(const FTutorialStep* TutorialStepInfo);

	bool CheckAndBeginTutorial(int32 ServerTutorialID);
	void BeginTutorial(int32 TutorialID);

	void TutorialStep_OnClickedButton(UButton* ClickedButton);
	void TutorialStep_OnOpenUI(const FName& OpenedUIName);

	bool IsProcessableTutorial(int32 TutorialID);
	bool IsSatisfiedRequirements(int32 TutorialID);
	bool IsSatisfiedOpenStage(const FTutorialInfo* TutorialInfo);
	bool IsSatisfiedCharacterLevel(const FTutorialInfo* TutorialInfo);
	bool IsPassableStep(const FTutorialStep* TutorialStepInfo);

	bool IsWaitingForStepRequire(const FTutorialStep* TutorialStepInfo);

	const FTutorialStep* GetNextStep(const FTutorialStep* CurrentStepInfo);
	const FTutorialStep* GetPrevStep(const FTutorialStep* CurrentStepInfo);

	FORCEINLINE bool IsProcessingTutorial() { return CurrentTutorialID != INDEX_NONE && CurrentTutorialStep != INDEX_NONE; }

	void OnCahcedCheckAndProgressTutorial();

// UI
	//class UB2UITutorial* GetTutorialUI();

	void ActivateHighlightUI(const FTutorialStep* TutorialStep);
	void ClearHighlightUI(const FTutorialStep* TutorialStep);

	void SetVisibleSkipButton(bool bVisible);

	FORCEINLINE void SetTutorialStepState(ETutorialStepState StepState) { CurrentStepState = StepState; }
	FORCEINLINE ETutorialStepState GetTutorialStepState() const { return CurrentStepState; }

	void RegisterExceptionFunc();
	bool ProcessExceptionRequire(const FTutorialStep* StepInfo);
	bool ProcessExceptionAction(const FTutorialStep* StepInfo);
	bool ProcessExceptionEnd(const FTutorialInfo* TutorialInfo);

	void UpdateLobbyMainContentOpenSlot();

// Sound
	void ProcessNPCSoundAction(const FTutorialStep* StepInfo);

// Server

	void OnGetTutorialProgress(FB2ResponseGetTutorialProgressPtr TutorialProgress);
	void OnSetTutorialProgress(FB2ResponseSetTutorialProgressPtr TutorialProgress);
	void OnUpdateTutorialProgress(FB2ResponseUpdateTutorialProgressPtr TutorialProgress);
	void OnSkipTutorialProgress(FB2ResponseSkipTutorialProgressPtr TutorialProgress);
	void OnDrawTutorialItem(FB2ResponseDrawTutorialItemPtr TutorialItem);
	void OnPrepareTutorial(FB2ResponsePrepareTutorialPtr PrepareInfo);
	void OnGiveTutorialReward(FB2ResponseGiveTutorialRewardPtr TutorialReward);

// Hardcoding Condition

	void OnStageClear();

private:
	DECLARE_TUTORIAL_DOC_FIELD(int32, CurrentTutorialID);
	DECLARE_TUTORIAL_DOC_FIELD(int32, CurrentTutorialStep);
	DECLARE_TUTORIAL_DOC_FIELD(int32, LastClearTutorialID);		// Server Data

	bool bTutorialAvailable;

	bool bSubscribed;

	UB2TutorialInfo* TutorialInfoStore;

	ETutorialStepState	CurrentStepState;

	TMap<FName, FIntPoint>	ContentButtonLockTable;

	// Step Require Function Table
	TMap<FName, FStepExceptionFunc>	StepExceptionRequireExecs;
	// Step Action Functin Table
	TMap<FName, FStepExceptionFunc>	StepExceptionActionExecs;
	// Tutorial End Action Function Table
	TMap<FName, FTutorialExceptionFunc> TutorialExceptionEndExecs;

	TArray<b2network::B2mdTutorialInfoPtr> ServerTutorialMasterDatas;

	bool bIsSkipNewTutorial;

	TArray<std::shared_ptr<issue>> Issues;
	
public: // 咯扁何畔 窍靛内爹狼 康开
	void CloseLobbyMainCombineMenu();

	void InitHardCodingProperties();

	bool bHasContentOpenTutorial;
	bool HasContentOpenTutorial() { return bHasContentOpenTutorial; }

	// Exception Function Delcare
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_FirstStageClear, 1);

	DECLARESTEP_ACTION_EXCEPTION(TutorialID_FirstStageClear, 2);
	DECLARESTEP_REQUIRE_EXCEPTION(TutorialID_FirstStageClear, 8);
	bool bStage1Clear;
	bool IsFirstStageClear(int32 iTutorialID, EStageDifficulty eStageDifficulty);
	DECLARETUTORIAL_END_EXCEPTION(TutorialID_FirstStageClear);

	void OnClickSummonItem();

	EPCClass m_epcRequestPrepareClass;
	EPCClass GetTutorialRequestPrepareClass() { return m_epcRequestPrepareClass; }

	// 厘厚 饭骇诀
	UB2UnitedWidgetBase* CachedUIItemIcon;
	void SetCachedUIItemIcon(UB2UnitedWidgetBase* pItemIcon);
	
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 0);
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 3);
	uint32 LobbyTryUpdateItemOPMainMenuTicket;
	bool bIngredItemRegistered;
	void OnItemIngredRegistered();

	DECLARESTEP_REQUIRE_EXCEPTION(TutorialID_ItemLevelup, 4);
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 4);
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_ItemLevelup, 5);

	uint32 EnterItemLevelupModeClassTicket;
	void OnItemLevelupClicked();

	DECLARESTEP_ACTION_EXCEPTION(TutorialID_CounterDungeon, 3);
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_CounterDungeon, 4);
	//UB2UnitedWidgetBase* CachedDungeonSlot;

	//痢飞傈
#define TutorialStep_Occupy_InitAndCenterCamera 11
#define TutorialStep_Occupy_BotCamera 14
#define TutorialStep_Occupy_TopCamera 16
#define TutorialStep_Occupy_IngameCamera 18
#define TutorialStep_Occupy_SetIngame 20

	DECLARESTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_InitAndCenterCamera);
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_BotCamera);
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_TopCamera);
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_IngameCamera);
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_Occupy, TutorialStep_Occupy_SetIngame);

	//厘厚 碍拳
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 0);
	bool bReadyForEnhance;

	DECLARESTEP_REQUIRE_EXCEPTION(TutorialID_ItemEnhance, 1);
	bool IsEquippedWeapon;
	FB2Item CachedEnhanceWeapon;
	bool HasEquippedWeapon(EPCClass PCClass, FB2Item& OutWeaponItem);
	bool HasInventoryWeapon(EPCClass PCClass, FB2Item& OutWeaponItem);
	
	void OnOpenItemDetailPopup();

	DECLARESTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 5);

	DECLARESTEP_REQUIRE_EXCEPTION(TutorialID_ItemEnhance, 6);
	bool bEnhanceIngredItemRegistered;
	void OnEnhanceIngredRegistered(const FB2Item& IngredItem);
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 6);

	void OnItemEnhancedClicked();

	DECLARESTEP_REQUIRE_EXCEPTION(TutorialID_ItemEnhance, 7);
	bool bItemEnhancedSucessfully;
	uint32 DeliveryEnhanceItemTicket;
	void OnEnhancedItem(const FB2EnhanceItem& EnhancedItem);
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_ItemEnhance, 7);

	void OnEnhanceDetailPopupClose();

	//ChapterWidget
	//UB2UnitedWidgetBase* CachedUIPStage1;
	void ChapterWidgetActiveHiglight(bool bCheck);
	void ActClearAnimStart();
	void ActClearAnimEnd();

	//ExpertDungon
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_ExpertDungon, 3);
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_ExpertDungon, 4);
	bool ExceptionTutorial_TutorialID_ExpertDungon_End(const FTutorialInfo* TutorialInfo);

	//Seal
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_Seal, 0);

	//DECLARESTEP_ACTION_EXCEPTION(TutorialID_NightmareDungeon, 2);
	//DECLARESTEP_ACTION_EXCEPTION(TutorialID_NightmareDungeon, 3);
//	bool ExceptionTutorial_TutorialID_NightmareDungeon_End(const FTutorialInfo* TutorialInfo);

	//ather new tutorial
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_Ether, 0);
	DECLARESTEP_ACTION_EXCEPTION(TutorialID_Ether, 13);
	void AtherEquipAnimEnd();

	void PrePareTutorialID_ItemLevelup();
	void PrePareTutorialID_ItemEnhance();
	void PrePareTutorialID_Ether();
	void PrePareTutorialID_Seal();


	bool bHighlightRenderCheck;
	bool HighlightRenderCheck(int32 TutorialID);

	bool LockWidgetRedDotVisibleCheck(int32 LockTutorialID);

	void GotoNextStep();
};

FText GetTutorialTitle(int32 TutorialID);
FText GetTutorialDesc(int32 TutorialID);

FText GetTutorialStepDialog(int32 TutorialID, int32 TutorialStep);
UTexture2D* GetTutorialIconTexture(int32 TutorialID);
UMaterialInterface* GetTutorialSpeakerMaterial(int32 TutorialID);

FString GetTutorialStepKey(int32 TutorialID, int32 TutorialStep);
FString GetTutorialKey(int32 TutorialID);

//FString GetWidgetName(UB2UnitedWidgetBase* TargetWidget);
//FName GetContentLockKey(UB2UnitedWidgetBase* TargetWidget, UButton* LockButton);
