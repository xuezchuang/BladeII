// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIHeaderResource.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "BladeIIGameImpl.h"
#include "B2UIToolTipTextBox.h"

UB2UIHeaderResource::UB2UIHeaderResource(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ValueActionTime = 1.0f;
}

void UB2UIHeaderResource::Init()
{
	Super::Init();

	m_nCurValueAmount = 0;
	m_nGoalValueAmount = -1;
	m_nMaxValueAmount = -1;

	m_fValueChangeStartTime = -1;
	
	m_nBladePointShowMaxValue = 99999; // const value
	m_maxBladeAmount = 150;			//家帕鼻 荤侩矫 弥措 康去八 荐摹啊 -1肺 檬扁拳登绰 巩力肺 钎矫俊辑父 窍靛内爹 凳.
	isRequirement = false;
}

void UB2UIHeaderResource::SetAmount(int32 NewValueAmount, int32 MaxValueAmount, bool bFromInit)
{
	// 檬扁拳俊辑 阂赴芭搁 官肺 咆胶飘俊 悸泼窍备 府畔
	if (bFromInit)
	{
		SetAmountText(NewValueAmount, MaxValueAmount);
		m_nGoalValueAmount = NewValueAmount;
		return;
	}

	// 绢凋啊 平俊辑 拌加 龋免登绰淀.. 格钎樊捞尔 鞍栏搁 府畔
	if (m_nGoalValueAmount == NewValueAmount)
		return;
	
	m_nOrgValueAmount = m_nCurValueAmount;
	m_nGoalValueAmount = NewValueAmount;
	//TB_Amount->SetText(txtAmount);
	
	// 捞扒 窜鉴 历厘侩
	m_nMaxValueAmount = MaxValueAmount;

	m_fValueChangeStartTime = GetWorld()->GetTimeSeconds();
}

void UB2UIHeaderResource::SetRemainTime(FText txtRemainTime)
{
	if (TB_RemainTime.IsValid())
		TB_RemainTime->SetText(txtRemainTime);
}

void UB2UIHeaderResource::SetAmountText(int32 NewValueAmount, int32 MaxValueAmoun /*= -1*/)
{
	m_nCurValueAmount = NewValueAmount;
	m_nMaxValueAmount = MaxValueAmoun;

	if (TB_Amount.IsValid())
	{
		if (MaxValueAmoun == -1)
		{
			TB_Amount->SetText(FText::AsNumber(NewValueAmount));

			if (EUIHeaderResourceType::BladePoint == ResourceUIType)
			{
				//胶抛固呈俊 抗寇贸府啊 腹酒廉辑 蝶肺 备泅
				SetBladePointAmountText(NewValueAmount, m_maxBladeAmount);
			}
		}
		else
		{
			TB_Amount->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(NewValueAmount), FText::AsNumber(MaxValueAmoun)));

			//[@AKI, 171113] B2BUG-541 康去八 家瘤 肮荐 钎泅 荐沥
			if (EUIHeaderResourceType::BladePoint == ResourceUIType)
			{
				//胶抛固呈俊 抗寇贸府啊 腹酒廉辑 蝶肺 备泅
				SetBladePointAmountText(NewValueAmount, MaxValueAmoun);
			}
		}
	}
}

void UB2UIHeaderResource::SetBladePointAmountText(const int32 NewValueAmount, const int32 MaxValueAmoun /* = -1 */)
{
	//郴啊 家蜡茄 康去八捞 120俺 固父捞绢辑 RemainTime阑 焊咯拎具 窍绰 版快
	if (NewValueAmount < MaxValueAmoun)
	{
		if (TB_RemainTime.IsValid())
			TB_RemainTime->SetVisibility(ESlateVisibility::Visible);
	}
	//郴啊 家蜡茄 康去八捞 120俺 捞惑捞绢辑 RemainTime阑 救焊咯拎具 窍绰 版快
	else
	{
		//老窜 捞版快绰 RemainTime阑 公炼扒 波淋.
		if (TB_RemainTime.IsValid())
			TB_RemainTime->SetVisibility(ESlateVisibility::Collapsed);

		/*999俺甫 逞阑锭*/
		if (NewValueAmount > m_nBladePointShowMaxValue	/* 999 */)
		{
			TB_Amount->SetText(FText::Format(FText::FromString(TEXT("{0}+/{1}")), FText::AsNumber(m_nBladePointShowMaxValue), FText::AsNumber(MaxValueAmoun)));
		}
	}
}

