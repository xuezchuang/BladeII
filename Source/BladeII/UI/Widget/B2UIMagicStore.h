// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "../../Common/EventSubsystem.h"
#include "B2UIMagicStore.generated.h"


UCLASS()
class BLADEII_API UB2UIMagicStore : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:

	UB2UIMagicStore(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SubscribeEvents();
	void UnsubscribeEvents();

	virtual void OnOpen(bool RightNow = false);
	virtual void OnClose(bool RightNow = false);

public:
	void SetItemList(bool bAnimPlay = true);
	void SetRemainTime();

	FORCEINLINE bool GetIsWaitingRequest() { return WaitingRequest; }

private:
	void UpdateRemainTime();

	void RequestGetMagicShop();

	template<class T>
	void ResponseMagicShopData(const T& MagicShopPtr);

	void ResponseReplaceCurrentGem(const FB2ResponseRenewalMagicShopPtr& MagicShopPtr);
	void ResponseBuyMagicShopProduct(const FB2ResponsePurchaseMagicShopPtr& MagicShopPtr);

	void ResponseRenewalMagicShopErrorPopup();
	void ResponsePurchaseMagicShopErrorPopup();

	void ConditionalCreateMagicStoreRefreshPopup();
	void DestroyMagicStoreRefreshPopup();
	void ConditionalCreateMagicStoreBuyConfirmPopup();
	void DestroyMagicStoreBuyConfirmPopup();

	void OnClickBuyButtonDelegate(int32 ProductID);
	void SetMagicStoreBuyConfirmPopup(int32 ProducID);

protected:
	UFUNCTION()
		void OnClickBTN_Refresh();

	UFUNCTION()
		void OpenMagicStoreRefreshPopup();
	UFUNCTION()
		void CloseMagicStoreRefreshPopup();
	UFUNCTION()
		void BuyMagicStoreRefreshPopup();

	UFUNCTION()
		void OpenMagicStoreBuyConfirmPopup();
	UFUNCTION()
		void CloseMagicStoreBuyConfirmPopup();
	UFUNCTION()
		void BuyMagicStoreBuyConfirmPopup();

private:
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UIMsgPopupMagicStoreRefresh> UIP_MagicStoreRefresh_Class;
	UPROPERTY(Transient)
		UB2UIMsgPopupMagicStoreRefresh* Created_UIP_MagicStoreRefresh;

	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UIMsgPopupMagicStoreBuyConfirm> UIP_MagicStoreBuyConfirm_Class;
	UPROPERTY(Transient)
		UB2UIMsgPopupMagicStoreBuyConfirm* Created_UIP_MagicStoreBuyConfirm;

private:

	TWeakObjectPtr<UCanvasPanel>	X_CP_MagicStoreRefresh_Panel;
	TWeakObjectPtr<UCanvasPanel>	X_CP_MagicStoreBuyConfirm_Panel;

	TWeakObjectPtr<UTextBlock>		TB_RemainTitle;
	TWeakObjectPtr<UTextBlock>		TB_RemainTime;
	TWeakObjectPtr<UB2Button>		BTN_Refresh;
	TWeakObjectPtr<UTextBlock>		TB_Refresh;

	TWeakObjectPtr<UTextBlock>		TB_Banner;

	TArray<TWeakObjectPtr<class UB2UIMagicStoreItemSlot>> MagicStoreItemSlotList;

private:
	TArray<std::shared_ptr<issue>>	Issues;

	FDateTime						NextRenewalTime;
	bool							StartTimeTick;
	bool							WaitingRequest;

	int32							BuyItemProductID;

};
