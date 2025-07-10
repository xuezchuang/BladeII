#include "B2UIArinConsult_PostDefeat.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "Event.h"
#include "B2ClientDataStore.h"

UB2UIArinConsult_PostDefeat::UB2UIArinConsult_PostDefeat(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UB2UIArinConsult_PostDefeat::Init()
{
	Super::Init();
}

void UB2UIArinConsult_PostDefeat::BindDocAuto()
{
	UnbindDoc();

	UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
	SetDoc(DocBS);
	if (DocBS)
	{
	
	}
}

void UB2UIArinConsult_PostDefeat::UnbindDoc()
{
	SetDoc(nullptr);
}

void UB2UIArinConsult_PostDefeat::CacheAssets()
{
	GET_SLOT(UB2UIArinConsult_Common, UIP_ACCommon);
	if (UIP_ACCommon.IsValid())
	{
		UIP_ACCommon->Init();
		UIP_ACCommon->SetSubType(EArinConsultType::ARCT_PostDefeatGettingStrong);
	}

	GET_SLOT(UTextBlock, TB_Arin);
	GET_SLOT(UTextBlock, TB_ConsultMessage);
	
	GET_SLOT(UB2Button, BTN_ItemEnhance);
	GET_SLOT(UTextBlock, TB_ItemEnhance);
	
	GET_SLOT(UB2Button, BTN_SkillEnhance);
	GET_SLOT(UTextBlock, TB_SkillEnhance);

	GET_SLOT(UB2Button, BTN_ItemPurchase);
	GET_SLOT(UTextBlock, TB_ItemPurchase);
}

void UB2UIArinConsult_PostDefeat::UpdateStaticText()
{
	if (TB_Arin.IsValid())
	{
		TB_Arin->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ArinConsult_MissySexyArin")));
	}
	if (TB_ConsultMessage.IsValid())
	{
		TB_ConsultMessage->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ArinConsult_PostDefeat_TryAgainMessage")));
	}
	if (TB_ItemEnhance.IsValid())
	{
		TB_ItemEnhance->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ArinConsult_EnhanceItem")));
	}
	if (TB_SkillEnhance.IsValid())
	{
		TB_SkillEnhance->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ArinConsult_EnhanceSkill")));
	}
	if (TB_ItemPurchase.IsValid())
	{
		TB_ItemPurchase->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ArinConsult_PurchaseItem")));
	}
}

void UB2UIArinConsult_PostDefeat::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemEnhance, &UB2UIArinConsult_PostDefeat::OnClickBtnItemEnhance)
	BIND_CLICK_FUNC_TO_BTN(BTN_SkillEnhance, &UB2UIArinConsult_PostDefeat::OnClickBtnSkillEnhance)
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemPurchase, &UB2UIArinConsult_PostDefeat::OnClickBtnItemPurchase)
}

void UB2UIArinConsult_PostDefeat::OnClickBtnItemEnhance()
{
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);

	CloseMe();
}

void UB2UIArinConsult_PostDefeat::OnClickBtnSkillEnhance()
{
	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Skill);

	CloseMe();
}

void UB2UIArinConsult_PostDefeat::OnClickBtnItemPurchase()
{
	DevShowNotReadyYetMessage();

	//CloseMe(); 기능 준비되면 Close 도 같이 되도록..
}

void UB2UIArinConsult_PostDefeat::CloseMe()
{
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	if (UIMgrInst)
	{
		UIMgrInst->CloseUI(UIFName::ArinConsult_PostDefeat);
	}
}