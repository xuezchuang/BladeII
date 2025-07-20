#include "B2LobbyUI_SetupSkill_UnityP.h"
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
#include "B2UIUpgradeStatNotify.h"
#include "B2UIUnitySkillRewardPopup.h"
#include "B2UIMsgPopupTooltipSkillOption.h"
#include "B2LobbyUI_UnitySkillWakePopup.h"
#include "B2AndroidBackManager.h"
#include "B2UIHeader.h"
#include "Retailer.h"

#include "BladeIIGameImpl.h"

UB2LobbyUI_SetupSkill_UnityP::UB2LobbyUI_SetupSkill_UnityP(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OwnerSkillMainPage = nullptr;
	HeroMgmtBaseNRef = nullptr;

	MainPCClass = EPCClass::EPC_End;
	SubPCClass = EPCClass::EPC_End;
	CachedLobbyGM = nullptr;
}

void UB2LobbyUI_SetupSkill_UnityP::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	Super::StartFromLobby(InUIManager, InGM);

	SubscribeEvents();

	if(Lobby_SetupSkill_UnityP_AwakePopup.IsValid())	
		Lobby_SetupSkill_UnityP_AwakePopup->StartFromLobby(InUIManager, InGM);

}

void UB2LobbyUI_SetupSkill_UnityP::InitFromOwnerSkillMain(UB2LobbyUI_SetupSkillMain* InOwnerPage, UB2LobbyUI_HeroMgmtCommon* InHeroMgmtCommon)
{
	OwnerSkillMainPage = InOwnerPage;
	HeroMgmtBaseNRef = InHeroMgmtCommon; // 傈崔父 罐绊 捞率辑 Init 鞍篮 芭 救窃.

	check(HeroMgmtBaseNRef);
	check(OwnerSkillMainPage);

	if (OwnerSkillMainPage && HeroMgmtBaseNRef)
	{
		MainPCClass = HeroMgmtBaseNRef->GetCurrentPCClass();

		UpdatePanelMain();
	}

	PageBlock(false);
}

void UB2LobbyUI_SetupSkill_UnityP::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_UnitySkillName);
	GET_SLOT(UTextBlock, STB_UnityTitle);
	GET_SLOT(UTextBlock, STB_UnityComplete);
	GET_SLOT(UTextBlock, STB_UnityRewardButton);
	GET_SLOT(UTextBlock, STB_IsEther);
	GET_SLOT(UTextBlock, STB_UnityAwake);
	GET_SLOT(UTextBlock, STB_UnityAwakeNotice);
	GET_SLOT(UTextBlock, TB_UnityChar1);
	GET_SLOT(UTextBlock, TB_UnityName);
	GET_SLOT(UTextBlock, TB_UnityDesc);
	GET_SLOT(UB2RichTextBlock, TB_SkillOption);

	GET_SLOT(UB2Button, BTN_UnitySkill);
	GET_SLOT(UB2Button, BTN_UnityReward);
	GET_SLOT(UB2Button, BTN_UnityDetail);

	GET_SLOT(UB2Button, BTN_UnityAwake);
	GET_SLOT(UB2Button, BTN_UnityAwakeNotice);

	GET_SLOT(UB2Image, IMG_UnitySkillIcon);
	GET_SLOT(UB2Image, IMG_UnitySelect);

	GET_SLOT(UWidgetSwitcher, WS_UnityInfo);

	GET_SLOT(UPanelWidget, P_IsEther);
	if (P_IsEther.IsValid())
		P_IsEther->SetVisibility(ESlateVisibility::Collapsed);


	GET_SLOT(UVerticalBox, List_UinityAccuire);
	if(List_UinityAccuire.IsValid())
		List_UinityAccuire->ClearChildren();
	
	UnityCharInfoArray.AddUninitialized(MAX_UNITYCHAR_COUNT);

	GET_SLOT_BYNAME(UB2UIUnityCharacterInfo, UnityCharInfoArray[0], UnityCharEntry1);
	GET_SLOT_BYNAME(UB2UIUnityCharacterInfo, UnityCharInfoArray[1], UnityCharEntry2);
	GET_SLOT_BYNAME(UB2UIUnityCharacterInfo, UnityCharInfoArray[2], UnityCharEntry3);

	GET_SLOT(UB2LobbyUI_UnitySkillWakePopup, Lobby_SetupSkill_UnityP_AwakePopup);
	
	if (Lobby_SetupSkill_UnityP_AwakePopup.IsValid())
	{
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(Lobby_SetupSkill_UnityP_AwakePopup.Get());
		Lobby_SetupSkill_UnityP_AwakePopup->SetVisibility(ESlateVisibility::Collapsed);
	}

	BindDelegates();
	UpdateStaticText();
}

