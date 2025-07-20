#include "B2UIPVPEntryHero.h"


#include "BladeIIUtil.h"

UB2UIPVPEntryHero::UB2UIPVPEntryHero(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurPCClass = EPCClass::EPC_End;
	CurClassLevel = 0;
}

void UB2UIPVPEntryHero::Init()
{
	Super::Init();

	SetEntryInfo(EPCClass::EPC_Gladiator, EEntryType::EntryType_None, 0);
}

#define VISIBILITIY_CHECK_SLOT(Slot, VisibleCheck)	\
	if(Slot.IsValid())								\
		Slot->SetVisibility(VisibleCheck ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);	

void UB2UIPVPEntryHero::SetEntryInfo(EPCClass PCClass, EEntryType EntryType, int32 ClassLevel)
{
	CurPCClass = PCClass;
	CurClassLevel = ClassLevel;

	SetVisibilityWidgets(EntryType);
}


void UB2UIPVPEntryHero::SetVisibilityWidgets(EEntryType EntryType)
{
	VISIBILITIY_CHECK_SLOT(OV_MainSelect, EntryType == EEntryType::EntryType_Select1);
	VISIBILITIY_CHECK_SLOT(OV_SubSelect, EntryType == EEntryType::EntryType_Select2);
	VISIBILITIY_CHECK_SLOT(OV_TeamSelect, (EntryType == EEntryType::EntryType_Team || EntryType == EEntryType::EntryType_MainChar));
	VISIBILITIY_CHECK_SLOT(IMG_Portrait_Active, EntryType != EEntryType::EntryType_None);
	VISIBILITIY_CHECK_SLOT(IMG_Portrait_Effec, EntryType != EEntryType::EntryType_None);
	VISIBILITIY_CHECK_SLOT(IMG_Portrait_De, EntryType == EEntryType::EntryType_None);
	VISIBILITIY_CHECK_SLOT(VB_Active, EntryType != EEntryType::EntryType_None);
	VISIBILITIY_CHECK_SLOT(VB_De, EntryType == EEntryType::EntryType_None);
	VISIBILITIY_CHECK_SLOT(TB_Level, EntryType != EEntryType::EntryType_None);
	VISIBILITIY_CHECK_SLOT(TB_Level_De, EntryType == EEntryType::EntryType_None);
	
	if (IMG_Portrait_Active.IsValid())
		IMG_Portrait_Active->SetBrushFromMaterial(FB2ClassPortraitImage::GetCharImage(PortraitInfo, CurPCClass));
	if(IMG_Portrait_De.IsValid())
		IMG_Portrait_De->SetBrushFromMaterial(FB2ClassPortraitImage::GetCharImage(Inactive_PortraitInfo, CurPCClass));

	auto LevelSlot = EntryType != EEntryType::EntryType_None ? TB_Level : TB_Level_De;
	if (LevelSlot.IsValid())
		LevelSlot->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_Entry_Level")), FText::AsNumber(CurClassLevel)));
}

void UB2UIPVPEntryHero::CacheAssets()
{
	GET_SLOT(UOverlay, OV_MainSelect);
	GET_SLOT(UOverlay, OV_SubSelect);
	GET_SLOT(UOverlay, OV_TeamSelect);
	GET_SLOT(UImage, IMG_Portrait_Active);
	GET_SLOT(UImage, IMG_Portrait_Effec);
	GET_SLOT(UImage, IMG_Portrait_De);
	GET_SLOT(UTextBlock, TB_Level);
	GET_SLOT(UTextBlock, TB_Level_De);
	GET_SLOT(UB2Button, BTN_Select);
	GET_SLOT(UVerticalBox, VB_De); // For only visible
	GET_SLOT(UVerticalBox, VB_Active); // For only visible
}

void UB2UIPVPEntryHero::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIPVPEntryHero::OnClickBtnSelect);
}

void UB2UIPVPEntryHero::OnClickBtnSelect()
{
	FOnClickBtnSelect.ExecuteIfBound(CurPCClass);
}

#undef VISIBILITIY_CHECK_SLOT