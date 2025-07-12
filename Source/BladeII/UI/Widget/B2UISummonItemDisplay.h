// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2LobbyUI_ItemDetailPostOp.h"
#include "B2UIBackWidget.h"
#include "../../BladeII.h"
#include "B2UISummonItemDisplay.generated.h"

DECLARE_DELEGATE(FOnFlipped)
DECLARE_DELEGATE_OneParam(FOnViewDetailItem, const FB2Item&);

enum class ESummonedItemFrom : uint8
{
	SummonItemShop,
	LotteryMail,
};

UCLASS()
class BLADEII_API UB2UISummonItemDisplaySlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	UB2UISummonItemDisplaySlot(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void BeginDestroy() override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	void UpdateItemData(const FB2Item& InItem);
	void SetWorldTransform(const FVector& WorldLoc);
	void SetWorldTransform(const FTransform& InTransform);
	void SetWorldTransform(const FMatrix& InMatrix);
	void SetCameraRotator(FRotator InRotator);

	FVector GetWorldLocation() {
		return ItemCardMatrix.GetOrigin();}
	FVector2D GetScale() {	return FVector2D(ItemCardMatrix.GetScaleVector().X / 2.f, ItemCardMatrix.GetScaleVector().Y / 2.f);	}
	void BindBTNFlip(const FOnFlipped& Delegate);
	void BindBTNViewDetail(const FOnViewDetailItem& Delegate);

	void DisplaySlotHidden(bool IsHidden);

	bool IsFlipped() { return bFlipped; }	

	void CreateCard_FrontFX();
	void DeleteCard_FrontFX();	

	void CreateCard_BackFX();
	void DeleteCard_BackFX();

	void ShowItemCard_Front();
	void ShowItemCard_Back();

	void SetAttachBoneName(FName BoneName){ SoftAttachedBoneName = BoneName; }
	FName GetAttachedBoneName(){ return SoftAttachedBoneName; }

public:
	UFUNCTION()
	void OnClickBTN_Flip();

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
	void ShowVisibility_ItemDataBP(bool bShow);

private:
	FTransform GetTransform();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class UParticleSystem*> ItemCardFXs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class UParticleSystem*> ItemCardBackFXs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bShowDetatil;


private:
	TWeakObjectPtr<UB2Button> BTN_Flip;
	TWeakObjectPtr<class UB2UISlotItem> UIP_ItemDetail;

	bool bFlipped;

	FOnFlipped OnFlip;
	FOnViewDetailItem OnViewDetail;

	FB2Item ItemInfoData;
	FName SoftAttachedBoneName;

	FVector		ItemCardWorldLoc;
	FMatrix		ItemCardMatrix;
	FRotator	CameraRotator;
	
	UPROPERTY(Transient)
	class UParticleSystemComponent* ItemCardFX;
	UPROPERTY(Transient)
	class UParticleSystemComponent* ItemCardBackFX;
	//TWeakObjectPtr<class UParticleSystemComponent> ItemCardFX;
	//TWeakObjectPtr<class UParticleSystemComponent> ItemCardBackFX;

	static const FName Name_FxMID_ItemBGPanel;
	static const FName Name_FxMID_ItemIcon;	
};

UCLASS()
class BLADEII_API UB2UISummonItemDisplay : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	float CardSlotSizeScale_Single;
	UPROPERTY(EditAnywhere)
	float CardSlotSizeScale_Multiple10;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;
