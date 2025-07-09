#include "BladeII.h"
#include "B2UIArinConsult_PreCombat.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2StageInfo.h"
#include "Event.h"
#include "B2ClientDataStore.h"


#include "BladeIIGameImpl.h"

UB2UIArinConsult_PreCombat::UB2UIArinConsult_PreCombat(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MainSubject = EArinConsultPreCombat_MainSubject::ARCPCMS_None;
}

void UB2UIArinConsult_PreCombat::Init()
{
	Super::Init();
}

void UB2UIArinConsult_PreCombat::BindDocAuto()
{
	UnbindDoc();

	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	SetDoc(DocBS);
	if (DocBS)
	{
		MainSubject = DocBS->GetArinConsultingSubject();
		BII_CHECK(MainSubject != EArinConsultPreCombat_MainSubject::ARCPCMS_None);

		UB2StageInfo* StageInfoTable = StaticFindStageInfo();
		const FServerStageID PendingServerStageId = DocBS->GetServerStageId();
		const int32 PendingClientStageId = DocBS->GetClientStageId();
		const int32 PendingChapterNum = StageInfoTable ? StageInfoTable->GetChapterNumByClientStageId(PendingClientStageId) : 0;
		const int32 PendingSubChapterNum = StageInfoTable ? StageInfoTable->GetSubChapterNumByClientStageId(PendingClientStageId) : 0;
		BII_CHECK(PendingChapterNum > 0 && PendingSubChapterNum > 0);

		UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
		const FText UserNickName = DocUser ? DocUser->GetUserNickName() : FText::FromString(TEXT("User"));
		
		if (TB_ConsultMessage.IsValid())
		{
			FText StageName;

			if (StageInfoTable && StageInfoTable->IsInvokedGameMode(PendingClientStageId, EB2GameMode::CounterDungeon))
				StageName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Stage_CounterDG"));
			else
				StageName = FText::FromString(FString::Printf(TEXT("%d-%d"), PendingChapterNum, PendingSubChapterNum));

			TB_ConsultMessage->SetText(				
				MainSubject == EArinConsultPreCombat_MainSubject::ARCPCMS_Skill
					?
					FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ArinConsult_ConsultingMessage_Skill")), UserNickName) 
					:
					(
					MainSubject == EArinConsultPreCombat_MainSubject::ARCPCMS_Defense
					?
						FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ArinConsult_ConsultingMessage_Defense")), StageName)
						:
						FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ArinConsult_ConsultingMessage_Attack")), StageName)
					)
				);
		}

		if (TB_ItemOrSkillEnhance.IsValid())
		{
			TB_ItemOrSkillEnhance->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL,
				MainSubject == EArinConsultPreCombat_MainSubject::ARCPCMS_Skill ?
					TEXT("ArinConsult_EnhanceTarget_Skill") :
						(MainSubject == EArinConsultPreCombat_MainSubject::ARCPCMS_Defense ?
						TEXT("ArinConsult_EnhanceTarget_Protection") : TEXT("ArinConsult_EnhanceTarget_Weapon"))
				));
		}

		if (BTN_ItemOrSkillEnhance.IsValid())
		{
			BTN_ItemOrSkillEnhance->SetStyle(
				MainSubject == EArinConsultPreCombat_MainSubject::ARCPCMS_Skill ?
					ButtonStyle_SkillEnhance :
						(MainSubject == EArinConsultPreCombat_MainSubject::ARCPCMS_Defense ?
						ButtonStyle_ProtectionEnhance : ButtonStyle_WeaponEnhance)
				);
		}
	}
}

void UB2UIArinConsult_PreCombat::UnbindDoc()
{
	SetDoc(nullptr);
}

void UB2UIArinConsult_PreCombat::SetConsultMessage(FText Message)
{
	if (TB_ConsultMessage.IsValid())
		TB_ConsultMessage->SetText(Message);
}

void UB2UIArinConsult_PreCombat::CacheAssets()
{
	GET_SLOT(UB2UIArinConsult_Common, UIP_ACCommon);
	if (UIP_ACCommon.IsValid())
	{
		UIP_ACCommon->Init();
		UIP_ACCommon->SetSubType(EArinConsultType::ARCT_PreCombatConsulting);
	}

	GET_SLOT(UTextBlock, TB_Arin);
	GET_SLOT(UTextBlock, TB_ConsultMessage);
	
	GET_SLOT(UB2Button, BTN_ItemOrSkillEnhance);
	GET_SLOT(UTextBlock, TB_ItemOrSkillEnhance);
	
	GET_SLOT(UB2Button, BTN_ProceedToCombat);
	GET_SLOT(UTextBlock, TB_ProceedToCombat);
}

