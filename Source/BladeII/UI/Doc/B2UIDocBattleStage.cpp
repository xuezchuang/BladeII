
#include "B2UIDocBattleStage.h"

UB2UIDocBattleStage::UB2UIDocBattleStage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bMultiple = false;
	bAnonymous = false;
	DocType = EUIDocType::BattleStage;
	RepeatBattleBoostMaxCount = 99999999;
}

UB2UIDocBattleStage::~UB2UIDocBattleStage()
{

}
