#include "BladeII.h"
#include "B2UIPlayerSkillInfo.h"
#include "B2LobbyUI_SetupSkillMain.h"
#include "B2UIManager.h"
#include "B2LobbyUI_HeroMgmtCommon.h"
#include "B2LobbyGameMode.h"
#include "B2PCClassInfoBox.h"
#include "B2SkillInfo.h"
#include "Event.h"
#include "B2UIMsgPopupItemCostConfirm.h"
#include "B2UIMsgPopupBuySkillPoint.h"
#include "B2Inventory.h"
#include "B2UIDocHelper.h"
#include "B2UIMsgPopupTooltipSkillOption.h"
#include "B2UIRegistSkill.h"
#include "B2UICommonTab.h"
#include "B2UIMailRewardPopUp.h"
#include "B2UIUnitySkillRewardPopup.h"
#include "B2UIRewardEventSet.h"
#include "B2UIHeader.h"
#include "Retailer.h"
#include "B2AssetLoader.h"
#include "B2UIShortagePopupManager.h"
#include "BladeIIGameImpl.h"

UB2LobbyUI_SetupSkillMain::UB2LobbyUI_SetupSkillMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentView = EViewSkillList::None;
	CurrentPCClass = EPCClass::EPC_End;
	CachedLobbyGM = nullptr;
	CreatedUnityP = nullptr;
	LoadedUnityPartClassCache = nullptr;
	SkillInfoIndexFromPassive.Add(EPassiveType::EPassiveType_Counter, 0);
	SkillInfoIndexFromPassive.Add(EPassiveType::EPassiveType_Tag, 1);
	SkillInfoIndexFromPassive.Add(EPassiveType::EPassiveType_Attack, 3);
	SkillInfoIndexFromPassive.Add(EPassiveType::EPassiveType_Defense, 4);
	IsLevelUP = false;
}

void UB2LobbyUI_SetupSkillMain::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	CurrentView = EViewSkillList::ActiveSkill;

	CAPTURE_UOBJECT(UB2LobbyUI_SetupSkillMain);

	DeliveryLevelUpSkillTicket = DeliveryLevelUpSkillClass<FB2LevelUpSkillInfo>::GetInstance().Subscribe(
		[Capture](const FB2LevelUpSkillInfo& LevelUpSkillInfo)
	{
		if (Capture.IsValid())
		{
			Capture->IsLevelUP = false;
		}
		else
		{
			GoToVillageClass<>::GetInstance().Signal();
		}
	}
	);

	DeliveryLevelUpSkillErrorTicket = DeliveryLevelUpSkillErrorClass<>::GetInstance().Subscribe(
		[Capture]()
	{
		if (Capture.IsValid())
		{
			Capture->IsLevelUP = false;
		}
		else
		{
			GoToVillageClass<>::GetInstance().Signal();
		}
	}
	);

	ChangeSkillPointTicket = ChangeSkillPointClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(ChangeSkillPoint)
		Capture->OnChangedCharacterSkillPoint();
	END_CAPTURE_OBJECT()
		);
	ChangeSkillLevelTicket = ChangeSkillLevelClass<int32>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(ChangeSkillLevel, int32 SkillId)
		Capture->OnChangedSkillLevel(SkillId);
	END_CAPTURE_OBJECT()
		);
	CloseSkillTooltipTicket = CloseSkillTooltipClass<int32>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(CloseSkillTooltip, int32 SkillId)
		Capture->UnselectSkillInfo(SkillId);
	END_CAPTURE_OBJECT()
		);
	RegistSkillTicket = RegistSkillClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(RegistSkill)
		Capture->UpdateRegistSkillList();
	END_CAPTURE_OBJECT()
		);
	UpdateSkillSlotTicket = UpdateSkillSlotClass<EPCClass, EItemPresetSlotType>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(UpdateSkillSlot, const EPCClass InPCClass, const EItemPresetSlotType InSlotSlot)
		Capture->ChangePreset(InPCClass, InSlotSlot);
	END_CAPTURE_OBJECT()
		);

	LobbySetHeroMgmtModePCSelectionTicket = LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbySetHeroMgmtModePCSelection, EPCClass PCClass)
		Capture->OnHeroMgmtPCChanged(PCClass);
	END_CAPTURE_OBJECT()
		);

	LobbyChangeSlotEffectTicket = LobbySlotChangeEffectClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(LobbyChangeSlotEffect)
		Capture->ChangeSlotSkillEffect();
	END_CAPTURE_OBJECT()

		);
	
	if (UIP_HeroMgmtBase.IsValid())
	{
		UIP_HeroMgmtBase->StartFromLobby(InUIManager, InGM); // 이건 LobbyUISwitcher 에서 모르니 직접 해 주어야..
		UIP_HeroMgmtBase->SetPresetMode(EInventoryPresetMode::EIPM_Skill);
	}

	TAB_ActiveSkill->ForcedClickTab();

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_SkillButton")));
}

void UB2LobbyUI_SetupSkillMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// 여기 Unity 파트를 따로 구성했는데 그쪽 탭 진입시 성능이 떨어지니 이 시점부터 로딩시작 ㄷㄷ.
	// Init 시점에는 async flush 요소가 있으므로 불가.
	RequestAsycLoadUnityPartClass();
}

