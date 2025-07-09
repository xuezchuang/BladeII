// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
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
	m_maxBladeAmount = 150;			//소탕권 사용시 최대 영혼검 수치가 -1로 초기화되는 문제로 표시에서만 하드코딩 됨.
	isRequirement = false;
}

void UB2UIHeaderResource::SetAmount(int32 NewValueAmount, int32 MaxValueAmount, bool bFromInit)
{
	// 초기화에서 불린거면 바로 텍스트에 셋팅하구 리턴
	if (bFromInit)
	{
		SetAmountText(NewValueAmount, MaxValueAmount);
		m_nGoalValueAmount = NewValueAmount;
		return;
	}

	// 어딘가 틱에서 계속 호출되는듯.. 목표량이랑 같으면 리턴
	if (m_nGoalValueAmount == NewValueAmount)
		return;
	
	m_nOrgValueAmount = m_nCurValueAmount;
	m_nGoalValueAmount = NewValueAmount;
	//TB_Amount->SetText(txtAmount);
	
	// 이건 단순 저장용
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
				//스테미너에 예외처리가 많아져서 따로 구현
				SetBladePointAmountText(NewValueAmount, m_maxBladeAmount);
			}
		}
		else
		{
			TB_Amount->SetText(FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(NewValueAmount), FText::AsNumber(MaxValueAmoun)));

			//[@AKI, 171113] B2BUG-541 영혼검 소지 갯수 표현 수정
			if (EUIHeaderResourceType::BladePoint == ResourceUIType)
			{
				//스테미너에 예외처리가 많아져서 따로 구현
				SetBladePointAmountText(NewValueAmount, MaxValueAmoun);
			}
		}
	}
}

void UB2UIHeaderResource::SetBladePointAmountText(const int32 NewValueAmount, const int32 MaxValueAmoun /* = -1 */)
{
	//내가 소유한 영혼검이 120개 미만이어서 RemainTime을 보여줘야 하는 경우
	if (NewValueAmount < MaxValueAmoun)
	{
		if (TB_RemainTime.IsValid())
			TB_RemainTime->SetVisibility(ESlateVisibility::Visible);
	}
	//내가 소유한 영혼검이 120개 이상이어서 RemainTime을 안보여줘야 하는 경우
	else
	{
		//일단 이경우는 RemainTime을 무조건 꺼줌.
		if (TB_RemainTime.IsValid())
			TB_RemainTime->SetVisibility(ESlateVisibility::Collapsed);

		/*999개를 넘을때*/
		if (NewValueAmount > m_nBladePointShowMaxValue	/* 999 */)
		{
			TB_Amount->SetText(FText::Format(FText::FromString(TEXT("{0}+/{1}")), FText::AsNumber(m_nBladePointShowMaxValue), FText::AsNumber(MaxValueAmoun)));
		}
	}
}

void UB2UIHeaderResource::OnPlusBtnClick()
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	bool bGDC = ShouldShowGDCVersionContent(); // [GDC] Feb 2017
	bool bCBT = ShouldShowCBTVersionContent();
		
	switch (ResourceUIType)
	{
	case EUIHeaderResourceType::None:
		break;

	case EUIHeaderResourceType::DarkCrystal:
		if (DocStore)
		{
			DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
			DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::GEM);
		}
		EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::Store));
		break;

	case EUIHeaderResourceType::BladePoint:
		if (DocStore)
		{
			DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
			DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::BLADE_POINT);
		}
		EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::Store));
		break;

	case EUIHeaderResourceType::Gem:
		if (DocStore)
		{
			DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
			DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::GEM);
		}	
		EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::Store));
		break;

	case EUIHeaderResourceType::Gold:
		if (DocStore)
		{
			DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
			DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::GOLD);
		}
		EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(EStorePageWhere::Store));
		break;

	case EUIHeaderResourceType::SocialPoint:
		break;

	case EUIHeaderResourceType::HeroTowerTicket:
		break;

	case EUIHeaderResourceType::HeroPiece:
		break;

	case EUIHeaderResourceType::DimensionPiece:
		break;

	case EUIHeaderResourceType::Max:
		break;

	default:
		break;
	}
}

void UB2UIHeaderResource::OnToolTipBtnClick()
{
	// 텍스트 추가하고 보여주자
	return;

	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
	{
#if WITH_EDITOR && !PLATFORM_MAC
		FText txtContent = FText::FromString(TEXT("툴팁\n이거 생각해보니 정렬도 약간\n문제가 되겠는데... 그냥 왼쪽 정렬하면 이쁠려나\n일단 잘나오는지 봐보자ㅋ"));
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
			BTN_Goods->SetItemInfo(FItemRefIDHelper::GetGoodsID_FriendshipPoint()); //소셜포인트는 없어서 임시로 영웅조각을 보여줌
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

	//testValue 헤더UI에 입력하게하거나..
	float fTimeTotalDuration = ValueActionTime;

	if (fElapseTime >= fTimeTotalDuration)
	{
		SetAmountText(m_nGoalValueAmount, m_nMaxValueAmount);
		m_fValueChangeStartTime = -1.0f;
		return;
	}

	// UI에 찍어줄 재화량
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