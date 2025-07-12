#include "B2UIDefeat.h"
#include "B2UIManager.h"
#include "B2UIDocHelper.h"
#include "B2ClientDataStore.h"
#include "B2UIShortagePopupManager.h"
#include "B2UIRepeatBattleCountDown.h"
#include "B2RaidGameMode.h"
#include "Retailer.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"

UB2UIDefeat::UB2UIDefeat(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SelectedBuffType = EResurrectBuffType::RBT_None;
}

void UB2UIDefeat::Init()
{
	Super::Init();

	SubscribeEvents();
}

void UB2UIDefeat::CacheAssets()
{
	GET_SLOT(UOverlay, O_Main);
	GET_SLOT(UButton, BTN_Quit);
	GET_SLOT(UTextBlock, TB_BtnQuit);
	GET_SLOT(UButton, BTN_Confirm);
	GET_SLOT(UTextBlock, TB_BtnConfirm);
	GET_SLOT(UButton, BTN_Resurrect);
	GET_SLOT(UTextBlock, TB_BtnResurrect);
	GET_SLOT(UTextBlock, TB_ResurrectCost);
	GET_SLOT(UTextBlock, TB_Title);
	GET_SLOT(UTextBlock, TB_Content_Upper);
	GET_SLOT(UImage, IMG_UpperTextBG);

	GET_SLOT(UTextBlock, STB_NoGem);
	GET_SLOT(UPanelWidget, P_RessurectButton);
	GET_SLOT(UImage, IMG_Jewel);

	GET_SLOT(UB2UIResurrectBuffIcon, UIP_BuffIcon_Attack);
	if (UIP_BuffIcon_Attack.IsValid()) {
		UIP_BuffIcon_Attack->Init();
	}
	GET_SLOT(UB2UIResurrectBuffIcon, UIP_BuffIcon_Defense);
	if (UIP_BuffIcon_Defense.IsValid()) {
		UIP_BuffIcon_Defense->Init();
	}
	GET_SLOT(UB2UIResurrectBuffIcon, UIP_BuffIcon_Health);
	if (UIP_BuffIcon_Health.IsValid()) {
		UIP_BuffIcon_Health->Init();
	}

	GET_SLOT(UOverlay, O_FailCountDown);
	GET_SLOT(UTextBlock, TB_CountDownNumber);

	GET_SLOT(UPanelWidget, P_Content);
	GET_SLOT(UPanelWidget, P_FailContent);
	GET_SLOT(UTextBlock, TB_Arin);
	GET_SLOT(UTextBlock, TB_ConsultMessage);

	GET_SLOT(UB2Button, BTN_ItemEnhance);
	GET_SLOT(UTextBlock, TB_ItemEnhance);

	GET_SLOT(UB2Button, BTN_SkillEnhance);
	GET_SLOT(UTextBlock, TB_SkillEnhance);

	GET_SLOT(UB2Button, BTN_ItemPurchase);
	GET_SLOT(UTextBlock, TB_ItemPurchase);
}

void UB2UIDefeat::UpdateStaticText()
{
	if (TB_BtnQuit.IsValid())
		TB_BtnQuit->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DefeatMenu_Quit")));

	if (TB_BtnResurrect.IsValid())
		TB_BtnResurrect->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DefeatMenu_Resurrect")));

	if (TB_Title.IsValid())
		TB_Title->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DefeatMenu_Defeat")));

	if (TB_Arin.IsValid())
		TB_Arin->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ArinConsult_MissySexyArin")));

	if (TB_ConsultMessage.IsValid())
		TB_ConsultMessage->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ArinConsult_PostDefeat_TryAgainMessage")));

	if (TB_ItemEnhance.IsValid())
		TB_ItemEnhance->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ArinConsult_EnhanceItem")));

	if (TB_SkillEnhance.IsValid())
		TB_SkillEnhance->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ArinConsult_EnhanceSkill")));

	if (TB_ItemPurchase.IsValid())
		TB_ItemPurchase->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ArinConsult_PurchaseItem")));
	
	if (STB_NoGem.IsValid())
		STB_NoGem->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_NotEnoughGems")));

	UpdateStaticTextByGameMode();
}