void UB2LobbyUI_SetupSkillMain::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UB2LobbyUI_HeroMgmtCommon, UIP_HeroMgmtBase);

	GET_SLOT(UPanelWidget, P_SkillPointSet);
	GET_SLOT(UPanelWidget, P_SkillListHorizontalBoxTop);
	GET_SLOT(UPanelWidget, P_SkillListHorizontalBoxBottom);
	GET_SLOT(UCanvasPanel, X_CP_UnitySkill);

	GET_SLOT(UB2UICommonTab, TAB_ActiveSkill);
	GET_SLOT(UB2UICommonTab, TAB_PassiveSkill);
	GET_SLOT(UB2UICommonTab, TAB_UnitySkill);

	GET_SLOT(UTextBlock, TB_ActiveSkillList);
	GET_SLOT(UTextBlock, TB_PassiveSkillList);
	GET_SLOT(UTextBlock, TB_SkillChange);
	GET_SLOT(UTextBlock, TB_BuySkillPoint);
	GET_SLOT(UTextBlock, TB_LeftSkillPoint);
	GET_SLOT(UTextBlock, TB_LeftSkillPointAmount);
	GET_SLOT(UTextBlock, TB_SkillReset);

	GET_SLOT(UB2Button, BTN_SkillChange);
	GET_SLOT(UB2Button, BTN_BuySkillPoint);
	GET_SLOT(UB2Button, BTN_SkillReset);

	GET_SLOT(UB2Image, IMG_TouchBlock);
	if (IMG_TouchBlock.IsValid())
		IMG_TouchBlock->SetVisibility(ESlateVisibility::Collapsed);

	GET_SLOT(UTextBlock, TB_ChangeSkill);

	SkillInfoArray.AddUninitialized(MAX_SKILLINFO_COUNT);

	GET_SLOT_BYNAME(UB2UIPlayerSkillInfo, SkillInfoArray[0], UIP_PlayerSkillInfo_01);
	GET_SLOT_BYNAME(UB2UIPlayerSkillInfo, SkillInfoArray[1], UIP_PlayerSkillInfo_02);
	GET_SLOT_BYNAME(UB2UIPlayerSkillInfo, SkillInfoArray[2], UIP_PlayerSkillInfo_03);
	GET_SLOT_BYNAME(UB2UIPlayerSkillInfo, SkillInfoArray[3], UIP_PlayerSkillInfo_04);
	GET_SLOT_BYNAME(UB2UIPlayerSkillInfo, SkillInfoArray[4], UIP_PlayerSkillInfo_05);
	GET_SLOT_BYNAME(UB2UIPlayerSkillInfo, SkillInfoArray[5], UIP_PlayerSkillInfo_06);

	BindDelegates();
	UpdateStaticText();
}

void UB2LobbyUI_SetupSkillMain::UpdateStaticText()
{
	if (TB_SkillChange.IsValid())
		TB_SkillChange->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_Change_Skill")));

	if (TB_LeftSkillPoint.IsValid())
		TB_LeftSkillPoint->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_Remaining_Skill")));

	if (TB_BuySkillPoint.IsValid())
		TB_BuySkillPoint->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_BuyPoint")));
		
	if(TB_ChangeSkill.IsValid())
		TB_ChangeSkill->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_Change_Skill")));

	if (TB_SkillReset.IsValid())
		TB_SkillReset->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_ResetSkillPoint_Title")));
}

void UB2LobbyUI_SetupSkillMain::BindDelegates()
{
	REGISTER_UITAB_BINDING(TAB_ActiveSkill.Get(), &UB2LobbyUI_SetupSkillMain::OnClickedActiveSkillListBtn);
	REGISTER_UITAB_BINDING(TAB_PassiveSkill.Get(), &UB2LobbyUI_SetupSkillMain::OnClickedPassiveSkillListBtn);
	REGISTER_UITAB_BINDING(TAB_UnitySkill.Get(), &UB2LobbyUI_SetupSkillMain::OnClickedUnitySkillBtn);

	BIND_CLICK_FUNC_TO_BTN(BTN_BuySkillPoint, &UB2LobbyUI_SetupSkillMain::OnClickedBuySkillPointBtn);
	BIND_CLICK_FUNC_TO_BTN(BTN_SkillChange, &UB2LobbyUI_SetupSkillMain::OnClickedSkillChangeBtn);
	BIND_CLICK_FUNC_TO_BTN(BTN_SkillReset, &UB2LobbyUI_SetupSkillMain::OnClickedInitSkillPointBtn);
	
}

