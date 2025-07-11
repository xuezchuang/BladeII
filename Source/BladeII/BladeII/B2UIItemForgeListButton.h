//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
////#include "BladeII.h"
//#include "CoreMinimal.h"
//#include "UI/B2UIWidgetBase.h"
//#include "B2UIItemForgeListButton.generated.h"
//
///**
// * 
// */
//UCLASS()
//class BLADEII_API UB2UIItemForgeListButton : public UB2UIWidgetBase
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
//public:
//	virtual void Init() override;
//	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
//
//	void SetOwnerItemForgeUI(class UB2UIItemForge* OwnerUI, int32 listID);
//	void YouHaveBeenChosen(bool selected=false);
//
//	int32 GetListID() { return ListID; }
//
//protected:
//	class UB2UIItemForge* ItemForgeMainUI;
//
//private:
//	TWeakObjectPtr<UTextBlock> TB_ListTitle;
//	//TWeakObjectPtr<UB2Button> BTN_Area;
//	//TWeakObjectPtr<UB2Image> IMG_Selected;
//
//private:
//	int32 ListID;
//};
