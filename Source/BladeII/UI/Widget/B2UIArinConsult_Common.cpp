#include "B2UIArinConsult_Common.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIArinConsult_PreCombat.h"

UB2UIArinConsult_Common::UB2UIArinConsult_Common(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SubType = EArinConsultType::ARCT_End;
}

void UB2UIArinConsult_Common::Init()
{
	Super::Init();

}

void UB2UIArinConsult_Common::SetSubType(EArinConsultType InType)
{
	SubType = InType;

	UpdateStaticText(); // StaticText 지만 SubType 세팅 이후의 static..
}

void UB2UIArinConsult_Common::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_ArinConsultTitle);
	GET_SLOT(UB2Button, BTN_Close);
}

void UB2UIArinConsult_Common::UpdateStaticText()
{
	if (TB_ArinConsultTitle.IsValid())
	{
		TB_ArinConsultTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL,
				(
				SubType == EArinConsultType::ARCT_PostDefeatGettingStrong ? 
				TEXT("ArinConsultTitle_PostDefeatGettingStrong") :
				TEXT("ArinConsultTitle_PreCombatConsulting")
				)
			));
	}
}

void UB2UIArinConsult_Common::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIArinConsult_Common::OnClickBtnClose);

}

void UB2UIArinConsult_Common::OnClickBtnClose()
{
	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();

	switch (SubType)
	{
	case EArinConsultType::ARCT_PreCombatConsulting:
		if (UIMgrInst)
		{ // 그냥 닫을 땐 무시하는 걸로 치고 향후 몇번 스킵하도록 처리.
			UB2UIArinConsult_PreCombat* OwnerUI = UIMgrInst->GetUI<UB2UIArinConsult_PreCombat>(UIFName::ArinConsult_PreCombat);
			if (OwnerUI)
			{
				OwnerUI->CloseAndIgnore();
			}
		}
		break;
	case EArinConsultType::ARCT_PostDefeatGettingStrong:
		if (UIMgrInst)
		{
			UIMgrInst->CloseUI(UIFName::ArinConsult_PostDefeat);
		}
		break;
	}

}
