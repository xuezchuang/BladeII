#include "B2UIGuildCreate.h"
#include "B2UIGuildMark.h"
#include "B2GuildMarkInfo.h"
#include "B2UIManager.h"
#include "Retailer.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2UIDocHelper.h"

void UB2UIGuildCreateSlot::Init()
{
	Super::Init();
	SlotIndex = 0;
}

void UB2UIGuildCreateSlot::CacheAssets()
{
	GET_SLOT(UB2Image, IMG_Guild_Slot)
	GET_SLOT(UB2Image, IMG_Choice_Slot)
	GET_SLOT(UB2Button, BTN_Slot)

	if (IMG_Choice_Slot.IsValid())
		IMG_Choice_Slot->SetVisibility(ESlateVisibility::Hidden);
}

void UB2UIGuildCreateSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Slot, &UB2UIGuildCreateSlot::OnClickSlot)
}

void UB2UIGuildCreateSlot::SetMeterial(UMaterialInstance* MaterialImage)
{
	if (IMG_Guild_Slot.IsValid())
		IMG_Guild_Slot->SetBrushFromMaterial(MaterialImage);
}

void UB2UIGuildCreateSlot::SetLinearColor(FLinearColor LinearColor)
{
	IMG_Guild_Slot->SetColorAndOpacity(LinearColor);
}

void UB2UIGuildCreateSlot::SetSlotIndex(int32 Index)
{
	SlotIndex = Index;
}

void UB2UIGuildCreateSlot::SetDelegate(FGuildMarkSlotIndex Delegate)
{
	GuildMarkSlotDelegate = Delegate;
}

