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

	UpdateStaticText(); // StaticText 瘤父 SubType 技泼 捞饶狼 static..
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
		{ // 弊成 摧阑 订 公矫窍绰 吧肺 摹绊 氢饶 割锅 胶诺窍档废 贸府.
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
