// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidgetBase.h"
#include "B2UILobbyNoticeSlot.generated.h"

UENUM()
enum class EModeOpenType : uint8
{
	EModeOpenType_Raid = 0,
	EModeOpenType_Occupy = 1,
	EModeOpenType_Guild_Enter = 2,
	EModeOpenType_Guild_Request = 3,
	EModeOpenType_Raid_Request = 4,
	EModeOpenType_Max,
};

UCLASS()
class BLADEII_API UB2UILobbyNoticeSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

	TWeakObjectPtr<UTextBlock>	TB_Mode;
	TWeakObjectPtr<UTextBlock>	TB_Notice;
	TWeakObjectPtr<UTextBlock>	TB_LeftTime;
	TWeakObjectPtr<UButton>		BTN_ModeOpen;

public:	//FUNC_BLUE_PRINT
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII LobbyNoticeSlot")
	void SetModeIcon_BP(int32 iType);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII LobbyNoticeSlot")
	void EnableLobbyNoticeSlot_BP(int32 iType);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void RegisterUIMarkForRedDot() override;

public:
	UB2UILobbyNoticeSlot(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetModeOpenType(int32 iType);

	void OpenRaidInviteCheckPopup();
	void OpenRaidInviteSelectClassPopup();
	void RemoveRaidInvite();

	void SetTimeText(int32 nSecond);
	void CheckLimittime();
	void StartTimer(int32 nSecond);
	UFUNCTION()
	void OnClickBtnModeOpen();
public:
	EModeOpenType			m_eModeOpenType;
private:
	int32 nCurrentLeftTime;
	FTimerHandle LeftTimeHandler;
	bool bUsedInvitePopup;
};