void UB2LobbyUI_SetupSkill_UnityP::UpdateStaticText()
{
	if (STB_UnityComplete.IsValid())
		STB_UnityComplete->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_Obtain")));
	if (STB_UnityRewardButton.IsValid())
		STB_UnityRewardButton->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Quest_Complete")));
	if (STB_DetailButton.IsValid())
		STB_DetailButton->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("DetailMyInfoPopup_Info")));
	if (STB_IsEther.IsValid())
		STB_IsEther->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_FuryEtherEquip")));
	if (STB_UnityAwake.IsValid())
		STB_UnityAwake->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_AwakenMission")));
	if (STB_UnityAwakeNotice.IsValid())
		STB_UnityAwakeNotice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_AwakenMission")));
}

void UB2LobbyUI_SetupSkill_UnityP::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_UnitySkill, &UB2LobbyUI_SetupSkill_UnityP::OnClickedUnityButton);
	BIND_CLICK_FUNC_TO_BTN(BTN_UnityReward, &UB2LobbyUI_SetupSkill_UnityP::OnClickedUnityReward);
	BIND_CLICK_FUNC_TO_BTN(BTN_UnityDetail, &UB2LobbyUI_SetupSkill_UnityP::OnClickedUnityDetail);

	BIND_CLICK_FUNC_TO_BTN(BTN_UnityAwake, &UB2LobbyUI_SetupSkill_UnityP::OnClickedUnityAwake);
	BIND_CLICK_FUNC_TO_BTN(BTN_UnityAwakeNotice, &UB2LobbyUI_SetupSkill_UnityP::OnClickedUnityAwakeNotice);

	for (auto UnityItem : UnityCharInfoArray)
	{
		if (UnityItem.IsValid())
		{
			UnityItem->Init();
			UnityItem->ButtonDelegate.BindUObject(this, &UB2LobbyUI_SetupSkill_UnityP::OnClickedUnityPortraitButton);
		}
	}
}

void UB2LobbyUI_SetupSkill_UnityP::BeginDestroy()
{
	//Editor 惑炔俊辑 巩力啊 粱 乐绢辑 咯扁辑档 unsubscribe
	if (CachedLobbyGM)
	{
		UnsubscribeEvents();
	}

	Super::BeginDestroy();
}

void UB2LobbyUI_SetupSkill_UnityP::DestroySelf()
{
	UnsubscribeEvents();

	UnityCharInfoArray.Empty();
	
	Super::DestroySelf();
}

void UB2LobbyUI_SetupSkill_UnityP::SubscribeEvents()
{
	CAPTURE_UOBJECT(UB2LobbyUI_SetupSkill_UnityP);

	DeliveryReceiveUnitySkillRewardTicket = DeliveryReceiveUnitySkillRewardClass<FB2ResponseUnitySkillRewardPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryReceiveUnitySkillReward, FB2ResponseUnitySkillRewardPtr Msg)
			Capture->OnReceiveUnitySkillReward(Msg);
		END_CAPTURE_OBJECT()
		);

	DeliveryAwakenUnitySkillTicket = DeliveryAwakenUnitySkillClass<FB2ResponseAwakenUnitySkillPtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryReceiveUnitySkillReward, FB2ResponseAwakenUnitySkillPtr Msg)
		Capture->OnReceiveUnitySkillAwake(Msg);
	END_CAPTURE_OBJECT()
		);

	AllClearUnityAwakenMissionTicket = AllClearUnityAwakenMissionClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(AllClearUnityAwakenMission)
		Capture->OnAllClearUnityAwakenMission();
	END_CAPTURE_OBJECT()
		);

	DeliveryUnityAwakeFailTicket = DeliveryUnityAwakeFailClass<>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryUnityAwakeFail)
		Capture->PageBlock(false);
	END_CAPTURE_OBJECT()
		);
}
void UB2LobbyUI_SetupSkill_UnityP::UnsubscribeEvents()
{
	DeliveryReceiveUnitySkillRewardClass<FB2ResponseUnitySkillRewardPtr>::GetInstance().Unsubscribe(DeliveryReceiveUnitySkillRewardTicket);
	DeliveryAwakenUnitySkillClass<FB2ResponseAwakenUnitySkillPtr>::GetInstance().Unsubscribe(DeliveryAwakenUnitySkillTicket);
	AllClearUnityAwakenMissionClass<>::GetInstance().Unsubscribe(AllClearUnityAwakenMissionTicket);
	DeliveryUnityAwakeFailClass<>::GetInstance().Unsubscribe(DeliveryUnityAwakeFailTicket);
}

