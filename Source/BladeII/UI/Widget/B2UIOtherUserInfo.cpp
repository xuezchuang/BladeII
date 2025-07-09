// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIOtherUserInfo.h"
#include "B2CombatStatEvaluator.h"
#include "B2DynItemIcon_LobbyEquip.h"
#include "B2LobbyUI_ItemDetail.h"
#include "B2LobbyUI_CharStatDetail.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"
#include "B2AndroidBackManager.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIOtherUserInfo::CacheAssets()
{
	Super::CacheAssets();


	GET_SLOT(UB2Button, BTN_Gladiator);
	GET_SLOT(UB2Button, BTN_Assassin);
	GET_SLOT(UB2Button, BTN_Wizard);
	GET_SLOT(UB2Button, BTN_Fighter);

	EquipPlaceIcon.Add(EItemEquipPlace::EIEP_Helmet);
	GET_SLOT_BYNAME(UB2DynItemIcon_LobbyEquip, EquipPlaceIcon[EItemEquipPlace::EIEP_Helmet], HelmetItem);

	EquipPlaceIcon.Add(EItemEquipPlace::EIEP_BodyArmor);
	GET_SLOT_BYNAME(UB2DynItemIcon_LobbyEquip, EquipPlaceIcon[EItemEquipPlace::EIEP_BodyArmor], ArmorItem);

	EquipPlaceIcon.Add(EItemEquipPlace::EIEP_Glove);
	GET_SLOT_BYNAME(UB2DynItemIcon_LobbyEquip, EquipPlaceIcon[EItemEquipPlace::EIEP_Glove], GloveItem);

	EquipPlaceIcon.Add(EItemEquipPlace::EIEP_Shoes);
	GET_SLOT_BYNAME(UB2DynItemIcon_LobbyEquip, EquipPlaceIcon[EItemEquipPlace::EIEP_Shoes], ShoesItem);

	EquipPlaceIcon.Add(EItemEquipPlace::EIEP_Weapon);
	GET_SLOT_BYNAME(UB2DynItemIcon_LobbyEquip, EquipPlaceIcon[EItemEquipPlace::EIEP_Weapon], WeaponItem);

	EquipPlaceIcon.Add(EItemEquipPlace::EIEP_Belt);
	GET_SLOT_BYNAME(UB2DynItemIcon_LobbyEquip, EquipPlaceIcon[EItemEquipPlace::EIEP_Belt], BeltItem);

	EquipPlaceIcon.Add(EItemEquipPlace::EIEP_Necklace);
	GET_SLOT_BYNAME(UB2DynItemIcon_LobbyEquip, EquipPlaceIcon[EItemEquipPlace::EIEP_Necklace], NecklaceItem);

	EquipPlaceIcon.Add(EItemEquipPlace::EIEP_Ring);
	GET_SLOT_BYNAME(UB2DynItemIcon_LobbyEquip, EquipPlaceIcon[EItemEquipPlace::EIEP_Ring], RingItem);


	GET_SLOT(UTextBlock, TB_CharLevel);
	GET_SLOT(UTextBlock, TB_CombatPower);
	GET_SLOT(UTextBlock, TB_HealthPoint);
	GET_SLOT(UTextBlock, TB_AttackPoint);
	GET_SLOT(UTextBlock, TB_DefensePoint);
	GET_SLOT(UTextBlock, TB_UserNickName);
	GET_SLOT(UTextBlock, TB_CharExpText);
	GET_SLOT(UProgressBar, PB_CharExpProgress);
	GET_SLOT(UB2Button, BTN_Friend);

	GET_SLOT(UTextBlock, TB_AttackLabel)
	GET_SLOT(UTextBlock, TB_DefenseLabel)
	GET_SLOT(UTextBlock, TB_HealthLabel)
	GET_SLOT(UTextBlock, TB_CombatLable)
	
	GET_SLOT(UB2LobbyUI_ItemDetail, Lobby_ItemDetailCommonBP);

	GET_SLOT(UB2LobbyUI_CharStatDetail, Lobby_CharStatDetailBP);
	if (Lobby_CharStatDetailBP.IsValid())
		Lobby_CharStatDetailBP->UpdateStaticTexts();
}

