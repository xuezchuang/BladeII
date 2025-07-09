// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIShortagePopupManager.h"
#include "B2UIManager.h"
#include "B2UIGoodsShortcutTooltipPopup.h"
#include "B2UILinkManager.h"

#include "Event.h"
#include "Retailer.h"

B2UIShortagePopupManager::B2UIShortagePopupManager()
	: GetContentsOpenStatusTicket(0)
	, CheckItemRefID(0)
{
	GoodsButtonType = EGoodsButtonType::EGoodsButtonType_NONE;
	IsContentsOpened.Empty();
}

B2UIShortagePopupManager::~B2UIShortagePopupManager()
{
}

void B2UIShortagePopupManager::SubscribeEvents()
{
	GetContentsOpenStatusTicket = DeliveryGetContentsOpenStatusClass<FB2ResponseGetContentsOpenStatusPtr>::GetInstance().Subscribe(
		[this](const FB2ResponseGetContentsOpenStatusPtr& ptr)
	{
		this->UpdateGetContentsOpenStatus(ptr);
	}
	);
}

void B2UIShortagePopupManager::UnsubscribeEvents()
{
	DeliveryGetContentsOpenStatusClass<FB2ResponseGetContentsOpenStatusPtr>::GetInstance().Unsubscribe(GetContentsOpenStatusTicket);
}

void B2UIShortagePopupManager::CheckContentsInfoShortcut(int32 RefID)
{
	B2_SCOPED_TRACK_LOG_AUTO();
	CheckItemRefID = RefID;
	GoodsButtonType = EGoodsButtonType::EGoodsButtonType_InfoShortcut;

	CheckGetContetnsOpenStatus();
}

void B2UIShortagePopupManager::CheckContentsShortageShortcut(int32 RefID)
{
	B2_SCOPED_TRACK_LOG_AUTO();
	CheckItemRefID = RefID;
	GoodsButtonType = EGoodsButtonType::EGoodsButtonType_ShortageShortcut;

	CheckGetContetnsOpenStatus();
}

void B2UIShortagePopupManager::CheckGetContetnsOpenStatus()
{
	SubscribeEvents();
	ReqGetContetnsOpenStatus(CheckItemRefID);
}

///////////////////////////////////////////////////////////////////////////////////////
void B2UIShortagePopupManager::PopupGoToShopForGemMessage()
{
	// 사용자 현질 유도의 관문
	B2_SCOPED_TRACK_LOG_AUTO();
	CreateGoodsShortcutTooltipPopup(FItemRefIDHelper::GetGoodsID_Gem());
}

void B2UIShortagePopupManager::PopupGoToShopForGoldMessage()
{
	B2_SCOPED_TRACK_LOG_AUTO();
	CreateGoodsShortcutTooltipPopup(FItemRefIDHelper::GetGoodsID_Gold());
}

void B2UIShortagePopupManager::PopupGotoShopForDarkCrystalMessage()
{
	B2_SCOPED_TRACK_LOG_AUTO();
	CreateGoodsShortcutTooltipPopup(FItemRefIDHelper::GetGoodsID_DarkCrystal());
}

void B2UIShortagePopupManager::PopupGotoShopForFamePointMessage()
{
	B2_SCOPED_TRACK_LOG_AUTO();
	CreateGoodsShortcutTooltipPopup(FItemRefIDHelper::GetGoodsID_FamePoint());
}

void B2UIShortagePopupManager::PopupGotoShopForStageBoostMessage()
{
	B2_SCOPED_TRACK_LOG_AUTO();
	CreateGoodsShortcutTooltipPopup(FItemRefIDHelper::ITEM_REF_ID_STAGE_BOOST);
}

void B2UIShortagePopupManager::PopupGotoShopForAutoClearTicket()
{
	B2_SCOPED_TRACK_LOG_AUTO();
	CreateGoodsShortcutTooltipPopup(FItemRefIDHelper::GetGoodsID_StageAutoClearTicket());
}

void B2UIShortagePopupManager::PopupGotoFriendForFriendShip()
{
	B2_SCOPED_TRACK_LOG_AUTO();
	CreateGoodsShortcutTooltipPopup(FItemRefIDHelper::GetGoodsID_FriendshipPoint());
}