void UB2UIDefeat::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Quit, &UB2UIDefeat::OnClickBtnQuit);
	BIND_CLICK_FUNC_TO_BTN(BTN_Resurrect, &UB2UIDefeat::OnClickBtnResurrect);
	BIND_CLICK_FUNC_TO_BTN(BTN_Confirm, &UB2UIDefeat::OnClickBtnConfirm);

	BIND_CLICK_FUNC_TO_BTN(BTN_ItemEnhance, &UB2UIDefeat::OnClickBtnItemEnhance);
	BIND_CLICK_FUNC_TO_BTN(BTN_SkillEnhance, &UB2UIDefeat::OnClickBtnSkillEnhance);
	BIND_CLICK_FUNC_TO_BTN(BTN_ItemPurchase, &UB2UIDefeat::OnClickBtnItemPurchase);

	if (UIP_BuffIcon_Attack.IsValid()) {
		UIP_BuffIcon_Attack->OnSelectedOuterDelegate.BindDynamic(this, &UB2UIDefeat::OnBuffIconPartClicked);
	}
	if (UIP_BuffIcon_Defense.IsValid()) {
		UIP_BuffIcon_Defense->OnSelectedOuterDelegate.BindDynamic(this, &UB2UIDefeat::OnBuffIconPartClicked);
	}
	if (UIP_BuffIcon_Health.IsValid()) {
		UIP_BuffIcon_Health->OnSelectedOuterDelegate.BindDynamic(this, &UB2UIDefeat::OnBuffIconPartClicked);
	}

	if (GetB2GameModeType(this) == EB2GameMode::Raid)
	{
		if (UB2UIDocRaid* DocRaid = UB2UIDocHelper::GetDocRaid())
			DocRaid->OnFailCountDownTimeChanged.AddUObject(this, &UB2UIDefeat::OnChangedFailCountDown);
	}
}

void UB2UIDefeat::BindDocAuto()
{
	UnbindDoc();

	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		SetOnResurrectEnableByRemainingChance(DocBattle->GetRemainingResurrectionChance() > 0);
		UpdateContentText(DocBattle->GetMaxResurrectionChance(), DocBattle->GetRemainingResurrectionChance());
		UpdateOnResurrectCost(DocBattle->GetResurrectionCost());

		DocBattle->OnRemainingResurrectionChanceChanged.AddUObject(this, &UB2UIDefeat::OnChangeRemainingResurrectionChance);
		DocBattle->OnResurrectionCostChanged.AddUObject(this, &UB2UIDefeat::OnChangeResurrectionCost);

		if (IsOnlyVisibleFailCountGameMode())
			SetVisibleFailCountOnly();
	}

	if (GetB2GameModeType(this) == EB2GameMode::Raid)
	{
		auto DocRaid = UB2UIDocHelper::GetDocRaid();
		if (DocRaid)
		{
			DocRaid->SetVisibleButtonForDefeat(false);
		}
	}

	UpdateBuffIconContent();

	if (UIP_BuffIcon_Attack.IsValid())
		UIP_BuffIcon_Attack->SetSelected(true); // �⺻ Attack ���� ����. �ٸ� �͵��� ���� �ݹ��� ���� Unselect ��.
}

void UB2UIDefeat::UnbindDoc()
{
	auto DocBattle = UB2UIDocHelper::GetDocBattle();
	if (DocBattle)
	{
		DocBattle->OnRemainingResurrectionChanceChanged.RemoveAll(this);
		DocBattle->OnResurrectionCostChanged.RemoveAll(this);
	}

	if (GetB2GameModeType(this) == EB2GameMode::Raid)
	{
		auto DocRaid = UB2UIDocHelper::GetDocRaid();
		if (DocRaid)
		{
			DocRaid->SetVisibleButtonForDefeat(true);
		}
	}
}

void UB2UIDefeat::SubscribeEvents()
{
	UnsubscribeEvents();

	//Issues.Add(DeliveryGetHeroTowerClass<FB2ResponseGetHeroTowerPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseGetHeroTowerPtr& msgPtr)
	//{
	//	UB2UIDocHeroTower* pDoc = UB2UIDocHelper::GetDocHeroTower();

	//	if (pDoc)
	//		pDoc->SetGetHeroTowerPtr(msgPtr);

	//	ReturnToHeroTowerMainMenuClass<>::GetInstance().Signal();
	//	UB2UIManager::GetInstance()->CloseUI(UIFName::Defeat);
	//}
	//));

	//Issues.Add(DeliveryGetLotteryShopClass<FB2ResponseGetLotteryShopPtr>::GetInstance().Subscribe2(
	//	[this](const FB2ResponseGetLotteryShopPtr& msgPtr)
	//{
	//	this->ResponseSetSummonItemUIData(msgPtr);
	//}
	//));
}

void UB2UIDefeat::UnsubscribeEvents()
{
	Issues.Empty();
}

bool UB2UIDefeat::IsResurrectPossibleByCost()
{ // ���⼱ ���� ��븸 üũ�Ѵ�.
	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	UB2UIDocUser* DocUser = UB2UIDocHelper::GetDocUser();
	if (DocBattle && DocUser)
	{
		return (DocUser->GetGem() >= DocBattle->GetResurrectionCost());
	}
	return false;
}