void UB2UIGuildCreateSlot::SetSelect(bool bInSelect)
{
	this->bSelect = bInSelect;
	if (IMG_Choice_Slot.IsValid())
	{
		if (bSelect)
			IMG_Choice_Slot->SetVisibility(ESlateVisibility::Visible);
		else
			IMG_Choice_Slot->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UIGuildCreateSlot::OnClickSlot()
{
	GuildMarkSlotDelegate.ExecuteIfBound(SlotIndex);
}


void UB2UIGuildCreate::Init()
{
	Super::Init();

	FUserGuildInfo GuildInfo = BladeIIGameImpl::GetClientDataStore().GetUserGuildInfo();


	PatternImageIndex = GuildInfo.Mark_Index;;
	PatternColorIndex = GuildInfo.MarkColor_Index;
	BGImageIndex = GuildInfo.MarkBG_Index;
	BGColorIndex = GuildInfo.MarkBGColor_Index;

}

void UB2UIGuildCreate::OnOpen(bool RightNow /*= false*/)
{
	SetOpenState(EUIOpenState::Opened);
	//CB_Public->SetCheckedState(ECheckBoxState::Checked);

	FClientDataStore& CliDataStore = BladeIIGameImpl::GetClientDataStore();

	int32 JoinOption = b2network::B2GuildJoinOption::PUBLIC;

	if (FB2ResponseGuildInfoPtr pGuildInfo = CliDataStore.GetMyGuildInfo().GuildInfo)
	{
		JoinOption = pGuildInfo->join_option;
	}
	
	if (JoinOption == b2network::B2GuildJoinOption::PUBLIC)
		CB_Public->SetCheckedState(ECheckBoxState::Checked);
	else
		CB_Closed->SetCheckedState(ECheckBoxState::Checked);


	auto UserDoc = UB2UIDocHelper::GetDocUser();

	if (!UserDoc)
		return;

	if (TB_Cost.IsValid())
	{
		TB_Cost->SetText(FText::AsNumber(CliDataStore.GetGuildSetupGoldCost()));
		TB_Cost->SetColorAndOpacity(FSlateColor(UserDoc->GetGold() >= CliDataStore.GetGuildSetupGoldCost() ? FLinearColor::White : FLinearColor::Red));
	}

	if (TB_UpdateCost.IsValid())
	{
		TB_UpdateCost->SetText(FText::AsNumber(CliDataStore.GetGuildUpdateCost()));
		TB_UpdateCost->SetColorAndOpacity(FSlateColor(UserDoc->GetGold() >= CliDataStore.GetGuildUpdateCost() ? FLinearColor::White : FLinearColor::Red));
	}

	OnClickPattern();


}

void UB2UIGuildCreate::CacheAssets()
{
	GET_SLOT(UB2UIGuildMark, UIP_LargeGuildMark)
	if (UIP_LargeGuildMark.IsValid())
		UIP_LargeGuildMark->Init();
	GET_SLOT(UB2UIGuildCreateSlot,UIP_GuildCreateSlot)
	GET_SLOT(UB2Button,BTN_Random)
	GET_SLOT(UEditableTextBox, ETB_GuildName)
	GET_SLOT(UCheckBox, CB_Public)
	GET_SLOT(UCheckBox, CB_Closed)

	GET_SLOT(UB2Button,BTN_Pattern)
	GET_SLOT(UB2Button, BTN_BGImage)
	GET_SLOT(UUniformGridPanel,UGP_MarkSlotList)
	GET_SLOT(UUniformGridPanel, UGP_ColorSlotList)
	GET_SLOT(UImage, IMG_ChoiceSlot)

	GET_SLOT(UB2Button, BTN_CreateGuild)
	GET_SLOT(UB2Button, BTN_Edit)
	GET_SLOT(UB2Button, BTN_Close)

	GET_SLOT(UTextBlock, TB_Cost)
	GET_SLOT(UTextBlock, TB_UpdateCost)

	TapSlotArr.Empty();
	TapSlotArr.SetNum(2);
	UnTapSlotArr.Empty();
	UnTapSlotArr.SetNum(2);

	GET_SLOT_BYNAME(UOverlay, TapSlotArr[0], OV_Text_Tap_01_A);
	GET_SLOT_BYNAME(UOverlay, TapSlotArr[1], OV_Text_Tap_03_A);

	GET_SLOT_BYNAME(UOverlay, UnTapSlotArr[0], OV_Text_Tap_01_B);
	GET_SLOT_BYNAME(UOverlay, UnTapSlotArr[1], OV_Text_Tap_03_B);

	GET_SLOT(UB2RichTextBlock, RTB_Open);
	GET_SLOT(UB2RichTextBlock, RTB_Private);
	GET_SLOT(UB2RichTextBlock, RTB_Flag1_1);
	GET_SLOT(UB2RichTextBlock, RTB_Flag1_2);
	GET_SLOT(UB2RichTextBlock, RTB_Flag2_1);
	GET_SLOT(UB2RichTextBlock, RTB_Flag2_2);
	GET_SLOT(UTextBlock, TB_Flag3);
	GET_SLOT(UB2RichTextBlock, RTB_Random);
	GET_SLOT(UTextBlock, TB_GuildCreate);
	GET_SLOT(UTextBlock, TB_GuildEdit);
	GET_SLOT(UTextBlock, TB_GuildEdit2);


}


void UB2UIGuildCreate::BindDelegates()
{
	
	BIND_CLICK_FUNC_TO_BTN(BTN_Pattern, &UB2UIGuildCreate::OnClickPattern);	
	BIND_CLICK_FUNC_TO_BTN(BTN_BGImage, &UB2UIGuildCreate::OnClickBGImage);	
	BIND_CLICK_FUNC_TO_BTN(BTN_CreateGuild, &UB2UIGuildCreate::OnClickCreateGuild);
	BIND_CLICK_FUNC_TO_BTN(BTN_Random, &UB2UIGuildCreate::OnClickRandom);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIGuildCreate::OnClickClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_Edit, &UB2UIGuildCreate::OnClickEdit);
}

void UB2UIGuildCreate::CloseWidgetDelegate()
{
	OnClickClose();
}

void UB2UIGuildCreate::UpdateStaticText()
{
	if (ETB_GuildName.IsValid())
		ETB_GuildName->SetHintText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_EnterGuildName")));

	if (RTB_Open.IsValid())
		RTB_Open->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Open")));

	if (RTB_Private.IsValid())
		RTB_Private->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Private")));

	if (RTB_Flag1_1.IsValid())
		RTB_Flag1_1->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Flag1")));

	if (RTB_Flag1_2.IsValid())
		RTB_Flag1_2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Flag1")));

	if (RTB_Flag2_1.IsValid())
		RTB_Flag2_1->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Join_PatternBg")));

	if (RTB_Flag2_1.IsValid())
		RTB_Flag2_2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Join_PatternBg")));

	if (TB_Flag3.IsValid())
		TB_Flag3->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Flag2")));

	if (RTB_Random.IsValid())
		RTB_Random->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_RandomCreate")));

	if (TB_GuildCreate.IsValid())
		TB_GuildCreate->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Create")));

	if (TB_GuildEdit.IsValid())
		TB_GuildEdit->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Edit2")));

	if (TB_GuildEdit2.IsValid())
		TB_GuildEdit2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_Edit2")));
}

#define SET_GUILDTAPSLOT(GetNumberFunction,UGPSlot, SlotMax, UIPSlot)							\
	UGPSlot->ClearChildren();																	\
	SlotMaxNumber = GuildMarkInfo->Get##GetNumberFunction();									\
	for (int32 i = 0; i < SlotMaxNumber; ++i){													\
	UB2UIGuildCreateSlot* UIP_Slot = nullptr;													\
	UIP_Slot = CreateWidget<UB2UIGuildCreateSlot>(GetWorld(), UIPSlot);							\
	check(UIP_Slot);																			\
	if ( ULocalPlayer* Player =	GetWorld()->GetFirstLocalPlayerFromController())				\
	{																							\
		UIP_Slot->SetPlayerContext(FLocalPlayerContext(Player));								\
	}																							\
	UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(UGPSlot->AddChild(UIP_Slot));			\
	UIP_Slot->Init();																			\
	check(GridSlot);																			\
	GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);						\
	GridSlot->SetColumn(i % SlotMax);															\
	GridSlot->SetRow(i / SlotMax);																\


#define GUILD_SLOT_DELEGATE(FunctionName)														\
	FGuildMarkSlotIndex Delegate;																\
	Delegate.BindUObject(this, FunctionName);													\
	UIP_Slot->SetDelegate(Delegate);															\


#define GET_GUILDTSLOTTYPE(ReturnType, GetFunction, SetReturnType)								\
	ReturnType RetType = GuildMarkInfo->Get##GetFunction(i);									\
	UIP_Slot->Set##SetReturnType(RetType);														\
	UIP_Slot->SetSlotIndex(i);}																	\


