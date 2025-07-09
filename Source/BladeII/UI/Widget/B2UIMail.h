#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIMailContent.h"
#include "B2UIMail.generated.h"

/*
* Lobby Main UI
*/

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMail : public UB2UIWidget
{
	GENERATED_BODY()
public:
	UB2UIMail(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	virtual void RegisterUIMarkForRedDot() final;
	virtual void DoMarkRedDot() override;
	UFUNCTION()
		void OnClicked_GiftTabBTN();
	UFUNCTION()
		void OnClicked_BladePointTabBTN();
	UFUNCTION()
		void OnClicked_FriendshipTabBTN();
	UFUNCTION()
		void OnClicked_TicketTabBTN();
	UFUNCTION()
		void OnClicked_ReceiveAllBTN();
	UFUNCTION()
		void OnClicked_UpgradeStoneReceiveAllBTN();
	UFUNCTION()
		void OnClicked_RareUpgradeStoneReceiveAllBTN();
	UFUNCTION()
		bool RedDotCondition_Gift();
	UFUNCTION()
		bool RedDotCondition_BladePoint();
	UFUNCTION()
		bool RedDotCondition_FriendShip();
	UFUNCTION()
		bool RedDotCondition_Ticket();
	void SetMailList(const FB2MailList& MailList);
	void UpdateMailList();
	void UpdateMailList(const TArray<int64>& InMailIdArray);
	void OpenMail(int64 MailId, const FB2ResponseOpenMailPtr& MailInfo);
	void OpenAllMail(const FB2OpenAllMail& OpenedAllMail);
	void OpenPopupReceiveOneMail(MailGiftType GiftType, int32 Amount);
	void OpenPopupReceiveOneMail(int32 ItemTemplateId, int32 Amount);
	// Belows open UIPs in on-demand dynamic loading/create manner.
	void OpenGeneralLottery(int64 MailId, const FText& TitleText);
	void OpenAllSelectiveLottery(int64 MailId, const TArray<int32>& SelectableItemRefIds, const FText& TitleText);
	void OpenPrefixSelectiveLottery(int64 MailId, const int32 SurpassLevel, const TArray<int32>& SelectableItemRefIds, const FText& TitleText);
	void OpenPrefixSelectiveLotteryCostume(int64 MailId, const int32 SurpassLevel, const TArray<int32>& SelectableItemRefIds, const FText& TitleText);
	void OpenSelectiveLottery(int64 MailId, const TArray<int32>& SelectableItemRefIds, const FText& TitleText);
	void OpenSelectiveLotteryConsumeItem(int64 MailId, const TArray<int32>& SelectableItemRefIds, const TArray<int32>& SelectableItemNums);
	void OpenPopupReceiveSelectiveLotteryConsumeItemResult(int32 ItemRefId, int32 Num);
	void OpenPopupReceiveSelectiveLotteryResult(const FB2Item& ResultItem);
	void SelectedCharacterGeneralLottery(EPCClass SelectedCharacter);
	void SelectedItemSelectiveLottery(int32 SelectedItemRefId);
	void ClientDataStoreAddNewAcquiredItem(FB2Item AddItem);
	FORCEINLINE MailTabIndex GetCurrentMailTab() { return CurrentTabIndex; }
	bool HasAnyMail();
	bool HasAnyMail(MailTabIndex TabIndex);
	void RequestCurrentTabMailList();
	void OpenMailUI(int64 InMailId, const FB2ResponseOpenMailPtr& InMailInfo);
	void OpenAllMailUI(FB2ResponseOpenAllMail InAllMailReceiveData);
	void SetTouch(bool InTouch);
	bool GetTouch();

	int32 upgradestone_mail_totalcount;
	int32 rare_upgradestone_mail_totalcount;

	int32 changed_upgradestone;
	int32 changed_rare_upgradestone;

	bool IsRare;

protected:
	virtual void CacheAssets() override;
	virtual void UpdateStaticText() override;
	virtual void BindDelegates() override;

private:
	void ConditionalCreateGeneralLotteryPopup();
	void ConditionalCreateAllSelectiveLotteryPopup();
	void ConditionalCreateAllSelectiveLotteryCostumePopup();
	void ConditionalCreateSelectiveLotteryPopup();
	void ConditionalCreateSelectiveLotteryConsumeItemPopup();
	void ConditionalCreateSelectiveLotteryConsumeItemResultPopup();
	void ConditionalCreateSelectiveLotteryResultPopup();
	void DestroyGeneralLotteryPopup();
	void DestroySelectiveLotteryPopup();
	void DestroyAllSelectiveLotteryPopup();
	void DestroyAllSelectiveLotteryCostumePopup();
	void DestroySelectiveLotteryConsumeItemPopup();
	void DestroySelectiveLotteryConsumeItemResultPopup();
	void DestroySelectiveLotteryResultPopup();
	void SetTabIndex(MailTabIndex Index);
	void OpenAllMailUI(
		const int32 ReceivedGold,
		const int32 ReceivedGem,
		const int32 ReceivedBladePoint,
		const int32 ReceivedSocialPoint,
		const int32 ReceivedDarkCrystalPoint,
		const int32 ReceivedFamePoint,
		const int32 ReceivedStageBoostTicket,
		const int32 ReceivedStageAutoClearTicket,
		const int32 ReceivedHotTimeGoldBuffTicket,
		const int32 ReceivedHotTimeExpBuffTicket,
		const int32 ReceivedShopTenLotteryTicket,
		const TArray<FB2MailLotteryItemInfoPtr>& ItemArray,
		const TArray<int64>& MailIdArray
	);
	void SubscribeEvent();
	void UnsubscribeEvent();
	void SetMailReceiveState(MailTabIndex InIndex);
	void DeleteMailReceiveState();
	bool MailReceiveAllBtnState();
	bool CheckUpgradeStoneReceiveAllBTN();
	bool CheckRareUpgradeStoneReceiveAllBTN();

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<UB2UIMailContent> MailContentClass;

protected:
	// Those below are UIP classes that will be dynamically loaded and created at relevant event.
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UIGeneralLotteryPopup> UIP_GeneralLotteryPopupClass;
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UISelectiveLotteryPopup> UIP_SelectiveLotteryPopupClass;
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UIAllSelectiveLotteryPopup> UIP_AllSelectiveLotteryPopupClass;
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UISelectiveLotteryConsumItemPopup> UIP_SelectiveLotteryConsumeItemPopupClass;
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UISelectiveLotteryConsumeItemResultPopup> UIP_SelectiveLotteryConsumeItemResultClass;
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UIMailSelectiveLotteryResult> UIP_SelectiveLotteryResultPopupClass;
	UPROPERTY(EditAnywhere)
		TSoftClassPtr<class UB2UIAllSelectiveLotteryCostumePopup> UIP_AllSelectiveLotteryCostumePopupClass;

	// Each of those below is dynamically created by matching TSoftClassPtr above.
	UPROPERTY(Transient)
		class UB2UIGeneralLotteryPopup* CreatedGeneralLotteryPopup;
	UPROPERTY(Transient)
		class UB2UISelectiveLotteryPopup* CreatedSelectiveLotteryPopup;
	UPROPERTY(Transient)
		class UB2UISelectiveLotteryConsumItemPopup* CreatedSelectiveLotteryConsumeItemPopup;
	UPROPERTY(Transient)
		class UB2UISelectiveLotteryConsumeItemResultPopup* CreatedSelectiveLotteryConsumeItemResultPopup;
	UPROPERTY(Transient)
		class UB2UIMailSelectiveLotteryResult* CreatedSelectiveLotteryResultPopup;
	UPROPERTY(Transient)
		class UB2UIAllSelectiveLotteryPopup* CreatedAllSelectiveLotteryResultPopup;
	UPROPERTY(Transient)
		class UB2UIAllSelectiveLotteryCostumePopup* CreatedAllSelectiveLotteryCostumePopup;

private:
	//Tab Menu
	struct MailTab
	{
		TWeakObjectPtr<UB2Button>	Button;
		TWeakObjectPtr<UImage>		SelectedImage;
		TWeakObjectPtr<UImage>		UnselectedImage;
		TWeakObjectPtr<UTextBlock>	SelectedText;
		TWeakObjectPtr<UTextBlock>	UnselectedText;
		TArray<TWeakObjectPtr<UB2UIMailContent>> Mails;
		int32 MailCount;
		MailTab() : MailCount(0) {}
	};
	
	TArray<MailTab> MailTabs;	
	MailTabIndex CurrentTabIndex;
	TWeakObjectPtr<UVerticalBox> VB_MailList;
	TWeakObjectPtr<UB2Button> BTN_ReceiveAll;
	TWeakObjectPtr<UB2Button> BTN_UpgradeStoneReceiveAll;
	TWeakObjectPtr<UB2Button> BTN_RareUpgradeStoneReceiveAll;
	TWeakObjectPtr<UTextBlock> TB_ReceiveAll;
	TWeakObjectPtr<UTextBlock> TB_UpgradeStone;
	TWeakObjectPtr<UTextBlock> TB_EmptySlot;
	//Counter
	TWeakObjectPtr<UTextBlock> TB_CurrentMail;	
	TWeakObjectPtr<UCanvasPanel> X_CP_UIPCreateBase;
	
	int64 MailIdToOpen;
	class B2UIMailReceiveState* MailReceiveState;
	bool IsTouch;

	TArray<std::shared_ptr<class issue>> Issues;
};

#undef TAB_MENU_SLOT

#define TAB_MENU_SLOT(MenuName, TabElem)	\
	GET_SLOT_BYNAME(UB2Button, TabElem.Button, BTN_##MenuName##Tab);					\
	GET_SLOT_BYNAME(UImage, TabElem.SelectedImage, IMG_##MenuName##Tab_Selected);		\
	GET_SLOT_BYNAME(UImage, TabElem.UnselectedImage, IMG_##MenuName##Tab_Unselected);	\
	GET_SLOT_BYNAME(UTextBlock, TabElem.SelectedText, TB_##MenuName##Tab_Selected);		\
	GET_SLOT_BYNAME(UTextBlock, TabElem.UnselectedText, TB_##MenuName##Tab_Unselected);