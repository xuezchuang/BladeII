// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "CommonStruct.h"
#include "EventSubsystem.h"
#include "B2UIMsgPopupGuestWarning.h"
#include "Components/Image.h"
#include "../../BladeII.h"
#include "B2UILobbyMain.generated.h"

struct FContentOpenSlot
{
public:
	TWeakObjectPtr<UOverlay>	O_ContentOpen;
	TWeakObjectPtr<UImage>		IMG_ContentOpen;
	TWeakObjectPtr<UTextBlock>	TB_ContentOpenTitle;
	TWeakObjectPtr<UTextBlock>	TB_ContentOpenDesc;
	TWeakObjectPtr<UB2Button>	BTN_ContentOpen;
	TWeakObjectPtr<UImage>		IMG_EffetContentOpen;

	int32						m_iTutorialID;

	void SetContentOpenSlot(int32 TutorialID);
	void SetVisibile(bool bVisible);
};


/*
* Lobby Main UI
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UILobbyMain : public UB2UIWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

protected:

	/** Whole UI contents in the widget blueprint will be hidden after this time without user interaction. */
	UPROPERTY(EditAnywhere)
	float AutoHideInterval;
public:
	FORCEINLINE float GetAutoHideInterval() const {
		return AutoHideInterval;
	}
	FORCEINLINE void SetReceiveGetChallengeMode(bool bIsReveive) {
		bReceiveGetChallengeMode = bIsReveive;
	}
	FORCEINLINE void SetReceiveGetDuelMode(bool bIsReveive) {
		bReceiveGetDuelModeInfo = bIsReveive;
	}

public:
	UB2UILobbyMain(const FObjectInitializer& ObjectInitializer);

	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	virtual void OnOpen(bool RightNow = false);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//virtual void OnClose(bool RightNow = false);
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII LobbyMain")
	void HideAllMenus(bool HideAllMenus);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII LobbyMain")
	void ShowCombineMenu_IMP(bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "BladeII LobbyMain")
	void OnClickBtnContentOpenSlots(int32 iArrayIndex);


protected:
	/* Events that occur when finished completely open. */
	virtual void OnOpenComplete() override;
	/* Events that occur when finished completely close. */
	virtual void OnCloseComplete() override;

	virtual void RegisterUIMarkForRedDot() final/* or override */;
private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	bool bSubscribed;
	TArray<std::shared_ptr<issue>> Issues;

	void SubscribeEvent();
	void UnSubscribeEvent();

	UPROPERTY(EditAnywhere)
	int32 RequireSurveyStageID;

	//===================================================Click
	UFUNCTION()
	void OnClickBtnDungeon();
	UFUNCTION()
	void OnClickBtnPVP();
	UFUNCTION()
	void OnClickBtnAdventure();
	UFUNCTION()
	void OnClickBtnInventory();
	UFUNCTION()
	void OnClickBtnPackage();
	UFUNCTION()
	void OnClickBtnEquipShop();
	UFUNCTION()
	void OnClickBtnGoodsShop();
	UFUNCTION()
	void OnClickBtnMagicShop();
	UFUNCTION()
	void OnClickBtnItemForge();
	UFUNCTION()
	void OnClickBtnHiddenableMenu();
	UFUNCTION()
	void OnClickBtnAttendance();
	/*UFUNCTION()
		void OnClickBtnChangeFocusCharacter();*/
	UFUNCTION()
	void OnClickBtnSlideMove();
	UFUNCTION()
	void OnClickBtnCombineMenu();
	UFUNCTION()
	void OnClickBtnUIOnOff();
	UFUNCTION()
	void OnClickBtnSurvey();
	UFUNCTION()
	void OnClickBtnDonationEvent();

	//====================================================Doc Delegate
	//detect Hide LobbyUIs
	UFUNCTION()
	void OnChangedHideAllLobbyMainUIs(class UB2UIDocBase* Sender, bool HideAllLobbyMainUIs, bool PrevHideAllLobbyMainUIs);

	UFUNCTION()
	void OnChangedLobbyFocusClass(class UB2UIDocBase* Sender, EPCClass CurrentFocusClass, EPCClass PrevFocusClass);

	UFUNCTION()
	bool RedDotCondition_Adventure();
	UFUNCTION()
	bool RedDotCondition_Dungeon();
	UFUNCTION()
	bool RedDotCondition_PVP();
	UFUNCTION()
	bool RedDotCondition_Inventory();
	UFUNCTION()
	bool RedDotCondition_Package();
	UFUNCTION()
	bool RedDotCondition_EquipShop();
	UFUNCTION()
	bool RedDotCondition_GoodsShop();
	UFUNCTION()
	bool RedDotCondition_MagicShop();
	// SlideMoving
	UFUNCTION()
	bool RedDotCondition_SlideMove();
	UFUNCTION()
	bool RedDotCondition_Slide_Event();

	UFUNCTION()
	bool RedDotCondition_CombineMenu();

	UFUNCTION()
	bool RedDotCondition_DonationEvent();

	//===========================
	void CheckSurvey();
	void ShowStoreEvent();

