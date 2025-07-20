// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_AutoEquip.h"
//#include "BladeII.h"

#include "B2DynItemIcon_LobbyEquip.h"
#include "B2LobbyInventory.h"
#include "B2LobbyGameMode.h"
#include "Event.h"
#include "B2CombatStatEvaluator.h"
#include "B2Airport.h"


#include "BladeIIGameImpl.h"

UB2LobbyUI_AutoEquip::UB2LobbyUI_AutoEquip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EquipItemIconWidgetClass = NULL;
	
	for (int32 II = 0; II < (int32)EItemEquipPlace::EIEP_End; ++II)
	{
		EquipItemIconPanels[II] = NULL;
		CreatedEquipItemIcons[II] = NULL;
	}

	PostAutoEquipCloseTime = 0.0f;
	bAutoEquipRequested = false;
}

void UB2LobbyUI_AutoEquip::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	UpdateStaticTexts();
	UpdateItemIcons();
	UpdateEquipSuggestionSummaryTexts();
}

void UB2LobbyUI_AutoEquip::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_AutoEquipTitle);
	GET_SLOT(UTextBlock, TB_AutoEquipButton);
	GET_SLOT(UTextBlock, TB_AttackLabel);
	GET_SLOT(UTextBlock, TB_DefenseLabel);
	GET_SLOT(UTextBlock, TB_HealthLabel);
	GET_SLOT(UTextBlock, TB_TotalAttackStat);
	GET_SLOT(UTextBlock, TB_TotalDefenseStat);
	GET_SLOT(UTextBlock, TB_TotalHealthStat);
}

void UB2LobbyUI_AutoEquip::CloseWidgetDelegate()
{
	OnCloseButtonClicked();
}

void UB2LobbyUI_AutoEquip::SetEquipItemIconPanelNativeRef(EItemEquipPlace InEquipPlace, class UCanvasPanel* InRef)
{
	if (InEquipPlace < EItemEquipPlace::EIEP_End)
	{
		EquipItemIconPanels[(int32)InEquipPlace] = InRef;
	}
}

UCanvasPanel* UB2LobbyUI_AutoEquip::GetEquipItemIconPanelNativeRef(EItemEquipPlace InEquipPlace)
{
	if (InEquipPlace < EItemEquipPlace::EIEP_End)
	{
		return EquipItemIconPanels[(int32)InEquipPlace];
	}
	return NULL;
}

void UB2LobbyUI_AutoEquip::SetEquipItemIcon(EItemEquipPlace InEquipPlace, class UB2DynItemIcon_LobbyEquip* InCreatedIcon)
{
	if (InEquipPlace < EItemEquipPlace::EIEP_End)
	{
		CreatedEquipItemIcons[(int32)InEquipPlace] = InCreatedIcon;
	}
}

UB2DynItemIcon_LobbyEquip* UB2LobbyUI_AutoEquip::GetEquipItemIcon(EItemEquipPlace InEquipPlace)
{
	if (InEquipPlace < EItemEquipPlace::EIEP_End)
	{
		return CreatedEquipItemIcons[(int32)InEquipPlace];
	}
	return NULL;
}

void UB2LobbyUI_AutoEquip::DestroyEquipItemIcon(EItemEquipPlace InEquipPlace)
{
	int32 CastedIndex = (int32)InEquipPlace;
	if (CastedIndex < (int32)EItemEquipPlace::EIEP_End)
	{
		if (CreatedEquipItemIcons[CastedIndex])
		{
			CreatedEquipItemIcons[CastedIndex]->DestroySelf();
			CreatedEquipItemIcons[CastedIndex] = NULL;
		}
	}
}

FVector2D UB2LobbyUI_AutoEquip::GetAllowedSingleEquipItemIconSize(EItemEquipPlace InEquipPlace)
{
	UCanvasPanel* PlacePanel = GetEquipItemIconPanelNativeRef(InEquipPlace);
	UCanvasPanelSlot* PanelSlot = PlacePanel ? Cast<UCanvasPanelSlot>(PlacePanel->Slot) : NULL;
	if (PanelSlot)
	{
		return PanelSlot->GetSize(); // 傈眉 荤捞令 弊措肺
	}
	return FVector2D(0.0f, 0.0f);
}

void UB2LobbyUI_AutoEquip::UpdateOnEquipChange()
{ // 厘馒 滚瓢 穿福绊 角力 厘馒捞 登搁 阂府霸 瞪 巴.
	UpdateItemIcons();
	UpdateEquipSuggestionSummaryTexts();
}

