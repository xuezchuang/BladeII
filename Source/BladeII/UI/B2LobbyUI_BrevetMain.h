// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UI/DJLegacy/B2LobbyUserWidget.h"
#include "B2UIBackWidget.h"
#include "CommonStruct.h"
#include "B2UnitedWidgetBase.h"
#include "B2ClientDataStore.h"

#include "B2LobbyUI_BrevetMain.generated.h"

USTRUCT(BlueprintType)
struct FGaugeAnimationEle
{
	GENERATED_BODY()

public:
	FGaugeAnimationEle() 
		: DestGaugePosition(-1)
		, TotalGaugeTime(-1)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GaugeSpeed")
	float DestGaugePosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GaugeSpeed")
	float TotalGaugeTime;
};

/**
 * 
 */
UCLASS()
class BLADEII_API UB2LobbyUI_BrevetMain : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
public:
	UB2LobbyUI_BrevetMain(const FObjectInitializer& ObjectInitializer);

protected:
	/** Contains common part of several hero management UI pages.
	* UMG script is responsible for setting this at runtime. */
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadWrite)
	class UB2LobbyUI_HeroMgmtCommon* HeroMgmtBaseNRef;

public:
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) /*override*/;
	virtual void DestroySelf() /*override*/;
	virtual void BeginDestroy() /*override*/;
	void UpdateStaticText();

	void OnHeroMgmtPCChanged(EPCClass InSelectedPCClass);
	void RefreshBrevetUIInfo(bool bReset = false);
	void RefreshBrevetUIScrollBoxInfo();
	bool CheckNeedNodeOpenAnimation();
	void OnSelectedCurrentBrevetNode();
	int32 GetCurrentBrevetRank();
	int32 GetCurrentBrevetNodeIndex();

	int32 GetAppliedBrevetRank();
	int32 GetAppliedBrevetNodeIndex();	

	int32 GetOldBrevetRank();
	int32 GetOldBrevetNodeIndex();
	bool IsCompletedCurrentBrevetNode();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NodeIcon")
	TSoftObjectPtr<UMaterialInterface> NodeIconAtk;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NodeIcon")
	TSoftObjectPtr<UMaterialInterface> NodeIconAtk_Light;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NodeIcon")
	TSoftObjectPtr<UMaterialInterface> NodeIconDef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NodeIcon")
	TSoftObjectPtr<UMaterialInterface> NodeIconDef_Light;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NodeIcon")
	TSoftObjectPtr<UMaterialInterface> NodeIconHp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NodeIcon")
	TSoftObjectPtr<UMaterialInterface> NodeIconHp_Light;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NodeIcon")
	TSoftObjectPtr<UMaterialInterface> NodeIconAtk_Progress;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NodeIcon")
	TSoftObjectPtr<UMaterialInterface> NodeIconAtk_ProgressBack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NodeIcon")
	TSoftObjectPtr<UMaterialInterface> NodeIconDef_Progress;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NodeIcon")
	TSoftObjectPtr<UMaterialInterface> NodeIconDef_ProgressBack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NodeIcon")
	TSoftObjectPtr<UMaterialInterface> NodeIconHp_Progress;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NodeIcon")
	TSoftObjectPtr<UMaterialInterface> NodeIconHp_ProgressBack;
	
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NodeIcon")
	//TSubclassOf<class UB2LobbyUI_BrevetNodeIcon> NodeItemClass;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NodeIcon")
	//TSubclassOf<class UB2LobbyUserWidget> NodeItemClass_DummyL;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NodeIcon")
	//TSubclassOf<class UB2LobbyUserWidget> NodeItemClass_DummyR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GaugeSpeed")
	TArray<FGaugeAnimationEle> BaseGaugeAnimationList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GaugeSpeed")
	TArray<FGaugeAnimationEle> SuccessGaugeAnimationList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GaugeSpeed")
	TArray<FGaugeAnimationEle> FailGaugeAnimationList;

	TQueue<FGaugeAnimationEle> CurGaugeAnimationQueue;

	//void SetNodeIconFromStatusType(UB2LobbyUI_BrevetNodeIcon* TargetUI, EBrevetStatusType StatusType);
	UMaterialInterface* GetNodeProgressIMGFromStatusType(EBrevetStatusType StatusType);
	virtual void CacheAssets() /*override*/;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual void CloseWidgetDelegate() /*override*/;
	void UpdateCurrentNodeGauge(float InDeltaTime);
	float GetCurGaugePercent();
	void SetCurrentNodeProgress(float fGaugePercent = 0.0f);
	void CheckCurrentScrollViewState();
	void CheckAutoUpgrade(float fDelayTime = 0.0f);
	void SetToggleAutoUpgradeBtn(bool IsOpenNoticePopupWindow = false);
	void UpdateVisibilityAutoUpgradeBtn();
	void StopAutoUpgrade(bool IsOpenNoticePopupWindow = false);
	bool IsUpgrading();

	void OpenResultUI(bool IsSuccess);
	void OpenAddStatusUI();
	void OpenRankUpSuccessUI();
	void SetCurrentNode();
	const FBrevetNodeInfo* GetCurrentBrevetNodeInfo();
	//UB2LobbyUI_BrevetNodeIcon* GetNextNode();

	UFUNCTION()
	void StartGaugeAnimation(bool IsSuccessEnhance);
	void UpdateNextGaugeAnimation(float CurVelocity, bool& OutIsOpenResultNotice, bool& OutIsEnhanceSuccess);

	void BlockInputBrevetMain(bool bCheck);

	// Widgets
