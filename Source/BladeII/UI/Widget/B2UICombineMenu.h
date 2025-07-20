// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UISettingMsgPopup.h"
#include "CommonStruct.h"
#include "B2UICombineMenu.generated.h"


UCLASS()
class BLADEII_API UB2UICombineMenu : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void CloseWidgetDelegate() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void RegisterUIMarkForRedDot() final;
	virtual void DoMarkRedDot() final;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void OpenMenu();
	void CloseMenu();
	void OnPlayOpenAni(bool bIsOpen);

	void KakaoPopupHidden();
	void KakaoConnect();

	void EnableHallOfFameButton();

protected:
	UFUNCTION()
	void OnClickBTN_Inven();
	UFUNCTION()
	void OnClickBTN_Collect();
	UFUNCTION()
	void OnClickBTN_Mission();
	UFUNCTION()
	void OnClickBTN_Friend();
	UFUNCTION()
	void OnClickBTN_Guild();
	UFUNCTION()
	void OnClickBTN_Skill();
	UFUNCTION()
	void OnClickBTN_Brevet();
	UFUNCTION()
	void OnClickBTN_Wing();
	UFUNCTION()
	void OnClickBTN_Relic();
	UFUNCTION()
	void OnClickBTN_Ether();
	UFUNCTION()
	void OnClickBTN_Attendance();
	UFUNCTION()
	void OnClickBTN_RestPoint();
	UFUNCTION()
	void OnClickBTN_Notice();
	UFUNCTION()
	void OnClickBTN_Community();
	UFUNCTION()
	void OnClickBTN_Event();
	UFUNCTION()
	void OnClickBTN_GoogleMission();
	UFUNCTION()
	void OnClickBTN_QNA();
	UFUNCTION()
	void OnClickBTN_Close();
	UFUNCTION()
	void OnClickBTN_HallOfFame();
	UFUNCTION()
	void OnClickBTN_Fairy();
	UFUNCTION()
	void OnClickBTN_Totem();
	UFUNCTION()
	void OnClickBTN_ItemForge();

	UFUNCTION()
	bool HasRedDot_Inventory();
	UFUNCTION()
	bool HasRedDot_ManageMent();
	UFUNCTION()
	bool HasRedDot_Collection();
	UFUNCTION()
	bool HasRedDot_Mission();
	UFUNCTION()
	bool HasRedDot_Friend();
	UFUNCTION()
	bool HasRedDot_Guild();

	UFUNCTION()
	bool HasRedDot_Skill();
	UFUNCTION()
	bool HasRedDot_Brevet();
	UFUNCTION()
	bool HasRedDot_Wing();
	UFUNCTION()
	bool HasRedDot_Relic();
	UFUNCTION()
	bool HasRedDot_Ether();

	UFUNCTION()
	bool HasRedDot_Attandance();
	UFUNCTION()
	bool HasRedDot_RestPoint();
	UFUNCTION()
	bool HasRedDot_Notice();
	UFUNCTION()
	bool HasRedDot_GoogleMission();
	UFUNCTION()
	bool HasRedDot_Event();
	UFUNCTION()
	bool HasRedDot_HallOfFame();
	UFUNCTION()
	bool HasRedDot_Fairy();
	UFUNCTION()
	bool HasRedDot_Totem();

	UFUNCTION(BlueprintImplementableEvent)
	void OnOpenCombineMenu_IMP(bool bIsOpen);

	void OpenHeroMgmtPagesWithAsyncLoadCommon(EHeroMgmtSubMode InHeroMgmtMode);