void UB2LobbyUI_SetupSkill_UnityP::UpdatePanelMain(EPCClass InPCClassIfChanged )
{
	if (InPCClassIfChanged != EPCClass::EPC_End) // 鞘夸茄 版快 PCClass 捞吧肺 烹秦 函版.
	{
		MainPCClass = InPCClassIfChanged;
	}

	// 捞傈俊 SetupSkillMain 捞尔 嘿绢乐阑 锭 捞率 徘 急琶捞唱 康旷包府 努贰胶 背眉 矫 荐青登带 悼累 葛澜.
	SetUnitySkillPanel();
	OnClickedUnityButton();
	DoMarkReddotAll();
	SetIsEther();
	SetUnityAwakeStateButton();
}

void UB2LobbyUI_SetupSkill_UnityP::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UnityCharInfoArray[0].Get()), &UB2LobbyUI_SetupSkill_UnityP::RedDotCondition_UnitySkillChar1);
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UnityCharInfoArray[1].Get()), &UB2LobbyUI_SetupSkill_UnityP::RedDotCondition_UnitySkillChar2);
	REGISTER_REDDOT_CONDITION(GetRedDotWidgetFromSubWidget(UnityCharInfoArray[2].Get()), &UB2LobbyUI_SetupSkill_UnityP::RedDotCondition_UnitySkillChar3);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(TEXT("BTN_UnityAwake")), &UB2LobbyUI_SetupSkill_UnityP::RedDotCondition_UnitySkillAwakenAble);
}

bool UB2LobbyUI_SetupSkill_UnityP::RedDotCondition_UnitySkillChar1()
{
	if (MainPCClass != EPCClass::EPC_End)
	{
		int32 TargetClass = (PCClassToInt(MainPCClass) + 1) % (int32)EPCClass::EPC_End;
		return BladeIIGameImpl::GetRedDotManager().HasLobbyReddot_HeroMgmt_UnitySkill(MainPCClass, IntToPCClass(TargetClass));
	}

	return false;
}


bool UB2LobbyUI_SetupSkill_UnityP::RedDotCondition_UnitySkillChar2()
{
	if (MainPCClass != EPCClass::EPC_End)
	{
		int32 TargetClass = (PCClassToInt(MainPCClass) + 2) % (int32)EPCClass::EPC_End;
		return BladeIIGameImpl::GetRedDotManager().HasLobbyReddot_HeroMgmt_UnitySkill(MainPCClass, IntToPCClass(TargetClass));
	}

	return false;
}


bool UB2LobbyUI_SetupSkill_UnityP::RedDotCondition_UnitySkillChar3()
{
	if (MainPCClass != EPCClass::EPC_End)
	{
		int32 TargetClass = (PCClassToInt(MainPCClass) + 3) % (int32)EPCClass::EPC_End;
		return BladeIIGameImpl::GetRedDotManager().HasLobbyReddot_HeroMgmt_UnitySkill(MainPCClass, IntToPCClass(TargetClass));
	}

	return false;
}

bool UB2LobbyUI_SetupSkill_UnityP::RedDotCondition_UnitySkillAwakenAble()
{
	if (MainPCClass != EPCClass::EPC_End)
	{
		return (BladeIIGameImpl::GetRedDotManager().HasLobbyReddot_HeroMgmt_UnitySkillAwakenAble(MainPCClass) || BladeIIGameImpl::GetRedDotManager().HasLobbyReddot_HeroMgmt_UnitySkillAwakenMission(MainPCClass));
	}

	return false;
}

void UB2LobbyUI_SetupSkill_UnityP::OnClickedUnityButton()
{
	for (auto UnityItem : UnityCharInfoArray)
	{
		UnityItem->OnChangeSelected(false);
	}

	OnClickedUnityPortraitButton(MainPCClass);

	if (STB_UnityTitle.IsValid())
		STB_UnityTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_Info")));
	
	SetUnityAwakeStateButton();
}

void UB2LobbyUI_SetupSkill_UnityP::OnClickedUnityReward()
{
	data_trader::Retailer::GetInstance().RequestReceiveUnitySkillReward(CliToSvrPCClassType(MainPCClass), CliToSvrPCClassType(SubPCClass));
}

void UB2LobbyUI_SetupSkill_UnityP::OnReceiveUnitySkillReward(FB2ResponseUnitySkillRewardPtr RewardPtr)
{
	if (OwnerSkillMainPage)
	{
		OwnerSkillMainPage->SetVisibleTouchBlockImage(true);
	}
	
	BladeIIGameImpl::GetLocalCharacterData().UpdateUnitySkillMissionInfo(RewardPtr);

	for (auto UnityItem : UnityCharInfoArray)
	{
		if (UnityItem->PCClass == SubPCClass)
		{
			if (RewardPtr->next_mission->state == b2network::B2MissionStateType::REWARDED && RewardPtr->next_mission->step == MAX_UNITYSTEP_COUNT)
				UnityItem->OnPlayAnim_MissionCount_IMP(RewardPtr->next_mission->step + 1);
			else
				UnityItem->OnPlayAnim_MissionCount_IMP(RewardPtr->next_mission->step);
		}
	}

	SetUnitySkillPanel();
	OnClickedUnityPortraitButton(SubPCClass);

	DoMarkReddotAll();
}

