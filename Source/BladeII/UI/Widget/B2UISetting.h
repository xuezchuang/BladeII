// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIWidget.h"
#include "B2UISettingMsgPopup.h"
#include "B2PlatformAPI.h"
#include "B2UIDLCFrontPopup.h"
#include "B2UIBackWidget.h"
#include "B2UISetting.generated.h"

UENUM()
enum class EUISettingSubMenu : uint8
{	
	subMenu_game = 0,
	subMenu_addons,
	subMenu_account,
	subMenu_info,
	max
};
FORCEINLINE int32 GetAllUISettingSubMenuNum() { return static_cast<int32>(EUISettingSubMenu::max); }
FORCEINLINE int32 GetUISettingSubMenuIndex(EUISettingSubMenu InSubMenu) { return static_cast<int32>(InSubMenu); }

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISetting : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()
	friend class UB2UISettingAccount;

	UPROPERTY(Transient)
	TMap<EUISettingSubMenu, UB2UIWidget*> subWidgets;
	UPROPERTY(Transient)
	TArray<UTextBlock*> TextBlockWidget;
	UPROPERTY(Transient)
	TArray<UTextBlock*> TextBlockWidget_UN;
	UPROPERTY(Transient)
	TArray<UImage*> Img_Widget;
	UPROPERTY(Transient)
	TArray<UImage*> Img_Widget_UN;

public:
	UB2UISetting(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
private:
	virtual void CacheAssets() override;	
	virtual void BindDelegates() override;

	UFUNCTION()
		void OnclickBtnExit();
	UFUNCTION()
		void OnclickBtnGame();
	UFUNCTION()
		void OnclickBtnAddOns();
	UFUNCTION()
		void OnclickBtnAccount();
	UFUNCTION()
		void OnclickBtnInfo();

	
	void UpdateStaticText();
	void SelectSubMenu(EUISettingSubMenu submenu);
	FThreadSafeBool m_CouponDelegateCalled;
#if PLATFORM_ANDROID
	FDelegateHandle OnKakaoCouponHandler;
	void KakaoCouponDelegate(JNIEnv *env, jobject thiz);
	
#endif
		
public:
	TWeakObjectPtr<UB2UIWidget>		UIP_Secession;

	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
		void ShowKakaoCommunityView();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
		void ShowKakaoCoupon();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
		void KakaoLoadMessage();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
		void ShowKakaoNotice();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
		void ShowCredit();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
		void RestartTutorial();
	UFUNCTION(BlueprintCallable, Category = "BladeII_Setting")
		void ShowGachaView();

protected:
	virtual void CloseWidgetDelegate() override;
	
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<class UB2UISettingGame> UIP_Setting_Game_Class;
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<class UB2UISettingAddOns> UIP_Setting_AddOns_Class;
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<class UB2UISettingAccount> UIP_Setting_Account_Class;
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<class UB2UIWidget> UIP_Setting_Info_Class;

	TWeakObjectPtr<UCanvasPanel> X_CP_UIP_Setting_Game;
	TWeakObjectPtr<UCanvasPanel> X_CP_UIP_Setting_AddOns;
	TWeakObjectPtr<UCanvasPanel> X_CP_UIP_Setting_Account;
	TWeakObjectPtr<UCanvasPanel> X_CP_UIP_Setting_Info;

	UPROPERTY(Transient)
	class UB2UISettingGame* Created_UIP_Setting_Game;
	UPROPERTY(Transient)
	class UB2UISettingAddOns* Created_UIP_Setting_AddOns;
	UPROPERTY(Transient)
	class UB2UISettingAccount* Created_UIP_Setting_Account;
	UPROPERTY(Transient)
	class UB2UIWidget* Created_UIP_Setting_Info;

	// UIP dynamic load and create..
	void ConditionalCreateSubUIP(EUISettingSubMenu InSubMenu);
	void ConditionalCreateUIPSettingGame();
	void ConditionalCreateUIPSettingAddOns();
	void ConditionalCreateUIPSettingAccount();
	void ConditionalCreateUIPSettingInfo();
	void AddNewCreatedSubMenuUIPtoArray(EUISettingSubMenu InSubMenu, class UB2UIWidget* NewCreatedWidget);
	void DestroyUIPSettingGame();
	void DestroyUIPSettingAddOns();
	void DestroyUIPSettingAccount();
	void DestroyUIPSettingInfo();


	TWeakObjectPtr<UButton>			BTN_Exit;

	TWeakObjectPtr<UButton>			BTN_Setting_Game;
	TWeakObjectPtr<UButton>			BTN_Setting_AddOns;
	TWeakObjectPtr<UButton>			BTN_Setting_Account;
	TWeakObjectPtr<UButton>			BTN_Setting_Info;

	TWeakObjectPtr<UPanelWidget>		X_Coupon;
	
	TWeakObjectPtr<UTextBlock>			TB_Setting_Title;

	TWeakObjectPtr<UTextBlock>			TXT_Setting_Game;
	TWeakObjectPtr<UTextBlock>			TXT_Setting_AddOns;
	TWeakObjectPtr<UTextBlock>			TXT_Setting_Account;
	TWeakObjectPtr<UTextBlock>			TXT_Setting_Info;

	TWeakObjectPtr<UTextBlock>		TB_Setting_Game_Unselected;
	TWeakObjectPtr<UTextBlock>		TB_Setting_Game_Selected;
	TWeakObjectPtr<UTextBlock>		TB_Setting_AddOns_Unselected;
	TWeakObjectPtr<UTextBlock>		TB_Setting_AddOns_Selected;
	TWeakObjectPtr<UTextBlock>		TB_Setting_Account_Unselected;
	TWeakObjectPtr<UTextBlock>		TB_Setting_Account_Selected;
	TWeakObjectPtr<UTextBlock>		TB_Setting_Info_Unselected;
	TWeakObjectPtr<UTextBlock>		TB_Setting_Info_Selected;

	TWeakObjectPtr<UImage>			IMG_Setting_Game_Unselected;
	TWeakObjectPtr<UImage>			IMG_Setting_Game_Selected;
	TWeakObjectPtr<UImage>			IMG_Setting_AddOns_Unselected;
	TWeakObjectPtr<UImage>			IMG_Setting_AddOns_Selected;
	TWeakObjectPtr<UImage>			IMG_Setting_Account_Unselected;
	TWeakObjectPtr<UImage>			IMG_Setting_Account_Selected;
	TWeakObjectPtr<UImage>			IMG_Setting_Info_Unselected;
	TWeakObjectPtr<UImage>			IMG_Setting_Info_Selected;

	TWeakObjectPtr<UB2UISettingMsgPopup> UI_MsgPopupSetting_01;
		

};
