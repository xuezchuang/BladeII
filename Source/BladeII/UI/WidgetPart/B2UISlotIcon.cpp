#include "B2UISlotIcon.h"
#include "B2UIManager.h"

UB2UISlotIcon::UB2UISlotIcon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bClickable = true;
}

void UB2UISlotIcon::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_Area)
	if (BTN_Area.IsValid())
	{
		//DefaultClickMethod = BTN_Area->ClickMethod;
		//DefaultTouchMethod = BTN_Area->TouchMethod;
	}
	GET_SLOT(UImage, IMG_Icon)
}

void UB2UISlotIcon::UpdateStaticText()
{
}

void UB2UISlotIcon::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UISlotIcon::OnClickBtnArea)
}

void UB2UISlotIcon::SetIcon(UTexture2D* Image)
{
	if (IMG_Icon.IsValid())
		IMG_Icon->SetBrushFromTexture(Image);
}

void UB2UISlotIcon::SetIcon(FString ImagePath)
{
	SetIcon(UB2UIManager::GetInstance()->LoadTexture2D(*(ImagePath)));
}

void UB2UISlotIcon::SetIcon(UMaterialInterface* InMaterial)
{
	if (IMG_Icon.IsValid())
		IMG_Icon->SetBrushFromMaterial(InMaterial);
}

UMaterialInterface* UB2UISlotIcon::GetIconMaterial() const
{
	return (IMG_Icon.IsValid() ? Cast<UMaterialInterface>(IMG_Icon->GetBrush().GetResourceObject()) : NULL);
}

void UB2UISlotIcon::ShowIcon(const bool bShow)
{
	if (IMG_Icon.IsValid())
	{
		bShow ?
			IMG_Icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible) :
			IMG_Icon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2UISlotIcon::SetIconDisable(const bool bDisabled)
{
	if (IMG_Icon.IsValid())
		IMG_Icon->SetIsEnabled(!bDisabled);
}

void UB2UISlotIcon::SetPreciseClickAndTab(bool bToSetPrecise)
{
	////促弗 Widget狼 Child
	//if (BTN_Area.IsValid())
	//{
	//	BTN_Area->ClickMethod = bToSetPrecise ? EButtonClickMethod::PreciseClick : DefaultClickMethod;
	//	BTN_Area->TouchMethod = bToSetPrecise ? EButtonTouchMethod::PreciseTap : DefaultTouchMethod;

	//	BTN_Area->OnChangedClickMethod();
	//}
}

void UB2UISlotIcon::SynchronizeProperties()
{
	Super::SynchronizeProperties();
#if WITH_EDITOR
	if (IMG_Icon == nullptr)
		CacheAssets();
	SetIcon(DefaultImage);
#endif	// WITH_EDITOR
}


//====================================================================================
//									Click Handler
//====================================================================================
void UB2UISlotIcon::OnClickBtnArea()
{
	OnSlotIconClickDelgate.ExecuteIfBound(this);
}