void UB2LobbyUI_SetupSkill_UnityP::OnReceiveUnitySkillAwake(FB2ResponseAwakenUnitySkillPtr RewardPtr)
{
	// 焊郴搁 公炼扒 己傍捞聪 诀单捞飘 贸府秦淋.
	BladeIIGameImpl::GetLocalCharacterData().SetUnitySkillAwakeSuccess(MainPCClass);
	OnPlayUnityWakeReward_IMP();
	DoMarkReddotAll();
}

void UB2LobbyUI_SetupSkill_UnityP::DoMarkReddotAll()
{
	// 搬加其捞瘤
	DoMarkRedDot();

	// 谅螟 某腐磐 滚瓢甸
	if (HeroMgmtBaseNRef)
		HeroMgmtBaseNRef->DoMarkRedDot();

	// 胶懦 徘滚瓢甸
	if (OwnerSkillMainPage)
		OwnerSkillMainPage->DoMarkRedDot();

	// 阿己 滚瓢甸
	if (Lobby_SetupSkill_UnityP_AwakePopup.IsValid())
		Lobby_SetupSkill_UnityP_AwakePopup->DoMarkRedDot();
}

void UB2LobbyUI_SetupSkill_UnityP::OnClickedUnityDetail()
{
	int32 CurrentUnitySkillIndex = GetUnitySkillID(MainPCClass);

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

		SkillTooltip->SetUnitySkillID(CurrentUnitySkillIndex, MainPCClass);
	}
}

void UB2LobbyUI_SetupSkill_UnityP::OnClickedUnityAwake()
{
	FUnitySkillState* UnityState = BladeIIGameImpl::GetLocalCharacterData().GetUnitySkillState(MainPCClass);

	if (UnityState->AllAwakenComplete)
	{
		PageBlock(true);
		data_trader::Retailer::GetInstance().RequestAwakenUnitySkill(CliToSvrPCClassType(MainPCClass));
	}
	else
	{
		if (Lobby_SetupSkill_UnityP_AwakePopup.IsValid())
		{
			Lobby_SetupSkill_UnityP_AwakePopup->Init(MainPCClass);
			B2AndroidBackManager::GetInstance()->AddBackWidget(Lobby_SetupSkill_UnityP_AwakePopup.Get());
			Lobby_SetupSkill_UnityP_AwakePopup->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UB2LobbyUI_SetupSkill_UnityP::OnClickedUnityAwakeNotice()
{
	// 捞抚 函版鼻 荤侩窍瘤 臼澜.
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("SensitiveNoti_Notification"))),
		BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("Skill_Unity_AwakenMissionCondition"))),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm);
}

void UB2LobbyUI_SetupSkill_UnityP::SetUnitySkillPanel()
{
	int32 MainPCClassIndex = PCClassToInt(MainPCClass);
	FUnitySkillMissionArray* CurrentMissions = BladeIIGameImpl::GetLocalCharacterData().GetUnitySkillMission(MainPCClass);
	
	for (int32 i = 0; i < MAX_UNITYCHAR_COUNT; i++)
	{
		int32 SubPCClassIndex = (MainPCClassIndex + i + 1) % (int32)EPCClass::EPC_End;

		UnityCharInfoArray[i]->SetCharacterPortrait(static_cast<EPCClass>(SubPCClassIndex));
		
		for (auto MissionItem : *CurrentMissions)
		{
			if (MissionItem.MainClass == MainPCClass && MissionItem.SubClass == static_cast<EPCClass>(SubPCClassIndex))
			{
				if (MissionItem.nStep == MAX_UNITYSTEP_COUNT && (MissionItem.nState != b2network::B2MissionStateType::PROGRESS))
				{
					UnityCharInfoArray[i]->SetMissionClearCount(MAX_UNITYSTEP_COUNT);
					UnityCharInfoArray[i]->OnPlayAnim_Portrait_Loop_IMP(true);
					CurrentStep = MissionItem.nStep;
				}
				else
				{
					UnityCharInfoArray[i]->SetMissionClearCount(MissionItem.nStep - 1);
					UnityCharInfoArray[i]->OnPlayAnim_Portrait_Loop_IMP(false);
					CurrentStep = MissionItem.nStep - 1;
				}
			}
		}
	}

	SetUnitySkillPortraitInfo();
}

