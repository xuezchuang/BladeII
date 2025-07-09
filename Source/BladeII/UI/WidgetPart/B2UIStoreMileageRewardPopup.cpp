#include "BladeII.h"
#include "B2UIStoreMileageRewardPopup.h"
#include "BladeIIUtil.h"
#include "B2UIStoreMileageRewardPopupSlot.h"
#include "B2UIDocHelper.h"


void UB2UIStoreMileageRewardPopup::Init()
{
	Super::Init();
}

void UB2UIStoreMileageRewardPopup::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_Summary);
	GET_SLOT(UTextBlock, TB_Caution1);
	GET_SLOT(UTextBlock, TB_Caution2);
	GET_SLOT(UHorizontalBox, HB_RewardList);
	GET_SLOT(UButton, BTN_Close);
}

void UB2UIStoreMileageRewardPopup::UpdateStaticText()
{
	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_STORE), FString(TEXT("Store_MileageGuide_Title"))));
	if (TB_Summary.IsValid())
		TB_Summary->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_STORE), FString(TEXT("Store_MileageGuide_Guide02"))));
	if (TB_Caution1.IsValid())
		TB_Caution1->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_STORE), FString(TEXT("Store_MileageGuide_Caution01"))));
	if (TB_Caution2.IsValid())
		TB_Caution2->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_STORE), FString(TEXT("Store_MileageGuide_Caution02"))));
}

void UB2UIStoreMileageRewardPopup::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIStoreMileageRewardPopup::OnClickBtnClose);
}

void UB2UIStoreMileageRewardPopup::SetRewardPopup(int32 MileageType)
{
	CurrentMileageType = MileageType;

	if (HB_RewardList.IsValid())
		HB_RewardList->ClearChildren();

	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		TArray<EUIStoreMileageRewardState> RewardState;
		RewardState = DocStore->GetMileageRewardState();

		for (int Idx = 0; Idx < RewardState.Num(); Idx++)
		{
			auto* CreatedWidget = CreateWidget<UB2UIStoreMileageRewardPopupSlot>(GetOwningPlayer(), RewardPopupSlot);

			if (CreatedWidget)
			{
				CreatedWidget->Init();
				CreatedWidget->CreateRewardBox(MileageType, Idx);
				CreatedWidget->SetRewardState(RewardState[Idx], DocStore->GetEnableMileageRewardState(Idx));

				HB_RewardList->AddChild(CreatedWidget);
			}
		}
	}
}

void UB2UIStoreMileageRewardPopup::OnClickBtnClose()
{
	CloseMe();
}