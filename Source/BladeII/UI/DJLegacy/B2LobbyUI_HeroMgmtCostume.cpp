// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_HeroMgmtCostume.h"
#include "B2CostumeDataStore.h"
#include "B2UIManager_Lobby.h"
#include "B2LobbyGameMode.h"
#include "BladeIIGameImpl.h"
#include "B2UILobby_CostumeMain.h"

void UB2LobbyUI_HeroMgmtCostume::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	HeroMgmtCostume_Init();
}

void UB2LobbyUI_HeroMgmtCostume::DestroySelf()
{
	BladeIIGameImpl::GetCostumeDataStore().SetLocalVisibleMode();
	Super::DestroySelf();
}

void UB2LobbyUI_HeroMgmtCostume::OnHeroPCChanged(EPCClass UpdatedClass)
{
	Super::OnHeroPCChanged(UpdatedClass);
	SyncCostumeButton();
}

void UB2LobbyUI_HeroMgmtCostume::InternalSubscribeEvents()
{
	Super::InternalSubscribeEvents();

	Issues.Add(CostumeEquipSlotVisibleClass<bool>::GetInstance().Subscribe2(
		[this](bool bInven)
	{
		if (bInven)
			LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(GetCurrentPCClass());

		DoMarkRedDot();
	}
	));

	BladeIIGameImpl::GetCostumeDataStore().OnChangeCostumeVisible.AddUObject(this, &UB2LobbyUI_HeroMgmtCostume::OnChangeVisible);
	BladeIIGameImpl::GetCostumeDataStore().OnChangeCostumeVisible.AddUObject(this, &UB2LobbyUI_HeroMgmtCostume::OnChangeVisibleMeshPart);
	BladeIIGameImpl::GetCostumeDataStore().OnChangedVisibleMode.AddUObject(this, &UB2LobbyUI_HeroMgmtCostume::OnChangeVisibleMode);
}

void UB2LobbyUI_HeroMgmtCostume::InternalUnsubscribeEvents()
{
	Super::InternalUnsubscribeEvents();
	Issues.Empty();
}

bool UB2LobbyUI_HeroMgmtCostume::RedDotCondition_ByCostumeMode(EPCClass PCClass)
{
	if (BladeIIGameImpl::GetCostumeDataStore().GetCachedCostumePageType() == ECostumePage::Store)
		return false;
	
	return Super::RedDotCondition_ByCostumeMode(PCClass);
}

void UB2LobbyUI_HeroMgmtCostume::HeroMgmtCostume_Init()
{
	HeroMgmtCostume_CacheAssets();
	HeroMgmtCostume_BindDelegates();
	OnChangeVisibleMode(BladeIIGameImpl::GetCostumeDataStore().IsPreviewMode());
}

void UB2LobbyUI_HeroMgmtCostume::HeroMgmtCostume_CacheAssets()
{
	GET_SLOT(UCheckBox, CB_ShowCostume_Head);
	GET_SLOT(UCheckBox, CB_ShowCostume_Weapon);
	GET_SLOT(UCheckBox, CB_ShowCostume_Amor);

	GET_SLOT(UB2Button, BTN_HeroInfo);
}

void UB2LobbyUI_HeroMgmtCostume::HeroMgmtCostume_BindDelegates()
{
	if (CB_ShowCostume_Head.IsValid())
	{
		CB_ShowCostume_Head->OnCheckStateChanged.AddDynamic(this, &UB2LobbyUI_HeroMgmtCostume::OnShowCostume_HeadButtonPressed);
	}
	
	if (CB_ShowCostume_Weapon.IsValid())
	{
		CB_ShowCostume_Weapon->OnCheckStateChanged.AddDynamic(this, &UB2LobbyUI_HeroMgmtCostume::OnShowCostume_WeaponButtonPressed);
	}

	if (CB_ShowCostume_Amor.IsValid())
	{
		CB_ShowCostume_Amor->OnCheckStateChanged.AddDynamic(this, &UB2LobbyUI_HeroMgmtCostume::OnShowCostume_AmorButtonPressed);
	}

	BIND_CLICK_FUNC_TO_BTN(BTN_HeroInfo, &UB2LobbyUI_HeroMgmtCostume::OnClickBTN_HeroInfo);
}