void UB2LobbyUI_SetupSkill_UnityP::SetUnitySkillPortraitInfo()
{
	int32 CurrentUnitySkillIndex = GetUnitySkillID(MainPCClass); // [todo] 捞芭 肺拿 纳腐磐单捞磐肺 秦具瞪淀?
	auto* ClassInfoBox = StaticFindPCClassInfoBox();
	auto* AllSkillInfo = ClassInfoBox ? ClassInfoBox->GetAllSkillInfo() : nullptr;
	
	FSingleSkillInfo* SkillClientData = AllSkillInfo ? const_cast<FSingleSkillInfo*>(AllSkillInfo->GetSingleInfoOfID(CurrentUnitySkillIndex)) : nullptr;

	if (IMG_UnitySkillIcon.IsValid() && SkillClientData)
	{
		IMG_UnitySkillIcon->SetBrushFromMaterial(Cast<UMaterialInstance>(SkillClientData->ButtonIcon_Normal.LoadSynchronous()));
	}

	if (TB_UnitySkillName.IsValid())
	{
		TB_UnitySkillName->SetText(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::SkillName, CurrentUnitySkillIndex)));
	}
}

void UB2LobbyUI_SetupSkill_UnityP::OnClickedUnityPortraitButton(EPCClass seletedclass)
{
	SubPCClass = seletedclass;

	FUnitySkillMissionArray* CurrentMissions = BladeIIGameImpl::GetLocalCharacterData().GetUnitySkillMission(MainPCClass);
	
	if(!CurrentMissions) return;
	if (!List_UinityAccuire.IsValid()) return;
	
	List_UinityAccuire->ClearChildren();

	EUnityBottomInfoType PanelState = EUnityBottomInfoType::InMission;
	EUnityLoopType UnityPanelLoopType = EUnityLoopType::Default;

	bool bRewardAble = true;

	for (auto MissionItem : *CurrentMissions) 
	{
		if (MissionItem.SubClass == SubPCClass)
		{
			if (MissionItem.SubClass == MainPCClass)
			{
				PanelState = EUnityBottomInfoType::Default;
			}
			else
			{
				if (MissionItem.nState != b2network::B2MissionStateType::PROGRESS)
					PanelState = EUnityBottomInfoType::ClearMission;
				else
					PanelState = EUnityBottomInfoType::InMission;
			}

			for (int32 i = 1; i < 3; i++) // 阿 固记狼 炼扒牢郸胶绰 1,2
			{
				UB2UIUnitySkillAcquireInfo* MissionInfoWidget = CreateWidget<UB2UIUnitySkillAcquireInfo>(GetWorld(), UnitySkillAcquireItemClass);
				if (MissionInfoWidget)
				{
					MissionInfoWidget->Init();

					FText MissionText = GLOBALUNITYSKILLMANAGER.GetTextSkillMission(MissionItem.MainClass, MissionItem.SubClass, MissionItem.nStep, i);
					if(MissionText.IsEmpty()) 
						continue;

					MissionInfoWidget->SetAcquireInfoText(MissionText);

					bool CheckCanReward = GLOBALUNITYSKILLMANAGER.CheckStateUnitySkillMissionByIndex(MissionItem.MainClass, MissionItem.SubClass, MissionItem.nStep, i);
					MissionInfoWidget->UpdateMissionSuccessState(CheckCanReward);
			
					List_UinityAccuire->AddChild(MissionInfoWidget);

					if (!CheckCanReward) // 窍唱扼档 炼扒俊 嘎瘤 臼阑版快 焊惑罐阑荐绝澜
						bRewardAble = false;
				}
			}
		}
	}

	BTN_UnityReward->SetIsEnabled(bRewardAble);

	for (auto UnityItem : UnityCharInfoArray)
	{
		if (UnityItem->PCClass == SubPCClass)
			UnityItem->OnChangeSelected(true);
		else
			UnityItem->OnChangeSelected(false);
	}

	if (IMG_UnitySelect.IsValid()) // 吝居滚瓢狼 急琶 捞固瘤 贸府绰 蝶肺.
		IMG_UnitySelect->SetVisibility(MainPCClass != SubPCClass ? ESlateVisibility::Hidden : ESlateVisibility::HitTestInvisible);

	if (STB_UnityTitle.IsValid())
		STB_UnityTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_Mission")));

	BTN_UnityAwake->SetVisibility(ESlateVisibility::Collapsed);
	BTN_UnityAwakeNotice->SetVisibility(ESlateVisibility::Collapsed);

	SetUnityBottomInfoPanel(PanelState);
	CheckAnimationUnityLoop(EUnityLoopType::AlreadyGet);

	if (Lobby_SetupSkill_UnityP_AwakePopup.IsValid())
	{
		B2AndroidBackManager::GetInstance()->RemoveBackWidget(Lobby_SetupSkill_UnityP_AwakePopup.Get());
		Lobby_SetupSkill_UnityP_AwakePopup->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UB2LobbyUI_SetupSkill_UnityP::SetUnityBottomInfoPanel(EUnityBottomInfoType infotype, int32 NextStep)
{
	int32 CurrentUnitySkillIndex = GetUnitySkillID(MainPCClass);

	switch (infotype)
	{
	case EUnityBottomInfoType::Default:
		if (TB_UnityName.IsValid())
			TB_UnityName->SetText(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::SkillName, CurrentUnitySkillIndex)));
		if (TB_UnityDesc.IsValid())
			TB_UnityDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::SkillDesc, CurrentUnitySkillIndex)));
		if (BTN_UnityDetail.IsValid())
			BTN_UnityDetail->SetVisibility(ESlateVisibility::Visible);
		if (BTN_UnityReward.IsValid())
			BTN_UnityReward->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EUnityBottomInfoType::InMission:
		if (BTN_UnityDetail.IsValid())
			BTN_UnityDetail->SetVisibility(ESlateVisibility::Hidden);
		if (BTN_UnityReward.IsValid())
			BTN_UnityReward->SetVisibility(ESlateVisibility::Visible);
		SetSkillOptionText();
		break;
	case EUnityBottomInfoType::ClearMission:
		if(TB_UnityChar1.IsValid())	
			TB_UnityChar1->SetText(BladeIIGameImpl::GetClientDataStore().GetLocalCharClassOrNickName(SubPCClass));
		if (BTN_UnityDetail.IsValid())
			BTN_UnityDetail->SetVisibility(ESlateVisibility::Hidden);
		if (BTN_UnityReward.IsValid())
			BTN_UnityReward->SetVisibility(ESlateVisibility::Hidden);
		break;
	}

	if (WS_UnityInfo.IsValid())
		WS_UnityInfo->SetActiveWidgetIndex((int32)infotype);
}