void UB2UIGuildCreate::SetGuildSlot(EUIGuildMarkTap GuildMarkType)
{

	UB2GuildMarkInfo* GuildMarkInfo = StaticFindGuildMarkInfo();

	check(GuildMarkInfo);

	
	int32 SlotMaxNumber = 0;
	TapType = GuildMarkType;

	if (GuildMarkType == EUIGuildMarkTap::PATTERNIMAGE)
	{
		SET_GUILDTAPSLOT(PatternImageNum, UGP_MarkSlotList, SlotWidthNumber, PatternSlot)
		GUILD_SLOT_DELEGATE(&UB2UIGuildCreate::ImageUpdate)
		GET_GUILDTSLOTTYPE(UMaterialInstance*, PatternImage, Meterial)
		SET_GUILDTAPSLOT(PatternColorNum, UGP_ColorSlotList, SlowColorWhidthNumber, ColorSlot)
		GUILD_SLOT_DELEGATE(&UB2UIGuildCreate::ColorUpdate)
		GET_GUILDTSLOTTYPE(FLinearColor, PatternColor, LinearColor)
	}
	else if (GuildMarkType == EUIGuildMarkTap::BGIMAGE)
	{
		SET_GUILDTAPSLOT(BGImageNum, UGP_MarkSlotList, SlotWidthNumber, PatternSlot)
		GUILD_SLOT_DELEGATE(&UB2UIGuildCreate::ImageUpdate)
		GET_GUILDTSLOTTYPE(UMaterialInstance*, BGImage, Meterial)
		SET_GUILDTAPSLOT(PatternColorNum, UGP_ColorSlotList, SlowColorWhidthNumber,ColorSlot)
		GUILD_SLOT_DELEGATE(&UB2UIGuildCreate::ColorUpdate)
		GET_GUILDTSLOTTYPE(FLinearColor, BGColor, LinearColor)
	}

}

