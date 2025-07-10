// Fill out your copyright notice in the Description page of Project Settings.


#include "B2HighlightWidget.h"
#include "STickImage.h"
#include "B2UIManager.h"
#include "B2UITutorial.h"
#include "TutorialManager.h"


namespace HighlightColor
{
	const FLinearColor EditorColor(1.f, 0.f, 0.f, 0.5f);
	const FLinearColor InvinsibleColor(0.f, 0.f, 0.f, 0.f);
}

UB2HighlightWidget::UB2HighlightWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//Visibility = ESlateVisibility::Hidden;
	//ColorAndOpacity = HighlightColor::EditorColor;

	//CachedGeometry = FGeometry();
	//bActivate = false;

	//CachedTutorialUI = nullptr;

	//ActiveTutorialID = INDEX_NONE;
	//ActiveTutorialStep = INDEX_NONE;
}

TSharedRef<SWidget> UB2HighlightWidget::RebuildWidget()
{
	MyImage = SNew(STickImage);
	if (STickImage* TickImage = static_cast<STickImage*>(MyImage.Get()))
	{
		TickImage->SetOwnerWidget(this);
	}

	return MyImage.ToSharedRef();
}

void UB2HighlightWidget::OnSlatePaint(const FGeometry& Geometry)
{
	if (TutorialManager::GetInstance().HasAnyTutorial() == false)
		return;

	TickTutorialCheck();

	if (IsActivate() && CachedTutorialUI.IsValid() && IsGeometryChanged(CachedGeometry, Geometry))
	{
		if (bDialogWidget)
			CachedTutorialUI->TickHighlightDialog(ConvertPosition(Geometry));

		else
			CachedTutorialUI->TickHighlight(ConvertPosition(Geometry), ConvertSize(Geometry));
		
		CachedGeometry = Geometry;
	}
}


void UB2HighlightWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	//const FName& MyName = GetFName();
	//ExtractTutorialInfo(MyName.ToString(), ActiveTutorialID, ActiveTutorialStep, bDialogWidget);

	//if (GIsEditor)
	//	ColorAndOpacity = HighlightColor::EditorColor;
	//else
	//	ColorAndOpacity = HighlightColor::InvinsibleColor;
}

