#include "B2UIBattleVignette.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2UIDocBattle.h"

UB2UIBattleVignette::UB2UIBattleVignette(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UB2UIBattleVignette::CacheAssets()
{
	//GET_SLOT(UImage,	IMG_Vignette)
}

void UB2UIBattleVignette::UpdateStaticText()
{
}

void UB2UIBattleVignette::BindDelegates()
{
}

void UB2UIBattleVignette::BindDoc(UB2UIDocBattle* DocBattle)
{
	UnbindDoc();
	SetDoc(DocBattle);
	if (!DocBattle) return;

	DocBattle->OnCurrPCInCriticalConditionChanged.AddUObject(this, &UB2UIBattleVignette::OnChangedCurrPCInCriticalCondition);
}

void UB2UIBattleVignette::UnbindDoc()
{
	auto DocBattle = Cast<UB2UIDocBattle>(GetDoc());
	if (DocBattle)
	{
		DocBattle->OnCurrPCInCriticalConditionChanged.RemoveAll(this);
	}
	
	SetDoc(nullptr);
}

void UB2UIBattleVignette::SetWarningHP(bool bWarning)
{
	SetWarningHP_BP(bWarning);
}


//====================================================================================
//									Click Handler
//====================================================================================


//====================================================================================
//									Doc Delegate
//====================================================================================
void UB2UIBattleVignette::OnChangedCurrPCInCriticalCondition(class UB2UIDocBase* Sender, bool CurrPCInCriticalCondition, bool PrevCurrPCInCriticalCondition)
{
	SetWarningHP(CurrPCInCriticalCondition);
}
