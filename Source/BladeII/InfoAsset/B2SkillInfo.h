// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "UObject/Object.h"
#include "UMG.h"
#include "BladeIIPlayer.h"
#include "B2AssetLoader.h"
#include "../BladeII/BladeIICharacter.h"
#include "B2SkillInfo.generated.h"

UENUM()
enum class ESkillType : uint8
{
	ESkillType_Attack,
	ESkillType_Defense,
	ESkillType_Survive,
	ESkillType_Buff,
	ESkillType_Counter,
	ESkillType_Professional,
	ESkillType_Tag
};

UENUM()
enum class EPassiveType : uint8
{
	EPassiveType_None,
	EPassiveType_Attack,
	EPassiveType_Defense,
	EPassiveType_Counter,
	EPassiveType_Tag
};

USTRUCT(BlueprintType)
struct FDisplaySkillOption
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	int32 SkillLevel;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	ESkillOption SkillOption;
};

/**
 * Actual information element that composes B2SkillInfo
 */
USTRUCT(BlueprintType)
struct FSingleSkillInfo
{
	GENERATED_USTRUCT_BODY()

	FSingleSkillInfo();

	/** The final skill ID applied in AnimBP. See also EAttackState */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	int32 SkillID;

	//[@AKI, 170706] <B2CLT-1705> 자동전투 시, 영웅이 사용하는 스킬 사거리 개선으로 추가
	//기획쪽에서 변경시 수정 잘 하겠다고 함.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	float SkillShootingRangeForAI;

	/** Required time to use skill again after uses this skill once. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	float CoolTime;
	
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	//EAttackState AttackAnimState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	ESkillType SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	EPassiveType PassiveType;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	//EWeaponSkillType WeaponSkillType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	int32 EnableWeaponSkillPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	TArray<FDisplaySkillOption>	SpecialDisplaySkillOption;

	/** Button icon brush materials to construct FButtonStyle. 
	 * Margin information is defined at elsewhere for shared use. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	TSoftObjectPtr<class UMaterialInterface> ButtonIcon_Normal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	TSoftObjectPtr<class UMaterialInterface> ButtonIcon_Hovered;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	TSoftObjectPtr<class UMaterialInterface> ButtonIcon_Pressed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillInfo")
	TSoftObjectPtr<class UMaterialInterface> ButtonIcon_Disabled;

#if WITH_EDITOR
	void EditorOnPostLoad();
#endif
	
	EPCClass UserClass;
};

/**
 * Skill Information for player character.. and possibly for NPCs too? (such as boss mob..)
 * Intended to make an object of this class for each character class that uses skill.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2SkillInfo : public UObject
{
	GENERATED_BODY()
	
private:
	TMap<int32, FSingleSkillInfo*> SkillInfoMap;

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSingleSkillInfo> SkillInfoArray_Gladiator;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSingleSkillInfo> SkillInfoArray_Assassin;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSingleSkillInfo> SkillInfoArray_Wizard;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSingleSkillInfo> SkillInfoArray_Fighter;

	/**
	 * Commonly used ButtonStyle settings.. except icon material setting.
	 * Common ButtonStyle settings are separated due to two different kind of skills
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FButtonStyle CommonButtonStyle_Regular;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FButtonStyle CommonButtonStyle_WeaponSkill;
	
public: 
	UB2SkillInfo(const FObjectInitializer& ObjectInitializer);
	
	virtual void PostLoad() override;

	const FSingleSkillInfo* GetSingleInfoOfID(int32 SkillInfoID) const;
	const FSingleSkillInfo* GetTagSkillInfo(EPCClass PCClass) const;
	const FSingleSkillInfo* GetCounterSkillInfo(EPCClass PCClass) const;
	const TMap<int32, FSingleSkillInfo*>& GetSkillInfoMap() const;
	TArray<ESkillOption>	GetSpecialDisplaySkillOption(int32 SkillInfoID, int32 SkillLevel);
	int32					GetNextSpecialDisplaySkillLevel(int32 SkillInfoID, int32 SkillLevel);

	float GetOriginalSkillShootingRangeForAI(int32 SkillInfoID) const;

	/** Original cooltime, unscaled by any other modification.. */
	float GetOriginalCoolTime(int32 SkillInfoID) const;

	//static int32 GetEnablePlayerLevel(int32 SkillInfoID);

	FButtonStyle GetButtonIconStyle(int32 SkillInfoID);

	EAttackState GetAttackState(int32 SkillInfoID) const;

	bool IsTagSkill(int32 SkillInfoID) const;

	float GetEnableWeaponSkillPoint(int32 SkillInfoID) const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};


////////////////////////////////////////////////////////////////////////////////
// SkillAnimInfo below..
//

