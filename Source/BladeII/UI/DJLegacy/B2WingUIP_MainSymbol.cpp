// Fill out your copyright notice in the Description page of Project Settings.
#include "B2WingUIP_MainSymbol.h"
//#include "BladeII.h"

#include "B2LobbyGameMode.h"
#include "B2Airport.h"
#include "B2WingInfo.h"
#include "B2ItemInfo.h"
#include "Event.h"


#include "BladeIIGameImpl.h"

UB2WingUIP_MainSymbol::UB2WingUIP_MainSymbol(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CachedAirport = NULL;
	UIPPropOptionPrefix = TEXT("UIP_PropOption");
	ExtraActivatedImagePrefix = TEXT("IMG_Activated");
}

void UB2WingUIP_MainSymbol::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_BigFatIcon);

	GET_SLOT(UB2WingUIP_EvolveGradeStar, UIP_EvolveGradeStar);

	PropOptionIcons.Empty();
	ExtraActivatedImages.Empty();
	for (int32 POPI = 1; POPI <= MAX_WING_PROP_OPTION_NUM; ++POPI)
	{// ������� ã�Ƽ� �ִ´�.
		const FString& ThisPartName = FString::Printf(TEXT("%s_%d"), *UIPPropOptionPrefix, POPI);
		UB2WingUIP_PropOptionIcon* ThisFoundElem = GetSlot<UB2WingUIP_PropOptionIcon>(FName(*ThisPartName));
		if (ThisFoundElem)
		{
			ThisFoundElem->SetUsageType(EWingPropOptionUIPUsage::WPOUU_WingPageMain); // ��Ʈ�� �� ��ŭ �̰� ������ ���� �����ϵ��� �ؾ�..
			PropOptionIcons.Add(ThisFoundElem);
		}

		const FString& ThisExtraImageName = FString::Printf(TEXT("%s_%d"), *ExtraActivatedImagePrefix, POPI);
		UImage* ThisFoundImage = GetSlot<UImage>(FName(*ThisExtraImageName));
		if (ThisFoundImage)
		{
			ExtraActivatedImages.Add(ThisFoundImage);
		}
	}

	GET_SLOT(UTextBlock, TB_WingTitle);

}

void UB2WingUIP_MainSymbol::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	CachedAirport = CachedLobbyGM->GetLobbyAirport();
	check(CachedAirport);

	UpdateStaticWidgets();
	UpdateByCurrentWingData();

	PlayOpeningAnim(); // �÷��� ������ �߰��� �� ����..
}

void UB2WingUIP_MainSymbol::DestroySelf()
{
	Super::DestroySelf();

}

void UB2WingUIP_MainSymbol::SetPropIconUsageType(EWingPropOptionUIPUsage InUsageType)
{
	for (UB2WingUIP_PropOptionIcon* ThisIcon : PropOptionIcons)
	{
		if (ThisIcon)
		{
			ThisIcon->SetUsageType(InUsageType);
		}
	}
}

