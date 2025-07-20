// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/B2UIWidget.h"
#include "CommonStruct.h"
#include "B2UIEnum.h"
#include <memory>
#include "B2UIItemForgePopup.generated.h"

/**
 * 
 */

struct TargetData 
{
	int32 RefID;
	int32 Count;

	TargetData(int32 RefId, int32 count)
	{
		RefID = RefId;
		Count = count;
	}
};

class ItemForgeSettingDatas 
{
public:
	ItemForgeSettingDatas() {}
	ItemForgeSettingDatas(EItemForgeSlot SType, int32 FID):
		FactoryID(FID),SlotType(SType){}

	EPCClass CharacterType;				// 某腐磐 鸥涝
	int32 FactoryID;					// 措眉 抛捞喉 蜡聪农 内靛
	int32 MakeCount;					// 力累 冉荐
	int32 MainItemRefID;				// 急琶 啊瓷茄 皋牢 酒捞袍
	int32 MainItemCount;				// 急琶 啊瓷茄 皋牢 酒捞袍狼 鞘夸 俺荐
	EItemForgeSlot SlotType;			// 抛捞喉 拿烦 急琶
	TArray<TargetData> ResourceItem;	// 措眉 抛捞喉 单捞磐
	TArray<FB2Item> HoldList;			// 蜡历 焊蜡 酒捞袍 府胶飘

public:

	void SetDataSet(EItemForgeSlot SType) {
		SlotType = SType;
	}
	// 单捞磐 技泼
	void SetDataSet(EPCClass CharType,int32 FID, EItemForgeSlot SType,
		int32 Makecount, int32 MainItem, int32 MainItemcount) {
		CharacterType = CharType;
		SetDataSet(SType);
		FactoryID = FID;
		MakeCount = Makecount;
		MainItemRefID = MainItem;
		MainItemCount = MainItemcount;
	}
};


UCLASS()
class BLADEII_API UB2UIItemForgePopup : public UB2UIWidget
{
	GENERATED_BODY()
	
public:
	
	TWeakObjectPtr<UTextBlock>	TB_Title;

	TWeakObjectPtr<class UB2ScrollBox>	SB_Item;


	TSubclassOf<class UB2DynItemIcon_ItemForge>   TabButtonBP;


	TWeakObjectPtr<class UB2Button>	BTN_Cancel;

	ItemForgeSettingDatas ItemForgeDataSet;		// 力累家 包访 单捞磐 悸

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UB2DynItemIcon_ItemForge> result;

	TWeakObjectPtr<class UHorizontalBox> HB_Items;
	TWeakObjectPtr<class UScrollBox> SB_ItemTargets;
	TWeakObjectPtr<class UB2Button>				BTN_Select;
	TWeakObjectPtr<UTextBlock>					TB_Confirm;
	class FLocalCharacterData*					CachedCharacterStore;	// 烙矫 历厘等 某腐磐 单捞磐 历厘家
private:
	FB2Item										SelectedItem;			// 急琶等 酒捞袍
	int32										Itemforge_SelectTicket;	// SelectedItem 技泼阑 困茄 Event System

	bool										bSubscribed;			// Event System 技泼 咯何
	//TArray<std::shared_ptr<issue>>				Issues;					// Event System 窃荐 沏磐

	class UB2UIItemForgeIngredientList* ItemForgeIngredientUI;			// 惑困 UI 努贰胶

	int32 requiredItemEnhanceLevel;
	int32 requiredItemLevel;
public:
	virtual void Init() override;		// B2UI 扁夯 Init
	// ItemForgePopup俊 鞘夸茄 葛电 单捞磐 技泼
	void InitializeForgePopup(EPCClass CharType, int32 FID, EItemForgeSlot SType,int32 Makecount,int32 MainItem ,int32 MainItemCount ,class UB2UIItemForgeIngredientList* MainUI, int32 enhanceLevel=0, int32 itemLevel=0);
	// EventSystem Sub,UnSub
	void SubscribeEvent();
	void UnSubscribeEvent();
public:
	UFUNCTION()
	void OnClickBTN_Cancel();
	UFUNCTION()
	void OnClickBTNSelect();
protected:
	// B2UI 扁夯 扁瓷
	virtual void CacheAssets()override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

	
	
};
