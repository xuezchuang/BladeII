// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UISelectAdventureDifficulty.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2UIDocHelper.h"
#include "B2UIDocChapter.h"



#include "Global.h"
#include "FB2ErrorMessage.h"
#include "FakeConfigure.h"
#include "BladeIIGameImpl.h"

#define SET_ADVENTURE_DIFFICULTY_SLOT(Name,CURDIFFICULTY, MAXDIFFICULTY)		\
	if(Name.IsValid()){														\
		Name->Init();														\
		Name->InitDifficulty(CURDIFFICULTY, MAXDIFFICULTY);					\
		UB2UIAdventureDifficultySlot_OnClickBTNSelect Delegate;						\
		Delegate.BindUObject(this, &UB2UISelectAdventureDifficulty::UpdateSlot);	\
		Name->SetDelegate(Delegate);												\
	}

void UB2UISelectAdventureDifficulty::Init()
{
	Super::Init();
}

void UB2UISelectAdventureDifficulty::BindDocAuto()
{
	UnbindDoc();

	auto DocChapter = UB2UIDocHelper::GetDocChapter();
	SetDoc(DocChapter);

	if (DocChapter)
	{
		// Setting Difficulty Slot 
		int32 MaxDifficulty = (int32)DocChapter->GetMaxOpenedStageDifficulty();
		
		// 泅犁 抄捞档狼 付瘤阜 胶抛捞瘤甫 努府绢沁栏搁 促澜 窜拌鳖瘤 坷锹
		bool IsAnimation = false;
		
		if (BladeIIGameImpl::GetStageDataStore().IsClearedStartAdventureModeClientStageId((EStageDifficulty)MaxDifficulty, true))
		{
			MaxDifficulty += 1;
			IsAnimation = true;
		}

		SET_ADVENTURE_DIFFICULTY_SLOT(UIP_SlotList[0], EStageDifficulty::ESD_Hard, (EStageDifficulty)MaxDifficulty);
		SET_ADVENTURE_DIFFICULTY_SLOT(UIP_SlotList[1], EStageDifficulty::ESD_VeryHard, (EStageDifficulty)MaxDifficulty);
		SET_ADVENTURE_DIFFICULTY_SLOT(UIP_SlotList[2], EStageDifficulty::ESD_Hell, (EStageDifficulty)MaxDifficulty);
		SET_ADVENTURE_DIFFICULTY_SLOT(UIP_SlotList[3], EStageDifficulty::ESD_End, (EStageDifficulty)MaxDifficulty);	// 绝绢咙..

		// Bind
		DocChapter->OnSelectHardStageDifficultyChanged.AddUObject(this, &UB2UISelectAdventureDifficulty::OnChangeDifficulty);
		UpdateSlot(DocChapter->GetSelectHardStageDifficulty());
	}
}

void UB2UISelectAdventureDifficulty::UnbindDoc()
{
	if (auto DocChapter = Cast<UB2UIDocChapter>(GetDoc()))
		DocChapter->OnSelectHardStageDifficultyChanged.RemoveAll(this);

	SetDoc(nullptr);
}

void UB2UISelectAdventureDifficulty::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UISelectAdventureDifficulty::OnClickBtnConfirm);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UISelectAdventureDifficulty::OnClickBtnClose);
}

void UB2UISelectAdventureDifficulty::SetDifficulty(EStageDifficulty InDifficulty)
{
	if (InDifficulty < EStageDifficulty::ESD_Hard)
		InDifficulty = EStageDifficulty::ESD_Hard;

	SetDifficulty_BP(FMath::Max((int32)InDifficulty - 2, 0));	// 喉风橇赴飘俊辑绰 牢郸胶啊 0何磐 矫累窍骨肺 蔼阑 烙狼肺 函版秦霖促.
}

void UB2UISelectAdventureDifficulty::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UButton, BTN_Confirm);
	GET_SLOT(UButton, BTN_Close);

	UIP_SlotList.SetNum(3);
	GET_SLOT_BYNAME(UB2UIAdventureDifficultySlot, UIP_SlotList[0], UIP_Hard);
	GET_SLOT_BYNAME(UB2UIAdventureDifficultySlot, UIP_SlotList[1], UIP_VeryHard);
	GET_SLOT_BYNAME(UB2UIAdventureDifficultySlot, UIP_SlotList[2], UIP_Nightmare);
	GET_SLOT_BYNAME(UB2UIAdventureDifficultySlot, UIP_SlotList[3], UIP_Hell);
}

void UB2UISelectAdventureDifficulty::UpdateStaticText()
{

}

void UB2UISelectAdventureDifficulty::OnClickBtnConfirm()
{
	// DocChapter 诀单捞飘
	if (auto DocChapter = Cast<UB2UIDocChapter>(GetDoc()))
		BladeIIGameImpl::GetStageDataStore().RequestGetActInfoAndChangeChapter(DocChapter->GetCurChapterNum(), DocChapter->GetSelectHardStageDifficulty());

	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UISelectAdventureDifficulty::OnClickBtnClose()
{
	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UISelectAdventureDifficulty::OnChangeDifficulty(class UB2UIDocBase* Sender, EStageDifficulty InDifficulty, EStageDifficulty PrevDiff)
{
	if (InDifficulty < EStageDifficulty::ESD_Hard)
		InDifficulty = EStageDifficulty::ESD_Hard;

	SetDifficulty(InDifficulty);

	// 函版等 郴侩 浇吩俊 舅妨林扁
	for (auto ThisSlot : UIP_SlotList)
	{
		if (ThisSlot.IsValid())
			ThisSlot->UpdateSlot(InDifficulty);
	}
}

void UB2UISelectAdventureDifficulty::UpdateSlot(EStageDifficulty UpdateDifficulty)
{
	if (UpdateDifficulty == EStageDifficulty::ESD_End)
		return;

	// DocChapter 诀单捞飘
	if (auto DocChapter = Cast<UB2UIDocChapter>(GetDoc()))
		DocChapter->SetSelectHardStageDifficulty(UpdateDifficulty);
}

#undef SET_ADVENTURE_DIFFICULTY_SLOT