void UB2LobbyUI_SetupSkillMain::BeginDestroy()
{
	//Editor 상황에서 문제가 좀 있어서 여기서도 unsubscribe
	if (CachedLobbyGM)
	{
		ChangeSkillPointClass<>::GetInstance().Unsubscribe(ChangeSkillPointTicket);
		ChangeSkillLevelClass<int32>::GetInstance().Unsubscribe(ChangeSkillLevelTicket);
		CloseSkillTooltipClass<int32>::GetInstance().Unsubscribe(CloseSkillTooltipTicket);
		RegistSkillClass<>::GetInstance().Unsubscribe(RegistSkillTicket);
		UpdateSkillSlotClass<EPCClass, EItemPresetSlotType>::GetInstance().Unsubscribe(UpdateSkillSlotTicket);
		LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Unsubscribe(LobbySetHeroMgmtModePCSelectionTicket);
		LobbySlotChangeEffectClass<>::GetInstance().Unsubscribe(LobbyChangeSlotEffectTicket);
		DeliveryLevelUpSkillClass<FB2LevelUpSkillInfo>::GetInstance().Unsubscribe(DeliveryLevelUpSkillTicket);
		DeliveryLevelUpSkillErrorClass<>::GetInstance().Unsubscribe(DeliveryLevelUpSkillErrorTicket);
	}

	Super::BeginDestroy();
}

void UB2LobbyUI_SetupSkillMain::DestroySelf()
{
	ChangeSkillPointClass<>::GetInstance().Unsubscribe(ChangeSkillPointTicket);
	ChangeSkillLevelClass<int32>::GetInstance().Unsubscribe(ChangeSkillLevelTicket);
	CloseSkillTooltipClass<int32>::GetInstance().Unsubscribe(CloseSkillTooltipTicket);
	RegistSkillClass<>::GetInstance().Unsubscribe(RegistSkillTicket);
	UpdateSkillSlotClass<EPCClass, EItemPresetSlotType>::GetInstance().Unsubscribe(UpdateSkillSlotTicket);
	LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Unsubscribe(LobbySetHeroMgmtModePCSelectionTicket);
	DeliveryLevelUpSkillClass<FB2LevelUpSkillInfo>::GetInstance().Unsubscribe(DeliveryLevelUpSkillTicket);
	DeliveryLevelUpSkillErrorClass<>::GetInstance().Unsubscribe(DeliveryLevelUpSkillErrorTicket);

	SkillInfoArray.Empty();

	DestroyUnityTabPart();

	if (UIP_HeroMgmtBase.IsValid())
	{
		UIP_HeroMgmtBase->DestroySelf();
		UIP_HeroMgmtBase = nullptr;
	}

	BladeIIGameImpl::GetClientDataStore().SaveLocalData();

	Super::DestroySelf();
}

void UB2LobbyUI_SetupSkillMain::CloseWidgetDelegate()
{
	if (CachedLobbyGM->GetLobbyInventory())
	{
		UB2LobbyUI_SetupSkillMain* InventoryUI = CachedLobbyGM ? Cast<UB2LobbyUI_SetupSkillMain>(CachedLobbyGM->DJLegacy_GetCurrLobbyUIPage()) : nullptr;
		if (InventoryUI && InventoryUI->GetVisibility() == ESlateVisibility::HitTestInvisible)
		{
			// 결속 애니 스킬 중에 백버튼 안먹히기 위해..
			return;
		}
	}

	if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	{
		EUIScene PrevUIScene = UIManager->GetCurrUIScene();
		//UIScene History에 의해 자동으로 Back된다. DJLegacy HeroMgmt 모드인 경우도 처리될 것.
		UIManager->ChangeUISceneBack();
	}
}

#define SET_SLOT_VISIBILITY(Slot, Visibility)		\
	if (Slot.IsValid())								\
	{												\
		Slot->SetVisibility(Visibility);			\
	}

void UB2LobbyUI_SetupSkillMain::OnClickedActiveSkillListBtn()
{
	if (CurrentView != EViewSkillList::ActiveSkill)
	{
		CurrentView = EViewSkillList::ActiveSkill;
		CreateSkillList();
	}

	SET_SLOT_VISIBILITY(P_SkillPointSet, ESlateVisibility::SelfHitTestInvisible);
	SET_SLOT_VISIBILITY(P_SkillListHorizontalBoxTop, ESlateVisibility::SelfHitTestInvisible);
	SET_SLOT_VISIBILITY(P_SkillListHorizontalBoxBottom, ESlateVisibility::SelfHitTestInvisible);
	SetUnityTabPartOpen(false); // 앞으로 바뀔 구성.. UnitySkill 쪽 별도 파트..

	if (UIP_HeroMgmtBase.IsValid()) UIP_HeroMgmtBase->SetPresetMode(EInventoryPresetMode::EIPM_Skill);
}

void UB2LobbyUI_SetupSkillMain::OnClickedPassiveSkillListBtn()
{
	if (CurrentView != EViewSkillList::PassiveSkill)
	{
		CurrentView = EViewSkillList::PassiveSkill;
		CreateSkillList();
	}

	SET_SLOT_VISIBILITY(P_SkillPointSet, ESlateVisibility::SelfHitTestInvisible);
	SET_SLOT_VISIBILITY(P_SkillListHorizontalBoxTop, ESlateVisibility::SelfHitTestInvisible);
	SET_SLOT_VISIBILITY(P_SkillListHorizontalBoxBottom, ESlateVisibility::SelfHitTestInvisible);
	SetUnityTabPartOpen(false); // 앞으로 바뀔 구성.. UnitySkill 쪽 별도 파트..

	if (UIP_HeroMgmtBase.IsValid())
	{
		UIP_HeroMgmtBase->SetPresetMode(EInventoryPresetMode::EIPM_Skill);
		UIP_HeroMgmtBase->SetHidePresetControls();
	}
}

