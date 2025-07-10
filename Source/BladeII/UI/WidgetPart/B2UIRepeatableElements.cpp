#include "B2UIRepeatableElements.h"
#include "B2UIManager.h"

UB2UIRepeatableElements::UB2UIRepeatableElements(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bHideEmptyImage = false;
	MaxNum = 6;
	CurNum = 1;
	PrefixEmptyAsset = TEXT("IMG_Filled");
	PrefixFilledAsset = TEXT("IMG_Empty");
	FilledWidgets.Empty();
	EmptyWidgets.Empty();
}

void UB2UIRepeatableElements::CacheAssets()
{
	GET_SLOT(UPanelWidget, CON_Elements)

	FilledWidgets.Empty();
	//FilledWidgets
	for (uint8 Cnt = 0; Cnt < MaxNum; ++Cnt)
	{
		FString FilledAssetName = PrefixFilledAsset + FString::FromInt(Cnt);
		auto FilledAsset = GetSlot<UWidget>(FName(*FilledAssetName));
		if (FilledAsset)
		{
			FilledWidgets.Add(FilledAsset);
		}
	}

	EmptyWidgets.Empty();
	//EmptyAssets
	for (uint8 Cnt = 0; Cnt < MaxNum; ++Cnt)
	{
		FString EmptyAssetName = PrefixEmptyAsset + FString::FromInt(Cnt);
		auto EmptyAsset = GetSlot<UWidget>(FName(*EmptyAssetName));
		if (EmptyAsset)
		{
			EmptyWidgets.Add(EmptyAsset);
		}
	}
}

void UB2UIRepeatableElements::SetMaxElementNum(int32 Num)
{
	MaxNum = Num;
	//Empty이미지와 Filled이미지를 포함하고 있는 ChildPanel을 제어한다.
	if (CON_Elements.IsValid())
	{
		for (uint8 Cnt = 0; Cnt < CON_Elements->GetChildrenCount(); Cnt++)
		{
			auto ThisSlot = CON_Elements->GetChildAt(Cnt);
			if (ThisSlot)
			{
				if (Cnt < Num)
					ThisSlot->SetVisibility(ESlateVisibility::HitTestInvisible);
				else
					ThisSlot->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
	ShowEmptyWidgets(!bHideEmptyImage);
}

void UB2UIRepeatableElements::SetCurElementNum(int32 Num)
{
	CurNum = Num;
	
	ShowFilledWidgets(Num);
}

void UB2UIRepeatableElements::ShowFilledWidgets(int32 InValue)
{
	for (uint8 Cnt = 0; Cnt < FilledWidgets.Num(); ++Cnt)
	{
		UWidget* Asset = FilledWidgets[Cnt];
		if (Asset)
		{
			if (Cnt < InValue)
				Asset->SetVisibility(ESlateVisibility::HitTestInvisible);
			else
				Asset->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UB2UIRepeatableElements::ShowEmptyWidgets(bool bShow)
{
	for (uint8 Cnt = 0; Cnt < EmptyWidgets.Num(); ++Cnt)
	{
		UWidget* Asset = EmptyWidgets[Cnt];
		if (Asset)
		{
			bShow ? Asset->SetVisibility(ESlateVisibility::HitTestInvisible) : Asset->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UB2UIRepeatableElements::SynchronizeProperties()
{
	Super::SynchronizeProperties();

//에디터에서 작업중인데, PIE가 아니면
#if WITH_EDITOR
	UWorld* World = GetWorld();
	UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
	if (GameInstance || (World && World->IsGameWorld()))
		return;

	if (!CON_Elements.IsValid())
		CacheAssets();

	SetMaxElementNum(MaxNum);
	SetCurElementNum(CurNum);
#endif	// WITH_EDITOR
}
