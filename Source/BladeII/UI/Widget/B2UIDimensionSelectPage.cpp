// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UIFairyLinkButton.h"
#include "B2UIStartButton.h"
#include "B2UICharacterSelectPage.h"
#include "B2StageManager.h"
#include "B2UIDocHelper.h"
#include "B2UIRecommandPower.h"
#include "B2LobbyGameMode.h"
#include "B2UIDimensionSelectPage.h"

#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIDimensionSelectPage::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, STB_Hero);
	GET_SLOT(UTextBlock, TB_HeroPower);
	GET_SLOT(UTextBlock, STB_Recommend);
	GET_SLOT(UTextBlock, TB_RecommendPower);

	GET_SLOT(UB2UIRecommandPower, UIP_RecommendPower);
	if (UIP_RecommendPower.IsValid())
	{
		UIP_RecommendPower->Init();
	}
}

void UB2UIDimensionSelectPage::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIDimensionSelectPage::Init()
{
	Super::Init();
}

void UB2UIDimensionSelectPage::OnSceneOpen(EUIScene InOpenedScene)
{
	Super::OnSceneOpen(InOpenedScene);

	int32 Diff = 1;
	auto DocDimension = UB2UIDocHelper::GetDocDimension();
	if (DocDimension)
	{
		Diff = DocDimension->GetDifficulty();
	}
	FText DiffText = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DimensionResult_NDifficulty")), Diff);

	FText StageText = FText::FromString(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_Dimension")).ToString() + " - " + DiffText.ToString());
	SetLobbyUIHeaderTitle(StageText);
}

void UB2UIDimensionSelectPage::DestroySelf(UB2UIManager* InUIManager)
{	
	Super::DestroySelf(InUIManager);
}

void UB2UIDimensionSelectPage::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (STB_Hero.IsValid())
		STB_Hero->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_CurrentCombatpower")));
	if (STB_Recommend.IsValid())
		STB_Recommend->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_RecomCombatPower")));
}

void UB2UIDimensionSelectPage::BattleStart()
{
	// 점검 정산 체크
	if (CheckContentsModeState(b2network::B2ContentsMode::DIMENSION))
		return;

	auto DocDimension = UB2UIDocHelper::GetDocDimension();
	if (DocDimension)
	{
		// 티켓부족
		if (DocDimension->GetDimensionTicket() < 1)
		{
			UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Dimension_StartError")),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::Confirm
				);

			return;
		}

		// 인벤토리 풀
		if (BladeIIGameImpl::GetClientDataStore().GetLocalCharacterData().IsFullTotemInventory())
		{
			UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Dimension_StartErrorTotemFull")),
				0.f,
				true,
				true,
				EUIMsgPopupButtonGroup::YesOrNo,
				FMsgPopupOnClick::CreateLambda([this](){
				
				if (auto* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this)))
				{
					ELobbyUIPages MatchingDjLegacyUIPage = HeroMgmtSubModeToLobbyUIPage(EHeroMgmtSubMode::EHMSM_Totem);
					bool bRequetAsyncLoading = BladeIIGameImpl::bAllowUIAsyncLoading && BladeIIGameImpl::bAllowUIOnDemandAsyncLoading;

					LobbyGM->DJLegacy_AsyncLoadUIPage(MatchingDjLegacyUIPage, bRequetAsyncLoading);
				}

			}));

			return;
		}

		DocDimension->SetMapID(DocDimension->GetDifficulty());

		if (auto* pGameMode = Cast<AGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			FCacheStageEssentialDataToKeep& StageData = AB2StageManager::GetCacheStageKeepEssentialData();
			FLocalCharacterData& CharDataStore = BladeIIGameImpl::GetLocalCharacterData();
			StageData.RequestStartDimension(pGameMode, DocDimension->GetDifficulty(), DocDimension->GetMapIndex(), CharDataStore.GetMainPlayerClass(), CharDataStore.GetSubPlayerClass());
		}	
	}
}

void UB2UIDimensionSelectPage::SetChangeCombatPower(int32 InPower, int32 InAttack, int32 InDefense)
{
	//auto DocDimension = UB2UIDocHelper::GetDocDimension();
	//if (!DocDimension)
	//	return;

	//FB2DimensionInfo* CurrentInfo = BladeIIGameImpl::GetTotemDataStore().GetDimensionInfo(DocDimension->GetDifficulty());
	//if (!CurrentInfo)
	//	return;

	//if (TB_HeroPower.IsValid())
	//{
	//	TB_HeroPower->SetText(FText::AsNumber(InPower));
	//	TB_HeroPower->SetColorAndOpacity(GetPowerColor(InPower, 100));
	//}
	//if (TB_RecommendPower.IsValid())
	//{
	//	TB_RecommendPower->SetText(FText::AsNumber(CurrentInfo->Recommend_power));
	//}

	//if (UIP_RecommendPower.IsValid())
	//{
	//	UIP_RecommendPower->SetInfo(InAttack, InDefense, CurrentInfo->Recommend_attack, CurrentInfo->Recommend_defense);
	//}
}