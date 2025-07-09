// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2ButtonGoodInfoToolTip.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattle.h"
#include "B2UIDocHero.h"
#include "B2ItemInfo.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "B2ItemInfo.h"
#include "B2UIGoodsShortcutTooltipPopup.h"
#include "B2UIShortagePopupManager.h"

UB2ButtonGoodInfoToolTip::UB2ButtonGoodInfoToolTip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, GoodsPopupType(EGoodsInfoPopupType::EGoodsPopup_None)
	, m_ItemInfoidx(0)
	, m_InOwnerUnitedWidget(nullptr)
	//,AuraImg(UWidgetBlueprintLibrary::NoResourceBrush())
	//,OutLineImg(UWidgetBlueprintLibrary::NoResourceBrush())
	//,ReflectImg(UWidgetBlueprintLibrary::NoResourceBrush())
{
	DefaultOnClickSoundIndex = -1;
	//이게 있어야 스크롤 가능함
	//ClickMethod = EButtonClickMethod::PreciseClick;
	//TouchMethod = EButtonTouchMethod::PreciseTap;
	//[@AKI, 170417] 위험이 있을 가능성이 있고, 아트쪽에서 사용 안하기로 했으므로 주석 처리
	//SetIcon(WidgetStyle.Normal, WidgetStyle.Normal, WidgetStyle.Normal);
	//SetDrawAs(ESlateBrushDrawType::Image);
}

void UB2ButtonGoodInfoToolTip::SetItemInfo(int32 _ItemInfoidx)
{
	m_ItemInfoidx = _ItemInfoidx;

	//ClickMethod = EButtonClickMethod::PreciseClick;
	//TouchMethod = EButtonTouchMethod::PreciseTap;
//	const FSingleItemInfoData* singleData = StaticFindItemInfo() ? StaticFindItemInfo()->GetInfoData(_ItemInfoidx) : nullptr;

	/*
	auto icon(GetIconMaterialFromItemTable(m_GoodsInfoRef));
	if (nullptr != icon)
	{
		FSlateBrush iconSlate( UWidgetBlueprintLibrary::MakeBrushFromMaterial(icon, WidgetStyle.Normal.ImageSize.X, WidgetStyle.Normal.ImageSize.Y) );
		iconSlate.Tiling = WidgetStyle.Normal.Tiling;
		iconSlate.TintColor = WidgetStyle.Normal.TintColor;
		//[@AKI, 170329] ESlateBrushDrawType::Image으로 하고 있기 때문에 Margin이 무시되어 넘겨줄 필요가 없음. ESlateBrushDrawType::Box로 변경될 경우 Margin값을 넘겨줘야 함
		//iconSlate.Margin = WidgetStyle.Normal.Margin; 

		//[@AKI, 170329] 일단은 Normal꺼를 공통으로 넣고 있으나 아트쪽 얘기를 들어보고
		//Normal, Hovered, Pressed를 분리 하고 싶다고 하면 분리 해야 함.
		//SetIcon(iconSlate, iconSlate, iconSlate);
		//SetDrawAs(ESlateBrushDrawType::Image);
	}
	*/
}
//[@AKI, 170329] MakeBrushFromMaterial을 하면 없어도 엔진의 Default를 주기 때문에 nullptr 체크를 안함
void UB2ButtonGoodInfoToolTip::SetIcon(const FSlateBrush& _NormalSlate, const FSlateBrush& _PressedSlate, const FSlateBrush& _HoveredSlate)
{
	/*/
	//[@AKI, 170329] 혹시 몰라 놔둠. 엔진의 Default이미지를 사용 할 경우 Normal과 같은것을 사용하게 하라고 함.
	FName DefaultResourceName(TEXT("../../../Engine/Content/Slate/Common/Button.png"));

	WidgetStyle.SetNormal(_NormalSlate);
	if (WidgetStyle.Pressed.GetResourceName() == DefaultResourceName)
		WidgetStyle.SetPressed(_PressedSlate);
	if (WidgetStyle.Hovered.GetResourceName() == DefaultResourceName)
		WidgetStyle.SetHovered(_HoveredSlate);
	/*/

	//WidgetStyle.SetNormal(_NormalSlate);
	//WidgetStyle.SetPressed(_PressedSlate);
	//WidgetStyle.SetHovered(_HoveredSlate);
	//*/
}

