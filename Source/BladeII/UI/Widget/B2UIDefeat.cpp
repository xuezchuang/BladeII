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
		UIP_BuffIcon_Attack->SetSelected(true); // 扁夯 Attack 栏肺 急琶. 促弗 巴甸篮 弊率 妮归阑 烹秦 Unselect 凳.
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
{ // 咯扁急 窜瘤 厚侩父 眉农茄促.
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
	
	UpdateResurrctBtnEnableState(); // 捞 救俊急 蝶肺 捣苞 窃膊 扁鸥 炼扒阑 眉农.. 促弗 镑俊辑档 荤侩窍蠢扼 咯扁肺 逞绢柯 牢磊甫 静瘤 臼扁肺..

	// 捣捞 乐绰 磊 何劝阑 且瘤绢促
	// 捣捞 绝栏搁 何劝 滚瓢篮 厚劝己拳窍绊 惑痢 皋矫瘤甫 剁款促.
	if (!IsResurrectPossibleByCost())
	{
		ResurrectImpossibleByCostAction();

		// 捣绝栏搁 官肺 酒赴狼 炼攫(何劝冉荐 0巢疽阑锭 唱坷绰芭) 剁况霖促.
		// + 巢篮 何劝 冉荐 : <- 咆胶飘 喉废档 见辫
		SetOnResurrectEnableByRemainingChance(false);
		SetContentText(false, FText::GetEmpty());
	}
}

void UB2UIDefeat::SetOnResurrectEnableByRemainingChance(bool InBool)
{
	// 咯变 何劝 蛮胶俊 蝶弗 啊瓷 咯何. 某浆俊 蝶弗 啊瓷 咯何绰 UpdateOnResurrectCost 俊辑.
	if (TB_BtnQuit.IsValid())
	{
		if (GetB2GameModeType(this) == EB2GameMode::CounterDungeon || GetB2GameModeType(this) == EB2GameMode::HeroTower)
			TB_BtnQuit->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("DefeatMenu_Quit"))));
		else
			TB_BtnQuit->SetText(InBool ? BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("DefeatMenu_Quit"))) : BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("DefeatMenu_ReturnToLobby"))));
	}

	if (P_RessurectButton.IsValid()) // 咯扁辑 BTN_Resurrect 狼 Enable 阑 技泼窍瘤绰 臼绰促. 弊扒 促弗 炼扒栏肺..
		P_RessurectButton->SetVisibility(InBool ? ESlateVisibility::Visible : ESlateVisibility::Collapsed); // 捞吧 Collapsed 肺 窍搁 BTN_Quit 捞 吝居栏肺 坷摆瘤..

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
	// 饭捞靛 鞍篮 葛靛俊辑 漂沥惑炔俊辑 墨款飘父 焊咯林绰 贸府啊 鞘夸且 版快 荤侩
	if (O_Main.IsValid())
		O_Main->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UIDefeat::UpdateContentText(int32 InMaxResurrectionChance, int32 InRemainingResurrectionChance)
{
	if (InRemainingResurrectionChance > 0)
	{
		// DefeatMenu_Desc 俊 秦寸窍绰 巩磊凯俊 {0} 器窃.
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
		// 饭捞靛俊辑绰 葛滴 磷绢乐绊, 磊扁磊脚捞 3锅(Max) 何劝阑 窍看栏搁 角菩 墨款飘促款 逞滚父 钎矫秦霖促.
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
	// 泅 备泅篮 Upper / Center 笛 吝 窍唱父 荤侩窍绰 芭.
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
	// 贸澜 BindDoc 技泼 矫俊..

	UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();

	if (DocBattle)
	{
		// UIDoc 俊绰 老辆狼 raw data 己拜栏肺 持绢初绊 咯扁辑 ClientDataStore 烹秦辑 弥辆利栏肺 钎矫且 蔼阑 掘绢咳.

		TMap<EResurrectBuffType, int32> SelectedBuffCount; // 咯扁俊促啊 滚橇 急琶 惑怕 单捞磐甫 犁备己.

		int32 SelectedAttack = DocBattle->GetSelectedResurrectBuffAttack();
		// 汲飞 蔼捞 0捞歹扼档 持绢具 茄促. 亲格 磊眉绰 乐绢具 GetAppliedResurrectBuff 俊辑 促澜 急琶矫 蔼阑 拌魂秦临 巴.
		SelectedBuffCount.Add(EResurrectBuffType::RBT_Attack, SelectedAttack);

		int32 SelectedDefense = DocBattle->GetSelectedResurrectBuffDefense();
		SelectedBuffCount.Add(EResurrectBuffType::RBT_Defense, SelectedDefense);

		int32 SelectedHealth = DocBattle->GetSelectedResurrectBuffHealth();
		SelectedBuffCount.Add(EResurrectBuffType::RBT_Health, SelectedHealth);

		TMap<EResurrectBuffType, int32> NextExpectedBuffValues; // 谎副 搬苞蔼
		// 泅 惑怕俊 蝶扼 利侩登绰 蔼捞 酒囱 促澜 急琶 捞饶 利侩瞪 蔼栏肺 掘绢咳.
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
{ // 巢篮 何劝 冉荐啊 绝绰 版快绰 弊成 collapse 登绢滚覆. 咯扁急 chance 咯何甫 力寇茄 唱赣瘤 炼扒甸.
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
	//	UB2UIManager::GetInstance()->CloseUI(UIFName::Defeat); // Resurrect 览翠捞 坷搁 捞 皋春啊 颇鲍瞪 芭变 窃..
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

	// 促弗 鸥涝狼 酒捞能甸阑 急琶 秦力

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
	//	// 傈眉 纠 傈券阑 窍搁 角 肺爹 矫埃捞 疵绢唱骨肺 盔窍绰 拳搁 流傈鳖瘤 UI History 父 父甸绢霖促. 
	//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//	if (UIMgrInst) {
	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
	//	}
	//	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
	//});

	//OpenBladeIILobbyCommon(Cast<AGameMode>(UGameplayStatics::GetGameMode(this))); // 纠 傈券 抗距 饶 夯拜 肺厚 甘 肺爹
}

void UB2UIDefeat::OnClickBtnSkillEnhance()
{
	//UB2UIManager::GetInstance()->CloseUI(UIFName::Defeat);
	//
	//FLobbySceneManager::DeferredRegistChangeLobbyScene([]() {
	//	// 傈眉 纠 傈券阑 窍搁 角 肺爹 矫埃捞 疵绢唱骨肺 盔窍绰 拳搁 流傈鳖瘤 UI History 父 父甸绢霖促. 
	//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//	if (UIMgrInst) {
	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
	//	}
	//	LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Skill);
	//});

	//OpenBladeIILobbyCommon(Cast<AGameMode>(UGameplayStatics::GetGameMode(this))); // 纠 傈券 抗距 饶 夯拜 肺厚 甘 肺爹
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
	//	// 傈眉 纠 傈券阑 窍搁 角 肺爹 矫埃捞 疵绢唱骨肺 盔窍绰 拳搁 流傈鳖瘤 UI History 父 父甸绢霖促. 
	//	UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	//	if (UIMgrInst) {
	//		UIMgrInst->ArtificialAddUIHistory(EUIScene::LobbyMain);
	//	}
	//	LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::SummonItemStore);
	//});

	//OpenBladeIILobbyCommon(Cast<AGameMode>(UGameplayStatics::GetGameMode(this))); // 纠 傈券 抗距 饶 夯拜 肺厚 甘 肺爹
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