
#include "B2UIDocBattle.h"

UB2UIDocBattle::UB2UIDocBattle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bMultiple = false;
	bAnonymous = false;
	IsCenematicMode = false;
	DocType = EUIDocType::Battle;

	CurPCClass = 0;
	TagPCClass = 1;

	//ResurrectGameModeType = EResurrectGameModeType::EResurrectType_Stage;
	QTEEnableState = EQTEUIAttackEnableTimingState::None;

	NeedResurrectBuffText = false;
	ClearBattleDataForStageClear();
}

UB2UIDocBattle::~UB2UIDocBattle()
{

}

void UB2UIDocBattle::ClearBattleDataForStageClear()
{
	// 胶抛捞瘤 秉 饶 努府绢
	SelectedResurrectBuffAttack = 0;
	SelectedResurrectBuffDefense = 0;
	SelectedResurrectBuffHealth = 0;

	SET_UIDOC_FIELD_FourField(CurResurrectBuffAttackCount, 0)
	SET_UIDOC_FIELD_FourField(CurResurrectBuffDefenseCount, 0)
	SET_UIDOC_FIELD_FourField(CurResurrectBuffHealthCount, 0)
}

