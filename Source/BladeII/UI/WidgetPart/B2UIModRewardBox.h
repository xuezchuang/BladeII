// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIModRewardBox.generated.h"


UCLASS()
class BLADEII_API UB2UIModRewardBox : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void RegisterUIMarkForRedDot() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void UpdateStaticText() override;

protected:
	UFUNCTION()
		void OnClickBTN_Reward();

	UFUNCTION()
		void OnClickBTN_Questionmark();
	UFUNCTION()
		void OnClickBTN_Detail();

private:
	TWeakObjectPtr<UTextBlock>		TB_BoxName;
	TWeakObjectPtr<UProgressBar>	PB_Progress;
	TWeakObjectPtr<UTextBlock>		TB_Progress;
	TWeakObjectPtr<UB2Image>		IMG_CountMark;
	TWeakObjectPtr<UB2Image>		IMG_Box;
	TWeakObjectPtr<UB2Button>		BTN_Reward;
	TWeakObjectPtr<UB2Button>		BTN_Questionmark;
	TWeakObjectPtr<UB2Button>		BTN_Detail;
	TWeakObjectPtr<UPanelWidget>	CP_Progress;
	TWeakObjectPtr<UPanelWidget>	CP_RewardBtnPanel;

	TWeakObjectPtr<UTextBlock>		TB_CreateBtn;

	TWeakObjectPtr<UTextBlock>		TB_ModExplanation;

	TWeakObjectPtr<UOverlay>		OV_MVPKeyText;
	TWeakObjectPtr<UPanelWidget>	HB_Defeat;

	int32 ModType;
	bool bIsReward;
	int32 m_nGrade;

public:
	void SetModRewardBox(bool IsRewardState,int32 ServerModType, int32 BoxGrade, int32 WinCount, int32 MaxCount, int32 DefeatCount = INDEX_NONE);
	void SetModExplanation(FText InExplanation);

private:
	void SetModRewardBoxType(int32 servertype, bool IsMvp);
	void SetModRewardBoxProgress(int32 WinCount, int32 MaxCount);
	void SetModRewardBoxGrade(int32 nGrade, bool IsMvp ,int32 DefaultWinCount = 0 , int32 WinMax = 0);
	void SetButtonEnable();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
		void DefeatCountEvent(int32 DefeatCount);

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
		void SetBoxImageGradeEvent(int32 nGrade);

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
		void SetMVPImageGradeEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
		void OpenRewardFx(bool InRewardState);

public:

	UPROPERTY(EditAnywhere)
		UTexture2D*						MVPBoxImage;

private:
	const int32 DdfeatMaxCount = 3;
	FTimerHandle ButtonTimerhandle;

};
UCLASS()
class BLADEII_API UB2UIMvpRewardBox : public UB2UIWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void RegisterUIMarkForRedDot() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

public:
	void SetMvpBox(int32 CurKey, int32 MaxKey);

	UFUNCTION()
		void OnClickBTN_Detail();
	UFUNCTION()
		void OnClickBTN_RewardBox();

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
		void OpenRewardFx(bool InRewardState);

	void SetButtonEnable();

protected:
	TWeakObjectPtr<UButton>		BTN_RewardBox;
	TWeakObjectPtr<UPanelWidget>	CP_Reward;
	TWeakObjectPtr<UPanelWidget>	CP_Progress;
	TWeakObjectPtr<UProgressBar>	PB_Progress;
	TWeakObjectPtr<UTextBlock>		TB_MVPKeyText;
	TWeakObjectPtr<UTextBlock>		TB_ProgressText;
	TWeakObjectPtr<UB2Button>		BTN_Detail;

private:
	bool bIsReward;
	FTimerHandle ButtonTimerhandle;
};