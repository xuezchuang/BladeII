// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIDonationPointStepReward.h"
#include "B2UIDocHelper.h"
#include "B2UIDocDonation.h"
#include "../../Common/Event.h"


void UB2UIDonationPointStepReward::Init()
{
	Super::Init();

	if (UIP_RedDot.IsValid())
	{
		UIP_RedDot->Init();
	}
}

void UB2UIDonationPointStepReward::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	if (UIP_RedDot.IsValid())
	{
		UIP_RedDot->DestroySelf(InUIManager);
	}
}

void UB2UIDonationPointStepReward::UpdateInfo()
{
	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		EDonationPointStepState CurState = DocDonation->GetDonationPointStepState(StepReward);
		if (IMG_Icon.IsValid() && FX_Complete1.IsValid() && TB_StepName.IsValid() && UIP_RedDot.IsValid() && CP_Completion.IsValid())
		{
			switch (CurState)
			{
			case EDonationPointStepState::Completion:
				IMG_Icon->SetBrushFromMaterial(CompletionIcon.LoadSynchronous());
				FX_Complete1->SetVisibility(ESlateVisibility::Hidden);
				UIP_RedDot->SetVisibility(ESlateVisibility::Hidden);
				CP_Completion->SetVisibility(ESlateVisibility::HitTestInvisible);
				TB_StepName->SetColorAndOpacity(CompletionColor);
				break;
			case EDonationPointStepState::Incompletion:
				IMG_Icon->SetBrushFromMaterial(IncompletionIcon.LoadSynchronous());
				FX_Complete1->SetVisibility(ESlateVisibility::Hidden);
				UIP_RedDot->SetVisibility(ESlateVisibility::Hidden);
				CP_Completion->SetVisibility(ESlateVisibility::Collapsed);
				TB_StepName->SetColorAndOpacity(IncompletionColor);
				break;
			case EDonationPointStepState::Reward:
				IMG_Icon->SetBrushFromMaterial(CompletionIcon.LoadSynchronous());
				FX_Complete1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				UIP_RedDot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				CP_Completion->SetVisibility(ESlateVisibility::Collapsed);
				TB_StepName->SetColorAndOpacity(CompletionColor);
				if (ANIM_Complete.IsValid())
				{
					PlayAnimation(ANIM_Complete.Get(), 0.0f, 0);
				}
				break;
			}
		}
	}
}

void UB2UIDonationPointStepReward::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2Image, IMG_Icon);
	GET_SLOT(UTextBlock, TB_DonationPoint);
	GET_SLOT(UTextBlock, TB_StepName);
	GET_SLOT(UB2Button, BTN_Back);
	GET_SLOT(UImage, FX_Complete1);
	ANIM_Complete = GetAnimation(this, "ANIM_Complete");
	GET_SLOT(UB2UIWidgetBase, UIP_RedDot);
	GET_SLOT(UPanelWidget, CP_Completion);
}

void UB2UIDonationPointStepReward::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Back, &UB2UIDonationPointStepReward::OnClickBack);
}

void UB2UIDonationPointStepReward::UpdateStaticText()
{
	UB2UIDocDonation* DocDonation = UB2UIDocHelper::GetDocDonation();
	if (DocDonation)
	{
		if (TB_DonationPoint.IsValid())
		{
			int32 RewardPoint = DocDonation->GetDonationPointStepRewardPoint(StepReward);
			TB_DonationPoint->SetText(FText::FromString(FString::Printf(TEXT("%dP"), RewardPoint)));
		}
	}

	if (TB_StepName.IsValid())
	{
		TB_StepName->SetText(StepName);
	}
}

void UB2UIDonationPointStepReward::OnClickBack()
{
	OpenDonationPointStepRewardInfoPopupClass<int32>::GetInstance().Signal(StepReward);
}

#if WITH_EDITOR
void UB2UIDonationPointStepReward::EditorOnPostLoad()
{//俊叼磐老锭 秦淋
	CompletionIcon.LoadSynchronous();
	IncompletionIcon.LoadSynchronous();
}
#endif
