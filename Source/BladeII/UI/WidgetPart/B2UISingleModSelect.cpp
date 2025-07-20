#include "B2UISingleModSelect.h"
#include "B2UIManager.h"

UB2UISingleModSelect::UB2UISingleModSelect(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyMod = ELobbyModSceneElem::LMODIDX_End;
	bIsLocked = false;
	ModImageMaterial = NULL;
	LockedImageMaterial = NULL;
}

void UB2UISingleModSelect::CacheAssets()
{
	//////////////////////////
	//胶农费苞 努腐 捞亥飘甫 鞍捞 贸府且 荐 绝扁俊
	//滚瓢屈侥捞 酒囱 老馆 Widget input贸府甫 秦具等促. (滚瓢捞 input阑 贸府窍绊 滚覆.)
	//Input包访 贸府绰 NativeOnMouse/NativeOnTouch肺 贸府窃.
	GET_SLOT(UB2Button, BTN_Area)
	if (BTN_Area.IsValid())
		BTN_Area->SetVisibility(ESlateVisibility::Hidden);

	this->SetVisibility(ESlateVisibility::Visible);
	///////////////////////////

	GET_SLOT(UTextBlock, TB_ModName)
	GET_SLOT(UImage, IMG_ModMain)
	//GET_SLOT(UImage, IMG_ModLocked)
}

void UB2UISingleModSelect::UpdateStaticText()
{
}

void UB2UISingleModSelect::BindDelegates()
{
	//NativeOnMouseButtonUp / NativeOnTouchEnded 俊辑 贸府窃.
	//BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UISingleModSelect::OnClickBtnArea)
}

void UB2UISingleModSelect::SetMyModInfo(ELobbyModSceneElem InModEnum, bool bInLocked)
{
	MyMod = InModEnum;
	bIsLocked = bInLocked;

	if (BTN_Area.IsValid())
	{
		BTN_Area->SetIsEnabled(bIsLocked ? false : true);
	}
	if (IMG_ModMain.IsValid())
	{ // 眠饶 捞率俊绰 image 甫 硅摹窍瘤 臼霸 瞪 荐档. Lock 父 硅摹窍霸 瞪 荐 乐澜.
		IMG_ModMain->SetBrushFromMaterial(bIsLocked ? LockedImageMaterial : ModImageMaterial);
		IMG_ModMain->SetVisibility(ESlateVisibility::HitTestInvisible); // 趣 角荐且瘤 葛福聪
	}
	if (TB_ModName.IsValid())
	{
		TB_ModName->SetText(GetModNameText(MyMod));
		TB_ModName->SetVisibility(ESlateVisibility::HitTestInvisible); // 趣 角荐且瘤 葛福聪
	}
}

FText UB2UISingleModSelect::GetModNameText(ELobbyModSceneElem InModEnum)
{
	switch (InModEnum)
	{
	case ELobbyModSceneElem::LMODIDX_PVP: return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_PVP"));
	case ELobbyModSceneElem::LMODIDX_TeamMatch: return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_TeamMatch"));
	case ELobbyModSceneElem::LMODIDX_HeroTop: return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_HeroTop"));
	case ELobbyModSceneElem::LMODIDX_GuildMatch: return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_GuildMatch"));
	case ELobbyModSceneElem::LMODIDX_CounterDungeon: return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_DayDungeon"));
	case ELobbyModSceneElem::LMODIDX_Raid: return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_Raid"));
	case ELobbyModSceneElem::LMODIDX_Control: return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_Control"));
	}
	return FText::FromString(TEXT("Unknown Challenge"));
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UISingleModSelect::OnClickBtnArea()
{
	OnBtnAreaClickDelegate.ExecuteIfBound();
	PlayButtonSound();
}

//====================================================================================
//									Input Override
//====================================================================================
#include "B2LobbyGameMode.h"

FReply UB2UISingleModSelect::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bPressed = true;
		TimeInputStarted = FApp::GetCurrentTime();
	}

	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (LobbyGameMode)
		LobbyGameMode->SetPressedInputInUI(true);

	return FReply::Handled();
}

FReply UB2UISingleModSelect::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Unhandled();
}

FReply UB2UISingleModSelect::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bPressed && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && (FApp::GetCurrentTime() - TimeInputStarted) <= GetTimeToDecideInputClickOrTab())
	{
		OnClickBtnArea();
	}

	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (LobbyGameMode)
		LobbyGameMode->SetPressedInputInUI(false);

	bPressed = false;

	return FReply::Handled();
}

void UB2UISingleModSelect::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	bPressed = false;
}

FReply UB2UISingleModSelect::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	UE_LOG(LogBladeII, Log, TEXT("TouchStart bPressed = %s"), bPressed ? TEXT("true") : TEXT("false"));

	TimeInputStarted = FApp::GetCurrentTime();
	bPressed = true;

	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (LobbyGameMode)
	{
		return LobbyGameMode->TouchStartFromUI(InGeometry, InGestureEvent);
	}

	return FReply::Unhandled();
}

FReply UB2UISingleModSelect::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	UE_LOG(LogBladeII, Log, TEXT("TouchMove bPressed = %s"), bPressed ? TEXT("true") : TEXT("false"));
	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (LobbyGameMode)
	{
		return LobbyGameMode->TouchMoveFromUI(InGeometry, InGestureEvent);
	}

	return FReply::Unhandled();
}

FReply UB2UISingleModSelect::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	double ElapsedTime = FApp::GetCurrentTime() - TimeInputStarted;

	UE_LOG(LogBladeII, Log, TEXT("TouchEnd bPressed = %s"), bPressed ? TEXT("true") : TEXT("false"));
	UE_LOG(LogBladeII, Log, TEXT("ElapsedTime = %0.2f"), static_cast<float>(ElapsedTime));

	if (bPressed && ElapsedTime <= GetTimeToDecideInputClickOrTab())
	{
		UE_LOG(LogBladeII, Log, TEXT("Call OnClickBtnArea"));
		OnClickBtnArea();
	}

	bPressed = false;

	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(this));
	if (LobbyGameMode)
	{
		return LobbyGameMode->TouchEndFromUI(InGeometry, InGestureEvent);
	}

	return FReply::Unhandled();
}