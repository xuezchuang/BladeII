// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "B2UIMsgPopupNeedClearStage.h"


#include "BladeIIGameImpl.h"


void UB2UIMsgPopupNeedClearStage::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Button, BTN_GoStageSelect);
	GET_SLOT(UB2Button, BTN_Close);

	BIND_CLICK_FUNC_TO_BTN(BTN_GoStageSelect, &UB2UIMsgPopupNeedClearStage::OnGoStageSelectBtnClick);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIMsgPopupNeedClearStage::OnCloseBtnClick);
}

void UB2UIMsgPopupNeedClearStage::OnGoStageSelectBtnClick()
{
	// �̰� ���� �κ�����̳� é��UI�� �ƴϸ� �۵��� ���Ѵ�.
	auto* ChapterUI = UB2UIDocHelper::GetDocChapter();
	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();
	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();
	
	FServerStageID ServerStageNum = StageDataStore.GetLastClearServerStageIdByLastPlayedStageDifficulty();
	int32 ChapterNum = StageDataStore.GetActIdByServerStageId(ServerStageNum);
	EStageDifficulty Difficulty = StageDataStore.GetStageDifficulty(ServerStageNum);

	// Open Highest Stage Tooltip
	if (ChapterNum != 0)
		StageDataStore.RequestGetActInfoAndChangeChapter(ChapterNum, Difficulty, true, ServerStageNum);

	// ��û������ ����
	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIMsgPopupNeedClearStage::OnCloseBtnClick()
{
	DestroySelf(UB2UIManager::GetInstance());
}
