// Fill out your copyright notice in the Description page of Project Settings.


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
	, AuraImg(UWidgetBlueprintLibrary::NoResourceBrush())
	, OutLineImg(UWidgetBlueprintLibrary::NoResourceBrush())
	, ReflectImg(UWidgetBlueprintLibrary::NoResourceBrush())
{
	DefaultOnClickSoundIndex = -1;
	//捞霸 乐绢具 胶农费 啊瓷窃
	//ClickMethod = EButtonClickMethod::PreciseClick;
	//TouchMethod = EButtonTouchMethod::PreciseTap;
	//[@AKI, 170417] 困氰捞 乐阑 啊瓷己捞 乐绊, 酒飘率俊辑 荤侩 救窍扁肺 沁栏骨肺 林籍 贸府
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
		//[@AKI, 170329] ESlateBrushDrawType::Image栏肺 窍绊 乐扁 锭巩俊 Margin捞 公矫登绢 逞败临 鞘夸啊 绝澜. ESlateBrushDrawType::Box肺 函版瞪 版快 Margin蔼阑 逞败拎具 窃
		//iconSlate.Margin = WidgetStyle.Normal.Margin; 

		//[@AKI, 170329] 老窜篮 Normal波甫 傍烹栏肺 持绊 乐栏唱 酒飘率 娟扁甫 甸绢焊绊
		//Normal, Hovered, Pressed甫 盒府 窍绊 酵促绊 窍搁 盒府 秦具 窃.
		//SetIcon(iconSlate, iconSlate, iconSlate);
		//SetDrawAs(ESlateBrushDrawType::Image);
	}
	*/
}
//[@AKI, 170329] MakeBrushFromMaterial阑 窍搁 绝绢档 浚柳狼 Default甫 林扁 锭巩俊 nullptr 眉农甫 救窃
void UB2ButtonGoodInfoToolTip::SetIcon(const FSlateBrush& _NormalSlate, const FSlateBrush& _PressedSlate, const FSlateBrush& _HoveredSlate)
{
	/*/
	//[@AKI, 170329] 趣矫 隔扼 出狄. 浚柳狼 Default捞固瘤甫 荤侩 且 版快 Normal苞 鞍篮巴阑 荤侩窍霸 窍扼绊 窃.
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
	//	if ( FMath::Abs(ratio - 1.7f) > 0.1f) // 16:9 == 1.6666667啊 酒匆锭
	//	{
	//		//16:9肺 碍力肺 函券秦 霖促.
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
	//	else //16:9老锭绰 ratio绰 1.0 烙
	//		ratio = 1.0f;
	//}
	//const FVector2D viewRatio(RatioViewportSize / UB2UnitedWidgetBase::DesignedRefViewSize);

	//const FVector2D halfUISize(_pUI->GetUISize() * 0.5f * viewRatio);
	//const FVector2D halfRatioViewportSize(RatioViewportSize * _pUI->GetSlotAnchors());
	//const FVector2D uiSize(_pUI->GetUISize() * viewRatio);
	//const FVector2D checkUISize(touchPos.X + uiSize.X + this->WidgetStyle.Normal.ImageSize.X, touchPos.Y - uiSize.Y - this->WidgetStyle.Normal.ImageSize.Y);
	////UI档 啊款单俊 巨目器牢飘啊 乐栏骨肺 啊款单肺 焊郴扁 困秦辑 沥府
	//FVector2D uiPos(touchPos.X + halfUISize.X - halfRatioViewportSize.X + this->WidgetStyle.Normal.ImageSize.X * 2, touchPos.Y - halfUISize.Y - halfRatioViewportSize.Y); //啊款单啊 0,0捞骨肺
	//if (checkUISize.X > viewportSize.X)
	//{
	//	uiPos.X = viewportSize.X * _pUI->GetSlotAnchors().X - halfUISize.X;
	//}
	//if (checkUISize.Y < 0)
	//{
	//	uiPos.Y = -viewportSize.Y * _pUI->GetSlotAnchors().Y + uiSize.Y;
	//}
//	if (FMath::Abs(ratio - 1.0f) > FLT_EPSILON) // 16:9啊 酒囱 秦惑档俊辑绰 UISize父怒 郴妨啊绰 巩力啊 乐绢 焊埃窃.
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
	//GoodsPopupType	= m_InOwnerUnitedWidget->GetGoodsPopupType();
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
		//弊成 老馆 扑诀
		auto* pUI = UIMgrInst->OpenUI<UB2UIGoodsShortcutTooltipPopup>(UIFName::GoodsShortcutTooltipPopup);
		if (pUI)
		{
			pUI->SetItemInfo(m_ItemInfoidx, EGoodsButtonType::EGoodsButtonType_Info);
			//[@AKI, 170720] M19呼靛 俺急俊 狼秦 吝居栏肺 颗辫.
			//促矫 磐摹 康开栏肺 函版 窍妨搁 酒贰 pUI->SetSlotPosition(GetPopupPosition(pUI)); 林籍阑 秦力窍搁 凳
			//pUI->SetSlotPosition(GetPopupPosition(pUI));
		}
	}
	else if (GoodsPopupType == EGoodsInfoPopupType::EGoodsPopup_Shortcut)
	{
		//官肺 啊扁啊 乐绰 扑诀
		ShortageMGR->CheckContentsInfoShortcut(m_ItemInfoidx);
		
	/*	
		auto* pUI = UIMgrInst->OpenUI<UB2UIGoodsShortcutTooltipPopup>(UIFName::GoodsShortcutTooltipPopup);
		if (pUI)
			pUI->SetItemInfo(m_ItemInfoidx, EGoodsButtonType::EGoodsButtonType_InfoShortcut);
			*/
		
	}
	else if (GoodsPopupType == EGoodsInfoPopupType::EGoodsPopup_Check)
	{
		//榜靛肺 抗甫 甸搁
		//榜靛啊 何练窍搁 GoTo 扑诀
		//榜靛啊 何练窍瘤 臼栏搁 Info 扑诀
	}
}

void UB2ButtonGoodInfoToolTip::ExtraOnPress()
{
	Super::ExtraOnPress();
}
void UB2ButtonGoodInfoToolTip::ExtraOnHovered()
{
	//Touch Moved 咯扁浚 构且鳖??
	Super::ExtraOnHovered();
}

void UB2ButtonGoodInfoToolTip::ExtraOnRelease()
{
	Super::ExtraOnRelease();
}
