#include "B2UIMsgPopupInput.h"
#include "B2UIManager.h"

UB2UIMsgPopupInput::UB2UIMsgPopupInput(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UB2UIMsgPopupInput::OnOpen(bool RightNow)
{
	Super::OnOpen(RightNow);

	if (ETB_Input.IsValid())
		ETB_Input->SetKeyboardFocus();
}