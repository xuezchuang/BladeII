// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2LobbyUserWidget.h"
#include "B2SkillInfo.h"
#include "B2UIWidgetBase.h"
#include "B2UIBackWidget.h"
#include "../Widget/B2ButtonGoodInfoToolTip.h"
#include "../Widget/B2Image.h"
#include "../../Common/EventSubsystem.h"
#include "B2LobbyUI_UnitySkillWakePopup.generated.h"

DECLARE_DELEGATE(FClickSelectButtonDelegate)

UCLASS()
class BLADEII_API UB2LobbyUI_UnitySkillWakePopup : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void CloseWidgetDelegate() override;

public:
	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;
	virtual void DestroySelf() override;
	virtual void BeginDestroy() override;

	void Init(EPCClass pcclass);
	void BindDelegates();
	void UpdateStaticText();
	void SubscribeEvents();
	void UnsubscribeEvents();

	void SetUnityAwakenSkillInfo();
	void SetAwakeMissionInfo(int32 Step);

	void UpdateAwakeSkillInfo();
	void OnReceiveAwakeSkillInfo(const FB2ResponseAwakenUnitySkillMissionPtr& data);
	
	void PageBlock(bool bIsBlock);
	int32 GetUnitySkillID(EPCClass InPCClass);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartGetSoulResultAnimation_BP(bool bSuccess);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartGetSoulAnimation_BP();

	UFUNCTION(BlueprintCallable, Category = "UnityAwake")
	void OnEndGetSoulAnimation_IMP();

	UFUNCTION(BlueprintCallable, Category = "UnityAwake")
	void OnEndGetSoulResultAnimation_IMP();

	UFUNCTION(BlueprintCallable, Category = "UnityAwake")
	void OnEndGetSoulResultFailAnimation_IMP();

protected:
	UFUNCTION()
	void OnClickBTN_Close();
	UFUNCTION()
	void OnClickBTN_GetSoul();

	UFUNCTION()
	void OnClick_SelectSoul(int32 nStep);

private:
	TWeakObjectPtr<UB2ButtonGoodInfoToolTip> BTN_StuffTip;
	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_GetSoul;

	TWeakObjectPtr<UTextBlock> STB_GetSoul;
	TWeakObjectPtr<UTextBlock> STB_Stuff;
	TWeakObjectPtr<UTextBlock> STB_Percent;
	TWeakObjectPtr<UTextBlock> STB_Title;
	TWeakObjectPtr<UTextBlock> STB_MAX;
	TWeakObjectPtr<UTextBlock> STB_Fail;
	
	TWeakObjectPtr<UTextBlock> TB_SkillName;
	TWeakObjectPtr<UTextBlock> TB_SkillDesc;
	TWeakObjectPtr<UTextBlock> TB_HaveCount;
	TWeakObjectPtr<UTextBlock> TB_NeedCount;
	TWeakObjectPtr<UTextBlock> TB_Percent;

	TWeakObjectPtr<UWidgetSwitcher> SW_BottomInfo;

	TWeakObjectPtr<UB2Image> IMG_NeedStuff;
	TWeakObjectPtr<UImage> IMG_SkillIcon;

	TArray<TWeakObjectPtr<class UB2LobbyUI_SkillStepIcon>> UnityInfinityStones;

	EPCClass CurrentPC;
	int32 CurrentStep;

	bool bIsPageBlock;
	bool bSubscribed;

	TArray<std::shared_ptr<issue>> Issues;
};

UCLASS()
class BLADEII_API UB2LobbyUI_SkillStepIcon : public UB2LobbyUserWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
public:
	virtual void DestroySelf() override;
	
	void Init();
	void BindDelegate();

	void SetState(bool bSelect, bool bGet);
	void GetState(bool& bSelect, bool& bGet);
	void SetIsGet(bool bGet);
	void SetIsSelect(bool bSelect);

	void SetTintColor(int32 nNumber);
	
	template<typename FunctorType>
	inline void SetButtonDelegateBindLambda(FunctorType&& InFunctor)
	{
		ButtonDelegate.BindLambda(Forward<FunctorType>(InFunctor));
	}

protected:
	UFUNCTION()
	void OnClick_Select();

private:
	TWeakObjectPtr<UWidgetSwitcher> WS_Switch;
	TWeakObjectPtr<UImage> IMG_Select;
	TWeakObjectPtr<UImage> IMG_On;
	TWeakObjectPtr<UButton> BTN_Select;

	FClickSelectButtonDelegate	ButtonDelegate;

	bool bIsSelect;
	bool bIsGet;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BladeII UnitySkillUI")
	TArray<FLinearColor> TintColors;

};