void UB2UIOtherUserInfo::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Gladiator, &UB2UIOtherUserInfo::OnClickGladiator);
	BIND_CLICK_FUNC_TO_BTN(BTN_Assassin, &UB2UIOtherUserInfo::OnClickAssassin);
	BIND_CLICK_FUNC_TO_BTN(BTN_Wizard, &UB2UIOtherUserInfo::OnClickWizard);
	BIND_CLICK_FUNC_TO_BTN(BTN_Fighter, &UB2UIOtherUserInfo::OnClickFightern);

	BIND_CLICK_FUNC_TO_BTN(BTN_Friend, &UB2UIOtherUserInfo::OnClickFriendAdd);

}

void UB2UIOtherUserInfo::Init()
{
	Super::Init();
	SubscribeEvent();
}

void UB2UIOtherUserInfo::DestroySelf(UB2UIManager* InUIManager)
{
	if (Lobby_ItemDetailCommonBP.IsValid())
	{
		if (Lobby_ItemDetailCommonBP->GetVisibility() == ESlateVisibility::Visible)
		{
			B2AndroidBackManager::GetInstance()->RemoveBackWidget(Lobby_ItemDetailCommonBP.Get());
		}
	}

	Super::DestroySelf(InUIManager);
}



void UB2UIOtherUserInfo::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	SetLobbyUIHeaderTitleByGeneralTextTableKey(TEXT("LobbyScene_CharacterInfo"));
}

void UB2UIOtherUserInfo::OnClose(bool RightNow /* = false */)
{
	Super::OnClose(RightNow);

	UnsubscribeEvent();
}

void UB2UIOtherUserInfo::SubscribeEvent()
{
	CAPTURE_UOBJECT(UB2UIOtherUserInfo);

	DeliveryAskFriendTicket = DeliveryAskFriendClass<>::GetInstance().Subscribe(
	USE_CAPTURE_OBJECT_AND_TICKET(DeliveryAskFriend)
		Capture->OnDeliveryAskFriend();
	END_CAPTURE_OBJECT());

	DeliveryCloseItemPopupTicket = DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Subscribe(
	USE_CAPTURE_OBJECT_AND_TICKET(DeliveryCloseItemPopup, ELobbySubPopups p)
		Capture->OnDeliveryCloseItemPopup(p);
	END_CAPTURE_OBJECT());

	Issues.Add(CostumeEquipSlotVisibleClass<bool>::GetInstance().Subscribe2(
	[Capture](bool IsVisible)
	{
		if (Capture.IsValid())
		{
			Capture->UpdateItemEquipSlotVisible(IsVisible);
		}
	}
	));
}

void UB2UIOtherUserInfo::UnsubscribeEvent()
{
	Issues.Empty();

	DeliveryAskFriendClass<>::GetInstance().Unsubscribe(DeliveryAskFriendTicket);
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Unsubscribe(DeliveryCloseItemPopupTicket);
}

void UB2UIOtherUserInfo::UpdateCharName(EPCClass PCClass)
{
	/*for (int32 i = 0 ; i < GetMaxPCClassNum(); ++i)
	{
		int32 PCClassInt = PCClassToInt(PCClass);

		if (i == PCClassInt)
			TCharName[i]->SetColorAndOpacity(FLinearColor::Yellow);
		else
			TCharName[i]->SetColorAndOpacity(FLinearColor::White);
	}*/
}


void UB2UIOtherUserInfo::SetUserNickName(FText NickName)
{
	if (TB_UserNickName.IsValid())
		TB_UserNickName->SetText(NickName);
}

