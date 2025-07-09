#include "BladeII.h"
#include "B2UIStoreMileage.h"
#include "B2UIDocHelper.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIStoreMileage::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_Mileage);
	GET_SLOT(UTextBlock, TB_MileageValue);
	GET_SLOT(UProgressBar, PB_Mileage);
	GET_SLOT(UTextBlock, TB_MileageS)
	GET_SLOT(UTextBlock, TB_MileageA)
	GET_SLOT(UTextBlock, TB_MileageB)
	GET_SLOT(UTextBlock, TB_MaxValue01);
	GET_SLOT(UTextBlock, TB_MaxValue02);
	GET_SLOT(UTextBlock, TB_MaxValue03);
	GET_SLOT(UB2Button, BTN_Box01Area);
	GET_SLOT(UB2Button, BTN_Box02Area);
	GET_SLOT(UB2Button, BTN_Box03Area);

	GET_SLOT(UOverlay, O_MileageS);
	GET_SLOT(UOverlay, O_MileageA);
	GET_SLOT(UOverlay, O_MileageB);
}

void UB2UIStoreMileage::UpdateStaticText()
{
	if (TB_Mileage.IsValid())
		TB_Mileage->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_MileageGuide_Title")));

	if (TB_MileageS.IsValid())
		TB_MileageS->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Mileage_SClass")));

	if (TB_MileageA.IsValid())
		TB_MileageA->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Mileage_AClass")));

	if (TB_MileageB.IsValid())
		TB_MileageB->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Mileage_BClass")));
}

void UB2UIStoreMileage::BindDelegates()
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		MileageChangedHandler = DocStore->OnMileageChanged.AddUObject(this, &UB2UIStoreMileage::UpdateMileage);
	}
}

void UB2UIStoreMileage::DestroySelf(class UB2UIManager* InUIManager)
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		DocStore->OnMileageChanged.Remove(MileageChangedHandler);
	}

	Super::DestroySelf(InUIManager);
}

UB2Button* UB2UIStoreMileage::GetBoxButtonPtr(EUIStoreMileageGrade BoxGrade)
{
	switch (BoxGrade)
	{
	case EUIStoreMileageGrade::BClass:
		return BTN_Box01Area.Get();
	case EUIStoreMileageGrade::AClass:
		return BTN_Box02Area.Get();
	case EUIStoreMileageGrade::SClass:
		return BTN_Box03Area.Get();
	default:
		return nullptr;
	}
}

void UB2UIStoreMileage::UpdateMileage()
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		SetMileageData(DocStore->GetCurrentMileageType());
	}
}

