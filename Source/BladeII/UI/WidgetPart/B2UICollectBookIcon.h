// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UICollectBookIcon.generated.h"

UCLASS()
class BLADEII_API UB2UICollectBookIcon : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	virtual void UpdateStaticText() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void SynchronizeProperties() override;

protected:
	UFUNCTION()
		void OnClickBTN_Area();

	UFUNCTION()
		void OnClickBTN_DetailView();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void OnInitCollectBookIcon(int32 StarGrade);

public:

	void SelectClearCollectBookIcon();
	void HideCollectBookIcon();
	void InitCollectBookIcon(EItemClass ItemClass, int32 ID);
	void SetIconDefaultTemplate(int32 StarGrade, UMaterialInterface* BgMaterial);
	void SetIconMaterial(UMaterialInterface* IconMaterial);
	void SetIconCollectionState(EItemCollectionState InCollectionState);
	void SetNewIcon(bool bVisible);
	void SetEnableSlotEffect(bool bVisible);
	void InitStarGrade(int32 StarGrade);
	void SetStarGrade(const int32 StarGrade, const bool IsSurpass);
	void HideStarGrade();

	FORCEINLINE UMaterialInterface* GetBackgroumdMaterial() const { return DefaultBackgroundMtrl; }
	FORCEINLINE int32 GetGrade() const { return DefaultStarGrade; }
	FORCEINLINE int32 GetSelectedID() const { return SelectedID; }
	FORCEINLINE EItemClass GetSelectedItemClass() const { return SelectedItemClass; }

	void UpdateSelectState(bool bInSelected);
	FORCEINLINE EItemCollectionState GetCollectionState() const { return CollectionState; }

	void SetOwnerCollectBookMainUI(class UB2UICollectBookMain* OwnerUI);

private:
	
	TWeakObjectPtr<UImage> IMG_Background;
	TWeakObjectPtr<UImage> IMG_ItemIcon;
	TWeakObjectPtr<UOverlay> O_RequestReward;
	TWeakObjectPtr<UTextBlock> TB_GetReward;
	TWeakObjectPtr<UB2Button> BTN_Area;
	TWeakObjectPtr<UB2UIWidgetBase> IMG_NewIcon;
	TWeakObjectPtr<UImage> IMG_Selected;
	TWeakObjectPtr<UImage> IMG_UnCollected;
	TWeakObjectPtr<UImage> IMG_Rewarded;
	TWeakObjectPtr<UImage> IMG_ExtraSlotEffect;

	TArray<TWeakObjectPtr<UImage>>	IMG_Stars;

	TWeakObjectPtr<UOverlay>	O_DetailView;
	TWeakObjectPtr<UB2Button>	BTN_DetailView;
	TWeakObjectPtr<UTextBlock>	TB_DetailView;

	TWeakObjectPtr<UScaleBox>	SB_StarGrade;
	TWeakObjectPtr<class UB2UIStarGradePart> StarGradePart;
protected:

	UPROPERTY(EditAnywhere, Category = "BladeII")
	UMaterialInterface* DefaultBackgroundMtrl;

	UPROPERTY(EditAnywhere, Category = "BladeII")
	uint8	DefaultStarGrade;

	EItemClass	SelectedItemClass;
	int32		SelectedID; /* ItemTemplateID or SetUniqueKey */

	bool	bSelected;
	EItemCollectionState CollectionState;

	class UB2UICollectBookMain* CollectBookMainUI;
};



UCLASS()
class BLADEII_API UB2UICollectBookIconRow : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	UB2UICollectBookIcon* GetIconWidget(int32 Grade);

	void SetCollectBookIconInfo(EItemClass ItemClass, int32 Grade, int32 ID);
	void ClearCollectBookIcon(int32 Grade);

	void SetRowTitleText(const FText& TitleText);

	void SetOwnerCollectBookMainUI(class UB2UICollectBookMain* OwnerUI);

private:

	void SetIconAsSetItem(int32 Index, int32 ID);
	void SetIconAsDefaultItem(int32 Index, int32 ID);

	int32 GetSuitableIconIndex(int32 Grade);
	
private:

	TArray<TWeakObjectPtr<UB2UICollectBookIcon>> ItemIcons;
	TWeakObjectPtr<UTextBlock>	TB_ItemRowName;

};

