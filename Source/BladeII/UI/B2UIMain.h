// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "BladeII.h"
#include "B2UIWidgetBase.h"
//#include "B2UINetworkLoading.h"
//#include "B2UIChatSinglePopup.h"
#include "B2Chatting.h"
#include "B2UIMain.generated.h"


/*
* The widget's top-level containers.
*/

UCLASS(BlueprintType, Blueprintable, Transient)
class BLADEII_API UB2UIMain : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UIMain(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPanelWidget* GetParentLayer(EUILayer InLayer);

	void ShowVersionInfo();
	void SetServerUrl(FString _CDNUrl, FString _GameServerUrl);

private:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	
	void SetDebugVersionInfo();

	TWeakObjectPtr<UCanvasPanel>		CP_TopMost;

	//TWeakObjectPtr<UB2UINetWorkLoading> UIP_NetWorkWaitResponseP;
	//// Use the same one as NetWorkLoading for now.. but still must be the different instance.
	//TWeakObjectPtr<UB2UINetWorkLoading> UIP_MiscLoadingP;

	//TWeakObjectPtr<UB2UIChatSinglePopup> UIP_ChatSinglePopupP;

	TWeakObjectPtr<UTextBlock> TB_DebugVersionInfo;
	TWeakObjectPtr<UTextBlock> TB_DebugServerVersionInfo;

	bool bSubscribed;
	uint8 DeliveryGetServerVersionTicket;

	////////////////////////////////////////////////////////////////////////////////
	// For default On-touch impulse ring, to be activated at non-combat UI scene.

	//TWeakObjectPtr<UB2Image> IMG_DefaultOnTouchImpulse;
protected:
	/** The time taken to fully extend the ring by material animation. */
	UPROPERTY(EditAnywhere, Category = "BladeII Fx")
	float OnTouchImpulseRingAnimTime;
	/** Parameter range of impulse ring material animation. */
	UPROPERTY(EditAnywhere, Category = "BladeII Fx")
	float OnTouchImpulseRingParamRange_Begin;
	UPROPERTY(EditAnywhere, Category = "BladeII Fx")
	float OnTouchImpulseRingParamRange_End;

	FImpluseRingInstInfo DefOnTouchImpulseRing; // Dynamically moving on-touch impulse ring for non-combat UI.
	uint32 bSignaledMaintainImpulseRing : 1;
	
	void OnOnTouchImpulseRingAnimTimeCycle(); // For every OnTouchImpulseRingAnimTime second.
public:	

	// To be driven by manual input event from LobbyGameMode.
	void ConditionalPlayOnTouchImpulseRing(const FVector2D& InPosition);
	void PendingStopOnTouchImpulseRing();
	void MoveOnTouchImpulseRing(const FVector2D& InMovedPosition);
	////////////////////////////////////////////////////////////////////////////////

	void ShowNetworkLoadingIcon(bool bInShow);
	void ShowMiscLoadingIcon(bool bInShow);

	void OpenChatSinglePopupWithNewMessage(const FB2ChatMessage& InNewMessage);
	void ForceCloseChatSinglePopup(); // Generally you don't have to call this explicitly. This is for special purpose.

	void SetServerVersionInfoText(FText ftServerInfoText);
};
