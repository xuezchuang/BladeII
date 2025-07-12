// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "UObject/Object.h"
#include "B2LobbyUserWidget.h"
#include "RelicManager.h"
#include "B2UIBackWidget.h"
#include "../Widget/B2ButtonGoodInfoToolTip.h"
#include "B2LobbyUI_RelicManagement.generated.h"


UCLASS()
class BLADEII_API UB2LobbyUI_RelicManagement : public UB2LobbyUserWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	class UB2Airport* CachedAirport;
public:
	UB2LobbyUI_RelicManagement(const FObjectInitializer& ObjectInitializer);

	virtual void CacheAssets() override;
	virtual void DestroySelf() override;

	virtual void StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM) override;

	void OnCreateRelicBoard();
	void UpdateRelicInfo();

	void PlayFailAnimation_LevelUp();
	void ShowFailPopup(bool bIsLevelUp);

	void OnPlaySucessAnimation(bool bIsLevelUp);
	void OnResponseRelicLevelUp(int32 nLevel);
	void OnResponseRelicGradeUp(int32 nGrade);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayFailPopup_IMP(bool bIsLevelUp);
	UFUNCTION(BlueprintCallable, Category = "Relic")
	void OnEndFailPopupAnimation_BP();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayUpgradeFail_IMP();
	UFUNCTION(BlueprintCallable, Category = "Relic")
	void OnEndUpgradeFail_BP();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayUpgradeAnimation_IMP(int32 nGrade);
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayLevelUpAnimation_IMP();
	UFUNCTION(BlueprintCallable, Category = "Relic")
	void OnEndAnimation_BP(bool bIsLevelUp);
protected:
	UFUNCTION()
	void OnClickBTN_Close();
	UFUNCTION()
	void OnClickBTN_AutoUpgrade();

	void RequestEnchanceRelic();
	void RequestPromotionRelic();

	void SetStaticText();

	void CheckButtonEnable();
	void SetVisibleCurrentGrade(int32 nGrade);

	void CachedWidgetInit();
	void CachedWidgetBind();
	
	void SetEnablePage(bool bEnable);

	class UB2UIUpgradeRelicBoard* GetCurrentBoardWidget();

	static class AB2LobbyGameMode* TransientLobbyGM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BladeIIGame)
	TArray<TSoftClassPtr<class UB2UIUpgradeRelicBoard>>	BoardWidgetArray;

	virtual void CloseWidgetDelegate() override;
	
private:
	TWeakObjectPtr<class UB2UIHeader>					UIP_CommonHeader; 
	TWeakObjectPtr<UB2Button>							BTN_Close;
	TWeakObjectPtr<UProgressBar>						PB_EnhancePoint;
	TWeakObjectPtr<UTextBlock>							STB_EnhancePoint;
	TWeakObjectPtr<UTextBlock>							STB_EntenHero;
	TWeakObjectPtr<UTextBlock>							TB_TotalAttText;
	TWeakObjectPtr<UTextBlock>							TB_TotalAttAmount;
	TWeakObjectPtr<UTextBlock>							TB_TotalDefText;
	TWeakObjectPtr<UTextBlock>							TB_TotalDefAmount;
	TWeakObjectPtr<UTextBlock>							TB_TotalHPText;
	TWeakObjectPtr<UTextBlock>							TB_TotalHPAmount;
	TWeakObjectPtr<UTextBlock>							STB_TotalGradeAtt;
	TWeakObjectPtr<UTextBlock>							TB_TotalGradeAtt;
	TWeakObjectPtr<UTextBlock>							STB_TotalGradeDef;
	TWeakObjectPtr<UTextBlock>							TB_TotalGradeDef;
	TWeakObjectPtr<UTextBlock>							STB_TotalGradeHP;
	TWeakObjectPtr<UTextBlock>							TB_TotalGradeHP;
	TWeakObjectPtr<UTextBlock>							TB_NextAttText;
	TWeakObjectPtr<UTextBlock>							TB_NextAttAmount;
	TWeakObjectPtr<UTextBlock>							TB_NextDefText;
	TWeakObjectPtr<UTextBlock>							TB_NextDefAmount;
	TWeakObjectPtr<UTextBlock>							TB_NextHPText;
	TWeakObjectPtr<UTextBlock>							TB_NextHPAmount;
	TWeakObjectPtr<UOverlay>							X_RelicBoard;
	TWeakObjectPtr<UTextBlock>							STB_Upgrade;
	TWeakObjectPtr<UB2Button>							BTN_AutoUpgrade;
	TWeakObjectPtr<UTextBlock>							STB_AutoUpgrade;
	TWeakObjectPtr<UTextBlock>							STB_EnhanceEffect;
	TWeakObjectPtr<UTextBlock>							STB_Stuff;
	TWeakObjectPtr<UTextBlock>							STB_Percent;
	TWeakObjectPtr<UTextBlock>							TB_Percent;
	TWeakObjectPtr<UWidgetSwitcher>						BottomInfoSwitcher;
	TWeakObjectPtr<UWidgetSwitcher>						UpgradePanelSwitcher;
	TWeakObjectPtr<UTextBlock>							TB_RelicName;
	TWeakObjectPtr<UTextBlock>							TB_RelicLevel;
	TWeakObjectPtr<UTextBlock>							TB_UpgradeIndex;
	TWeakObjectPtr<class UB2LobbyUI_RelicStarItem>	p_NextGrade;
	TWeakObjectPtr<class UB2UIValueButton>				BTN_Enchance;
	TWeakObjectPtr<class UB2UIValueButton>				BTN_Promotion;

	TArray<TWeakObjectPtr<class UWidget>>				StarList;

	TWeakObjectPtr<UB2ButtonGoodInfoToolTip>			BTN_StuffTip;
	TWeakObjectPtr<UB2Image>							IMG_NeedStuff;
	TWeakObjectPtr<UImage>								IMG_TouchBlock;
	
	TWeakObjectPtr<UTextBlock>							TB_HaveCount;
	TWeakObjectPtr<UTextBlock>							TB_NeedCount;

	TWeakObjectPtr<UTextBlock>							STB_EnhanceEfttect;

	bool bIsAuto = false;
	bool bIsAni = false;
};
