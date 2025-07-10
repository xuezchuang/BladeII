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
	{// 순서대로 찾아서 넣는다.
		const FString& ThisPartName = FString::Printf(TEXT("%s_%d"), *UIPPropOptionPrefix, POPI);
		UB2WingUIP_PropOptionIcon* ThisFoundElem = GetSlot<UB2WingUIP_PropOptionIcon>(FName(*ThisPartName));
		if (ThisFoundElem)
		{
			ThisFoundElem->SetUsageType(EWingPropOptionUIPUsage::WPOUU_WingPageMain); // 파트로 뺀 만큼 이건 별도로 세팅 가능하도록 해야..
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

	PlayOpeningAnim(); // 플레이 조건이 추가될 수 있음..
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
	// 여기서 실제로 어떤 옵션을 빼올지는 달라질 수 있음. 개선해야 함.
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo(); // 여기서 쓰는 게 좀 있음.
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
			UIP_EvolveGradeStar->SetVisibility(ESlateVisibility::HitTestInvisible); // 날개가 없는 상태에서 숨겨놨다가 갱신이 되는 걸 수도 있으니 켜 줌.
			UIP_EvolveGradeStar->SetCurrentGrade(CurrentWingData.EvolutionGrade);
		}

		for (int32 POPI = 0; POPI < PropOptionIcons.Num(); ++POPI)
		{ // 인덱스로 맞춰서 데이터 세팅. UIP 는 앞 인덱스가 좌측일 건데, 실제 데이터도 앞부분 element 가 먼저 open 될 것으로 간주를..
			// 물론 이거 좀 안맞는다고 컴퓨터 폭발하고 그런 건 아니고 걍 좀 야리꾸리해지는 정도
			UB2WingUIP_PropOptionIcon* ThisPart = PropOptionIcons[POPI];
			const FWingPropOption* ThisOptionPtr = CurrentWingData.GetPropOptionPtr(POPI);
			if (ThisPart && ThisOptionPtr)
			{
				ThisPart->SetMyOptionInfo(*ThisOptionPtr);
				ThisPart->SetWingPropExtraInfo(SelectedPCClass, POPI);
			}
		}
		// PropOptionIcons 랑 마찬가지 순서로.
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
			{ // 진화등급에 따른 텍스트 색상은 아이템 별 등급과 마찬가지로 함.
				TB_WingTitle->SetColorAndOpacity(ItemMiscInfo->GetItemNameTextColor(CurrentWingData.EvolutionGrade));
			}
		}

		UpdateByCurrentWingData_BP(SelectedPCClass, CurrentWingData.EvolutionGrade, CurrentWingData.EnhanceLevel);
	}
	else
	{
		// 아직 날개가 없는 상황이라 치고 이것저것 숨기거나 더미 정보를 표시하거나.
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
		{ // 숨기는 것보다 잠긴 상태로 보여주자..
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
	// 개별 버튼 (결국 속성 옵션 아이콘) 단위로 끄고 켤 수도 있지만 일단은 단순하게 가자..
	this->SetVisibility(bInDisable ? ESlateVisibility::HitTestInvisible : ESlateVisibility::SelfHitTestInvisible);
}

void UB2WingUIP_MainSymbol::UpdateStaticWidgets()
{
	
}