void UB2LobbyUI_SetupSkill_UnityP::SetSkillOptionText()
{
	int32 CurrentUnitySkillIndex = GetUnitySkillID(MainPCClass);
	FUnitySkillOption SkillOption = GLOBALUNITYSKILLMANAGER.GetUnitySkillOptionInfo(MainPCClass,SubPCClass,true);
	if (TB_SkillOption.IsValid())
	{
		if(SkillOption.OptionId != (int32)ESkillOption::ESO_IncMotion)
			TB_SkillOption->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::OptionTooltipDesc, CurrentUnitySkillIndex, SkillOption.OptionId)), FText::AsNumber(SkillOption.OptionValue)));
		else
			TB_SkillOption->SetText(BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::OptionTooltipDesc, CurrentUnitySkillIndex, SkillOption.OptionId, SkillOption.OptionValue)));
	}
}

void UB2LobbyUI_SetupSkill_UnityP::CheckAnimationUnityLoop(EUnityLoopType nType)
{
	switch (nType)
	{
	case EUnityLoopType::Default:
		break;
	case EUnityLoopType::AbleGet:
		break;
	case EUnityLoopType::AlreadyGet:
		UnityPortraiLoopAniForAllClear();
		break;
	}

	OnPlayUnitySkillLoopAnim_IMP(nType);
}

void UB2LobbyUI_SetupSkill_UnityP::UnityPortraiLoopAniForAllClear()
{
	for (auto CharItem : UnityCharInfoArray)
	{
		CharItem->OnPlayAnim_Portrait_Loop_IMP(false);
	}
}


void UB2LobbyUI_SetupSkill_UnityP::OnCallbackPlayMailRewardPopup_BP()
{
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().ClearTimer(DelayBlockPageCallbackTH);
	UGameplayStatics::GetGameMode(this)->GetWorldTimerManager().SetTimer(DelayBlockPageCallbackTH, this, &UB2LobbyUI_SetupSkill_UnityP::OnDelayBlockPage, 1.0f, false);

	ShowUnityStatPopup();
}

void UB2LobbyUI_SetupSkill_UnityP::OnCallbackUnityWakeReward_BP()
{
	PageBlock(false);
	UpdatePanelMain(MainPCClass);

	UB2UIManager::GetInstance()->OpenUI(UIFName::UnitySkillRewardPopup);

	auto* RewardPopupWidget = UB2UIManager::GetInstance()->GetUI<UB2UIUnitySkillRewardPopup>(UIFName::UnitySkillRewardPopup);
	if (RewardPopupWidget)
	{
		RewardPopupWidget->SetUnitySkillImage((PCClassToInt(MainPCClass) + 1) * 100 + 3);
	}
}

