// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIRaidDefeat.h"
#include "B2UIManager.h"
#include "B2UIResurrectBuffIcon.h"
#include "B2UIDocHelper.h"
#include "B2ClientDataStore.h"
#include "B2UIRepeatBattleCountDown.h"
#include "B2RaidGameMode.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

void UB2UIRaidDefeat::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UPanelWidget, P_RessurectButton);
	GET_SLOT(UPanelWidget, P_FailCountDown);
	GET_SLOT(UPanelWidget, P_NoResurrectMsg);
	GET_SLOT(UPanelWidget, P_Buff);

	GET_SLOT(UTextBlock, TB_CountDownNumber);
	GET_SLOT(UTextBlock, TB_ResurrectCost);
	GET_SLOT(UTextBlock, TB_Content_Upper);
	
	GET_SLOT(UTextBlock, STB_BtnQuit);
	GET_SLOT(UTextBlock, STB_BtnResurrect);
	GET_SLOT(UTextBlock, STB_Fail);
	GET_SLOT(UTextBlock, STB_Title);
	GET_SLOT(UTextBlock, STB_NoGem);
	GET_SLOT(UTextBlock, STB_LimitDesc1);
	GET_SLOT(UTextBlock, STB_LimitDesc2);

	GET_SLOT(UB2Button, BTN_Resurrect);
	GET_SLOT(UB2Button, BTN_Quit);

	GET_SLOT(UImage, IMG_Jewel);

	GET_SLOT(UB2UIResurrectBuffIcon, UIP_BuffIcon_Attack);
	if (UIP_BuffIcon_Attack.IsValid()) UIP_BuffIcon_Attack->Init();
	GET_SLOT(UB2UIResurrectBuffIcon, UIP_BuffIcon_Defense);
	if (UIP_BuffIcon_Defense.IsValid()) UIP_BuffIcon_Defense->Init();
	GET_SLOT(UB2UIResurrectBuffIcon, UIP_BuffIcon_Health);
	if (UIP_BuffIcon_Health.IsValid()) UIP_BuffIcon_Health->Init();
}

void UB2UIRaidDefeat::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Quit, &UB2UIRaidDefeat::OnClickBTN_Quit);
	BIND_CLICK_FUNC_TO_BTN(BTN_Resurrect, &UB2UIRaidDefeat::OnClickBTN_Resurrect);

	if (UIP_BuffIcon_Attack.IsValid()) {
		UIP_BuffIcon_Attack->OnSelectedOuterDelegate.BindDynamic(this, &UB2UIRaidDefeat::OnBuffIconPartClicked);
	}
	if (UIP_BuffIcon_Defense.IsValid()) {
		UIP_BuffIcon_Defense->OnSelectedOuterDelegate.BindDynamic(this, &UB2UIRaidDefeat::OnBuffIconPartClicked);
	}
	if (UIP_BuffIcon_Health.IsValid()) {
		UIP_BuffIcon_Health->OnSelectedOuterDelegate.BindDynamic(this, &UB2UIRaidDefeat::OnBuffIconPartClicked);
	}

	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
		DocRaid->OnFailCountDownTimeChanged.AddUObject(this, &UB2UIRaidDefeat::OnChangedFailCountDown);
}

void UB2UIRaidDefeat::Init()
{
	Super::Init();
	SelectedBuffType = EResurrectBuffType::RBT_None;
}

void UB2UIRaidDefeat::DestroySelf(UB2UIManager* InUIManager)
{
	if (UIP_BuffIcon_Attack.IsValid()) UIP_BuffIcon_Attack->DestroySelf(InUIManager);
	if (UIP_BuffIcon_Defense.IsValid()) UIP_BuffIcon_Defense->DestroySelf(InUIManager);
	if (UIP_BuffIcon_Health.IsValid()) UIP_BuffIcon_Health->DestroySelf(InUIManager);

	Super::DestroySelf(InUIManager);
}

void UB2UIRaidDefeat::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (STB_BtnQuit.IsValid())
		STB_BtnQuit->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DefeatMenu_Quit")));

	if (STB_BtnResurrect.IsValid())
		STB_BtnResurrect->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DefeatMenu_Resurrect")));

	if (STB_Title.IsValid())
		STB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DefeatMenu_Defeat")));

	if (STB_NoGem.IsValid())
		STB_NoGem->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_NotEnoughGems")));

	if (STB_Fail.IsValid())
		STB_Fail->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_FailComingsoon")));

	if (STB_LimitDesc1.IsValid())
		STB_LimitDesc1->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_ResurrectionDesc1")));

	if (STB_LimitDesc2.IsValid())
		STB_LimitDesc2->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_ResurrectionDesc3")));
}

