#include "B2UISelectiveLotteryConsumeItemResultPopup.h"
#include "B2UISelectiveLotteryPopUp.h"

void UB2UISelectiveLotteryConsumeItemResultPopup::OpenPopup(int32 ItemRefId, int32 Num)
{
	SetItemData(ItemRefId, Num);
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	OnOpenedPopup();
}

void UB2UISelectiveLotteryConsumeItemResultPopup::CacheAssets()
{
	GET_SLOT(UB2UISelectiveLotteryItemSlot, UIP_ItemResult);
	if (UIP_ItemResult.IsValid())
		UIP_ItemResult->Init();
	GET_SLOT(UB2Button, BTN_Close);
}

void UB2UISelectiveLotteryConsumeItemResultPopup::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UISelectiveLotteryConsumeItemResultPopup::CloseMe);
}

void UB2UISelectiveLotteryConsumeItemResultPopup::SetItemData(int32 ItemRefId, int32 Num)
{
	if (UIP_ItemResult.IsValid())
		UIP_ItemResult->SetItemData(ItemRefId, Num);
}

void UB2UISelectiveLotteryConsumeItemResultPopup::CloseMe()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
