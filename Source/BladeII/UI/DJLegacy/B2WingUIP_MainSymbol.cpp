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
	{// 鉴辑措肺 茫酒辑 持绰促.
		const FString& ThisPartName = FString::Printf(TEXT("%s_%d"), *UIPPropOptionPrefix, POPI);
		UB2WingUIP_PropOptionIcon* ThisFoundElem = GetSlot<UB2WingUIP_PropOptionIcon>(FName(*ThisPartName));
		if (ThisFoundElem)
		{
			ThisFoundElem->SetUsageType(EWingPropOptionUIPUsage::WPOUU_WingPageMain); // 颇飘肺 猾 父怒 捞扒 喊档肺 技泼 啊瓷窍档废 秦具..
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

	PlayOpeningAnim(); // 敲饭捞 炼扒捞 眠啊瞪 荐 乐澜..
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
	// 咯扁辑 角力肺 绢恫 可记阑 哗棵瘤绰 崔扼龙 荐 乐澜. 俺急秦具 窃.
	const bool bGotValidWingData = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(SelectedPCClass, CurrentWingData);
	UB2ItemMiscInfo* ItemMiscInfo = StaticFindItemMiscInfo(); // 咯扁辑 静绰 霸 粱 乐澜.
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
			UIP_EvolveGradeStar->SetVisibility(ESlateVisibility::HitTestInvisible); // 朝俺啊 绝绰 惑怕俊辑 见败躇促啊 盎脚捞 登绰 吧 荐档 乐栏聪 难 淋.
			UIP_EvolveGradeStar->SetCurrentGrade(CurrentWingData.EvolutionGrade);
		}

		for (int32 POPI = 0; POPI < PropOptionIcons.Num(); ++POPI)
		{ // 牢郸胶肺 嘎苗辑 单捞磐 技泼. UIP 绰 菊 牢郸胶啊 谅螟老 扒单, 角力 单捞磐档 菊何盒 element 啊 刚历 open 瞪 巴栏肺 埃林甫..
			// 拱沸 捞芭 粱 救嘎绰促绊 哪腔磐 气惯窍绊 弊繁 扒 酒聪绊 傲 粱 具府操府秦瘤绰 沥档
			UB2WingUIP_PropOptionIcon* ThisPart = PropOptionIcons[POPI];
			const FWingPropOption* ThisOptionPtr = CurrentWingData.GetPropOptionPtr(POPI);
			if (ThisPart && ThisOptionPtr)
			{
				ThisPart->SetMyOptionInfo(*ThisOptionPtr);
				ThisPart->SetWingPropExtraInfo(SelectedPCClass, POPI);
			}
		}
		// PropOptionIcons 尔 付蛮啊瘤 鉴辑肺.
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
			{ // 柳拳殿鞭俊 蝶弗 咆胶飘 祸惑篮 酒捞袍 喊 殿鞭苞 付蛮啊瘤肺 窃.
				TB_WingTitle->SetColorAndOpacity(ItemMiscInfo->GetItemNameTextColor(CurrentWingData.EvolutionGrade));
			}
		}

		UpdateByCurrentWingData_BP(SelectedPCClass, CurrentWingData.EvolutionGrade, CurrentWingData.EnhanceLevel);
	}
	else
	{
		// 酒流 朝俺啊 绝绰 惑炔捞扼 摹绊 捞巴历巴 见扁芭唱 歹固 沥焊甫 钎矫窍芭唱.
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
		{ // 见扁绰 巴焊促 泪变 惑怕肺 焊咯林磊..
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
	// 俺喊 滚瓢 (搬惫 加己 可记 酒捞能) 窜困肺 掺绊 挠 荐档 乐瘤父 老窜篮 窜鉴窍霸 啊磊..
	this->SetVisibility(bInDisable ? ESlateVisibility::HitTestInvisible : ESlateVisibility::SelfHitTestInvisible);
}

void UB2WingUIP_MainSymbol::UpdateStaticWidgets()
{
	
}
