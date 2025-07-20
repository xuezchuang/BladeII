// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIItemChangePopupMode.h"
#include "B2UIManager.h"
#include "B2Inventory.h"
#include "BladeIIGameImpl.h"


void UB2UIItemChangePopupMode::AddHandler(EUIItemChangePopupMode ButtonType, const FItemChangePopupMode& Handler)
{
	auto Found = ClickHandlerMap.Find(ButtonType);
	if (Found)
	{
		*Found = Handler;
		return;
	}

	ClickHandlerMap.Add(ButtonType, Handler);
}

void UB2UIItemChangePopupMode::Init()
{
	Super::Init();	
}

void UB2UIItemChangePopupMode::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIItemChangePopupMode::CacheAssets()
{
	Super::CacheAssets();
}

void UB2UIItemChangePopupMode::UpdateStaticText()
{
	Super::UpdateStaticText();
}

void UB2UIItemChangePopupMode::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIItemChangePopupMode::ItemSlotSetting(const FB2Item& ItemInfo)
{

	FLocalCharacterData* CachedCharacterStore = &BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();
	FPCItemContainer &ItemContainer = CachedCharacterStore->GetUserAllItems();
	bool IsEquipState =  ItemContainer.IsInItemPresets(IntToPCClass(ItemInfo.AllowedPCClass), ItemInfo.InstanceUID);

	if (IsEquipState)
		HeroButtonHiddenEvent();

	if (ItemInfo.StarGrade == 1)
		OptionButtonHiddenEvent();

	
	if (ItemInfo.Quality < MAX_ITEM_QUALITY)
		EnableQualityChangeBTN();
	else
		DisableQualityChangeBTN();
}

void UB2UIItemChangePopupMode::CloseWidgetDelegate()
{
	OnClickDestorySelf();
}

void UB2UIItemChangePopupMode::OnClickBtnOption()
{
	auto Found = ClickHandlerMap.Find(EUIItemChangePopupMode::Option);

	if (Found && Found->IsBound())
		Found->Execute();

	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIItemChangePopupMode::OnClickBtnEnhance()
{
	auto Found = ClickHandlerMap.Find(EUIItemChangePopupMode::Enhance);

	if (Found && Found->IsBound())
		Found->Execute();

	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIItemChangePopupMode::OnClickBtnHero()
{
	auto Found = ClickHandlerMap.Find(EUIItemChangePopupMode::Hero);

	if (Found && Found->IsBound())
		Found->Execute();

	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIItemChangePopupMode::OnClickBtnQuality()
{
	auto Found = ClickHandlerMap.Find(EUIItemChangePopupMode::Quality);

	if (Found && Found->IsBound())
		Found->Execute();

	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIItemChangePopupMode::OnClickDestorySelf()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::MsgEnhancementChangeMode);
}
