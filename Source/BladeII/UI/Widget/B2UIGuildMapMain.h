// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIWidgetBase.h"
#include "B2UIBackWidget.h"
#include "B2ScrollBox.h"
#include "../B2RichTextBlock.h"
#include "B2UIGuildMapMain.generated.h"

UCLASS()
class BLADEII_API UB2UIGuildMapMain : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	UB2UIGuildMapMain(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	void MedalDestroySelf(UB2UIManager* InUIManager,TArray<class UB2UIGuildInfoMedalCastlePart*>& InMedalUI);

	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void InitOpenGuildMap();
	

public:
	void	CameraPosSetting(float PosY1, float PosY2, float PosY3);
	float	ScrollCameraSetting();
	float	DoneMoveCameraSetting();
	void	FinishMatineeSetting();
	void	ScrollSetting(bool MyMap, int32 Member);
	void	SetGuildInfo();
	void	MedalMoveUI();
	void	SetMedalMoveUI(TArray<class AB2GuildCastleActor*>& InCasleInfo, TArray<class UB2UIGuildInfoMedalCastlePart*>& InMedalUI);
	void	WorldMapUIUpdate(int32 Lv);
	void	SetWorldMapUIUpdate(int32 InIndex, TArray<class AB2GuildCastleActor*>& InCasleInfo, TArray<class UB2UIGuildInfoMedalCastlePart*>& InMedalUI);
	void	SetCastleUISetting(TArray<class AB2GuildCastleActor*>& InCasleInfo, TArray<class UB2UIGuildInfoMedalCastlePart*>& InMedalUI, int32 InPage);
	void	SetCastleInfo();
	void	UpdateQuipSlot();
	void	SetGuildChangeText();

	void	OnGuildRightGuildInfo(bool bMyGuild, const b2network::B2GuildBattleMemberPtr& GuildMember, const TArray<b2network::B2GuildBattleAttackPtr>& AttackInfo, int32 CastleIndex);
	void	QuickScrollMove(int32 MapIndex);
	void	ChangeGuildCastleUI();

	void	UIHidden(bool State);
	void	EnterMatineePlay(class ALevelSequenceActor * EnterA, class ALevelSequenceActor * EnterB);
	void	GuildBattleTimeTick();

	void	RefreshInfo();


	void	RequestBattleInfo();
	bool	IsPagePopup();
	b2network::B2GuildBattleMemberPtr	GetMyGuildMemberInfo();

	static int32 MaxAttackCount;
	static int32 MaxMedal;
	static int32 MaxMapSlotCount;

	static bool MyGuildInfoState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TSubclassOf<class UB2UIGuildMapSlot>				MapSlot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TSubclassOf<class UB2UIGuildInfoMedalCastlePart>			GuildInfoMedal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII")
		float CastleSlotYPos;

	UFUNCTION(BlueprintImplementableEvent)
		void SpecialVisibleAni();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayVisibleAni();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayHiddenAni();


protected:
	UFUNCTION()
		void OnClickBTN_FormationInfo();
	UFUNCTION()
		void OnClickBTN_BattleList();

	UFUNCTION()
		void OnClickBTN_OtherGuild();

	UFUNCTION()
		void OnClickBTN_Back();
	UFUNCTION()
		void OnClickBTN_Chat();
	UFUNCTION()
		void OnClickBTN_QuipUp();
	UFUNCTION()
		void OnClickBTN_QuipDown();
	UFUNCTION()
		void OpenAnimFinish();

private:
	TWeakObjectPtr<UB2RichTextBlock> TB_Time;
	TWeakObjectPtr<UB2Button> BTN_FormationInfo;
	TWeakObjectPtr<UB2Button> BTN_BattleList;
	TWeakObjectPtr<UB2Button> BTN_Back;
	TWeakObjectPtr<UB2Button> BTN_Chat;
	TWeakObjectPtr<UB2Button> BTN_QuipUp;
	TWeakObjectPtr<UB2Button> BTN_QuipDown;

	TWeakObjectPtr<UB2Button>						BTN_OtherGuild;
	TWeakObjectPtr<UB2RichTextBlock>				TB_BTNOtherGuildText;
	TWeakObjectPtr<class UB2UIGuildMapEntryDirect>	UIP_GuildMapEntryDirect;
	TWeakObjectPtr<class UB2UIGuildMapCharPopup>	UIP_GuildMapInfoPopup;
	TWeakObjectPtr<class UB2UIGuildMapCharPopup>	UIP_GuildMapInfoPopupRight;
	TWeakObjectPtr<UB2Image>						IMG_ScrollSlot;
	TWeakObjectPtr<UTextBlock>	TB_GuildMemberCount;

	TWeakObjectPtr<UTextBlock>	TB_AttackCount;

	TWeakObjectPtr<UB2ScrollBox>		SB_MapScroll;
	TWeakObjectPtr<UVerticalBox>		VB_MapScroll;

	TWeakObjectPtr<UVerticalBox>		VB_MapQuipSlot;
	TWeakObjectPtr<class UB2UIGuildBattleSiuation>	GuildBattleSituationPopup;


	int32 ScrollLv;
	int32 NowScrollLv;
	int32 MyGuildCastleIndex;
	int32 SlotMax;

	TArray<UB2UIGuildInfoMedalCastlePart*>				ArrUIGuildInfoMedal1_1;
	TArray<UB2UIGuildInfoMedalCastlePart*>				ArrUIGuildInfoMedal1;
	TArray<UB2UIGuildInfoMedalCastlePart*>				ArrUIGuildInfoMedal2;
	TArray<UB2UIGuildInfoMedalCastlePart*>				ArrUIGuildInfoMedal2_1;
	TWeakObjectPtr<UWidgetAnimation> Anim_Open;
	TWeakObjectPtr<UWidgetAnimation> Anim_SpecialOpen;

	FTimerHandle Timerhandle;

	bool MatineeState;
	bool ScrollState;
	bool bUIHideState;


	float FirstCameraY;
	float SecondCameraY;
	float ThirdCameraY;

	int32 SelectIndex;

	int32 MyGuildIndex;
	int32 OtherGuildIndex;

	int64 GuildBattleEndTime;

};