// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_CharStatDetailPopup.h"
//#include "BladeII.h"

#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "Event.h"
#include "B2DynText_Multiple.h"
#include "B2CombatStatEvaluator.h"


#include "BladeIIGameImpl.h"

UB2LobbyUI_CharStatDetailPopup::UB2LobbyUI_CharStatDetailPopup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StatTextDisplayClass = NULL;
	StatSlotOffSet = 30.f;
	BasePopupHeight = 0;
}

void UB2LobbyUI_CharStatDetailPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UVerticalBox, VB_Stats)	
	GET_SLOT(UTextBlock, TB_StatDetailTitle)
	GET_SLOT(UB2Button, BTN_Close)
	GET_SLOT(UCanvasPanel, CP_Detail)
}

void UB2LobbyUI_CharStatDetailPopup::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);
	
	if (CP_Detail.IsValid())
	{
		if (UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(CP_Detail->Slot))
			BasePopupHeight = PanelSlot->GetSize().Y;
	}

	UpdateStaticTexts();
}

void UB2LobbyUI_CharStatDetailPopup::DestroySelf()
{
	Super::DestroySelf();
	DestroyStatTexts();
}

void UB2LobbyUI_CharStatDetailPopup::UpdateStaticTexts()
{
}

void UB2LobbyUI_CharStatDetailPopup::UpdateStats(EPCClass PCClassToShow, EUnitedCombatOptions CombatOption, bool bFindAccountInfo /*= false*/)
{
	DestroyStatTexts();

	int32 RowIndexOffensiveStat = CreateStats(VB_Stats.Get(), CombatOption, PCClassToShow, bFindAccountInfo);
	UpdateWidgetSize(RowIndexOffensiveStat);
}


int32 UB2LobbyUI_CharStatDetailPopup::CreateItemOptionStat(class UVerticalBox* VBToCreateIn, int32 RowIndexInVB, ECharStatApplyType InPrimaryPointApplyType, EUnitedCombatOptions InUnitedOptionType, EPCClass InPCClass, float EffectiveValue)
{
	// 配袍栏肺 牢秦 罚待 可记俊辑绰 皑家蔼鳖瘤 钎扁 啊瓷窍促.
	if (InPrimaryPointApplyType != ECharStatApplyType::ECSA_RandomOptions)
	{
		if (EffectiveValue <= 0)
			return RowIndexInVB;
	}

	EItemOption ItemOption = CombatStatEval::GetItemOptionOfUnitedOption(InUnitedOptionType);
	EItemPrimaryPointType PrimaryPointType = CombatStatEval::ConvertItemOptionToItemPrimaryPointType(ItemOption);
	
	FText ApplyTypeText;	
	if (PrimaryPointType != EItemPrimaryPointType::EIPP_End)
	{
		ApplyTypeText = GetLOCTextOfPrimaryPointApplyType(InPrimaryPointApplyType, PrimaryPointType);
	}
	else if (ItemOption != EItemOption::EIO_End)
	{
		ApplyTypeText = GetLOCTextOfPrimaryPointApplyType(InPrimaryPointApplyType, ItemOption, InPCClass);
	}
	else
	{
		ApplyTypeText = GetLOCTextOfPrimaryPointApplyType(InPrimaryPointApplyType);
	}	
	
	return CreateStatDynText(VBToCreateIn, RowIndexInVB, ApplyTypeText, GetItemOptionValueDisplayText(ItemOption, EffectiveValue, false));
}

int32 UB2LobbyUI_CharStatDetailPopup::CreateGuildOptionStat(class UVerticalBox* VBToCreateIn, int32 RowIndexInVB, FText InGuildText, EUnitedCombatOptions InUnitedOptionType, float EffectiveValue)
{
	if(EffectiveValue <= 0)
		return RowIndexInVB;

	EItemOption ItemOption = CombatStatEval::GetItemOptionOfUnitedOption(InUnitedOptionType);

	return CreateStatDynText(VBToCreateIn, RowIndexInVB, InGuildText, GetItemOptionValueDisplayText(ItemOption, EffectiveValue, false));
}

int32 UB2LobbyUI_CharStatDetailPopup::CreateStatDynText(class UVerticalBox* VBToCreateIn, int32 RowIndexInVB, const FText& InStatLabelText, const FText& InStatValueText)
{
	UB2DynText_Multiple* ThisCreated = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(StatTextDisplayClass, this, VBToCreateIn, FMargin()));
	if (ThisCreated)
	{
		ThisCreated->SetDynText(RowIndexInVB,InStatLabelText,InStatValueText);
		++RowIndexInVB;
		CreatedStatTextBlocks.Add(ThisCreated);
	}

	return RowIndexInVB;
}