public:
	void ChangeChaperScene(int32 ChapterNum);

	void InitQuestSlot(TWeakObjectPtr<class UB2UIQuestSlot>& QuestSlot, b2network::B2QuestInfoPtr QuestData);
	void UpdateQuestDisplay(const TArray<b2network::B2QuestInfoPtr>& QuestList);
	void UpdateSingleQuestDisplay(int64 QuestUniqueID);
	void ShowQuestNotifyDialog(const TArray<b2network::B2QuestInfoPtr>& QuestList);

	void OpenUnappliedItemOptionChangeUI();

	void OnReceiveCheckRedDot(FB2MessageInfoResponseCheckRedDotPtr LobbyRedDotInfo);
	void OnReceiveTutorialProgress(FB2ResponseUpdateTutorialProgressPtr TutorialProgress);
	void OnReceiveKakaoMessageProcessing(FB2ResponseKakaoMessageProcessingPtr KakaoMessageProcessing);
	void OnReceiveGetFeaturedMission(FB2ReciveGetFeaturedMission GetFeaturedMission);
	void OnReceiveLobbyMissionAlarm();
	void ResponseDailyAttendance(const FB2ReceiveDailyAttendance& AttendaceInfo);
	void ResponseGetPeriodPackageState(FB2ResponseGetPeriodPackageStatePtr ReciveGetPeriodPackageState);

	void UpdateDailyNotice();
	void HideLobbyContentOpenSlots();
	void UpdateLobbyContentOpenSlots();

	void OnOpenComplete_RequestToServer();
	void OnOpenComplete_Once();

	bool HasAttendanceNotice();
	bool GetCombineMenuIsVisible();
	bool CheckOtherPopupVisible();

	void DoMarkCombineRedDot(bool CheckVisible = false);
	//void CheckConnectRewardRedDot(const AB2LobbyGameMode* LobbyGameMode);

	void UpdateCombineMenuVisible(bool bVisible);
	class UB2UICombineMenu* GetCombineMenuUI();

	void CloseCombineMenu();
	void ForcedChangeFocusCharacter();
	void ExecutePendingDungeonSceneInner();
	void ExecutePendingPVPSceneInner();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeRedDot_Adventure(bool bShowRedDot);
	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeRedDot_Dungeon(bool bShowRedDot);
	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeRedDot_PVP(bool bShowRedDot);
	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeRedDot_Inventory(bool bShowRedDot);
	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeRedDot_Package(bool bShowRedDot);
	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeRedDot_EquipShop(bool bShowRedDot);
	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeRedDot_GoodsShop(bool bShowRedDot);
	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeRedDot_MagicShop(bool bShowRedDot);
	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeRedDot_CombineMenu(bool bShowRedDot);

	//////////////////////////////////////////////////
	// Deferred Challenge/Adventure button click handling (for widget anim)
