#include "B2UITeamMatchBattle.h"
#include "B2UITeamMatchBattleNickName.h"
#include "B2UITeamMatchBattleSlot.h"
#include "B2TMGameMode.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "B2UIBattleSkill.h"

UB2UITeamMatchBattle::UB2UITeamMatchBattle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UITeamMatchBattle::Init()
{
	Super::Init();
}

void UB2UITeamMatchBattle::BindDocAuto()
{
	UnbindDoc();

	auto* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();
	BII_CHECK(TeamMatchDoc);
	if (!TeamMatchDoc)
		return;

	SetDoc(TeamMatchDoc);

	if (LeftSlots[0].IsValid()){
		TeamMatchDoc->OnPCHealth1Changed.AddUObject(LeftSlots[0].Get(), &UB2UITeamMatchBattleSlot::OnChanagedHealth);
		TeamMatchDoc->OnPCShield1Changed.AddUObject(LeftSlots[0].Get(), &UB2UITeamMatchBattleSlot::OnChanagedShield);
	}
	if (LeftSlots[1].IsValid()){
		TeamMatchDoc->OnPCHealth2Changed.AddUObject(LeftSlots[1].Get(), &UB2UITeamMatchBattleSlot::OnChanagedHealth);
		TeamMatchDoc->OnPCShield2Changed.AddUObject(LeftSlots[1].Get(), &UB2UITeamMatchBattleSlot::OnChanagedShield);
	}
	if (LeftSlots[2].IsValid()){
		TeamMatchDoc->OnPCHealth3Changed.AddUObject(LeftSlots[2].Get(), &UB2UITeamMatchBattleSlot::OnChanagedHealth);
		TeamMatchDoc->OnPCShield3Changed.AddUObject(LeftSlots[2].Get(), &UB2UITeamMatchBattleSlot::OnChanagedShield);
	}

	if (RightSlots[0].IsValid()){
		TeamMatchDoc->OnOpponentHealth1Changed.AddUObject(RightSlots[0].Get(), &UB2UITeamMatchBattleSlot::OnChanagedHealth);
		TeamMatchDoc->OnOpponentShield1Changed.AddUObject(RightSlots[0].Get(), &UB2UITeamMatchBattleSlot::OnChanagedShield);
	}
	if (RightSlots[1].IsValid()){
		TeamMatchDoc->OnOpponentHealth2Changed.AddUObject(RightSlots[1].Get(), &UB2UITeamMatchBattleSlot::OnChanagedHealth);
		TeamMatchDoc->OnOpponentShield2Changed.AddUObject(RightSlots[1].Get(), &UB2UITeamMatchBattleSlot::OnChanagedShield);
	}
	if (RightSlots[2].IsValid()){
		TeamMatchDoc->OnOpponentHealth3Changed.AddUObject(RightSlots[2].Get(), &UB2UITeamMatchBattleSlot::OnChanagedHealth);
		TeamMatchDoc->OnOpponentShield3Changed.AddUObject(RightSlots[2].Get(), &UB2UITeamMatchBattleSlot::OnChanagedShield);
	}

	for (uint8 Cnt = 0; Cnt < 3; Cnt++)
	{
		FString SkillAssetName = PrefixSkillAsset + FString::FromInt(Cnt);
		auto SkillAsset = GetSlot<UB2UIBattleSkill>(FName(*SkillAssetName));
		if (SkillAsset)
		{
			SkillAssets.Add(SkillAsset);
			SkillAsset->Init();
			SkillAsset->SetIndex(Cnt);	//SkillRT쪽에서는 1베이스로 작업한 듯
			SkillAsset->OnBattleSkillClicked.BindDynamic(this, &UB2UITeamMatchBattle::OnPressedSkillBtn);
		}
	}
	
	BindDocChar(0,UB2UIDocHelper::GetDocHero(PCClassToInt(TeamMatchDoc->GetPCCharacterClass1())));
	BindDocChar(1,UB2UIDocHelper::GetDocHero(PCClassToInt(TeamMatchDoc->GetPCCharacterClass2())));
	BindDocChar(2,UB2UIDocHelper::GetDocHero(PCClassToInt(TeamMatchDoc->GetPCCharacterClass3())));
}

