#include "B2UICharacterIntro.h"
#include "B2UIDocHelper.h"
#include "B2UIDocHero.h"
#include "B2UIManager.h"
#include "B2PCClassInfo.h"
#include "B2SomeInfo.h"


#include "Global.h"
#include "BladeIIGameImpl.h"

UB2UICharacterIntro::UB2UICharacterIntro(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SelectedHeroDoc = nullptr;
	CurrCharIntroFlipbookMID = nullptr;

	NameTextColor_Selected = FSlateColor(FLinearColor(1.0f, 1.0f, 0.0f));
	NameTextColor_UnSelected = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f));

	NameTextSelectScale = 1.2f;

	CharIntroFlipbookResetDelay = 0.0f;
	CharIntroFlipbookMIDTimeSync = 0.0f;
}

void UB2UICharacterIntro::Init()
{
	Super::Init();
}

void UB2UICharacterIntro::CacheAssets()
{
	GET_SLOT(UButton,	BTN_Confirm)
	GET_SLOT(UButton,	BTN_Gladiator)
	GET_SLOT(UButton,	BTN_Assassin)
	GET_SLOT(UButton,	BTN_Wizard)
	GET_SLOT(UButton,	BTN_Fighter)
	GET_SLOT(UTextBlock,TB_Title)
	GET_SLOT(UTextBlock,TB_SelectedCharDesc)
	GET_SLOT(UImage,	IMG_IntroFlipbook)

	//static text
	GET_SLOT(UTextBlock, TB_Gladiator)
	GET_SLOT(UTextBlock, TB_Assassin)
	GET_SLOT(UTextBlock, TB_Wizard)
	GET_SLOT(UTextBlock, TB_Fighter)
	GET_SLOT(UTextBlock, TB_Stat_Physical)
	GET_SLOT(UTextBlock, TB_Stat_Attack)
	GET_SLOT(UTextBlock, TB_Stat_Defense)
	GET_SLOT(UTextBlock, TB_BTNConfirm)
}

void UB2UICharacterIntro::UpdateStaticText()
{
	UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(EPCClass::EPC_Gladiator));
	if (TB_Gladiator.IsValid() && DocHero)
		TB_Gladiator->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(DocHero->GetHeroClass()));

	DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(EPCClass::EPC_Assassin));
	if (TB_Assassin.IsValid() && DocHero)
		TB_Assassin->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(DocHero->GetHeroClass()));

	DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(EPCClass::EPC_Wizard));
	if (TB_Wizard.IsValid() && DocHero)
		TB_Wizard->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(DocHero->GetHeroClass()));

	DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(EPCClass::EPC_Fighter));

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

	if (TB_Fighter.IsValid() && DocHero)
		TB_Fighter->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(DocHero->GetHeroClass()));

	if (TB_BTNConfirm.IsValid())
		TB_BTNConfirm->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyCharacterIntro_Next"))));

	if (TB_Stat_Physical.IsValid())
		TB_Stat_Physical->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Health"))));

	if (TB_Stat_Attack.IsValid())
		TB_Stat_Attack->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Attack"))));

	if (TB_Stat_Defense.IsValid())
		TB_Stat_Defense->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("LobbyStatText_Defense"))));
}

void UB2UICharacterIntro::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm,		&UB2UICharacterIntro::OnClickBtnConfirm)
	BIND_CLICK_FUNC_TO_BTN(BTN_Gladiator,	&UB2UICharacterIntro::OnClickBtnGladiator)
	BIND_CLICK_FUNC_TO_BTN(BTN_Assassin,	&UB2UICharacterIntro::OnClickBtnAssassin)
	BIND_CLICK_FUNC_TO_BTN(BTN_Wizard,		&UB2UICharacterIntro::OnClickBtnWizard)
	BIND_CLICK_FUNC_TO_BTN(BTN_Fighter,		&UB2UICharacterIntro::OnClickBtnFighter)
}

void UB2UICharacterIntro::OnOpen(bool RightNow)
{
	Super::OnOpen(RightNow);

	SelectCharacter(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(), true);	//叼弃飘 急琶. 坷锹矫 某腐磐 急琶矫俊绰 SkillPV 局丛阑 久绢霖促. 恐衬窍搁 OpenAnim俊 SkillPV局丛捞 吝汗登绢 乐扁 锭巩....;;;

	WrappedResetCharIntroFlipbookAnim(); // 霉 橇饭烙何磐 矫累.
}

void UB2UICharacterIntro::OnClose(bool RightNow)
{
	Super::OnClose(RightNow);
	ResetCharIntroFlipbookAnim();
}

void UB2UICharacterIntro::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateCharIntroFlipbookMID();
}

void UB2UICharacterIntro::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

#if WITH_EDITOR
	if (!GIsEditor)
#endif
	{
		UB2SomeInfo* SomeInfo = StaticFindSomeInfo();
		if (SomeInfo)
		{
			SomeInfo->UnloadAllPCIntroFlipbookMtrl(); // 娟甸捞 奴 咆胶媚 耽绢府扼 皋葛府甫 惑寸洒 冈阑 巴. 捞 捞饶浚 鞘夸 绝栏聪 朝覆.
		}
	}
}

