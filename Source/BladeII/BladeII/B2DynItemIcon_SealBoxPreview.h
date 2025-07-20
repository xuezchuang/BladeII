// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UI/DJLegacy/B2DynItemIcon.h"
//#include "B2UIEnum.h"
//#include "B2DynItemIcon_SealBoxPreview.generated.h"

///**
// * 
// */
//UCLASS()
//class BLADEII_API UB2DynItemIcon_SealBoxPreview : public UB2DynItemIcon
//{
//	GENERATED_BODY()
//	
//		TWeakObjectPtr<class UB2Button>				BTN_Area;
//
//
//
//	TWeakObjectPtr<UImage>						MainIconImage;
//
//	TWeakObjectPtr<UTextBlock>					ItemAmountText;
//
//	TWeakObjectPtr<UTextBlock>					ItemLevelText;
//
//	TWeakObjectPtr<UTextBlock>					ItemEnhanceLevelText;
//
//	TWeakObjectPtr<UImage>						ItemLockedStateImage_Store;
//	TWeakObjectPtr<UImage>						ItemLockedStateImage_Detail;
//
//	TWeakObjectPtr<UImage>						ItemQualityBGImage;
//	TWeakObjectPtr<UTextBlock>						ItemQualityTextBlock;
//
//	TWeakObjectPtr<UTextBlock>					TB_Preset;
//	TWeakObjectPtr<UImage>						IMG_EquippedCheck;
//
//	TWeakObjectPtr<UImage>						DisabledSlotImage;
//
//	TWeakObjectPtr<class UB2UIWidgetBase>		UIP_RedDot;
//
//	TWeakObjectPtr<UTextBlock>					ItemNameText;
//
//
//
//	TWeakObjectPtr<UPanelWidget>				P_EquipInfo;
//	//TWeakObjectPtr<UImage>						IMG_PresetBG;
//	TWeakObjectPtr<UImage>						BackgroundImage;
//	TWeakObjectPtr<UImage>						IMG_CanNotSelect;
//	TWeakObjectPtr<UOverlay>					OV_ItemQualitySet;
//	TWeakObjectPtr<UOverlay>					OV_Selected;
//	TWeakObjectPtr<UOverlay>					OV_LockedImages;
//
//	TWeakObjectPtr<UCanvasPanel>	CP_Main;
//	TWeakObjectPtr<UCanvasPanel>	CP_BackGroundOnly;
//
//private:
//	EItemForgeSlot								MaterialSlot;
//	class FLocalCharacterData*					CachedCharacterStore;
//
//public:
//
//	UB2DynItemIcon_SealBoxPreview(const FObjectInitializer& ObjectInitializer);
//	virtual void CacheAssets() override;
//
//	virtual void UpdateItemData(const FB2Item& InItem) override;
//	void SetItem(int32 RefId			//家葛前 RefID
//		, int32 minQuantity = -1		//家葛前 弥家 肮荐
//		, int32 maxQuantity = -1		//家葛前 弥措 肮荐
//	);
//	void SetItem(int32 groupID			//厘厚 立滴绢
//		, EPCClass equipClass			//厘厚 努贰胶
//		, int32 equipCategory			//厘厚 辆幅
//		, int32 equipPlace				//厘厚 厘馒 何困
//		, int32 starGrade				//厘厚 喊殿鞭
//		, int32 quality					//厘厚 前龙
//		, int32 enhancedLevel			//厘厚 碍拳饭骇
//		, int32 surpassLevel			//厘厚 檬岿饭骇
//	);
//	void SetItem(int32 RefId, bool showQuantity);
//
//	void SetResultPannel(class UB2UISealBoxResultSimple* mainResult);
//
//// 	厘厚 RefID 俊 措茄 沥焊
//// 	(哭率阑 霉锅掳肺 茄促)
//// 	霉掳 磊府:		厘厚牢瘤 咯何 (1捞搁 厘厚)	(CommonStruct.h狼 3278锅 临 曼绊)
//// 	笛掳 磊府:		努贰胶 (0捞搁 八捧荤)		(EPCClass 努贰胶 曼绊)
//// 	悸掳 磊府:		己 10狼 磊府
//// 	齿掳 磊府:		馒侩 何困 (0捞搁 公扁)		(EItemEquipPlace 努贰胶 曼绊)
//// 	促几掳 磊府:	己 1狼 磊府
//// 	咯几掳 磊府:	立滴绢 10狼 磊府			(b2network::B2GroupPrefix 匙烙胶其捞胶 曼绊)
//// 	老蚌掳 磊府:	立滴绢 1狼 磊府
//	
//
//	virtual void SetBackgroundImageFromGrade(int32 InItemGrade, bool bForConsumables) override;
//
//	// Button input handling interfaces for ManualScrollBox feature.
//	virtual void OnClickedSenderBTNArea() override;
//	virtual void OnPressedSenderBTNArea() override;
//	virtual void OnReleasedSenderBTNArea() override;
//
//	//void InitItemForgeSlot(EItemForgeSlot EIFS, EItemForgeItemIconUsage UseType, int32 FactoryId, int32 enhanceLevel = 0, int32 itemLevel = 0);
//	void SetMaterialSlotType(EItemForgeSlot EIFS) { MaterialSlot = EIFS; }
//
//	void ShowJustEmptySlot();
//
//	ESlateVisibility GetDisabledSlotImageVisibility() const;
//	bool bIsNativeItemLocked;
//	UFUNCTION()
//		void OnClickBTNArea();
//	UFUNCTION()
//		void AskForUnlockItemPopupCommon();
//
//	void SetOV_SelectedVisibility(ESlateVisibility VisibleType) { OV_Selected->SetVisibility(VisibleType); }
//protected:
//	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
//		void DoResultAnimationThingy(int32 effectGrade);
//
//	UFUNCTION(BlueprintCallable, Category = "BladeII")
//		void FinishedResultAnimationThingy();
//
//private:
//
//	FText GetItemAmountText();
//	FText GetItemAmountText(int32 minValue, int32 maxValue);
//
//	FText GetItemLevelText() const;
//	FText GetItemEnhanceLevelText() const;
//
//	FText SetItemEnhanceLevelText(int32 enhanceLevel = 0) const;
//	FText SetItemLevelText(int32 itemLevel = 1) const;
//
//	int32 GenerateRefID(int32 groupID, EPCClass equipClass, int32 equipPlace, int32 equipCategory, int32 starGrade=0);
//
//	class UB2UISealBoxResultSimple* resultPannel;
//};