int32 UB2LobbyUI_SetupSkill_UnityP::GetUnitySkillID(EPCClass InPCClass)
{
	return BladeIIGameImpl::GetLocalCharacterData().GetUnitySkillId(InPCClass);
}

void UB2LobbyUI_SetupSkill_UnityP::ShowUnityStatPopup()
{
	int32 CurrentUnitySkillIndex = GetUnitySkillID(MainPCClass);
	FUnitySkillOption SkillOption = GLOBALUNITYSKILLMANAGER.GetUnitySkillOptionInfo(MainPCClass, SubPCClass);

	UB2UIUpgradeStatNotify* UpgradeStatNotifyUI = UB2UIManager::GetInstance()->OpenUI<UB2UIUpgradeStatNotify>(UIFName::UpgradeStatNotify);
	FText SkillOptionText;
	if (UpgradeStatNotifyUI != nullptr)
	{
		UpgradeStatNotifyUI->ClearList();
		SkillOptionText = BladeIIGetLOCText(B2LOC_CAT_SKILLDATA, GetSkillLocalizedTextKey(ESkillTextDataType::OptionPopupDesc, CurrentUnitySkillIndex, SkillOption.OptionId));
		UpgradeStatNotifyUI->AddListItem(SkillOptionText, GetSkillOptionValueDisplayText(SvrToCliSkillOption(SkillOption.OptionId), SkillOption.OptionValue), SkillOption.OptionValue > 0);
	}

	UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(MainPCClass, true);

}

void UB2LobbyUI_SetupSkill_UnityP::OnDelayBlockPage()
{
	if (OwnerSkillMainPage)
	{
		OwnerSkillMainPage->SetVisibleTouchBlockImage(false);
	}
}

void UB2LobbyUI_SetupSkill_UnityP::SetIsEther()
{
	bool bIsEther = false;
	FB2OriginCharacterInfoPtr PlayerStatBasicInfo = BladeIIGameImpl::GetLocalCharacterData().GetPlayerStatBasicInfo(MainPCClass);
	if (PlayerStatBasicInfo)
	{
		if (PlayerStatBasicInfo->aether_offense_set_option_id == b2network::B2AetherSetOptionType::SKILL_GAGE)
		{
			bIsEther = true;
		}
	}

	if (P_IsEther.IsValid())
		P_IsEther->SetVisibility(bIsEther ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

}

void UB2LobbyUI_SetupSkill_UnityP::SetUnityAwakeStateButton()
{
	FUnitySkillState* UnityState = BladeIIGameImpl::GetLocalCharacterData().GetUnitySkillState(MainPCClass);

	if (BTN_UnityAwake.IsValid() && BTN_UnityAwakeNotice.IsValid() && STB_UnityAwake.IsValid())
	{
		if (UnityState->HaveAwaken)
		{
			BTN_UnityAwake->SetVisibility(ESlateVisibility::Collapsed);
			BTN_UnityAwakeNotice->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			if (UnityState->AllAwakenComplete)
			{
				BTN_UnityAwake->SetVisibility(ESlateVisibility::Visible);
				BTN_UnityAwakeNotice->SetVisibility(ESlateVisibility::Collapsed);
				STB_UnityAwake->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_Awaken")));

				if (Lobby_SetupSkill_UnityP_AwakePopup.IsValid())
				{
					if (Lobby_SetupSkill_UnityP_AwakePopup->GetVisibility() == ESlateVisibility::Visible)
					{
						B2AndroidBackManager::GetInstance()->RemoveBackWidget(Lobby_SetupSkill_UnityP_AwakePopup.Get());
						Lobby_SetupSkill_UnityP_AwakePopup->SetVisibility(ESlateVisibility::Collapsed);
					}
				}

			}
			else if (UnityState->AllMissionComplete)
			{
				BTN_UnityAwake->SetVisibility(ESlateVisibility::Visible);
				BTN_UnityAwakeNotice->SetVisibility(ESlateVisibility::Collapsed);
				STB_UnityAwake->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_AwakenMission")));
			}
			else
			{
				BTN_UnityAwake->SetVisibility(ESlateVisibility::Collapsed);
				BTN_UnityAwakeNotice->SetVisibility(ESlateVisibility::Visible);
				STB_UnityAwake->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_AwakenMission")));
			}
		}


	}
}

void UB2LobbyUI_SetupSkill_UnityP::OnAllClearUnityAwakenMission()
{
	SetUnityAwakeStateButton();
}

void UB2LobbyUI_SetupSkill_UnityP::PageBlock(bool bIsBlock)
{
	if (GetRootWidget())
		GetRootWidget()->SetVisibility(bIsBlock ? ESlateVisibility::HitTestInvisible : ESlateVisibility::SelfHitTestInvisible);

	if (CachedLobbyGM->GetLobbyInventory())
	{
		UB2LobbyUI_SetupSkillMain* InventoryUI = CachedLobbyGM ? Cast<UB2LobbyUI_SetupSkillMain>(CachedLobbyGM->DJLegacy_GetCurrLobbyUIPage()) : nullptr;
		if (InventoryUI)
			InventoryUI->SetVisibility(bIsBlock ? ESlateVisibility::HitTestInvisible : ESlateVisibility::SelfHitTestInvisible);
	}

	UB2UIHeader* HeaderUI = UB2UIManager::GetInstance()->GetUI<UB2UIHeader>(UIFName::Header);
	if (HeaderUI)
		HeaderUI->SetIsEnabled(!bIsBlock);
}

////////////////////////////////////
// UB2UIUnitySkillAcquireInfo
////////////////////////////////////

void UB2UIUnitySkillAcquireInfo::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UWidgetSwitcher, WS_Line);
	GET_SLOT(UTextBlock, TB_OFF_Info);
	GET_SLOT(UTextBlock, TB_On_Info);
	GET_SLOT(UTextBlock, STB_Success);
}

