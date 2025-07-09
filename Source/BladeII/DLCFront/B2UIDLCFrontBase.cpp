#include "BladeII.h"
#include "B2UIDLCFrontBase.h"

UB2UIDLCFrontBase::UB2UIDLCFrontBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UB2UIDLCFrontBase::Init()
{
	CacheAssets();
	UpdateStaticText();
	BindDelegates();
}

void UB2UIDLCFrontBase::DestroySelf()
{
	DestroySelfCommon();
}
