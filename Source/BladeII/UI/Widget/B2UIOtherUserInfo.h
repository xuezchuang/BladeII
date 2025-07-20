// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2LobbyUISwitcher.h"
#include "../../DataStore/B2ModPlayerInfoData.h"
#include "../../Common/EventSubsystem.h"
#include "B2UIOtherUserInfo.generated.h"


UCLASS()
class BLADEII_API UB2UIOtherUserInfo : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void OnClose(bool RightNow = false) override;

	void SubscribeEvent();
	void UnsubscribeEvent();
	void UpdateCharName(EPCClass PCClass);
	void SetUserNickName(FText NickName);
	void SetBasestats(FB2ModPlayerInfo& UserInfo, EPCClass PCClass);
	void SetExp(FB2ModPlayerInfo& UserInfo, EPCClass PCClass);
	void ItemSet(FB2ModPlayerInfo& UserInfo, EPCClass PCClass);
	void TotalUpdate(EPCClass PCClass);
	void FriendAddState();
	void OnDeliveryAskFriend();
	void OnDeliveryCloseItemPopup(ELobbySubPopups InPopupEnum);

	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
		void PCClassButtonSlotSettingEvent(EPCClass PCClass);
	UFUNCTION(BlueprintImplementableEvent, Category = BladeII)
		void CharSelectUpdateEvent(EPCClass PCClass);

	UFUNCTION(BlueprintImplementableEvent, Category = "B2UIOtherUserInfo")
		void PlayChangeAnimEquipSlotOrCostumeSlot_BP(bool bIEquipSlotVisible);
	
	UFUNCTION()
		void OnClickGladiator();

	UFUNCTION()
		void OnClickAssassin();

	UFUNCTION()
		void OnClickWizard();

	UFUNCTION()
		void OnClickFightern();

	UFUNCTION(BlueprintCallable, Category = BladeII)
		void OnClickItemSelect(EItemEquipPlace EquipPlace);

	UFUNCTION()
		void OnClickFriendAdd();

private:
	void UpdateItemEquipSlotVisible(const bool& bIsVisibleCostumeSlot);

protected:
	TWeakObjectPtr<UB2Button>		BTN_Gladiator;
	TWeakObjectPtr<UB2Button>		BTN_Assassin;
	TWeakObjectPtr<UB2Button>		BTN_Wizard;
	TWeakObjectPtr<UB2Button>		BTN_Fighter;

	//TArray<TWeakObjectPtr<UTextBlock>> TCharName;

	TWeakObjectPtr<UTextBlock>			TB_CharLevel;
	TWeakObjectPtr<UTextBlock>			TB_CombatPower;
	TWeakObjectPtr<UTextBlock>			TB_HealthPoint;
	TWeakObjectPtr<UTextBlock>			TB_AttackPoint;
	TWeakObjectPtr<UTextBlock>			TB_DefensePoint;

	TWeakObjectPtr<UTextBlock>			TB_AttackLabel;
	TWeakObjectPtr<UTextBlock>			TB_DefenseLabel;
	TWeakObjectPtr<UTextBlock>			TB_HealthLabel;
	TWeakObjectPtr<UTextBlock>			TB_CombatLable;

	TWeakObjectPtr<UTextBlock>			TB_UserNickName;
	TWeakObjectPtr<UTextBlock>			TB_CharExpText;
	TWeakObjectPtr<UProgressBar>		PB_CharExpProgress;
	TWeakObjectPtr<UB2Button>			BTN_Friend;

	TWeakObjectPtr<class UB2LobbyUI_ItemDetail>	Lobby_ItemDetailCommonBP;
	TWeakObjectPtr<class UB2LobbyUI_CharStatDetail>	Lobby_CharStatDetailBP;

	TMap<EItemEquipPlace, TWeakObjectPtr<class UB2DynItemIcon_LobbyEquip>> EquipPlaceIcon;


	uint32 DeliveryAskFriendTicket;
	uint32 DeliveryCloseItemPopupTicket;
	EPCClass SelectPCClass;

private:
	TArray<std::shared_ptr<issue>> Issues;
};
