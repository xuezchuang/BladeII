// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIHeroTowerResult.h"
#include "B2StageManager.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"
#include "B2UIRewardEventSet.h"
#include "B2HeroTowerGameMode.h"
#include "B2UIStageResultLevelUp.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIHeroTowerResult::Init()
{
	Super::Init();

	DeliveryGetHeroTowerTicket = DeliveryGetHeroTowerClass<FB2ResponseGetHeroTowerPtr>::GetInstance().Subscribe([this](const FB2ResponseGetHeroTowerPtr& msgPtr)
	{
		UB2UIDocHeroTower* pDoc = UB2UIDocHelper::GetDocHeroTower();

		if (pDoc)
			pDoc->SetGetHeroTowerPtr(msgPtr);
		
		ReturnToHeroTowerMainMenuClass<>::GetInstance().Signal();
	});

	m_nRemainAutoMoveStageTimeSec = 0;
}

void UB2UIHeroTowerResult::OnOpenComplete()
{
	Super::OnOpenComplete();

	CheckAndOpenLevelUpPart();
}

void UB2UIHeroTowerResult::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	DeliveryGetHeroTowerClass<FB2ResponseGetHeroTowerPtr>::GetInstance().Unsubscribe(DeliveryGetHeroTowerTicket);
}

void UB2UIHeroTowerResult::BindDocAuto()
{

}

void UB2UIHeroTowerResult::UnbindDoc()
{

}

void UB2UIHeroTowerResult::NativeConstruct()
{
	UB2UIDocHeroTower* pDoc = UB2UIDocHelper::GetDocHeroTower();

	if (pDoc)
	{

		if (pDoc->IsClearHeroTower())
		{
			if (CP_Clear.IsValid())
				CP_Clear->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			if (CP_Fail.IsValid())
				CP_Fail->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}

		FCacheStageEssentialDataToKeep& StageData = AB2StageManager::GetCacheStageKeepEssentialData();
		int32 nFloor = StageData.GetHeroTowerFloor();

		if (TB_NFloor.IsValid())
		{			
			TB_NFloor->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTower_NFloor")), FText::AsNumber(nFloor)));
		}

		// 최상층일때 다음층 가기 버튼처리
		if (nFloor == BladeIIGameImpl::GetClientDataStore().GetHeroTowerMaxFloor())
		{
			if (O_NextFloor.IsValid())
				O_NextFloor->SetVisibility(ESlateVisibility::Hidden);
		}
		 

		if (TB_RecordSec.IsValid())
		{
			int32 nMin = (pDoc->m_nCurrentFloorRecordTimeMilliSecForResult / 1000) / 60;
			int32 nSec = (pDoc->m_nCurrentFloorRecordTimeMilliSecForResult / 1000) % 60;
			int32 nMSec = pDoc->m_nCurrentFloorRecordTimeMilliSecForResult % 1000;

			if (nMin == 0)
			{
				TB_RecordSec->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Time_NS_NMS")), FText::AsNumber(nSec), FText::AsNumber(nMSec)));
			}
			else
			{
				TB_RecordSec->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Time_NM_NS_NMS")), FText::AsNumber(nMin), FText::AsNumber(nSec), FText::AsNumber(nMSec)));
			}
		}

		if (HB_Rewards.IsValid())
		{
			//지우고
			TArray<UPanelSlot*> Slots = HB_Rewards->GetSlots();

			for (UPanelSlot* pSlot : Slots)
			{
				if (pSlot)
					HB_Rewards->RemoveChild(pSlot->Content);
			}

			//만들고
			if (pDoc->IsClearHeroTower() && pDoc->m_ptrFinishHeroTower)
			{
				for (auto RewardItem : pDoc->m_ptrFinishHeroTower->rewards)
				{
					if (RewardItem->raw_reward->id != 0)
					{
						UB2UIRewardIcon* pUIP = CreateWidget<UB2UIRewardIcon>(GetOwningPlayer(), RewardItemClass);
						HB_Rewards->AddChild(pUIP);
						pUIP->Init();
						pUIP->UpdateRewardIcon(RewardItem->raw_reward->id, FText::AsNumber(RewardItem->raw_reward->count));
						pUIP->SetPadding(FMargin(0, 0, 10, 0));
					}
				}
			}
		}
	}
}

