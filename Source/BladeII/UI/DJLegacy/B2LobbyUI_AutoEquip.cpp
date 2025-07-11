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
		return PanelSlot->GetSize(); // ��ü ������ �״��
	}
	return FVector2D(0.0f, 0.0f);
}

void UB2LobbyUI_AutoEquip::UpdateOnEquipChange()
{ // ���� ��ư ������ ���� ������ �Ǹ� �Ҹ��� �� ��.
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
	PendingEquipSuggestedItemIDs.Empty(); // ���� �������� �������� �ʴ��� PendingEquipSuggestedItemIDs ������ �̸� ��������.
	UB2LobbyInventory::GetEquipSuggestion(PendingEquipSuggestedItemIDs);

	DestroyAllItemIcons();
	UpdateIconsForCurrentEquipState(); // �ϴ� ���� ���´�� ������ ������Ʈ �Ŀ�
	UpdateIconsForEquipSuggestion(); // ���� ���忡 ���� ������Ʈ
}

void UB2LobbyUI_AutoEquip::UpdateEquipSuggestionSummaryTexts()
{
	EPCClass DisplayPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End; // �������� �����.
	if (DisplayPCClass == EPCClass::EPC_End){
		return; // EPC_End �� GetPlayerSharedStat ���� �۵� ���� ��.
	}

//	const int32 PCLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(DisplayPCClass);

	// ���� ���� ���¿� ���� ���� �������µ� GetEquipApplied** ��������� �ɼǿ� ���� ��ġ���� ����� ���̴�.
	const int32 CurrentAttack = (int32)CombatStatEval::GetPCAttack(DisplayPCClass);
	const int32 CurrentDefense = (int32)CombatStatEval::GetPCDefense(DisplayPCClass);
	const int32 CurrentHealth = (int32)CombatStatEval::GetPCHealth(DisplayPCClass);

	// PendingEquipSuggestedItemIDs �� �ִ� ġ�� ���� ������ ������ ����.
	TArray<FB2Item> EquipSuggestedItems;
	for (int64 CurrEquipSuggestID : PendingEquipSuggestedItemIDs)
	{
		// GetEquipSuggestion ������ �ϴ� ���۵��ε� ���� ID ���� �ٽ� ã�ƿ��±�. �Ƚ� ���� ����
		FB2Item CurrSuggestItem;
		if (UB2LobbyInventory::FindStoredItem(CurrSuggestItem, CurrEquipSuggestID))
		{
			EquipSuggestedItems.Add(CurrSuggestItem);
		}
	}

	// ���⿡ ���� ���� �����ϴ� �ֵ� ����Ʈ�� ���ؾ� �Ѵ�.
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

		if (!bHasSuggested){ // �̹� ������ ��� �ش��ϴ� ���� �������� ����.
			NonSuggestedEquipList.Add(CurrEquipped);
		}
	}

	EquipSuggestedItems.Append(NonSuggestedEquipList); // ���� ���� + ���� ���� �� ���� ���� �� ��.

	FB2Wing WingData; // GetEquip** ������ �ȿ��� wing �� �������� ���̹Ƿ� ���⼭�� �־���.
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
	// �κ� �κ��丮���� ���� ���õ� PC �� ���� �������� ���� EPC_End �� ����ߴµ� static �Լ��� ��������� 
	// ��ǻ� LobbyInventory �� �����Ǿ� �ִ� ���¿����� ���� ���õ� PC �� ����� �ν��� �� ��.
	UB2LobbyInventory::GetAllEquippedItem(CurrPCEquipItemList, EPCClass::EPC_End);

	for (int32 EI = 0; EI < CurrPCEquipItemList.Num(); ++EI)
	{
		FB2Item& CurrEquip = CurrPCEquipItemList[EI];

		UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(EquipItemIconWidgetClass->GetDefaultObject());

		// ��ġ�� canvas panel ���� EquipItemIcon �� ����.
		UB2DynItemIcon_LobbyEquip* NewIcon = Cast<UB2DynItemIcon_LobbyEquip>(DynCreateInCanvasPanel(
			EquipItemIconWidgetClass, this, GetEquipItemIconPanelNativeRef(CurrEquip.EquipPlace), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f),
			GetAllowedSingleEquipItemIconSize(CurrEquip.EquipPlace), FVector2D(0.0f, 0.0f), 0, false
			));

		if (NewIcon)
		{
			NewIcon->SetEquipItemIconType(ELobbyEquipItemIconType::EEIIT_AutoEquipCurrent);
			NewIcon->UpdateItemData(CurrEquip); // ���� ������ ����.
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
		// GetEquipSuggestion ������ �ϴ� ���۵��ε� ���� ID ���� �ٽ� ã�ƿ��±�. �Ƚ� ���� ����
		// GetEquipSuggestion �� ����� �����ߴٸ� CurrSuggestItem �� CurrEquipped ���� ���ƾ� ��. (�ƴϸ� CurrEquipped �� ���ų�)
		FB2Item CurrSuggestItem;
		if(!UB2LobbyInventory::FindStoredItem(CurrSuggestItem, CurrEquipSuggestID))
			continue;
		FB2Item CurrEquipped;
		bool bCurrentlyEquippedAny = UB2LobbyInventory::GetEquippedItemAtPlace(CurrEquipped, CurrSuggestItem.EquipPlace);
		if (!bCurrentlyEquippedAny){ // ������ ���� ���ٸ� 0 �� ���ϵ��� ������ �� ����.
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
			NewIcon->UpdateStatIncSigns(CurrEquipped); // �� ����� �־��� ��
			DestroyEquipItemIcon(CurrSuggestItem.EquipPlace); // ������ �����ߴ� �� �ִٸ� �����.
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
		PlayAutoEquipCommitSoundBP(); // ���� ������ �Ǵ� ��Ȳ������ ���带 �÷��� �Ѵ�.
	}

	// UpdateOnEquipChange ���� �Ҹ����� �Ϸ��� �ߴµ� �� ���� �ǵ� ���� �������� �� �޶�� �ؼ�.
	//if (bAutoEquipRequested)
	{ // �ڵ� �ݱ�.
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
			// �κ��丮 �� Ȯ�� ���� �����Կ��� ���ŵ� �� �ִ� ���� ��ŭ �ű� ȹ�� ǥ�� ����Ʈ������ ����. ���� Ȯ���ϰԴ� ������ ���� �� �ؾ� �Ǳ� ��. �׷��Ա��� ö���ϰ� �ؾ� �� �ʿ���� ���� �� ����..
//			BladeIIGameImpl::GetRedDotManager().RemoveNewItem(CurrSuggestItem);
		}
	}

	PendingEquipSuggestedItemIDs.Empty();

	// ���� ���������� ���� ������ ���� ������ ������Ʈ�� �� ��.

	return bAtLeastOnePending;
}

void UB2LobbyUI_AutoEquip::CloseMe()
{
	DestroyAllItemIcons();
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoEquip);
}