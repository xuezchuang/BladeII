//// Fill out your copyright notice in the Description page of Project Settings.
//#pragma once
////#include "BladeII.h"
//#include "CoreMinimal.h"
//#include "UI/B2UIWidgetBase.h"
//#include "B2UIItemForgeTabButton.generated.h"
//
//UCLASS()
//class BLADEII_API UB2UIItemForgeTabButton : public UB2UIWidgetBase
//{
//	GENERATED_BODY()
//	
//protected:
//	virtual void CacheAssets() override;
//	virtual void BindDelegates() override;
//
//	virtual void UpdateStaticText() override;
//
//	UFUNCTION()
//		void OnClickBTN_Area();
//
//	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
//		void OnSelected_BP();
//	
//public:
//	virtual void Init() override;
//	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
//
//	void SetOwnerItemForgeUI(class UB2UIItemForge* OwnerUI, int32 tabID);
//	void YouHaveBeenChosen(bool selected);
//	int32 GetTabID() { return TabID; }
//	void SetVisible(bool bVisible);
//
//protected:
//	class UB2UIItemForge* ItemForgeMainUI;
//
//private:
//	TWeakObjectPtr<UPanelWidget> p_UnSelected;
//	TWeakObjectPtr<UPanelWidget> p_Selected;
//	TWeakObjectPtr<UTextBlock> TB_Unselected;
//	TWeakObjectPtr<UTextBlock> TB_Selected;
//	//TWeakObjectPtr<UB2Button> BTN_Tab;
//
//private:
//	int32 TabID;
//	bool _IsSelected;
//};
