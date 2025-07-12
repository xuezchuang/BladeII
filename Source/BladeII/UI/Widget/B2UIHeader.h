// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIDocBindable.h"
#include "../B2RichTextBlock.h"
#include "B2UIHeader.generated.h"

/*
* Common Header UI
*/
#define  DEF_RAID_INVITE_COUNT_MAX 20;

USTRUCT(BlueprintType)
struct FApplyheaderResourceInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ResourceType")
	TArray<EUIScene> ApplyScenes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ResourceType")
	TArray<TSubclassOf<class UB2UIHeaderResource>> ToCreateResources;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIHeader : public UB2UIWidget, public IB2UIBackWidget, public IB2UIDocBindable
{
	GENERATED_BODY()

	/**
	 * DJLegacy (Lobby's Hero Management) is a separated system, but need this UIHeader as a common part. Otherwise, we need duplicated code.
	 */
	uint32 bIsInDJLegacyHeroMgmt : 1;
	EHeroMgmtSubMode DJLegacyHeroMgmtSubMode; // Valid only when bIsInDJLegacyHeroMgmt is true

public:
	UB2UIHeader(const FObjectInitializer& ObjectInitializer);

	void SetIsInDJLegacyHeroMgmt(bool bInDJLegacy, EHeroMgmtSubMode InSubMode);

	virtual void Init() override;
	virtual void BindDocAuto() override;
	virtual void UnbindDoc() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	void ShowUserInfo(bool bShow); //User information widgets. (Level, vip levels, Portrait ...)
	void ShowDailyNotice(bool bShow);
	void ShowBackBtn(bool bShow);
	void ShowTitle(bool bShow); //Header title text. For example, "1-1 chapter name."
	
	void SetTitle(const FText& InText);
	void SetSocialPoint(int32 InValue, int32 InValueMax, bool bFromInit = false);
	void SetGold(int32 InValue, bool bFromInit = false);
	void SetGem(int32 InValue, bool bFromInit = false);
	void SetStamina(int32 InValueCur, int32 InValueMax, bool bFromInit = false);
	void SetNextTimeStaminaGet(int32 InValue);
	void SetUserName(const FText& InText);
	void SetUserNickName();
	void SetHotTime();
	void SetDailyNotice();
	void SetUserGuild();
	void SetUserLevel(int32 InValue);
	void SetUserExp(int32 InValueCur, int32 InValueMax);
	void SetUserVipLevel(int32 InValue);
	void SetUserPortrait(UMaterialInterface* InMaterial);
	void SetHeroTowerTicket(int32 InValue, int32 InValueMax, bool bFromInit = false);
	void SetHeroPiece(int32 InValue, bool bFromInit = false);
	void SetUserBrevetPoint(int32 InValue, bool bFromInit = false);
	void SetUserAdvancedBrevetPoint(int32 InValue, bool bFromInit = false);
	void SetTagMatchTicket(int32 InValueCur, int32 InValueMax, bool bFromInit = false);
	void SetNextTimeTagMatchTicketGet(int32 InValue);
	void SetTeamMatchTicket(int32 InValueCur, int32 InValueMax, bool bFromInit = false);
	void SetNextTimeTeamMatchTicketGet(int32 InValue);
	void SetControlMatchTicket(int32 InValue, int32 InValueMax, bool bFromInit = false);
	void SetUserDarkCrystal(int32 InValue, bool bFromInit = false);
	void SetEtherPiece(int32 InValue, bool bFromInit = false);
	void SetFamePoint(int32 InValue, bool bFromInit = false);
	void SetDimensionPiece(int32 InValue, bool bFromInit = false);

	void RefreshUIInfo();

	void ApplyAddAmountAction(ERewardType rewardType);

	void SetForceVisibileMailRedDot(bool bVisible);

	void UpdateUIVisibleVerticalMode(bool bVerticalMode);

	void OnReceiveCheckModeOpen(const FB2ResponseCheckModeOpenPtr& fb2r_CheckModeOpen);
	bool GetModeOpenNoticeRedDot();
	void SetModeOpenNoticeRedDot(bool bEnable);

	bool GetRestoreRotate() { return IsAddRestoreRotate; }

	void SetForcedBlockHeader(bool isBlock);
	void SetForcedBlockHeader(bool isBlock, FLinearColor NewColor);

	virtual void CloseWidgetDelegate() override;
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Header")
		void ShowUserInfo_BP(bool bShow);
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Header")
		void ShowDailyNotice_BP(bool bShow);
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Header")
		void ShowBackBtn_BP(bool bShow);
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Header")
		void ShowTitle_BP(bool bShow);
	/* Visible or not visible.. */
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Header")
		void HideHeader(bool bHide);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Header")
	void AnimModeOpenNoticeSlots_BP(bool bOpenORClose);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII Header")
	void StartPollingModeOpenNotice_BP();

	UFUNCTION(BlueprintCallable, Category = "BladeII Header")
	void PollingModeOpenNotice_BP();

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	virtual void RegisterUIMarkForRedDot() final;

	/* It allows you to change the layout process according to the scene transition. */
	void ChangeLayoutByUIScene(EUIScene InScene);

	void CheckResourceParts(EUIScene InScene);

	void CreateResourcePartsUI(TArray<TSubclassOf<class UB2UIHeaderResource>> ResourcePartsUIClasses);

	void RotateScreen(bool bRotateToDefault);

	void SubscribeEvent();
	void UnSubscribeEvent();

	void OnUpdateCombineMenu(bool bVisible);
	void UpdateHelpButton();
	bool IsAnimPlayCheck(const FName& animName);

	void DelayFunc_OpenUI(FName UIName, float Delay = 0.0f);

	//===================================================Click
	UFUNCTION()
		void OnClickBtnBack();
	UFUNCTION()
		void OnClickBtnHome();
	UFUNCTION()
		void OnClickBtnMail();
	UFUNCTION()
		void OnClickBtnSettings();
	UFUNCTION()
		void OnClickBtnChat();
	UFUNCTION()
		void OnClickBtnUser();
    UFUNCTION()
        void OnClickBtnUserNickname();
	UFUNCTION()
		void OnClickBtnScreenRotate();
	UFUNCTION()
		void OnClickBtnRestoreRotate();
	UFUNCTION()
		void OnClickBtnNotice();
	UFUNCTION()
		void OnClickBtnHelp();
	UFUNCTION()
		void OnClickHotTime();
	UFUNCTION()
		void OnClickDailyNotice();

	//====================================================Doc Delegate
	UFUNCTION()
		void OnChangedSocialPoint(class UB2UIDocBase* Sender, int32 Point, int32 PrevPoint);
	UFUNCTION()
		void OnChangedGold(class UB2UIDocBase* Sender, int32 Gold, int32 PrevGold);
	UFUNCTION()
		void OnChangedGem(class UB2UIDocBase* Sender, int32 Gem, int32 PrevGem);
	UFUNCTION()
		void OnChangedFamePoint(class UB2UIDocBase* Sender, int32 FamePoint, int32 PrevFamePoint);
	UFUNCTION()
		void OnChangedStaminaCur(class UB2UIDocBase* Sender, int32 StaminaCur, int32 PrevStaminaCur);
	UFUNCTION()
		void OnChangedStaminaMax(class UB2UIDocBase* Sender, int32 StaminaMax, int32 PrevStaminaMax);
	UFUNCTION()
		void OnChangedNextTimeStaminaGet(class UB2UIDocBase* Sender, int32 NextTimeStaminaGet, int32 PrevNextTimeStaminaGet);
	UFUNCTION()
		void OnChangedTagMatchTicketCur(class UB2UIDocBase* Sender, int32 TagTicketCur, int32 PrevTagTicketCur);
	UFUNCTION()
		void OnChangedNextTimeTagMatchTicketGet(class UB2UIDocBase* Sender, int32 NextTimeTagTicketGet, int32 PrevNextTimeTagTicketGet);
	UFUNCTION()
		void OnChangedTeamMatchTicketCur(class UB2UIDocBase* Sender, int32 TeamTicketCur, int32 PrevTeamTicketur);
	UFUNCTION()
		void OnChangedNextTimeTeamMatchTicketGet(class UB2UIDocBase* Sender, int32 NextTimeTeamTicketGet, int32 PrevNextTimeTeamTicketGet);
	UFUNCTION()
		void OnChangedUserLevel(class UB2UIDocBase* Sender, int32 UserLevel, int32 PrevUserLevel);
	UFUNCTION()
		void OnChangedUserExpCur(class UB2UIDocBase* Sender, int32 UserExpCur, int32 PrevUserExpCur);
	UFUNCTION()
		void OnChangedUserExpMax(class UB2UIDocBase* Sender, int32 UserExpMax, int32 PrevUserExpMax);
	UFUNCTION()
		void OnChangedUserVipLevel(class UB2UIDocBase* Sender, int32 UserVipLevel, int32 PrevUserVipLevel);
	UFUNCTION()
		void OnChangedUserNickName(class UB2UIDocBase* Sender, FText UserNickName, FText PrevUserNickName);
	UFUNCTION()
		void OnChangedUserPortrait(class UB2UIDocBase* Sender, UMaterialInterface* UserPortrait, UMaterialInterface* PrevUserPortrait);
	UFUNCTION()
		void OnChangedGuildName(class UB2UIDocBase* Sender, FText GuildName, FText PrevGuildName);
	UFUNCTION()
		void OnChangedUserGuildPortrait(class UB2UIDocBase* Sender, UMaterialInterface* UserGuildPortrait, UMaterialInterface* PrevUserGuildPortrait);
	UFUNCTION()
		void OnChangedHeaderHeroPiece(class UB2UIDocBase* Sender, int32 Cur, int32 Prev);
	UFUNCTION()
		void OnChangedHeaderRankPromotionPoint(class UB2UIDocBase* Sender, int32 Cur, int32 Prev);
	UFUNCTION()
		void OnChangedHeaderAdvancedRankPromotionPoint(class UB2UIDocBase* Sender, int32 Cur, int32 Prev);
	UFUNCTION()
		void OnChangedHeaderDarkCrystal(class UB2UIDocBase* Sender, int32 Cur, int32 Prev);
	UFUNCTION()
		void OnChangedHeaderEtherPiece(class UB2UIDocBase* Sender, int32 Cur, int32 Prev);
	UFUNCTION()
		void OnChangedDimensionPiece(class UB2UIDocBase* Sender, int32 Cur, int32 Prev);
	UFUNCTION()
		void OnChangedShowVersion(class UB2UIDocBase* Sender, bool bCur, bool bPrev);
	UFUNCTION()
		void OnChangeHeaderChat(class UB2UIDocBase* Sender, int32 Cur, int32 Prev);
	UFUNCTION()
		void OnPlatformResume();
	//detect new UIScene Opening
	UFUNCTION()
		void OnChangedCurrUIScene(class UB2UIDocBase* Sender, EUIScene CurrUIScene, EUIScene PrevCurrUIScene);

	//detect header title text
	UFUNCTION()
		void OnChangedHeaderTitle(class UB2UIDocBase* Sender, FText HeaderTitle, FText PrevHeaderTitle);

	//detect Hide LobbyUIs
	UFUNCTION()
		void OnChangedHideAllLobbyMainUIs(class UB2UIDocBase* Sender, bool HideAllLobbyMainUIs, bool PrevHideAllLobbyMainUIs);

	UFUNCTION()
		bool RedDotCondition_Mail();
	UFUNCTION()
		bool RedDotCondition_Chat();
	UFUNCTION()
		bool RedDotCondition_Setting();
	UFUNCTION()
		bool RedDotCondition_ModeOpenNotice();

	bool IsAddRestoreRotate;

	bool m_bSenser;
	bool m_bRotate;
	bool m_bPopupCnt;
	bool m_bPortRate;
	bool m_LoadMovie;
	EUIScene m_CurrentSceneAOS;
protected:

	// Somewhat special notification to BP for playing sound.
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void PlaySoundOnBackBtn(EUIScene PrevUIScene);

	UB2UIHeaderResource* GetHeaderResourceUI(EUIHeaderResourceType UItype);

protected:

	TWeakObjectPtr<UPanelWidget>		P_Header;
	TWeakObjectPtr<UPanelWidget>		P_User;

	TWeakObjectPtr<UButton>				BTN_Back;
	TWeakObjectPtr<UButton>				BTN_Back_Text;
	TWeakObjectPtr<UButton>				BTN_Home;
	TWeakObjectPtr<UButton>				BTN_Mail;
	TWeakObjectPtr<UButton>				BTN_Setting;
	TWeakObjectPtr<UButton>				BTN_Chat;
	TWeakObjectPtr<UButton>				BTN_ScreenRotate;
	TWeakObjectPtr<UButton>				BTN_Notice;
	TWeakObjectPtr<UButton>				X_BTN_Help;
	TWeakObjectPtr<UB2RichTextBlock>	TB_Title;
	TWeakObjectPtr<UButton>				BTN_RestoreRotate;

	TWeakObjectPtr<class UB2UIHotTimeIcon> UIP_Hottime;
	TWeakObjectPtr<UButton>				BTN_HotTime;

	TWeakObjectPtr<class UB2UIHotTimeIcon> UIP_DailyNotice;
	TWeakObjectPtr<UButton>				BTN_DailyNotice;


	//User Info
	TWeakObjectPtr<UButton>				BTN_User;
	TWeakObjectPtr<UTextBlock>			TB_UserName;
	TWeakObjectPtr<UTextBlock>			TB_UserNickName;
    TWeakObjectPtr<UButton>             BTN_UserNickname;
	TWeakObjectPtr<UTextBlock>			TB_UserGuildName;
	TWeakObjectPtr<UTextBlock>			TB_UserLevel;
	TWeakObjectPtr<UTextBlock>			TB_UserExpCur;
	TWeakObjectPtr<UTextBlock>			TB_UserExpMax;
	TWeakObjectPtr<UTextBlock>			TB_UserVipLevel;
	TWeakObjectPtr<UImage>				IMG_UserPortrait;
	TWeakObjectPtr<class UB2UIGuildMark>	UIP_GuildMark;
	TWeakObjectPtr<UImage>				IMG_DefaultMark;
	TWeakObjectPtr<UHorizontalBox>		HB_ResourceBox;
	TWeakObjectPtr<UBorder>				X_ForcedBlockHeader;
	bool bIgnoreBackBTN;

	bool m_bIsHideHeader;

	bool bScreenVertical;

	bool m_bSubscribed;

	TWeakObjectPtr<UOverlay>							Overlay_ModOpenNotice;
	TArray<TWeakObjectPtr<class UB2UILobbyNoticeSlot>>	ModeOpenNoticeSlots;
	const int32											m_iMaxCountModeOpenNoticeSlot = 4;


	uint32 DeliveryCheckModeOpenTicket;
	uint32 OnUpdateCombineMenuTicket;

	bool	m_bModeOpenAnimPlayCheck;
    int		m_numNicknameClicked;
	
	bool	CachedDoneLoadingScreen;

	FTimerHandle UpdateHotTimeHandle;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ResourceType")
	TArray<TSubclassOf<class UB2UIHeaderResource>> BaseResources;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ResourceType")
	TArray<FApplyheaderResourceInfo> HeaderResourceSelectInfo;

	/** Prevent GC */
	UPROPERTY(Transient) 
	TArray<UB2UIHeaderResource*> m_arResourcePartsUI;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RaidInvite")
	int32 RaidInviteMaxCount;
*/

};
