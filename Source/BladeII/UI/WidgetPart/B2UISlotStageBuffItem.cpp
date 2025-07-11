#include "B2UISlotStageBuffItem.h"
#include "B2UIRepeatableElements.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"


#include "BladeIIGameImpl.h"

UB2UISlotStageBuffItem::UB2UISlotStageBuffItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyBuffType = EStageBuffType::SBT_End;
	bSelected = false;
	ServerStageID = FServerStageID();

	MainIconImageMat_UnSelected = NULL;
	MainIconImageMat_Selected = NULL;
}

void UB2UISlotStageBuffItem::SynchronizeProperties()
{
	if (!TB_ItemName.IsValid()){
		CacheAssets();
	}

	Super::SynchronizeProperties();
}

void UB2UISlotStageBuffItem::Init()
{
	Super::Init();

}

void UB2UISlotStageBuffItem::SetBuffSlotInfo(FServerStageID InServerStageNum, EStageBuffType InType)
{
	MyBuffType = InType;
	ServerStageID = InServerStageNum;

	int32 BuffCost = BladeIIGameImpl::GetStageDataStore().GetStageSingleBuffGoldCost(InServerStageNum, MyBuffType);
	bool bHasTicket = false; //BladeIIGameImpl::GetClientDataStore().HasTicket(MyBuffType);

	UpdateBuffIconDisplay(BuffCost, bHasTicket);
}

void UB2UISlotStageBuffItem::SetSelected(bool InbSelected)
{
	bSelected = InbSelected;

	// �ʿ��� �̹����� �г� visibility ����

	if (IMG_MainIcon.IsValid())
	{
		IMG_MainIcon->SetBrushFromMaterial(bSelected ? MainIconImageMat_Selected : MainIconImageMat_UnSelected);
	}

	// �߰� ���� ���� �̹���. �̰� ���ҽ� �۾��� �̷��� �� ���¿��� �ͼ� �� �ϵ��ڵ� ��.
	switch (MyBuffType)
	{
	case EStageBuffType::SBT_IncreaseExp:
		if (IMG_Selected_IncExp.IsValid())
		{
			IMG_Selected_IncExp->SetVisibility(bSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		}
		break;
	case EStageBuffType::SBT_IncreaseGold:
		if (IMG_Selected_IncGold.IsValid())
		{
			IMG_Selected_IncGold->SetVisibility(bSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		}
		break;
	case EStageBuffType::SBT_AutoSkill:
		if (IMG_Selected_AutoSkill.IsValid())
		{
			IMG_Selected_AutoSkill->SetVisibility(bSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		}
		break;
	case EStageBuffType::SBT_BoostGameSpeed:
		if (IMG_Selected_BoostGameSpeed.IsValid())
		{
			IMG_Selected_BoostGameSpeed->SetVisibility(bSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		}
		break;
	}
	
	SetSelected_BP(bSelected);
}

void UB2UISlotStageBuffItem::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_ItemName)
	GET_SLOT(UImage, IMG_MainIcon)
	if (IMG_MainIcon.IsValid())
	{
		IMG_MainIcon->SetBrushFromMaterial(MainIconImageMat_UnSelected);
	}

	GET_SLOT(UImage, IMG_CostTicketIcon)
	GET_SLOT(UImage, IMG_CostGoldIcon)
	GET_SLOT(UTextBlock, TB_Cost)

	GET_SLOT(UImage, IMG_Selected_BoostGameSpeed)
	if (IMG_Selected_BoostGameSpeed.IsValid())
	{
		IMG_Selected_BoostGameSpeed->SetVisibility(ESlateVisibility::Hidden);
	}
	GET_SLOT(UImage, IMG_Selected_IncExp)
	if (IMG_Selected_IncExp.IsValid())
	{
		IMG_Selected_IncExp->SetVisibility(ESlateVisibility::Hidden);
	}
	GET_SLOT(UImage, IMG_Selected_IncGold)
	if (IMG_Selected_IncGold.IsValid())
	{
		IMG_Selected_IncGold->SetVisibility(ESlateVisibility::Hidden);
	}
	GET_SLOT(UImage, IMG_Selected_AutoSkill)
	if (IMG_Selected_AutoSkill.IsValid())
	{
		IMG_Selected_AutoSkill->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UISlotStageBuffItem::UpdateStaticText()
{
	Super::UpdateStaticText();

}

void UB2UISlotStageBuffItem::BindDelegates()
{
	Super::BindDelegates();

}

void UB2UISlotStageBuffItem::UpdateBuffIconDisplay(int32 GoldCost, bool bHasTicket)
{
	if (TB_ItemName.IsValid())
	{
		TB_ItemName->SetText(GetLOCTextOfStageBuffItem(MyBuffType));
	}

	// �̿�� ������ ������ �̿���� �ִٸ� Gold ���� �̰� ���� ���� �ȴ�.
	if (IMG_CostTicketIcon.IsValid())
	{ 
		IMG_CostTicketIcon->SetVisibility(bHasTicket ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (IMG_CostGoldIcon.IsValid())
	{ // �̿���� ���� ��Ȳ�̶�� Gold �� ������ �� ��.
		IMG_CostGoldIcon->SetVisibility(bHasTicket ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}

	if (TB_Cost.IsValid())
	{
		int32 DisplayCost = bHasTicket ? 0 : GoldCost;
		TB_Cost->SetText(FText::AsNumber(DisplayCost));
	}
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UISlotStageBuffItem::OnClickBtnArea()
{
	// Select, Unselect ���δ� ���⼭ ���� �ʴ´�.

	if (GetClickable()){
		OnSlotStageBuffItemClickDelgate.ExecuteIfBound(this);
	}
}

//====================================================================================
//									Doc Delegate
//====================================================================================