void UB2UITeamMatchBattle::UnbindDoc()
{
	auto* TeamMatchDoc = Cast<UB2UIDocTeamMatch>(GetDoc());
	if (TeamMatchDoc)
	{
		if (LeftSlots[0].IsValid()){
			TeamMatchDoc->OnPCHealth1Changed.RemoveAll(LeftSlots[0].Get());
			TeamMatchDoc->OnPCShield1Changed.RemoveAll(LeftSlots[0].Get());
		}
		if (LeftSlots[1].IsValid()){
			TeamMatchDoc->OnPCHealth2Changed.RemoveAll(LeftSlots[1].Get());
			TeamMatchDoc->OnPCShield2Changed.RemoveAll(LeftSlots[1].Get());
		}
		if (LeftSlots[2].IsValid()){
			TeamMatchDoc->OnPCHealth3Changed.RemoveAll(LeftSlots[2].Get());
			TeamMatchDoc->OnPCShield3Changed.RemoveAll(LeftSlots[2].Get());
		}

		if (RightSlots[0].IsValid()){
			TeamMatchDoc->OnOpponentHealth1Changed.RemoveAll(RightSlots[0].Get());
			TeamMatchDoc->OnOpponentShield1Changed.RemoveAll(RightSlots[0].Get());
		}
		if (RightSlots[1].IsValid()){
			TeamMatchDoc->OnOpponentHealth2Changed.RemoveAll(RightSlots[1].Get());
			TeamMatchDoc->OnOpponentShield2Changed.RemoveAll(RightSlots[1].Get());
		}
		if (RightSlots[2].IsValid()){
			TeamMatchDoc->OnOpponentHealth3Changed.RemoveAll(RightSlots[2].Get());
			TeamMatchDoc->OnOpponentShield3Changed.RemoveAll(RightSlots[2].Get());
		}
	}

	for (int SkillCnt = 0; SkillCnt < SkillAssets.Num(); ++SkillCnt)
	{
		UB2UIBattleSkill* SkillAsset = SkillAssets[SkillCnt];
		SkillAsset->OnBattleSkillClicked.Unbind();
		SkillAsset->UnbindDoc();
	}

	SetDoc(nullptr);
}

void UB2UITeamMatchBattle::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UITeamMatchBattle::SetReadyLeftSlot(int32 SlotIdx, EPCClass CharacterClass, int32 Lv, int8 FormationNum)
{
	if (!LeftSlots.IsValidIndex(SlotIdx) || !LeftSlots[SlotIdx].IsValid())
		return;

	CharacterClass == EPCClass::EPC_End ? LeftSlots[SlotIdx]->SetVisibility(ESlateVisibility::Hidden)
	: LeftSlots[SlotIdx]->SetVisibility(ESlateVisibility::Visible), LeftSlots[SlotIdx]->SetData(CharacterClass, Lv, true, SlotIdx, FormationNum);
}

void UB2UITeamMatchBattle::SetReadyRightSlot(int32 SlotIdx, EPCClass CharacterClass, int32 Lv)
{
	if (!RightSlots.IsValidIndex(SlotIdx) || !RightSlots[SlotIdx].IsValid())
		return;

	CharacterClass == EPCClass::EPC_End ? RightSlots[SlotIdx]->SetVisibility(ESlateVisibility::Hidden)
		: RightSlots[SlotIdx]->SetVisibility(ESlateVisibility::Visible), RightSlots[SlotIdx]->SetData(CharacterClass, Lv, false, SlotIdx);
}

void UB2UITeamMatchBattle::SetLeftPlayerInfo(const FString& NickName, int32 Lv, int32 Grade, int32 Star)
{
	if (!LeftNickName.IsValid())
		return;

	LeftNickName->SetPlayerInfo(NickName, Lv, Grade, Star);
	LeftNickName->SetMyPlayerBadge(true);
}

void UB2UITeamMatchBattle::SetRightPlayerInfo(const FString& NickName, int32 Lv, int32 Grade, int32 Star)
{
	if (!RightNickName.IsValid())
		return;

	RightNickName->SetPlayerInfo(NickName, Lv, Grade, Star);
	RightNickName->SetMyPlayerBadge(false);
}

void UB2UITeamMatchBattle::SetBattleTime(int32 BattleTime)
{
	TB_BattleTime->SetText(FText::FromString(UB2UIManager::FormatSecondsAsTime(BattleTime, TEXT("m:ss"))));
	if (BattleTime < 5 && IMG_FocusTimer.IsValid() && IMG_FocusTimer->GetVisibility() != ESlateVisibility::Visible)
	{
		PlayFocusTimer_BP();
		IMG_FocusTimer->SetVisibility(ESlateVisibility::Visible);
	}
}

void UB2UITeamMatchBattle::SetSlotCamera(bool bSet, bool bLeft, int32 SlotIdx)
{
	const TArray<TWeakObjectPtr<UB2UITeamMatchBattleSlot>>& Slots = bLeft ? LeftSlots : RightSlots;

	if (Slots.IsValidIndex(SlotIdx) && Slots[SlotIdx].IsValid())
		Slots[SlotIdx]->SetCamera(bSet);
}

