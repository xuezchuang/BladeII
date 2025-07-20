// Fill out your copyright notice in the Description page of Project Settings.
#pragma once


#include "B2UIDLCFrontBase.h"
#include "B2UIRewardEventSet.h"
#include "B2UIDLCFrontPopup.generated.h"

UENUM(BlueprintType)
enum class EPopupType : uint8
{
	EPopupType_CheckPermission = 0,
	EPopupType_DownloadVolume,
	EPopupType_KakaoLogin,
	EPopupType_ErrorRespones,
	EPopupType_NotEnoughDiskSpace,
	None,
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIDLCFrontPopup : public UB2UIDLCFrontBase
{
	GENERATED_BODY()
	friend class UB2UIPatch;
	friend class UB2UIActionLogo;
	friend class UB2UISettingAccount;
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	UB2UIDLCFrontPopup(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	
	class UB2UIPatch* owner_;
	class UB2UIActionLogo* ownerLogo_;
	class UB2UISettingAccount* ownerSetting_;
	void SetOwnerUI(class  UB2UIPatch* owner);
	void SetOwnerLogoUI(class  UB2UIActionLogo* ownerLogo);
	void SetOwnerSettingUI(class  UB2UISettingAccount* ownerSetting);
	void SetPopupType(EPopupType eType);
	void SetDownLoadVolume(int64 nVolume);

	void  SetResponsCode(int32 nResponsCode);
	void  SetRewardItem(int32 nitemID, int32 nCnt);
	void SetNeedDiskSize(FString DiskSize);
	
private:
	TWeakObjectPtr<UB2Button> BTN_Positive;
	TWeakObjectPtr<UB2Button> BTN_Negative;

	TWeakObjectPtr<UTextBlock> TXT_DownloadFileValue;
	TWeakObjectPtr<class UB2UIRewardIcon>	UIP_RewardIcon;

	TWeakObjectPtr<UTextBlock> TXT_NetWorkFail;
	TWeakObjectPtr<UTextBlock> TXT_FileError;
	TWeakObjectPtr<UTextBlock> TXT_FileErrorSub;
	TWeakObjectPtr<UTextBlock> TXT_NotEnoughDiskSize;

	EPopupType m_PopupType;
	UFUNCTION(BlueprintCallable, Category = "DCLFront")
		void OnClickNegative();
	UFUNCTION(BlueprintCallable, Category = "DCLFront")
		void OnClickPositive();
	UFUNCTION(BlueprintCallable, Category = "DCLFront")
		void OnClickKakaoExitBtn();
	
};
