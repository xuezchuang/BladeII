// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIPVPCharacterSelectPage.generated.h"

UCLASS()
class BLADEII_API UB2UIPVPCharacterSelectPage : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void OnClose(bool RightNow = false) override;
	virtual void UpdateStaticText() override;
	virtual void SetChangeCombatPower(int32 InValue);

	UFUNCTION()
	void OnClickBtnEquipManagement();
	
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void PlayChangeChacterAnimBP();
	
	UFUNCTION()
	void OnClickBtnChangeMainSub();
	
	UFUNCTION()
	void OnClickMainButton();
	
	UFUNCTION()
	void OnClickSubButton();

	void BattleStart();
	void UpDateHero();
	void CheckStartButton();

	void PageBlock(bool bBlock);
	
	bool CanStartBattle();

	void SlotDelegateFunction(EPCClass InValue);

	void OnPrevAcceptInvitation();

	void OnInvitationDeclineByInvitee();
	void OnInvitationSuccess();
	void ShowUserInviteUI();

protected:

	void RequestStartPvPMatch(bool bUsedAdditional);
	bool CheckBuyTicket();
	void CheckGemForBuyTicket();

	void ShowFriendshipGameTimer();
	void SetGameStartSecondText(float Second);
	void StartFriendshipGame();
	void TimerTick_StartFriendshipGame(float LeftSeconds);

	bool CanStartFriendshipGame();

	FString GetHeaderTextKey(bool bFriendshipGame);

private:
	TArray<TWeakObjectPtr<class UB2UICharacterSelectPageSlot>>		HeroArr;
	
	TWeakObjectPtr<class UB2UIStartButton>		UIP_StartBT;
	
	TWeakObjectPtr<UTextBlock>		STB_CombatPower;
	TWeakObjectPtr<UTextBlock>		TB_CombatPower;
	TWeakObjectPtr<UB2Button>		BTN_EquipManagement;
	TWeakObjectPtr<UB2Image>		IMG_EmptyMain;
	TWeakObjectPtr<UB2Image>		IMG_EmptySub;
	TWeakObjectPtr<UVerticalBox>	VB_MainCharacter;
	TWeakObjectPtr<UVerticalBox>	VB_SubCharacter;
	TWeakObjectPtr<UB2Button>		BTN_MainHero;
	TWeakObjectPtr<UB2Button>		BTN_SubHero;
	TWeakObjectPtr<UB2Button>		BTN_CharacterChange;
	TWeakObjectPtr<UPanelWidget>	P_AutoStart;
	TWeakObjectPtr<UTextBlock>		TB_AutoStartNotice;

	TWeakObjectPtr<class UB2UIFairyLinkButton> UIP_FairyLinkButton;

	int32	nCurrentCombatPower;
	int32	ChangeMainClass;
	int32	ChangeSubClass;

	bool	bFriendshipMode;
	bool	IsInvitee;
	
	float	AutoStartSeconds;
	float	ElapsedSeconds;

	bool	bInInvitationSelect;
};
