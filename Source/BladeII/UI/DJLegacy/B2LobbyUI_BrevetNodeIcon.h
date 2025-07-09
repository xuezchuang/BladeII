// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/DJLegacy/B2LobbyUserWidget.h"
#include "B2LobbyUI_BrevetMain.h"
#include "B2UIBrevetResultNotice.h"
#include "B2LobbyUI_BrevetNodeIcon.generated.h"

/**
 * 
 */
enum class EBrevetNodeState : uint8
{
	EBNS_None,
	EBNS_Locked,
	EBNS_UnLocked,
	EBNS_Complete,
	EBNS_Locked_Current,
};

UCLASS()
class BLADEII_API UB2LobbyUI_BrevetNodeIcon : public UB2LobbyUserWidget
{
	GENERATED_BODY()
	
public:
	UB2LobbyUI_BrevetNodeIcon(const FObjectInitializer& ObjectInitializer);

	void InitBrevetNodeIcon(UB2LobbyUI_BrevetMain* pOwnerUI, const FBrevetNodeInfo* BrevetNodeInfo, EBrevetNodeState BrevetNodeState);
	void SetIconImage(UMaterialInterface* pMI);
	void SetIconLightImage(UMaterialInterface* pMI);
	void SetProgressImage(UMaterialInterface* pMI);
	void SetProgressBackImage(UMaterialInterface* pMI);
	void SetProgressGauge(float fProgress);
	float GetProgressGauge();	
	void UpdateStaticText();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_St_Current_Lock();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_St_Next_Lock();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_St_Current_Open();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_St_Current_Close();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_Anim_Current_Lock();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_Anim_Current_Open();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_Anim_Current_Close();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_Anim_ReadytoActivate();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationEvent_Anim_FailNotice_BP();

	void SetBreverNodeNum(int32 NewNum);

	UFUNCTION(BlueprintCallable, Category = "B2 Brevet")
	int32 GetBrevetNodeNum();

	void SetSuccessRateText(int32 nSuccessRate, bool bIsComplete = false);
	void SetBrevetNodeState(EBrevetNodeState BrevetNodeState);
	EBrevetNodeState GetBrevetNodeState();
	void SetNotOpenText(int32 nNodeNum);

	UFUNCTION()
	void OnSelectBrevetNode();
		
protected:
	virtual void CacheAssets() override;

	UFUNCTION(BlueprintCallable, Category = "B2 Brevet")
	void OnFinishAnimationEvent_Anim_Current_Close();

	UFUNCTION(BlueprintCallable, Category = "B2 Brevet")
	void OnFinishAnimationEvent_Anim_UnLock_BP();

	UFUNCTION(BlueprintCallable, Category = "B2 Brevet")
	void OnFinishAnimationEvent_Anim_FailNotice_BP();
protected:
	UB2LobbyUI_BrevetMain* m_pOwnerUI;
	TWeakObjectPtr<UImage> IMG_StatusIcon;
	TWeakObjectPtr<UImage> IMG_StatusIcon_Activate;
	TWeakObjectPtr<UImage> IMG_Progress;
	TWeakObjectPtr<UImage> IMG_ProgressBack;
	TWeakObjectPtr<UB2Button> BTN_Main;
	TWeakObjectPtr<UTextBlock> TB_BeforeNodeNum;
	TWeakObjectPtr<UTextBlock> TB_ActiveOpen;
	TWeakObjectPtr<UTextBlock> TB_GetStatusType;
	TWeakObjectPtr<UTextBlock> TB_GetStatusValue;
	TWeakObjectPtr<UTextBlock> TB_StatusType;
	TWeakObjectPtr<UTextBlock> TB_StatusValue;
	TWeakObjectPtr<UTextBlock> TB_RateText;

	TWeakObjectPtr<UTextBlock>	TB_Completed;
	TWeakObjectPtr<UTextBlock>	TB_Success;


private:
	int32 m_nBrevetNodeIndex;
	EBrevetNodeState m_eBrevetNodeState;
	float m_fGaugeProgress;

public:
	const FBrevetNodeInfo* m_pBrevetNodeInfo;
};
