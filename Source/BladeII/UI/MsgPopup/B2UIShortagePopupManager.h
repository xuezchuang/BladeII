// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../Widget/B2Button.h"
#include "../../Common/CommonStruct.h"

/**
 *
 */
class BLADEII_API B2UIShortagePopupManager
{
//Function
public:

	B2UIShortagePopupManager();
	~B2UIShortagePopupManager();

	void SubscribeEvents();
	void UnsubscribeEvents();

	void CheckContentsInfoShortcut(int32 RefID);
	void CheckContentsShortageShortcut(int32 RefID);
	void CheckGetContetnsOpenStatus();

	// Enter Don't Check
	void PopupGoToShopForGemMessage();
	void PopupGoToShopForGoldMessage();
	void PopupGotoShopForDarkCrystalMessage();
	void PopupGotoShopForFamePointMessage();
	void PopupGotoShopForStageBoostMessage();
	void PopupGotoShopForAutoClearTicket();
	void PopupGotoFriendForFriendShip();

	// Enter Check
	void PopupGoToControlGameModeMessage(int32 _ScrollType);
	void PopupGoToCounterDungeonForUpgradeStone(int32 _StoneType);
	void PopupGotoEtherPieceMessage();
	// Message Popup
	void PopupGoToHeroTowerMessage();
	void PopupGoToBossPieceChapterMessage(int32 _id, FText _msg);

	// Don't Check Tooltip
	static void CreateGoodsShortcutTooltipPopup(int32 _type);
	// Check Tooltip
	static void CreateGoodsOpenStatusTooltipPopup(int32 _type, TArray<bool> _IsContentsOpened, EGoodsButtonType _ButtonType);

	void ReqGetContetnsOpenStatus(int32 linkKey);
	void UpdateGetContentsOpenStatus(const FB2ResponseGetContentsOpenStatusPtr& ptr);

protected:

private:

//Variable
public:

protected:

private:

	uint32				GetContentsOpenStatusTicket;
	uint32				CheckItemRefID;
	EGoodsButtonType	GoodsButtonType;

	TArray<bool>		IsContentsOpened;
};