void UB2UIHeaderResource::OnPlusBtnClick()
{
	//auto* DocStore = UB2UIDocHelper::GetDocStore();
	//bool bGDC = ShouldShowGDCVersionContent(); // [GDC] Feb 2017
	//bool bCBT = ShouldShowCBTVersionContent();
	//	
	//switch (ResourceUIType)
	//{
	//case EUIHeaderResourceType::None:
	//	break;

	//case EUIHeaderResourceType::DarkCrystal:
	//	if (DocStore)
	//	{
	//		DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
	//		DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::GEM);
	//	}
	//	EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::Store));
	//	break;

	//case EUIHeaderResourceType::BladePoint:
	//	if (DocStore)
	//	{
	//		DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
	//		DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::BLADE_POINT);
	//	}
	//	EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::Store));
	//	break;

	//case EUIHeaderResourceType::Gem:
	//	if (DocStore)
	//	{
	//		DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
	//		DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::GEM);
	//	}	
	//	EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::Store));
	//	break;

	//case EUIHeaderResourceType::Gold:
	//	if (DocStore)
	//	{
	//		DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
	//		DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::GOLD);
	//	}
	//	EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::Store));
	//	break;

	//case EUIHeaderResourceType::SocialPoint:
	//	break;

	//case EUIHeaderResourceType::HeroTowerTicket:
	//	break;

	//case EUIHeaderResourceType::HeroPiece:
	//	break;

	//case EUIHeaderResourceType::DimensionPiece:
	//	break;

	//case EUIHeaderResourceType::Max:
	//	break;

	//default:
	//	break;
	//}
}

void UB2UIHeaderResource::OnToolTipBtnClick()
{
	// 咆胶飘 眠啊窍绊 焊咯林磊
	return;

	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
	{
#if WITH_EDITOR && !PLATFORM_MAC
		FText txtContent = FText::FromString(TEXT("砒屏\n捞芭 积阿秦焊聪 沥纺档 距埃\n巩力啊 登摆绰单... 弊成 哭率 沥纺窍搁 捞卉妨唱\n老窜 肋唱坷绰瘤 毫焊磊せ"));
#else
		FText txtContent = FText::FromString(TEXT("[TEMP] Tootip\nAlignment might be an issue too..\nLet's just see if left alignment works fine."));
#endif
		pUIManager->OpenToolTipTextBox(txtContent,
			this, 
			BTN_ToolTip.Get());
	}
}

void UB2UIHeaderResource::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateValueAmount();
}

void UB2UIHeaderResource::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_ToolTip);
	GET_SLOT(UTextBlock, TB_Amount);
	GET_SLOT(UTextBlock, TB_RemainTime);
	GET_SLOT(UB2Button, BTN_Plus);
	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_Goods);

	if (BTN_Goods.IsValid())
	{		
		if(ResourceUIType == EUIHeaderResourceType::Brevet)
			BTN_Goods->SetItemInfo(FItemRefIDHelper::ITEM_REF_ID_BREVET_STONE);
		else if (ResourceUIType == EUIHeaderResourceType::AdvancedBrevet)
			BTN_Goods->SetItemInfo(FItemRefIDHelper::ITEM_REF_ID_ADVANCED_BREVET_STONE);
		else if (ResourceUIType == EUIHeaderResourceType::HeroPiece)
			BTN_Goods->SetItemInfo(FItemRefIDHelper::ITEM_REF_ID_HERO_PIECE);
		else if (ResourceUIType == EUIHeaderResourceType::SocialPoint)
			BTN_Goods->SetItemInfo(FItemRefIDHelper::GetGoodsID_FriendshipPoint()); //家既器牢飘绰 绝绢辑 烙矫肺 康旷炼阿阑 焊咯淋
		else if (ResourceUIType == EUIHeaderResourceType::DimensionPiece)
			BTN_Goods->SetItemInfo(FItemRefIDHelper::ITEM_REF_ID_DIMENSION_PIECE);
		else if (ResourceUIType == EUIHeaderResourceType::Gold)
			BTN_Goods->SetItemInfo(FItemRefIDHelper::GetGoodsID_Gold());
	}

	BIND_CLICK_FUNC_TO_BTN(BTN_ToolTip, &UB2UIHeaderResource::OnToolTipBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_Plus, &UB2UIHeaderResource::OnPlusBtnClick);
}

void UB2UIHeaderResource::UpdateValueAmount()
{
	if (m_fValueChangeStartTime == -1.0f)
		return;

	float fElapseTime = GetWorld()->GetTimeSeconds() - m_fValueChangeStartTime;

	//testValue 庆歹UI俊 涝仿窍霸窍芭唱..
	float fTimeTotalDuration = ValueActionTime;

	if (fElapseTime >= fTimeTotalDuration)
	{
		SetAmountText(m_nGoalValueAmount, m_nMaxValueAmount);
		m_fValueChangeStartTime = -1.0f;
		return;
	}

	// UI俊 嘛绢临 犁拳樊
	int32 nAmount = int32(m_nOrgValueAmount + ((m_nGoalValueAmount - m_nOrgValueAmount) * (fElapseTime / fTimeTotalDuration)));
	SetAmountText(nAmount, m_nMaxValueAmount);

}

void UB2UIHeaderResource::OpenReadyPopUp()
{
	DevShowNotReadyYetPopup();
}

void UB2UIHeaderResource::SetVisibilityOfPlus(bool showing)
{
	if (showing == true)
	{
		BTN_Plus->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BTN_Plus->SetVisibility(ESlateVisibility::Hidden);
		isRequirement = true;
	}
}

void UB2UIHeaderResource::SetTextColorByAmount(int32 currentHave, int32 requireedAmount)
{
	if(currentHave<requireedAmount)
		TB_Amount->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
	else
		TB_Amount->SetColorAndOpacity(FLinearColor::White);
}