void UB2UIDefeat::UpdateOnResurrectCost(int32 InValue)
{
	if (TB_ResurrectCost.IsValid())
		TB_ResurrectCost->SetText(FText::AsNumber(InValue));
	
	UpdateResurrctBtnEnableState(); // �� �ȿ��� ���� ���� �Բ� ��Ÿ ������ üũ.. �ٸ� �������� ����ϴ��� ����� �Ѿ�� ���ڸ� ���� �ʱ��..

	// ���� �ִ� �� ��Ȱ�� �������
	// ���� ������ ��Ȱ ��ư�� ��Ȱ��ȭ�ϰ� ���� �޽����� ����.
	if (!IsResurrectPossibleByCost())
	{
		ResurrectImpossibleByCostAction();

		// �������� �ٷ� �Ƹ��� ����(��ȰȽ�� 0�������� �����°�) ����ش�.
		// + ���� ��Ȱ Ƚ�� : <- �ؽ�Ʈ ��ϵ� ����
		SetOnResurrectEnableByRemainingChance(false);
		SetContentText(false, FText::GetEmpty());
	}
}

void UB2UIDefeat::SetOnResurrectEnableByRemainingChance(bool InBool)
{
	// ���� ��Ȱ ������ ���� ���� ����. ĳ���� ���� ���� ���δ� UpdateOnResurrectCost ����.
	if (TB_BtnQuit.IsValid())
	{
		if (GetB2GameModeType(this) == EB2GameMode::CounterDungeon || GetB2GameModeType(this) == EB2GameMode::HeroTower)
			TB_BtnQuit->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("DefeatMenu_Quit"))));
		else
			TB_BtnQuit->SetText(InBool ? BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("DefeatMenu_Quit"))) : BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("DefeatMenu_ReturnToLobby"))));
	}

	if (P_RessurectButton.IsValid()) // ���⼭ BTN_Resurrect �� Enable �� ���������� �ʴ´�. �װ� �ٸ� ��������..
		P_RessurectButton->SetVisibility(InBool ? ESlateVisibility::Visible : ESlateVisibility::Collapsed); // �̰� Collapsed �� �ϸ� BTN_Quit �� �߾����� ������..

	if (BTN_Confirm.IsValid())
		BTN_Confirm->SetVisibility(ESlateVisibility::Collapsed);
	
	if (InBool == false)
		ImpossibleResurrectChance_BP();

	SetVisibleReaminChanceContent(InBool);
}

