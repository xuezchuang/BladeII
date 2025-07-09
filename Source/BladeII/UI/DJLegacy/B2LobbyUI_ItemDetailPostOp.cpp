// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2LobbyUI_ItemDetailPostOp.h"
#include "B2UIManager_Lobby.h"
#include "B2Inventory.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2LobbyUISwitcher.h"
#include "Event.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2LobbyUI_ItemDetailSetEffect.h"
#include "B2UICostumeItemDetailSetEffect.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "TutorialManager.h"
#include "B2AndroidBackManager.h"
#include "B2UIItemDetailOption.h"


UB2LobbyUI_ItemDetailPostOp::UB2LobbyUI_ItemDetailPostOp(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedInventory = NULL;
	MyItemOpMode = ELobbyInvenItemOpMode::EIVIOP_None;

	ItemIconWidgetClass = NULL;
	CreatedItemIcon = NULL;
	
	ItemStatIncColor = FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f));
	ItemStatSameColor = FSlateColor(FLinearColor(10.0f, 10.0f, 10.0f));
	ItemStatDecColor = FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f));

	JackpotFxBeginSeconds = 0.0f;
	IsLevelupDuringJackpot = false;
}

void UB2LobbyUI_ItemDetailPostOp::OnUpdateProgressLevel(int32 CurrentLevel)
{
	if (PB_ItemExpProgress.IsValid() && ExpProgressAnim.IsApplied())
		PB_ItemExpProgress->SetVisibility(ESlateVisibility::Hidden);

	if (TB_ItemLevel.IsValid())
		TB_ItemLevel->SetText(FText::AsNumber(CurrentLevel));
}

void UB2LobbyUI_ItemDetailPostOp::OnUpdateProgressPercent(float CurrentPercent)
{
	// 최대 레벨에선 만땅 채워주기
	const bool bAtMaxLevel = (ExpProgressAnim.GetCurrentAnimatedLevel() == NativeItemData_After.MaxLevel);

	if (PB_ItemJackPotExpProgress.IsValid())
		PB_ItemJackPotExpProgress->SetPercent(bAtMaxLevel ? 1.0f : CurrentPercent);
	
	if (TB_ItemExpPercent.IsValid())
	{
		TB_ItemExpPercent->SetText(
			bAtMaxLevel ? BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Maximum_ValueMark")) :
			FText::FromString(FString::Printf(TEXT("%.1f %%"), CurrentPercent * 100.0f))
			);
	}	
}

void UB2LobbyUI_ItemDetailPostOp::OnFinishProgressAnimation()
{
	OnBeginJackpotAnimation_BP((CachedInventory && CachedInventory->GetCachedItemLevelupJackpotType() == EItemLevelUpJackpotType::ER_GreateJackpot));

	// jackpot으로 레벨업이 되었으면 레벨업 연출을 호출
	if (IsLevelupDuringJackpot)
		OnMoveItemIconByJackpotLevelup_BP();
}

void UB2LobbyUI_ItemDetailPostOp::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	B2AndroidBackManager::GetInstance()->AddBackWidget(this);

	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);
	MyItemOpMode = CachedInventory->GetItemOpMode();
	SetResultTypeText(MyItemOpMode);

	ExpProgressAnim.OnLevelChanged.AddUObject(this, &UB2LobbyUI_ItemDetailPostOp::OnUpdateProgressLevel);
	ExpProgressAnim.OnPrecentChanged.AddUObject(this, &UB2LobbyUI_ItemDetailPostOp::OnUpdateProgressPercent);
	ExpProgressAnim.OnFinishProgAnimation.AddUObject(this, &UB2LobbyUI_ItemDetailPostOp::OnFinishProgressAnimation);
	
	X_CP_Jackpot->SetVisibility(ESlateVisibility::Hidden);
}

void UB2LobbyUI_ItemDetailPostOp::NativeConstruct()
{
	Super::NativeConstruct();
}