void UB2LobbyUI_HeroMgmtCostume::SyncCostumeButton()
{
	const TArray<ECostumeEquipPlace>& CostumeArray = FCostumeDataStore::GetCostumeEquipArray().CostumeEquipArray;
	for (ECostumeEquipPlace Place : CostumeArray)
		OnChangeVisible(GetCurrentPCClass(), Place, BladeIIGameImpl::GetCostumeDataStore().GetCostumeVisible(GetCurrentPCClass(), Place));
}

void UB2LobbyUI_HeroMgmtCostume::OnShowCostume_HeadButtonPressed(bool IsCheck)
{
	BladeIIGameImpl::GetCostumeDataStore().SetCostumeVisible(GetCurrentPCClass(), ECostumeEquipPlace::ECEP_Helmet, IsCheck);
}

void UB2LobbyUI_HeroMgmtCostume::OnShowCostume_WeaponButtonPressed(bool IsCheck)
{
	BladeIIGameImpl::GetCostumeDataStore().SetCostumeVisible(GetCurrentPCClass(), ECostumeEquipPlace::ECEP_Weapon, IsCheck);
}

void UB2LobbyUI_HeroMgmtCostume::OnShowCostume_AmorButtonPressed(bool IsCheck)
{
	BladeIIGameImpl::GetCostumeDataStore().SetCostumeVisible(GetCurrentPCClass(), ECostumeEquipPlace::ECEP_Body, IsCheck);
}

void UB2LobbyUI_HeroMgmtCostume::OnClickBTN_HeroInfo()
{
	// 팝업 추가할 panel 정보.. 이 파트를 사용하는 UI 페이지 자체적으로 있는 경우.
	UB2UILobby_CostumeMain* OwnerAsInvenUI = CachedLobbyGM ? Cast<UB2UILobby_CostumeMain>(CachedLobbyGM->DJLegacy_GetCurrLobbyUIPage()) : nullptr;
	UB2LobbyUserWidget* OwnerUI = OwnerAsInvenUI;
	UPanelWidget* PopupAddPanel = OwnerAsInvenUI ? OwnerAsInvenUI->GetDetailPopupParentPanel(true) : nullptr;
	if (!OwnerUI)
	{ // 별도로 지정된 거 없음 이쪽에 설치된 패널에
		OwnerUI = this;
		PopupAddPanel = GetInfoPopupParentPanel(true);
	}

	// 세부 스탯 pop-up
	DJLegacy_OpenLobbySubPopupClass<ELobbySubPopups, FDJLegacySubpopupAddPanelInfo>::GetInstance().Signal(ELobbySubPopups::ELSPU_CharStatDetail, FDJLegacySubpopupAddPanelInfo(OwnerUI, PopupAddPanel));
}

void UB2LobbyUI_HeroMgmtCostume::OnChangeVisible(EPCClass InPCClass, ECostumeEquipPlace InEquipPlace, bool bVisible)
{
	switch (InEquipPlace)
	{
	case ECostumeEquipPlace::ECEP_Weapon:
		if (CB_ShowCostume_Weapon.IsValid())
			CB_ShowCostume_Weapon->SetIsChecked(bVisible);
		break;
	case ECostumeEquipPlace::ECEP_Helmet:
		if (CB_ShowCostume_Head.IsValid())
			CB_ShowCostume_Head->SetIsChecked(bVisible);
		break;
	case ECostumeEquipPlace::ECEP_Body:
		if (CB_ShowCostume_Amor.IsValid())
			CB_ShowCostume_Amor->SetIsChecked(bVisible);
		break;
	}
}

void UB2LobbyUI_HeroMgmtCostume::OnChangeVisibleMeshPart(EPCClass InPCClass, ECostumeEquipPlace InEquipPlace, bool bVisible)
{
	LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(InPCClass);
}

void UB2LobbyUI_HeroMgmtCostume::OnChangeVisibleMode(bool bPreview)
{
	SyncCostumeButton();
}
