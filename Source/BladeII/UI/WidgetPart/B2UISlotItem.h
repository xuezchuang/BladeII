// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UISlotIcon.h"
#include "B2UISlotItem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSlotItemClickDelgate, UB2UISlotItem*, ClickedItem);


// For Stage reward item info preview at BattleStageInfo.
struct FB2UISlotItemStageRewardInfo
{
	FB2UISlotItemStageRewardInfo()
	{
		PrimPointMinValue = 0.0f;
		PrimPointMaxValue = 0.0f;
		RandomOptionCount = 0;
	}

	float PrimPointMinValue;
	float PrimPointMaxValue;
	int32 RandomOptionCount;
};

/*
* In the form of slots and indicates the item.
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISlotItem : public UB2UISlotIcon
{
	GENERATED_BODY()

public:
	// UWidget interface
	virtual void SynchronizeProperties() override;

	UB2UISlotItem(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void BindDoc(const struct FB2Item& ItemStruct, int32 quantity =-1);
	void BindDoc(const FB2RewardItemPreviewInfo& ItemPreviewInfoStruct);
	void BindByRefID(int32 RefID);
	void BindEther(const struct FB2Item& ItemStruct);
	void BindEther(const struct FB2AetherItemPreviewInfo& ItemPreviewInfoStruct);
	void BindItemInfoData(struct FSingleItemInfoData* InItemInfoData, int32 ItemQuality);
	void UnbindDoc();

	//Item info
	void SetItemLevel(int32 InValue);
	void ShowItemLevel(bool bShow);
	void SetItemName(const FText& InText);
	void SetItemType(EItemClass ItemType);
	void SetStarGrade(const FB2Item& InValue);
	void SetStarGrade(const int32 InItemGrade, const bool InIsSurpass);
	void SetStageInfoStarGrade(int32 MinValue, int32 MaxValue);
	void SetEnhanceLevel(int32 InValue, bool showQuantity = false);
	void SetPrimaryPoint(int32 InValue);
	void SetPrimaryPointType(EItemPrimaryPointType InType);

	//[@AKI, 170617] UIP_Stars 甫 焊咯临瘤 救焊咯 临瘤. 焊胶 炼阿篮 喊殿鞭捞 绝绢辑 Collapsed肺 秦林扁 困秦 眠啊 窃
	void SetVisibleStar(ESlateVisibility _visible);
	void SetVisibleStageInfoItemIcon(ESlateVisibility _visible, bool isBossItem);

	//count
	void SetCount(const int32 InNum);
	int32 GetCount() const { return Count; }
	void ShowCount(const bool bShow);
	void SetCountTextColor(int32 ItemRefID);

	//select
	void SetSelected(bool InbSelected);
	bool GetSelected() const { return bSelected; }

	FORCEINLINE void SetExtraStageRewardInfo(const FB2UISlotItemStageRewardInfo& InInfo) { ExtraStageRewardInfo = InInfo; }
	FB2RewardItemPreviewInfo GetStageRewardPreviewInfo();

	/** Get currently active BG panel material according to item grade. 
	 * It assumes specific way of widget set-up */
	UMaterialInterface* GetActiveItemBGPanelMaterial() const;
	UMaterialInterface* GetActiveEtherItemBGPanelMaterial() const;

	/** True sets background and icon image invisible. It is meant to be used when the icon body is displayed using 3D mesh. */
	void SetShowOverlayInfoOnly(bool bOverlayInfoOnly);

	int32 GetItemRefId() const { return BindedStruct.ItemRefID; }
	EPCClass GetItemAllowedCharacter() const { return IntToPCClass(BindedStruct.AllowedPCClass); }
	EItemClass GetItemClass() const { return BindedStruct.ItemClass; }
	int8 GetItemStarGrade() const { return BindedStruct.StarGrade; }
	bool IsConsumables() const { return (BindedStruct.InventoryType == EItemInvenType::EIIVT_Consumables); }

	void SetItemCountTextColor(FSlateColor sColor);
	void SetEnableReadingGlasses(bool Enable);

	void SetBTNArea(bool InValue);
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII SlotItem")
	void SetPrimaryPointType_BP(EItemPrimaryPointType InType);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII SlotItem")
	void SetSelected_BP(bool InSelected);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII SlotItem")
	void SetStarGrade_BP(int32 InValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII SlotItem")
	void SetQuality_BP(int32 InQuality);

	struct FB2Item& GetDocStruct() { return BindedStruct; }
protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

	void SetDocStruct(const struct FB2Item& ItemStruct)			{ BindedStruct = ItemStruct; }
		
	virtual void OnSetupManualScrollBoxSender(UB2UnitedWidgetBase* InReceiver, UB2ScrollBox* HandlingScrollBox) override;

	// Button input handling interfaces for ManualScrollBox feature.
	virtual void OnClickedSenderBTNArea() override;
	virtual void OnPressedSenderBTNArea() override;
	virtual void OnReleasedSenderBTNArea() override;

	//=================================================== Btn
	//UFUNCTION()
	virtual void OnClickBtnArea() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Bladeii")
	void OnClickedBtnAreaBP();
	UFUNCTION(BlueprintImplementableEvent, Category = "Bladeii")
	void OnPressedBtnAreaBP();
	UFUNCTION(BlueprintImplementableEvent, Category = "Bladeii")
	void OnReleasedBtnAreaBP();

	//==================================================== Doc Delegate


public:
	FOnSlotItemClickDelgate OnSlotItemClickDelgate;

	/* Number of items. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII SlotItem")
	int32 Count;

	/*Whether or not put an x in front of the number counted. Ex) x100 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII SlotItem")
	bool bUseCountPrefix = false;

	/*If the count is zero whether to hide the count text. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII SlotItem")
	bool bCount_0_Text_Invisible;

	/*If the count is zero whether to hide the icon image. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII SlotItem")
	bool bCount_0_Icon_Invisible;

	/** See UB2ItemMiscInfo::GetItemIconBGMtrl, about bSimpleBG argument */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII SlotItem")
	bool bUseSimpleBGImage;

protected:
	TWeakObjectPtr<UImage>						    IMG_Reading_Glasses;
	TWeakObjectPtr<UImage>							IMG_ItemBGPanel;
	TWeakObjectPtr<UImage>							IMG_ItemQualityBG;
	TWeakObjectPtr<UTextBlock>						TB_ItemName;
	TWeakObjectPtr<UTextBlock>						TB_ItemLevel;
	TWeakObjectPtr<UTextBlock>						TB_ItemCount;
	TWeakObjectPtr<UTextBlock>						TB_EnhanceLevel;
	TWeakObjectPtr<UTextBlock>						TB_PrimaryPoint;
	TWeakObjectPtr<UPanelWidget>					O_Selected;

	TWeakObjectPtr<class UB2UIStarGradePart>	UIP_Stars;
	// Stage Info 俊辑 荤侩
	TWeakObjectPtr<UImage>							IMG_StarIcon;
	TWeakObjectPtr<UImage>							IMG_BossItemIcon;
	TWeakObjectPtr<UTextBlock>						TB_StarCount;

	// Ether Position
	TWeakObjectPtr<UImage>							IMG_EtherPos;

	TWeakObjectPtr<UPanelWidget> CP_Stars;
	TWeakObjectPtr<UPanelWidget> OV_ItemQualitySet;

	TWeakObjectPtr<UTextBlock> TB_ItemQuality;
	TWeakObjectPtr<UImage> IMG_ExtraSlotEffect;
	TWeakObjectPtr<UTextBlock> TB_DevRefItemId;


private:
	struct FB2Item BindedStruct;

	bool bSelected;
	bool bShowCount;

	// For Stage reward item info preview at BattleStageInfo.
	FB2UISlotItemStageRewardInfo ExtraStageRewardInfo;
};
