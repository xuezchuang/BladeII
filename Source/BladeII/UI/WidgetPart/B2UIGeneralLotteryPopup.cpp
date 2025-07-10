#include "B2UIGeneralLotteryPopup.h"

void UB2UIGeneralLotteryPopup::Init()
{
	Super::Init();

	SelectPCType = EPCClass::EPC_End;
}

void UB2UIGeneralLotteryPopup::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if (InVisibility == ESlateVisibility::Hidden)
		SelectPCType = EPCClass::EPC_End;

	UpdateCharSlot(SelectPCType);
}

void UB2UIGeneralLotteryPopup::CacheAssets()
{
	GET_SLOT(UButton, BTN_Gladiator);
	GET_SLOT(UButton, BTN_Assassin);
	GET_SLOT(UButton, BTN_Wizard);
	GET_SLOT(UButton, BTN_Fighter);
	GET_SLOT(UButton, BTN_Close);

	O_OnCharacter.SetNum(static_cast<int32>(EPCClass::EPC_End));
	O_OffCharacter.SetNum(static_cast<int32>(EPCClass::EPC_End));

	GET_SLOT_BYNAME(UOverlay, O_OnCharacter[0], O_GladiatorOn);
	GET_SLOT_BYNAME(UOverlay, O_OnCharacter[1], O_AssassinOn);
	GET_SLOT_BYNAME(UOverlay, O_OnCharacter[2], O_WizardOn);
	GET_SLOT_BYNAME(UOverlay, O_OnCharacter[3], O_FighterOn);

	GET_SLOT_BYNAME(UOverlay, O_OffCharacter[0], O_GladiatorOff);
	GET_SLOT_BYNAME(UOverlay, O_OffCharacter[1], O_AssassinOff);
	GET_SLOT_BYNAME(UOverlay, O_OffCharacter[2], O_WizardOff);
	GET_SLOT_BYNAME(UOverlay, O_OffCharacter[3], O_FighterOff)


	GET_SLOT(UB2Button, BTN_Confirm);

	GET_SLOT(UTextBlock, TB_Title);

}

void UB2UIGeneralLotteryPopup::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIGeneralLotteryPopup::CloseMe);

	BIND_CLICK_FUNC_TO_BTN(BTN_Gladiator, &UB2UIGeneralLotteryPopup::SelectedGladiator);
	BIND_CLICK_FUNC_TO_BTN(BTN_Assassin, &UB2UIGeneralLotteryPopup::SelectedAssassin);
	BIND_CLICK_FUNC_TO_BTN(BTN_Wizard, &UB2UIGeneralLotteryPopup::SelectedWizard);
	BIND_CLICK_FUNC_TO_BTN(BTN_Fighter, &UB2UIGeneralLotteryPopup::SelectedFighter);
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIGeneralLotteryPopup::SelectConfirm);
}

void UB2UIGeneralLotteryPopup::CloseMe()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIGeneralLotteryPopup::SelectedGladiator()
{

	SelectPCType = EPCClass::EPC_Gladiator;

	UpdateCharSlot(SelectPCType);
}

void UB2UIGeneralLotteryPopup::SelectedAssassin()
{

	SelectPCType = EPCClass::EPC_Assassin;

	UpdateCharSlot(SelectPCType);
}

void UB2UIGeneralLotteryPopup::SelectedWizard()
{

	SelectPCType = EPCClass::EPC_Wizard;

	UpdateCharSlot(SelectPCType);
}

void UB2UIGeneralLotteryPopup::SelectedFighter()
{
	SelectPCType = EPCClass::EPC_Fighter;

	UpdateCharSlot(SelectPCType);
}

void UB2UIGeneralLotteryPopup::SelectConfirm()
{
	SelectedCharacter(SelectPCType);
}

#include "B2UIMail.h"
void UB2UIGeneralLotteryPopup::SelectedCharacter(EPCClass SelectedCharacter)
{
	BII_CHECK(MailBox);

	if (MailBox)
	{
		MailBox->SelectedCharacterGeneralLottery(SelectedCharacter);
	}
}

void UB2UIGeneralLotteryPopup::UpdateCharSlot(EPCClass SelectCharacter)
{
	for (int32 i = 0; i < static_cast<int32>(EPCClass::EPC_End); ++i)
	{
		bool bSelect = false;

		if (SelectCharacter == static_cast<EPCClass>(i))
			bSelect = true;

		if(O_OnCharacter[i].IsValid())
			O_OnCharacter[i]->SetVisibility(bSelect ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

		if (O_OffCharacter[i].IsValid())
			O_OffCharacter[i]->SetVisibility(bSelect ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}

	if (BTN_Confirm.IsValid())
		BTN_Confirm->SetIsEnabled(SelectCharacter == EPCClass::EPC_End ? false : true);
}

void UB2UIGeneralLotteryPopup::SetTitleText(const FText& Title)
{
	if(TB_Title.IsValid())
	{
		TB_Title->SetText(Title);
	}
}