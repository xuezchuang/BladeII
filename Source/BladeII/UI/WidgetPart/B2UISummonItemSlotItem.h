// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIStoreProductCost.h"
#include "B2UISummonItemSlotItem.generated.h"

namespace ESummonItemSlotIndex
{
	const int32 SummonItem_Gem = 0;
	const int32 SummonItem_FriendShip = 1;
	const int32 Max = 2;
};

enum ESummonButtonType : uint8
{
	SummonButtonType_Ten = 0,
	SummonButtonType_One = 1,

	Max,
};

struct FB2UISummonButton
{
	TWeakObjectPtr<UB2UIWidgetBase>				UIP_RedDot;
	TWeakObjectPtr<UTextBlock>					TB_Summon;
	TWeakObjectPtr<UB2RichTextBlock>			RTB_BuyCount;
	TWeakObjectPtr<UTextBlock>					TB_SummonDesc;
	TWeakObjectPtr<UB2Button>					BTN_Summon;
	TWeakObjectPtr<UB2UIStoreProductCost>		UIP_Cost;
	TWeakObjectPtr<class UB2UIStoreEventLabel>	UIP_Store_Event_Label;

	ESummonButtonType	SummonButtonType;		// Ten, One
	int32				SlotHashKey;			// HashKey
	int64				LeftTime;
};


UCLASS()
class BLADEII_API UB2UISummonItemSlotItem : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:

	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

public:

	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void UpdateLeftTime();
	void UpdateRedDot();

	void SetSummonItemSlot(ESummonItemCost InCostType);
	void SetGradeRange(int32 InMin, int32 InMax);
	void SetSummonItem(ESummonButtonType InButtonType, int32 InSlotHashKey, int64 InLeftTime);
	void SetSummonButtonItem(ESummonButtonType InButtonType, ESummonItemCost InCostType, int32 InCostCount, int32 InCalculatedCost, int32 InCount, int32 MaxCount, bool IsTenLotteryTicket);

	void SetLimitCount(ESummonButtonType InButtonType, int32 InLimitCount = 0, int32 InMaxCount = 0);
	void SetVisibleFreeDesc(ESummonButtonType InButtonType, bool bVisible = false);
	void SetLotterySale(ESummonButtonType InButtonType);
	
	void StartFreeLeftTime(bool ToToggle);
	void SetFreeLeftTime();
	FText GetLeftTimeText(int64 Seconds);

	bool IsFreeSummonableSlot(int32 SlotHashKey);

protected:

	UFUNCTION()
		void OnClickBTN_TenSummon();
	UFUNCTION()
		void OnClickBTN_OneSummon();

private:
	struct FSummonBuyInfo
	{
		int32 CostCount;
		int32 CalCost;
		EStoreItemCost StoreItemCost;
	};

	void OpenErrorPopup(EStoreItemCost InCostType);
	bool IsSummonItemBuy(FSummonBuyInfo& InSummonBuyInfo);

	TWeakObjectPtr<UTextBlock>	TB_Title;

	TWeakObjectPtr<UOverlay>	OV_GemBox;
	TWeakObjectPtr<UOverlay>	OV_FriendShipBox;

	TWeakObjectPtr<UOverlay>	OV_EableMileage;
	TWeakObjectPtr<UTextBlock>	TB_EableMileage;

	TWeakObjectPtr<UTextBlock>	TB_MinGrade;
	TWeakObjectPtr<UTextBlock>	TB_MaxGrade;
	TWeakObjectPtr<UTextBlock>	TB_EnableGrade;

	TWeakObjectPtr<UTextBlock>  TB_LotteryNotice;

	TArray<FB2UISummonButton>	SummonButtonArray;
	bool						bStartCountTime;

	TMap<ESummonButtonType, FSummonBuyInfo> SummonBuyInfo;
};
