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
		return PanelSlot->GetSize(); // 전체 사이즈 그대로
	}
	return FVector2D(0.0f, 0.0f);
}

void UB2LobbyUI_AutoEquip::UpdateOnEquipChange()
{ // 장착 버튼 누르고 실제 장착이 되면 불리게 될 것.
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
	PendingEquipSuggestedItemIDs.Empty(); // 설령 아이콘이 생성되지 않더라도 PendingEquipSuggestedItemIDs 까지는 미리 얻어놓도록.
	UB2LobbyInventory::GetEquipSuggestion(PendingEquipSuggestedItemIDs);

	DestroyAllItemIcons();
	UpdateIconsForCurrentEquipState(); // 일단 현재 상태대로 아이콘 업데이트 후에
	UpdateIconsForEquipSuggestion(); // 장착 권장에 따른 업데이트
}

void UB2LobbyUI_AutoEquip::UpdateEquipSuggestionSummaryTexts()
{
	EPCClass DisplayPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End; // 영웅관리 모드임.
	if (DisplayPCClass == EPCClass::EPC_End){
		return; // EPC_End 면 GetPlayerSharedStat 부터 작동 안할 것.
	}

//	const int32 PCLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(DisplayPCClass);

	// 현재 장착 상태에 따른 값을 가져오는데 GetEquipApplied** 씨리즈에서는 옵션에 따른 수치까지 고려될 것이다.
	const int32 CurrentAttack = (int32)CombatStatEval::GetPCAttack(DisplayPCClass);
	const int32 CurrentDefense = (int32)CombatStatEval::GetPCDefense(DisplayPCClass);
	const int32 CurrentHealth = (int32)CombatStatEval::GetPCHealth(DisplayPCClass);

	// PendingEquipSuggestedItemIDs 는 있다 치고 실제 아이템 데이터 얻어옴.
	TArray<FB2Item> EquipSuggestedItems;
	for (int64 CurrEquipSuggestID : PendingEquipSuggestedItemIDs)
	{
		// GetEquipSuggestion 에서도 하는 동작들인데 개별 ID 별로 다시 찾아오는군. 안습 ㄷㄷ 낄낄
		FB2Item CurrSuggestItem;
		if (UB2LobbyInventory::FindStoredItem(CurrSuggestItem, CurrEquipSuggestID))
		{
			EquipSuggestedItems.Add(CurrSuggestItem);
		}
	}

	// 여기에 장착 상태 유지하는 애들 리스트도 더해야 한다.
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

		if (!bHasSuggested){ // 이번 장착된 장비에 해당하는 권장 아이템이 없음.
			NonSuggestedEquipList.Add(CurrEquipped);
		}
	}

	EquipSuggestedItems.Append(NonSuggestedEquipList); // 장착 권장 + 기존 장착 중 권장 없는 것 합.

	FB2Wing WingData; // GetEquip** 에서는 안에서 wing 을 감안했을 것이므로 여기서도 넣어줌.
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
	// 로비 인벤토리에서 현재 선택된 PC 에 대해 가져오기 위해 EPC_End 를 사용했는데 static 함수를 사용하지만 
	// 사실상 LobbyInventory 가 생성되어 있는 상태에서나 현재 선택된 PC 가 제대로 인식이 될 것.
	UB2LobbyInventory::GetAllEquippedItem(CurrPCEquipItemList, EPCClass::EPC_End);

	for (int32 EI = 0; EI < CurrPCEquipItemList.Num(); ++EI)
	{
		FB2Item& CurrEquip = CurrPCEquipItemList[EI];

		UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(EquipItemIconWidgetClass->GetDefaultObject());

		// 설치한 canvas panel 위에 EquipItemIcon 을 생성.
		UB2DynItemIcon_LobbyEquip* NewIcon = Cast<UB2DynItemIcon_LobbyEquip>(DynCreateInCanvasPanel(
			EquipItemIconWidgetClass, this, GetEquipItemIconPanelNativeRef(CurrEquip.EquipPlace), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f),
			GetAllowedSingleEquipItemIconSize(CurrEquip.EquipPlace), FVector2D(0.0f, 0.0f), 0, false
			));

		if (NewIcon)
		{
			NewIcon->SetEquipItemIconType(ELobbyEquipItemIconType::EEIIT_AutoEquipCurrent);
			NewIcon->UpdateItemData(CurrEquip); // 개별 아이템 정보.
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
		// GetEquipSuggestion 에서도 하는 동작들인데 개별 ID 별로 다시 찾아오는군. 안습 ㄷㄷ 낄낄
		// GetEquipSuggestion 이 제대로 동작했다면 CurrSuggestItem 이 CurrEquipped 보다 좋아야 함. (아니면 CurrEquipped 가 없거나)
		FB2Item CurrSuggestItem;
		if(!UB2LobbyInventory::FindStoredItem(CurrSuggestItem, CurrEquipSuggestID))
			continue;
		FB2Item CurrEquipped;
		bool bCurrentlyEquippedAny = UB2LobbyInventory::GetEquippedItemAtPlace(CurrEquipped, CurrSuggestItem.EquipPlace);
		if (!bCurrentlyEquippedAny){ // 장착된 것이 없다면 0 과 비교하도록 값들을 좀 세팅.
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
			NewIcon->UpdateStatIncSigns(CurrEquipped); // 비교 대상을 넣어줌 ㅋ
			DestroyEquipItemIcon(CurrSuggestItem.EquipPlace); // 기존에 생성했던 게 있다면 지운다.
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
		PlayAutoEquipCommitSoundBP(); // 실제 장착이 되는 상황에서는 사운드를 플레이 한다.
	}

	// UpdateOnEquipChange 에서 불리도록 하려고 했는데 걍 장착 되든 말든 닫히도록 해 달라고 해서.
	//if (bAutoEquipRequested)
	{ // 자동 닫기.
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
			// 인벤토리 탭 확인 없이 보관함에서 제거될 수 있는 것인 만큼 신규 획득 표시 리스트에서도 제거. 보다 확실하게는 응답이 왔을 때 해야 되긴 함. 그렇게까지 철저하게 해야 될 필요까진 없을 거 같고..
//			BladeIIGameImpl::GetRedDotManager().RemoveNewItem(CurrSuggestItem);
		}
	}

	PendingEquipSuggestedItemIDs.Empty();

	// 실제 서버에서의 장착 응답이 오면 아이콘 업데이트가 될 것.

	return bAtLeastOnePending;
}

void UB2LobbyUI_AutoEquip::CloseMe()
{
	DestroyAllItemIcons();
	//DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_AutoEquip);
}