void UB2LobbyUI_ItemDetailPostOp::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UCanvasPanel, X_CP_ItemIconPanel)
	GET_SLOT(UCanvasPanel, X_CP_Jackpot)

	GET_SLOT(UTextBlock, TB_Combat)
	GET_SLOT(UTextBlock, TB_CombatPower)
	GET_SLOT(UTextBlock, TB_CombatPowerDelta)
	GET_SLOT(UTextBlock, TB_ResultType)
	GET_SLOT(UTextBlock, TB_PrimaryPointType)
	GET_SLOT(UTextBlock, TB_PrimaryPoint)
	GET_SLOT(UTextBlock, TB_PrimaryPointDelta)
	GET_SLOT(UTextBlock, TB_ItemLevel)
	GET_SLOT(UTextBlock, TB_MaxItemLevel)
	GET_SLOT(UTextBlock, TB_Enhance)
	GET_SLOT(UTextBlock, TB_EnhanceLevel)
	GET_SLOT(UTextBlock, TB_EnhanceLevelDelta)

	GET_SLOT(UProgressBar, PB_ItemExpProgress)
	GET_SLOT(UProgressBar, PB_ItemJackPotExpProgress)
	GET_SLOT(UTextBlock, TB_ItemExpPercent)

	GET_SLOT(UButton, BTN_Confirm)
	GET_SLOT(UButton, BTN_Receive)
	GET_SLOT(UTextBlock, TB_ConfirmBtn)

	GET_SLOT(UVerticalBox, VB_SpecialInfoDisplayPanel)

	GET_SLOT(UB2DynItemIcon_LobbyInven,RaffleIcon)
	GET_SLOT(UOverlay, X_O_ExpDelta)
	GET_SLOT(UTextBlock, TB_ExpDelta)

	GET_SLOT(UPanelWidget , X_P_ItemDescriptionPanel)
	GET_SLOT(UPanelWidget, X_P_ItemInfoDisplayPanel)

	GET_SLOT(UB2RichTextBlock, RTB_ItemDescription)
	GET_SLOT(UB2UIItemDetailOption, UIP_ItemDetailOption)

	GET_SLOT(UPanelWidget, EnhanceExpProgressSet)
		
	Anim_ItemRaffle = GetAnimation(this, "Anim_ItemRaffle");

}

void UB2LobbyUI_ItemDetailPostOp::DestroySelf()
{
	Super::DestroySelf();

	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

	DestroyItemIcon();

	ExpProgressAnim.ClearCustomTimerTick();
	ExpProgressAnim.OnLevelChanged.RemoveAll(this);
	ExpProgressAnim.OnPrecentChanged.RemoveAll(this);

	if (JackpotBeginHandle.IsValid())
		GetOwningPlayer()->GetWorldTimerManager().ClearTimer(JackpotBeginHandle);

	if (UIP_ItemDetailOption.IsValid())
	{
		UIP_ItemDetailOption->DestroySelf();
	}
}

void UB2LobbyUI_ItemDetailPostOp::UpdateItemData(const FB2Item& InBeforeLevelupItem, const FB2Item& InAfterLevelupItem)
{
	NativeItemData_Before = InBeforeLevelupItem;
	NativeItemData_After = InAfterLevelupItem;
	
	FB2Item PreviewItem;
	GetPreviewLevelupProgressItem(IsJackpot(), NativeItemData_Before, NativeItemData_After, PreviewItem);

	UpdateItemIcon(PreviewItem);
	UpdateSubWidgets(NativeItemData_Before, PreviewItem); // 본격 배치한 widget 들 업데이트
	UpdateExpProcess(PreviewItem.Level, PreviewItem.Exp);
	UpdateItemOptions(); // 많은 부분을 차지하는 옵션 리스트.
	UpdataItemSetEffect();

	UpdateEssence(InAfterLevelupItem);
}

