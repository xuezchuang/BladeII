// Fill out your copyright notice in the Description page of Project Settings.


#include "B2UI_TotemInvenFilter.h"

#include "Global.h"
#include "B2UIManager.h"
#include "BladeIIUtil.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyInventory.h"
#include "B2UIFilterCheckBox.h"

#include "BladeIIGameImpl.h"

void UB2UI_TotemInvenFilter::DestroySelf(class UB2UIManager* InUIManager)
{
	for (auto& CategoryFilter : CategoryFilters)
	{
		if (CategoryFilter.IsValid())
		{
			CategoryFilter->DestroySelf(InUIManager);
		}
	}

	for (auto& GradeFilter : GradeFilters)
	{
		if (GradeFilter.IsValid())
		{
			GradeFilter->DestroySelf(InUIManager);
		}
	}

	if (SmeltingFilter.IsValid())
	{
		SmeltingFilter->DestroySelf(InUIManager);
	}

	CategoryFilters.Empty();
	GradeFilters.Empty();
	Super::DestroySelf(InUIManager);
}

void UB2UI_TotemInvenFilter::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_TotemCategory);
	GET_SLOT(UTextBlock, TB_TotemGrade);
	GET_SLOT(UTextBlock, TB_TotemState);

	CategoryFilters.Empty();
	CategoryFilters.SetNum(CategoryFiltersNumMax);
	GET_SLOT_BYNAME(UB2UIFilterCheckBox, CategoryFilters[static_cast<int32>(ETotemBasePrefix::TBP_VALOROUS)],	UIP_Valor);
	GET_SLOT_BYNAME(UB2UIFilterCheckBox, CategoryFilters[static_cast<int32>(ETotemBasePrefix::TBP_PATRON)],		UIP_Protection);
	GET_SLOT_BYNAME(UB2UIFilterCheckBox, CategoryFilters[static_cast<int32>(ETotemBasePrefix::TBP_VIGOROUS)],	UIP_Vitality);
	for (auto& CategoryFilter : CategoryFilters)
	{
		if (CategoryFilter.IsValid())
		{
			CategoryFilter->Init();
		}
	}

	GradeFilters.Empty();
	GradeFilters.SetNum(GradeFiltersNumMax);
	GET_SLOT_BYNAME(UB2UIFilterCheckBox, GradeFilters[static_cast<int32>(ETotemGrade::TG_NORMAL)],	UIP_Normal);
	GET_SLOT_BYNAME(UB2UIFilterCheckBox, GradeFilters[static_cast<int32>(ETotemGrade::TG_MAGIC)],	UIP_Magic);
	GET_SLOT_BYNAME(UB2UIFilterCheckBox, GradeFilters[static_cast<int32>(ETotemGrade::TG_RARE)],	UIP_Rare);
	GET_SLOT_BYNAME(UB2UIFilterCheckBox, GradeFilters[static_cast<int32>(ETotemGrade::TG_HERO)],	UIP_Hero);
	GET_SLOT_BYNAME(UB2UIFilterCheckBox, GradeFilters[static_cast<int32>(ETotemGrade::TG_LEGEND)],	UIP_Legend);
	GET_SLOT_BYNAME(UB2UIFilterCheckBox, GradeFilters[static_cast<int32>(ETotemGrade::TG_RELIC)],	UIP_Relic);
	for (auto& GradeFilter : GradeFilters)
	{
		if (GradeFilter.IsValid())
		{
			GradeFilter->Init();
		}
	}

	GET_SLOT_BYNAME(UB2UIFilterCheckBox, SmeltingFilter, UIP_Smelting);
	if (SmeltingFilter.IsValid())
	{
		SmeltingFilter->Init();
	}
}

