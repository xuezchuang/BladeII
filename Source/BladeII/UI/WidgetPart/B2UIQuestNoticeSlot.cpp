// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "QuestManager.h"
#include "B2UIQuestSlot.h"
#include "B2UIQuestNoticeSlot.h"


#include "BladeIIUtil.h"

void UB2UIQuestNoticeSlot::Init()
{
	Super::Init();
}

void UB2UIQuestNoticeSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_ProgressState);
	GET_SLOT(UTextBlock, TB_ProgressState);
	GET_SLOT(UB2UIQuestSlot, UIP_QuestSlot);
	if(UIP_QuestSlot.IsValid())
		UIP_QuestSlot->Init();
}

void UB2UIQuestNoticeSlot::UpdateProcessImage(int32 ProcessState)
{
	if (ProcessMaterials.IsValidIndex(ProcessState))
	{
		UMaterialInterface* ProcessMtrl = ProcessMaterials[ProcessState];
		if (IMG_ProgressState.IsValid() && ProcessMtrl != nullptr)
		{
			IMG_ProgressState->SetBrushFromMaterial(ProcessMtrl);
		}
	}
}

void UB2UIQuestNoticeSlot::InitQuestNoticeSlot(int32 SlotID, int32 QuestState)
{
	if (UIP_QuestSlot.IsValid())
	{
		UIP_QuestSlot->InitQuestDefault(SlotID, true);

		UpdateProcessImage(QuestState -1);
		FText ProcessText = FText::FromString(TEXT("Unknown"));
		switch (QuestState)
		{
		case EQS_PROGRESS: ProcessText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Quest_InProgress")); break;
		case EQS_NOT_ACCEPTED: ProcessText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Quest_New")); break;
		case EQS_COMPLETED: ProcessText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Quest_Complete")); break;
		}

		if(TB_ProgressState.IsValid())
			TB_ProgressState->SetText(ProcessText);
	}
}
