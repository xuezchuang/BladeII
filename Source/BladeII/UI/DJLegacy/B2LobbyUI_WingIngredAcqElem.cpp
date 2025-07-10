// Fill out your copyright notice in the Description page of Project Settings.

#include "B2LobbyUI_WingIngredAcqElem.h"
#include "Event.h"
#include "B2LobbyGameMode.h"
#include "B2ItemInfo.h"
#include "Retailer.h"

UB2LobbyUI_WingIngredAcqElem::UB2LobbyUI_WingIngredAcqElem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyType = EWingIngredAcqElemType::WAET_EnhanceIngred;
}

void UB2LobbyUI_WingIngredAcqElem::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_IngredIcon);
	GET_SLOT(UTextBlock, TB_AcquirePath);
	GET_SLOT(UTextBlock, TB_Go);
	GET_SLOT(UB2Button, BTN_Go);
	BIND_CLICK_FUNC_TO_BTN(BTN_Go, &UB2LobbyUI_WingIngredAcqElem::OnClickBtnGo);
}

void UB2LobbyUI_WingIngredAcqElem::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);
	
	UpdateStaticWidgets();
}

void UB2LobbyUI_WingIngredAcqElem::DestroySelf()
{
	Super::DestroySelf();

}

void UB2LobbyUI_WingIngredAcqElem::SetAcqElemType(EWingIngredAcqElemType InType)
{
	MyType = InType;

	UpdateWidgetsOnTypeChange();
}

void UB2LobbyUI_WingIngredAcqElem::UpdateStaticWidgets()
{
	if (TB_Go.IsValid())
	{
		TB_Go->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_IngredAcquire_Go")));
	}
}

void UB2LobbyUI_WingIngredAcqElem::UpdateWidgetsOnTypeChange()
{
	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	if (IMG_IngredIcon.IsValid() && ItemInfoTable)
	{
		FSingleItemInfoData* ItemInfoForMyType = ItemInfoTable->GetInfoData(FItemRefIDHelper::GetWingResourceRefIDByAcqElemType(MyType));

		if (ItemInfoForMyType)
		{
			IMG_IngredIcon->SetBrushFromMaterial(ItemInfoForMyType->GetIconMaterial(ItemInfoTable));
		}
	}
	if (TB_AcquirePath.IsValid())
	{
		TB_AcquirePath->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT,
			MyType == EWingIngredAcqElemType::WAET_EnhanceIngred ? TEXT("Wing_IngredAcquire_Shop") :
				(MyType == EWingIngredAcqElemType::WAET_EvolveIngred ? TEXT("Wing_IngredAcquire_CounterDungeon") :
					TEXT("Wing_IngredAcquire_TeamMatch"))
			));	
	}
}

void UB2LobbyUI_WingIngredAcqElem::OnClickBtnGo()
{
	if (CachedLobbyGM && CachedLobbyGM->IsHeroMgmtMode())
	{
		LobbyQuitHeroMgmtModeClass<>::GetInstance().Signal(); // 어떤 경우든 영웅관리 모드 나가게 되는 거..?
	}

	// 각 재료별로 습득할 수 있는 곳으로 이동하는 실질적인 부분이 여기가 되겠다.
	switch (MyType)
	{
	case EWingIngredAcqElemType::WAET_EnhanceIngred: // 상점으로
		
		// PopupGoToShopForGoldMessage 를 참고. 사전에 특정 위치로 StoreTab 을 세팅해야 할 수 있다.
		EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::Store));

		break;

	case EWingIngredAcqElemType::WAET_EvolveIngred: // 반격던전

		// UIModSelect 에서 반격던전 누르면 실행되는 걸로
		data_trader::Retailer::GetInstance().RequestGetCounterDungeon();

		break;

	case EWingIngredAcqElemType::WAET_PropOptionIngred: // 팀대전

		// UIModSelect 에서 팀대전 누르면 실행되는 걸로
		data_trader::Retailer::GetInstance().RequestGetTeamBattleStatus();

		break;
	}

	// 그리고 팝업 쪽에도 신호..
	OnGoBtnClickDelegate.ExecuteIfBound(this);
}