void UB2LobbyUI_SetupSkillMain::OnClickedUnitySkillBtn()
{
	if (CurrentView != EViewSkillList::UnitySkill)
	{
		CurrentView = EViewSkillList::UnitySkill;
		SetUnityTabPartOpen(true); // 앞으로 바뀔 구성.. UnitySkill 쪽 별도 파트..
		CreateSkillList();
	}
	SET_SLOT_VISIBILITY(P_SkillPointSet, ESlateVisibility::Collapsed);
	SET_SLOT_VISIBILITY(P_SkillListHorizontalBoxTop, ESlateVisibility::Collapsed);
	SET_SLOT_VISIBILITY(P_SkillListHorizontalBoxBottom, ESlateVisibility::Collapsed);
	
	if (UIP_HeroMgmtBase.IsValid())
	{
		UIP_HeroMgmtBase->SetPresetMode(EInventoryPresetMode::EIPM_Skill);
		UIP_HeroMgmtBase->SetHidePresetControls();
	}
}

void UB2LobbyUI_SetupSkillMain::OnClickedBuySkillPointBtn()
{
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_SetupSkillMain::OnDelayBuySkillPoint, 0.1f, false);
}

void UB2LobbyUI_SetupSkillMain::OnClickedSkillChangeBtn()
{
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayCallbackTH, this, &UB2LobbyUI_SetupSkillMain::OnDelaySkillChange, 0.1f, false);
}

void UB2LobbyUI_SetupSkillMain::OnClickedLevelupSkillPoint(int32 SkillID)
{
	// 스킬 포인트만 예외 처리 (골드는 HandleServerError911Class 서버 에러로 처리)
	if (CurrentSkillPoint < 1) // 스킬 포인트가 하나도 없을경우 예외처리..
	{
		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_GoToSkillPoint")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::YesOrNo,
			FMsgPopupOnClick::CreateLambda([this]() {
			OnClickedBuySkillPointBtn();
		})
		);
		return;
	}

	if (IsLevelUP == false)
	{
		IsLevelUP = true;
		BladeIIGameImpl::GetClientDataStore().RequestLevelUpSkill(SkillID);
	}
}

bool UB2LobbyUI_SetupSkillMain::IsBuyableSkillPoint()
{
	int32 BuyableSkillPoint = 0;
	auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();

	BuyableSkillPoint += ClientDataStore.GetMaxSkillPointBuyable(EPCClass::EPC_Gladiator);
	BuyableSkillPoint += ClientDataStore.GetMaxSkillPointBuyable(EPCClass::EPC_Assassin);
	BuyableSkillPoint += ClientDataStore.GetMaxSkillPointBuyable(EPCClass::EPC_Wizard);
	BuyableSkillPoint += ClientDataStore.GetMaxSkillPointBuyable(EPCClass::EPC_Fighter);

	DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

	return BuyableSkillPoint > 0;
}

void UB2LobbyUI_SetupSkillMain::OnClickedInitSkillPointBtn()
{
	auto DocHero = UB2UIDocHelper::GetDocHero(PCClassToInt(CurrentPCClass));

	if (DocHero)
	{
		auto* Popup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupItemCostConfirm>(
			EUIMsgPopup::ItemCostConfirm,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_ResetSkillPoint_Title")),
			FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_ResetSkillPoint_Content")),
				BladeIIGameImpl::GetClientDataStore().GetLocalCharClassName(DocHero->GetHeroClass()))
			);
		if (Popup)
		{
			Popup->SetButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_ResetSkillPoint_Button")));

			FIntPoint ItemCost(FItemRefIDHelper::GetGoodsID_Gem(), BladeIIGameImpl::GetClientDataStore().GetGemResetSkill());
			Popup->SetItemCost(ItemCost);
			Popup->AddHandler(FMsgPopupOnClick::CreateLambda([this]()
			{
				if (BladeIIGameImpl::GetClientDataStore().GetGemAmount() < BladeIIGameImpl::GetClientDataStore().GetGemResetSkill())
				{
					ShortageMGR->PopupGoToShopForGemMessage();
				}
				else
				{
					BladeIIGameImpl::GetClientDataStore().RequestResetSkillPreset(this->CurrentPCClass);
					this->OnClickedMsgResetSkillPoint_BP();
				}
			}
			));
		}
	}
}

void UB2LobbyUI_SetupSkillMain::OnChangedCharacterSkillPoint()
{
	CurrentSkillPoint = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLeftSkillPoint(CurrentPCClass);

	if (TB_LeftSkillPointAmount.IsValid())
		TB_LeftSkillPointAmount->SetText(FText::AsNumber(CurrentSkillPoint));

	for (auto Skill : SkillInfoArray)
	{
		if (Skill.IsValid())
		{
			Skill->UpdateEnableLevelUp(CurrentSkillPoint > 0);
			Skill->SetSkillLevelupPoint(Skill->GetSkillId(), CurrentSkillPoint);
		}
	}

	UpdateMenuButtonEnable();
}

