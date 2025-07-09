// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDimensionBattle.h"


void UB2UIDimensionBattle::CacheAssets()
{
	Super::CacheAssets(); 
	GET_SLOT(UTextBlock, TB_DimensionStep);
}

void UB2UIDimensionBattle::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIDimensionBattle::Init()
{
	Super::Init();

	CreateOrUpdateDimensionBuffIcon(EResurrectBuffType::RBT_Attack);
	CreateOrUpdateDimensionBuffIcon(EResurrectBuffType::RBT_Defense);
}

void UB2UIDimensionBattle::DestroySelf(UB2UIManager* InUIManager)
{
	for (UB2UIInGameBuffIcon* ThisBuffIcon : AllMyBuffIcons_Dimension)
	{
		if (ThisBuffIcon)
		{
			ThisBuffIcon->UnbindDoc();
			ThisBuffIcon->DestroySelf(UB2UIManager::GetInstance());
		}
	}
	AllMyBuffIcons_Dimension.Empty();

	Super::DestroySelf(InUIManager);
}

void UB2UIDimensionBattle::BindDocAuto()
{
	Super::BindDocAuto();

	// UIBattleMain 에서 사용안하는 것들은 꺼주자.
	SetWidgetForDimensionBattle();
}

void UB2UIDimensionBattle::UnbindDoc()
{
	Super::UnbindDoc();
}

void UB2UIDimensionBattle::SetWidgetForDimensionBattle()
{
	CurrentGameModeType = EB2GameMode::Dimension;

	// 1. Close Control(Widget)
	SetVisibilityStageNumberWidget(false);
	SetVisibilityTagWidget(false);
	SetVisibilityGoldWidget(false);

	// 3. Remove/Delete Doc Delegate
	UB2UIDocBattle* DocBattle = Cast<UB2UIDocBattle>(GetDoc());
	if (DocBattle)
	{
		DocBattle->SetIsTagPossible(false);
		DocBattle->SetIsTagAttackSuggested(false);
		DocBattle->SetIsTagSuggested(false);

		DocBattle->OnTagPCHealthChanged.RemoveAll(this);
		DocBattle->OnTagPCMaxHealthChanged.RemoveAll(this);
		DocBattle->OnIsCountingTagCooltimeChanged.RemoveAll(this);
		DocBattle->OnIsTagPossibleChanged.RemoveAll(this);
		DocBattle->OnIsTagDisabledByCooltimeChanged.RemoveAll(this);
		DocBattle->OnRemainingTagCooltimeChanged.RemoveAll(this);
		DocBattle->OnMaxCounterAttackPointChanged.RemoveAll(this);
		DocBattle->OnCounterAttackPointChanged.RemoveAll(this);
	}
}

void UB2UIDimensionBattle::UpdateStaticText()
{
	Super::UpdateStaticText();
	
	if (TB_DimensionStep.IsValid())
	{
		auto DocDimension = UB2UIDocHelper::GetDocDimension();
		if (DocDimension)
		{
			if (TB_DimensionStep.IsValid())
			{
				TB_DimensionStep->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("counterDungeon_Step")), FText::AsNumber(DocDimension->GetDifficulty())));
			}
		}
	}
}

UB2UIInGameBuffIcon_DimensionTower* UB2UIDimensionBattle::CreateOrUpdateDimensionBuffIcon(EResurrectBuffType InBuffType)
{
	// 차원의 균열 성소에서 획득하는 버프 이미지는 부활버프 이미지
	// 하지만 중복 적용이 가능...
	UB2UIInGameBuffIcon_DimensionTower* CreatedOrFound = NULL;

	for (UB2UIInGameBuffIcon* ThisBuffIcon : AllMyBuffIcons_Dimension)
	{
		UB2UIInGameBuffIcon_DimensionTower* CastedResurrectBuffIcon = Cast<UB2UIInGameBuffIcon_DimensionTower>(ThisBuffIcon);

		if (CastedResurrectBuffIcon && CastedResurrectBuffIcon->GetBuffType() == InBuffType)
		{
			CreatedOrFound = CastedResurrectBuffIcon;
			break;
		}
	}

	if (!CreatedOrFound)
		CreatedOrFound = Cast<UB2UIInGameBuffIcon_DimensionTower>(CreateBuffIcon(BuffIconClass_Dimension, HB_Resurrect.Get()));

	if (CreatedOrFound)
	{
		CreatedOrFound->SetBuffType(InBuffType);
		return CreatedOrFound;
	}
	return NULL;
}

UB2UIInGameBuffIcon* UB2UIDimensionBattle::CreateBuffIcon(TSubclassOf<UB2UIInGameBuffIcon> InCreateClass, UHorizontalBox* InToCreatePanel)
{
	if (InCreateClass)
	{
		const bool bIsForSoRSet = (InCreateClass == BuffIconClass_Stage || InCreateClass == BuffIconClass_Resurrect); // For Stage or Resurrect buff.
		UB2UIInGameBuffIcon* CreateClassDefObj = Cast<UB2UIInGameBuffIcon>(InCreateClass->GetDefaultObject());
		UB2UIInGameBuffIcon* CreatedWidget = Cast<UB2UIInGameBuffIcon>(
			DynCreateInHorizontalBox(InCreateClass, this, InToCreatePanel, BuffIconSize, FMargin())
			);

		if (CreatedWidget)
		{
			CreatedWidget->Init();
			AllMyBuffIcons_Dimension.Add(CreatedWidget);
		}

		return CreatedWidget;
	}
	return NULL;
}