void UB2UIRaidDefeat::BindDocAuto()
{
	UnbindDoc();

	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		SetOnResurrectEnableByRemainingChance(DocBattle->GetRemainingResurrectionChance() > 0);
		UpdateContentText(DocBattle->GetMaxResurrectionChance(), DocBattle->GetRemainingResurrectionChance());
		UpdateOnResurrectCost(DocBattle->GetResurrectionCost());

		DocBattle->OnRemainingResurrectionChanceChanged.AddUObject(this, &UB2UIRaidDefeat::OnChangeRemainingResurrectionChance);
		DocBattle->OnResurrectionCostChanged.AddUObject(this, &UB2UIRaidDefeat::OnChangeResurrectionCost);

		if (IsOnlyVisibleFailCount())
			SetVisibleFailCountOnly();
	}

	auto DocRaid = UB2UIDocHelper::GetDocRaid();
	if (DocRaid)
	{
		DocRaid->SetVisibleButtonForDefeat(false);
	}

	UpdateBuffIconContent();

	if (UIP_BuffIcon_Attack.IsValid())
		UIP_BuffIcon_Attack->SetSelected(true); // �⺻ Attack ���� ����. �ٸ� �͵��� ���� �ݹ��� ���� Unselect ��.
}

void UB2UIRaidDefeat::UnbindDoc()
{
	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		DocBattle->OnRemainingResurrectionChanceChanged.RemoveAll(this);
		DocBattle->OnResurrectionCostChanged.RemoveAll(this);
	}

	auto DocRaid = UB2UIDocHelper::GetDocRaid();
	if (DocRaid)
	{
		DocRaid->SetVisibleButtonForDefeat(true);
	}
}

bool UB2UIRaidDefeat::IsResurrectPossibleByCost()
{ // ���⼱ ���� ��븸 üũ�Ѵ�.
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
	if (DocBattle && DocUser)
	{
		return (DocUser->GetGem() >= DocBattle->GetResurrectionCost());
	}
	return false;
}

void UB2UIRaidDefeat::UpdateOnResurrectCost(int32 InValue)
{
	if (TB_ResurrectCost.IsValid())
		TB_ResurrectCost->SetText(FText::AsNumber(InValue));

	UpdateResurrctBtnEnableState();

	if (!IsResurrectPossibleByCost())
		ResurrectImpossibleByCostAction();
}

