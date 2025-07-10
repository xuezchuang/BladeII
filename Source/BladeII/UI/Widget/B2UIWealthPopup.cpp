// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIWealthPopup.h"

#include "B2UIManager.h"
#include "B2UIWealthPopupSlot.h"
#include "B2UIDocHelper.h"
#include "B2UIDocUICondition.h"
#include "B2AndroidBackManager.h"

void UB2UIWealthPopupSlotHB::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UHorizontalBox, HorizontalBox);
}

void UB2UIWealthPopupSlotHB::AddChildWidget(UWidget* Content)
{
	if (HorizontalBox.IsValid())
		HorizontalBox->AddChild(Content);
}

void UB2UIWealthPopup::Init()
{
	Super::Init();

	this->BindDocAuto();

	B2AndroidBackManager::GetInstance()->AddBackWidget(this);
}

void UB2UIWealthPopup::DestroySelf(class UB2UIManager* InUIManager)
{
	B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

	this->UnbindDoc();

	Super::DestroySelf(InUIManager);
}

void UB2UIWealthPopup::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_Close);
	GET_SLOT(UVerticalBox, VB_WealthSlotList);
	GET_SLOT(UImage, IMG_WealthSlotListBG);
}

void UB2UIWealthPopup::BindDocAuto()
{
	UnbindDoc();

	if (UB2UIDocUICondition* pDocUICondition = UB2UIDocHelper::GetDocUICondition())
	{
		pDocUICondition->OnHideAllLobbyMainUIsChanged.AddUObject(this, &UB2UIWealthPopup::OnChangedHideAllLobbyMainUIs);
	}
}

void UB2UIWealthPopup::UnbindDoc()
{
	if (UB2UIDocUICondition* pDocUICondition = UB2UIDocHelper::GetDocUICondition())
	{
		pDocUICondition->OnHideAllLobbyMainUIsChanged.RemoveAll(this);
	}
}

void UB2UIWealthPopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIWealthPopup::OnClickBTN_Close);
}

void UB2UIWealthPopup::CloseWidgetDelegate()
{
	OnClickBTN_Close();
}

void UB2UIWealthPopup::OnClickBTN_Close()
{
	this->DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIWealthPopup::OnChangedHideAllLobbyMainUIs(class UB2UIDocBase* Sender, bool HideAllLobbyMainUIs, bool PrevHideAllLobbyMainUIs)
{
	if (HideAllLobbyMainUIs == true)
	{
		OnClickBTN_Close();
	}
}

void UB2UIWealthPopup::InitWealthPopup(FVector2D v2WealthPopupSlotPostion)
{
	this->Init();

	if (VB_WealthSlotList.IsValid() == false)
	{
		return;
	}

	VB_WealthSlotList->ClearChildren();
	VB_WealthSlotList->SetRenderTranslation(v2WealthPopupSlotPostion);

	CreateWealthPopupSlot();

	if (IMG_WealthSlotListBG.IsValid())
	{
		IMG_WealthSlotListBG->SetRenderTranslation(v2WealthPopupSlotPostion);
	}
}

void UB2UIWealthPopup::CreateWealthPopupSlot()
{
	auto* pWealthPopupSlotInfo = WealthPopupSlotInfo->GetDefaultObject<UB2UIWealthPopupSlotInfo>();

	if (pWealthPopupSlotInfo == nullptr)
	{
		return;
	}
	
	TWeakObjectPtr<UB2UIWealthPopupSlotHB> pWealthSlotHB = nullptr;

	const TArray<FWealthInfo>& ArrayWealthInfo = pWealthPopupSlotInfo->GetArrayWealthInfo();

	const int32 WealthSlotMax = ArrayWealthInfo.Num();
	for (int32 idx = 0; idx < WealthSlotMax; ++idx)
	{
		if (ArrayWealthInfo.IsValidIndex(idx) == false)
		{
			continue;
		}

		if (idx % MAX_Horizontal == 0)
		{
			auto* pCreateHBWidget = CreateWidget<UB2UIWealthPopupSlotHB>(GetOwningPlayer(), WealthSlotHBClass);
			pCreateHBWidget->Init();
			VB_WealthSlotList->AddChild(pCreateHBWidget);
			pWealthSlotHB = pCreateHBWidget;
		}

		if (pWealthSlotHB.IsValid())
		{
			auto* pWealthSlot = CreateWidget<UB2UIWealthPopupSlot>(GetOwningPlayer(), WealthPopupSlotClass);

			pWealthSlot->InitWealthPopupSlot(ArrayWealthInfo[idx].WealthType, ArrayWealthInfo[idx].MaxCount);
			pWealthSlot->SetWealthImageFromMaterial(ArrayWealthInfo[idx].WealthMaterial.LoadSynchronous());

			pWealthSlotHB->AddChildWidget(pWealthSlot);
		}
	}
}