void UB2UI_TotemInvenFilter::UpdateStaticText()
{
	if (TB_TotemCategory.IsValid())
	{
		TB_TotemCategory->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Category"))));
	}
	if (TB_TotemGrade.IsValid())
	{
		TB_TotemGrade->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Grade"))));
	}
	if (TB_TotemState.IsValid())
	{
		TB_TotemState->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyInvenText_Etc"))));
	}

	// CategoryFilters
	if (CategoryFilters[static_cast<int32>(ETotemBasePrefix::TBP_VALOROUS)].IsValid())
	{
		CategoryFilters[static_cast<int32>(ETotemBasePrefix::TBP_VALOROUS)]->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_prefix_valor"))));
	}
	if (CategoryFilters[static_cast<int32>(ETotemBasePrefix::TBP_PATRON)].IsValid())
	{
		CategoryFilters[static_cast<int32>(ETotemBasePrefix::TBP_PATRON)]->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_prefix_protect"))));
	}
	if (CategoryFilters[static_cast<int32>(ETotemBasePrefix::TBP_VIGOROUS)].IsValid())
	{
		CategoryFilters[static_cast<int32>(ETotemBasePrefix::TBP_VIGOROUS)]->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_prefix_vitality"))));
	}

	// GradeFilters
	if (GradeFilters[static_cast<int32>(ETotemGrade::TG_NORMAL)].IsValid())
	{
		GradeFilters[static_cast<int32>(ETotemGrade::TG_NORMAL)]->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_Grade_Normal"))));
	}
	if (GradeFilters[static_cast<int32>(ETotemGrade::TG_MAGIC)].IsValid())
	{
		GradeFilters[static_cast<int32>(ETotemGrade::TG_MAGIC)]->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_Grade_Magic"))));
	}
	if (GradeFilters[static_cast<int32>(ETotemGrade::TG_RARE)].IsValid())
	{
		GradeFilters[static_cast<int32>(ETotemGrade::TG_RARE)]->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_Grade_Rare"))));
	}
	if (GradeFilters[static_cast<int32>(ETotemGrade::TG_HERO)].IsValid())
	{
		GradeFilters[static_cast<int32>(ETotemGrade::TG_HERO)]->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_Grade_Hero"))));
	}
	if (GradeFilters[static_cast<int32>(ETotemGrade::TG_LEGEND)].IsValid())
	{
		GradeFilters[static_cast<int32>(ETotemGrade::TG_LEGEND)]->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_Grade_Legend"))));
	}
	if (GradeFilters[static_cast<int32>(ETotemGrade::TG_RELIC)].IsValid())
	{
		GradeFilters[static_cast<int32>(ETotemGrade::TG_RELIC)]->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_Grade_Artifact"))));
	}

	// SmeltingFilters
	if (SmeltingFilter.IsValid())
	{
		SmeltingFilter->SetLabel(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_smelting"))));
	}
}

void UB2UI_TotemInvenFilter::BindDelegates()
{
	for (auto& CategoryFilter : CategoryFilters)
	{
		if (CategoryFilter.IsValid())
		{
			CategoryFilter->OnCheckStateChanged.AddDynamic(this, &UB2UI_TotemInvenFilter::FilterCBChanged);
		}
	}

	for (auto& GradeFilter : GradeFilters)
	{
		if (GradeFilter.IsValid())
		{
			GradeFilter->OnCheckStateChanged.AddDynamic(this, &UB2UI_TotemInvenFilter::FilterCBChanged);
		}
	}

	if (SmeltingFilter.IsValid())
	{
		SmeltingFilter->OnCheckStateChanged.AddDynamic(this, &UB2UI_TotemInvenFilter::FilterCBChanged);
	}
}

void UB2UI_TotemInvenFilter::GetSelectedFilters(TArray<ETotemBasePrefix>& OutTotemBasePrefixFilter, TArray<ETotemGrade>& OutTotemGradeFilter, bool& IsSmelting)
{
	for (int32 i = 0 ; i < CategoryFilters.Num(); i++)
	{
		if (CategoryFilters[i].IsValid() && CategoryFilters[i]->IsChecked())
		{
			if (i >= static_cast<int32>(ETotemBasePrefix::TBP_MAX))
			{
				continue;
			}

			OutTotemBasePrefixFilter.Add(static_cast<ETotemBasePrefix>(i));
		}
	}

	for (int32 i = 0; i < GradeFilters.Num(); i++)
	{
		if (GradeFilters[i].IsValid() && GradeFilters[i]->IsChecked())
		{
			if (i >= static_cast<int32>(ETotemGrade::TG_MAX))
			{
				continue;
			}

			OutTotemGradeFilter.Add(static_cast<ETotemGrade>(i));
		}
	}

	IsSmelting = SmeltingFilter.IsValid() ? SmeltingFilter->IsChecked() : false;
}

void UB2UI_TotemInvenFilter::BindWarningPopup()
{
	for (int32 i = WarningPopupStartGrade; i < GradeFilters.Num(); i++)
	{
		if (GradeFilters[i].IsValid())
		{
			int32 RealGrade = i;

			OnPreSelectFilter OnPreSelect;
			OnPreSelect.BindLambda(
				[RealGrade, this](UB2UIFilterCheckBox* FilterCheckBox) {
				if (FilterCheckBox && FilterCheckBox->IsChecked())
					return;

				UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
					BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
					FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_Dismantle"))),
						GetGradeText(static_cast<ETotemGrade>(RealGrade)), this->FiltetTitleText),
					0.f,
					true,
					true,
					EUIMsgPopupButtonGroup::YesOrNo,
					FMsgPopupOnClick::CreateLambda([FilterCheckBox]() {
					if (FilterCheckBox)
						FilterCheckBox->SetIsChecked(true);
				}));
			});

			GradeFilters[i]->SetPreFilterCheck(OnPreSelect);
		}
	}
}

