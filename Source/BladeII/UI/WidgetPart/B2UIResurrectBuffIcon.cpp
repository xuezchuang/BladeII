#include "BladeII.h"
#include "B2UIResurrectBuffIcon.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"

UB2UIResurrectBuffIcon::UB2UIResurrectBuffIcon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyBuffType = EResurrectBuffType::RBT_None;
	MyBuffAmount = 0;
	bIsSelected = false;
		
	SetupForAttackBuff.StatTextColor = FSlateColor(FLinearColor(1.0f, 0.0f, 1.0f));
	SetupForDefenseBuff.StatTextColor = FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f));
	SetupForHeathBuff.StatTextColor = FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f));
}

void UB2UIResurrectBuffIcon::Init()
{
	Super::Init();
}

void UB2UIResurrectBuffIcon::CacheAssets()
{
	GET_SLOT(UB2Button, BTN_Area);
	GET_SLOT(UImage, IMG_MainIcon);
	GET_SLOT(UImage, IMG_Selected);
	GET_SLOT(UTextBlock, TB_BuffTypeLabel);
	GET_SLOT(UTextBlock, TB_BuffAmount);
}

void UB2UIResurrectBuffIcon::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Area, &UB2UIResurrectBuffIcon::OnClickBtnArea)
}

void UB2UIResurrectBuffIcon::SetBuffInfo(EResurrectBuffType InBuffType, int32 InBuffAmount)
{
	MyBuffType = InBuffType;
	MyBuffAmount = InBuffAmount;

	UpdateWidgets();
}

void UB2UIResurrectBuffIcon::SetSelected(bool bSelect)
{
	bIsSelected = bSelect;

	if (bIsSelected)
	{
		// 다른 타입을 끄기 위해 외부로 이벤트 전파.
		OnSelectedOuterDelegate.ExecuteIfBound(MyBuffType);
	}

	UpdateWidgets();
}

void UB2UIResurrectBuffIcon::UpdateWidgets()
{
	if (MyBuffType == EResurrectBuffType::RBT_None || MyBuffType == EResurrectBuffType::RBT_End)
	{
		if (IMG_MainIcon.IsValid()){
			IMG_MainIcon->SetVisibility(ESlateVisibility::Hidden);
		}
		if (TB_BuffTypeLabel.IsValid()){
			TB_BuffTypeLabel->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		if (IMG_MainIcon.IsValid())
		{
			IMG_MainIcon->SetVisibility(ESlateVisibility::HitTestInvisible);

			FResurrectBuffIconPerTypeSetup& DesiredSetup = (MyBuffType == EResurrectBuffType::RBT_Attack) ? SetupForAttackBuff
				: (MyBuffType == EResurrectBuffType::RBT_Defense ? SetupForDefenseBuff : SetupForHeathBuff);

			IMG_MainIcon->SetBrushFromMaterial(bIsSelected ? DesiredSetup.IconMaterial : DesiredSetup.IconMaterial_NonSelected);
		}
		if (TB_BuffTypeLabel.IsValid())
		{
			TB_BuffTypeLabel->SetVisibility(ESlateVisibility::HitTestInvisible);
			TB_BuffTypeLabel->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL,
				MyBuffType == EResurrectBuffType::RBT_Attack ? TEXT("LobbyStatText_Attack")
				: (MyBuffType == EResurrectBuffType::RBT_Defense ? TEXT("LobbyStatText_Defense")
					: TEXT("LobbyStatText_Health"))
				));
		}
	}

	if (TB_BuffAmount.IsValid())
	{
		TB_BuffAmount->SetVisibility(MyBuffAmount >= 0 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		TB_BuffAmount->SetText(FText::FromString(FString::Printf(TEXT("+%d%%"), MyBuffAmount)));

		TB_BuffAmount->SetColorAndOpacity(
			MyBuffType == EResurrectBuffType::RBT_Attack ? SetupForAttackBuff.StatTextColor
			: (MyBuffType == EResurrectBuffType::RBT_Defense ? SetupForDefenseBuff.StatTextColor
				: SetupForHeathBuff.StatTextColor)
			);
	}

	if (IMG_Selected.IsValid())
	{
		IMG_Selected->SetVisibility(bIsSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
		OnPlaySelectAnimation_IMP(bIsSelected);
	}
}

void UB2UIResurrectBuffIcon::OnClickBtnArea()
{
	if (MyBuffType != EResurrectBuffType::RBT_None || MyBuffType != EResurrectBuffType::RBT_End)
	{
		SetSelected(true);  // On/Off 토글이 아닌 라디오버튼처럼 선택만. 대신 SetSelected 에서 다른 것들을 끄기 위해 외부로 이벤트를 전파할 것.
	}
}