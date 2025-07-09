#pragma once

#include "B2LobbyUserWidget.h"
#include "B2SkillInfo.h"
#include "B2UIBackWidget.h"
#include "B2UIWidgetBase.h"
#include "B2LobbyUI_SetupSkill_UnityP.h"
#include "B2LobbyUI_SetupSkillMain.generated.h"

/**
* Main inventory page user widget. Many other sub pop-ups can pop-up based on this.
*/
UCLASS()
class BLADEII_API UB2LobbyUI_SetupSkillMain : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_UCLASS_BODY()

protected:
	/** Contains common part of several hero management UI pages. */
	TWeakObjectPtr<class UB2LobbyUI_HeroMgmtCommon> UIP_HeroMgmtBase;
	
	/** For on-demand UnitySkill part creation. */
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<class UB2LobbyUI_SetupSkill_UnityP> UnityPartClass;
	/** It is a little specail. As UnityPartClass is somewhat heavy, we try to async load UnityPartClass here. and cache it. */
	UPROPERTY(Transient)
	TSubclassOf<class UB2LobbyUI_SetupSkill_UnityP> LoadedUnityPartClassCache;
	/** Widget set for unity skill tab. */
	UPROPERTY(Transient)
	class UB2LobbyUI_SetupSkill_UnityP* CreatedUnityP;

public:
	UFUNCTION()
	void OnClickedActiveSkillListBtn();
	UFUNCTION()
	void OnClickedPassiveSkillListBtn();
	UFUNCTION()
	void OnClickedUnitySkillBtn();
	UFUNCTION()
	void OnClickedBuySkillPointBtn();
	UFUNCTION()
	void OnClickedInitSkillPointBtn();
	UFUNCTION()
	void OnClickedSkillChangeBtn();

	UFUNCTION()
	bool RedDotCondition_ActiveSkillTab();
	UFUNCTION()
	bool RedDotCondition_PassiveSkillTab();
	UFUNCTION()
	bool RedDotCondition_UnitySkillTab();

	UFUNCTION(BlueprintImplementableEvent, Category = "Audio")
	void OnClickedMsgResetSkillPoint_BP();
	
	void SetVisibleTouchBlockImage(bool bInVisible);

public:
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void DestroySelf() override;
	virtual void BeginDestroy() override;
	virtual void CloseWidgetDelegate() override;

	void BindDelegates();
	void UpdateStaticText();

	void OnHeroMgmtPCChanged(EPCClass InSelectedPCClass);

	void OnChangedCharacterSkillPoint();
	void OnChangedSkillLevel(int32 SkillId);

	void ShowTooltipSkill(int32 SkillId, int32 SkillLv, int32 SkillMaxLv);
	void DoMarkRedDotAll_Unity();

protected:
	virtual void CacheAssets() override;
	virtual void RegisterUIMarkForRedDot() final;

private:
	void CreateSkillList();
	void CreateSkillWidget(const int32 UserSkillindex, const bool bEmptyWidget, const FB2SkillInfo* Skill = nullptr, const struct FSingleSkillInfo* SkillSingleInfo = nullptr);
	void SetPCSkillList(const class UB2SkillInfo* AllSkillInfo, TArray<FB2SkillInfo>& Skills, bool bIncludeAllSkills = false);

	bool HasSkillLevelUp();

	void UnselectSkillInfo(int32 SkillId);
	bool IsBuyableSkillPoint();

	void UpdateMenuButtonEnable();

	void UpdateRegistSkillList();

	void ChangePreset(const EPCClass InPCClass, const EItemPresetSlotType InSlotID);

	void OnClickedLevelupSkillPoint(int32 SkillID);

	void OnDelaySkillChange();
	void OnDelayBuySkillPoint();
		
	void SetUnityTabPartOpen(bool bInOpen); // Open or Close, dynamically create if not created yet.
	void ConditionalCreateUnityTabPart();
	void DestroyUnityTabPart();

	void RequestAsycLoadUnityPartClass();
	void OnLoadedUnitySkillAsset(const FString& CompleteRequest);

	void ChangeSlotSkillEffect();

	const FString& GetUnityPartRequestName();
private:
	
	enum class EViewSkillList : int8
	{
		None,
		ActiveSkill,
		PassiveSkill,
		UnitySkill
	};

	const int32 MAX_SKILLINFO_COUNT = 6;
	
	EViewSkillList CurrentView;
	EPCClass CurrentPCClass;
	
	EItemPresetSlotType CurrentPresetID;
	int32 CurrentSkillPoint;
	
	FTimerHandle DelayCallbackTH;

	TWeakObjectPtr<UPanelWidget> P_SkillPointSet;
	TWeakObjectPtr<UPanelWidget> P_SkillListHorizontalBoxTop;
	TWeakObjectPtr<UPanelWidget> P_SkillListHorizontalBoxBottom;
	
	// Where the UnitySkill part will be dynamically created..
	TWeakObjectPtr<UCanvasPanel> X_CP_UnitySkill;

	TWeakObjectPtr<class UB2UICommonTab>	TAB_ActiveSkill;
	TWeakObjectPtr<class UB2UICommonTab>	TAB_PassiveSkill;
	TWeakObjectPtr<class UB2UICommonTab>	TAB_UnitySkill;

	TWeakObjectPtr<UTextBlock> TB_ActiveSkillList;
	TWeakObjectPtr<UTextBlock> TB_PassiveSkillList;
	TWeakObjectPtr<UTextBlock> TB_SkillChange;
	TWeakObjectPtr<UTextBlock> TB_BuySkillPoint;
	TWeakObjectPtr<UTextBlock> TB_LeftSkillPoint;
	TWeakObjectPtr<UTextBlock> TB_LeftSkillPointAmount;
	TWeakObjectPtr<UTextBlock> TB_SkillReset;

	TWeakObjectPtr<UB2Button> BTN_BuySkillPoint;
	TWeakObjectPtr<UB2Button> BTN_SkillChange;
	TWeakObjectPtr<UB2Button> BTN_SkillReset;

	TWeakObjectPtr<UB2Image> IMG_TouchBlock;

	TWeakObjectPtr<UTextBlock>	TB_ChangeSkill;

	TArray<TWeakObjectPtr<class UB2UIPlayerSkillInfo>> SkillInfoArray;
	TMap<EPassiveType, int32>	SkillInfoIndexFromPassive;

	uint32 ChangeSkillPointTicket;
	uint32 ChangeSkillLevelTicket;
	uint32 CloseSkillTooltipTicket;
	uint32 RegistSkillTicket;
	uint32 UpdateSkillSlotTicket;
	uint32 LobbySetHeroMgmtModePCSelectionTicket;
	uint32 DeliveryLevelUpSkillTicket;
	uint32 DeliveryLevelUpSkillErrorTicket;
	bool IsLevelUP;

	uint32 LobbyChangeSlotEffectTicket;
};