void UB2UIStoreMileage::SetMileageData(int32 MileageType)
{
	auto* SClassData = BladeIIGameImpl::GetClientDataStore().GetStoreMileageData().Find(FStoreMileageData::GetHashkey(MileageType, static_cast<uint32>(EUIStoreMileageGrade::SClass)));
	auto* AClassData = BladeIIGameImpl::GetClientDataStore().GetStoreMileageData().Find(FStoreMileageData::GetHashkey(MileageType, static_cast<uint32>(EUIStoreMileageGrade::AClass)));
	auto* BClassData = BladeIIGameImpl::GetClientDataStore().GetStoreMileageData().Find(FStoreMileageData::GetHashkey(MileageType, static_cast<uint32>(EUIStoreMileageGrade::BClass)));

	int32 SMaxMileagePoint = SClassData ? SClassData->MaxMileage : 0;
	int32 AMaxMileagePoint = AClassData ? AClassData->MaxMileage : 0;
	int32 BMaxMileagePoint = BClassData ? BClassData->MaxMileage : 0;

	int32 CurrentMileagePoint = 0;
	int32 MaxMileagePoint = 0;

	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		DocStore->SetCurrentMileageType(MileageType);

		CurrentMileagePoint = DocStore->GetMileagePoint(MileageType);

		if (PB_Mileage.IsValid())
			PB_Mileage->SetPercent(CurrentMileagePoint / (float)SMaxMileagePoint);

		// Box 위치 변경
		UHorizontalBoxSlot* MileageSBoxSlot = Cast<UHorizontalBoxSlot>(O_MileageS->Slot);
		UHorizontalBoxSlot* MileageABoxSlot = Cast<UHorizontalBoxSlot>(O_MileageA->Slot);
		UHorizontalBoxSlot* MileageBBoxSlot = Cast<UHorizontalBoxSlot>(O_MileageB->Slot);

		FSlateChildSize MileageBSize;
		MileageBSize.Value = (float)BMaxMileagePoint / (float)SMaxMileagePoint;
		MileageBSize.SizeRule = ESlateSizeRule::Fill;

		FSlateChildSize MileageASize;
		MileageASize.Value = (float)(AMaxMileagePoint - BMaxMileagePoint) / (float)SMaxMileagePoint;
		MileageASize.SizeRule = ESlateSizeRule::Fill;

		FSlateChildSize MileageSSize;
		MileageSSize.Value = 1.0f - MileageBSize.Value - MileageASize.Value;
		MileageSSize.SizeRule = ESlateSizeRule::Fill;

		MileageBBoxSlot->SetSize(FSlateChildSize(MileageBSize));
		MileageABoxSlot->SetSize(FSlateChildSize(MileageASize));
		MileageSBoxSlot->SetSize(FSlateChildSize(MileageSSize));

		int32 LastReceivedRewardClassIndex = DocStore->GetMileageReceivedIndex(MileageType);

		TArray<EUIStoreMileageRewardState> RewardStates;
		RewardStates.Empty(3);
		RewardStates.Add((BClassData && BClassData->MaxMileage <= CurrentMileagePoint)
			? ((static_cast<int32>(EUIStoreMileageGrade::BClass) <= LastReceivedRewardClassIndex) ? EUIStoreMileageRewardState::Rewarded : EUIStoreMileageRewardState::Reached)
			: EUIStoreMileageRewardState::NotReached);
		RewardStates.Add((AClassData && AClassData->MaxMileage <= CurrentMileagePoint)
			? ((static_cast<int32>(EUIStoreMileageGrade::AClass) <= LastReceivedRewardClassIndex) ? EUIStoreMileageRewardState::Rewarded : EUIStoreMileageRewardState::Reached)
			: EUIStoreMileageRewardState::NotReached);
		RewardStates.Add((SClassData && SClassData->MaxMileage <= CurrentMileagePoint)
			? ((static_cast<int32>(EUIStoreMileageGrade::SClass) <= LastReceivedRewardClassIndex) ? EUIStoreMileageRewardState::Rewarded : EUIStoreMileageRewardState::Reached)
			: EUIStoreMileageRewardState::NotReached);

		SetReachedState(EUIStoreMileageGrade::BClass, RewardStates[0]);
		SetReachedState(EUIStoreMileageGrade::AClass, RewardStates[1]);
		SetReachedState(EUIStoreMileageGrade::SClass, RewardStates[2]);

		DocStore->SetMileageRewardState(RewardStates);
	}

	if (CurrentMileagePoint < BMaxMileagePoint)
		MaxMileagePoint = BMaxMileagePoint;
	else if (CurrentMileagePoint < AMaxMileagePoint)
		MaxMileagePoint = AMaxMileagePoint;
	else
		MaxMileagePoint = SMaxMileagePoint;

	if (TB_MileageValue.IsValid())
		TB_MileageValue->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Mileage_Value")), CurrentMileagePoint, MaxMileagePoint));	

	if (TB_MaxValue01.IsValid())
		TB_MaxValue01->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Bracket")), BMaxMileagePoint));

	if (TB_MaxValue02.IsValid())
		TB_MaxValue02->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Bracket")), AMaxMileagePoint));

	if (TB_MaxValue03.IsValid())
		TB_MaxValue03->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Bracket")), SMaxMileagePoint));
}