void UB2UIRaidDefeat::SetOnResurrectEnableByRemainingChance(bool InBool)
{
	// ���� ��Ȱ ������ ���� ���� ����. ĳ���� ���� ���� ���δ� UpdateOnResurrectCost ����.
	/*if (STB_BtnQuit.IsValid())
	{
		STB_BtnQuit->SetText(InBool ? BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("DefeatMenu_Quit"))) : BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("DefeatMenu_ReturnToLobby"))));
	}*/

	if (P_RessurectButton.IsValid()) // ���⼭ BTN_Resurrect �� Enable �� ���������� �ʴ´�. �װ� �ٸ� ��������..
		P_RessurectButton->SetVisibility(InBool ? ESlateVisibility::Visible : ESlateVisibility::Collapsed); 
	if(P_Buff.IsValid())
		P_Buff->SetVisibility(InBool ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	if (STB_LimitDesc1.IsValid())
		STB_LimitDesc1->SetVisibility(InBool ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

void UB2UIRaidDefeat::SetFailCountDown(int32 LimitTime)
{
	if (P_FailCountDown.IsValid())
		P_FailCountDown->SetVisibility((LimitTime > 0) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (TB_CountDownNumber.IsValid())
		TB_CountDownNumber->SetText(FText::AsNumber(LimitTime));
}

void UB2UIRaidDefeat::SetVisibleFailCountOnly()
{
	if (P_NoResurrectMsg.IsValid())
		P_NoResurrectMsg->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIRaidDefeat::UpdateContentText(int32 InMaxResurrectionChance, int32 InRemainingResurrectionChance)
{
	if (InRemainingResurrectionChance > 0)
	{
		// DefeatMenu_Desc �� �ش��ϴ� ���ڿ��� {0} ����.
		SetContentText(true,
			FText::Format(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("DefeatMenu_Resurrect_Desc"))),
				FText::AsNumber(InRemainingResurrectionChance), FText::AsNumber(InRemainingResurrectionChance), FText::AsNumber(InMaxResurrectionChance)
			));
	}
	else
	{
		SetContentText(false, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DefeatMenu_Desc_NoMoreChance")));
	}
}

void UB2UIRaidDefeat::ResurrectImpossibleByCostAction() 
{ // ��ȰȽ���� ������, ���� ������� ȭ�� ǥ��
	if (P_RessurectButton.IsValid())
		P_RessurectButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	SetEnableRessurectButton(false);

	if(P_Buff.IsValid())
		P_Buff->SetVisibility(ESlateVisibility::Collapsed);
	
	if (P_NoResurrectMsg.IsValid())
		P_NoResurrectMsg->SetVisibility(ESlateVisibility::Visible);
}

bool UB2UIRaidDefeat::IsOnlyVisibleFailCount()
{
	if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
	{
		if (auto* RaidGameMode = Cast<AB2RaidGameMode>(GetBladeIIGameMode(this)))
		{
			if (DocBattle->GetRemainingResurrectionChance() <= 0 && RaidGameMode->IsAllPlayerDead())
				return true;
		}
	}

	return false;
}

void UB2UIRaidDefeat::SetContentText(bool bTrueForUpper, const FText& InText)
{
	// �� ������ Upper / Center �� �� �ϳ��� ����ϴ� ��.
	if (TB_Content_Upper.IsValid())
	{
		TB_Content_Upper->SetVisibility(bTrueForUpper ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		if (bTrueForUpper)
		{
			TB_Content_Upper->SetText(InText);
		}
	}
}

void UB2UIRaidDefeat::UpdateBuffIconContent()
{
	// ó�� BindDoc ���� �ÿ�..

	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();

	if (DocBattle)
	{
		// UIDoc ���� ������ raw data �������� �־���� ���⼭ ClientDataStore ���ؼ� ���������� ǥ���� ���� ����.

		TMap<EResurrectBuffType, int32> SelectedBuffCount; // ���⿡�ٰ� ���� ���� ���� �����͸� �籸��.

		int32 SelectedAttack = DocBattle->GetSelectedResurrectBuffAttack();
		// ���� ���� 0�̴��� �־�� �Ѵ�. �׸� ��ü�� �־�� GetAppliedResurrectBuff ���� ���� ���ý� ���� ������� ��.
		SelectedBuffCount.Add(EResurrectBuffType::RBT_Attack, SelectedAttack);

		int32 SelectedDefense = DocBattle->GetSelectedResurrectBuffDefense();
		SelectedBuffCount.Add(EResurrectBuffType::RBT_Defense, SelectedDefense);

		int32 SelectedHealth = DocBattle->GetSelectedResurrectBuffHealth();
		SelectedBuffCount.Add(EResurrectBuffType::RBT_Health, SelectedHealth);

		TMap<EResurrectBuffType, int32> NextExpectedBuffValues; // �Ѹ� �����
																// �� ���¿� ���� ����Ǵ� ���� �ƴ� ���� ���� ���� ����� ������ ����.
		//BladeIIGameImpl::GetClientDataStore().GetAppliedResurrectBuff(DocBattle->GetResurrectGameModeType(), SelectedBuffCount, NextExpectedBuffValues, true);

		if (UIP_BuffIcon_Attack.IsValid())
		{
			int32* ExpectedValue = NextExpectedBuffValues.Find(EResurrectBuffType::RBT_Attack);
			UIP_BuffIcon_Attack->SetBuffInfo(EResurrectBuffType::RBT_Attack, ExpectedValue ? *ExpectedValue : 0);
		}
		if (UIP_BuffIcon_Defense.IsValid())
		{
			int32* ExpectedValue = NextExpectedBuffValues.Find(EResurrectBuffType::RBT_Defense);
			UIP_BuffIcon_Defense->SetBuffInfo(EResurrectBuffType::RBT_Defense, ExpectedValue ? *ExpectedValue : 0);
		}
		if (UIP_BuffIcon_Health.IsValid())
		{
			int32* ExpectedValue = NextExpectedBuffValues.Find(EResurrectBuffType::RBT_Health);
			UIP_BuffIcon_Health->SetBuffInfo(EResurrectBuffType::RBT_Health, ExpectedValue ? *ExpectedValue : 0);
		}
	}
}

void UB2UIRaidDefeat::OnClickBTN_Quit()
{
/*
	GoToVillageDefeatedClass<>::GetInstance().Signal();
	UB2UIManager::GetInstance()->CloseUI(UIFName::Defeat);
*/

	FText msg = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_PenaltyNotice2"));
	if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
	{
		if (DocRaid->GetAlreadyExitUser())
			msg = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_NonPenaltyNotice")); // �̹� Ż���� �������.

		DocRaid->SetReturnToPageType(ERaidGotoWhere::RaidMain);
	}

	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		msg,
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([]() {

		ReturnToRaidMainClass<>::GetInstance().Signal();
		UB2UIManager::GetInstance()->CloseUI(UIFName::Defeat);

	})
	);
}

void UB2UIRaidDefeat::OnClickBTN_Resurrect()
{
	if (SelectedBuffType != EResurrectBuffType::RBT_None && SelectedBuffType != EResurrectBuffType::RBT_End)
	{
		UB2UIManager::GetInstance()->CloseUI(UIFName::Defeat); // Resurrect ������ ���� �� �޴��� �ı��� �ű� ��..
		RequestResurrectClass<EResurrectBuffType>::GetInstance().Signal(SelectedBuffType);
	}
}

void UB2UIRaidDefeat::OnBuffIconPartClicked(EResurrectBuffType ClickedBuffType)
{
	SelectedBuffType = ClickedBuffType;

	// �ٸ� Ÿ���� �����ܵ��� ���� ����

	if (UIP_BuffIcon_Attack.IsValid() && ClickedBuffType != EResurrectBuffType::RBT_Attack)
	{
		UIP_BuffIcon_Attack->SetSelected(false);
	}
	if (UIP_BuffIcon_Defense.IsValid() && ClickedBuffType != EResurrectBuffType::RBT_Defense)
	{
		UIP_BuffIcon_Defense->SetSelected(false);
	}
	if (UIP_BuffIcon_Health.IsValid() && ClickedBuffType != EResurrectBuffType::RBT_Health)
	{
		UIP_BuffIcon_Health->SetSelected(false);
	}

	UpdateResurrctBtnEnableState();
}

void UB2UIRaidDefeat::UpdateResurrctBtnEnableState()
{ // ���� ��Ȱ Ƚ���� ���� ���� �׳� collapse �Ǿ����. ���⼱ chance ���θ� ������ ������ ���ǵ�.
	SetEnableRessurectButton(IsResurrectPossibleByCost() && SelectedBuffType != EResurrectBuffType::RBT_None && SelectedBuffType != EResurrectBuffType::RBT_End);
}

void UB2UIRaidDefeat::SetEnableRessurectButton(bool bEnable)
{
	if (BTN_Resurrect.IsValid())
		BTN_Resurrect->SetIsEnabled(bEnable);
	if (TB_ResurrectCost.IsValid())
		TB_ResurrectCost->SetColorAndOpacity(bEnable ? FLinearColor::White : UB2UIManager::GetInstance()->TextColor_Decrease);
	if (STB_NoGem.IsValid())
		STB_NoGem->SetVisibility(bEnable ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	if (STB_BtnResurrect.IsValid())
		STB_BtnResurrect->SetIsEnabled(bEnable);
	if (IMG_Jewel.IsValid())
		IMG_Jewel->SetIsEnabled(bEnable);
}

//====================================================================================
//									Doc Delegate
//====================================================================================
void UB2UIRaidDefeat::OnChangeRemainingResurrectionChance(class UB2UIDocBase* Sender, int32 RemainingResurrectionChance, int32 PrevRemainingResurrectionChance)
{
	SetOnResurrectEnableByRemainingChance(RemainingResurrectionChance > 0);

	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	UpdateContentText(DocBattle ? DocBattle->GetMaxResurrectionChance() : RemainingResurrectionChance, RemainingResurrectionChance);
}

void UB2UIRaidDefeat::OnChangeResurrectionCost(class UB2UIDocBase* Sender, int32 ResurrectionCost, int32 PrevResurrectionCost)
{
	UpdateOnResurrectCost(ResurrectionCost);
}

void UB2UIRaidDefeat::OnChangedFailCountDown(class UB2UIDocBase* Sender, int32 diff, int32 PrevDiff)
{
	SetFailCountDown(diff);
}