void UB2UIUnitySkillAcquireInfo::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIUnitySkillAcquireInfo::Init()
{
	Super::Init();
}

void UB2UIUnitySkillAcquireInfo::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIUnitySkillAcquireInfo::UpdateStaticText()
{
	if (STB_Success.IsValid())
		STB_Success->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Skill_Unity_Complete")));
}

void UB2UIUnitySkillAcquireInfo::SetAcquireInfoText(const FText infotext)
{
	if (TB_OFF_Info.IsValid())
		TB_OFF_Info->SetText(infotext);
	if (TB_On_Info.IsValid())
		TB_On_Info->SetText(infotext);
}

void UB2UIUnitySkillAcquireInfo::UpdateMissionSuccessState(bool bsuccess)
{
	if (WS_Line.IsValid())
		WS_Line->SetActiveWidgetIndex(bsuccess);
}

////////////////////////////////////
// UB2UIUnityCharacterInfo
////////////////////////////////////

void UB2UIUnityCharacterInfo::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UImage, IMG_Portrait);
	GET_SLOT(UImage, IMG_Select);

	SlotImages.Empty();
	SlotImages.SetNum(MAX_UNITYSTEP_COUNT);
	GET_SLOT_BYNAME(UImage, SlotImages[0], IMG_Slot_01);
	GET_SLOT_BYNAME(UImage, SlotImages[1], IMG_Slot_02);
	GET_SLOT_BYNAME(UImage, SlotImages[2], IMG_Slot_03);
	GET_SLOT_BYNAME(UImage, SlotImages[3], IMG_Slot_04);
	GET_SLOT_BYNAME(UImage, SlotImages[4], IMG_Slot_05);
	GET_SLOT_BYNAME(UImage, SlotImages[5], IMG_Slot_06);
	GET_SLOT_BYNAME(UImage, SlotImages[6], IMG_Slot_07);
	GET_SLOT_BYNAME(UImage, SlotImages[7], IMG_Slot_08);

	GET_SLOT(UB2Button, BTN_Select);

	BIND_CLICK_FUNC_TO_BTN(BTN_Select, &UB2UIUnityCharacterInfo::OnClickedSelectButton);
}

void UB2UIUnityCharacterInfo::BindDelegates()
{
	Super::BindDelegates();
}

void UB2UIUnityCharacterInfo::Init()
{
	Super::Init();
}

void UB2UIUnityCharacterInfo::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIUnityCharacterInfo::SetCharacterPortrait(EPCClass pcclass)
{
	if (IMG_Portrait.IsValid())
		IMG_Portrait->SetBrushFromMaterial(FB2UnitySkillPortraitImage::GetCharImage(PortraitInfo, pcclass));

	PCClass = pcclass;
}

void UB2UIUnityCharacterInfo::SetMissionClearCount(int32 clearcount)
{
	for (int32 i = 0; i < SlotImages.Num(); i++)
	{
		bool bIsVisible = (i < clearcount);
		SlotImages[i]->SetVisibility( bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UB2UIUnityCharacterInfo::OnChangeSelected(bool selected)
{
	if (IMG_Select.IsValid())
		IMG_Select->SetVisibility(selected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UB2UIUnityCharacterInfo::OnClickedSelectButton()
{
	ButtonDelegate.ExecuteIfBound(PCClass);
}