private:

	TWeakObjectPtr<UB2Button> BTN_Inven;
	TWeakObjectPtr<UB2Button> BTN_Collect;
	TWeakObjectPtr<UB2Button> BTN_Mission;
	TWeakObjectPtr<UB2Button> BTN_Friend;
	TWeakObjectPtr<UB2Button> BTN_Guild;
	TWeakObjectPtr<UB2Button> BTN_Skill;
	TWeakObjectPtr<UB2Button> BTN_Brevet;
	TWeakObjectPtr<UB2Button> BTN_Wing;
	TWeakObjectPtr<UB2Button> BTN_Relic;
	TWeakObjectPtr<UB2Button> BTN_Ether;
	TWeakObjectPtr<UB2Button> BTN_Attendance;
	TWeakObjectPtr<UB2Button> BTN_RestPoint;
	TWeakObjectPtr<UB2Button> BTN_Notice;
	TWeakObjectPtr<UB2Button> BTN_Community;
	TWeakObjectPtr<UB2Button> BTN_Event;
	TWeakObjectPtr<UB2Button> BTN_GoogleMission;
	TWeakObjectPtr<UB2Button> BTN_QNA;
	TWeakObjectPtr<UB2Button> BTN_HallOfFame;
	TWeakObjectPtr<UB2Button> BTN_Fairy;
	TWeakObjectPtr<UB2Button> BTN_Totem;
	TWeakObjectPtr<UB2Button> BTN_ItemForge;

	TWeakObjectPtr<class UB2UICostumeOpenBtn> UIP_CostumeOpenBtn;

	TWeakObjectPtr<class UB2UISeasonEventCombineMenuBtn> UIP_SeasonEventCombineMenuBtn;

	TWeakObjectPtr<UB2Button> BTN_Close;

	TWeakObjectPtr<UPanelWidget> P_HeroMgt;
	TWeakObjectPtr<UImage> IMG_HeroMgtSelect;


	TWeakObjectPtr<UTextBlock>		TB_Mgmt;
	TWeakObjectPtr<UTextBlock>		TB_HeroMgt;
	TWeakObjectPtr<UTextBlock>		TB_Collect;
	TWeakObjectPtr<UTextBlock>		TB_Mission;
	TWeakObjectPtr<UTextBlock>		TB_Friend;
	TWeakObjectPtr<UTextBlock>		TB_Guild;
	TWeakObjectPtr<UTextBlock>		TB_Skill;
	TWeakObjectPtr<UTextBlock>		TB_Promote;
	TWeakObjectPtr<UTextBlock>		TB_Wing;
	TWeakObjectPtr<UTextBlock>		TB_AncientRelic;
	TWeakObjectPtr<UTextBlock>		TB_Ether;
	TWeakObjectPtr<UTextBlock>		TB_Attendance;
	TWeakObjectPtr<UTextBlock>		TB_BreakTime;
	TWeakObjectPtr<UTextBlock>		TB_NoticeMenu;
	TWeakObjectPtr<UTextBlock>		TB_Community;
	TWeakObjectPtr<UTextBlock>		TB_Event;
	TWeakObjectPtr<UTextBlock>		TB_GoogleMission;
	TWeakObjectPtr<UTextBlock>		TB_HelpMenu;
	TWeakObjectPtr<UTextBlock>		TB_HallOfFame;
	TWeakObjectPtr<UTextBlock>		TB_Fairy;
	TWeakObjectPtr<UTextBlock>		TB_Totem;
	TWeakObjectPtr<UTextBlock>		TB_Forge;

	TWeakObjectPtr<UB2UISettingMsgPopup> UI_MsgPopupSetting_01;

	bool bSubscribed;
#if PLATFORM_ANDROID
	FDelegateHandle OnKakaoLinkHandler;
	void KakaoLinkDelegate(JNIEnv *env, jobject thiz);
#elif PLATFORM_IOS
	FDelegateHandle OnKakaoLinkHandler;
	void KakaoLinkDelegate(bool bIsSuccess);
#endif
	void MarkKakaoLink();
	void SubscribeEvent();
	void UnSubscribeEvent();

	void OnReceiveKakaoAccountConversionPrepare(FB2ReciveKakaoAccountConversionPrepare KakaoMessageProcessing);
	void OnReceiveKakaoAccountConversionComplete(FB2ReciveKakaoAccountConversionComplete KakaoMessageProcessing);
	
	bool HallOfFameButtonDelay;
	FTimerHandle HallOfFameButtonDelayTH;

	FThreadSafeBool m_KakaoLinkDelegateCalled;
	uint8	DeliveryKakaoAccountConversionPrepareTicket;
	uint8	DeliveryKakaoAccountConversionCompleteTicket;
};