protected:
	/** Time for animation play on clicking Challenge/Adventure button */
	UPROPERTY(EditAnywhere)
	float EnterGameSelectionDelayTime;

	uint32 bPendingEnterGameSelection : 1; // Being true once clicked Challenge/Adventure button to prevent counting time again
	FTimerHandle EnterGameSelectionTH;
	uint32 bWaitingForPreFetchedActInfo : 1;
	void ExecutePendingDungeonScene();
	void ExecutePendingPVPScene();
	void ExecutePendingAdventureScene();
	void CheckAnGoToChapterView();
	void PreFetchAdventureSceneInfo();
	//////////////////////////////////////////////////
	UPROPERTY(EditAnywhere)
	float EnterCharacterSelectionDelayTime;
	FTimerHandle EnterCharacterSelectionTH;
	void ExecutePendingSelectCharacterScene();
	//////////////////////////////////////////////////
	TWeakObjectPtr<class UB2UIQuestSlot> GetSuitableQuestSlot(int32 QuestSlotIndex);

	TWeakObjectPtr<UPanelWidget>		P_TotalCanvas;
	TWeakObjectPtr<UEditableTextBox>	ETB_StageNum;

	TWeakObjectPtr<UButton>				BTN_Adventure;
	TWeakObjectPtr<UImage>				FX_Adventure;

	TWeakObjectPtr<UButton>				BTN_Dungeon;
	TWeakObjectPtr<UImage>				FX_Dungeon;

	TWeakObjectPtr<UButton>				BTN_PVP;
	TWeakObjectPtr<UImage>				FX_PVP;

	TWeakObjectPtr<UButton>				BTN_Inventory;
	TWeakObjectPtr<UButton>				BTN_Package;
	TWeakObjectPtr<UButton>				BTN_EquipShop;
	TWeakObjectPtr<UButton>				BTN_GoodsShop;
	TWeakObjectPtr<UButton>				BTN_MagicShop;
	TWeakObjectPtr<UButton>				BTN_ItemForge;

	TWeakObjectPtr<UB2Button>			BTN_DonationEvent;
	TWeakObjectPtr<UOverlay>			OV_DonationTimer;
	TWeakObjectPtr<UTextBlock>			TB_DonationTimer;
	TWeakObjectPtr<UTextBlock>			TB_DonationTitle;

	TWeakObjectPtr<UTextBlock>			TBS_Inventory;
	TWeakObjectPtr<UTextBlock>			TBS_Package;
	TWeakObjectPtr<UTextBlock>			TBS_EquipShop;
	TWeakObjectPtr<UTextBlock>			TBS_GoodsShop;
	TWeakObjectPtr<UTextBlock>			TBS_MagicShop;
	TWeakObjectPtr<UTextBlock>			TBS_ItemForge;

	TArray< TWeakObjectPtr<class UB2UIQuestSlot>>	QuestSlots;

	TWeakObjectPtr<UB2UIWidgetBase>				IMG_NewMark_HeroMgnt;

	/*TWeakObjectPtr<UTextBlock>			TB_ClassLevel;
	TWeakObjectPtr<UTextBlock>			TB_UserNickname;
	TWeakObjectPtr<UTextBlock>			STB_CombatPower;
	TWeakObjectPtr<UTextBlock>			TB_PowerValue;*/

	//TWeakObjectPtr<UButton>				BTN_ChangeFocusCharacter;
	TWeakObjectPtr<UButton>				BTN_UIOnOff;

	TArray<TWeakObjectPtr<UButton>>		BTN_SlideButtons;
	TWeakObjectPtr<UWidgetAnimation>	ANIM_SlideMove;
	TWeakObjectPtr<UButton>	BTN_SlideMove;

	TWeakObjectPtr<UB2Button>				BTN_CombineMenu;
	TWeakObjectPtr<UImage>					FX_CombineMenu;

	TWeakObjectPtr<class UB2UICombineMenu>	UI_CombineMenu;
	TWeakObjectPtr<UB2Image>				IMG_CombineMenu_Up;
	TWeakObjectPtr<UB2Image>				IMG_CombineMenu_Down;

	TWeakObjectPtr<UTextBlock>				TB_Menu;
	TWeakObjectPtr<UProgressBar>			ExpProgressBar;
	TWeakObjectPtr<UProgressBar>			ExpProgressBar_Max;

	const int MaxSlideButtonCount = 6;
	bool SlideOpen;
	bool bAllUIHidden;
	float ElapsedTimeNoInput; // 일정시간 후에 마티니 재생속도를 1.0f 로 만들기 위한 변수

	TUTORIAL TArray<FContentOpenSlot>		ContentOpenSlots;
	TUTORIAL TWeakObjectPtr<UImage>			IMG_TutorialBagLock;

	TWeakObjectPtr<UButton>					X_BTN_Survey;
	TWeakObjectPtr<UTextBlock>				X_TB_Survey;
	TWeakObjectPtr<class UB2UILobbyBanner>	UIP_Lobby_Banner;

	TWeakObjectPtr<class UB2UIStoreEventLabel>	UIP_Store_Event_Label;
	TWeakObjectPtr<class UB2UIStoreEventLabel>	UIP_SummonItem_Event_Label;

	TWeakObjectPtr<class UB2UIModeOpenNotice>	UIP_ModeOpenNotice;

	bool bReceiveGetChallengeMode;
	bool bExecutePendingDungeonScene;
	bool bReceiveGetDuelModeInfo;
	bool bExecutePendingPVPScene;

#if PLATFORM_ANDROID
	FDelegateHandle OnKakaoPromotionDeepLinkHandler;
	void OnKakaoPromotionDeepLinkPlatformCallback(JNIEnv* env, jobject thiz, jint nProductCode);
#elif PLATFORM_IOS
	FDelegateHandle OnKakaoPromotionDeepLinkHandler;
	void OnKakaoDeepLink(FString DeepLink);
#endif

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UB2UIMsgPopupGuestWarning> UIP_GuestWarningPopup_Class;

	UPROPERTY(Transient)
	UB2UIMsgPopupGuestWarning* Created_UIP_GuestWarningPopup;

	TWeakObjectPtr<UCanvasPanel> X_CP_GuestWarningPopup_Panel;

	void ConditionalCreateGuestWarningPopup();
	void DestroyGuestWarningPopup();

	UFUNCTION()
	void OpenGuestWarningPopup();
	UFUNCTION()
	void CloseGuestWarningPopup();
	UFUNCTION()
	void BuyGuestWarningPopup();

	int32 m_nProductID;
	FString m_sMarketProductID;

private:
	void RequestCombatPower(EPCClass SelectPCClass);
	void ResponseTrackingPowerDetail(FB2ResponseTrackingPowerDetailPtr Message);

	void ResponseJackpotFairy(FB2ServerCmdPtr Message);

	void InitDonationEvent();
	void UpdateDonationEvent(uint8 InState);

	void InitDonationRemainingTimer();
	void SetDonationRemainingTime();

	void InitDonationStartTimer();
	void SetDonationStartTime();

	void InitDonationExpireTimer();
	void SetDonationExpireTime();

	void EndDonationEventTimer();

	FTimerHandle DonationEventTimer;
	FDateTime DonationEventEndTime;
	FDateTime DonationEventStartTime;
	FDateTime DonationEventExpireTime;

	TArray<b2network::B2CharacterPowerSpecificPtr> CharacterPowerSpecificArray;
};