void UB2UITeamMatchBattle::SetSlotDeath(bool bSet, bool bLeft, int32 SlotIdx)
{
	const TArray<TWeakObjectPtr<UB2UITeamMatchBattleSlot>>& Slots = bSet ? LeftSlots : RightSlots;

	if (Slots.IsValidIndex(SlotIdx) && Slots[SlotIdx].IsValid())
		Slots[SlotIdx]->SetDeath(bSet);
}

void UB2UITeamMatchBattle::OnSlotChangedOnClickCharacter(bool bMyTeam, int32 SlotIdx)
{
	//TeamMatchChangeCharacterCamClass<bool, int32>::GetInstance().Signal(bMyTeam, SlotIdx);
}

void UB2UITeamMatchBattle::OnSlotChangedHealthAndShieldCommon(bool bMyTeam, int32 SlotIdx)
{
	auto* TeamMatchDoc = UB2UIDocHelper::GetDocTeamMatch();
	if (TeamMatchDoc)
	{
		const TArray<TWeakObjectPtr<UB2UITeamMatchBattleSlot>>& Slots = (bMyTeam ? LeftSlots : RightSlots);

		if (Slots.IsValidIndex(SlotIdx) && Slots[SlotIdx].IsValid())
			Slots[SlotIdx]->SetHpAndShield(
				TeamMatchDoc->GetCharacterHP(bMyTeam, SlotIdx),
				TeamMatchDoc->GetCharacterMaxHP(bMyTeam, SlotIdx),
				TeamMatchDoc->GetCharacterShield(bMyTeam, SlotIdx));
	}
}

void UB2UITeamMatchBattle::CacheAssets()
{
	GET_SLOT(UB2UITeamMatchBattleNickName, LeftNickName);
	if (LeftNickName.IsValid())
		LeftNickName->Init();

	LeftSlots.AddDefaulted(3);
	GET_SLOT_BYNAME(UB2UITeamMatchBattleSlot, LeftSlots[0], LeftSlot1);
	GET_SLOT_BYNAME(UB2UITeamMatchBattleSlot, LeftSlots[1], LeftSlot2);
	GET_SLOT_BYNAME(UB2UITeamMatchBattleSlot, LeftSlots[2], LeftSlot3);

	for (auto& ThisSlot : LeftSlots)
	{
		if (ThisSlot.IsValid())
		{
			ThisSlot->Init();
			ThisSlot->FOnChangedHealthAndShieldCommon.BindUObject(this, &UB2UITeamMatchBattle::OnSlotChangedHealthAndShieldCommon);
			ThisSlot->FOnClickCharacter.BindUObject(this, &UB2UITeamMatchBattle::OnSlotChangedOnClickCharacter);
		}
	}

	GET_SLOT(UB2UITeamMatchBattleNickName, RightNickName);
	if (RightNickName.IsValid())
		RightNickName->Init();

	RightSlots.AddDefaulted(3);
	GET_SLOT_BYNAME(UB2UITeamMatchBattleSlot, RightSlots[0], RightSlot1);
	GET_SLOT_BYNAME(UB2UITeamMatchBattleSlot, RightSlots[1], RightSlot2);
	GET_SLOT_BYNAME(UB2UITeamMatchBattleSlot, RightSlots[2], RightSlot3);

	for (auto& ThisSlot : RightSlots)
	{
		if (ThisSlot.IsValid())
		{
			ThisSlot->Init();
			ThisSlot->FOnChangedHealthAndShieldCommon.BindUObject(this, &UB2UITeamMatchBattle::OnSlotChangedHealthAndShieldCommon);
			ThisSlot->FOnClickCharacter.BindUObject(this, &UB2UITeamMatchBattle::OnSlotChangedOnClickCharacter);
		}
	}

	GET_SLOT(UButton, BTN_ChangeCam);
	GET_SLOT(UTextBlock, TB_BattleTime);
	GET_SLOT(UImage, IMG_FocusTimer);

	GET_SLOT(UButton, BTN_Pause);

	GET_SLOT(UImage, IMG_LongCamera);
	GET_SLOT(UImage, IMG_ShortCamera);

	GET_SLOT(UImage, IMG_Victory);
	GET_SLOT(UImage, IMG_Lose);
	GET_SLOT(UImage, IMG_Draw);
}

void UB2UITeamMatchBattle::UpdateStaticText()
{

}

void UB2UITeamMatchBattle::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_ChangeCam, &UB2UITeamMatchBattle::OnClickCamButton);
	BIND_CLICK_FUNC_TO_BTN(BTN_Pause, &UB2UITeamMatchBattle::OnClickPauseButton);
}

void UB2UITeamMatchBattle::BindDocChar(int32 slotIndex, class UB2UIDocHero* DocHero)
{
	UB2UIBattleSkill* SkillAsset = SkillAssets[slotIndex];
	auto SkillDoc = DocHero->GetSkillDocByInputIndex(3);
	BindDocSkill(slotIndex, SkillDoc);
}

