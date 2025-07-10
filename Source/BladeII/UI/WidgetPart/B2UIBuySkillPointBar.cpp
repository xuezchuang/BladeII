#include "B2UIBuySkillPointBar.h"
#include "Event.h"
#include "BladeIILocText.h"

/*
void UB2UIBuySkillPointBar::UpdateAmountPerMaxText()
{
	if (TB_AmountPerMax.IsValid())
	{
		if (MaxNum > 0)
			TB_AmountPerMax->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_BuySkillPoint_Amount")), FText::AsNumber(CurrentNum), FText::AsNumber(MaxNum)));
		else
			TB_AmountPerMax->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_BuySkillPoint_Max")));
	}
}*/

void UB2UIBuySkillPointBar::SetMaxNum(int32 InMaxNum)
{
	MaxNum = InMaxNum;

	//UpdateAmountPerMaxText();

	if (BTN_Decrease.IsValid())
		BTN_Decrease->SetIsEnabled(MaxNum != 0);

	if (BTN_Increase.IsValid())
		BTN_Increase->SetIsEnabled(MaxNum != 0);

	if (BTN_SetMax.IsValid())
		BTN_SetMax->SetIsEnabled(MaxNum != 0);
}

void UB2UIBuySkillPointBar::SetNum(int32 InNum)
{
	CurrentNum = FMath::Clamp(InNum, 1, MaxNum);

	if (S_SkillBuySlider.IsValid())
		S_SkillBuySlider->SetValue(static_cast<float>(CurrentNum) / static_cast<float>(MaxNum));

	if (BTN_SetMax.IsValid())
		BTN_SetMax->SetIsEnabled(CurrentNum == MaxNum ? false : true);


	//UpdateAmountPerMaxText();		

	ChangeBuySkillPointClass<>::GetInstance().Signal();

}

void UB2UIBuySkillPointBar::SetSlider(float InValue)
{
	int32 SliderNum = FMath::RoundHalfToZero(InValue * MaxNum);

	SetNum(SliderNum);
}

void UB2UIBuySkillPointBar::SetCharacterName(const FText& Name)
{
	if (TB_CharacterName.IsValid())
		TB_CharacterName->SetText(Name);
}

void UB2UIBuySkillPointBar::CacheAssets()
{
	GET_SLOT(UTextBlock, TB_CharacterName);
	
	GET_SLOT(UButton, BTN_Decrease);
	GET_SLOT(UButton, BTN_Increase);
	GET_SLOT(UButton, BTN_SetMax);

	GET_SLOT(USlider, S_SkillBuySlider);
}

void UB2UIBuySkillPointBar::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Decrease, &UB2UIBuySkillPointBar::OnClickBtnDecrease);
	BIND_CLICK_FUNC_TO_BTN(BTN_Increase, &UB2UIBuySkillPointBar::OnClickBtnIncrease);
	BIND_CLICK_FUNC_TO_BTN(BTN_SetMax, &UB2UIBuySkillPointBar::OnClickBtnSetMax);
	S_SkillBuySlider->OnValueChanged.AddDynamic(this, &UB2UIBuySkillPointBar::SetSlider);
	/*if (S_SkillBuySlider.IsValid())
		S_SkillBuySlider->OnValueChanged = FOnFloatValueChangedEvent::CreateLambda([]() {});*/
}

void UB2UIBuySkillPointBar::OnClickBtnDecrease()
{
	
	SetNum(CurrentNum - 1);
}

void UB2UIBuySkillPointBar::OnClickBtnIncrease()
{
	SetNum(CurrentNum + 1);
}

void UB2UIBuySkillPointBar::OnClickBtnSetMax()
{
	SetNum(MaxNum);
}