// Fill out your copyright notice in the Description page of Project Settings.
#include "B2WingUIP_Enhancement.h"
//#include "BladeII.h"

#include "B2Airport.h"
#include "B2WingInfo.h"
#include "B2LobbyGameMode.h"
#include "B2LobbyUISwitcher.h"
#include "B2ItemInfo.h"
#include "B2LobbyInventory.h"
#include "Event.h"
#include "B2ClientDataStore.h"


#include "BladeIIGameImpl.h"

UB2WingUIP_Enhancement::UB2WingUIP_Enhancement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LevelProgElemPrefix = TEXT("LevelProgElem");
	ProgressEffectFlashDuration = 1.0f;
}

void UB2WingUIP_Enhancement::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_EnhancementTitle);

	GET_SLOT(UTextBlock, TB_EnhanceLevelLabel);

	GET_SLOT(UImage, IMG_EnhanceIngred);
	GET_SLOT(UTextBlock, TB_CurrentIngredNum);
	GET_SLOT(UTextBlock, TB_RequiredIngredNum);

	GET_SLOT(UProgressBar, PB_EnhancePoint);
	GET_SLOT(UTextBlock, TB_EnhancePointProgress);

	GET_SLOT(UB2RichTextBlock, TB_EnhanceNote);
	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_EnhanceGoodInfo);

	LevelProgElements.Empty();
	// MIN_WING_ENHANCE_LEVEL 은 아무 강화가 안된 0 이라 1부터..
	for (int32 SI = MIN_WING_ENHANCE_LEVEL + 1; SI <= MAX_WING_ENHANCE_LEVEL; ++SI)
	{ // 순서대로 찾아서 넣는다.
		const FString& ThisLevelImageName = FString::Printf(TEXT("%s_%d"), *LevelProgElemPrefix, SI);
		UB2WingUIP_EnhLevelProgElem* ThisFoundElem = GetSlot<UB2WingUIP_EnhLevelProgElem>(FName(*ThisLevelImageName));
		if (ThisFoundElem)
		{
			ThisFoundElem->SetMyEnhanceLevel(SI);
			LevelProgElements.Add(ThisFoundElem);
		}
	}

	GET_SLOT(UCanvasPanel, X_CP_ProgressEffectPanel);
	GET_SLOT(UImage, IMG_ProgressEffect);
}

void UB2WingUIP_Enhancement::InitFromWingMain()
{
	UpdateStaticWidgets();

	if (IMG_ProgressEffect.IsValid())
	{
		IMG_ProgressEffect->SetVisibility(ESlateVisibility::Hidden); // 처음에는 숨겨놓는다.
	}
}

void UB2WingUIP_Enhancement::PlayLevelupAnim()
{
	// LastCachedWingData 와 LevelProgElements 들이 새로운 강화 레벨에 맞게 업데이트 된 (UpdateByWingData) 후에 불리워야 될 것.

	for (int32 FII = 0; FII < LevelProgElements.Num(); ++FII)
	{
		UB2WingUIP_EnhLevelProgElem* ThisProgElem = LevelProgElements[FII];
		if (ThisProgElem && ThisProgElem->GetMyEnhanceLevel() == LastCachedWingData.EnhanceLevel)
		{
			ThisProgElem->PlayLevelupAnim();
		}
	}
}

void UB2WingUIP_Enhancement::UpdateByWingData(const FB2Wing& InWingData)
{
	//LastCachedWingData = InWingData;

	//// 강화레벨 프로그레스
	//for (int32 FII = 0; FII < LevelProgElements.Num(); ++FII)
	//{
	//	UB2WingUIP_EnhLevelProgElem* ThisProgElem = LevelProgElements[FII];
	//	if (ThisProgElem)
	//	{
	//		ThisProgElem->UpdateByWingEnhanceLevel(LastCachedWingData.EnhanceLevel);
	//	}
	//}

	//int32 CurrentEnhanceIngredNum = UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::GetWingEnhanceResourceRefID());
	//int32 RequiredEnhanceIngredNum = 0; // 이건 서버에서 온 마스터 데이터 룩업
	//int32 RequiredGoldCost = 0;
	//int32 RequiredEnhancePoint = 0; 
	//int32 EnhancePointToAcquire = 0; // 당장 여기서 필요한 건 아닌 듯 하지만 단지 데이터 룩업을 위해
	//BladeIIGameImpl::GetClientDataStore().GetWingEnhanceRefData(LastCachedWingData, RequiredEnhanceIngredNum, RequiredGoldCost, EnhancePointToAcquire, RequiredEnhancePoint);
	//if (TB_CurrentIngredNum.IsValid())
	//{
	//	TB_CurrentIngredNum->SetText(FText::AsNumber(CurrentEnhanceIngredNum));
	//	TB_CurrentIngredNum->SetColorAndOpacity( // 컬러값 걍 하드코딩.. 굳이..
	//		(CurrentEnhanceIngredNum >= RequiredEnhanceIngredNum) ? FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f)) : FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f))
	//		);
	//}
	//if (TB_RequiredIngredNum.IsValid())
	//{
	//	TB_RequiredIngredNum->SetText(FText::AsNumber(RequiredEnhanceIngredNum));
	//}

	//if (PB_EnhancePoint.IsValid())
	//{
	//	const float PercentBefore = PB_EnhancePoint->Percent;
	//	PB_EnhancePoint->SetPercent((float)LastCachedWingData.EnhancePoint / (float)RequiredEnhancePoint);

	//	if (PB_EnhancePoint->Percent > PercentBefore){ // 실제로 Progress 상승 시 이펙트 뿌림.
	//		BeginShowProgressEffect(PB_EnhancePoint->Percent);
	//	}
	//}
	//if (TB_EnhancePointProgress.IsValid())
	//{
	//	TB_EnhancePointProgress->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), LastCachedWingData.EnhancePoint, RequiredEnhancePoint)));
	//}
}

