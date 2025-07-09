#include "BladeII.h"
#include "B2UIStore.h"
#include "B2UIStoreItemSlotIcon.h"

void UB2UIStoreItemSlotIcon::Init()
{
	Super::Init();
}

void UB2UIStoreItemSlotIcon::CacheAssets()
{
	GET_SLOT(UImage, IMG_Icon);
	GET_SLOT(UOverlay, OV_Effect_1);
	GET_SLOT(UOverlay, OV_Effect_2);
	GET_SLOT(UOverlay, OV_Effect_3);
}

void UB2UIStoreItemSlotIcon::BindDelegates()
{

}

void UB2UIStoreItemSlotIcon::SetIcon(UMaterialInterface* InMaterial)
{
	if (IMG_Icon.IsValid())
		IMG_Icon->SetBrushFromMaterial(InMaterial);
}

void UB2UIStoreItemSlotIcon::SetEffect(int32 EffecNum)
{
	if (OV_Effect_1.IsValid())
		OV_Effect_1->SetVisibility(ESlateVisibility::Collapsed);
	if (OV_Effect_2.IsValid())
		OV_Effect_2->SetVisibility(ESlateVisibility::Collapsed);
	if (OV_Effect_3.IsValid())
		OV_Effect_3->SetVisibility(ESlateVisibility::Collapsed);

	switch (static_cast<ESTORE_SLOT_EFFECT_TYPE>(EffecNum))
	{
	case  ESTORE_SLOT_EFFECT_TYPE::EFFECT_1:
		if (OV_Effect_1.IsValid())
			OV_Effect_1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		break;

	case  ESTORE_SLOT_EFFECT_TYPE::EFFECT_2:
		if (OV_Effect_2.IsValid())
			OV_Effect_2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		break;
	case  ESTORE_SLOT_EFFECT_TYPE::EFFECT_3:
		if (OV_Effect_3.IsValid())
			OV_Effect_3->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		break;

	default:
		break;
	}
}