void UB2HighlightWidget::ExtractTutorialInfo(const FString& WidgetName, int32& TutorialID, int32& TutorialStep, bool& bDialog)
{
	static const FString Prefix = TEXT("TutorialStep");
	static const int32 PrefixLength = Prefix.Len();

	bool bValidPrefix = WidgetName.Find(Prefix) != INDEX_NONE ? true : false;
	if (bValidPrefix == false)
		return;

	static const TCHAR* DivideChar = TEXT("_");

	const int32 FirstDividePos = WidgetName.Find(DivideChar, ESearchCase::IgnoreCase, ESearchDir::FromStart);
	if (FirstDividePos != INDEX_NONE)
	{
		FString StringID = WidgetName.Mid(PrefixLength, FirstDividePos - PrefixLength);
		TutorialID = FCString::Atoi(*StringID);

		const int32 StepBeginPos = FirstDividePos + 1;
		int32 StepStrCount = WidgetName.Len() - (StepBeginPos);

		const int32 SecondDividePos = WidgetName.Find(DivideChar, ESearchCase::IgnoreCase, ESearchDir::FromStart, StepBeginPos);
		if (SecondDividePos != INDEX_NONE)
		{
			// _dialog 형식의 Highlight
			StepStrCount = SecondDividePos - StepBeginPos;

			bDialog = true;
		}

		FString StringStep = WidgetName.Mid(StepBeginPos, StepStrCount);
		TutorialStep = FCString::Atoi(*StringStep);
	}

	if (TutorialManager::GetInstance().IsTutorialAvailable())
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2HighlightWidget::SetActivate(bool IsActivate)
{
	bActivate = IsActivate;

	if (bActivate)
	{
		if(CachedTutorialUI == nullptr)
			CachedTutorialUI = UB2UIManager::GetInstance()->GetUI<UB2UITutorial>(UIFName::Tutorial);

		if(GIsEditor == false)
			SetColorAndOpacity(HighlightColor::InvinsibleColor);

		SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (ActiveTutorialID == INDEX_NONE)
			ActiveTutorialID = TutorialManager::GetInstance().GetCurrentTutorialID();

		if (ActiveTutorialStep == INDEX_NONE)
			ActiveTutorialStep = TutorialManager::GetInstance().GetCurrentTutorialStep();
	}

	else
	{
		CachedTutorialUI = nullptr;

		SetVisibility(ESlateVisibility::Collapsed);
		CachedGeometry = FGeometry();
	}
}

bool UB2HighlightWidget::IsGeometryChanged(const FGeometry& PrevDraw, const FGeometry& CurrDraw)
{
	const bool IsPositionChanged = !(PrevDraw.AbsolutePosition.Equals(CurrDraw.AbsolutePosition, 1.f));

	if (bDialogWidget)
		return IsPositionChanged;

	const bool IsSizeChanged = !(PrevDraw.GetAbsoluteSize().Equals(CurrDraw.GetAbsoluteSize(), 1.f));

	return IsPositionChanged || IsSizeChanged;
}

void UB2HighlightWidget::TickTutorialCheck()
{
	TutorialManager& TutorialMgr = TutorialManager::GetInstance();

	int32 iCurrentTutorialID = TutorialMgr.GetCurrentTutorialID();

	if (TutorialMgr.IsTutorialAvailable() == false ||
		iCurrentTutorialID > ActiveTutorialID ||
		TutorialMgr.HighlightRenderCheck(ActiveTutorialID) == false)
	{
		bActivate = false;
		//SetActivate(false);
		return;
	}

	if (iCurrentTutorialID < ActiveTutorialID)
	{
		bActivate = false;
		return;
	}

	if (CachedTutorialUI == nullptr)
		CachedTutorialUI = UB2UIManager::GetInstance()->GetUI<UB2UITutorial>(UIFName::Tutorial);

	if (!CachedTutorialUI.IsValid())
		return;

	int32 iCachedTutorialUITutorialID = CachedTutorialUI->GetUIWidgetTuTorialID();
	int32 iCachedTutorialUITutorialStep = CachedTutorialUI->GetUIWidgetTuTorialStep();
	
	if (iCachedTutorialUITutorialID == ActiveTutorialID)
	{
		if (iCachedTutorialUITutorialStep > ActiveTutorialStep)
		{
			bActivate = false;
			//SetActivate(false);
		}
		else if (iCachedTutorialUITutorialStep == ActiveTutorialStep)
		{
			bActivate = true;

			if (bDialogWidget)
			{
				if (CachedTutorialUI->GetHighlightDialogRedrawCheck())
				{
					CachedTutorialUI->SetHighlightDialogRedrawCheck(false);
					CachedGeometry = FGeometry();
				}
			}
			else
			{
				if (CachedTutorialUI->GetHighlightRedrawCheck())
				{
					CachedTutorialUI->SetHighlightRedrawCheck(false);
					CachedGeometry = FGeometry();
				}
			}
		}
	}

}

FVector2D UB2HighlightWidget::ConvertPosition(const FGeometry& Geometry)
{
	FPaintGeometry RenderInfo = Geometry.ToPaintGeometry();

	float fConvertScale = 1.f;

	if (CachedTutorialUI.IsValid())
	{
		FPaintGeometry fDPIScaleGeometry = CachedTutorialUI->GetCachedGeometry().ToPaintGeometry();

		fConvertScale /= fDPIScaleGeometry.DrawScale;
	}

	FVector2D fv2ConvertPosition;// = RenderInfo.DrawPosition;

#if WITH_EDITOR	//새 에디터 창에서 실행시 타이틀바까지 포함하여 윈도우 좌표 계산해주는것을 뺌

	if (CachedTutorialUI.IsValid())
	{
		if (auto* ViewportClient = CachedTutorialUI->GetWorld()->GetGameViewport())
		{
			TSharedPtr<SWindow> Window = ViewportClient->GetWindow();

			if (Window.IsValid())
			{
				FOptionalSize fSize = Window->GetTitleBarSize();

				if (fSize.Get() > 0.f)	//bCreateTitleBar 체크
				{
					const FVector2D fWindowBezel = FVector2D(5, 30);
					fv2ConvertPosition -= fWindowBezel;
				}
			}
		}
	}

#endif

	//fv2ConvertPosition *= fConvertScale;

	return fv2ConvertPosition;
}

FVector2D UB2HighlightWidget::ConvertSize(const FGeometry& Geometry)
{
	FPaintGeometry RenderInfo = Geometry.ToPaintGeometry();

	float fConvertScale = 1.f;

	if (CachedTutorialUI.IsValid())
	{
		FPaintGeometry fDPIScaleGeometry = CachedTutorialUI->GetCachedGeometry().ToPaintGeometry();

		fConvertScale /= fDPIScaleGeometry.DrawScale;
	}

	FVector2D fv2ConvertSize = Geometry.GetAbsoluteSize();

	fv2ConvertSize *= fConvertScale;

	return fv2ConvertSize;
}