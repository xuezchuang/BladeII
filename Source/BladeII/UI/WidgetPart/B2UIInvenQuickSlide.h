#pragma once

#include "B2UIWidget.h"
#include "B2UIInvenQuickSlide.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIInvenQuickSlide : public UB2UIWidget
{
	GENERATED_BODY()

public:
	UB2UIInvenQuickSlide(const FObjectInitializer& ObjectInitializer);	
	virtual void Init() override;

	virtual void DoMarkRedDot() final;
	virtual void RegisterUIMarkForRedDot() final;
	virtual void UnRegisterUIMarkForRedDot() final;

	UFUNCTION(BlueprintCallable, Category = "InvenQck")
		void GotoInvenScene();
	UFUNCTION(BlueprintCallable, Category = "InvenQuick")
		void GotoSkillScene();
	UFUNCTION(BlueprintCallable, Category = "InvenQuick")
		void GotoBrevetScene();
	UFUNCTION(BlueprintCallable, Category = "InvenQuick")
		void GotoWingScene();
	UFUNCTION(BlueprintCallable, Category = "InvenQuick")
		void GotoConstantheroScene();

	UFUNCTION(BlueprintCallable, Category = "InvenQuick")
		bool GetInvenQuickSlide();
	
private:
	virtual void CacheAssets() override;	
	virtual void BindDelegates() override;	
	void SetVisibleVersion();

	TWeakObjectPtr<UButton>			BTN_QuickInven;
	TWeakObjectPtr<UButton>			BTN_QuickSkill;
	TWeakObjectPtr<UButton>			BTN_QuickBrevet;
	TWeakObjectPtr<UButton>			BTN_QuickWing;
	TWeakObjectPtr<UButton>			BTN_QuickConstanthero;

	TWeakObjectPtr<UB2UIWidgetBase>			IMG_MarkForNew_Inven;
	TWeakObjectPtr<UB2UIWidgetBase>			IMG_MarkForNew_Skill;
	TWeakObjectPtr<UB2UIWidgetBase>			IMG_MarkForNew_Brevet;
	TWeakObjectPtr<UB2UIWidgetBase>			IMG_MarkForNew_Wing;
	TWeakObjectPtr<UB2UIWidgetBase>			IMG_MarkForNew_Relic;
	TWeakObjectPtr<UB2UIWidgetBase>			IMG_MarkForNew_All;

	TArray<TWeakObjectPtr<UImage>>		ArrImg_Lock;
	TArray<TWeakObjectPtr<UImage>>		ArrImg_Icon;

public:
	bool bCheckTest = false;
	
		
};