void UB2UIOtherUserInfo::SetBasestats(FB2ModPlayerInfo& UserInfo, EPCClass PCClass)
{
	if (TB_CharLevel.IsValid())
		TB_CharLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_Level_Abbrev")), FText::AsNumber(UserInfo.GetCharacterLevel(PCClass))));

	if (TB_HealthPoint.IsValid())
		TB_HealthPoint->SetText(FText::AsNumber(static_cast<int32>(CombatStatEval::GetPCHealth(PCClass, &UserInfo))));

	if (TB_AttackPoint.IsValid())
		TB_AttackPoint->SetText(FText::AsNumber(static_cast<int32>(CombatStatEval::GetPCAttack(PCClass, &UserInfo))));

	if (TB_DefensePoint.IsValid())
		TB_DefensePoint->SetText(FText::AsNumber(static_cast<int32>(CombatStatEval::GetPCDefense(PCClass, &UserInfo))));

	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::AsNumber(static_cast<int32>(CombatStatEval::GetPCCombatPower(PCClass, &UserInfo))));

	if (TB_AttackLabel.IsValid())
		TB_AttackLabel->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Attack"))));

	if (TB_DefenseLabel.IsValid())
		TB_DefenseLabel->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Defense"))));

	if (TB_HealthLabel.IsValid())
		TB_HealthLabel->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Health"))));

	if (TB_CombatLable.IsValid())
		TB_CombatLable->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Raking_CombatPower"))));	
}


void UB2UIOtherUserInfo::SetExp(FB2ModPlayerInfo& UserInfo, EPCClass PCClass)
{
	int32 GetCharacterExp(EPCClass);
	int32 GetCharacterExpMax(EPCClass);
	float GetCharacterExpPercent(EPCClass);

	if(TB_CharExpText.IsValid())
		TB_CharExpText->SetText(FText::FromString(FString::Printf(TEXT("(%d%%)") ,int32(UserInfo.GetCharacterExpPercent(PCClass)))));

	if (PB_CharExpProgress.IsValid())
		PB_CharExpProgress->SetPercent(UserInfo.GetCharacterExpPercent(PCClass) / 100);
}

