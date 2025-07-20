// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "B2UICollectBookItemDetail.h"
#include "B2ScrollBox.h"
#include "../B2RichTextBlock.h"
#include "B2UICollectBookMain.generated.h"

struct FCollectionTab
{
public:
	FCollectionTab() {}
	FCollectionTab(ECollectionType ItemType, const FString& LocalizeKey) : CollectionType(ItemType), CollectCount(0), MaxCollectCount(0), TabLocalizeKey(LocalizeKey) {}

	void	InitTabDynamicTemplate(EPCClass CharacterClass);
	void	InitTabDefaultTemplate();
	FText	GetTabDisplayString();

	void	SetAlarmIcon(bool bVisible);
	void	SetActive(bool bActive);

public:

	TWeakObjectPtr<UButton>		BTN_Tab;
	TWeakObjectPtr<UTextBlock>	TB_TabName;
	TWeakObjectPtr<UB2UIWidgetBase>	IMG_NewIcon;

	TWeakObjectPtr<UWidgetSwitcher>	WS_Click;

	TWeakObjectPtr<UTextBlock>	TB_TabNameActive;
	TWeakObjectPtr<UB2UIWidgetBase> IMG_NewIconActive;

	ECollectionType CollectionType;

	int32			CollectCount;
	int32			MaxCollectCount;

	FString			TabLocalizeKey;

	bool			bActiveCurrentTab;
};

struct FSelectedIconInfo
{
public:
	FSelectedIconInfo() : ItemClass(EItemClass::EIC_End), SelectedID(0), IconWidget(nullptr) {}
	void UpdateIconInfo(EItemClass InItemClass, class UB2UICollectBookIcon* NewIcon);
	void UpdateSelect(bool bSelect);
	void IconInfoClear() { UpdateIconInfo(EItemClass::EIC_End, nullptr); }

	bool IsSelectedIcon() { return SelectedID > 0 && IconWidget != nullptr; }

public:
	EItemClass				ItemClass;
	int32					SelectedID;	// ItemTemplateID or SetUniqueKey
	UB2UICollectBookIcon*	IconWidget;
};

struct FEquipTypeTab
{
	EPCClass						CharacterClass;
	EItemClass						ItemClass;

	bool							bVisibleTab;

	TWeakObjectPtr<class UB2UICommonTab>		UI_CommonTab;
	TWeakObjectPtr<UB2UnitedWidgetBase>			UI_RedDotWidget;

	FEquipTypeTab() : CharacterClass(EPCClass::EPC_End), ItemClass(EItemClass::EIC_End), bVisibleTab(false) {}

	void Init();
	void InitEquipTypeTab(EPCClass InCharacterClass, EItemClass ItemClass);
	void SetVisible(bool bVisible);
	void DoMarkRedDot();
};
	

struct FCharacterButton
{
	FCharacterButton() : BTN_Area(nullptr), IMG_CharacterSelected(nullptr), IMG_CharacterUnSelected(nullptr), TB_ClassName(nullptr), IMG_NewIcon(nullptr) {}
	
	TWeakObjectPtr<UButton>		BTN_Area;
	TWeakObjectPtr<UImage>		IMG_CharacterSelected;
	TWeakObjectPtr<UImage>		IMG_CharacterUnSelected;
	TWeakObjectPtr<UTextBlock>	TB_ClassName;
	TWeakObjectPtr<UB2UIWidgetBase>	IMG_NewIcon;

	void SetSelect(bool bSelect);
	void SetAlarmIcon(bool bVisible);
};

struct FIconRow
{
	TWeakObjectPtr<class UB2UICollectBookIconRow> UIP_IconRow;
	TWeakObjectPtr<UTextBlock>		TB_IconRowText;
	TWeakObjectPtr<UCanvasPanel>	CP_IconRow;

	FIconRow() : TB_IconRowText(nullptr), CP_IconRow(nullptr) {}

	void SetVisible(bool bVisible);
	void SetText(const FText& RowText);
};


UCLASS()
class BLADEII_API UB2UICollectBookMain : public UB2UIWidget
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void UpdateStaticText() override;
	virtual void OnSceneOpen(EUIScene InOpenedScene) override;