void UB2ButtonGoodInfoToolTip::SetDrawAs(const ESlateBrushDrawType::Type& _type)
{
	//WidgetStyle.Normal.DrawAs	= _type;
	//WidgetStyle.Pressed.DrawAs	= _type;
	//WidgetStyle.Hovered.DrawAs	= _type;
}

FVector2D UB2ButtonGoodInfoToolTip::GetPopupPosition(UB2UIGoodsShortcutTooltipPopup* const _pUI) const
{
	FVector2D touchPos(UB2UnitedWidgetBase::GetWidgetScreenPos(m_InOwnerUnitedWidget));

	//FVector2D viewportSize(FVector2D::ZeroVector);
	//FVector2D RatioViewportSize(FVector2D::ZeroVector);
	//UGameViewportClient* const ViewportClient = GetWorld()->GetGameViewport();
	//float ratio = 1.0f;
	//if (ViewportClient)
	//{
	//	//ViewportClient->GetMousePosition(touchPos);
	//	ViewportClient->GetViewportSize(viewportSize);
	//	RatioViewportSize = viewportSize;
	//	ratio = viewportSize.X / viewportSize.Y;
	//	if ( FMath::Abs(ratio - 1.7f) > 0.1f) // 16:9 == 1.6666667가 아닐때
	//	{
	//		//16:9로 강제로 변환해 준다.
	//		if(viewportSize.X < viewportSize.Y)
	//		{
	//			ratio = viewportSize.X / viewportSize.Y;
	//			RatioViewportSize.X *= ratio;
	//			touchPos.X *= ratio;
	//		}
	//		else
	//		{
	//			ratio = viewportSize.Y / viewportSize.X;
	//			RatioViewportSize.Y *= ratio;
	//			touchPos.Y *= ratio;
	//		}
	//	}
	//	else //16:9일때는 ratio는 1.0 임
	//		ratio = 1.0f;
	//}
	//const FVector2D viewRatio(RatioViewportSize / UB2UnitedWidgetBase::DesignedRefViewSize);

	//const FVector2D halfUISize(_pUI->GetUISize() * 0.5f * viewRatio);
	//const FVector2D halfRatioViewportSize(RatioViewportSize * _pUI->GetSlotAnchors());
	//const FVector2D uiSize(_pUI->GetUISize() * viewRatio);
	//const FVector2D checkUISize(touchPos.X + uiSize.X + this->WidgetStyle.Normal.ImageSize.X, touchPos.Y - uiSize.Y - this->WidgetStyle.Normal.ImageSize.Y);
	////UI도 가운데에 앵커포인트가 있으므로 가운데로 보내기 위해서 정리
	//FVector2D uiPos(touchPos.X + halfUISize.X - halfRatioViewportSize.X + this->WidgetStyle.Normal.ImageSize.X * 2, touchPos.Y - halfUISize.Y - halfRatioViewportSize.Y); //가운데가 0,0이므로
	//if (checkUISize.X > viewportSize.X)
	//{
	//	uiPos.X = viewportSize.X * _pUI->GetSlotAnchors().X - halfUISize.X;
	//}
	//if (checkUISize.Y < 0)
	//{
	//	uiPos.Y = -viewportSize.Y * _pUI->GetSlotAnchors().Y + uiSize.Y;
	//}
//	if (FMath::Abs(ratio - 1.0f) > FLT_EPSILON) // 16:9가 아닌 해상도에서는 UISize만큼 내려가는 문제가 있어 보간함.
//		uiPos.Y -= uiSize.Y;

	return touchPos;
}

UMaterialInterface* UB2ButtonGoodInfoToolTip::GetIconMaterialFromItemTable(int32 RewardID) const
{
	return StaticFindItemInfo() ? StaticFindItemInfo()->GetItemIcon(RewardID) : nullptr;
}