void UB2LobbyUI_SetupSkillMain::OnChangedSkillLevel(int32 SkillId)
{
	int32 SkillLevel = BladeIIGameImpl::GetLocalCharacterData().GetCharacterSkillLevel(SkillId);
	if (SkillLevel != -1)
	{
		for (auto Skill : SkillInfoArray)
		{
			if (Skill.IsValid() && Skill->GetSkillId() == SkillId)
			{
				Skill->SetSkillLevel(SkillLevel);
				Skill->UpdateLock(BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(CurrentPCClass));
				Skill->UpdateEnableLevelUp(CurrentSkillPoint > 0);
				Skill->SetSkillLevelupGold(SkillId);
				Skill->SetSkillLevelupPoint(SkillId, CurrentSkillPoint);
				break;
			}
		}
	}
}

void UB2LobbyUI_SetupSkillMain::UpdateRegistSkillList()
{
	CreateSkillList();

	UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(CurrentPCClass, true);
}

void UB2LobbyUI_SetupSkillMain::UpdateMenuButtonEnable()
{
	if (BTN_BuySkillPoint.IsValid())
	{
		BTN_BuySkillPoint->SetIsEnabled(IsBuyableSkillPoint());
	}
	if (BTN_SkillChange.IsValid())
	{
		TArray<int32> Skills;
		BladeIIGameImpl::GetLocalCharacterData().GetCharacterUnregistredSkills(CurrentPCClass, Skills);
		BTN_SkillChange->SetIsEnabled(Skills.Num() > 0);
	}
	if (BTN_SkillReset.IsValid())
	{
		BTN_SkillReset->SetIsEnabled(HasSkillLevelUp());
	}

	DoMarkRedDot();
	if (UIP_HeroMgmtBase.IsValid())
		UIP_HeroMgmtBase->DoMarkRedDot();
}

void UB2LobbyUI_SetupSkillMain::DoMarkRedDotAll_Unity()
{
	if (CreatedUnityP)
		CreatedUnityP->DoMarkReddotAll();

}

void UB2LobbyUI_SetupSkillMain::OnHeroMgmtPCChanged(EPCClass InSelectedPCClass)
{
	if (CurrentPCClass != InSelectedPCClass)
	{
		CurrentPCClass = InSelectedPCClass;

		CurrentSkillPoint = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLeftSkillPoint(CurrentPCClass);
		if (TB_LeftSkillPointAmount.IsValid())
			TB_LeftSkillPointAmount->SetText(FText::AsNumber(CurrentSkillPoint));

		CreateSkillList();

		UpdateMenuButtonEnable();

		if (UIP_HeroMgmtBase.IsValid())
		{
			EItemPresetSlotType SlotType = BladeIIGameImpl::GetLocalCharacterData().GetPCClassSkillSlotID(CurrentPCClass);

			UIP_HeroMgmtBase->SetPresetMode(EInventoryPresetMode::EIPM_Skill);
			UIP_HeroMgmtBase->SwitchPresetState(SlotType);
			if(CurrentView != EViewSkillList::ActiveSkill)
				UIP_HeroMgmtBase->SetHidePresetControls();
		}

		DoMarkRedDot();
	}
}

void UB2LobbyUI_SetupSkillMain::CreateSkillList()
{
	if (CurrentView == EViewSkillList::UnitySkill)
	{
		// 앞으로 바뀔 구성.. UnitySkill 쪽 별도 파트..
		if (CreatedUnityP)
		{
			CreatedUnityP->UpdatePanelMain(CurrentPCClass);
		}

		return;
	}

	auto* ClassInfoBox = StaticFindPCClassInfoBox();
	auto* AllSkillInfo = ClassInfoBox ? ClassInfoBox->GetAllSkillInfo() : nullptr;

	check(AllSkillInfo);

	for (auto SkillInfo : SkillInfoArray)
	{
		if (SkillInfo.IsValid())
			SkillInfo->SetVisibility(ESlateVisibility::Collapsed);
	}

	TArray<int32> PassiveIndexArray; // 어떤 슬롯이 빈슬롯이 되야하는지 알기위해
	TArray<FB2SkillInfo> PCSkills;
	SetPCSkillList(AllSkillInfo, PCSkills);

	for (int32 i = 0; i < SkillInfoArray.Num() && i < PCSkills.Num(); ++i)
	{
		const auto* SkillSingleInfo = AllSkillInfo->GetSingleInfoOfID(PCSkills[i].skill_id);

		BII_CHECK(SkillSingleInfo);
		if (nullptr == SkillSingleInfo)
			continue;

		int32 WidgetIndex = 0;

		if (CurrentView == EViewSkillList::PassiveSkill)
		{
			const int32* PassiveIndex = SkillInfoIndexFromPassive.Find(SkillSingleInfo->PassiveType);

			BII_CHECK(PassiveIndex); // EPassiveType 및 SkillInfoIndexFromPassive에 패시브 추가됐는지 확인
			if (PassiveIndex)
			{
				WidgetIndex = *PassiveIndex;

				if (false == PassiveIndexArray.Contains(WidgetIndex))
				{
					PassiveIndexArray.Add(WidgetIndex);
					CreateSkillWidget(WidgetIndex, false, &PCSkills[i], SkillSingleInfo);
				}
			}
		}
		else if (CurrentView == EViewSkillList::ActiveSkill)
		{
			WidgetIndex = i;
			PassiveIndexArray.Add(WidgetIndex);
			CreateSkillWidget(WidgetIndex, false, &PCSkills[i], SkillSingleInfo);
		}
	}

	// 슬롯은 MAX_SKILLINFO_COUNT개인데 스킬이 MAX_SKILLINFO_COUNT개 미만일때 빈슬롯 처리
	if (PassiveIndexArray.Num() < MAX_SKILLINFO_COUNT)
	{
		for (int32 i = 0; i < MAX_SKILLINFO_COUNT; ++i)
		{
			if (false == PassiveIndexArray.Contains(i))
				CreateSkillWidget(i, true);
		}
	}
}