void UB2UIHeroTowerResult::UpdateStaticText()
{
	if (STB_PastTime.IsValid())
		STB_PastTime->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTower_ClearTime")));
	if (STB_RewardTitle.IsValid())
		STB_RewardTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CounterDG_SubRewardTitle")));
	if (STB_NextFloor.IsValid())
		STB_NextFloor->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTower_NextFloor")));
	if (STB_Quit.IsValid())
		STB_Quit->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Quit")));
	if (STB_Confirm.IsValid())
		STB_Confirm->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Confirm")));
}

void UB2UIHeroTowerResult::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UButton, BTN_OK);
	GET_SLOT(UButton, BTN_NextFloor);
	GET_SLOT(UButton, BTN_FailConfirm);

	GET_SLOT(UCanvasPanel, CP_Clear);
	GET_SLOT(UCanvasPanel, CP_Fail);

	GET_SLOT(UTextBlock, TB_NFloor);
	GET_SLOT(UTextBlock, TB_RecordSec);
	GET_SLOT(UHorizontalBox, HB_Rewards);
	GET_SLOT(UB2RichTextBlock, TB_AutoSceneMoveTime);

	GET_SLOT(UTextBlock, STB_PastTime);
	GET_SLOT(UTextBlock, STB_RewardTitle);
	GET_SLOT(UTextBlock, STB_NextFloor);
	GET_SLOT(UTextBlock, STB_Quit);
	GET_SLOT(UTextBlock, STB_Confirm);

	GET_SLOT(UOverlay, O_NextFloor);

	GET_SLOT(UB2UIStageResultLevelUp, UIP_LevelUp);
	if (UIP_LevelUp.IsValid())
		UIP_LevelUp->Init();
}

void UB2UIHeroTowerResult::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_OK, &UB2UIHeroTowerResult::OnClickBtnOK);
	BIND_CLICK_FUNC_TO_BTN(BTN_NextFloor, &UB2UIHeroTowerResult::OnClickBtnNextFloor);

	BIND_CLICK_FUNC_TO_BTN(BTN_FailConfirm, &UB2UIHeroTowerResult::OnClickBtnOK);
}

void UB2UIHeroTowerResult::CloseWidgetDelegate()
{
	if (CP_Clear.IsValid() && CP_Clear->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		OnClickBtnOK();
	}
}

void UB2UIHeroTowerResult::OnClickBtnOK()
{
	// 타이머 제거
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(TimeToAutoMoveStage);

	data_trader::Retailer::GetInstance().RequestGetHeroTower();
}

void UB2UIHeroTowerResult::OnClickBtnNextFloor()
{
	// 타이머 제거
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(TimeToAutoMoveStage);

	// 점검 정산 체크
	if (CheckContentsModeState(b2network::B2ContentsMode::HERO_TOWER))
		return;

	// 다음층 가자
	UB2UIDocHeroTower* pDoc = UB2UIDocHelper::GetDocHeroTower();

	if (!pDoc)
		return;



	AGameMode* pGameMode = Cast<AGameMode>(UGameplayStatics::GetGameMode(this));
	
	FCacheStageEssentialDataToKeep& StageData = AB2StageManager::GetCacheStageKeepEssentialData();
	FLocalCharacterData& CharDataStore = BladeIIGameImpl::GetLocalCharacterData();

	// 서버에서 받은층 이용
	int32 nFloor = pDoc->GetNextFloorFromFinishInfo();

	StageData.SetHeroTowerFloor(nFloor);

	// colosseum comment : bUseGem 제거
	//StageData.RequestStartHeroTower(pGameMode, nFloor, CharDataStore.GetMainPlayerClass(), CharDataStore.GetSubPlayerClass(), false, pDoc->GetPlayTokenFromFinishInfo());
	StageData.RequestStartHeroTower(pGameMode, nFloor, CharDataStore.GetMainPlayerClass(), CharDataStore.GetSubPlayerClass(), pDoc->GetPlayTokenFromFinishInfo());

	ForcedCloseToolTipPopupClass<>::GetInstance().Signal(); // 아이템 보상 툴팁 떠있으면 지움
}

