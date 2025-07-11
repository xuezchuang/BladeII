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
//	void SetItem(int32 RefId			//소모품 RefID
//		, int32 minQuantity = -1		//소모품 최소 갯수
//		, int32 maxQuantity = -1		//소모품 최대 갯수
//	);
//	void SetItem(int32 groupID			//장비 접두어
//		, EPCClass equipClass			//장비 클래스
//		, int32 equipCategory			//장비 종류
//		, int32 equipPlace				//장비 장착 부위
//		, int32 starGrade				//장비 별등급
//		, int32 quality					//장비 품질
//		, int32 enhancedLevel			//장비 강화레벨
//		, int32 surpassLevel			//장비 초월레벨
//	);
//	void SetItem(int32 RefId, bool showQuantity);
//
//	void SetResultPannel(class UB2UISealBoxResultSimple* mainResult);
//
//// 	장비 RefID 에 대한 정보
//// 	(왼쪽을 첫번째로 한다)
//// 	첫째 자리:		장비인지 여부 (1이면 장비)	(CommonStruct.h의 3278번 줄 참고)
//// 	둘째 자리:		클래스 (0이면 검투사)		(EPCClass 클래스 참고)
//// 	셋째 자리:		성 10의 자리
//// 	넷째 자리:		착용 부위 (0이면 무기)		(EItemEquipPlace 클래스 참고)
//// 	다섯째 자리:	성 1의 자리
//// 	여섯째 자리:	접두어 10의 자리			(b2network::B2GroupPrefix 네임스페이스 참고)
//// 	일곱째 자리:	접두어 1의 자리
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