void UB2LobbyUI_SetupSkillMain::SetPCSkillList(const UB2SkillInfo* AllSkillInfo, TArray<FB2SkillInfo>& Skills, bool bIncludeAllSkills)
{
	Skills.Empty();

	TArray<FB2SkillInfo> CharacterSkills;
	BladeIIGameImpl::GetLocalCharacterData().GetCharacterSkills(CurrentPCClass, CharacterSkills);

	for (auto CharacterSkill : CharacterSkills)
	{
		const auto* SkillSingleInfo = AllSkillInfo->GetSingleInfoOfID(CharacterSkill.skill_id);

		BII_CHECK(SkillSingleInfo);
		if (nullptr == SkillSingleInfo)
			continue;

		// 패시브일때
		if (bIncludeAllSkills || (CurrentView == EViewSkillList::PassiveSkill  && SkillSingleInfo->PassiveType != EPassiveType::EPassiveType_None))
		{
			Skills.Add(CharacterSkill);
		}
		
		// 액티브일때
		if (bIncludeAllSkills || (CurrentView == EViewSkillList::ActiveSkill && SkillSingleInfo->PassiveType == EPassiveType::EPassiveType_None))
		{
			Skills.Add(CharacterSkill);
		}
	}
}

bool UB2LobbyUI_SetupSkillMain::HasSkillLevelUp()
{
	auto* ClassInfoBox = StaticFindPCClassInfoBox();
	auto* AllSkillInfo = ClassInfoBox ? ClassInfoBox->GetAllSkillInfo() : nullptr;
	if (AllSkillInfo)
	{
		TArray<FB2SkillInfo> PCSkills;

		// 패시브 액티브 둘다포함해서 검사
		SetPCSkillList(AllSkillInfo, PCSkills, true);

		for (auto& SkillInfo : PCSkills)
		{
			if (SkillInfo.skill_level > 0)
				return true;
		}
		return false;
	}
	return false;
}

void UB2LobbyUI_SetupSkillMain::CreateSkillWidget(const int32 UIIndex, const bool bForceLock, const FB2SkillInfo* Skill, const struct FSingleSkillInfo* SkillSingleInfo)
{
	if (SkillInfoArray.Num() <= UIIndex)
	{
		BII_CHECK(false);
		return;
	}

	auto* CreatedWidget = SkillInfoArray[UIIndex].Get() ? CreateWidget<UB2UIPlayerSkillInfo>(GetOwningPlayer(), SkillInfoArray[UIIndex]->GetClass()) : nullptr;
	auto* OldWidget = SkillInfoArray[UIIndex].Get();

	BII_CHECK(CreatedWidget || OldWidget)
		if (CreatedWidget && OldWidget)
		{
			auto* ParentPanel = Cast<UCanvasPanel>(OldWidget->GetParent());
			check(ParentPanel);
			if (ParentPanel)
			{
				SkillInfoArray[UIIndex] = CreatedWidget;

				ParentPanel->ReplaceChildAt(ParentPanel->GetChildIndex(OldWidget), CreatedWidget);
				auto* CPSlot = Cast<UCanvasPanelSlot>(CreatedWidget->Slot);

				BII_CHECK(CPSlot);
				if (CPSlot)
				{
					auto SCanvas = ParentPanel->GetCanvasWidget();
					SCanvas->RemoveSlot(OldWidget->TakeWidget());
					CPSlot->BuildSlot(ParentPanel->GetCanvasWidget().ToSharedRef());
				}
			}

			CreatedWidget->Init();
			CreatedWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			if (nullptr == SkillSingleInfo || nullptr == Skill || bForceLock)
			{
				CreatedWidget->UpdatePassiveSkillLock();
			}
			else
			{
				CreatedWidget->SetSkillId(Skill->skill_id);
				CreatedWidget->SetSkillLevel(Skill->skill_level);
				CreatedWidget->SetRegistred(BladeIIGameImpl::GetLocalCharacterData().IsSkillRegistred(CurrentPCClass, Skill->skill_id));
				CreatedWidget->UpdateLock(BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(CurrentPCClass));
				CreatedWidget->SetSkillType(SkillSingleInfo->SkillType);
				CreatedWidget->UpdateEnableLevelUp(CurrentSkillPoint > 0);
				CreatedWidget->SetSkillLevelupPoint(Skill->skill_id, CurrentSkillPoint);
				CreatedWidget->SetSkillIconHandler(FSkillInfoOnClick::CreateLambda([this, CreatedWidget]()
				{
					if (CreatedWidget)
					{
						this->ShowTooltipSkill(CreatedWidget->GetSkillId(), CreatedWidget->GetSkillLv(), CreatedWidget->GetSkillMaxLv());
						CreatedWidget->SetSelected(true);

					}
				}
				));
				CreatedWidget->SetSkillLevelUpHandler(FSkillLevelUpOnClick::CreateLambda([this, CreatedWidget]()
				{
					if (CreatedWidget)
					{
						this->OnClickedLevelupSkillPoint(CreatedWidget->GetSkillId());
					}
				}
				));
			}
		}
}