void UB2UIArinConsult_PreCombat::UpdateStaticText()
{
	if (TB_Arin.IsValid())
	{
		TB_Arin->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ArinConsult_MissySexyArin")));
	}
	if (TB_ProceedToCombat.IsValid())
	{
		TB_ProceedToCombat->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ArinConsult_ProceedToCombat")));
	}
}

void UB2UIArinConsult_PreCombat::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemOrSkillEnhance, &UB2UIArinConsult_PreCombat::OnClickBtnItemOrSkillEnhance)
	BIND_CLICK_FUNC_TO_BTN(BTN_ProceedToCombat, &UB2UIArinConsult_PreCombat::OnClickBtnProceedToCombat)
}

void UB2UIArinConsult_PreCombat::OnClickBtnItemOrSkillEnhance()
{
	FLocalCharacterData& LocalCharData = BladeIIGameImpl::GetLocalCharacterData();

	if (MainSubject == EArinConsultPreCombat_MainSubject::ARCPCMS_Attack || MainSubject == EArinConsultPreCombat_MainSubject::ARCPCMS_Defense)
	{
		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
		
		// 실제로 스탯이 약한 애가 메인이냐 서브냐에 따라서도 다르게 해야 할 수도.. AB2LobbyGameMode::GameStageCheckForArinConsulting. 근데 단순히 누구 스탯이 더 높은가로 정하는 것도 여러 상황을 따졌을 때 정확하지는 않고 좀 애매하군.
		const EPCClass MainCharClass = LocalCharData.GetMainPlayerClass();
		LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Signal(MainCharClass);

		LobbyExternalSetInventoryTabClass<EItemInvenType,bool>::GetInstance().Signal(
			(MainSubject == EArinConsultPreCombat_MainSubject::ARCPCMS_Attack) ? EItemInvenType::EIIVT_Weapon : EItemInvenType::EIIVT_Protection,
			false
			);
	}
	else if (MainSubject == EArinConsultPreCombat_MainSubject::ARCPCMS_Skill)
	{
		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Skill);

		// 스킬포인트는 적어도 없는 쪽 캐릭터로는 안가도록 할 수 있겠다.
		const EPCClass MainCharClass = LocalCharData.GetMainPlayerClass();
		const EPCClass TagCharClass = LocalCharData.GetSubPlayerClass();

		const int32 MainSP = LocalCharData.GetCharacterLeftSkillPoint(MainCharClass);
		const int32 TagSP = LocalCharData.GetCharacterLeftSkillPoint(TagCharClass);

		const EPCClass FinalSkillTabClass = (MainSP <= 0 && TagSP > 0) ? TagCharClass : MainCharClass;
		LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Signal(FinalSkillTabClass);
	}

	CloseMe();
}

#include "B2StageManager.h"
void UB2UIArinConsult_PreCombat::OnClickBtnProceedToCombat()
{
	auto* DocBS = UB2UIDocHelper::GetDocBattleStage();
	auto* DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBS && DocBattle)
	{ 
		if (bSetFromCounterDungeon)
		{
			auto& StageData = AB2StageManager::GetCacheStageKeepEssentialData();
			StageData.RequestStartCounterDungeon(DocBS->GetServerStageId(), IntToPCClass(DocBattle->GetCurPCClass()), IntToPCClass(DocBattle->GetTagPCClass()));
		}
		else {// AB2LobbyGameMode::GoGameStageInfo 에서 세팅된 StageId 그대로..
			UB2UIDocUICondition* DocCon = UB2UIDocHelper::GetDocUICondition();
			StartGameStageFromLobbyClass<int32, EStageDifficulty, const TArray<EStageBuffType>&, bool>::GetInstance().Signal(DocBS->GetClientStageId(), DocBS->GetStageDifficulty(), DocBS->GetSelectedStageBuffs(), (DocCon ? DocCon->GetbUserSelectedSkipStory() : false));
		}
	}

	CloseAndIgnore();
}

void UB2UIArinConsult_PreCombat::CloseMe()
{
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		UIMgrInst->CloseUI(UIFName::ArinConsult_PreCombat);
	}
}

void UB2UIArinConsult_PreCombat::CloseAndIgnore()
{
	// 향후 일정 횟수는 스킵하도록 UIDocUICondition 쪽 세팅.
	UB2UIDocUICondition* DocCondition = UB2UIDocHelper::GetDocUICondition();
	if (GConfig && DocCondition)
	{
		int32 IgnoreSkipCount = 0;
		GConfig->GetInt(TEXT("/Script/BladeII.B2UIManager"), TEXT("PreCombatArinConsultingIgnoreSkipCount"), IgnoreSkipCount, GGameIni);
		DocCondition->SetPreCombatArinConsultIgnoreCount(IgnoreSkipCount);
	}

	CloseMe();
}