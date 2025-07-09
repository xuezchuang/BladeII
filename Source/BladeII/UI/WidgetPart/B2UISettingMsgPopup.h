// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2PlatformAPI.h"
#include "B2UIBackWidget.h"
#include "B2UISettingMsgPopup.generated.h"

UENUM(BlueprintType)
enum class EPopupTypeSetting : uint8
{
	EPopupType_CheckPermission = 0,
	EPopupType_DownloadVolume,
	EPopupType_KakaoLogin,
	EPopupType_ErrorRespones,
	EPopupType_SettingKakaoLink,
	EPopupType_CombineKakaoLink,
	EPopupType_GuestWarningLink,

	None,
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISettingMsgPopup : public UB2UIWidget
{
	GENERATED_BODY()

	friend class UB2UISettingAccount;
	friend class UB2UICombineMenu;
	friend class UB2UIMsgPopupGuestWarning;

protected:
	virtual void CacheAssets() override;
public:
	UB2UISettingMsgPopup(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	class UB2UISettingAccount* ownerSetting_;
	class UB2UICombineMenu* ownerCombineMenu_;
	class UB2UIMsgPopupGuestWarning* ownerGuestWarning_;

	void SetOwnerSettingUI(class  UB2UISettingAccount* ownerSetting);
	void SetOwnerCombineUI(class  UB2UICombineMenu* ownerCombine);
	void SetOwnerGuestWarningUI(class  UB2UIMsgPopupGuestWarning* ownerGuestWarning);

	void SetPopupType(EPopupTypeSetting eType);
	void SetRewardItem(int32 nitemID, int32 nCnt);

private:
	TWeakObjectPtr<UB2Button> BTN_Positive;
	TWeakObjectPtr<UB2Button> BTN_Negative;

	TWeakObjectPtr<UTextBlock> TXT_DownloadFileValue;
	TWeakObjectPtr<class UB2UIRewardIcon>	UIP_RewardIcon;

	TWeakObjectPtr<UTextBlock> TXT_NetWorkFail;
	TWeakObjectPtr<UTextBlock> TXT_FileError;
	TWeakObjectPtr<UTextBlock> TXT_FileErrorSub;

	EPopupTypeSetting m_PopupType;
	UFUNCTION(BlueprintCallable, Category = "DCLFront")
		void OnClickNegative();
	UFUNCTION(BlueprintCallable, Category = "DCLFront")
		void OnClickKakaoExitBtn();

};