void UB2LobbyUI_SetupSkillMain::ShowTooltipSkill(int32 SkillId, int32 SkillLv, int32 SkillMaxLv)
{
	auto* SkillTooltip = UB2UIManager::GetInstance()->GetUI<UB2UIMsgPopupTooltipSkillOption>(UIFName::MsgPopupTooltipSkillOption);
	if (SkillTooltip == nullptr)
	{
		SkillTooltip = UB2UIManager::GetInstance()->OpenUI<UB2UIMsgPopupTooltipSkillOption>(UIFName::MsgPopupTooltipSkillOption);
	}
	else
	{
		CloseSkillTooltipClass<int32>::GetInstance().Signal(SkillTooltip->GetSkillID());
	}

	if (SkillTooltip)
	{
		if (SkillTooltip->GetOpenState() != EUIOpenState::OnOpening || SkillTooltip->GetOpenState() != EUIOpenState::Opened)
		{
			UB2UIManager::GetInstance()->OpenUI(UIFName::MsgPopupTooltipSkillOption);
		}

		SkillTooltip->SetSkillID(SkillId);
		SkillTooltip->SetSkillLevel(SkillLv, SkillMaxLv);
	}
}

void UB2LobbyUI_SetupSkillMain::UnselectSkillInfo(int32 SkillId)
{
	for (auto& Skill : SkillInfoArray)
	{
		if (Skill.IsValid() && Skill->GetSkillId() == SkillId)
		{
			Skill->SetSelected(false);
		}
	}
}

void UB2LobbyUI_SetupSkillMain::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(TAB_ActiveSkill.Get()), &UB2LobbyUI_SetupSkillMain::RedDotCondition_ActiveSkillTab);
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(TAB_PassiveSkill.Get()), &UB2LobbyUI_SetupSkillMain::RedDotCondition_PassiveSkillTab);
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(TAB_UnitySkill.Get()), &UB2LobbyUI_SetupSkillMain::RedDotCondition_UnitySkillTab);
}

bool UB2LobbyUI_SetupSkillMain::RedDotCondition_ActiveSkillTab()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_PointSkill(CurrentPCClass, true);
}

bool UB2LobbyUI_SetupSkillMain::RedDotCondition_PassiveSkillTab()
{
	return BladeIIGameImpl::GetRedDotManager().HasLobbyRedDot_HeroMgmt_PointSkill(CurrentPCClass, false);
}

bool UB2LobbyUI_SetupSkillMain::RedDotCondition_UnitySkillTab()
{
	if (CurrentPCClass != EPCClass::EPC_End)
		return BladeIIGameImpl::GetRedDotManager().HasLobbyReddot_HeroMgmt_UnitySkill(CurrentPCClass);

	return false;
}

void UB2LobbyUI_SetupSkillMain::ChangePreset(const EPCClass InPCClass, const EItemPresetSlotType InSlotID)
{
	if (CurrentPresetID != InSlotID)
	{
		CurrentPresetID = InSlotID;
		CurrentPCClass = InPCClass;

		BladeIIGameImpl::GetLocalCharacterData().SetUpdatePCClassSkillSlotID(InPCClass, InSlotID);

		CurrentSkillPoint = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLeftSkillPoint(CurrentPCClass);
		if (TB_LeftSkillPointAmount.IsValid())
			TB_LeftSkillPointAmount->SetText(FText::AsNumber(CurrentSkillPoint));

		CreateSkillList();

		UpdateMenuButtonEnable();

		UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(CurrentPCClass, true);
	}
}

void UB2LobbyUI_SetupSkillMain::OnDelaySkillChange()
{
	auto* RegistSkillWidget = UB2UIManager::GetInstance()->OpenUI<UB2UIRegistSkill>(UIFName::RegistSkill);
	if (RegistSkillWidget)
	{
		RegistSkillWidget->SetPCClass(CurrentPCClass);
	}
}

