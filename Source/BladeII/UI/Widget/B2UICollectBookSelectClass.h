// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UICollectBookSelectClass.generated.h"


UCLASS()
class BLADEII_API UB2UICollectBookSelectClass : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:

	void SubscribeEvent();
	void UnSubscribeEvent();

	void OnReceiveCollectionItems(FB2CollectionItemPtr CollectionItems);

	uint8 DeliveryGetCollectionItemTicket;

private:

	TWeakObjectPtr<class UB2UICollectBookClassPart> UIP_ClassGladiator;
	TWeakObjectPtr<class UB2UICollectBookClassPart> UIP_ClassAssassin;
	TWeakObjectPtr<class UB2UICollectBookClassPart> UIP_ClassWizard;
	TWeakObjectPtr<class UB2UICollectBookClassPart> UIP_ClassFighter;

	TWeakObjectPtr<UTextBlock> TB_Title;
};


UCLASS()
class BLADEII_API UB2UICollectBookClassPart: public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void SynchronizeProperties() override;

	void InitClassType(EPCClass InClassType);
	void InitcollectionProgress();

	void SetCollectionProgress(int32 CurrentCount, int32 MaxCount);

private:

protected:

	UFUNCTION()
	void OnClickBtn_Area();

private:
	TWeakObjectPtr<UOverlay> O_CollectPerfect;
	TWeakObjectPtr<UTextBlock> TB_CollectPerfect;

	TWeakObjectPtr<UOverlay> O_CollectProgress;
	TWeakObjectPtr<UProgressBar> PB_CollectRate;
	TWeakObjectPtr<UTextBlock> TB_CollectRateLeft;
	TWeakObjectPtr<UTextBlock> TB_CollectRateRight;
	TWeakObjectPtr<UTextBlock>	TB_CollectRatePer;
	
	TWeakObjectPtr<UB2Image> IMG_Character;
	TWeakObjectPtr<UTextBlock> TB_Character;

	TWeakObjectPtr<UB2UIWidgetBase> IMG_NewIcon;
	TWeakObjectPtr<UButton> BTN_Area;

protected:

	EPCClass ClassType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
	UMaterialInterface* ClassMaterial;
};