void UB2UIGuildCreate::TapUpdate(EUIGuildMarkTap Intype)
{
	for (int32 i = 0; i < 2; ++i)
	{
		if (static_cast<int32>(Intype) == i)
		{
			TapSlotArr[i]->SetVisibility(ESlateVisibility::Visible);
			UnTapSlotArr[i]->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			TapSlotArr[i]->SetVisibility(ESlateVisibility::Hidden);
			UnTapSlotArr[i]->SetVisibility(ESlateVisibility::Visible);
		}
	}

}


void UB2UIGuildCreate::OnClickPattern()
{
	SetGuildSlot(EUIGuildMarkTap::PATTERNIMAGE);

	TapUpdate(EUIGuildMarkTap::PATTERNIMAGE);
	ImageUpdate(PatternImageIndex);
	ColorUpdate(PatternColorIndex);
}

void UB2UIGuildCreate::OnClickPatternColor()
{
	/*SetGuildSlot(EUIGuildMarkTap::PATTERNCOLOR);

	TapUpdate(EUIGuildMarkTap::PATTERNCOLOR);
	ImageUpdate(PatternColorIndex);*/
}

void UB2UIGuildCreate::OnClickBGImage()
{
	SetGuildSlot(EUIGuildMarkTap::BGIMAGE);

	TapUpdate(EUIGuildMarkTap::BGIMAGE);
	ImageUpdate(BGImageIndex);
	ColorUpdate(BGColorIndex);
}

void UB2UIGuildCreate::OnClickBGImageColor()
{
	/*SetGuildSlot(EUIGuildMarkTap::BGIMAGECOLOR);
	
	TapUpdate(EUIGuildMarkTap::BGIMAGECOLOR);
	ImageUpdate(BGColorIndex);*/
}

void UB2UIGuildCreate::OnClickCreateGuild()
{
	const FString GuildName = ETB_GuildName->GetText().ToString();

	if (!GuildName.Len())
	{
		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")), 
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_GuildNameEmpty")), 0, true, true,
			EUIMsgPopupButtonGroup::Confirm
		);


		return;
	}

	int32 JoinOption = CB_Public->GetCheckedState() == ECheckBoxState::Checked ? b2network::B2GuildJoinOption::PUBLIC : b2network::B2GuildJoinOption::PRIVATE;

	data_trader::Retailer::GetInstance().RequestCreateGuild(GuildName, JoinOption, PatternImageIndex, PatternColorIndex, BGImageIndex, BGColorIndex);
}

void UB2UIGuildCreate::OnClickRandom()
{
	UB2GuildMarkInfo* GuildMarkInfo = StaticFindGuildMarkInfo();

	check(GuildMarkInfo);

	FRandomStream Random;
	Random.GenerateNewSeed();

	int32 NewPatternImageIndex = Random.RandHelper(GuildMarkInfo->GetPatternImageNum());
	int32 NewPatternColorIndex = Random.RandHelper(GuildMarkInfo->GetPatternColorNum());
	int32 NewBGImageIndex = Random.RandHelper(GuildMarkInfo->GetBGImageNum());
	int32 NewBGColorIndex = Random.RandHelper(GuildMarkInfo->GetBGColorNum());

	if (TapType == EUIGuildMarkTap::PATTERNIMAGE)
	{
		ImageUpdate(NewPatternImageIndex);
		ColorUpdate(NewPatternColorIndex);
	}
	else if (TapType == EUIGuildMarkTap::BGIMAGE)
	{
		ImageUpdate(NewBGImageIndex);
		ColorUpdate(NewBGColorIndex);
	}

	PatternImageIndex = NewPatternImageIndex;
	PatternColorIndex = NewPatternColorIndex;
	BGImageIndex = NewBGImageIndex;
	BGColorIndex = NewBGColorIndex;


	UIP_LargeGuildMark->SetGuildMark(PatternImageIndex, PatternColorIndex, BGImageIndex, BGColorIndex);
}


void UB2UIGuildCreate::OnClickClose()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::GuildEdit);
}

void UB2UIGuildCreate::OnClickEdit()
{
	int32 JoinOption = CB_Public->GetCheckedState() == ECheckBoxState::Checked ? b2network::B2GuildJoinOption::PUBLIC : b2network::B2GuildJoinOption::PRIVATE;
	data_trader::Retailer::GetInstance().RequestUpdateGuild(JoinOption, PatternImageIndex, PatternColorIndex, BGImageIndex, BGColorIndex);
}