void UB2LobbyUI_SetupSkillMain::OnDelayBuySkillPoint()
{
	auto* Popup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupBuySkillPoint>(
		EUIMsgPopup::BuySkillPoint,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_BuySkillPoint_Title")),
		FText::GetEmpty()
		);
	if (Popup)
	{
		auto& ClientDataStore = BladeIIGameImpl::GetClientDataStore();

		Popup->InitializeBuySkillPoint(CurrentPCClass);
		DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();

		Popup->SetButtonText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_BuySkillPoint_Button")));
		Popup->SetGemCostUnit(BladeIIGameImpl::GetClientDataStore().GetGemsBuySkillPoint());
		Popup->UpdateGemCostTotal();
	}
}

void UB2LobbyUI_SetupSkillMain::SetVisibleTouchBlockImage(bool bInVisible)
{ // 하위 파트에서 사용할 일이 있을 것..
	if (IMG_TouchBlock.IsValid())
		IMG_TouchBlock->SetVisibility(bInVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

const FString& UB2LobbyUI_SetupSkillMain::GetUnityPartRequestName()
{
	static FString RequestName = TEXT("UnitySkilPart");
	return RequestName;
}

void UB2LobbyUI_SetupSkillMain::SetUnityTabPartOpen(bool bInOpen)
{
	if (bInOpen)
	{ // 처음 한번 동적 생성.
		ConditionalCreateUnityTabPart();
	}

	if (CreatedUnityP)
	{
		CreatedUnityP->SetVisibility(bInOpen ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UB2LobbyUI_SetupSkillMain::ConditionalCreateUnityTabPart()
{
	if (CreatedUnityP == nullptr && X_CP_UnitySkill.IsValid())
	{
		const FString& RequestName = GetUnityPartRequestName();
		auto LoadingState = UB2AssetLoader::Get()->GetLoadingState(RequestName);

		bool bLoadComplete = LoadingState == ELoadingState::Complete ? true : false;
		if (LoadingState == ELoadingState::InLoading)
		{
			if (UB2AssetLoader::Get()->WaitUntilLoadComplete(RequestName))
				bLoadComplete = true;
		}

		if (bLoadComplete && UnityPartClass.IsValid())
		{
			CreatedUnityP = DynLoadAndCreateInCanvasPanelFull<UB2LobbyUI_SetupSkill_UnityP>(UnityPartClass, this, X_CP_UnitySkill.Get());
			if (CreatedUnityP)
			{
				CreatedUnityP->StartFromLobby(OwnerUIManager, CachedLobbyGM);
				CreatedUnityP->InitFromOwnerSkillMain(this, UIP_HeroMgmtBase.Get());
			}
		}
	}
}

void UB2LobbyUI_SetupSkillMain::DestroyUnityTabPart()
{
	UB2AssetLoader::Get()->UnloadAsset(UnityPartClass.ToSoftObjectPath());
	LoadedUnityPartClassCache = nullptr;

	if (CreatedUnityP)
	{
		CreatedUnityP->DestroySelf();
		CreatedUnityP = nullptr;
	}
}

void UB2LobbyUI_SetupSkillMain::RequestAsycLoadUnityPartClass()
{
	//if (UnityPartClass.IsValid() == false)
	//{
	//	auto* AssetLoader = UB2AssetLoader::Get();

	//	const FString& RequestName = GetUnityPartRequestName();
	//	if (AssetLoader->GetLoadingState(RequestName) != ELoadingState::InLoading)
	//	{
	//		FB2AsyncLoadingComplete CompleteCallback;
	//		CompleteCallback = FB2AsyncLoadingComplete::CreateUObject(this, &UB2LobbyUI_SetupSkillMain::OnLoadedUnitySkillAsset);

	//		AssetLoader->RequestAsyncLoad(GetUnityPartRequestName(), TArray<FStringAssetReference> {UnityPartClass.ToSoftObjectPath() }, CompleteCallback);
	//	}
	//}
}

void UB2LobbyUI_SetupSkillMain::OnLoadedUnitySkillAsset(const FString& CompleteRequest)
{
	if (CompleteRequest == GetUnityPartRequestName())
		LoadedUnityPartClassCache = UnityPartClass.Get();
}

void UB2LobbyUI_SetupSkillMain::ChangeSlotSkillEffect()
{
	int32 SkillSlotMax = BladeIIGameImpl::GetClientDataStore().GetSkillPresetMaxCount();

	auto& CharacterDataStore = BladeIIGameImpl::GetLocalCharacterData();

	TArray<int32> SlotSkillIds;

	
	CharacterDataStore.GetRegistredTargetSlotSkills(CurrentPCClass, CurrentPresetID, SlotSkillIds);
	

	int32 Skill0 = SlotSkillIds.IsValidIndex(0) ? SlotSkillIds[0] : SKILL_INVALID_ID;
	int32 Skill1 = SlotSkillIds.IsValidIndex(1) ? SlotSkillIds[1] : SKILL_INVALID_ID;
	int32 Skill2 = SlotSkillIds.IsValidIndex(2) ? SlotSkillIds[2] : SKILL_INVALID_ID;

	for (auto SkillInfoSlot : SkillInfoArray)
	{
		if (Skill0 == SkillInfoSlot->GetSkillId() ||
			Skill1 == SkillInfoSlot->GetSkillId() ||
			Skill2 == SkillInfoSlot->GetSkillId())
		{
			SkillInfoSlot->PlaySkillEnhance_BP();
		}
	}

}