void UB2WingUIP_MainSymbol::UpdateByCurrentWingData()
{
	EPCClass SelectedPCClass = CachedLobbyGM ? CachedLobbyGM->GetHeroMgmtModePCClass() : EPCClass::EPC_End;
	FB2Wing CurrentWingData;
	// ���⼭ ������ � �ɼ��� �������� �޶��� �� ����. �����ؾ� ��.
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo(); // ���⼭ ���� �� �� ����.
	UB2WingInfo* WingInfo = StaticFindWingInfo();

	if (bGotValidWingData)
	{
		if (IMG_BigFatIcon.IsValid())
		{
			IMG_BigFatIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
			UMaterialInterface* GradeIconMtrl = WingInfo ? WingInfo->GetWingMainIconMtrlForGrade(CurrentWingData.EvolutionGrade) : NULL;
			if (GradeIconMtrl)
			{
				IMG_BigFatIcon->SetBrushFromMaterial(GradeIconMtrl);
			}
		}

		if (UIP_EvolveGradeStar.IsValid())
		{
			UIP_EvolveGradeStar->SetVisibility(ESlateVisibility::HitTestInvisible); // ������ ���� ���¿��� ���ܳ��ٰ� ������ �Ǵ� �� ���� ������ �� ��.
			UIP_EvolveGradeStar->SetCurrentGrade(CurrentWingData.EvolutionGrade);
		}

		for (int32 POPI = 0; POPI < PropOptionIcons.Num(); ++POPI)
		{ // �ε����� ���缭 ������ ����. UIP �� �� �ε����� ������ �ǵ�, ���� �����͵� �պκ� element �� ���� open �� ������ ���ָ�..
			// ���� �̰� �� �ȸ´´ٰ� ��ǻ�� �����ϰ� �׷� �� �ƴϰ� �� �� �߸��ٸ������� ����
			UB2WingUIP_PropOptionIcon* ThisPart = PropOptionIcons[POPI];
			const FWingPropOption* ThisOptionPtr = CurrentWingData.GetPropOptionPtr(POPI);
			if (ThisPart && ThisOptionPtr)
			{
				ThisPart->SetMyOptionInfo(*ThisOptionPtr);
				ThisPart->SetWingPropExtraInfo(SelectedPCClass, POPI);
			}
		}
		// PropOptionIcons �� �������� ������.
		for (int32 POPI = 0; POPI < ExtraActivatedImages.Num(); ++POPI)
		{ 
			UImage* ThisExtraImage = ExtraActivatedImages[POPI];
			const FWingPropOption* ThisOptionPtr = CurrentWingData.GetPropOptionPtr(POPI);
			if (ThisExtraImage && ThisOptionPtr)
			{
				ThisExtraImage->SetVisibility(ThisOptionPtr->bIsOpen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
			}
		}
		
		if (TB_WingTitle.IsValid())
		{
			TB_WingTitle->SetText(GetLOCTextOfWingName(SelectedPCClass, CurrentWingData.EvolutionGrade));
			if (ItemMiscInfo)
			{ // ��ȭ��޿� ���� �ؽ�Ʈ ������ ������ �� ��ް� ���������� ��.
				TB_WingTitle->SetColorAndOpacity(ItemMiscInfo->GetItemNameTextColor(CurrentWingData.EvolutionGrade));
			}
		}

		UpdateByCurrentWingData_BP(SelectedPCClass, CurrentWingData.EvolutionGrade, CurrentWingData.EnhanceLevel);
	}
	else
	{
		// ���� ������ ���� ��Ȳ�̶� ġ�� �̰����� ����ų� ���� ������ ǥ���ϰų�.
		if (IMG_BigFatIcon.IsValid()){
			IMG_BigFatIcon->SetVisibility(ESlateVisibility::Hidden);
		}
		if (TB_WingTitle.IsValid()){
			TB_WingTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_WingButton")));
		}
		if (UIP_EvolveGradeStar.IsValid()){
			UIP_EvolveGradeStar->SetVisibility(ESlateVisibility::Collapsed);
		}
		FWingPropOption DummyLockedOptionInfo;
		DummyLockedOptionInfo.bIsOpen = false;
		for (int32 POPI = 0; POPI < PropOptionIcons.Num(); ++POPI)
		{ // ����� �ͺ��� ��� ���·� ��������..
			UB2WingUIP_PropOptionIcon* ThisPart = PropOptionIcons[POPI];
			if (ThisPart){
				ThisPart->SetMyOptionInfo(DummyLockedOptionInfo);
				ThisPart->SetWingPropExtraInfo(SelectedPCClass, POPI);
			}
		}
	}
}

void UB2WingUIP_MainSymbol::SetExtraDisableButtons(bool bInDisable)
{
	// ���� ��ư (�ᱹ �Ӽ� �ɼ� ������) ������ ���� �� ���� ������ �ϴ��� �ܼ��ϰ� ����..
	this->SetVisibility(bInDisable ? ESlateVisibility::HitTestInvisible : ESlateVisibility::SelfHitTestInvisible);
}

void UB2WingUIP_MainSymbol::UpdateStaticWidgets()
{
	
}
