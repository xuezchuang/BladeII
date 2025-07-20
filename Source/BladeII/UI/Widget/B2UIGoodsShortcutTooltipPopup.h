// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIGoodsShortcutTooltipPopup.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIGoodsShortcutTooltipPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	
//Functions
public:
	UB2UIGoodsShortcutTooltipPopup(const FObjectInitializer& ObjectInitializer);
	
	virtual void Init() override;
	
	UFUNCTION()
	void CacheAssets();
	//virtual void PostLoad() override;
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
	UFUNCTION()
	void SetItemInfo(uint32 _IteminfoIdx, const EGoodsButtonType& _type);
	void SetOpenStageItemInfo(int32 _ItemRefID, TArray<bool> _IsContentsOpened, const EGoodsButtonType& _GoodsButtonType);

	FVector2D GetUISize() const;
	void SetSlotPosition(const FVector2D& _Pos);
	FVector2D GetSlotAnchors() const;


	UFUNCTION()
	void DestroySelfUsingUIManager();

protected:
	UFUNCTION()
	void SetGoodsIcon(int32 _GoodsIconID);
	UFUNCTION()
	void SetGoodsNameText(int32 _GoodsNameStringID);
	UFUNCTION()
	void SetGoodsInfoText(int32 _GoodsInfoStringID);
	UFUNCTION()
	void SetGoodsShortageText(int32 _GoodsInfoStringID);
	UFUNCTION()
	void SetGoodsShortageTextColor(const FSlateColor& color);

	void addRow(const struct FSingleGoodsTypeInfoData* _GoodsShortcutInfo);
	void addRowOpenStage(const struct FSingleGoodsTypeInfoData*  _GoodsShortcutInfo, TArray<bool> _IsContentsOpened);

	void AllTextBoxHide();

	struct FSingleGoodsTypeInfoData* GetGoodsTypeFromGoodsTypeInfoObject(FName _GoodsTypeInfoID) const;
	
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Delegate or Event
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	UFUNCTION()
	void OnClickBtnClose();
	UFUNCTION()
	void OnClickBtnModal();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void OnForcedCloseToolTipPopup();
private:

//Variable
public:
protected:
	TWeakObjectPtr<UOverlay> O_ShortcutTooltip;
	TWeakObjectPtr<UOverlay> O_InfoTooltip;
	TWeakObjectPtr<UImage> IMG_BGImage;
	TWeakObjectPtr<UImage> IMG_GoodsIcon;
	TWeakObjectPtr<UTextBlock> TB_Shortage;
	TWeakObjectPtr<UTextBlock> TB_GoodsName;
	TWeakObjectPtr<UTextBlock> TB_GoodsInfo;
	TWeakObjectPtr<UCanvasPanel> CP_ShortcutPage;
	TWeakObjectPtr<UB2ScrollBox> SB_ShortcutPage;
	TWeakObjectPtr<UVerticalBox> VB_ShortcutPage;

	TWeakObjectPtr<UImage> IMG_InfoGoodsIcon;
	TWeakObjectPtr<UTextBlock> TB_InfoGoodsName;
	TWeakObjectPtr<UTextBlock> TB_InfoGoodsString;

	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_Modal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UB2UIGoodsShortcutParts>	UIP_Shortcut_Button;

	uint32 m_IteminfoIdx;
	EGoodsButtonType m_type;
private:
	
	
};
