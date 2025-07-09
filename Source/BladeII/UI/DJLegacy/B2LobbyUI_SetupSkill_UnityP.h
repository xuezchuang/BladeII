#pragma once

#include "B2LobbyUserWidget.h"
#include "B2SkillInfo.h"
#include "B2UIWidgetBase.h"
#include "B2LobbyUI_SetupSkill_UnityP.generated.h"

#define MAX_UNITYSTEP_COUNT 8

UENUM(BlueprintType)
enum class EUnityLoopType : uint8
{
	Default = 0,
	AbleGet,
	AlreadyGet,
};

enum class EUnityBottomInfoType
{
	Default = 0,
	InMission,
	ClearMission,
};

/**
 * Part of SetupSkillMain, but used only when unity-skill tab is pressed.
 * It is to be loaded dynamically on-demand, to reduce skill main page loading time.
 */
UCLASS()
class BLADEII_API UB2LobbyUI_SetupSkill_UnityP : public UB2LobbyUserWidget
{
	GENERATED_UCLASS_BODY()

protected:

	UPROPERTY(Transient)
	class UB2LobbyUI_SetupSkillMain* OwnerSkillMainPage;
	
	// HeroMgmtCommon part from OwnerSkillMainPage..
	UPROPERTY(Transient)
	class UB2LobbyUI_HeroMgmtCommon* HeroMgmtBaseNRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII UnitySkillUI")
	TSubclassOf<class UB2UIUnitySkillAcquireInfo> UnitySkillAcquireItemClass;

public:

	UFUNCTION()
	void OnClickedUnityButton();
	UFUNCTION()
	void OnClickedUnityReward();
	UFUNCTION()
	void OnClickedUnityDetail();

	UFUNCTION()
	void OnClickedUnityAwake();
	UFUNCTION()
	void OnClickedUnityAwakeNotice();

	UFUNCTION()
	bool RedDotCondition_UnitySkillChar1();
	UFUNCTION()
	bool RedDotCondition_UnitySkillChar2();
	UFUNCTION()
	bool RedDotCondition_UnitySkillChar3();
	UFUNCTION()
	bool RedDotCondition_UnitySkillAwakenAble();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnPlayUnitySkillLoopAnim_IMP(EUnityLoopType nType);
	
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnCallbackPlayMailRewardPopup_BP();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnPlayUnityWakeReward_IMP();
	
	UFUNCTION(BlueprintCallable, Category = "BladeII")
	void OnCallbackUnityWakeReward_BP();

public:
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	void InitFromOwnerSkillMain(class UB2LobbyUI_SetupSkillMain* InOwnerPage, class UB2LobbyUI_HeroMgmtCommon* InHeroMgmtCommon);
	virtual void DestroySelf() override;
	virtual void BeginDestroy() override;
	void BindDelegates();
	void UpdateStaticText();
	void SubscribeEvents();
	void UnsubscribeEvents();
		

	/** Update with optional new PCClass info. You may designate EPC_End if don't want change it and just update the widgets. */
	void UpdatePanelMain(EPCClass InPCClassIfChanged = EPCClass::EPC_End);

	void OnReceiveUnitySkillReward(FB2ResponseUnitySkillRewardPtr RewardPtr);
	void OnReceiveUnitySkillAwake(FB2ResponseAwakenUnitySkillPtr RewardPtr);

	void DoMarkReddotAll();
protected:
	virtual void CacheAssets() override;
	virtual void RegisterUIMarkForRedDot() final;

private:
	void SetUnitySkillPanel();
	void SetUnitySkillPortraitInfo();
	void SetUnityBottomInfoPanel(EUnityBottomInfoType infotype, int32 NextStep = 0);
	void SetSkillOptionText();
	void OnClickedUnityPortraitButton(EPCClass seletedclass);
	void UnityPortraiLoopAniForAllClear();
	void CheckAnimationUnityLoop(EUnityLoopType nType);
	int32 GetUnitySkillID(EPCClass InPCClass);
	void ShowUnityStatPopup();
	void OnDelayBlockPage();

	void SetIsEther();
	void SetUnityAwakeStateButton();
	void OnAllClearUnityAwakenMission();
	void PageBlock(bool bIsBlock);
private:
	const int32 MAX_UNITYCHAR_COUNT = 3;

	EPCClass MainPCClass;
	EPCClass SubPCClass;
	int32 CurrentStep;

