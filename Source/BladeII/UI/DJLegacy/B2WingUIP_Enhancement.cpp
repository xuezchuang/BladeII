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
	// MIN_WING_ENHANCE_LEVEL �� �ƹ� ��ȭ�� �ȵ� 0 �̶� 1����..
	for (int32 SI = MIN_WING_ENHANCE_LEVEL + 1; SI <= MAX_WING_ENHANCE_LEVEL; ++SI)
	{ // ������� ã�Ƽ� �ִ´�.
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
		IMG_ProgressEffect->SetVisibility(ESlateVisibility::Hidden); // ó������ ���ܳ��´�.
	}
}

void UB2WingUIP_Enhancement::PlayLevelupAnim()
{
	// LastCachedWingData �� LevelProgElements ���� ���ο� ��ȭ ������ �°� ������Ʈ �� (UpdateByWingData) �Ŀ� �Ҹ����� �� ��.

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

	//// ��ȭ���� ���α׷���
	//for (int32 FII = 0; FII < LevelProgElements.Num(); ++FII)
	//{
	//	UB2WingUIP_EnhLevelProgElem* ThisProgElem = LevelProgElements[FII];
	//	if (ThisProgElem)
	//	{
	//		ThisProgElem->UpdateByWingEnhanceLevel(LastCachedWingData.EnhanceLevel);
	//	}
	//}

	//int32 CurrentEnhanceIngredNum = UB2LobbyInventory::GetSharedConsumableAmountOfType(FItemRefIDHelper::GetWingEnhanceResourceRefID());
	//int32 RequiredEnhanceIngredNum = 0; // �̰� �������� �� ������ ������ ���
	//int32 RequiredGoldCost = 0;
	//int32 RequiredEnhancePoint = 0; 
	//int32 EnhancePointToAcquire = 0; // ���� ���⼭ �ʿ��� �� �ƴ� �� ������ ���� ������ ����� ����
	//BladeIIGameImpl::GetClientDataStore().GetWingEnhanceRefData(LastCachedWingData, RequiredEnhanceIngredNum, RequiredGoldCost, EnhancePointToAcquire, RequiredEnhancePoint);
	//if (TB_CurrentIngredNum.IsValid())
	//{
	//	TB_CurrentIngredNum->SetText(FText::AsNumber(CurrentEnhanceIngredNum));
	//	TB_CurrentIngredNum->SetColorAndOpacity( // �÷��� �� �ϵ��ڵ�.. ����..
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

	//	if (PB_EnhancePoint->Percent > PercentBefore){ // ������ Progress ��� �� ����Ʈ �Ѹ�.
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
		// ��ȭ����� ������ ������ ID �� ����.
		FSingleItemInfoData* EnhanceIngredItemInfoData = ItemInfoTable->GetInfoData(FItemRefIDHelper::GetWingEnhanceResourceRefID());
		if (EnhanceIngredItemInfoData)
		{ // ������ �̹��� �ϳ��� ������ �̹��� �ٲ�鼭 ������ �� �����Ƿ� �������̺� �����ͼ� �־��ش�.
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
		// ��ǥ ������ ���� �� �� CanvasPanel �ȿ� ��ġ�Ǿ� �־�� ��.
		UCanvasPanelSlot* ProgressEffectPanelSlot = Cast<UCanvasPanelSlot>(X_CP_ProgressEffectPanel->Slot);
		UCanvasPanelSlot* ProgressEffectImageSlot = Cast<UCanvasPanelSlot>(IMG_ProgressEffect->Slot);
		if (ProgressEffectPanelSlot && ProgressEffectImageSlot)
		{
			IMG_ProgressEffect->SetVisibility(ESlateVisibility::HitTestInvisible);

			const float TotalLength = ProgressEffectPanelSlot->GetSize().X; // �̰� �ö� �ִ� ���α׷��� �ٿ� ���� ũ�⿩�� ��.
			const float ImageHalfWidth = ProgressEffectImageSlot->GetSize().X * 0.5f;

			const float ProgressPosition = TotalLength * InProgress;

			ProgressEffectImageSlot->SetPosition(FVector2D(ProgressPosition - ImageHalfWidth, ProgressEffectImageSlot->GetPosition().Y));
		}
	}

	// �ڵ� ���� Ÿ�̸� ��ġ.
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().SetTimer(ProgressEffectFlashTH, this, &UB2WingUIP_Enhancement::FinishShowProgressEffect, ProgressEffectFlashDuration, false);
	}
	
	PlayEnhanceProgressFxAnim(); // IMG_ProgressEffect ��ü�� ���� �÷��� ȿ�� �ִϸ��̼�.
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
