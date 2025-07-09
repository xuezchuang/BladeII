#include "BladeII.h"
#include "B2UIDocUICondition.h"

UB2UIDocUICondition::UB2UIDocUICondition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bMultiple = false;
	bAnonymous = false;
	DocType = EUIDocType::UICondition;
	PreCombatArinConsultIgnoreCount = 0;
}

UB2UIDocUICondition::~UB2UIDocUICondition()
{

}