void UB2LobbyUI_AutoEquip::UpdateStaticTexts()
{
	if (TB_AutoEquipTitle.IsValid())
	{
		TB_AutoEquipTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_AutoEquip")));
	}
	if (TB_AutoEquipButton.IsValid())
	{
		TB_AutoEquipButton->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
	}

	if (TB_AttackLabel.IsValid())
	{
		TB_AttackLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Attack")));
	}
	if (TB_DefenseLabel.IsValid())
	{
		TB_DefenseLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Defense")));
	}
	if (TB_HealthLabel.IsValid())
	{
		TB_HealthLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyStatText_Health")));
	}
}

void UB2LobbyUI_AutoEquip::UpdateItemIcons()
{
	PendingEquipSuggestedItemIDs.Empty(); // 汲飞 酒捞能捞 积己登瘤 臼歹扼档 PendingEquipSuggestedItemIDs 鳖瘤绰 固府 掘绢初档废.
	UB2LobbyInventory::GetEquipSuggestion(PendingEquipSuggestedItemIDs);

	DestroyAllItemIcons();
	UpdateIconsForCurrentEquipState(); // 老窜 泅犁 惑怕措肺 酒捞能 诀单捞飘 饶俊
	UpdateIconsForEquipSuggestion(); // 厘馒 鼻厘俊 蝶弗 诀单捞飘
}

void UB2LobbyUI_AutoEquip::UpdateEquipSuggestionSummaryTexts()
{
	EPCClass DisplayPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End; // 康旷包府 葛靛烙.
	if (DisplayPCClass == EPCClass::EPC_End){
		return; // EPC_End 搁 GetPlayerSharedStat 何磐 累悼 救且 巴.
	}

//	const int32 PCLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(DisplayPCClass);

	// 泅犁 厘馒 惑怕俊 蝶弗 蔼阑 啊廉坷绰单 GetEquipApplied** 揪府令俊辑绰 可记俊 蝶弗 荐摹鳖瘤 绊妨瞪 巴捞促.
	const int32 CurrentAttack = (int32)CombatStatEval::GetPCAttack(DisplayPCClass);
	const int32 CurrentDefense = (int32)CombatStatEval::GetPCDefense(DisplayPCClass);
	const int32 CurrentHealth = (int32)CombatStatEval::GetPCHealth(DisplayPCClass);

	// PendingEquipSuggestedItemIDs 绰 乐促 摹绊 角力 酒捞袍 单捞磐 掘绢咳.
	TArray<FB2Item> EquipSuggestedItems;
	for (int64 CurrEquipSuggestID : PendingEquipSuggestedItemIDs)
	{
		// GetEquipSuggestion 俊辑档 窍绰 悼累甸牢单 俺喊 ID 喊肺 促矫 茫酒坷绰焙. 救嚼 ぇぇ 偿偿
		FB2Item CurrSuggestItem;
		if (UB2LobbyInventory::FindStoredItem(CurrSuggestItem, CurrEquipSuggestID))
		{
			EquipSuggestedItems.Add(CurrSuggestItem);
		}
	}

	// 咯扁俊 厘馒 惑怕 蜡瘤窍绰 局甸 府胶飘档 歹秦具 茄促.
	TArray<FB2Item> CurrentEquippedItems;
	TArray<FB2Item> NonSuggestedEquipList;
	UB2LobbyInventory::GetAllEquippedItem(CurrentEquippedItems, DisplayPCClass);
	for (const FB2Item& CurrEquipped : CurrentEquippedItems)
	{
		bool bHasSuggested = false;
		for (const FB2Item& EquipSuggested : EquipSuggestedItems)
		{
			if (EquipSuggested.EquipPlace == CurrEquipped.EquipPlace){
				bHasSuggested = true;
				break;
			}
		}

		if (!bHasSuggested){ // 捞锅 厘馒等 厘厚俊 秦寸窍绰 鼻厘 酒捞袍捞 绝澜.
			NonSuggestedEquipList.Add(CurrEquipped);
		}
	}

	EquipSuggestedItems.Append(NonSuggestedEquipList); // 厘馒 鼻厘 + 扁粮 厘馒 吝 鼻厘 绝绰 巴 钦.

	FB2Wing WingData; // GetEquip** 俊辑绰 救俊辑 wing 阑 皑救沁阑 巴捞骨肺 咯扁辑档 持绢淋.
	const bool bHasWing = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(DisplayPCClass, WingData);

	const int32 SuggestedAttack = (int32)CombatStatEval::GetPCAttack(DisplayPCClass, NULL, -1, &EquipSuggestedItems);
	const int32 SuggestedDefense = (int32)CombatStatEval::GetPCDefense(DisplayPCClass, NULL, -1, &EquipSuggestedItems);
	const int32 SuggestedHealth = (int32)CombatStatEval::GetPCHealth(DisplayPCClass, NULL, -1, &EquipSuggestedItems);

	FText AttackStatText = FText::FromString(FString::Printf(TEXT("%d (%s%s)"),
		SuggestedAttack, 
		(SuggestedAttack > CurrentAttack) ? TEXT("+") : TEXT("-"),
		(SuggestedAttack != CurrentAttack) ? *FString::FromInt(FMath::Abs(SuggestedAttack - CurrentAttack)) : TEXT("")
		));
	if (TB_TotalAttackStat.IsValid())
	{
		TB_TotalAttackStat->SetText(AttackStatText);
	}

	FText DefenseStatText = FText::FromString(FString::Printf(TEXT("%d (%s%s)"),
		SuggestedDefense,
		(SuggestedDefense > CurrentDefense) ? TEXT("+") : TEXT("-"),
		(SuggestedDefense != CurrentDefense) ? *FString::FromInt(FMath::Abs(SuggestedDefense - CurrentDefense)) : TEXT("")
		));
	if (TB_TotalDefenseStat.IsValid())
	{
		TB_TotalDefenseStat->SetText(DefenseStatText);
	}

	FText HealthStatText = FText::FromString(FString::Printf(TEXT("%d (%s%s)"),
		SuggestedHealth,
		(SuggestedHealth > CurrentHealth) ? TEXT("+") : TEXT("-"),
		(SuggestedHealth != CurrentHealth) ? *FString::FromInt(FMath::Abs(SuggestedHealth - CurrentHealth)) : TEXT("")
		));
	if (TB_TotalHealthStat.IsValid())
	{
		TB_TotalHealthStat->SetText(HealthStatText);
	}
}

