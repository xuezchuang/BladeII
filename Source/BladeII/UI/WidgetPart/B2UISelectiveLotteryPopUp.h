#pragma once

#include "B2UIWidget.h"
#include "B2UICollectBookItemDetail.h"
#include "B2UISelectiveLotteryCostumeRow.h"
#include "B2UISelectiveLotteryPopup.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISelectiveLotteryItemRow : public UB2UIWidget
{
	GENERATED_BODY()

public:
	void SetSelectiveLotteryPopup(class UB2UISelectiveLotteryPopup* InLotteryPopup) { LotteryPopup = InLotteryPopup; }
	EPCClass GetItemCharacterClass() const { return CharacterClass; }
	class UB2UISlotItem* SetItemRefId(int32 index, int32 RefId);	

	void SetItemCharacterClass(EPCClass InEPCClass) { CharacterClass = InEPCClass; }
	void SetEnableReadingGlasses(bool Enable);

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION()
		void SelectItem(UB2UISlotItem* ClickedItem);

private:
	TWeakObjectPtr<class UB2UISlotItem> UIP_ItemElem01;
	TWeakObjectPtr<class UB2UISlotItem> UIP_ItemElem02;
	TWeakObjectPtr<class UB2UISlotItem> UIP_ItemElem03;
	TWeakObjectPtr<class UB2UISlotItem> UIP_ItemElem04;

	class UB2UISelectiveLotteryPopup* LotteryPopup;

	EPCClass CharacterClass;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISelectiveLotteryItemDetail : public UB2UICollectBookItemDetail
{
	GENERATED_BODY()

public:
	void SetItemIconDefaultTemplate(int32 ItemGrade, EItemCollectionState ItemCollectionState);
};


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISelectiveLotteryPopup : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	void SetMailBox(class UB2UIMail* InMailBox) { MailBox = InMailBox; }
	void AddBackWidget();
		
	void SetItemRefIds(const TArray<int32>& ItemRefIds);
	void SetItemRefIds_AutoSort(const TArray<int32>& ItemRefIds);

	virtual void SetSelectedItemRefId(int32 ToSet);

	void SetTitleText(const FText& Title);

	void Close() { CloseMe(); }

	UFUNCTION()
	void HideDetailView();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void CloseWidgetDelegate() override;

	UFUNCTION()
		void CloseMe();

	UFUNCTION()
		void ShowSelectedItemDetail(class UB2UISlotItem* SelectedItem);

	UFUNCTION()
		void CloseSelectedItemDetail();

	UFUNCTION(BlueprintCallable, Category="BladeII")
		virtual void RequestItemSelected();

	UFUNCTION(BlueprintImplementableEvent, Category ="BladeII")
		void OnClickSelectedConfirm();
	
protected:
	UPROPERTY(EditAnywhere, Category="BladeII")
	TSubclassOf<UB2UISelectiveLotteryItemRow> ItemListRowUIClass;

protected:
	TWeakObjectPtr<class UB2UISlotItem> UIP_SelectedItem;

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_SelectFullText;

	TWeakObjectPtr<UVerticalBox> VB_ItemList;

	TWeakObjectPtr<UB2Button> BTN_Select;
	TWeakObjectPtr<UTextBlock> TB_Select;

	TWeakObjectPtr<UB2Button> BTN_Close;
	TWeakObjectPtr<UB2Button> BTN_ItemDetailBackGround;

	TWeakObjectPtr<UB2UISelectiveLotteryItemDetail> UIP_SelectedItemDetail;
	TWeakObjectPtr<class UB2UICollectBookItemDetail> UIP_ItemDetailView;
	
	class UB2UIMail* MailBox;
	int32 SelectedId;

	TArray<TWeakObjectPtr<class UB2UISlotItem>> ItemUILists;
};

