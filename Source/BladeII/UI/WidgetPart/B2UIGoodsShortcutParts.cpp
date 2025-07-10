// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIGoodsShortcutParts.h"
#include "B2UIManager.h"

#include "B2UILinkManager.h"
#include "B2UIGoodsShortcutTooltipPopup.h"
#include "B2LobbyGameMode.h"
#include "B2UIItemForge.h"

UB2UIGoodsShortcutParts::UB2UIGoodsShortcutParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, BTN_Positive(NULL)
	, BTN_Lock(NULL)
	, TB_AddComment(NULL)
	, m_InfoStringID(0)
	, m_LinkScene(EUIScene::None)
	, m_Optional1(0)
	, m_Optional2(0)
{
}

void UB2UIGoodsShortcutParts::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2RichTextBlock, TB_AddComment)
	GET_SLOT(UButton, BTN_Positive)
	GET_SLOT(UButton, BTN_Lock)

	BindDelegates();
	BindData();
}

void UB2UIGoodsShortcutParts::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Positive, &UB2UIGoodsShortcutParts::OnClickBtnMove);
}

void UB2UIGoodsShortcutParts::BindData()
{
	//데이터 테이블 만든 후에 넣을 데이터
	if (m_InfoStringID != 0 && TB_AddComment.IsValid())
		TB_AddComment->SetText(BladeIIGetLOCText(B2LOC_CAT_TOOLTIPTEXT, *(FString::FromInt(m_InfoStringID))));
}


void UB2UIGoodsShortcutParts::SetGoodsShortcutInfo(uint32 _InfoStringID, EUIScene _LinkScene, int32 _Optional1, int32 _Optional2)
{
	m_InfoStringID	= _InfoStringID;
	m_LinkScene		= _LinkScene;
	m_Optional1		= _Optional1;
	m_Optional2		= _Optional2;

	BindData();
}

void UB2UIGoodsShortcutParts::SetVisibleMoveButton(bool IsMovable)
{
	if (BTN_Positive.IsValid())
	{
		BTN_Positive->SetVisibility(IsMovable ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	if (BTN_Lock.IsValid())
	{
		BTN_Lock->SetVisibility(IsMovable ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	}
}

void UB2UIGoodsShortcutParts::SetVisibleButton(bool IsVisible)
{
	if (BTN_Positive.IsValid())
	{
		BTN_Positive->SetVisibility(IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	if (BTN_Lock.IsValid())
	{
		BTN_Lock->SetVisibility(IsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UIGoodsShortcutParts::OnClickBtnMove()
{
	FB2UILinkManager::LinkUIScene(m_LinkScene, m_Optional1, m_Optional2);

	//새로운 메인 UI쪽 관련 작업에 대한 숙달도가 낮아서 앞으로 바로가기 씬을 추가할때 이 부분에도 추가해야 한다.
	if (m_LinkScene == EUIScene::ItemForge)
	{
		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::ItemForge);

		UB2UIItemForge* ItemForgeUI = UB2UIManager::GetInstance()->GetUI<UB2UIItemForge>(UIFName::ItemForge);
		if (ItemForgeUI)
		{
			ItemForgeUI->ForgeInit();
		}
	}

	ToolTipMoveClass<>::GetInstance().Signal();
	m_ParentUI->DestroySelfUsingUIManager();
}