protected:
	TWeakObjectPtr<UImage> IMG_RankIcon;
	TWeakObjectPtr<UTextBlock> TB_RankName;
	TWeakObjectPtr<UTextBlock> TB_BrevetAtk;
	TWeakObjectPtr<UTextBlock> TB_BrevetDef;
	TWeakObjectPtr<UTextBlock> TB_BrevetHp;
	TWeakObjectPtr<UTextBlock> TB_HonerPoint;
	TWeakObjectPtr<UTextBlock> TB_GoldPoint;	
	TWeakObjectPtr<UTextBlock> TB_BtnAutoUpgrade;
	TWeakObjectPtr<UB2Button> BTN_ToCenter;
	TWeakObjectPtr<UB2Button> BTN_HelpBrevetRank;
	TWeakObjectPtr<UB2Button> BTN_HelpHonerPoint;
	//TWeakObjectPtr<UB2ScrollBox> SB_BrevetNode;
	TWeakObjectPtr<UB2Button> BTN_Block;
	TWeakObjectPtr<UB2Button> BTN_Block2;
	TWeakObjectPtr<UB2Button> BTN_AutoUpgrade;
	//TWeakObjectPtr<UB2ButtonGoodInfoToolTip> BTN_BrevetGoods;
	TWeakObjectPtr<UImage> IMG_BrevetGoods;
	//TWeakObjectPtr<UB2ButtonGoodInfoToolTip> BTN_GoldGoods;

	TWeakObjectPtr<UTextBlock>				TB_TotalAbil;
	TWeakObjectPtr<UTextBlock>				TB_ExAtk;
	TWeakObjectPtr<UTextBlock>				TB_ExDef;
	TWeakObjectPtr<UTextBlock>				TB_ExHp;
	TWeakObjectPtr<UTextBlock>				TB_ProgressGrade;
	TWeakObjectPtr<UTextBlock>				TB_Cost;
	TWeakObjectPtr<UTextBlock>				TB_Automatic;
	TWeakObjectPtr<UTextBlock>				TB_BtnUpgrade;

	//UB2LobbyUI_BrevetNodeIcon* m_pCurrentBrevetNode;

	
	//TArray<UB2LobbyUserWidget*> m_arBrevetNodes;

	// Tickets
private:
	int32 DeliveryEnhanceRankNodeTicket;
	int32 LobbySetHeroMgmtModePCSelectionTicket;

	// Member Variables
private:
	int32 temporaryVariable;
	EPCClass CurrentPCClass;
	float m_fStartTime;
	float m_fStartBaseSpeed;
	float m_fGaugePercentOld;
	float m_fDestGaugePercent;
	float m_fAcceleration;
	float m_fTotalTime;

	bool m_bIsAutoUpgradeState;
	bool m_bIsUpgrading;
	FTimerHandle DelayAutoUpgradeTimerHandle;

	int32 m_nOldRank;
	int32 m_nOldNodeIndex;

public:
	UFUNCTION()
	void OnAutoUpgradeBtnClick();
	UFUNCTION()
	void OnToCenterBtnClick();
	UFUNCTION()
	void OnHelpHonerPoint();
	UFUNCTION()
	void OnHelpBrevetRank();	

	void OnUpgradeBtnClick();	
	void OnDeliveryEnhanceRankNode(FB2EnhanceRankNode EnhanceRankNode);	
	void OnFinishAnimationEvent_BrevetAddStatusInitAnimation();
	void OnFinishAnimationEvent_Anim_Current_Close();
	void OnFinishAnimationEvent_Anim_RankUp();
	void OnFinishAnimationEvent_Anim_UnLock_BP();

	UFUNCTION(BlueprintCallable, Category = "B2 Brevet")
	void OnFinishAnimationEvent_Anim_FailNotice_BP();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_FailPopup_BP();

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySoundStartUpgradeNode_BP();

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySoundResultUpgradeNode_BP(const bool IsSuccess);
};