void UB2WingUIP_Enhancement::UpdateStaticWidgets()
{
	if (TB_EnhancementTitle.IsValid())
	{
		TB_EnhancementTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_Enhancement")));
	}
	if (TB_EnhanceLevelLabel.IsValid())
	{
		TB_EnhanceLevelLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_EnhanceLevel")));
	}
	if (TB_EnhanceNote.IsValid())
	{
		TB_EnhanceNote->SetText(
			FText::Format(BladeIIGetLOCText(B2LOC_CAT_WINGTEXT, TEXT("Wing_Enhance_Note")), FText::AsNumber(MAX_WING_ENHANCE_LEVEL))
			);
	}

	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	if (ItemInfoTable && IMG_EnhanceIngred.IsValid())
	{
		// 강화재료의 정해진 아이템 ID 가 있음.
		FSingleItemInfoData* EnhanceIngredItemInfoData = ItemInfoTable->GetInfoData(FItemRefIDHelper::GetWingEnhanceResourceRefID());
		if (EnhanceIngredItemInfoData)
		{ // 고정된 이미지 하나긴 하지만 이미지 바뀌면서 누락될 수 있으므로 인포테이블서 가져와서 넣어준다.
			IMG_EnhanceIngred->SetBrushFromMaterial(EnhanceIngredItemInfoData->GetIconMaterial(ItemInfoTable));
		}
	}

	if (BTN_EnhanceGoodInfo.IsValid())
	{
		BTN_EnhanceGoodInfo->SetItemInfo(FItemRefIDHelper::GetWingEnhanceResourceRefID());
	}
}

void UB2WingUIP_Enhancement::BeginShowProgressEffect(float InProgress)
{
	if (X_CP_ProgressEffectPanel.IsValid() && IMG_ProgressEffect.IsValid())
	{
		// 좌표 조절을 위해 둘 다 CanvasPanel 안에 배치되어 있어야 함.
		UCanvasPanelSlot* ProgressEffectPanelSlot = Cast<UCanvasPanelSlot>(X_CP_ProgressEffectPanel->Slot);
		UCanvasPanelSlot* ProgressEffectImageSlot = Cast<UCanvasPanelSlot>(IMG_ProgressEffect->Slot);
		if (ProgressEffectPanelSlot && ProgressEffectImageSlot)
		{
			IMG_ProgressEffect->SetVisibility(ESlateVisibility::HitTestInvisible);

			const float TotalLength = ProgressEffectPanelSlot->GetSize().X; // 이게 올라가 있는 프로그레스 바와 같은 크기여야 함.
			const float ImageHalfWidth = ProgressEffectImageSlot->GetSize().X * 0.5f;

			const float ProgressPosition = TotalLength * InProgress;

			ProgressEffectImageSlot->SetPosition(FVector2D(ProgressPosition - ImageHalfWidth, ProgressEffectImageSlot->GetPosition().Y));
		}
	}

	// 자동 꺼짐 타이머 설치.
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().SetTimer(ProgressEffectFlashTH, this, &UB2WingUIP_Enhancement::FinishShowProgressEffect, ProgressEffectFlashDuration, false);
	}
	
	PlayEnhanceProgressFxAnim(); // IMG_ProgressEffect 자체에 대한 플래쉬 효과 애니메이션.
}

void UB2WingUIP_Enhancement::FinishShowProgressEffect()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(ProgressEffectFlashTH);
	}
	if (IMG_ProgressEffect.IsValid())
	{
		IMG_ProgressEffect->SetVisibility(ESlateVisibility::Hidden);
	}
}