DECLARE_DELEGATE_OneParam(FOnSelectItemSlot, int32);

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISelectiveLotteryItemSlot : public UB2UIWidgetBase
{
	GENERATED_BODY()

public:
	void SetItemData(int32 InItemRefId, int32 Num);
	void SetItemData(int32 InItemRefId);
	int32 GetItemRefId() { return ItemRefId; }
	void SetSelected(bool bToSet);

	bool IsSelected() {	return bSelected; }

	void SetSelectDelegate(const FOnSelectItemSlot& ToSetDelegate);

protected:
	virtual void CacheAssets() override;	
	virtual void BindDelegates() override;

	UFUNCTION()
		void OnClickBTNArea();

private:
	void SetItemIcon(int32 InItemRefId);

private:
	TWeakObjectPtr<UB2Button> BTN_Area;
	TWeakObjectPtr<UImage> IMG_Icon;
	TWeakObjectPtr<UTextBlock> TB_ItemName;
	TWeakObjectPtr<UTextBlock> TB_ItemAmount;
	TWeakObjectPtr<UOverlay> OV_Selected;

	int32 ItemRefId;
	bool bSelected;
	FOnSelectItemSlot SelectDelegate;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UISelectiveLotteryConsumItemPopup : public UB2UIWidget
{
	GENERATED_BODY()

public:
	void SetMailBox(class UB2UIMail* InMailBox) { MailBox = InMailBox; }

	void SetItemData(const TArray<int32>& ItemRefIds, const TArray<int32>& ItemNums);

	void SetSelectedItemRefId(int32 ToSet);

	void SetTitleText(const FText& Title);

	void Close() { CloseMe(); }

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

	UFUNCTION()
		void CloseMe();
	
	UFUNCTION()
		void RequestItemSelected();

private:
	TArray<TWeakObjectPtr<UB2UISelectiveLotteryItemSlot>> UIP_ItemSlots;

	TWeakObjectPtr<UTextBlock> TB_Title;
	TWeakObjectPtr<UTextBlock> TB_SelectFullText;

	TWeakObjectPtr<UVerticalBox> VB_ItemList;

	TWeakObjectPtr<UB2Button> BTN_SelectConfirm;
	TWeakObjectPtr<UTextBlock> TB_SelectConfirm;

	TWeakObjectPtr<UB2Button> BTN_Close;
	
	class UB2UIMail* MailBox;
	int32 SelectedId;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIAllSelectiveLotteryPopupBase : public UB2UISelectiveLotteryPopup
{
	GENERATED_BODY()

public:
	virtual void InitLotterySetting() { };

	UFUNCTION(BlueprintImplementableEvent, Category = "BladeII")
		void EventChangedHeroMgmtPCClass(EPCClass InSelectedClass);

	UFUNCTION()
		void OnClickedBTNGladiator();
	UFUNCTION()
		void OnClickedBTNAssassin();
	UFUNCTION()
		void OnClickedBTNWizard();
	UFUNCTION()
		void OnClickedBTNFighter();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void SetItemSlotVisible(EPCClass InEPCClass) { };

	void SetButtonClick(EPCClass InEPCClass);

protected:
	EPCClass	CurrentCharacterTab;

public:
	// Other images and/or buttons for each character are under the panel for each character.
	TWeakObjectPtr<UB2Button>		BTN_SelectGladiator;
	TWeakObjectPtr<UB2Button>		BTN_SelectAssassin;
	TWeakObjectPtr<UB2Button>		BTN_SelectWizard;
	TWeakObjectPtr<UB2Button>		BTN_SelectFighter;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIAllSelectiveLotteryPopup : public UB2UIAllSelectiveLotteryPopupBase
{
	GENERATED_BODY()

public:
	virtual void InitLotterySetting() override;

	void SetLotterySurpassLevel(const int32 InSurpassLevel) { LotterySurpassLevel = InSurpassLevel; }
	int32 GetLotterySurpassLevel() const { return LotterySurpassLevel; }
	void UpdateItemSurpassLevel();

protected:
	virtual void SetItemSlotVisible(EPCClass InEPCClass) override;

private:
	int32		LotterySurpassLevel;
};

UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIAllSelectiveLotteryCostumePopup : public UB2UIAllSelectiveLotteryPopupBase
{
	GENERATED_BODY()

public:
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	virtual void InitLotterySetting() override;
	virtual void SetSelectedItemRefId(int32 ToSet) override;

	void SetItemRefIds_AutoCostumeSort(const TArray<int32>& ItemRefIds);

	//UFUNCTION(BlueprintCallable, Category = "BladeII")
		virtual void RequestItemSelected() override;

	//UFUNCTION()
		void ShowSelectedItemDetail(class UB2UISelectiveLotteryCostumeSlot* SelectedItem);

	UFUNCTION()
		void HideCostumeDetailView();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void SetItemSlotVisible(EPCClass InEPCClass) override;

protected:
	UPROPERTY(EditAnywhere, Category = "BladeII")
		TSubclassOf<UB2UISelectiveLotteryCostumeRow> CostumeListRowUIClass;

	TWeakObjectPtr<class UB2UISelectiveLotteryCostumeDT> UIP_CostumeDetailView;
	TWeakObjectPtr<class UB2UISelectiveLotteryCostumeSlot> UIP_SelectedCostume;
	TArray<TWeakObjectPtr<class UB2UISelectiveLotteryCostumeSlot>> CostumeUILists;
};