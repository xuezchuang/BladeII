// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIDailyNoticePopup.h"
#include "B2UIManager.h"
#include "B2AndroidBackManager.h"

#include "BladeIIGameImpl.h"
#include "B2UIDocHelper.h"
#include "B2UIDailyNoticeSlot.h"


void UB2UIDailyNoticePopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_Desc);

	GET_SLOT(UVerticalBox, VB_DailyNoticeList);

	GET_SLOT(UB2Button, BTN_Cancel);
}

void UB2UIDailyNoticePopup::UpdateStaticText()
{
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DailyNotice_Title")));
	if (TB_Desc.IsValid())
		TB_Desc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DailyNotice_Desc")));
}

void UB2UIDailyNoticePopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Cancel, &UB2UIDailyNoticePopup::OnClickBTN_Cancel);
}

void UB2UIDailyNoticePopup::CloseWidgetDelegate()
{
	CloseSelf();
}

void UB2UIDailyNoticePopup::Init()
{
	Super::Init();

	SetDailyNoticeList();
}

void UB2UIDailyNoticePopup::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);
}

void UB2UIDailyNoticePopup::SetDailyNoticeList()
{
	ItemSlots.Empty();

	TArray<FStoreProductData> ProductList;
	BladeIIGameImpl::GetClientDataStore().GetDailyNoticePackageList(ProductList);

	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
	{
		// 惑前俊辑绰 狐脸栏唱, 捞傈俊 备概茄巴捞 酒流 扁埃捞 巢篮巴阑 钎矫窍扁 困秦 荐沥
		TArray<int32> ReceivedProducts = DocStore->GetReceivedPeriodicPackageProductIDs();

		for (auto& Elem : ReceivedProducts)
		{
			int64 EndTime = DocStore->GetReceivedPeriodicPackageLeftTime(Elem);
			if (EndTime > 0)
			{
				auto* CreatedWidget = CreateWidget<UB2UIDailyNoticeSlot>(GetOwningPlayer(), DailyNoticeSlot);
				if (CreatedWidget)
				{
					CreatedWidget->Init();
					CreatedWidget->SetDailyNoticeData(Elem);
					CreatedWidget->SetMoveButton(false);
					CreatedWidget->SetPeriodicPackageLeftTime(EndTime);

					if (VB_DailyNoticeList.IsValid())
						VB_DailyNoticeList->AddChild(CreatedWidget);
				}
			}
		}

		for (auto& Elem : ProductList)
		{
			int64 EndTime = DocStore->GetReceivedPeriodicPackageLeftTime(Elem.Product.ProductId);
			if (EndTime == 0)
			{
				auto* CreatedWidget = CreateWidget<UB2UIDailyNoticeSlot>(GetOwningPlayer(), DailyNoticeSlot);
				if (CreatedWidget)
				{
					CreatedWidget->Init();
					CreatedWidget->SetDailyNoticeData(Elem.Product.ProductId);
					CreatedWidget->SetMoveButton(true);

					if (VB_DailyNoticeList.IsValid())
						VB_DailyNoticeList->AddChild(CreatedWidget);
				}
			}
		}
	}
}

void UB2UIDailyNoticePopup::OnClickBTN_Cancel()
{
	CloseSelf();
}