void UB2UIOtherUserInfo::ItemSet(FB2ModPlayerInfo& UserInfo, EPCClass PCClass)
{
	TMap<EItemEquipPlace, FB2Item> EquipItem;
	UserInfo.GetEquippedItems(PCClass, EquipItem);

	for (auto El : EquipPlaceIcon)
	{
		if (EquipItem.Contains(El.Key))
		{
			El.Value.Get()->UpdateItemData(EquipItem[El.Key]);
			El.Value.Get()->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
			El.Value.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIOtherUserInfo::TotalUpdate(EPCClass PCClass)
{
	SelectPCClass = PCClass;
	LobbyHeroFindAccountClass<EPCClass>::GetInstance().Signal(PCClass);			//캐릭터메쉬 바꿔주기

	UpdateCharName(PCClass);								//버튼에 직업 이름들
	auto& UserInfo = BladeIIGameImpl::GetClientDataStore().GetOtherUserInfo();

	Lobby_CharStatDetailBP->UpdateStats(PCClass, true);	//상세정보

	SetBasestats(UserInfo, PCClass);						//기본공방 레벨등
	SetUserNickName(FText::FromString(UserInfo.GetUserNickName()));	//닉네임
	ItemSet(UserInfo, PCClass);								//아이템세팅

	SetExp(UserInfo, PCClass);
	FriendAddState();
}

void UB2UIOtherUserInfo::FriendAddState()
{
	auto SomeDoc = UB2UIDocHelper::GetDocSome();
	auto FriendDoc = UB2UIDocHelper::GetDocFriend();

	if (!SomeDoc && FriendDoc)
		return;

	bool IsFriendAble = FriendDoc->IsFriendAskable(SomeDoc->GetFindUserAccountID());
	BTN_Friend->SetVisibility(IsFriendAble ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UIOtherUserInfo::OnDeliveryAskFriend()
{
	auto SomeDoc = UB2UIDocHelper::GetDocSome();
	auto FriendDoc = UB2UIDocHelper::GetDocFriend();
	if (!SomeDoc && !FriendDoc)
		return;

	int64 AccountId = SomeDoc->GetFindUserAccountID();
	FB2UserPtr FriendInfo(new b2network::B2User);

	FriendDoc->AddFriendSendInfo(AccountId, FriendInfo);
}

void UB2UIOtherUserInfo::OnDeliveryCloseItemPopup(ELobbySubPopups InPopupEnum)
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(Lobby_ItemDetailCommonBP.Get());
}


void UB2UIOtherUserInfo::OnClickGladiator()
{
	if (SelectPCClass == EPCClass::EPC_Gladiator) return; //[@AKI, 171122][B2BUG-836]타 유저의 캐릭터 상세보기에서 캐릭터 등장 이펙트 반복해서 나오는 오류 수정

	CharSelectUpdateEvent(EPCClass::EPC_Gladiator);
	TotalUpdate(EPCClass::EPC_Gladiator);
}

void UB2UIOtherUserInfo::OnClickAssassin()
{
	if (SelectPCClass == EPCClass::EPC_Assassin) return; //[@AKI, 171122][B2BUG-836]타 유저의 캐릭터 상세보기에서 캐릭터 등장 이펙트 반복해서 나오는 오류 수정

	CharSelectUpdateEvent(EPCClass::EPC_Assassin);
	TotalUpdate(EPCClass::EPC_Assassin);
}

void UB2UIOtherUserInfo::OnClickWizard()
{
	if (SelectPCClass == EPCClass::EPC_Wizard) return;//[@AKI, 171122][B2BUG-836]타 유저의 캐릭터 상세보기에서 캐릭터 등장 이펙트 반복해서 나오는 오류 수정

	CharSelectUpdateEvent(EPCClass::EPC_Wizard);
	TotalUpdate(EPCClass::EPC_Wizard);
}

void UB2UIOtherUserInfo::OnClickFightern()
{
	if (SelectPCClass == EPCClass::EPC_Fighter) return;//[@AKI, 171122][B2BUG-836]타 유저의 캐릭터 상세보기에서 캐릭터 등장 이펙트 반복해서 나오는 오류 수정

	CharSelectUpdateEvent(EPCClass::EPC_Fighter);
	TotalUpdate(EPCClass::EPC_Fighter);
}

void UB2UIOtherUserInfo::OnClickItemSelect(EItemEquipPlace EquipPlace)
{
	auto& UserInfo = BladeIIGameImpl::GetClientDataStore().GetOtherUserInfo();

	TMap<EItemEquipPlace, FB2Item> EquipItem;
	UserInfo.GetEquippedItems(SelectPCClass, EquipItem);

	if (!EquipItem.Contains(EquipPlace))
		return;


	if (Lobby_ItemDetailCommonBP.IsValid())
	{
		if (Lobby_ItemDetailCommonBP->GetVisibility() != ESlateVisibility::Visible)
		{
			B2AndroidBackManager::GetInstance()->AddBackWidget(Lobby_ItemDetailCommonBP.Get());
		}
		Lobby_ItemDetailCommonBP->SetVisibility(ESlateVisibility::Visible);
		Lobby_ItemDetailCommonBP->UpdateItemData(EquipItem[EquipPlace]);
		Lobby_ItemDetailCommonBP->SetFindUserInfoButtonHidden();
	}
}

void UB2UIOtherUserInfo::OnClickFriendAdd()
{
	auto SomeDoc = UB2UIDocHelper::GetDocSome();
	auto FriendDoc = UB2UIDocHelper::GetDocFriend();
	if (!SomeDoc && !FriendDoc)
		return;

	int64 AccountId = SomeDoc->GetFindUserAccountID();

	if (FriendDoc->GetFriendSendInfo(AccountId))
	{
		DevShowError(TEXT("5503"));			//이미친구신청을 했을때
		return;
	}
	else if (FriendDoc->GetFriendSendListInfo().Num() >= BladeIIGameImpl::GetClientDataStore().GetMaxSendAskFriend())
	{
		DevShowError(TEXT("5504"));			//보낸친구가 오버 할경우
		return;
	}

	data_trader::Retailer::GetInstance().RequestAskFriend(AccountId);
}

void UB2UIOtherUserInfo::UpdateItemEquipSlotVisible(const bool& bIsVisibleCostumeSlot)
{
	const bool bIsItemEquipSlotVisible = bIsVisibleCostumeSlot ? false : true;

	PlayChangeAnimEquipSlotOrCostumeSlot_BP(bIsItemEquipSlotVisible);
}


