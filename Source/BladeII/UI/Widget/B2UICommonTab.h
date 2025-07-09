// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "B2UIWidgetBase.h"
#include "B2UICommonTab.generated.h"

DECLARE_DELEGATE(FTabButtonDelegate)

UCLASS()
class BLADEII_API UB2UICommonTab : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION()
	void OnClickBTN_Tab();
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetTabSelected(bool selected);
	void SetTabName(FText TabName);
	void SetVisible(bool bVisible);
	void SetTabHeadImage(UMaterialInstance* image);
	void SetTabEvent(bool bFundsSale);
	void SetTabStoreEventLabel(int32 ProductMark);
	void SetTabLimitLevel(bool bIsLimitLevel);

	FORCEINLINE void ForcedClickTab() { OnClickBTN_Tab(); };
	FORCEINLINE bool GetTabSelected() { return _IsSelected; }

	FORCEINLINE void SetTabButtonIndex(int32 ButtonIdx) { ButtonIndex = ButtonIdx; };
	FORCEINLINE int32 GetTabButtonIndex() { return ButtonIndex; }

	template<typename FunctorType>
	inline void TabButtonDelegateBindLambda(FunctorType&& InFunctor)
	{
		ButtonDelegate.BindLambda(Forward<FunctorType>(InFunctor));
	}

	void SetIsTabEnable(bool InValue);
	void ClearDelegate();
	
	UB2UnitedWidgetBase* GetMyRedDotWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	FText CurrentTabName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	UMaterialInstance* TabHeaderImage;

private:
	TWeakObjectPtr<UPanelWidget> p_UnSelected;
	TWeakObjectPtr<UPanelWidget> p_Selected;
	TWeakObjectPtr<UTextBlock> TB_Unselected;
	TWeakObjectPtr<UTextBlock> TB_Selected;
	TWeakObjectPtr<UB2Button> BTN_Tab;
	
	TWeakObjectPtr<class UB2UIStoreEventLabel>	UIP_Store_Event_Label;
	
	bool _IsSelected;
	FTabButtonDelegate	ButtonDelegate;

	int32 ButtonIndex;
};