void UB2UIDefeat::SetFailCountDown(int32 LimitTime)
{
	if (O_FailCountDown.IsValid())
		O_FailCountDown->SetVisibility((LimitTime > 0) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (TB_CountDownNumber.IsValid())
		TB_CountDownNumber->SetText(FText::AsNumber(LimitTime));
}

void UB2UIDefeat::SetVisibleFailCountOnly()
{
	// ���̵� ���� ��忡�� Ư����Ȳ���� ī��Ʈ�� �����ִ� ó���� �ʿ��� ��� ���
	if (O_Main.IsValid())
		O_Main->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIDefeat::UpdateContentText(int32 InMaxResurrectionChance, int32 InRemainingResurrectionChance)
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
		switch (GetB2GameModeType(this))
		{
		case EB2GameMode::CounterDungeon:
		case EB2GameMode::HeroTower:
		case EB2GameMode::Dimension:
			SetContentText(false, FText::GetEmpty());
			break;
		default:
			SetContentText(false, BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DefeatMenu_Desc_NoMoreChance")));
			break;
		}
	}
}

void UB2UIDefeat::UpdateStaticTextByGameMode()
{
	switch (GetB2GameModeType(this))
	{
	case EB2GameMode::Raid:
		if (TB_BtnConfirm.IsValid())
			TB_BtnConfirm->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DefeatMenu_Observe")));
		break;
	default:
		if (TB_BtnConfirm.IsValid())
			TB_BtnConfirm->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DefeatMenu_Confirm")));
		break;
	}
}

void UB2UIDefeat::ResurrectImpossibleByCostAction()
{
	switch (GetB2GameModeType(this))
	{
	case EB2GameMode::Raid:
	case EB2GameMode::CounterDungeon:
	case EB2GameMode::Scenario:
	case EB2GameMode::HeroTower:
	case EB2GameMode::Dimension:
		break;
	default:
		ShortageMGR->PopupGoToShopForGemMessage();
		break;
	}

	ImpossibleResurrectCost_BP();
}

bool UB2UIDefeat::CanChargeGemByShop()
{
	switch (GetB2GameModeType(this))
	{
	case EB2GameMode::Raid:
	case EB2GameMode::CounterDungeon:
	case EB2GameMode::HeroTower:
	case EB2GameMode::Dimension:
		return false;
	}

	return true;
}

bool UB2UIDefeat::IsOnlyVisibleFailCountGameMode()
{
	switch (GetB2GameModeType(this))
	{
	case EB2GameMode::Raid:
	{
		// ���̵忡���� ��� �׾��ְ�, �ڱ��ڽ��� 3��(Max) ��Ȱ�� �Ͽ����� ���� ī��Ʈ�ٿ� �ѹ��� ǥ�����ش�.
		if (auto* DocBattle = UB2UIDocHelper::GetDocBattle())
		{
			if (auto* RaidGameMode = Cast<AB2RaidGameMode>(GetBladeIIGameMode(this)))
			{
				if (DocBattle->GetRemainingResurrectionChance() <= 0 && RaidGameMode->IsAllPlayerDead())
					return true;
			}
		}
	}
	break;
	}

	return false;
}

void UB2UIDefeat::SetContentText(bool bTrueForUpper, const FText& InText)
{
	// �� ������ Upper / Center �� �� �ϳ��� ����ϴ� ��.
	if (TB_Content_Upper.IsValid())
	{
		TB_Content_Upper->SetVisibility(bTrueForUpper ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		if (bTrueForUpper)
		{
			TB_Content_Upper->SetText(InText);
		}
		if (IMG_UpperTextBG.IsValid())
		{
			IMG_UpperTextBG->SetVisibility(bTrueForUpper ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		}
	}
}

void UB2UIDefeat::SetVisibleReaminChanceContent(bool bVisible)
{
	if (P_FailContent.IsValid())
		P_FailContent->SetVisibility(!bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (P_Content.IsValid())
		P_Content->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UB2UIDefeat::UpdateBuffIconContent()
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

void UB2UIDefeat::UpdateResurrctBtnEnableState()
{ // ���� ��Ȱ Ƚ���� ���� ���� �׳� collapse �Ǿ����. ���⼱ chance ���θ� ������ ������ ���ǵ�.
	SetEnableRessurectButton(IsResurrectPossibleByCost() && SelectedBuffType != EResurrectBuffType::RBT_None && SelectedBuffType != EResurrectBuffType::RBT_End);
}

void UB2UIDefeat::SetEnableRessurectButton(bool bEnable)
{
	if (BTN_Resurrect.IsValid())
		BTN_Resurrect->SetIsEnabled(bEnable);
	if (TB_BtnResurrect.IsValid())
		TB_BtnResurrect->SetIsEnabled(bEnable);
	if (IMG_Jewel.IsValid())
		IMG_Jewel->SetIsEnabled(bEnable);

	if (TB_ResurrectCost.IsValid())
		TB_ResurrectCost->SetColorAndOpacity(bEnable ? FLinearColor::White : UB2UIManager::GetInstance()->TextColor_Decrease);
	if (STB_NoGem.IsValid())
		STB_NoGem->SetVisibility(bEnable ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

//====================================================================================
//									Click Handler
//====================================================================================
void UB2UIDefeat::OnClickBtnQuit()
{
	//if (GetB2GameModeType(this) == EB2GameMode::HeroTower)
	//{
	//	data_trader::Retailer::GetInstance().RequestGetHeroTower();
	//}
	//else
	//{
	//	GoToVillageDefeatedClass<>::GetInstance().Signal();
	//	UB2UIManager::GetInstance()->CloseUI(UIFName::Defeat);
	//}
}

void UB2UIDefeat::OnClickBtnResurrect()
{
	//if (SelectedBuffType != EResurrectBuffType::RBT_None && SelectedBuffType != EResurrectBuffType::RBT_End)
	//{
	//	UB2UIManager::GetInstance()->CloseUI(UIFName::Defeat); // Resurrect ������ ���� �� �޴��� �ı��� �ű� ��..
	//	RequestResurrectClass<EResurrectBuffType>::GetInstance().Signal(SelectedBuffType);
	//}
}

void UB2UIDefeat::OnClickBtnConfirm()
{
	DestroySelf(UB2UIManager::GetInstance());
}

void UB2UIDefeat::DestroySelf(class UB2UIManager* InUIManager)
{
	UnsubscribeEvents();

	Super::DestroySelf(InUIManager);
}

void UB2UIDefeat::OnBuffIconPartClicked(EResurrectBuffType ClickedBuffType)
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

void UB2UIDefeat::OnClickBtnItemEnhance()
{
	//UB2UIManager::GetInstance()->CloseUI(UIFName::Defeat);

	//FLobbySceneManager::DeferredRegistChangeLobbyScene([]() {
	//	// ��ü �� ��ȯ�� �ϸ� �� �ε� �ð��� �þ�Ƿ� ���ϴ� ȭ�� �������� UI History �� ������ش�. 
	//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//	if (UIMgrInst) {
	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
	//	}
	//	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
	//});

	//OpenBladeIILobbyCommon(Cast<AGameMode>(UGameplayStatics::GetGameMode(this))); // �� ��ȯ ���� �� ���� �κ� �� �ε�
}

void UB2UIDefeat::OnClickBtnSkillEnhance()
{
	//UB2UIManager::GetInstance()->CloseUI(UIFName::Defeat);
	//
	//FLobbySceneManager::DeferredRegistChangeLobbyScene([]() {
	//	// ��ü �� ��ȯ�� �ϸ� �� �ε� �ð��� �þ�Ƿ� ���ϴ� ȭ�� �������� UI History �� ������ش�. 
	//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//	if (UIMgrInst) {
	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
	//	}
	//	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Skill);
	//});

	//OpenBladeIILobbyCommon(Cast<AGameMode>(UGameplayStatics::GetGameMode(this))); // �� ��ȯ ���� �� ���� �κ� �� �ε�
}

void UB2UIDefeat::OnClickBtnItemPurchase()
{
	data_trader::Retailer::GetInstance().RequestGetLotteryShop();
}

void UB2UIDefeat::ResponseSetSummonItemUIData(const FB2ResponseGetLotteryShopPtr& ShopDataPtr)
{
	for (auto TimeData : ShopDataPtr->shop_lotteries)
	{
		int32 CostType = TimeData->draw_price_type - b2network::B2ShopDrawPriceType::GEM;
		int32 HashKey = FSummonItemSlotData::GetHashKey(CostType, TimeData->is_continuous, false);

		BladeIIGameImpl::GetClientDataStore().SetStoreSummonItemFreeTime(HashKey, TimeData->next_free_time);
		BladeIIGameImpl::GetClientDataStore().SetStoreSummonItemDailyCount(HashKey, TimeData->lottery_count);

		HashKey = FSummonItemSlotData::GetHashKey(CostType, TimeData->is_continuous, true);

		BladeIIGameImpl::GetClientDataStore().SetStoreSummonItemFreeTime(HashKey, TimeData->next_free_time);
		BladeIIGameImpl::GetClientDataStore().SetStoreSummonItemDailyCount(HashKey, TimeData->lottery_count);
	}

	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		DocStore->SetSummonItemMileage(ShopDataPtr->mileage);
		DocStore->SetSummonItemMileageReceivedIndex(ShopDataPtr->reward_index - 1);
	}

	GotoSummonItemStore();
}

void UB2UIDefeat::GotoSummonItemStore()
{
	//UB2UIManager::GetInstance()->CloseUI(UIFName::Defeat);

	//FLobbySceneManager::DeferredRegistChangeLobbyScene([]() {
	//	// ��ü �� ��ȯ�� �ϸ� �� �ε� �ð��� �þ�Ƿ� ���ϴ� ȭ�� �������� UI History �� ������ش�. 
	//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//	if (UIMgrInst) {
	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
	//	}
	//	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::SummonItemStore);
	//});

	//OpenBladeIILobbyCommon(Cast<AGameMode>(UGameplayStatics::GetGameMode(this))); // �� ��ȯ ���� �� ���� �κ� �� �ε�
}

void UB2UIDefeat::CloseWidgetDelegate()
{
	OnClickBtnQuit();
}

//====================================================================================
//									Doc Delegate
//====================================================================================
void UB2UIDefeat::OnChangeRemainingResurrectionChance(class UB2UIDocBase* Sender, int32 RemainingResurrectionChance, int32 PrevRemainingResurrectionChance)
{
	SetOnResurrectEnableByRemainingChance(RemainingResurrectionChance > 0);

	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
	UpdateContentText(DocBattle ? DocBattle->GetMaxResurrectionChance() : RemainingResurrectionChance, RemainingResurrectionChance);
}

void UB2UIDefeat::OnChangeResurrectionCost(class UB2UIDocBase* Sender, int32 ResurrectionCost, int32 PrevResurrectionCost)
{
	UpdateOnResurrectCost(ResurrectionCost);
}

void UB2UIDefeat::OnChangedFailCountDown(class UB2UIDocBase* Sender, int32 diff, int32 PrevDiff)
{
	SetFailCountDown(diff);
}