public:
	virtual void NativeConstruct() override;
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;
	

	UB2UICollectBookMain(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void OnClickedItemIcon(class UB2UICollectBookIcon* IconWidget);

	UFUNCTION()
	void OnClickedDetailViewIcon(class UB2UICollectBookIcon* IconWidget);


	UFUNCTION()
	void OnClickedWingGradeButton(int32 WingGrade);

	void SetCharacterClass(EPCClass InCharacterClass);
	const TArray<FB2Item>& GetCollectBookCurrentEquipItems();

	void LinkUICollectBook(EPCClass InCharacterClass, EItemClass InItemClass);

private:
	void SetHeaderTitle(EPCClass InCharacterClass);

	void ShowCollectionTabButtonList(bool bVisible);
	void InitIconRowWidget(class UB2UICollectBookIconRow* IconRowWidget);
	UB2UICollectBookIconRow* GetOrCreateIconRowWidget(int32 RowIndex);

	bool GetCollectionTab(ECollectionType CollectionType, FCollectionTab& OutTab);
	FCollectionTab& GetCollectionTab(ECollectionType CollectionType);
	void InitCollectionTab(EPCClass InCharacterClass);
	
	void OnClickCollectionTab(ECollectionType CollectionType, bool bForce = false);
	void OnChangedCollectionTab(ECollectionType PrevCollection, ECollectionType NewCollection);
	void UpdateCollectionTab(ECollectionType CollectionType, EItemClass FocusItemClass = EItemClass::EIC_End);
	void UpdateEquipItemByChangeTab(ECollectionType PrevCollection, ECollectionType NewCollection);

	void UpdateSingleIconRow(class UB2UICollectBookIconRow* IconRowWidget, const TArray<int32>& ItemIDs, EItemClass ItemClass);
	FText GetIconRowText(EItemClass ItemClass, int32 ID);

	TArray<int32> GetIconRowItemIDs(EItemClass ItemClass, int32 GroupID);
	void UpdateCollectionPage(ECollectionType CollectionType, EItemClass ItemClass);

	void UpdateWingPage(bool bVisible);

	void ClearLobbyCharacterView();
	void ClearAllEquipItems();
	void OnUpdateLobbyCharacterView(EItemClass ItemClass, int32 ItemTemplateID, bool bEquip);
	void OnUpdateLobbyCharacterViewWing(int32 WingGrade, bool bEquip);
	void SetCurrentEquipSetItem(EItemClass ItemClass, int32 ItemTemplateID, bool bEquip);
	void SetCurrentEquipItem(EItemClass ItemClass, int32 ItemTemplateID, bool bEquip);
	int32 GetCurrentEquipItemTemplateID(EItemClass ItemClass);
	void ResetAllClickedIcons();

	void UpdateShowWingButton(bool bShowState);

	bool GetSelectIconInfo(EItemClass ItemClass, FSelectedIconInfo& OutIconInfo);
	void SetSelectIconInfo(EItemClass ItemClass, UB2UICollectBookIcon*	IconInfo);
	void ClearSelectIconInfo(EItemClass ItemClass);
	void ClearAllSelectIconInfo();
	void ClearIconSelectedImage(EItemClass ItemClass);

	EItemClass	GetItemClassByIndex(ECollectionType CollectionType, int32 Index);
	int32		GetItemClassTypeCount(ECollectionType CollectionType);

	int32 GetEquipTypeTabIndex(EItemClass ItemClass);
	void UpdateAllEquipTypeTabs(ECollectionType CollectionType, EItemClass FocusItemClass = EItemClass::EIC_End);
	void OnClickedEquipTypeTab(ECollectionType CollectionType, EItemClass ItemClass);
	void UpdateEquipTypeTab(ECollectionType CollectionType, EItemClass ItemClass);

	void SetItemNameText(EItemClass ItemClass, int32 ItemID);
	void SetStarGrade(int32 StarGrade);
		
	void ShowItemDetailView(int32 ItemTemplateID);
	void ShowSetDetailView(int32 SetUniqueKey);

	UFUNCTION()
	void HideDetailView();

	void InitCharacterButtons();
	void UpdateCharacterButton(EPCClass InCharacterClass);
	void OnCharacterChange(EPCClass ChangeCharacterClass);
	void UpdateCharacterChange(EPCClass NewCharacterClass);

	void GetSortedSetItemUniqueKeys(TArray<int32>& OutUniqueKeys, EPCClass TargetClass);

	void RequestCollectionReward(ECollectionType CollectionType, int32 RequestID);

	void RequestCollectionReward(int32 ItemTemplateID);
	void RequestSetCollectionReward(int32 GroupID, int32 CharacterType, int32 Grade);

	bool IsEquippedWing();

	void SubscribeEvent();
	void UnSubscribeEvent();

	void OnReceiveRewardCollectionItem(FB2RewardCollectionItemPtr RewardInfo);
	void OnReceiveRewardCollectionSetItem(FB2RewardCollectionSetItemPtr RewardInfo);

	// UIP dynamic load and create..
	void ConditionalCreateItemDetailViewUIP();
	void ConditionalCreateWingPageUIP();
	void DestroyItemDetailViewUIP();
	void DestroyWingPageUIP();

	void FreeupSomeMemory();

protected:

	TArray<FCollectionTab>				CollectionTabList;

	EPCClass		CharacterClass;

	ECollectionType PrevCollectionType;
	ECollectionType CurrentCollectionType;
	EItemClass		CurrentItemClass;
	
	TArray<FSelectedIconInfo>	SelectedIconInfos;
	TArray<FEquipTypeTab>		EquipTypeTabs;

	int32						CurrentEquipTypeTabIndex;
	int32						CurrentTabItemTypeCount;

	TArray<int32>				CurrentSetUniqueKeys;
	TArray<int32>				CurrentTabItems;
	TArray<class UB2UICollectBookItemDetail*>	ItemDetailViewWidgets;

	TArray<FB2Item>		CurrentEquipItems;
	TArray<FB2Item>		SetEquipItems;

	int32	CachedSurpassLevel;
	FB2Item CachedItem;
	
	int32				CurrentEquipSetItemKey;

	bool				bShowDetailView;
	bool				bShowWing;
	int32				EquippedWingGrade;

	bool				bDirtyRequestGetCollection;
	
	uint8				DeliveryRewardCollectionItemTicket;
	uint8				DeliveryRewardCollectionSetItemTicket;
	uint8				LinkUICollectBookTicket;
	float				CachedScrollOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UB2UICollectBookIconRow>	IconRowClassBP;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UB2UICollectBookItemDetail> UIP_ItemDetailViewClass;
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<class UB2UICollectBookWing> UIP_WingPageClass;

protected:

	UFUNCTION()
	void OnClickBTN_CollectionTab();
	UFUNCTION()
	void OnClickBTN_SetTab();
	UFUNCTION()
	void OnClickBTN_WeaponTab();
	UFUNCTION()
	void OnClickBTN_ArmorTab();
	UFUNCTION()
	void OnClickBTN_AccTab();
	UFUNCTION()
	void OnClickBTN_WingTab();
	UFUNCTION()
	void OnClickBTN_UnEquip();
	UFUNCTION()
	void OnShowHeadButtonPressed(bool IsCheck);

	UFUNCTION()
	void OnClickBTN_ShowWing();
	UFUNCTION()
	void OnClickBTN_Gladiator();
	UFUNCTION()
	void OnClickBTN_Assassin();
	UFUNCTION()
	void OnClickBTN_Wizard();
	UFUNCTION()
	void OnClickBTN_Fighter();

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateCharacterChangeBP(EPCClass PrevClass, EPCClass NewClass);


private:

	TWeakObjectPtr<UVerticalBox>	VB_CollectionTab;
	TWeakObjectPtr<UButton>			BTN_CollectionTab;
	TWeakObjectPtr<UTextBlock>		TB_CollectionTab;

	TWeakObjectPtr<UCanvasPanel> CP_ItemPage;
	TWeakObjectPtr<UB2ScrollBox> SB_ItemPage;
	TWeakObjectPtr<UOverlay> O_ItemName;
	TWeakObjectPtr<UTextBlock> TB_ItemName;
	TWeakObjectPtr<UB2Button> BTN_UnEquip;
	TWeakObjectPtr<UTextBlock> TB_UnEquip;
	TWeakObjectPtr<UVerticalBox> VB_IconRows;
	TWeakObjectPtr<UB2RichTextBlock> RTB_RewardDesc;

	TWeakObjectPtr<UCheckBox> CB_ShowHead;
	
	// Wing
	TWeakObjectPtr<UB2Button> BTN_ShowWing;
	TWeakObjectPtr<UTextBlock> TB_ShowWing;
	TWeakObjectPtr<UImage> IMG_ShowWing_BGOn;
	TWeakObjectPtr<UImage> IMG_ShowWing_BGOff;
	TWeakObjectPtr<UImage> IMG_ShowWing_IconOn;
	TWeakObjectPtr<UImage> IMG_ShowWing_IconOff;

	TArray<UB2UICollectBookIconRow*>	ItemIconRows;
	TArray<TWeakObjectPtr<UImage>>	IMG_Stars;

	TWeakObjectPtr<UCanvasPanel> X_CP_WingPagePanel;
	UPROPERTY(Transient)
	class UB2UICollectBookWing* CreatedUIP_WingPage;

	TWeakObjectPtr<UCanvasPanel> X_CP_ItemDetailViewPanel;
	UPROPERTY(Transient)
	UB2UICollectBookItemDetail*	CreatedUIP_ItemDetailView;

	TArray<FCharacterButton>	CharacterChangeButtons;
};

int32 GetTotalItemCount(EPCClass TargetClass, ECollectionType CollectionType);
int32 GetCollectionItemCount(EPCClass TargetClass, ECollectionType CollectionType);
int32 GetEnhancedCollectionItemCount(EPCClass TargetClass, ECollectionType CollectionType);
ECollectionType GetCollectionTypeByItemClass(EItemClass ItemClass);