public:

	UB2UISummonItemDisplay(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void UpdateStaticText() override;

	void ChangeSummonItemCardMatineeActorToFX(const TArray<FB2Item>& Items, class AMatineeActor* Matinee);
	void UpdateLocationSummonItemFXs();
	void UpdateLocationSummonItemFX(USkeletalMeshComponent* Mesh, int32 Index);
	void UpdateLocationSummonItemUIs();
	void EnterShopException();
 	FORCEINLINE	void SetItemsCount(int32 InItemNumber) { OpenCardNumber = InItemNumber; }
	FORCEINLINE void SetItemMaxNumber(int32 InitemMaxNumber) { OpenCardNumber = OpenCardMaxNumber = InitemMaxNumber; }
	FORCEINLINE int32 GetItemsCount() { return OpenCardNumber; }


	void CheckFlippedCardIfFinished();
	void NotifyOpenCard(int32 Inindex);


	void SetSummonedItemFrom(ESummonedItemFrom InFrom) { SummonedItemFrom = InFrom; }
	void SetOpenCardState(bool IsOpenState) { bCardOpenTouch = IsOpenState; }
	void OnFinishedSummonItemDisplayResultMatinee();

	void UpdateButtonState(int32 CardNumber);

	TUTORIAL void Tutorial_SetSummonSlotFix();

	void InventoryFullSimplePopup(EPCClass PCClass);

	float GetCardSlotSizeScale() const;

	UFUNCTION()
	void OnStartDrawShopLottery(const int32 nLotteryCount);

	EStoreItemCost GetStoreItemCostType();
	bool OpenAppStoreReview();

protected:
	UFUNCTION()
	void OnClickBTN_Confirm();

	UFUNCTION()
	void OnClickBTN_SummonOneMore();

	void OpenDetailSummonedItem(const FB2Item& InItem);
	UFUNCTION()
	void CloseDetailSummonedItem();

	UFUNCTION()
	void OnClickBTN_OpenTouch();

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void PlayAnimSummonOneFlip();

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void PlaySoundOpenSingleCard();

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void PlaySoundOpenEachCard();

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void PlaySoundOpenAllCard();
	
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void OnStartDrawShopLottery_BP(const int32 nLotteryCount);

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
	void SetSummonOneMoreCost(int32 Cost, int32 CalculatedCost, EStoreItemCost CostType);

private:
	TWeakObjectPtr<class UB2LobbyUI_ItemDetailPostOp> UIP_SummonedDetail;

	TArray<TWeakObjectPtr<class UB2UISummonItemDisplaySlot>> UIP_SummonedItems;

	TWeakObjectPtr<UCanvasPanel>	CP_Confirm;
	TWeakObjectPtr<UB2Button>		BTN_Confirm;
	TWeakObjectPtr<UTextBlock>		TB_Confirm;
	TWeakObjectPtr<UTextBlock>		TB_ItemCount;

	TWeakObjectPtr<UOverlay>		OV_SummonItemDetail;
	TWeakObjectPtr<UOverlay>		OV_SummonItems;

	TWeakObjectPtr<UOverlay>					OV_EssenceDetail;
	TWeakObjectPtr<class UB2LobbyUI_ItemDetail>	Lobby_ItemDetailCommonBP;
	TWeakObjectPtr<UB2Button>					BTN_EssenceClose;

	TWeakObjectPtr<UCanvasPanel>	CP_OneMore;
	TWeakObjectPtr<UB2Button>		BTN_OneMore;
	TWeakObjectPtr<UB2Button>		BTN_OpenTouch;
	TWeakObjectPtr<class UB2UIStoreProductCost>	UIP_OneMoreCost;

	TWeakObjectPtr<UOverlay>		OV_SaleForSummon;
	TWeakObjectPtr<UTextBlock>		TB_SaleForSummon;

	TWeakObjectPtr<class ASkeletalMeshActor> SummonBoxActor;
	TWeakObjectPtr<class ASkeletalMeshActor> SummonBoxActorBG;

	TWeakObjectPtr<UTextBlock>		TB_OneMore;

	int32 OpenCardNumber;
	int32 OpenCardMaxNumber;
	bool bFinishedFlip;

	bool bCardOpenTouch;

	bool bNeedCardLocationUpdate;
	bool bCardResult;

	bool bOpenCards;
	int32 OpenCardNumberResult;
	float bOpenCardsTime;

	bool bStoreFinish;
	
	ESummonedItemFrom SummonedItemFrom;

	uint32 EnterShopTicket;
	TWeakObjectPtr<UPanelWidget> CP_Gem;
	TWeakObjectPtr<UTextBlock> TB_GemAmount;
};