void UB2LobbyUI_ItemDetailPostOp::UpdateEssence(const FB2Item& InItem)
{
	bool IsEssenceType = false;

	if (InItem.ItemClass == EItemClass::EIC_WeaponEssence ||
		InItem.ItemClass == EItemClass::EIC_ProtectionEssence ||
		InItem.ItemClass == EItemClass::EIC_AccessoryEssence ||	
		InItem.ItemClass == EItemClass::EIC_WeaponSurpassAnvil ||
		InItem.ItemClass == EItemClass::EIC_ArmorSurpassAnvil ||
		InItem.ItemClass == EItemClass::EIC_AccessorySurpassAnvil
		)
	{
		IsEssenceType = true;
	}

	if(X_P_ItemInfoDisplayPanel.IsValid())
		X_P_ItemInfoDisplayPanel->SetVisibility(IsEssenceType ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	if (UIP_ItemDetailOption.IsValid())
	{
		UIP_ItemDetailOption->SetVisibility(IsEssenceType ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}

	if (VB_SpecialInfoDisplayPanel.IsValid())
		VB_SpecialInfoDisplayPanel->SetVisibility(IsEssenceType ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	if (X_P_ItemDescriptionPanel.IsValid())
		X_P_ItemDescriptionPanel->SetVisibility(IsEssenceType ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (X_P_ItemDescriptionPanel.IsValid())
		RTB_ItemDescription->SetText(BladeIIGetLOCText(B2LOC_CAT_ITEMNAMESUB, FString::FromInt(InItem.ItemRefID)));
}

void UB2LobbyUI_ItemDetailPostOp::StartFromRaffle(const FB2Item& ItemInfo)
{
	if (Anim_ItemRaffle.IsValid())
		PlayAnimation(Anim_ItemRaffle.Get());

	NativeItemData_After = ItemInfo;
	UB2LobbyInventory::GetEquippedItemAtPlace(NativeItemData_Before, ItemInfo.EquipPlace, EPCClass(ItemInfo.AllowedPCClass));

	CreatedItemIcon = RaffleIcon.Get();

	if (CreatedItemIcon)
	{
		CreatedItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemRafle); // 보관함에서 보여지던 몇가지 정보는 보여주지 않도록 추가 설정.
		CreatedItemIcon->UpdateItemData(NativeItemData_After); // 기본적으로는 After 정보를 쓴다.
	}
	UpdateItemData(NativeItemData_Before, NativeItemData_After);
}

void UB2LobbyUI_ItemDetailPostOp::OnChangeItemIconByJackpotLevelup()
{
	// 잭팟 연출이 끝나면 슬롯을 실제 셋팅으로 변경해줌
	UpdateItemIcon(NativeItemData_After);
	UpdateSubWidgets(NativeItemData_Before, NativeItemData_After, false);
}

void UB2LobbyUI_ItemDetailPostOp::OnRaffleAnimationFinished()
{
}

void UB2LobbyUI_ItemDetailPostOp::UpdateItemIcon(const FB2Item& InItem)
{
	//DestroyItemIcon();	// spivy
	if (ItemIconWidgetClass == NULL || !X_CP_ItemIconPanel.IsValid())
		return;
	
	// Create ItemIcon
	if (CreatedItemIcon == nullptr)
	{
		// Anchor 설정에 따라 GetSize 는 원하는 값이 안 나올 것이므로 주의.
		UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(X_CP_ItemIconPanel->Slot);
		FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);
		UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

		// ItemIconPanelNRef 위에 최종적인 아이템 아이콘 widget 생성
		CreatedItemIcon = Cast<UB2DynItemIcon_LobbyInven>(DynCreateInCanvasPanel(ItemIconWidgetClass, this, X_CP_ItemIconPanel.Get(),
			DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize, FVector2D(0.0f, 0.0f), 0, false));

		if (CreatedItemIcon)
			CreatedItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemDetailWindow); // 보관함에서 보여지던 몇가지 정보는 보여주지 않도록 추가 설정.)
	}

	if (CreatedItemIcon)
		CreatedItemIcon->UpdateItemData(InItem);
}

void UB2LobbyUI_ItemDetailPostOp::DestroyItemIcon()
{
	if (CreatedItemIcon)
	{
		CreatedItemIcon->DestroySelf();
		CreatedItemIcon = NULL;
	}
}

void UB2LobbyUI_ItemDetailPostOp::OnVisibleUpdate()
{
	if (IsJackpot())
		GetOwningPlayer()->GetWorldTimerManager().SetTimer(JackpotBeginHandle, FTimerDelegate::CreateUObject(this, &UB2LobbyUI_ItemDetailPostOp::StartJackpotAnimation), JackpotFxBeginSeconds, false);
}

void UB2LobbyUI_ItemDetailPostOp::StartJackpotAnimation()
{
	ExpProgressAnim.ApplyCustomTimerTick(this);

	//EItemLevelUpJackpotType JackPotType = EItemLevelUpJackpotType::ER_Jackpot;
	//if (CachedInventory)
	//	JackPotType = CachedInventory->GetCachedItemLevelupJackpotType();
	//if (PB_ItemExpProgress.IsValid() && JackPotFillImages.Contains(JackPotType))
	//	PB_ItemExpProgress->WidgetStyle.SetFillImage(JackPotFillImages[JackPotType]);
}

bool UB2LobbyUI_ItemDetailPostOp::IsJackpot()
{
#if WITH_EDITORONLY_DATA
	if (bEnhanceTest == true)
		return true;
#endif

	// 레벨업에서만 대박/초대박이 있음.
	if (MyItemOpMode != ELobbyInvenItemOpMode::EIVIOP_Levelup)
		return false;

	return (CachedInventory && CachedInventory->IsItemLevelupJackpot());
}

void UB2LobbyUI_ItemDetailPostOp::GetPreviewLevelupProgressItem(bool InIsJackpot, const FB2Item& InBeforeItem, const FB2Item& InAfterItem, FB2Item& OutPreviewITem)
{
	OutPreviewITem = InAfterItem;

	if (MyItemOpMode != ELobbyInvenItemOpMode::EIVIOP_Levelup)
		return;

	if (InIsJackpot == false || CachedInventory == nullptr)
		return;

	// Jackpot의 경우 Preview[기대값] 을 먼저 보여준 후 Jackpot 수치만큼 추가로 Level, Exp를 올려주는 연출이 필요
	
	TArray<FB2Item> CachedIngredItems;
	CachedInventory->GetAllGhostIngreds(CachedIngredItems);
	if (CachedIngredItems.Num() <= 0)
		return;

	// jackpot Level,exp는 Preview[ 서버와 동일 공식 ] 값과 다름
	GetCalculateItem_PreviewItemLevelup(InBeforeItem, CachedIngredItems, OutPreviewITem, true);
}

void UB2LobbyUI_ItemDetailPostOp::UpdateSubWidgets(const FB2Item& InBeforeItem, const FB2Item& InAfterItem, bool IsChangeExpBar /*= true*/)
{	 
	if (TB_ItemLevel.IsValid())
		TB_ItemLevel->SetText(FText::AsNumber(InAfterItem.Level));

	if (TB_MaxItemLevel.IsValid())
		TB_MaxItemLevel->SetText(FText::AsNumber(InAfterItem.MaxLevel));

	if (TB_EnhanceLevel.IsValid())
		TB_EnhanceLevel->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Stat_EnhanceLevel")), FText::AsNumber(InAfterItem.EnhanceLevel)));

	// 최대 레벨에선 만땅 채워주기
	const bool bAtMaxLevel = (InAfterItem.Level == InAfterItem.MaxLevel);

	if (PB_ItemExpProgress.IsValid() && IsChangeExpBar)
		PB_ItemExpProgress->SetPercent(bAtMaxLevel ? 1.0f : InAfterItem.Exp * 0.01f);

	if (PB_ItemJackPotExpProgress.IsValid())
		PB_ItemJackPotExpProgress->SetPercent(bAtMaxLevel ? 1.0f : InAfterItem.Exp * 0.01f);
	
	if (TB_ItemExpPercent.IsValid())
	{
		TB_ItemExpPercent->SetText(
			bAtMaxLevel ? BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Maximum_ValueMark")) :
			FText::FromString(FString::Printf(TEXT("%.1f %%"), InAfterItem.Exp))
			);
	}

	float DeltaExp = 0.0f;
	if (MyItemOpMode == ELobbyInvenItemOpMode::EIVIOP_Levelup)
	{
		DeltaExp = InAfterItem.Exp - InBeforeItem.Exp;

		if (InAfterItem.Level != InBeforeItem.Level)
			DeltaExp += (100.f * (InAfterItem.Level - InBeforeItem.Level));
	}

	if (X_O_ExpDelta.IsValid())
		X_O_ExpDelta->SetVisibility((DeltaExp != 0.f) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (TB_ExpDelta.IsValid())
		TB_ExpDelta->SetText(FText::FromString(FString::Printf(TEXT("%.1f %%"), DeltaExp)));

	if (PB_ItemJackPotExpProgress.IsValid())
		PB_ItemJackPotExpProgress->SetVisibility(IsJackpot() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (TB_Enhance.IsValid())
		TB_Enhance->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_EnhanceLevelDesc")));

	if (TB_Combat.IsValid())
		TB_Combat->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raking_CombatPower")));


	if (TB_ConfirmBtn.IsValid())
		TB_ConfirmBtn->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Confirm"))));

	if (TB_CombatPower.IsValid())
		TB_CombatPower->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)InAfterItem.Power)));

	if (TB_PrimaryPointType.IsValid())
		TB_PrimaryPointType->SetText(GetLOCTextOfPrimPointType(InAfterItem.PrimaryPointType));

	if (TB_PrimaryPoint.IsValid())
		TB_PrimaryPoint->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)InAfterItem.GetPrimaryPoint())));
	
	int32 CombatDelta = InAfterItem.Power - InBeforeItem.Power;
	int32 PrimaryPointDelta = (int32)InAfterItem.GetPrimaryPoint() - (int32)InBeforeItem.GetPrimaryPoint();
	int32 EnhanceLevelDelta = InAfterItem.EnhanceLevel - InBeforeItem.EnhanceLevel;

	// 기존 아이템 상세 창에서 기능을 가져와서 이런데 사실상 강화 후면 + 가 나오겠지?	
	FString CombatDeltaString = (CombatDelta == 0) ? TEXT("( - )") : FString::Printf(TEXT("( %s%d )"), (CombatDelta > 0) ? TEXT("+") : TEXT("-"), FMath::Abs(CombatDelta));
	FString PrimaryDeltaString = (PrimaryPointDelta == 0) ? TEXT("( - )") : FString::Printf(TEXT("( %s%d )"), (PrimaryPointDelta > 0) ? TEXT("+") : TEXT("-"), FMath::Abs(PrimaryPointDelta));
	FString EnhanceLevelDeltaString = (EnhanceLevelDelta == 0) ? TEXT("( - )") : FString::Printf(TEXT("( %s%d )"), (EnhanceLevelDelta> 0) ? TEXT("+") : TEXT("-"), FMath::Abs(EnhanceLevelDelta));

	if (TB_CombatPowerDelta.IsValid())
	{
		TB_CombatPowerDelta->SetText(FText::FromString(CombatDeltaString));
		TB_CombatPowerDelta->SetColorAndOpacity((CombatDelta > 0) ? ItemStatIncColor : (CombatDelta == 0 ? ItemStatSameColor : ItemStatDecColor));
	}

	if (TB_PrimaryPointDelta.IsValid())
	{
		TB_PrimaryPointDelta->SetText(FText::FromString(PrimaryDeltaString));
		TB_PrimaryPointDelta->SetColorAndOpacity((PrimaryPointDelta > 0) ? ItemStatIncColor : (PrimaryPointDelta == 0 ? ItemStatSameColor : ItemStatDecColor));
	}

	if (TB_EnhanceLevelDelta.IsValid())
	{
		TB_EnhanceLevelDelta->SetText(FText::FromString(EnhanceLevelDeltaString));
		TB_EnhanceLevelDelta->SetColorAndOpacity((EnhanceLevelDelta > 0) ? ItemStatIncColor : (EnhanceLevelDelta == 0 ? ItemStatSameColor : ItemStatDecColor));
	}

	if (EnhanceExpProgressSet.IsValid())
		EnhanceExpProgressSet->SetVisibility(IsCostumeItem(InAfterItem) ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UB2LobbyUI_ItemDetailPostOp::UpdateExpProcess(int32 InCurItemLevel, float InCurItemExp)
{
	// 잭팟일 경우만 게이지 연출을 켜준다.
	if (IsJackpot() == false)
		return;

	ExpProgressAnim.SetTargetValues(InCurItemLevel, InCurItemExp * 0.01f, true);
	ExpProgressAnim.SetTargetValues(NativeItemData_After.Level, NativeItemData_After.Exp * 0.01f, false);

	IsLevelupDuringJackpot = (InCurItemLevel != NativeItemData_After.Level);
}

void UB2LobbyUI_ItemDetailPostOp::UpdataItemSetEffect()
{
	if (VB_SpecialInfoDisplayPanel.IsValid() == false)
		return;

	if (IsItemSetEffectAvailable(NativeItemData_After) == false)
		return;

	if (!IsCostumeItem(NativeItemData_After))
	{
		if (UB2LobbyUI_ItemDetailSetEffect* SetItemInfo = CreateWidget<UB2LobbyUI_ItemDetailSetEffect>(GetWorld(), SetEffectInfoDisplayClass))
		{
			VB_SpecialInfoDisplayPanel->ClearChildren();
			VB_SpecialInfoDisplayPanel->AddChild(SetItemInfo);
			SetItemInfo->UpdateItemData(NativeItemData_After);
		}
	}
	else
	{
		if (UB2UICostumeItemDetailSetEffect* SetItemInfo = CreateWidget<UB2UICostumeItemDetailSetEffect>(GetWorld(), CostumeSetEffectInfoDisplayClass))
		{
			VB_SpecialInfoDisplayPanel->ClearChildren();
			VB_SpecialInfoDisplayPanel->AddChild(SetItemInfo);
			SetItemInfo->InitCostumeItemDetailSetEffect(NativeItemData_After);
		}
	}
}

void UB2LobbyUI_ItemDetailPostOp::UpdateItemOptions()
{
	if (UIP_ItemDetailOption.IsValid())
	{
		UIP_ItemDetailOption->SetItemData(NativeItemData_After, NativeItemData_Before, false, ItemStatIncColor, ItemStatSameColor, ItemStatDecColor);
	}
/*	if (NativeItemData_After.IntrinsicOptions.Num() > 0)
	{
		// Padding 은 일단 주지 않는다.
		CreatedIntrinsicOptionTitle = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(IntrinsicOptionTitleClass, this, VB_OptionsDisplayPanel.Get(), FMargin()));
		if (CreatedIntrinsicOptionTitle)
		{
			CreatedIntrinsicOptionTitle->SetDynText(0, // 걍 첫줄만 있음.
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyItemDetail_IntrinsicOptionPanelTitle")))
				);
		}

		for (int32 AI = 0; AI < NativeItemData_After.IntrinsicOptions.Num(); ++AI)
		{
			FItemOption& CurrOption = NativeItemData_After.IntrinsicOptions[AI];
			FItemOption SameOptionOfBefore; // 강화 이전의 동일 옵션 수치
			bool bFoundSameOptionOfBefore = false; // 물론 true 여야겠지? 아마도?
			
			// bFoundSameOptionOfBefore 가 true 이면 SameOptionOfEquipped 가 valid 한 것.
			bFoundSameOptionOfBefore = GetItemIntrinsicOptionOfType(NativeItemData_Before, SameOptionOfBefore, CurrOption.OptionType);
			
			UB2DynText_ItemDetailOption* CurrCreated = Cast<UB2DynText_ItemDetailOption>(DynCreateInVerticalBox(IntrinsicOptionDisplayClass, this, VB_OptionsDisplayPanel.Get(), FMargin()));
			if (CurrCreated)
			{ // SameOptionOfBefore 는 찾은 경우 아니면 NULL 로. AllowedPCClass 는 일부 캐릭터 별로 다르게 들어가는 옵션 표시를 위함인데, 공용이라면 그런 옵션이 없어야겠지..?
				CurrCreated->UpdateOptionTextSet(false, AI, ItemAllowedPCClassEnum(NativeItemData_After.AllowedPCClass), &CurrOption, bFoundSameOptionOfBefore ? &SameOptionOfBefore : NULL, ItemStatIncColor, ItemStatSameColor, ItemStatDecColor, true);
				CreatedIntrinsicOptionDisplay.Add(CurrCreated);
			}
		}
	}

	if (NativeItemData_After.RandomOptions.Num() > 0)
	{
		CreatedRandomOptionTitle = Cast<UB2DynText_Multiple>(DynCreateInVerticalBox(RandomOptionTitleClass, this, VB_OptionsDisplayPanel.Get(), FMargin()));
		if (CreatedRandomOptionTitle)
		{
			CreatedRandomOptionTitle->SetDynText(0, // 걍 첫줄만 있음.
				BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyItemDetail_RandOptionPanelTitle")))
				);
		}

		for (int32 AI = 0; AI < NativeItemData_After.RandomOptions.Num(); ++AI)
		{
			FItemOption& CurrOption = NativeItemData_After.RandomOptions[AI];
			FItemOption SameOptionOfBefore; // 강화 이전의 동일 옵션 수치
			bool bFoundSameOptionOfBefore = false; // 물론 true 여야겠지? 아마도?
			// bFoundSameOptionOfBefore 가 true 이면 SameOptionOfBefore 가 valid 한 것.
			bFoundSameOptionOfBefore = GetItemRandomOptionOfType(NativeItemData_Before, SameOptionOfBefore, CurrOption.OptionType);

			UB2DynText_ItemDetailOption* CurrCreated = Cast<UB2DynText_ItemDetailOption>(DynCreateInVerticalBox(RandomOptionDisplayClass, this, VB_OptionsDisplayPanel.Get(), FMargin()));
			if (CurrCreated)
			{ // SameOptionOfBefore 는 찾은 경우 아니면 NULL 로.
				CurrCreated->UpdateOptionTextSet(false, AI, ItemAllowedPCClassEnum(NativeItemData_After.AllowedPCClass), &CurrOption, bFoundSameOptionOfBefore ? &SameOptionOfBefore : NULL, ItemStatIncColor, ItemStatSameColor, ItemStatDecColor);
				CreatedRandomOptionDisplay.Add(CurrCreated);
			}
		}
	}*/
}

void UB2LobbyUI_ItemDetailPostOp::CloseWidgetDelegate()
{
	OnConfirmButtonClicked();
}

void UB2LobbyUI_ItemDetailPostOp::OnConfirmButtonClicked()
{
	if (OnConfirmButtonClickedDelegate.IsBound())
		OnConfirmButtonClickedDelegate.Broadcast();
	else
		CloseMe();
}

void UB2LobbyUI_ItemDetailPostOp::SetShowMainItemIcon(bool bShow)
{
	if (CreatedItemIcon)
	{
		CreatedItemIcon->SetVisibility(bShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UB2LobbyUI_ItemDetailPostOp::ClosePopup()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	SetClose(false);
}

void UB2LobbyUI_ItemDetailPostOp::ShowBackButton()
{
	SetClose(true);
}

void UB2LobbyUI_ItemDetailPostOp::CloseMe()
{
	// 이 widget page 만이 아닌 ItemOp 모드를 닫는 것임.
	UGameplayStatics::SetGlobalTimeDilation(CachedLobbyGM, 1.0f); // 연출에서 OverallPlayRate 에 따라 변경했던 거 복구.
	QuitItemOpModeClass<bool>::GetInstance().Signal(true);

	UpdateLobbyInventoryControlClass<>::GetInstance().Signal();

	TUTORIAL // Tutorial Hardcoding
	if (TutorialManager::GetInstance().IsCurrentTutorial(TutorialID_ItemEnhance, 7))
	{
		TutorialManager::GetInstance().OnEnhanceDetailPopupClose();
	}
}

void UB2LobbyUI_ItemDetailPostOp::SetResultTypeText(ELobbyInvenItemOpMode InItemOpMode)
{
	if (TB_ResultType.IsValid() == false)
		return;

	switch (InItemOpMode)
	{
	case ELobbyInvenItemOpMode::EIVIOP_Levelup:
		TB_ResultType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemDetailPost_LevelupItemResult")));
		break;
	case ELobbyInvenItemOpMode::EIVIOP_Upgrade:
		TB_ResultType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemDetailPost_UpgradeItemResult")));
		break;
	case ELobbyInvenItemOpMode::EIVIOP_Composition:
		TB_ResultType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemDetailPost_ComposeItemResult")));
		break;
	case ELobbyInvenItemOpMode::EIVIOP_Surpass:
		TB_ResultType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ItemDetailPost_SurpassItemResult")));
		break;
	case ELobbyInvenItemOpMode::EIVIOP_Enhancement:
		{
		bool IsEnhanceSuccess = (CachedInventory->IsSuccessEnhanceItemForResultPresent());

		TB_ResultType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, IsEnhanceSuccess ? TEXT("ItemDetailPost_EnhanceItemSuccess") : TEXT("ItemDetailPost_EnhanceItemFail")));
		TB_ResultType->SetColorAndOpacity(IsEnhanceSuccess ? HeaderTextColor_EnhanceSuccess : HeaderTextColor_EnhanceFail);
		}
		break;
	case ELobbyInvenItemOpMode::EIVIOP_EnhancementCostume:
		TB_ResultType->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StatUpgradeSuccess")));
		break;
	}

}