void UB2UIGuildCreate::ChangeCheckBoxState(UCheckBox* checkbox, bool bCheck)
{
	
	if (CB_Public.Get() == checkbox)
	{
		if (bCheck)
		{
			CB_Closed->SetCheckedState(ECheckBoxState::Unchecked);
			CB_Public->SetCheckedState(ECheckBoxState::Checked);
		}
		else
		{
			CB_Closed->SetCheckedState(ECheckBoxState::Checked);
			CB_Public->SetCheckedState(ECheckBoxState::Unchecked);
		}
	}

	if (CB_Closed.Get() == checkbox)
	{
		if (bCheck)
		{
			CB_Public->SetCheckedState(ECheckBoxState::Unchecked);
			CB_Closed->SetCheckedState(ECheckBoxState::Checked);
		}
		else
		{
			CB_Public->SetCheckedState(ECheckBoxState::Checked);
			CB_Closed->SetCheckedState(ECheckBoxState::Unchecked);
		}
	}
	ButtonEnableCheck();
}

void UB2UIGuildCreate::ColorUpdate(int32 index)
{
	int32 oldIndex = 0;
	if (TapType == EUIGuildMarkTap::BGIMAGE)
	{
		oldIndex = BGColorIndex;
		BGColorIndex = index;
	}
	else if (TapType == EUIGuildMarkTap::PATTERNIMAGE)
	{
		oldIndex = PatternColorIndex;
		PatternColorIndex = index;
	}

	UB2UIGuildCreateSlot* pSlot = static_cast<UB2UIGuildCreateSlot*>(UGP_ColorSlotList->GetChildAt(oldIndex));
	if (pSlot)
		pSlot->SetSelect(false);

	pSlot = static_cast<UB2UIGuildCreateSlot*>(UGP_ColorSlotList->GetChildAt(index));
	if (pSlot)
		pSlot->SetSelect(true);

	UIP_LargeGuildMark->SetGuildMark(PatternImageIndex, PatternColorIndex, BGImageIndex, BGColorIndex);

	ButtonEnableCheck();
}

void UB2UIGuildCreate::ButtonEnableCheck()
{

	if (!BTN_Edit.IsValid())
		return;

	FUserGuildInfo GuildInfo = BladeIIGameImpl::GetClientDataStore().GetUserGuildInfo();

	bool EditButtonState = false;

	int32 JoinOption = BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->join_option;
	int32 InJoinOption = CB_Public->GetCheckedState() == ECheckBoxState::Checked ? b2network::B2GuildJoinOption::PUBLIC : b2network::B2GuildJoinOption::PRIVATE;

	if (PatternImageIndex != GuildInfo.Mark_Index
		|| PatternColorIndex != GuildInfo.MarkColor_Index
		|| BGImageIndex != GuildInfo.MarkBG_Index
		|| BGColorIndex != GuildInfo.MarkBGColor_Index
		|| JoinOption != InJoinOption)
	{
		EditButtonState = true;
	}
	else
		EditButtonState = false;

	BTN_Edit->SetIsEnabled(EditButtonState);

}

void UB2UIGuildCreate::ImageUpdate(int32 index)
{
	int32 oldIndex = 0;
	if (TapType == EUIGuildMarkTap::PATTERNIMAGE)
	{
		oldIndex = PatternImageIndex;
		PatternImageIndex = index;
	}
	else if (TapType == EUIGuildMarkTap::BGIMAGE)
	{
		oldIndex = BGImageIndex;
		BGImageIndex = index;
	}
	
	UB2UIGuildCreateSlot* pSlot = static_cast<UB2UIGuildCreateSlot*>(UGP_MarkSlotList->GetChildAt(oldIndex));
	if (pSlot)
		pSlot->SetSelect(false);
	pSlot = static_cast<UB2UIGuildCreateSlot*>(UGP_MarkSlotList->GetChildAt(index));
	if (pSlot)
		pSlot->SetSelect(true);

	UIP_LargeGuildMark->SetGuildMark(PatternImageIndex, PatternColorIndex, BGImageIndex, BGColorIndex);

	ButtonEnableCheck();
}
 