/** Struct to contain loaded TAsset references of a single FSkillAnimPerPCInfoSet (SkillAnimInfo) entry to prevent it get GCd. */
USTRUCT()
struct FSingleSkillAnimInfoLoadedRefHolder
{
	GENERATED_USTRUCT_BODY()

	FSingleSkillAnimInfoLoadedRefHolder()
	{
		LoadedInfoPtr = nullptr;
	}

	UPROPERTY(Transient)
	class UAnimSequenceBase* LoadedInfoPtr;
};

/**
 * Intermediate data struct for SkillAnimInfo, to hold data per PCClass
 */
USTRUCT(BlueprintType)
struct FSkillAnimPerPCInfoSet
{
	GENERATED_USTRUCT_BODY()

	FSkillAnimPerPCInfoSet()
	{
		MyPCClass = EPCClass::EPC_End;
	}

	EPCClass MyPCClass; // Simply comes from my containing InfoData map key.

	/** All skill animation data for this PCClass */
	UPROPERTY(EditAnywhere)
	TMap<ESkillAnimType, TSoftObjectPtr<class UAnimSequenceBase>> SkillAnims;

	/** FCombinedPCSkillAnimID is like unique identifier of Skillanims, but here used only for Loaded ptr map key.. just because it doesn't look so fine as Info data key. */
	static FCombinedPCSkillAnimID GetLoadedPtrMapKey(EPCClass InPCClass, ESkillAnimType InSkillAnimType);
};

/**
 * Skill animation resources, bound by PCClass and SkillAnimType.
 * It is specially managed unlike other combat animations directly placed at AnimBP, because skill animations are in big size and can be played in relatively simple manner.
 * However, at sometime, all combat animations can be managed like this if AnimBP is totally replaced by other anim playing system.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2SkillAnimInfo : public UObject, public IB2AsyncInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<EPCClass, FSkillAnimPerPCInfoSet> InfoData;

	/** Loaded asset holder, to prevent GC. It is required as skill animation is too big to dynamically load at run-time.
	 * Skill anim data here will be loaded at map loading time, only for selected data. */
	UPROPERTY(Transient)
	TMap<uint32, UObject*>	LoadedPtrMap;

public:
	UB2SkillAnimInfo(const FObjectInitializer& ObjectInitializer);

	virtual void PostLoad() override;

	void SyncPreloadAssets(const TArray<FCombinedPCSkillAnimID>& InLoadAnimIDs);

	/** Main skill AnimSequence access interface. */
	class UAnimSequenceBase* GetSingleSkillAnim(EPCClass InPCClass, ESkillAnimType InSkillAnimType);
	class UAnimSequenceBase* GetSingleSkillAnim(const FCombinedPCSkillAnimID& InCombinedID);

	class UAnimSequenceBase* FindSingleSkillAnim(const FCombinedPCSkillAnimID& InCombinedID);

	TSoftObjectPtr<UAnimSequenceBase> GetAnimationAsset(const FCombinedPCSkillAnimID& CombineID);

	void UnloadAll();

	/** For ManagedSkillAnimUnload */

	void UnloadAnimAsset(const FCombinedPCSkillAnimID& CombineID);

	void UnloadExcept(const TArray<FCombinedPCSkillAnimID>& InExceptIDs);
	void UnloadAllOrExceptSelected(UObject* WorldContextObject);


	TArray<FCombinedPCSkillAnimID> GetCurrentPlayerSkillAnimIDs(UObject* WorldContextObject);
	TArray<FB2AsyncRequestInfo> GetAsyncRequestAssetList(const TArray<FCombinedPCSkillAnimID>& CombineIDs);

	/* Async Interface */
	virtual void OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FB2AsyncRequestInfo>& CompleteRequestList) override;
	virtual void OnAsyncLoadComplete_EachObject(const uint32 AssetKey, UObject* LoadedAssetObject) override;

	

private:
	
	void EnsureDataIntegrity();
	//void ConditionalSetLoadedSkillAnimsRootSet();

public:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP /* Doesn't look like "TEMP" anymore.. */
	void CleanupOnPreSave();
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif

	/**
	* If true, loaded SkillAnimInfo data won't be simply unloaded at level transition time and be cached for reduce next level loading.
	* You should always use ShouldUnloadAllSkillAnimOnLevelTransition instead of direct access of this variable.
	*/
	static bool bManagedSkillAnimUnload;
	static bool ShouldUnloadAllSkillAnimOnLevelTransition();

#if !UE_BUILD_SHIPPING
	// Setting bManagedSkillAnimUnload at runtime is only for Development!
	static void DevSetManagedSkillAnimUnload(bool bSet) { bManagedSkillAnimUnload = bSet; }
#endif
};