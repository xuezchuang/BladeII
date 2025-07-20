
#include "B2UITMFormation.h"
#include "B2UIDocHelper.h"
#include "Animation/WidgetAnimation.h"
#include "B2UIDocTeamMatch.h"

void UB2UITMFormation::Init()
{
	Super::Init();
}

void UB2UITMFormation::SetStartPos(ETMFormation Pos)
{
	AfterFormation = Pos;
	bIsChangeAnimation = false;

	StartPosSettingEvent();
}

void UB2UITMFormation::UIDocSetCharacter()
{
	UB2UIDocTeamMatch* TeamUIDoc = UB2UIDocHelper::GetDocTeamMatch();

	if (TeamUIDoc == nullptr)
		return;

	if (CharacterTexture.Num() < GetMaxPCClassNum())
		return;

	IMG_Character1->SetBrushFromMaterial(CharacterTexture[PCClassToInt(TeamUIDoc->GetPCCharacterClass1())]);
	IMG_Character2->SetBrushFromMaterial(CharacterTexture[PCClassToInt(TeamUIDoc->GetPCCharacterClass2())]);
	IMG_Character3->SetBrushFromMaterial(CharacterTexture[PCClassToInt(TeamUIDoc->GetPCCharacterClass3())]);

	for (int32 i = 0; i < 3; ++i)
	{
			CircleImageArr[i]->SetVisibility(ESlateVisibility::Hidden);
			CharacterImageArr[i]->SetColorAndOpacity(WhiteColor.GetSpecifiedColor());	
	}
}

void UB2UITMFormation::SetSlotCharacter(EPCClass PCClass)
{
	UB2UIDocTeamMatch* TeamUIDoc = UB2UIDocHelper::GetDocTeamMatch();

	if (TeamUIDoc == nullptr)
		return;

	if (CharacterTexture.Num() < GetMaxPCClassNum())
		return;

	if (TeamUIDoc->GetPCCharacterClass1() == EPCClass::EPC_End)
		TeamUIDoc->SetPCCharacterClass1(PCClass);
	else if (TeamUIDoc->GetPCCharacterClass2() == EPCClass::EPC_End)
		TeamUIDoc->SetPCCharacterClass2(PCClass);
	else if (TeamUIDoc->GetPCCharacterClass3() == EPCClass::EPC_End)
		TeamUIDoc->SetPCCharacterClass3(PCClass);

}

void UB2UITMFormation::NoneSelectCharacterSlot()
{
	IMG_Character1->SetColorAndOpacity(WhiteColor.GetSpecifiedColor());
	IMG_Character2->SetColorAndOpacity(WhiteColor.GetSpecifiedColor());
	IMG_Character3->SetColorAndOpacity(WhiteColor.GetSpecifiedColor());

	IMG_FormationCircle1->SetVisibility(ESlateVisibility::Hidden);
	IMG_FormationCircle2->SetVisibility(ESlateVisibility::Hidden);
	IMG_FormationCircle3->SetVisibility(ESlateVisibility::Hidden);
}

bool UB2UITMFormation::TeamDocPCClassTest(class UB2UIDocTeamMatch* TeamUIDoc)
{
	if (TeamUIDoc->GetPCCharacterClass1() == EPCClass::EPC_End)
		return true;
	else if (TeamUIDoc->GetPCCharacterClass2() == EPCClass::EPC_End)
		return true;
	else if (TeamUIDoc->GetPCCharacterClass3() == EPCClass::EPC_End)
		return true;


	return false;
}

void UB2UITMFormation::CacheAssets()
{
	GET_SLOT(UCanvasPanel, Formation1);
	GET_SLOT(UCanvasPanel, Formation2);
	GET_SLOT(UCanvasPanel, Formation3);

	GET_SLOT(UImage, IMG_EmptySlot1);
	GET_SLOT(UImage, IMG_Character1);
	GET_SLOT(UImage, IMG_EmptySlot2);
	GET_SLOT(UImage, IMG_Character2);
	GET_SLOT(UImage, IMG_EmptySlot3);
	GET_SLOT(UImage, IMG_Character3);

	CharacterImageArr.Add(IMG_Character1.Get());
	CharacterImageArr.Add(IMG_Character2.Get());
	CharacterImageArr.Add(IMG_Character3.Get());


	GET_SLOT(UButton, BTN_Formation1);
	GET_SLOT(UButton, BTN_Formation2);
	GET_SLOT(UButton, BTN_Formation3);

	GET_SLOT(UImage, IMG_FormationCircle1);
	GET_SLOT(UImage, IMG_FormationCircle2);
	GET_SLOT(UImage, IMG_FormationCircle3);

	GET_SLOT(UTextBlock, X_FirstCharFomationNum);
	GET_SLOT(UTextBlock, X_SecondCharFomationNum);
	GET_SLOT(UTextBlock, X_ThirdCharFomationNum);

	CircleImageArr.Add(IMG_FormationCircle1.Get());
	CircleImageArr.Add(IMG_FormationCircle2.Get());
	CircleImageArr.Add(IMG_FormationCircle3.Get());
}

void UB2UITMFormation::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Formation1, &UB2UITMFormation::OnClickFormation1);
	BIND_CLICK_FUNC_TO_BTN(BTN_Formation2, &UB2UITMFormation::OnClickFormation2);
	BIND_CLICK_FUNC_TO_BTN(BTN_Formation3, &UB2UITMFormation::OnClickFormation3);
}

void UB2UITMFormation::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	CircleImageArr.Empty();
	CharacterImageArr.Empty();
}