void UB2LobbyUI_AutoEquip::UpdateIconsForCurrentEquipState()
{
	if (EquipItemIconWidgetClass == NULL)
	{
		return;
	}

	TArray<FB2Item> CurrPCEquipItemList;
	// 肺厚 牢亥配府俊辑 泅犁 急琶等 PC 俊 措秦 啊廉坷扁 困秦 EPC_End 甫 荤侩沁绰单 static 窃荐甫 荤侩窍瘤父 
	// 荤角惑 LobbyInventory 啊 积己登绢 乐绰 惑怕俊辑唱 泅犁 急琶等 PC 啊 力措肺 牢侥捞 瞪 巴.
	UB2LobbyInventory::GetAllEquippedItem(CurrPCEquipItemList, EPCClass::EPC_End);

	for (int32 EI = 0; EI < CurrPCEquipItemList.Num(); ++EI)
	{
		FB2Item& CurrEquip = CurrPCEquipItemList[EI];

		UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(EquipItemIconWidgetClass->GetDefaultObject());

		// 汲摹茄 canvas panel 困俊 EquipItemIcon 阑 积己.
		UB2DynItemIcon_LobbyEquip* NewIcon = Cast<UB2DynItemIcon_LobbyEquip>(DynCreateInCanvasPanel(
			EquipItemIconWidgetClass, this, GetEquipItemIconPanelNativeRef(CurrEquip.EquipPlace), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f),
			GetAllowedSingleEquipItemIconSize(CurrEquip.EquipPlace), FVector2D(0.0f, 0.0f), 0, false
			));

		if (NewIcon)
		{
			NewIcon->SetEquipItemIconType(ELobbyEquipItemIconType::EEIIT_AutoEquipCurrent);
			NewIcon->UpdateItemData(CurrEquip); // 俺喊 酒捞袍 沥焊.
			SetEquipItemIcon(CurrEquip.EquipPlace, NewIcon);
		}
	}
}

