// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIHeroTowerBattleSub.h"
#include "B2HeroTowerGameMode.h"
#include "B2UIRewardEventSet.h"


#include "BladeIIGameImpl.h"

void UB2UIHeroTowerBattleSub::Init()
{
	Super::Init();
}

void UB2UIHeroTowerBattleSub::NativeConstruct()
{
	Super::NativeConstruct();

	int32 nFloor = 0;

	if(TB_Floor.IsValid())
	{
		AB2HeroTowerGameMode* pGM = Cast<AB2HeroTowerGameMode>(UGameplayStatics::GetGameMode(this));

		if (pGM)
		{
			nFloor = pGM->GetHeroTowerFloor();
			TB_Floor->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTower_NFloor")), FText::AsNumber(nFloor)));
		}		
	}

	if (HB_Rewards.IsValid())
	{
		//지우고
		TArray<UPanelSlot*> Slots = HB_Rewards->GetSlots();

		for (UPanelSlot* pSlot : Slots)
		{
			if (pSlot)
				HB_Rewards->RemoveChild(pSlot->Content);
		}

		//만들고
		TArray<FRewardItem>* pArray = BladeIIGameImpl::GetClientDataStore().GetHeroTowerRewards(nFloor);
		for (FRewardItem& RewardItem : *pArray)
		{
			if (RewardItem.Id != 0)
			{
				UB2UIRewardIcon* pUIP = CreateWidget<UB2UIRewardIcon>(GetOwningPlayer(), RewardItemClass);
				HB_Rewards->AddChild(pUIP);
				pUIP->Init();
				pUIP->UpdateRewardIconAndName(RewardItem.Id);
			}
		}
	}
}

void UB2UIHeroTowerBattleSub::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (TB_ElapsedTime.IsValid())
	{	
		AB2HeroTowerGameMode* pGM = Cast<AB2HeroTowerGameMode>(UGameplayStatics::GetGameMode(this));

		if (pGM)
		{
			FTimespan timeElapsed = pGM->GetElapsedTime();

			TB_ElapsedTime->SetText(FText::FromString(timeElapsed.ToString(TEXT("%m:%s.%f"))));
		}
	}
}

void UB2UIHeroTowerBattleSub::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UTextBlock, TB_Floor);
	GET_SLOT(UTextBlock, TB_ElapsedTime);
	GET_SLOT(UHorizontalBox, HB_Rewards);
}
