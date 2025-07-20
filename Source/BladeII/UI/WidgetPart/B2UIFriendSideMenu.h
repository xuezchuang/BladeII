// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidgetBase.h"
#include "B2UIFriendSideMenu.generated.h"

UENUM()
enum class EB2UIFriendSideMenu : uint8
{
	MyFriendList,
	FindFriend,
	MyKakaoFriendList,
	KakaoInvite,
	End
};

class UB2UIFriendSideMenu;

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIFriendSideMenuRow : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	void SetButtonEnabled(bool InEnabled);
	bool GetIsButtonEnabled();

	void SetUIParentInfo(const UB2UIFriendSideMenu *Parent);

	EB2UIFriendSideMenu GetSideMenuType() { return SideMenuType; }

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

	void UpdateButtonState();

	UFUNCTION()
	void OnButtonCallback();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeII)
	EB2UIFriendSideMenu SideMenuType;

private:
	TWeakObjectPtr<UImage> IMG_MyFriendList_Unselected;
	TWeakObjectPtr<UImage> IMG_MyFriendList_Selected;

	TWeakObjectPtr<UTextBlock> TB_MyFriendList_Unselected;
	TWeakObjectPtr<UTextBlock> TB_MyFriendList_Selected;

	TWeakObjectPtr<UB2Button> BTN_MyFriendList;

	TWeakObjectPtr<UB2UnitedWidgetBase> UIP_Reddot_FriendList;

	TWeakObjectPtr<UB2UIFriendSideMenu> ParentMenu;

private:
	bool bIsEnabledState;
};
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIFriendSideMenu : public UB2UIWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	void OnMenuSelect(EB2UIFriendSideMenu SelectMenu);

	UFUNCTION()
	bool RedDotCondition_MyFriends();
	UFUNCTION()
	bool RedDotCondition_SendAbleFriendPoint();
	UFUNCTION()
	bool RedDotCondition_RequestFriends();

	virtual void RegisterUIMarkForRedDot() final;
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;

private:
	TArray<TWeakObjectPtr<class UB2UIFriendSideMenuRow>> MenuTabs;
};