void UB2UIHeroTowerResult::UpdateAutoMoveStageTimeSec()
{
	UB2UIDocHeroTower* pDoc = UB2UIDocHelper::GetDocHeroTower();

	if (!pDoc)
		return;

	FCacheStageEssentialDataToKeep& StageData = AB2StageManager::GetCacheStageKeepEssentialData();
	int32 nFloor = StageData.GetHeroTowerFloor();

	if (TB_AutoSceneMoveTime.IsValid())
	{
		if (pDoc->IsClearHeroTower() && nFloor != BladeIIGameImpl::GetClientDataStore().GetHeroTowerMaxFloor())
			TB_AutoSceneMoveTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTower_GotoNextFloorAutoNSec")), FText::AsNumber(m_nRemainAutoMoveStageTimeSec)));
		else
			TB_AutoSceneMoveTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("HeroTower_GotoMainAutoNSec")), FText::AsNumber(m_nRemainAutoMoveStageTimeSec)));
	}

	// 다음업데이트 오기전에 미리빼놈.
	m_nRemainAutoMoveStageTimeSec--;

	// UI텍스트에 0으로 표기되는 순간임
	if (m_nRemainAutoMoveStageTimeSec < 0)
	{
		if (pDoc->IsClearHeroTower() && nFloor != BladeIIGameImpl::GetClientDataStore().GetHeroTowerMaxFloor())
			OnClickBtnNextFloor();
		else
			OnClickBtnOK();
	}
}

void UB2UIHeroTowerResult::OnShowResult()
{
	m_nRemainAutoMoveStageTimeSec = AutoMoveStageTimeSec;
	UpdateAutoMoveStageTimeSec();
	// 타이머 등록		
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(TimeToAutoMoveStage, this, &UB2UIHeroTowerResult::UpdateAutoMoveStageTimeSec, 1.0f, true);
	
	auto* GameMode = Cast<AB2HeroTowerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->PlayVictoryBGM();
		PlayAnimationEvent_Anim_Open();
	}

}

void UB2UIHeroTowerResult::CheckAndOpenLevelUpPart()
{
	FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

	TArray<EPCClass> ThisPCClass;

	ThisPCClass.Add(CharacterDataStore.GetMainPlayerClass());
	ThisPCClass.Add(CharacterDataStore.GetSubPlayerClass());

	for (auto pcclass : ThisPCClass)
	{
		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore 개선
		//UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(pcclass));
		//bool bCharLevelUp = (FMath::Max(DocHero->GetBasicUpgradeLevel(), DocHero->GetExtraUpgradeLevel()) > DocHero->GetCurrentLevel());
		const int32 ClearAfterLevel = CharacterDataStore.GetCharacterLevel(pcclass);
		const int32 ClearBeforeLevel = CharacterDataStore.GetBeforeCharacterLevel(pcclass);
		if (ClearAfterLevel > ClearBeforeLevel)
		{
			if (UIP_LevelUp.IsValid())
			{
				UIP_LevelUp->StartMe(ThisPCClass);
				UIP_LevelUp->LevelupFinishDelegate = FOnLevelUpFXFinish::CreateLambda([this]() { this->OnLevelUpFXFinish(); });
				return;
			}
		}
	}

	OnLevelUpFXFinish();
}

void UB2UIHeroTowerResult::PlayAnimationEvent_Anim_End()
{

}

void UB2UIHeroTowerResult::OnLevelUpFXFinish()
{
	//OnShowResult();
	auto* GameMode = Cast<AB2HeroTowerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->StartEventScene_GameClear();
	}
}