void UB2UICharacterIntro::SelectCharacter(EPCClass CharType, bool bCancelEffectOpeningSkillPV)
{
	//CharacterIntro俊辑 急琶茄 康旷篮 弊成 某腐磐 家俺父 焊咯瘤绰 侩档
	LobbyCharacterIntroSetPCSelectionClass<EPCClass>::GetInstance().Signal(CharType);
	SelectedHeroDoc = UB2UIDocHelper::GetDocHero((int32)CharType);

	WrappedResetCharIntroFlipbookAnim(); // 某腐磐啊 官差搁 Flipbook material 档 官拆 抛聪 霉 橇饭烙何磐 矫累窍档废 府悸.

	SelectCharacter_BP(CharType, bCancelEffectOpeningSkillPV);	//BP率俊辑 贸府 且 楷免甸

	// Flipbook 悼康惑 material 技泼. 贸澜 肺爹 矫 喉肺欧捞 乐阑 荐 乐促.
	UB2SomeInfo* SomeInfo = StaticFindSomeInfo();
	UMaterialInterface* SelectedCharIntroFlipbook = SomeInfo ? SomeInfo->GetPCIntroFlipbookMtrl(CharType) : NULL;
	if (IMG_IntroFlipbook.IsValid() && SelectedCharIntroFlipbook)
	{
		IMG_IntroFlipbook->SetBrushFromMaterial(SelectedCharIntroFlipbook);
	}

	SetTitle(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(CharType));
	if (SelectedHeroDoc)
	{		
		SetSelectedCharDesc(SelectedHeroDoc->GetClassDesc());
	}
}

void UB2UICharacterIntro::SetTitle(const FText& InText)
{
	if (TB_Title.IsValid())
		TB_Title->SetText(InText);
}

void UB2UICharacterIntro::SetSelectedCharDesc(const FText& InText)
{
	if (TB_SelectedCharDesc.IsValid())
		TB_SelectedCharDesc->SetText(InText);
}


void UB2UICharacterIntro::WrappedResetCharIntroFlipbookAnim()
{
	// 傲 ResetCharIntroFlipbookAnim 溜矫 龋免 酒聪搁 鸥捞赣. Flipbook anim image 局聪皋捞记 锭巩俊.
	if (CharIntroFlipbookResetDelay > 0.0f)
	{
		APlayerController* OwningPC = GetOwningPlayer();
		if (OwningPC)
		{
			OwningPC->GetWorldTimerManager().SetTimer(CharIntroFlipbookResetDelayTH, this, &UB2UICharacterIntro::ResetCharIntroFlipbookAnim, CharIntroFlipbookResetDelay, false);
		}
	}
	else
	{
		ResetCharIntroFlipbookAnim();
	}
}

void UB2UICharacterIntro::ResetCharIntroFlipbookAnim()
{
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		OwningPC->GetWorldTimerManager().ClearTimer(CharIntroFlipbookResetDelayTH);
	}

	UWorld* TheWorld = GetWorld();
	// UpdateCharIntroFlipbookMID 俊辑 捞 流饶俊 ExternalTime 颇扼固磐 诀单捞飘 矫 0 何磐 矫累.
	CharIntroFlipbookMIDTimeSync = TheWorld ? TheWorld->GetTimeSeconds() : 0.0f;
}

void UB2UICharacterIntro::UpdateCharIntroFlipbookMID()
{
	if (IMG_IntroFlipbook.IsValid())
		CurrCharIntroFlipbookMID = IMG_IntroFlipbook->GetDynamicMaterial();

	UWorld* TheWorld = GetWorld();
	if (CurrCharIntroFlipbookMID && TheWorld)
	{
		// 秦寸 material 捞 ExternalTime 阑 荤侩窍档废 技泼登绢 乐绢具 累悼且 巴. 弊霸 酒聪搁 material 郴何利栏肺 矫埃捞 磊悼 诀单捞飘 瞪 巴捞促.
		CurrCharIntroFlipbookMID->SetScalarParameterValue(FName(TEXT("ExternalTime")), TheWorld->GetTimeSeconds() - CharIntroFlipbookMIDTimeSync);
	}
}



//====================================================================================
//									Click Handler
//====================================================================================
void UB2UICharacterIntro::OnClickBtnConfirm()
{
	SetFirstCharacterViewState(1);
	LobbyProceedOnCharSelClass<>::GetInstance().Signal();
}
void UB2UICharacterIntro::OnClickBtnGladiator()
{
	SelectCharacter(EPCClass::EPC_Gladiator);
}
void UB2UICharacterIntro::OnClickBtnAssassin()
{
	SelectCharacter(EPCClass::EPC_Assassin);
}
void UB2UICharacterIntro::OnClickBtnWizard()
{
	SelectCharacter(EPCClass::EPC_Wizard);
}
void UB2UICharacterIntro::OnClickBtnFighter()
{
	SelectCharacter(EPCClass::EPC_Fighter);
}

void SetFirstCharacterViewState(int32 InState)
{
	GConfig->SetInt(TEXT("FirstCharacterView"), *FString::Printf(TEXT("isFirst")), InState, GB2UserSavedStateIni);
	GConfig->Flush(false, GB2UserSavedStateIni);
}
