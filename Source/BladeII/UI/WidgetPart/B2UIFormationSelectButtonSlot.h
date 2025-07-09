#pragma once
#include "B2UIWidgetBase.h"
#include "B2UIFormationSelectButtonSlot.generated.h"

DECLARE_DELEGATE_OneParam(FFormationSlotStateDelegate, ETMFormation);

UCLASS(BlueprintType, Blueprintable)
class UB2UIFormationSelectButtonSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()
public:
	void SetFormationType(ETMFormation FormationType, int32 Lv);
	void SetFormationType(ETMFormation FormationType);
	void SetSlotState(ETMFormation FormationState);

	void SetOnSlotState();
	void SetOffSlotState();

	UFUNCTION()
		void OnClickFormationSelect();
	
public:
	virtual void CacheAssets() override;
	virtual void Init() override;
	virtual void BindDelegates() override;

	FFormationSlotStateDelegate  SlotStateDelegate;

private:
	TWeakObjectPtr<UTextBlock>	TB_Lv;
	TWeakObjectPtr<UTextBlock>	TB_Formation;
	TWeakObjectPtr<UImage>		IMG_Formation;
	TWeakObjectPtr<UButton>		BTN_SelectButton;


	ETMFormation				EFormationInfo;



public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TArray<UMaterialInterface*>		FormationOnIcon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BladeII")
		TArray<UMaterialInterface*>		FormationOffIcon;


	UFUNCTION(BlueprintImplementableEvent)
		void OnSelect();

	UFUNCTION(BlueprintImplementableEvent)
		void OffSelect();
};