	TWeakObjectPtr<UTextBlock> TB_UnitySkillName;
	TWeakObjectPtr<UTextBlock> STB_UnityTitle;
	TWeakObjectPtr<UTextBlock> STB_UnityComplete;
	TWeakObjectPtr<UTextBlock> STB_UnityRewardButton;
	TWeakObjectPtr<UTextBlock> STB_IsEther;
	TWeakObjectPtr<UTextBlock> STB_DetailButton;
	TWeakObjectPtr<UTextBlock> STB_UnityAwake;
	TWeakObjectPtr<UTextBlock> STB_UnityAwakeNotice;
	
	TWeakObjectPtr<UTextBlock> TB_UnityChar1;
	TWeakObjectPtr<UTextBlock> TB_UnityName;
	TWeakObjectPtr<UTextBlock> TB_UnityDesc;
	TWeakObjectPtr<UB2RichTextBlock> TB_SkillOption;

	TWeakObjectPtr<UB2Button> BTN_UnitySkill;
	TWeakObjectPtr<UB2Button> BTN_UnityReward;
	TWeakObjectPtr<UB2Button> BTN_UnityDetail;

	TWeakObjectPtr<UB2Button> BTN_UnityAwake;
	TWeakObjectPtr<UB2Button> BTN_UnityAwakeNotice;

	TWeakObjectPtr<UB2Image> IMG_UnitySkillIcon;
	TWeakObjectPtr<UB2Image> IMG_UnitySelect;
	
	TWeakObjectPtr<UWidgetSwitcher> WS_UnityInfo;

	TWeakObjectPtr<UPanelWidget> P_IsEther;
	
	TWeakObjectPtr<UVerticalBox> List_UinityAccuire;

	TArray<TWeakObjectPtr<class UB2UIUnityCharacterInfo>> UnityCharInfoArray;
	
	uint32 DeliveryReceiveUnitySkillRewardTicket;
	uint32 DeliveryAwakenUnitySkillTicket;
	uint32 AllClearUnityAwakenMissionTicket;
	uint32 DeliveryUnityAwakeFailTicket;

	FTimerHandle DelayBlockPageCallbackTH;

	TWeakObjectPtr<class UB2LobbyUI_UnitySkillWakePopup> Lobby_SetupSkill_UnityP_AwakePopup;
};

UCLASS()
class BLADEII_API UB2UIUnitySkillAcquireInfo : public UB2UIWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;

	void SetAcquireInfoText(const FText infotext);
	void UpdateMissionSuccessState(bool bsuccess);

	TWeakObjectPtr<UWidgetSwitcher> WS_Line;
	TWeakObjectPtr<UTextBlock> TB_OFF_Info;
	TWeakObjectPtr<UTextBlock> TB_On_Info;
	TWeakObjectPtr<UTextBlock> STB_Success;
};

USTRUCT()
struct FB2UnitySkillPortraitImage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EPCClass CharClass;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* Portrait;

	static UMaterialInterface* GetCharImage(const TArray<FB2UnitySkillPortraitImage>& ImageInfos, EPCClass PCClass)
	{
		for (auto& ImageInfo : ImageInfos)
		{
			if (ImageInfo.CharClass == PCClass)
				return ImageInfo.Portrait;
		}

		return nullptr;
	}
};

DECLARE_DELEGATE_OneParam(FUnitySelectCharDelegate, EPCClass);

UCLASS()
class BLADEII_API UB2UIUnityCharacterInfo : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetCharacterPortrait(EPCClass pcclass);
	void SetMissionClearCount(int32 clearcount);
	void OnChangeSelected(bool selected);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnPlayAnim_MissionCount_IMP(int32 nCount);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void OnPlayAnim_Portrait_Loop_IMP(bool bLoop);

	UFUNCTION()
	void OnClickedSelectButton();

	UPROPERTY(EditAnywhere)
	TArray<FB2UnitySkillPortraitImage> PortraitInfo;

	EPCClass PCClass;
private:
	TWeakObjectPtr<UImage> IMG_Portrait;
	TWeakObjectPtr<UImage> IMG_Select;
	TArray <TWeakObjectPtr<UImage>> SlotImages;

	TWeakObjectPtr<UB2Button> BTN_Select;

public:
	FUnitySelectCharDelegate ButtonDelegate;
};