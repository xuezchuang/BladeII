// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIPMainQuestTab.h"
#include "BladeIIUtil.h"
#include "BladeIILocText.h"
#include "QuestManager.h"


void UB2UIPMainQuestTab::Update(int32 InActID, bool InIsLock, int32 QuestID)
{
	ActID = InActID;
	IsLock = InIsLock;
	
	if (TB_Title.IsValid())
	{
		FText StageDiffText = FText::FromString(FString::Printf(TEXT("ChapterName_%d"), (int32)InActID));
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_WORLDNAME, StageDiffText.ToString()));
	}

	if (IsLock == false)
	{
		if (IMG_TabBG_Normal.IsValid())
		{
			IMG_TabBG_Normal->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TSoftObjectPtr<UTexture2D>* FoundAssetPtr = TabNormalBG.Find(ActID);
			if (FoundAssetPtr)
			{
				if (UTexture2D* ActTexture = (*FoundAssetPtr).LoadSynchronous())
				{
					IMG_TabBG_Normal->SetBrushFromTexture(ActTexture);
				}
			}
		}

		if (IMG_TabBG_Disabled.IsValid())
		{
			IMG_TabBG_Disabled->SetVisibility(ESlateVisibility::Hidden);
		}

		if (IMG_Lock.IsValid())
		{
			IMG_Lock->SetVisibility(ESlateVisibility::Hidden);
		}

		if (TB_Count.IsValid())
		{
			TB_Count->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			int32 TotalNum = QuestManager::GetInstance().GetTotalNumberMainQuestByAct(ActID);
			int32 CurNum = QuestID - 1; //완료한 퀘스트 갯수
			for (int i = 1; i < ActID; i++)
			{
				CurNum -= QuestManager::GetInstance().GetTotalNumberMainQuestByAct(i); 
				// 현재 액트에서 퀘스트를 몇개 완료 했는지 알기 위해 완료한 이전 액트의 퀘스트 갯수를 빼줌
			}

			CurNum = CurNum > TotalNum || QuestManager::GetInstance().IsEndMainQuest() ? TotalNum : CurNum;
			CurNum = CurNum < 0 ? 0 : CurNum;

			TB_Count->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("QuestMenu_Progress")), FText::AsNumber(CurNum), FText::AsNumber(TotalNum)));
		}
	}
	else
	{
		if (IMG_TabBG_Disabled.IsValid())
		{
			IMG_TabBG_Disabled->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TSoftObjectPtr<UTexture2D>* FoundAssetPtr = TabDisableBG.Find(ActID);
			if (FoundAssetPtr)
			{
				if (UTexture2D* ActTexture = (*FoundAssetPtr).LoadSynchronous())
				{
					IMG_TabBG_Disabled->SetBrushFromTexture(ActTexture);
				}
			}
		}

		if (IMG_TabBG_Normal.IsValid())
		{
			IMG_TabBG_Normal->SetVisibility(ESlateVisibility::Hidden);
		}

		if (IMG_Lock.IsValid())
		{
			IMG_Lock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (TB_Count.IsValid())
		{
			TB_Count->SetVisibility(ESlateVisibility::Hidden);
		}

		if (TB_Title.IsValid())
		{
			TB_Title->SetColorAndOpacity(TitleDisableColor);
		}
	}
}

void UB2UIPMainQuestTab::SetSelected(bool InValue)
{
	if (InValue)
	{
		if (IMG_SelectFrame.IsValid())
		{
			IMG_SelectFrame->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		if (TB_Title.IsValid())
		{
			TB_Title->SetColorAndOpacity(TitleSelectColor);
		}

		if (TB_Count.IsValid())
		{
			TB_Count->SetColorAndOpacity(CountSelectColor);
		}
	}
	else
	{
		if (TB_Title.IsValid())
		{
			if (IsLock)
			{
				TB_Title->SetColorAndOpacity(TitleDisableColor);
			}
			else
			{
				TB_Title->SetColorAndOpacity(TitleNormalColor);
			}
		}

		if (TB_Count.IsValid())
		{
			TB_Count->SetColorAndOpacity(CountNormalColor);
		}

		if (IMG_SelectFrame.IsValid())
		{
			IMG_SelectFrame->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UB2UIPMainQuestTab::OnClickBTN_Tab()
{
	UpdateMainQuestScrollByIndexClass<const int32>::GetInstance().Signal(ActID);
}

void UB2UIPMainQuestTab::CacheAssets()
{
	GET_SLOT(UB2Image, IMG_TabBG_Normal);
	GET_SLOT(UB2Image, IMG_TabBG_Disabled);
	GET_SLOT(UB2Image, IMG_SelectFrame);
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_Count);
	GET_SLOT(UB2Image, IMG_Lock);
	GET_SLOT(UB2Button, BTN_Tab);
}

void UB2UIPMainQuestTab::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Tab, &UB2UIPMainQuestTab::OnClickBTN_Tab);
}