void UB2ButtonGoodInfoToolTip::RegisterExtraStuff(UB2UnitedWidgetBase* InOwnerUnitedWidget)
{
	OnClicked.AddUniqueDynamic(this, &UB2ButtonGoodInfoToolTip::ExtraOnClick);
	OnPressed.AddUniqueDynamic(this, &UB2ButtonGoodInfoToolTip::ExtraOnPress);
	OnReleased.AddUniqueDynamic(this, &UB2ButtonGoodInfoToolTip::ExtraOnRelease);
	OnHovered.AddUniqueDynamic(this, &UB2ButtonGoodInfoToolTip::ExtraOnHovered);

	//OnTouchImpulseRingInst.Init(InOwnerUnitedWidget, OnTouchImpulseRingWidgetName, OnTouchImpulseRingAnimTime, OnTouchImpulseRingParamRange_Begin, OnTouchImpulseRingParamRange_End);

	m_InOwnerUnitedWidget = InOwnerUnitedWidget;
	/*
	UB2UnitedWidgetBase* pParent = InOwnerUnitedWidget;
	while (pParent)
	{
		if (pParent->WidgetGeneratedByClass)
		{
			if (pParent->WidgetGeneratedByClass->ClassGeneratedBy)
				pParent = (UB2UnitedWidgetBase*)pParent->WidgetGeneratedByClass->ClassGeneratedBy;
		}
	}
	*/
	GoodsPopupType	= m_InOwnerUnitedWidget->GetGoodsPopupType();
}

void UB2ButtonGoodInfoToolTip::UnregisterExtraStuff(UB2UnitedWidgetBase* InOwnerUnitedWidget)
{
	OnClicked.RemoveDynamic(this, &UB2ButtonGoodInfoToolTip::ExtraOnClick);
	OnPressed.RemoveDynamic(this, &UB2ButtonGoodInfoToolTip::ExtraOnPress);
	OnReleased.RemoveDynamic(this, &UB2ButtonGoodInfoToolTip::ExtraOnRelease);
	OnHovered.RemoveDynamic(this, &UB2ButtonGoodInfoToolTip::ExtraOnHovered);
	OnTouchImpulseRingInst.Clear();
}

void UB2ButtonGoodInfoToolTip::ExtraOnClick()
{
	Super::ExtraOnClick();

	UB2UIManager* const UIMgrInst = UB2UIManager::GetInstance();
	if (!UIMgrInst)	return;

	//GoodsPopupType = EGoodsInfoPopupType::EGoodsPopup_Info;

	if (GoodsPopupType == EGoodsInfoPopupType::EGoodsPopup_Info)
	{
		//그냥 일반 팝업
		auto* pUI = UIMgrInst->OpenUI<UB2UIGoodsShortcutTooltipPopup>(UIFName::GoodsShortcutTooltipPopup);
		if (pUI)
		{
			pUI->SetItemInfo(m_ItemInfoidx, EGoodsButtonType::EGoodsButtonType_Info);
			//[@AKI, 170720] M19빌드 개선에 의해 중앙으로 옮김.
			//다시 터치 영역으로 변경 하려면 아래 pUI->SetSlotPosition(GetPopupPosition(pUI)); 주석을 해제하면 됨
			//pUI->SetSlotPosition(GetPopupPosition(pUI));
		}
	}
	else if (GoodsPopupType == EGoodsInfoPopupType::EGoodsPopup_Shortcut)
	{
		//바로 가기가 있는 팝업
		ShortageMGR->CheckContentsInfoShortcut(m_ItemInfoidx);
		
	/*	
		auto* pUI = UIMgrInst->OpenUI<UB2UIGoodsShortcutTooltipPopup>(UIFName::GoodsShortcutTooltipPopup);
		if (pUI)
			pUI->SetItemInfo(m_ItemInfoidx, EGoodsButtonType::EGoodsButtonType_InfoShortcut);
			*/
		
	}
	else if (GoodsPopupType == EGoodsInfoPopupType::EGoodsPopup_Check)
	{
		//골드로 예를 들면
		//골드가 부족하면 GoTo 팝업
		//골드가 부족하지 않으면 Info 팝업
	}
}

void UB2ButtonGoodInfoToolTip::ExtraOnPress()
{
	Super::ExtraOnPress();
}
void UB2ButtonGoodInfoToolTip::ExtraOnHovered()
{
	//Touch Moved 여기엔 뭐할까??
	Super::ExtraOnHovered();
}

void UB2ButtonGoodInfoToolTip::ExtraOnRelease()
{
	Super::ExtraOnRelease();
}