///////////////////////////////////////////////////////////////////////////////////////
void B2UIShortagePopupManager::PopupGoToControlGameModeMessage(int32 _ScrollType)
{
	B2_SCOPED_TRACK_LOG_AUTO();
	CheckContentsShortageShortcut(_ScrollType);
}

void B2UIShortagePopupManager::PopupGoToCounterDungeonForUpgradeStone(int32 _StoneType)
{
	B2_SCOPED_TRACK_LOG_AUTO();
	CheckContentsShortageShortcut(_StoneType);
}

void B2UIShortagePopupManager::PopupGotoEtherPieceMessage()
{
	B2_SCOPED_TRACK_LOG_AUTO();
	CheckContentsShortageShortcut(FItemRefIDHelper::ITEM_REF_ID_ETHER_PIECE);
}

///////////////////////////////////////////////////////////////////////////////////////
void B2UIShortagePopupManager::PopupGoToHeroTowerMessage()
{
	B2_SCOPED_TRACK_LOG_AUTO();

	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		UIMgrInst->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GoToHeroTower")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::ConfirmOrCancel,
			FMsgPopupOnClick::CreateLambda([]() {

			if (ShouldShowGDCVersionContent())
			{ // [GDC] Feb 2017
				DevShowNotReadyYetPopup();
			}
			else
			{
				UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::HeroTowerReady);
			}
		})
		);
	}
}

void B2UIShortagePopupManager::PopupGoToBossPieceChapterMessage(int32 _id, FText _msg)
{
	B2_SCOPED_TRACK_LOG_AUTO();

	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		UIMgrInst->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			_msg,
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::ConfirmOrCancel,
			FMsgPopupOnClick::CreateLambda([]() {

			DevShowNotReadyYetPopup();
			/*if (ShouldShowGDCVersionContent())
			{ // [GDC] Feb 2017
			DevShowNotReadyYetPopup();
			}
			else
			{
			int32 ClientStageID = GLOBALRELICMANAGER.getRelicOpenStageID(nRelicID);

			int32 nMapID = (int32)ClientStageID / 10;
			int32 nStageID = ClientStageID % 10 == 0 ? 10 : ClientStageID % 10;

			FB2UILinkManager::LinkUIScene(EUIScene::Chapter, nMapID, nStageID); //임시...
			//UB2UIManager::GetInstance()->ChangeUIScene(EUIScene::HeroTowerReady);
			}*/
		})
		);
	}
}

void B2UIShortagePopupManager::CreateGoodsShortcutTooltipPopup(int32 _ItemRefID)
{
	auto* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		auto* pUI = UIMgrInst->OpenUI<UB2UIGoodsShortcutTooltipPopup>(UIFName::GoodsShortcutTooltipPopup);
		pUI->SetItemInfo(_ItemRefID, EGoodsButtonType::EGoodsButtonType_ShortageShortcut);
	}
}

void B2UIShortagePopupManager::CreateGoodsOpenStatusTooltipPopup(int32 _ItemRefID, TArray<bool> _IsContentsOpened, EGoodsButtonType _ButtonType)
{
	auto* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		auto* pUI = UIMgrInst->OpenUI<UB2UIGoodsShortcutTooltipPopup>(UIFName::GoodsShortcutTooltipPopup);
		pUI->SetOpenStageItemInfo(_ItemRefID, _IsContentsOpened, _ButtonType);
	}
}

void B2UIShortagePopupManager::ReqGetContetnsOpenStatus(int32 linkKey)
{
	data_trader::Retailer::GetInstance().RequestGetContentsOpenStatus(linkKey);
}

void B2UIShortagePopupManager::UpdateGetContentsOpenStatus(const FB2ResponseGetContentsOpenStatusPtr& ptr)
{
	IsContentsOpened.Empty();

	for (auto El : ptr->contents)
	{
		IsContentsOpened.Add(El->opened);
	}
	CreateGoodsOpenStatusTooltipPopup(CheckItemRefID, IsContentsOpened, GoodsButtonType);

	CheckItemRefID = 0;

	UnsubscribeEvents();
}