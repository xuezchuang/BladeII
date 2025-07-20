// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UIBackWidget.h"
#include "B2UIGuildInvitePopup.generated.h"


UCLASS()
class BLADEII_API UB2UIGuildInvitePopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void OnOpen(bool RightNow = false) override;
	virtual void OnClose(bool RightNow = false) override;

	void SetInviteList(const FB2ResponseGetGuildMemberCandidatesPtr& MemberCandidtates);

	void SetFindUser(const FB2FindUser& FindUser);
protected:
	UFUNCTION()
	void OnClickBTN_Surch();
	UFUNCTION()
		void OnClickClose();

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UB2UIGuildInviteSlot> InviteSlot;
private:
	TWeakObjectPtr<UB2Button>			BTN_Surch;
	TWeakObjectPtr<UB2Button>			BTN_Close;
	TWeakObjectPtr<UEditableTextBox>	ETB_SearchGuild;
	TWeakObjectPtr<UVerticalBox>		VB_SlotList;



	int64 DeliveryFindUserTicket;

};
