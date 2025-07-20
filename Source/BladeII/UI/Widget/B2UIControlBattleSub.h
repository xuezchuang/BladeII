// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "../../BladeII/BladeIICharacter.h"
#include "../B2RichTextBlock.h"
#include "B2UIControlBattleSub.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class EControlNpcUiState : uint8
{
	None,
	WaitSpawn,
	Live,
	QTE_Red,
	QTE_Blue,
};

USTRUCT(BlueprintType)
struct FControlChatMsgInfo
{
	GENERATED_USTRUCT_BODY()

	FControlChatMsgInfo()
	{
		MessageLocTextKey = TEXT("");
		MessageIconMaterial = NULL;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ControlChat")
	FString MessageLocTextKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ControlChat")
	TSoftObjectPtr<UMaterialInterface> MessageIconMaterial;
};

USTRUCT(BlueprintType)
struct FControlSerialKillTextureInfo
{
	GENERATED_BODY()

	FControlSerialKillTextureInfo()
	{
		KillCount = 0;
		KillTexture = NULL;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 KillCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> KillTexture;
};

UCLASS()
class BLADEII_API UB2UIControlBattleSub : public UB2UIWidget
{
	GENERATED_BODY()
public:
	UB2UIControlBattleSub(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void NativeConstruct() override;
	virtual void UpdateStaticText() override;

	virtual void BindDelegates() override;

	void SetControlObjectState(uint8 ControlObjectState, float fConquestGageRed, float fConquestGageBlue);
	void SetConquestGageInfo(bool bRed, float fRate);

	UFUNCTION(BlueprintImplementableEvent)
	void SetControlPoint(bool bRed, float fPoint);

	void AddTeamPlayerInfoUI(class ABladeIINetPlayer* pPlayer);
	void UpdateKillInfo(class ABladeIINetPlayer* pKillerPlayer, class ABladeIINetPlayer* pDeadPlayer);
	void SetUserKillInfos(TMap<uint32, int32> KillInfos);
	void SetUserKillCountToPlayerInfoUI(uint32 NetId, int32 KillCount);

	void SetSerialKillCount(int32 nKillCount);
	void ViewPlayerJoinUI(class ABladeIINetPlayer* pPlayer);
	void ViewControlGameNotice(bool bSuccess);
	void PlayResultAction(ETeamType WinTeam, ETeamType MyTeam);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_AnimOpenLock();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_AnimSingleKillNotice(const int32 KillCount);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_AnimControlGameNotice(bool bSuccess);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_AnimJoinNotice();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_AnimResultAction(bool bWin);

	UFUNCTION(BlueprintImplementableEvent)
	void SetControlNpcUIState(EControlNpcUiState state, float TimerSec = 0.0f);

	// BP 譬配府倔侩 UI利侩 or 檬扁拳
	UFUNCTION(BlueprintImplementableEvent)
	void SetTutorialUIState(bool bEndTutorial = false);

	UFUNCTION(BlueprintImplementableEvent)
	UB2UIWidget* CreateChatMessageWidget(EControlChatType ChatType, const FText& ContentText, UMaterialInterface* IconMaterial, class ABladeIIPlayer* OwnerPlayer);

	UFUNCTION(BlueprintImplementableEvent)
	void OnRespawnControlSanctuary();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetLeaveEnemyText(ESlateVisibility InVisible);
	void SetHiddneTimerUI(bool bHiddne);
	void UpdateTimerUI(FText& TextContent);

	void CloseChatPanel();

	void NotifySendControlChat();
	void NotifyReceiveControlChat(uint32 NetId, EControlChatType ChatType, int32 MessageIndex);

	void SetPlayerResurrectTime(uint32 NetId, float RemainTime);

	void SetMsgSubmersion(bool InValue);
	void SetMsgSubmersionTime(int32 InTime);

protected:
	virtual void CacheAssets() override;

	void SetControlStateImage(UMaterialInterface* pMI);

	void SetProgressImage(UMaterialInterface* pMI);

	void SetProgressBackImage(UMaterialInterface* pMI);

	void SetProgressGage(float fProgress);

	void InitPlayerInfos();

	void SetTeamKillCount(int32 RedKillCount, int32 BlueKillCount);

	void UpdateMyAssultNotice();
	void UpdateAssultState();
	void UpdateControlChatCoolTime();

private:
	void SubscribeEvent();
	void UnsubscribeEvent();

	UFUNCTION()
	void OnClickBtnChat();

	void InitControlChatPanel();

	void SetChatImageCoolTimeProgress(float fProgress);

	class UTexture2D* GetSerialKillTexture(int32 nKillCount);

	void SetVisibilityChatButton(ESlateVisibility NewVisibility);

	class UOverlay* GetPlayerInfoOverlay(ETeamType TeamType);

protected:
	TWeakObjectPtr<UImage> IMG_PrograssBG;
	TWeakObjectPtr<UImage> IMG_Prograss;
	TWeakObjectPtr<UImage> IMG_Stat;

	TWeakObjectPtr<UProgressBar> PB_ControlPointRed;
	TWeakObjectPtr<UProgressBar> PB_ControlPointBlue;

	TWeakObjectPtr<UVerticalBox> VB_Kill_Infos;

	TWeakObjectPtr<UTextBlock> TB_PointRed;
	TWeakObjectPtr<UTextBlock> TB_PointBlue;
	TWeakObjectPtr<UTextBlock> TB_KillCountRed;
	TWeakObjectPtr<UTextBlock> TB_KillCountBlue;
	TWeakObjectPtr<UTextBlock>	TB_LeaveEnemy;

	TWeakObjectPtr<UImage> IMG_SerialKillCount;
	TWeakObjectPtr<UTextBlock> TB_ControlGameNotice;
	TWeakObjectPtr<UTextBlock> TB_RemainGameTime;

	TWeakObjectPtr<UOverlay> O_MyAssultNotice;
	TWeakObjectPtr<UTextBlock> TB_MyAssultNoticeR;
	TWeakObjectPtr<UTextBlock> TB_MyAssultNoticeB;
	TWeakObjectPtr<UTextBlock> TB_ResultNotice;
	TWeakObjectPtr<UB2RichTextBlock> RTB_JoinNotice;

	TWeakObjectPtr<UOverlay> O_Inside_Conlict;
	TWeakObjectPtr<UOverlay> O_Inside_B;
	TWeakObjectPtr<UOverlay> O_Inside_R;

	TWeakObjectPtr<UOverlay> O_Msg_Time;

	TWeakObjectPtr<UOverlay> O_Red1;
	TWeakObjectPtr<UOverlay> O_Red2;
	TWeakObjectPtr<UOverlay> O_Red3;
	TWeakObjectPtr<UOverlay> O_Red4;
	TWeakObjectPtr<UOverlay> O_Blue1;
	TWeakObjectPtr<UOverlay> O_Blue2;
	TWeakObjectPtr<UOverlay> O_Blue3;
	TWeakObjectPtr<UOverlay> O_Blue4;

	TWeakObjectPtr<UB2Button> BTN_Chat;
	TWeakObjectPtr<UB2Image> IMG_ChatDis;
	TWeakObjectPtr<UB2Image> IMG_ChatCoolTime;

	TWeakObjectPtr<class UB2UIControlChatPanel> UI_Control_Chat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ControlArea")
	TSoftObjectPtr<UMaterialInterface> ControlAreaMarkNeutral;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ControlArea")
	TSoftObjectPtr<UMaterialInterface> ControlAreaMarkRed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ControlArea")
	TSoftObjectPtr<UMaterialInterface> ControlAreaMarkBlue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ControlArea")
	TSoftObjectPtr<UMaterialInterface> ConquestGageRed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ControlArea")
	TSoftObjectPtr<UMaterialInterface> ConquestGageBlue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerInfo")
	TSubclassOf<class UB2UIControlTeamPlayerInfo> PlayerInfoClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerInfo")
	TSubclassOf<class UB2UIControlKillInfo> KillInfoClass;

	TArray<UB2UIControlTeamPlayerInfo*> m_arTeamPlayerInfoUI;

	uint8 m_ControlObjectState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ControlChat")
	TMap<int32, FControlChatMsgInfo> MessageInfos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ControlChat")
	TMap<int32, FControlChatMsgInfo> EmoticonInfos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ControlChat")
	float ControlChatCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ControlSerialKill")
	TArray<FControlSerialKillTextureInfo> ControlSerialKillTextureInfos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ControlUIPos")
	FVector2D vComboUIPos;


	//Static Text

	TWeakObjectPtr<UTextBlock>					TB_MinoNotice;
	TWeakObjectPtr<UTextBlock>					TB_SancNotice;
	TWeakObjectPtr<UTextBlock>					T_R;
	TWeakObjectPtr<UTextBlock>					T_B;

private:
	uint32 NotifyInputTouchBeganInViewPortTicket;

	float ControlChatSendTimeSec;

	TArray<UOverlay*> arRedPlayerOverlays;
	TArray<UOverlay*> arBluePlayerOverlays;
	TWeakObjectPtr<UPanelWidget> O_Msg_Submersion;
	TWeakObjectPtr<UTextBlock> TB_SubmersionTitle;
	TWeakObjectPtr<UTextBlock> TB_SubmersionDesc;
	TWeakObjectPtr<UTextBlock> TB_SubmersionTime;
};
