// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidgetBase.h"
#include "B2UIBackWidget.h"
#include "B2UIGuildMapCharPopup.generated.h"

DECLARE_DELEGATE(FGuildPopupClose)

UCLASS()
class BLADEII_API UB2UIGuildMapCharPopup : public UB2UIWidgetBase, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void OpenGuildInfo(const b2network::B2GuildBattleMemberPtr& InAllyGuildMember, const TArray<b2network::B2GuildBattleAttackPtr>& AttackInfo);
	void OpenGuildInfo(const b2network::B2GuildBattleMemberPtr& InAllyGuildMember, const b2network::B2GuildBattleMemberPtr& InEnemyGuildMember);
	void SetMyGuild();

	void SetGuildInfo(const b2network::B2GuildBattleMemberPtr& GuildMember);
	void SetAttackInfo(const TArray<b2network::B2GuildBattleAttackPtr>& AttackInfo);
	void SetCloseDelegate(FGuildPopupClose Delegate);

	void SetVisibilityPopup(bool InVisible);

protected:
	UFUNCTION()
	void OnClickBTN_Battle();
	UFUNCTION()
	void OnClickBTN_Close();

private:
	TWeakObjectPtr<UB2RichTextBlock> TB_AllyGuildName;
	TWeakObjectPtr<class UB2UIGuildInfoMedal> UIP_AllyGuildMedal1;

	TWeakObjectPtr<UTextBlock> TB_AllyPowerIndex;


	TWeakObjectPtr<class UB2UIGuildInfoMedalPart>	UIP_GuildAttackInfo1;
	TWeakObjectPtr<class UB2UIGuildInfoMedalPart>	UIP_GuildAttackInfo2;

	TWeakObjectPtr<UB2RichTextBlock>		TB_MedalCount;

	TWeakObjectPtr<UB2Button>		BTN_Battle;
	TWeakObjectPtr<UB2Button>		BTN_Close;
	TWeakObjectPtr<UOverlay>		CP_Battle;

	TWeakObjectPtr<class UB2UIGuildFormation>			UIP_MyGuildFormation1;
	TWeakObjectPtr<class UB2UIGuildFormation>			UIP_MyGuildFormation2;

	TWeakObjectPtr<UB2RichTextBlock>					TB_EnemyGuildName;
	TWeakObjectPtr<UB2RichTextBlock>					TB_EnemyMedalCount;
	TWeakObjectPtr<class UB2UIGuildFormation>			UIP_EnemyGuildFormation1;
	TWeakObjectPtr<class UB2UIGuildFormation>			UIP_EnemyGuildFormation2;

	TWeakObjectPtr<UTextBlock>							TB_EnemyPowerIndex;

	TWeakObjectPtr<UPanelWidget>						O_AllyPanel;
	TWeakObjectPtr<UPanelWidget>						O_EnemyPanel;


	int64 AccountID;


	FGuildPopupClose				GuildPopupClose;
};
