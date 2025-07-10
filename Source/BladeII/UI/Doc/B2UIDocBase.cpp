
#include "B2UIDocBase.h"

UB2UIDocBase::UB2UIDocBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UB2UIDocBase::~UB2UIDocBase()
{

}

void UB2UIDocBase::Destroy()
{
	RemoveFromRoot();
	ConditionalBeginDestroy();
}