void UB2UITeamMatchBattle::BindDocSkill(int32 SkillInputIndex, UB2UIDocSkill* SkillDoc)
{
	UB2UIBattleSkill* SkillAsset = SkillAssets.IsValidIndex(SkillInputIndex) ? SkillAssets[SkillInputIndex] : nullptr;

	if (SkillAsset)
	{
		SkillAsset->SetEmpty(SkillDoc == nullptr);
		if (SkillDoc)
			SkillAsset->BindDoc(SkillDoc);
		else
			SkillAsset->UnbindDoc();
	}
}

BattleSlotArray& UB2UITeamMatchBattle::GetBattleSlot(bool IsMyTeam)
{
	return IsMyTeam ? LeftSlots : RightSlots;
}

void UB2UITeamMatchBattle::OnClickCamButton()
{
	AB2TMGameMode* GameMode = Cast<AB2TMGameMode>(UGameplayStatics::GetGameMode(this));


	if (GameMode && GameMode->ChangeFixedCamera())
	{
		if (IMG_ShortCamera->GetVisibility() == ESlateVisibility::Visible)
		{
			IMG_ShortCamera->SetVisibility(ESlateVisibility::Hidden);
			IMG_LongCamera->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			IMG_ShortCamera->SetVisibility(ESlateVisibility::Visible);
			IMG_LongCamera->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	/*if (GameMode)
		GameMode->ChangeFixedCamera() ? IMG_LongCamera->SetVisibility(ESlateVisibility::Visible), IMG_ShortCamera->SetVisibility(ESlateVisibility::Hidden)
		: IMG_LongCamera->SetVisibility(ESlateVisibility::Hidden), IMG_ShortCamera->SetVisibility(ESlateVisibility::Visible);*/
}

void UB2UITeamMatchBattle::CloseWidgetDelegate()
{
	OnClickPauseButton();
}

void UB2UITeamMatchBattle::OnClickPauseButton()
{
	//StartPauseMenuClass<EB2GameMode>::GetInstance().Signal(GetB2GameModeType(this));

	//SetPendingPause();
}

void UB2UITeamMatchBattle::SetDamagePercent(bool IsMyTeam, const TArray<float>& TotalDamages)
{
	auto DamageSum = 0.0f;
	auto MaxClassCount = static_cast<int32>(EPCClass::EPC_End);
	auto& BattleSlots = GetBattleSlot(IsMyTeam);

	for (auto& Damage : TotalDamages)
	{
		DamageSum += Damage;
	}

	for (auto BattleSlot : BattleSlots)
	{
		auto CurrentPCClass = BattleSlot->GetCurrentPCClass();
		if (MaxClassCount <= CurrentPCClass)
			continue;
		auto Percent = TotalDamages[CurrentPCClass] / DamageSum * 100.0f;
		BattleSlot->SetDamagePercent(Percent);
	}
}

void UB2UITeamMatchBattle::ShowResultImage(const ENetMatchResult& result)
{
	//[@AKI, 170821] [B2CLT-1888] 모드별 승패 연출 통일 개선
	PlayAnimationEvent_AnimResultAction(result);
	//PlayAnimationEvent_AnimResultAction(result == ENetMatchResult::LocalWin);
	/*
	//혹시 모르니 일단 전부 다 끔
	if (IMG_Victory.IsValid())
	{
		IMG_Victory->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IMG_Lose.IsValid())
	{
		IMG_Lose->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IMG_Draw.IsValid())
	{
		IMG_Draw->SetVisibility(ESlateVisibility::Collapsed);
	}
	//다 안보이게 한 상태에서 보여야 할 것만 찾아서 보여줌
	if (result == ENetMatchResult::LocalWin)
	{
		if (IMG_Victory.IsValid())
		{
			IMG_Victory->SetVisibility(ESlateVisibility::Visible);
			PlayResultImage_BP();
		}
	}
	else if (result == ENetMatchResult::LocalLose)
	{
		if (IMG_Lose.IsValid())
		{
			IMG_Lose->SetVisibility(ESlateVisibility::Visible);
			PlayResultImage_BP();
		}
	}
	else //Draw시에는 양쪽다
	{
		if (IMG_Draw.IsValid())
		{
			IMG_Draw->SetVisibility(ESlateVisibility::Visible);
			PlayResultImage_BP();
		}
	}
	*/
}

void UB2UITeamMatchBattle::OnPressedSkillBtn(UB2UIBattleSkill* ClickedSkill)
{
	//int32 SkillIndex = ClickedSkill->GetIndex();
	//PlayerStartWeaponSkillByIndexClass<int32>::GetInstance().Signal(SkillIndex);
}