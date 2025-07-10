#include "B2UIMsgPopupCenter.h"
#include "B2UIManager.h"


//BP에서 MsgPopupBase를 바로 받아서 처리 해도 되지만, 추후 확장을 대비해서 별도의 클래스 마련

UB2UIMsgPopupCenter::UB2UIMsgPopupCenter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UB2UIMsgPopupCenter::CacheAssets()
{
	Super::CacheAssets();
}

void UB2UIMsgPopupCenter::UpdateStaticText()
{
	Super::UpdateStaticText();
}

void UB2UIMsgPopupCenter::BindDelegates()
{
	Super::BindDelegates();
}
