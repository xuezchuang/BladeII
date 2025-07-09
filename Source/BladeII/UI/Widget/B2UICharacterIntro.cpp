#include "BladeII.h"
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

	SelectCharacter(BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass(), true);	//디폴트 선택. 오픈시 캐릭터 선택시에는 SkillPV 애님을 씹어준다. 왜냐하면 OpenAnim에 SkillPV애님이 중복되어 있기 때문....;;;

	WrappedResetCharIntroFlipbookAnim(); // 첫 프레임부터 시작.
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
			SomeInfo->UnloadAllPCIntroFlipbookMtrl(); // 얘들이 큰 텍스쳐 덩어리라 메모리를 상당히 먹을 것. 이 이후엔 필요 없으니 날림.
		}
	}
}

void UB2UICharacterIntro::SelectCharacter(EPCClass CharType, bool bCancelEffectOpeningSkillPV)
{
	//CharacterIntro에서 선택한 영웅은 그냥 캐릭터 소개만 보여지는 용도
	LobbyCharacterIntroSetPCSelectionClass<EPCClass>::GetInstance().Signal(CharType);
	SelectedHeroDoc = UB2UIDocHelper::GetDocHero((int32)CharType);

	WrappedResetCharIntroFlipbookAnim(); // 캐릭터가 바뀌면 Flipbook material 도 바뀔 테니 첫 프레임부터 시작하도록 리셋.

	SelectCharacter_BP(CharType, bCancelEffectOpeningSkillPV);	//BP쪽에서 처리 할 연출들

	// Flipbook 동영상 material 세팅. 처음 로딩 시 블로킹이 있을 수 있다.
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
	// 걍 ResetCharIntroFlipbookAnim 즉시 호출 아니면 타이머. Flipbook anim image 애니메이션 때문에.
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
	// UpdateCharIntroFlipbookMID 에서 이 직후에 ExternalTime 파라미터 업데이트 시 0 부터 시작.
	CharIntroFlipbookMIDTimeSync = TheWorld ? TheWorld->GetTimeSeconds() : 0.0f;
}

void UB2UICharacterIntro::UpdateCharIntroFlipbookMID()
{
	if (IMG_IntroFlipbook.IsValid())
		CurrCharIntroFlipbookMID = IMG_IntroFlipbook->GetDynamicMaterial();

	UWorld* TheWorld = GetWorld();
	if (CurrCharIntroFlipbookMID && TheWorld)
	{
		// 해당 material 이 ExternalTime 을 사용하도록 세팅되어 있어야 작동할 것. 그게 아니면 material 내부적으로 시간이 자동 업데이트 될 것이다.
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