bool UB2UI_TotemInvenFilter::GetAllTargetTotems(TArray<FB2Totem> &OutList)
{
	TArray<ETotemBasePrefix>	TotemBasePrefixFilter;
	TArray<ETotemGrade>			TotemGradeFilter;
	bool IsSmelting;

	GetSelectedFilters(TotemBasePrefixFilter, TotemGradeFilter, IsSmelting);

	// 厘馒 棺 泪陛, 橇府悸俊 厘馒登绢 乐绰芭 力寇.
	GetFilteredItemList(OutList, IsSmelting, TotemBasePrefixFilter, TotemGradeFilter);

	return (OutList.Num() > 0);
}

void UB2UI_TotemInvenFilter::SaveFilterInfo(const TCHAR* InSectionName)
{
	for (int32 i = 0; i < CategoryFilters.Num(); i++)
	{
		if (CategoryFilters[i].IsValid())
		{
			GConfig->SetBool(InSectionName, *FString::Printf(TEXT("Category%d"), i), CategoryFilters[i]->IsChecked(), GB2UserSavedStateIni);
		}
	}

	for (int32 i = 0; i < GradeFilters.Num(); i++)
	{
		if (GradeFilters[i].IsValid())
		{
			GConfig->SetBool(InSectionName, *FString::Printf(TEXT("Grade%d"), i), GradeFilters[i]->IsChecked(), GB2UserSavedStateIni);
		}
	}

	if (SmeltingFilter.IsValid() == true)
	{
		GConfig->SetBool(InSectionName, *FString::Printf(TEXT("Smelting")), SmeltingFilter->IsChecked(), GB2UserSavedStateIni);
	}

	GConfig->Flush(false, GB2UserSavedStateIni);
}

void UB2UI_TotemInvenFilter::LoadFilterInfo(const TCHAR* InSectionName)
{
	bool CheckedValue = false;

	for (int32 i = 0; i < CategoryFilters.Num(); i++)
	{
		if (CategoryFilters[i].IsValid())
		{
			GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Category%d"), i), CheckedValue, GB2UserSavedStateIni);
			CategoryFilters[i]->SetIsChecked(CheckedValue);
		}
	}

	for (int32 i = 0; i < GradeFilters.Num(); i++)
	{
		if (GradeFilters[i].IsValid())
		{
			GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Grade%d"), i), CheckedValue, GB2UserSavedStateIni);
			GradeFilters[i]->SetIsChecked(CheckedValue);
		}
	}

	if (SmeltingFilter.IsValid() == true)
	{
		GConfig->GetBool(InSectionName, *FString::Printf(TEXT("Smelting")), CheckedValue, GB2UserSavedStateIni);
		SmeltingFilter->SetIsChecked(CheckedValue);
	}
}

