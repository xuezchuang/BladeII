// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_ItemAfterOption.h"
#include "B2LobbyUI_ItemUpgradeMain.h"
#include "Event.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2DynItemIcon_LobbyInven.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "B2UIShortagePopupManager.h"

#include "BladeIIGameImpl.h"

UB2LobbyUI_ItemAfterOption::UB2LobbyUI_ItemAfterOption(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2LobbyUI_ItemAfterOption::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, STB_OptionTitle);
	GET_SLOT(UTextBlock, STB_Option);
	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UB2ScrollBox, SB_Options);

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2LobbyUI_ItemAfterOption::OnClickBTN_Close);
}

void UB2LobbyUI_ItemAfterOption::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	// �ܼ��� ������ ������ �������� Inventory �ν��Ͻ� ���� static ���� ���� �� ������ �ռ� ���� ����� �κ��丮�� �� �ִ� ���¿��� ����.
	CachedInventory = CachedLobbyGM->GetLobbyInventory();
	check(CachedInventory);

	SetPositionInViewport(FVector2D(0.0f, 0.0f));
	UpdateStaticTexts();
}

void UB2LobbyUI_ItemAfterOption::UpdateStaticTexts()
{
	if (STB_OptionTitle.IsValid())
		STB_OptionTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_PopupOption")));
}

void UB2LobbyUI_ItemAfterOption::DestroySelf()
{
	if (SB_Options.IsValid())
	{
		for (int32 i = 0; i < SB_Options->GetChildrenCount(); i++)
		{
			UB2LobbyUI_ItemAfterOptionSlot* ItemSlot = Cast<UB2LobbyUI_ItemAfterOptionSlot>(SB_Options->GetChildAt(i));
			if (ItemSlot)
				ItemSlot->DestroySelf();
		}

		SB_Options->ClearChildren();
	}

	Super::DestroySelf();
}

void UB2LobbyUI_ItemAfterOption::OnClickBTN_Close()
{
	ClickCloseDelegate.ExecuteIfBound();
	DJLegacy_CloseLobbySubPopupClass<ELobbySubPopups>::GetInstance().Signal(ELobbySubPopups::ELSPU_ItemAfterOpiton);
}

void UB2LobbyUI_ItemAfterOption::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2LobbyUI_ItemAfterOption::UpdateOnInfoStateChange()
{

}

void UB2LobbyUI_ItemAfterOption::UpdateItemData(FB2Item& InItemData, bool bIsOptionChange)
{
	FB2Item NextGradeItem = InItemData;
	NextGradeItem.StarGrade = bIsOptionChange ? InItemData.StarGrade : (InItemData.StarGrade + 1); // �±޽ÿ��� ���� ����� ��ġ�� �����ֱ⶧���� +1
	NextGradeItem.Level = bIsOptionChange ? InItemData.Level : 1; // �ɼǺ��涧���� ���� ������ AddValueByLevel �ݿ��� ���� ��������ϰ�, 
																 // �±޽ÿ��� ���� ����� 1 ������ ������

	const TArray<FSelectItemOption>* OptionArray = BladeIIGameImpl::GetClientDataStore().GetSelectItemOptionData(CliToSvrItemEquipPlace(NextGradeItem.EquipPlace), NextGradeItem.StarGrade);
	
	if (SB_Options.IsValid())
	{
		SB_Options->ClearChildren();

		if (OptionArray  != nullptr && (*OptionArray).Num() > 0)
		{
			for (int32 i = 0; i < (*OptionArray).Num(); i++)
			{
				FSelectItemOption CurrentOption = (*OptionArray)[i];
				UB2LobbyUI_ItemAfterOptionSlot* NewItemRow = CreateWidget<UB2LobbyUI_ItemAfterOptionSlot>(GetOwningPlayer(), ItemOptionWidgetClass);
				if (NewItemRow == NULL || CurrentOption.Prob == 0) // Prob �� 0 �̶�°��� �ش���񿡼��� ���� �ȳ����� �ɼ���.
					continue;

				float AddValueByLevel = BladeIIGameImpl::GetClientDataStore().GetItemLevelupFactor_PerOptionInc(SvrToCliOptionType(CurrentOption.OptionId));
				int32 LevelCount = static_cast<int32>(NextGradeItem.Level / 10); // 10�������� AddValueByLevel�� ������.
				float MinByLevel = CurrentOption.MinValue + (static_cast<float>(LevelCount)*AddValueByLevel);
				float MaxByLevel = CurrentOption.MaxValue + (static_cast<float>(LevelCount)*AddValueByLevel);

				SB_Options->AddChild(NewItemRow);
				NewItemRow->SetSkillInfo(CurrentOption.OptionId, MinByLevel, MaxByLevel, IntToPCClass(NextGradeItem.AllowedPCClass));
			}
		}
	}

	if (STB_Option.IsValid())
	{
		STB_Option->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, bIsOptionChange ? TEXT("LobbyInvenText_ChangeOptionGuide") : TEXT("LobbyInvenText_UpgradeOptionGuide")));
	}
}

// SkillSlot

void UB2LobbyUI_ItemAfterOptionSlot::NativeConstruct()
{
	Super::NativeConstruct();

}

void UB2LobbyUI_ItemAfterOptionSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_OptionName);
	GET_SLOT(UTextBlock, TB_MinValue);
	GET_SLOT(UTextBlock, TB_MaxValue);

}

void UB2LobbyUI_ItemAfterOptionSlot::DestroySelf()
{
	Super::DestroySelf();
}

void UB2LobbyUI_ItemAfterOptionSlot::SetSkillInfo(int32 OptionId, float MinValue, float MaxValue, EPCClass PCClass)
{
	if (TB_OptionName.IsValid())
		TB_OptionName->SetText(GetLOCTextOfItemOption(SvrToCliOptionType(OptionId), PCClass));
	
	if (TB_MinValue.IsValid())
		TB_MinValue->SetText(GetItemOptionValueDisplayText(SvrToCliOptionType(OptionId), MinValue, true));

	if (TB_MaxValue.IsValid())
		TB_MaxValue->SetText(GetItemOptionValueDisplayText(SvrToCliOptionType(OptionId), MaxValue, true));
}