int32 UB2LobbyUI_CharStatDetailPopup::CreateStatDynText(class UVerticalBox* VBToCreateIn, int32 RowIndexInVB, const FText& InStatLabelText, int32 InStatValue)
{
	if (InStatValue == 0)
		return RowIndexInVB;

	return CreateStatDynText(VBToCreateIn, RowIndexInVB, InStatLabelText, FText::FromString(FString::Printf(TEXT("%d"), InStatValue)));
}

int32 UB2LobbyUI_CharStatDetailPopup::CreateStats(class UVerticalBox* VBToCreateIn, EUnitedCombatOptions InUnitedOptionType, EPCClass InDisplayPCClass, bool bFindAccountInfo)
{
	ICharacterDataStore* LocalCharData = &BladeIIGameImpl::GetLocalCharacterData();
	FB2ModPlayerInfo* OtherPlayer = &BladeIIGameImpl::GetClientDataStore().GetOtherUserInfo();
	ICharacterDataStore* DisplayCharDataStore = (bFindAccountInfo ? OtherPlayer : LocalCharData);
		
	EItemOption ItemOption = CombatStatEval::GetItemOptionOfUnitedOption(InUnitedOptionType);
	EItemPrimaryPointType PrimaryPointType = CombatStatEval::ConvertItemOptionToItemPrimaryPointType(ItemOption);
	FText TitleName = GetLOCTextOfItemOption(ItemOption, InDisplayPCClass);

	// 1.傍规眉 八荤
	int32 RowIndexInVB = 0;
	if (PrimaryPointType != EItemPrimaryPointType::EIPP_End)
	{
		switch (PrimaryPointType)
		{
		case EItemPrimaryPointType::EIPP_Attack:
			RowIndexInVB = CreateStatDynText(VBToCreateIn, RowIndexInVB, FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyCharStatDetail_CharacterPrimaryPointType"))), GetLOCTextOfPrimPointType(PrimaryPointType)),
				(int32)CombatStatEval::GetPCBaseAttack(LocalCharData->GetCharacterLevel(InDisplayPCClass)));

			TitleName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Attack")));
			break;
		case EItemPrimaryPointType::EIPP_Defense:
			RowIndexInVB = CreateStatDynText(VBToCreateIn, RowIndexInVB, FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyCharStatDetail_CharacterPrimaryPointType"))), GetLOCTextOfPrimPointType(PrimaryPointType)),
				(int32)CombatStatEval::GetPCBaseDefense(LocalCharData->GetCharacterLevel(InDisplayPCClass)));

			TitleName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Defense")));
			break;
		case EItemPrimaryPointType::EIPP_Health:
			RowIndexInVB = CreateStatDynText(VBToCreateIn, RowIndexInVB, FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyCharStatDetail_CharacterPrimaryPointType"))), GetLOCTextOfPrimPointType(PrimaryPointType)),
				(int32)CombatStatEval::GetPCBaseHealth(LocalCharData->GetCharacterLevel(InDisplayPCClass)));

			TitleName = BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Health")));
			break;
		}
	}	

	// 2. 可记甸 贸府
	CombatStatInfoRawDatas CombatStatInfoData;
	CombatStatEval::GetUnitedOptionStatusRawValues(InDisplayPCClass, InUnitedOptionType, CombatStatInfoData, DisplayCharDataStore);
	for (auto PrimaryPointRawData : CombatStatInfoData.PrimaryPointRawValues)
	{
		ECharStatApplyType ChatStatApplyType = PrimaryPointRawData.Key;
		float ItemOptionValue = GetEffectiveOptionValueTempl(ItemOption, PrimaryPointRawData.Value);
		RowIndexInVB = CreateItemOptionStat(VBToCreateIn, RowIndexInVB, ChatStatApplyType, InUnitedOptionType, InDisplayPCClass, ItemOptionValue);
	}

	// 3. 碍拳 技飘 包访 单捞磐 眠啊
	if (PrimaryPointType != EItemPrimaryPointType::EIPP_End)
	{
		float EnhanceFactor = LocalCharData->GetEnhanceSetEffectFactor(InDisplayPCClass);
		EnhanceFactor *= 0.01f;
		RowIndexInVB = CreateItemOptionStat(VBToCreateIn, RowIndexInVB, ECharStatApplyType::ECSA_EnhanceSetEffect, EUnitedCombatOptions::UCO_End, InDisplayPCClass, EnhanceFactor);
	}

	// 4. 辨靛 滚橇 眠啊
	FText GuildBuffString = FText::FromString(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Guild_BuffBtn"))).ToString());

	if (PrimaryPointType != EItemPrimaryPointType::EIPP_End)				//盖 关俊 眠啊秦具窍绰单 胶困摹甫 肚结具窍唱.. 快急 鞭窍霸 持嚼聪促..
	{
		switch (PrimaryPointType)
		{
		case EItemPrimaryPointType::EIPP_Attack:
			RowIndexInVB = CreateGuildOptionStat(VBToCreateIn, RowIndexInVB, GuildBuffString, EUnitedCombatOptions::UCO_End, (int32)CombatStatEval::GetPCAttackGuildBuffValue(DisplayCharDataStore) * 0.01f);
			break;
		case EItemPrimaryPointType::EIPP_Defense:
			RowIndexInVB = CreateGuildOptionStat(VBToCreateIn, RowIndexInVB, GuildBuffString, EUnitedCombatOptions::UCO_End, (int32)CombatStatEval::GetPCDefendseGuildBuffValue(DisplayCharDataStore) * 0.01f);
			break;
		case EItemPrimaryPointType::EIPP_Health:
			RowIndexInVB = CreateGuildOptionStat(VBToCreateIn, RowIndexInVB, GuildBuffString, EUnitedCombatOptions::UCO_End, (int32)CombatStatEval::GetPCHalthGuildBuffValue(DisplayCharDataStore) * 0.01f);
			break;
		}
	}


	

	// 鸥捞撇 函版
	if (TB_StatDetailTitle.IsValid())
		TB_StatDetailTitle->SetText(TitleName);

	return RowIndexInVB;
}

void UB2LobbyUI_CharStatDetailPopup::UpdateWidgetSize(int32 StatSlotNum)
{
	if (VB_Stats.IsValid() == false || CP_Detail.IsValid() == false)
		return;

	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(CP_Detail->Slot);
	if (PanelSlot == nullptr)
		return;

	FVector2D PanelSize = PanelSlot->GetSize();

	// 1.Update Widget Height Size!!
	PanelSlot->SetSize(FVector2D(PanelSize.X, BasePopupHeight + (StatSlotOffSet * StatSlotNum)));

	// 2.Update Widget Position!!
	AdjustWidgetPositionForViewPortSize();
}

void UB2LobbyUI_CharStatDetailPopup::AdjustWidgetPositionForViewPortSize()
{
	if (CP_Detail.IsValid() == false)
		return;

	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(CP_Detail->Slot);
	if (PanelSlot == nullptr)
		return;
	
	FVector2D ViewPortPos;
	if (GetWorld() && GetWorld()->GetGameViewport())
		GetWorld()->GetGameViewport()->GetViewportSize(ViewPortPos);

	if (ViewPortPos.IsZero())
		return;

	// 扑诀捞 拳搁 酒贰肺 郴妨啊瘤臼霸 焊沥
	FVector2D PanelPos = CP_Detail->GetRenderTransform().Translation;
	FVector2D PanelSize = PanelSlot->GetSize();
	if ((PanelPos.Y + PanelSize.Y) > ViewPortPos.Y)
	{
		float DeltaY = ((PanelPos.Y + PanelSize.Y) - ViewPortPos.Y);
		CP_Detail->SetRenderTranslation(FVector2D(PanelPos.X, PanelPos.Y-DeltaY));
	}
}

void UB2LobbyUI_CharStatDetailPopup::DestroyStatTexts()
{
	for (auto CurrElem : CreatedStatTextBlocks)
	{
		if (CurrElem)
			CurrElem->DestroySelf();
	}

	CreatedStatTextBlocks.Empty();
}

void UB2LobbyUI_CharStatDetailPopup::UpdatePosition(FVector2D position)
{
	if (CP_Detail.IsValid())
		CP_Detail->SetRenderTranslation(position);

	AdjustWidgetPositionForViewPortSize();
}

void UB2LobbyUI_CharStatDetailPopup::OnCloseButtonClicked()
{
	OnClose.ExecuteIfBound();

	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_CharStatDetailPopup);
}
