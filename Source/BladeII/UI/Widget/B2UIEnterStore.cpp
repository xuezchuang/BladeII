// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIEnterStore.h"
#include "B2UIManager.h"
#include "B2RedDotManager.h"



#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIEnterStore::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2Button, BTN_Equip);
	GET_SLOT(UB2Button, BTN_Goods);

	GET_SLOT(UTextBlock, TB_GoodsStore);
	GET_SLOT(UTextBlock, TB_SummonItem);
}

void UB2UIEnterStore::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Equip, &UB2UIEnterStore::OnClickBTN_Equip);
	BIND_CLICK_FUNC_TO_BTN(BTN_Goods, &UB2UIEnterStore::OnClickBTN_Goods);
}

void UB2UIEnterStore::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_GeneralStore")));
}

void UB2UIEnterStore::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Equip.Get()), &UB2UIEnterStore::RedDotCondition_EquipStore);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(BTN_Goods.Get()), &UB2UIEnterStore::RedDotCondition_GoodsStore);
}

void UB2UIEnterStore::UpdateStaticText()
{
	if (TB_SummonItem.IsValid())
		TB_SummonItem->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_SummonItem")));

	if (TB_GoodsStore.IsValid())
		TB_GoodsStore->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_GoodsStore")));
}

void UB2UIEnterStore::Init()
{
	Super::Init();
}

void UB2UIEnterStore::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIEnterStore::OnClickBTN_Equip()
{
	EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::EquipStore));
}

void UB2UIEnterStore::OnClickBTN_Goods()
{

	EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::Store));
}

bool UB2UIEnterStore::RedDotCondition_EquipStore()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_FreeItem() || BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Mileage_Item();
}

bool UB2UIEnterStore::RedDotCondition_GoodsStore()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_Mileage_Cash();
}
