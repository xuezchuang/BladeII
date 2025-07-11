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
//	void SetItem(int32 RefId			//�Ҹ�ǰ RefID
//		, int32 minQuantity = -1		//�Ҹ�ǰ �ּ� ����
//		, int32 maxQuantity = -1		//�Ҹ�ǰ �ִ� ����
//	);
//	void SetItem(int32 groupID			//��� ���ξ�
//		, EPCClass equipClass			//��� Ŭ����
//		, int32 equipCategory			//��� ����
//		, int32 equipPlace				//��� ���� ����
//		, int32 starGrade				//��� �����
//		, int32 quality					//��� ǰ��
//		, int32 enhancedLevel			//��� ��ȭ����
//		, int32 surpassLevel			//��� �ʿ�����
//	);
//	void SetItem(int32 RefId, bool showQuantity);
//
//	void SetResultPannel(class UB2UISealBoxResultSimple* mainResult);
//
//// 	��� RefID �� ���� ����
//// 	(������ ù��°�� �Ѵ�)
//// 	ù° �ڸ�:		������� ���� (1�̸� ���)	(CommonStruct.h�� 3278�� �� ����)
//// 	��° �ڸ�:		Ŭ���� (0�̸� ������)		(EPCClass Ŭ���� ����)
//// 	��° �ڸ�:		�� 10�� �ڸ�
//// 	��° �ڸ�:		���� ���� (0�̸� ����)		(EItemEquipPlace Ŭ���� ����)
//// 	�ټ�° �ڸ�:	�� 1�� �ڸ�
//// 	����° �ڸ�:	���ξ� 10�� �ڸ�			(b2network::B2GroupPrefix ���ӽ����̽� ����)
//// 	�ϰ�° �ڸ�:	���ξ� 1�� �ڸ�
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