void UB2UI_TotemInvenFilter::ResetFilter()
{
	for (int32 i = 0; i < CategoryFilters.Num(); ++i)
	{
		if (CategoryFilters[i].IsValid())
		{
			CategoryFilters[i]->SetIsChecked(false);
		}
	}

	for (int32 i = 0; i < GradeFilters.Num(); ++i)
	{
		if (GradeFilters[i].IsValid())
		{
			GradeFilters[i]->SetIsChecked(false);
		}
	}

	if (SmeltingFilter.IsValid())
	{
		SmeltingFilter->SetIsChecked(false);
	}
}

void UB2UI_TotemInvenFilter::SetFilterTitle(FText TitleText)
{
	FiltetTitleText = TitleText;
}

void UB2UI_TotemInvenFilter::SetVisibleEssenceCheckBox(ESlateVisibility InVisibility)
{

}

FText UB2UI_TotemInvenFilter::GetGradeText(ETotemGrade TotemGrade)
{
	switch (TotemGrade)
	{
	case ETotemGrade::TG_NORMAL: return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_Grade_Normal")));
	case ETotemGrade::TG_MAGIC: return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_Grade_Magic")));
	case ETotemGrade::TG_RARE: return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_Grade_Rare")));
	case ETotemGrade::TG_HERO: return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_Grade_Hero")));
	case ETotemGrade::TG_LEGEND: return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_Grade_Legend")));
	case ETotemGrade::TG_RELIC: return BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Totem_Grade_Artifact")));
	default: return FText::FromString(TEXT(""));
	}
}

void UB2UI_TotemInvenFilter::FilterCBChanged(bool bIsChecked)
{
	OnCheckStateChanged.Broadcast(bIsChecked);
}

void UB2UI_TotemInvenFilter::GetFilteredItemList(TArray<FB2Totem>& OutResult, bool bSmelting, TArray<ETotemBasePrefix>& CateogryFilter, TArray<ETotemGrade>& StarGradeFilter)
{
	TArray<FB2Totem> MergedList;
	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
	UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
	if (LobbyInven)
	{
		LobbyInven->GetUnEquippedTotemList(MergedList);
	}
																   
	//TArray 静搁 Resize 老绢朝锭 Lambda 罐篮 std::function捞 皋葛府 柄咙
	std::list<std::function<bool(FB2Totem)>> FIlters;
	// 瞒肥措肺 鞘磐 眠啊
	FIlters.emplace_back(
		[&CateogryFilter](FB2Totem CurrItem)
	{
		int CurrentGroupID = BladeIIGameImpl::GetTotemDataStore().GetTotemGroupID(CurrItem);

		for (ETotemBasePrefix CurrInvenType : CateogryFilter) // InvenTypeFilter 俊 乐绰 element 吝 绢蠢 窍唱客 老摹窍搁 器窃.
		{
			if (CurrentGroupID == static_cast<int32>(CurrInvenType) + 1)
			{
				return true;
			}
		}
		return false;
	});

	FIlters.emplace_back(
		[&StarGradeFilter](FB2Totem CurrItem)
	{
		for (ETotemGrade CurrStarGrade : StarGradeFilter) // StarGradeFilter 俊 乐绰 element 吝 绢蠢 窍唱客 老摹窍搁 器窃.
		{
			if (CurrItem.Grade == CurrStarGrade)
			{
				return true;
			}
		}
		return false;
	});

	// Lock 等 巴阑 吧矾郴绰 巴篮 磊悼魄概唱 磊悼碍拳 鞍篮 扁瓷俊辑 荤侩.
	FIlters.emplace_back(
		[](FB2Totem CurrItem)
	{
		return !CurrItem.IsLock;
	});

	if (!bSmelting)
	{
		FIlters.emplace_back(
			[](FB2Totem CurrItem)
		{
			return (!CurrItem.IsSmelting() && !CurrItem.IsSmeltingComplete());
		});
	}
	else
	{
		FIlters.emplace_back(
			[](FB2Totem CurrItem)
		{
			return (!CurrItem.IsSmeltingComplete());
		});
	}

	for (auto &Filter : FIlters)
	{
		if (MergedList.Num() == 0)
			break;
		MergedList = MergedList.FilterByPredicate(Filter);
	}

	OutResult = MergedList; // 搬苞 府胶飘肺 傈崔.
}