#define SET_ONCLICKFORMAITON(Index)											\
	if (CharacterSlotUpdateDelegate.IsBound() == false) return;				\
	UB2UIDocTeamMatch* TeamUIDoc = UB2UIDocHelper::GetDocTeamMatch();		\
	if (TeamUIDoc == nullptr) return;										\
	ChoiceClass = TeamUIDoc->GetPCCharacterClass##Index();					\
	if(!TeamDocPCClassTest(TeamUIDoc))										\
	{		TeamUIDoc->SetPCCharacterClass##Index(EPCClass::EPC_End);		\
			SetSlotState(Index-1);											\
	} else NoneSelectCharacterSlot();										\
	CharacterSlotUpdateDelegate.ExecuteIfBound(ChoiceClass);				\


void UB2UITMFormation::SetFomationNum(ETMFormation Formation)
{
	UB2UIDocTeamMatch* TeamDoc = UB2UIDocHelper::GetDocTeamMatch();

	if (X_FirstCharFomationNum.IsValid())
	{
		X_FirstCharFomationNum->SetText(FText::AsNumber(static_cast<int8>(TeamDoc->GetFirstCharcterForamtionNUM())));
	}

	if (X_SecondCharFomationNum.IsValid())
	{
		X_SecondCharFomationNum->SetText(FText::AsNumber(static_cast<int8>(TeamDoc->GetSecondCharcterForamtionNUM())));
	}

	if(X_ThirdCharFomationNum.IsValid())
	{
		X_ThirdCharFomationNum->SetText(FText::AsNumber(static_cast<int8>(TeamDoc->GetThirdCharcterForamtionNUM())));
	}
}

void UB2UITMFormation::OnClickFormation1()
{
	SET_ONCLICKFORMAITON(1);
}

void UB2UITMFormation::OnClickFormation2()
{
	SET_ONCLICKFORMAITON(2);
}

void UB2UITMFormation::OnClickFormation3()
{
	SET_ONCLICKFORMAITON(3);
}

void UB2UITMFormation::CancelFormation()
{
	UB2UIDocTeamMatch* TeamUIDoc = UB2UIDocHelper::GetDocTeamMatch();				

	if (TeamUIDoc == nullptr) 
		return;

	if (TeamUIDoc->GetPCCharacterClass1() == EPCClass::EPC_End)
		TeamUIDoc->SetPCCharacterClass1(ChoiceClass);
	else if (TeamUIDoc->GetPCCharacterClass2() == EPCClass::EPC_End)
		TeamUIDoc->SetPCCharacterClass2(ChoiceClass);
	else if (TeamUIDoc->GetPCCharacterClass3() == EPCClass::EPC_End)
		TeamUIDoc->SetPCCharacterClass3(ChoiceClass);
}

void UB2UITMFormation::StartPlayAnimation(UWidgetAnimation* ANIM_Formation_1_1, UWidgetAnimation* ANIM_Formation_1_2, UWidgetAnimation* ANIM_Formation_2_1, UWidgetAnimation* ANIM_Formation_2_2, UWidgetAnimation* ANIM_Formation_3_1, UWidgetAnimation* ANIM_Formation_3_2)
{
	SetFomationNum(AfterFormation);
	if (BeforeFormation == ETMFormation::ETM_Normal)
	{
		if (AfterFormation == ETMFormation::ETM_Attack)
			PlayAnimation(ANIM_Formation_1_1);
		else if (AfterFormation == ETMFormation::ETM_Defense)
			PlayAnimation(ANIM_Formation_1_2);
	}
	else if (BeforeFormation == ETMFormation::ETM_Attack)
	{
		if (AfterFormation == ETMFormation::ETM_Normal)
			PlayAnimation(ANIM_Formation_2_1);
		else if (AfterFormation == ETMFormation::ETM_Defense)
			PlayAnimation(ANIM_Formation_2_2);
	}
	else if (BeforeFormation == ETMFormation::ETM_Defense)
	{
		if (AfterFormation == ETMFormation::ETM_Normal)
			PlayAnimation(ANIM_Formation_3_1);
		else if (AfterFormation == ETMFormation::ETM_Attack)
			PlayAnimation(ANIM_Formation_3_2);
	}
}

void UB2UITMFormation::StartPosSetting(UWidgetAnimation* NormalPosAni, UWidgetAnimation* AttPosAni, UWidgetAnimation* DefPosAni)
{
	SetFomationNum(AfterFormation);
	if (AfterFormation == ETMFormation::ETM_Normal)
		PlayAnimation(NormalPosAni, NormalPosAni->GetEndTime());
	else if (AfterFormation == ETMFormation::ETM_Attack)
		PlayAnimation(AttPosAni, AttPosAni->GetEndTime());
	else if (AfterFormation == ETMFormation::ETM_Defense)
		PlayAnimation(DefPosAni, DefPosAni->GetEndTime());
}

void UB2UITMFormation::SetSlotState(int32 SelectIndex)
{
	int32 IndexMax = 3;

	for (int32 i = 0; i < IndexMax; ++i)
	{
		if (i == SelectIndex)
		{
			CircleImageArr[i]->SetVisibility(ESlateVisibility::Visible);
			CharacterImageArr[i]->SetColorAndOpacity(WhiteColor.GetSpecifiedColor());
		}
		else
		{
			CircleImageArr[i]->SetVisibility(ESlateVisibility::Hidden);
			CharacterImageArr[i]->SetColorAndOpacity(DarkColor.GetSpecifiedColor());
		}
	}
}

void UB2UITMFormation::AnimationComplete()
{
	bIsChangeAnimation = false;
}

void UB2UITMFormation::SetPlayAnimation(ETMFormation beforeFormation, ETMFormation afterFormation)
{
	bIsChangeAnimation = true;

	BeforeFormation = beforeFormation;
	AfterFormation = afterFormation;

	PlayAnimationEvent();
}
