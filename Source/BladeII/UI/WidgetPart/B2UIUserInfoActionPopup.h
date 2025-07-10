#pragma once
//#include "BladeII.h"
#include "B2UIWidgetBase.h"
#include "B2UIBackWidget.h"
#include "B2UIUserInfoActionPopup.generated.h"

/*
 * Popup to select user info action.
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIUserInfoActionPopup : public UB2UIWidgetBase, public IB2UIBackWidget
{
	GENERATED_BODY()

private:
	bool IsBlockingUser;

	uint32 DeliveryAskFriendTicket;
	uint32 HandleServerError5503Ticket;

	int64 CachedAccountID;
	int64 CachedGuildID;
	FString TheUserName;

	float ButtonDelay;

protected:

	TWeakObjectPtr<UB2Button> BTN_Visit;
	TWeakObjectPtr<UB2Button> BTN_Friendship;
	TWeakObjectPtr<UB2Button> BTN_VisitGuild;
	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_UserBlock;

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_UserName;
	TWeakObjectPtr<UTextBlock> TB_Visit;
	TWeakObjectPtr<UTextBlock> TB_Friendship;
	TWeakObjectPtr<UTextBlock> TB_VisitGuild;
	TWeakObjectPtr<UTextBlock> TB_UserBlock;

public:
	UB2UIUserInfoActionPopup(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetUserInfo(const int64 InAccountID, const int64 GuildID, const FString& InUserName); // Add any more necessary info.. (Guild, etc..)
	void SetUserBlockState(bool Isblock);

	void SubscribeEvent();
	void UnsubscribeEvent();

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	void OnDeliveryAskFriend();

	UFUNCTION()
	void OnClickBtnVisit();

	UFUNCTION()
	void OnClickBtnFriendship();

	UFUNCTION()
	void OnClickBtnVisitGuild();

	UFUNCTION()
	void OnClickBtnClose();

	UFUNCTION()
	void OnClickBtnUserBlock();

public:
	void CloseMe();
};