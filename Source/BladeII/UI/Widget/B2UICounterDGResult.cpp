#include "B2UICounterDGResult.h"
#include "B2UIDocHelper.h"
#include "B2UIManager.h"
#include "B2ItemInfo.h"
#include "B2RewardInfo.h"
#include "B2CounterAttackGameMode.h"
#include "B2UIStageResultLevelUp.h"
#include "B2UIRewardEventSet.h"


#include "BladeIIGameImpl.h"

void UB2UICounterDGResult::Init()
{
	Super::Init();

	if (BTN_OK.IsValid())
		BTN_OK->SetVisibility(ESlateVisibility::Hidden);

	if (TB_SendMail.IsValid())
	{
		TB_SendMail->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("General_GetMailReward"))));
		TB_SendMail->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	SetCounterDungeonLv();
}

void UB2UICounterDGResult::OnOpenComplete()
{
	Super::OnOpenComplete();

	auto* GameMode = Cast<AB2CounterAttackGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
		GameMode->PlayVictoryBGM();
}

void UB2UICounterDGResult::BindDocAuto()
{
	auto* DocCounter = UB2UIDocHelper::GetDocCounterAttack();
	if (DocCounter)
	{
		TArray<b2network::B2RewardPtr> RewardItems = DocCounter->GetRewardItems();

		
		if (List_SubReward.IsValid())
		{
			List_SubReward->ClearChildren();

		

			for (int32 i = 0; i < RewardItems.Num(); i++)
			{
				if (RewardItems.IsValidIndex(i))
				{
					UB2UIRewardIcon* pUIP = CreateWidget<UB2UIRewardIcon>(GetOwningPlayer(), i == 0 ? MainRewardIconClass : SubRewardIconClass);
					pUIP->Init();
					pUIP->UpdateRewardIcon(RewardItems[i]);
					pUIP->SetPadding(FMargin(0, 0, 10, 0));
					List_SubReward->AddChild(pUIP);
				}
			}
		}
	}
}

void UB2UICounterDGResult::SetCounterDungeonLv()
{
	UB2UIDocCounterAttack* CounterDoc = UB2UIDocHelper::GetDocCounterAttack();

	if (!CounterDoc)
		return;

	if (TB_CounterDungeonLv.IsValid())
	{
		TB_CounterDungeonLv->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("counterDungeon_StepComplete")), FText::AsNumber(CounterDoc->GetNewDifficulty())));
	}
}

void UB2UICounterDGResult::UnbindDoc()
{
}

void UB2UICounterDGResult::CacheAssets()
{
	GET_SLOT(UButton, BTN_OK);
	GET_SLOT(UTextBlock, TB_MainRewardName);
	GET_SLOT(UTextBlock, TB_SendMail);

	GET_SLOT(UTextBlock, TB_CounterDungeonLv);

	GET_SLOT(UHorizontalBox, List_SubReward);
	GET_SLOT(UB2UIRewardIcon, UIP_Counter_MainRewardIcon);
	if (UIP_Counter_MainRewardIcon.IsValid())
	{
		UIP_Counter_MainRewardIcon->Init();
	}


	GET_SLOT(UB2UIStageResultLevelUp, UIP_LevelUp);
	if (UIP_LevelUp.IsValid())
	{
		UIP_LevelUp->Init();
	}
}

void UB2UICounterDGResult::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_OK, &UB2UICounterDGResult::OnClickBtnOK);
}

void UB2UICounterDGResult::OnClickBtnOK()
{
	GoToMapClass<>::GetInstance().Signal();
}

void UB2UICounterDGResult::CheckAndOpenLevelUpPart()
{
	auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	auto* DocHero = DocBattle ? UB2UIDocHelper::GetDocHero(DocBattle->GetCurPCClass()) : nullptr;

	if (DocHero)
	{
		//[@SameLine, 180612] explevelinfo doc, datastore -> datastore °³¼±
		EPCClass pcClass = IntToPCClass(DocBattle->GetCurPCClass());
		FLocalCharacterData& CharacterDataStore = BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData();

		const int32 ClearAfterLevel = CharacterDataStore.GetCharacterLevel(pcClass);
		const int32 ClearBeforeLevel = CharacterDataStore.GetBeforeCharacterLevel(pcClass);

		if (ClearAfterLevel > ClearBeforeLevel)
		{
			if (UIP_LevelUp.IsValid())
			{
				TArray<EPCClass> ThisPcClass;
				ThisPcClass.Add(pcClass);
				UIP_LevelUp->StartMe(ThisPcClass);
				UIP_LevelUp->LevelupFinishDelegate = FOnLevelUpFXFinish::CreateLambda([this]() { this->OnFinishLevelUpFX(); });
			}
		}
		else
		{
			if (BTN_OK.IsValid())
				BTN_OK->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UB2UICounterDGResult::OnFinishLevelUpFX()
{
	if (BTN_OK.IsValid())
		BTN_OK->SetVisibility(ESlateVisibility::Visible);
}