void UB2LobbyUI_AutoEquip::UpdateIconsForEquipSuggestion()
{
	if (EquipItemIconWidgetClass == NULL){
		return;
	}

	for (int64 CurrEquipSuggestID : PendingEquipSuggestedItemIDs)
	{
		// GetEquipSuggestion 俊辑档 窍绰 悼累甸牢单 俺喊 ID 喊肺 促矫 茫酒坷绰焙. 救嚼 ぇぇ 偿偿
		// GetEquipSuggestion 捞 力措肺 悼累沁促搁 CurrSuggestItem 捞 CurrEquipped 焊促 亮酒具 窃. (酒聪搁 CurrEquipped 啊 绝芭唱)
		FB2Item CurrSuggestItem;
		if(!UB2LobbyInventory::FindStoredItem(CurrSuggestItem, CurrEquipSuggestID))
			continue;
		FB2Item CurrEquipped;
		bool bCurrentlyEquippedAny = UB2LobbyInventory::GetEquippedItemAtPlace(CurrEquipped, CurrSuggestItem.EquipPlace);
		if (!bCurrentlyEquippedAny){ // 厘馒等 巴捞 绝促搁 0 苞 厚背窍档废 蔼甸阑 粱 技泼.
			CurrEquipped.EquipPlace = CurrSuggestItem.EquipPlace;
			CurrEquipped.bCurrentlyEquipped = true;
			CurrEquipped.SetPrimaryPoint(0.0f);
		}

		UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(EquipItemIconWidgetClass->GetDefaultObject());

		UB2DynItemIcon_LobbyEquip* NewIcon = Cast<UB2DynItemIcon_LobbyEquip>(DynCreateInCanvasPanel(
			EquipItemIconWidgetClass, this, GetEquipItemIconPanelNativeRef(CurrSuggestItem.EquipPlace), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f),
			GetAllowedSingleEquipItemIconSize(CurrSuggestItem.EquipPlace), FVector2D(0.0f, 0.0f), false
			));

		if (NewIcon)
		{
			NewIcon->SetEquipItemIconType(ELobbyEquipItemIconType::EEIIT_AutoEquipSuggest);
			NewIcon->UpdateItemData(CurrSuggestItem);
			NewIcon->UpdateStatIncSigns(CurrEquipped); // 厚背 措惑阑 持绢淋 せ
			DestroyEquipItemIcon(CurrSuggestItem.EquipPlace); // 扁粮俊 积己沁带 霸 乐促搁 瘤款促.
			SetEquipItemIcon(CurrSuggestItem.EquipPlace, NewIcon);
		}
	}
}

void UB2LobbyUI_AutoEquip::DestroyAllItemIcons()
{
	for (int32 II = 0; II < (int32)EItemEquipPlace::EIEP_End; ++II)
	{
		if (CreatedEquipItemIcons[II])
		{
			CreatedEquipItemIcons[II]->DestroySelf();
			CreatedEquipItemIcons[II] = NULL;
		}
	}
}

void UB2LobbyUI_AutoEquip::OnAutoEquipButtonClicked()
{
	if (DoAutoEquip())
	{
		bAutoEquipRequested = true;
		PlayAutoEquipCommitSoundBP(); // 角力 厘馒捞 登绰 惑炔俊辑绰 荤款靛甫 敲饭捞 茄促.
	}

	// UpdateOnEquipChange 俊辑 阂府档废 窍妨绊 沁绰单 傲 厘馒 登电 富电 摧洒档废 秦 崔扼绊 秦辑.
	//if (bAutoEquipRequested)
	{ // 磊悼 摧扁.
		if (PostAutoEquipCloseTime > 0.0f && GetOwningPlayer())
		{
			GetOwningPlayer()->GetWorldTimerManager().SetTimer(
				PostAutoEquipCloseTH, this, &UB2LobbyUI_AutoEquip::CloseMe, PostAutoEquipCloseTime, false
				);
		}
		else
		{
			CloseMe();
		}
	}
}

void UB2LobbyUI_AutoEquip::OnCloseButtonClicked()
{
	CloseMe();
}

bool UB2LobbyUI_AutoEquip::DoAutoEquip()
{
	const bool bAtLeastOnePending = (PendingEquipSuggestedItemIDs.Num() > 0);

	LobbyReqEquipItemsClass<const TArray<int64>&>::GetInstance().Signal(PendingEquipSuggestedItemIDs);

	for (int64 CurrEquipSuggestID : PendingEquipSuggestedItemIDs)
	{
		FB2Item CurrSuggestItem;
		if (UB2LobbyInventory::FindStoredItem(CurrSuggestItem, CurrEquipSuggestID))
		{
			// 牢亥配府 徘 犬牢 绝捞 焊包窃俊辑 力芭瞪 荐 乐绰 巴牢 父怒 脚痹 裙垫 钎矫 府胶飘俊辑档 力芭. 焊促 犬角窍霸绰 览翠捞 吭阑 锭 秦具 登变 窃. 弊犯霸鳖瘤 枚历窍霸 秦具 瞪 鞘夸鳖柳 绝阑 芭 鞍绊..
//			BladeIIGameImpl::GetRedDotManager().RemoveNewItem(CurrSuggestItem);
		}
	}

	PendingEquipSuggestedItemIDs.Empty();

	// 角力 辑滚俊辑狼 厘馒 览翠捞 坷搁 酒捞能 诀单捞飘啊 瞪 巴.

	return bAtLeastOnePending;
}

void UB2LobbyUI_AutoEquip::CloseMe()
{
	DestroyAllItemIcons();
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoEquip);
}