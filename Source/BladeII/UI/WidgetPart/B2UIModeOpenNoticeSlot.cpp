// Fill out your copyright notice in the Description page of Project Settings.
#include "BladeII.h"
#include "B2UIManager.h"
#include "B2UILinkManager.h"
#include "BladeIIUtil.h"
#include "BladeIIGameImpl.h"
#include "TutorialManager.h"
#include "Retailer.h"
#include "B2UIDocHelper.h"
#include "B2GameInstance.h"
#include "B2UIModeOpenNoticeSlot.h"

UB2UIModeOpenNotice::UB2UIModeOpenNotice(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CheckInfos.Empty();
	m_bSubscribed = false;
	CurrentTimeStemp = 0;
	bActiveTick = false;
}

void UB2UIModeOpenNotice::Init()
{
	Super::Init();

	ClearInfoWidgets();

	SubscribeEvent();
}

void UB2UIModeOpenNotice::SubscribeEvent()
{
	if (m_bSubscribed == false)
	{
		Issues.Add(DeliveryCheckModeOpenClass<FB2ResponseCheckModeOpenPtr>::GetInstance().Subscribe2(
			[this](FB2ResponseCheckModeOpenPtr fb2r_CheckModeOpen)
		{
			this->OnReceiveCheckModeOpen(fb2r_CheckModeOpen);
		}
		));
		Issues.Add(PlatformPausePressClass<>::GetInstance().Subscribe2(
			[this]()
		{
			this->RefreshCurrentTime();
		}
		));

		m_bSubscribed = true;
	}
}

void UB2UIModeOpenNotice::UnSubscribeEvent()
{
	m_bSubscribed = false;
}

void UB2UIModeOpenNotice::DestroySelf(class UB2UIManager* InUIManager)
{
	UnSubscribeEvent();

	ClearInfoWidgets();

	Super::DestroySelf(InUIManager);
}

void UB2UIModeOpenNotice::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UPanelWidget, P_ContentNotice);

	if (P_ContentNotice.IsValid())
		P_ContentNotice->ClearChildren();

}

void UB2UIModeOpenNotice::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (CheckInfos.Num() < 1)
		return;

	if (!bActiveTick)
		return;

	CurrentTimeStemp += InDeltaTime;

	if (CurrentTimeStemp > 86400) // 날짜가 바뀌었음.
	{
		bActiveTick = false;
		data_trader::Retailer::GetInstance().RequestCheckModeOpen();

		return;
	}

	if (P_ContentNotice.IsValid())
	{
		TArray<EModeOpenType> AddModeType;
		TArray<EModeOpenType> DeleteModeType;

		for (auto EachItem : CheckInfos)
		{
			if (FMath::IsWithin(CurrentTimeStemp, EachItem.StartTime, EachItem.EndTime))
			{
				AddModeType.Add(EachItem.ModeType);
			}
			else if (EachItem.ModeType == EModeOpenType::EModeOpenType_Guild_Enter && 
				BladeIIGameImpl::GetClientDataStore().GetUserGuildID())
			{
				AddModeType.Add(EachItem.ModeType);
			}
			else
			{
				DeleteModeType.Add(EachItem.ModeType);
			}
		}

		CreateOrRemoveSlot(AddModeType, DeleteModeType);
	}
}

void UB2UIModeOpenNotice::CreateOrRemoveSlot(TArray<EModeOpenType>& AddModeType, TArray<EModeOpenType>& DeleteModeType)
{
	// 시간 값이 2개씩 있어 하나는 이미 끝났고 하나는 열려있을때 처리를 위해..
	for (int32 i = 0; i < AddModeType.Num(); i++) 
	{
		if (AddModeType.IsValidIndex(i))
		{
			if (DeleteModeType.Contains(AddModeType[i]))
			{
				DeleteModeType.Remove(AddModeType[i]);
			}
		}
	}

	for (int32 i = 0; i < AddModeType.Num(); i++)
	{
		if (AddModeType.IsValidIndex(i))
		{
			if (!IsValidSlotWidget(AddModeType[i]))
			{
				AddModeOpenSlot(AddModeType[i]);
			}
		}
	}

	for (int32 i = 0; i < DeleteModeType.Num(); i++)
	{
		if (DeleteModeType.IsValidIndex(i))
		{
			if (IsValidSlotWidget(DeleteModeType[i]))
			{
				DeleteModeOpenSlot(DeleteModeType[i]);
			}
		}
	}
}

bool UB2UIModeOpenNotice::IsValidSlotWidget(EModeOpenType eModeType)
{
	for (int32 i = 0; i < P_ContentNotice->GetChildrenCount(); i++)
	{
		if (P_ContentNotice->GetChildAt(i))
		{
			UB2UIModeOpenNoticeSlot* ChildWidget = Cast<UB2UIModeOpenNoticeSlot>(P_ContentNotice->GetChildAt(i));
			
			if (ChildWidget && ChildWidget->GetModeType() == eModeType)
				return true;
		}
	}

	return false;
}

void UB2UIModeOpenNotice::ClearInfoWidgets()
{
	CheckInfos.Empty();

	if (P_ContentNotice.IsValid())
	{
		for (int32 i = 0; i < P_ContentNotice->GetChildrenCount(); i++)
		{
			if (P_ContentNotice->GetChildAt(i))
			{
				UB2UIModeOpenNoticeSlot* ChildWidget = Cast<UB2UIModeOpenNoticeSlot>(P_ContentNotice->GetChildAt(i));
				if (ChildWidget)
				{
					ChildWidget->DestroySelf(NULL);
				}
			}
		}
		
		P_ContentNotice->ClearChildren();
	}
}

void UB2UIModeOpenNotice::OnReceiveCheckModeOpen(const FB2ResponseCheckModeOpenPtr& fb2r_CheckModeOpen)
{
	ClearInfoWidgets();

	TArray<b2network::B2MatchOpenPeriodInfoPtr> OpenTimes;
	bool bRaidIsOpen = BladeIIGameImpl::GetClientDataStore().GetCurrentModeOpenTimes(fb2r_CheckModeOpen->raid_open_period_infos, OpenTimes);

	for (auto TimeInfo : OpenTimes)
	{
		int32 TimeOpen = (TimeInfo->begin_hour * 3600) + (TimeInfo->begin_min*60);
		int32 TimeClose = (TimeInfo->end_hour * 3600) + (TimeInfo->end_min * 60);

		FCheckModeInfo NewItem;
		NewItem.ModeType = EModeOpenType::EModeOpenType_Raid;
		NewItem.StartTime = static_cast<float>(TimeOpen);
		NewItem.EndTime = static_cast<float>(TimeClose);
		CheckInfos.Add(NewItem);
	}

	bool bAssultIsOpen = BladeIIGameImpl::GetClientDataStore().GetCurrentModeOpenTimes(fb2r_CheckModeOpen->assault_open_period_infos, OpenTimes);

	for (auto TimeInfo : OpenTimes)
	{
		int32 TimeOpen = (TimeInfo->begin_hour * 3600) + (TimeInfo->begin_min * 60);
		int32 TimeClose = (TimeInfo->end_hour * 3600) + (TimeInfo->end_min * 60);

		FCheckModeInfo NewItem;
		NewItem.ModeType = EModeOpenType::EModeOpenType_Occupy;
		NewItem.StartTime = static_cast<float>(TimeOpen);
		NewItem.EndTime = static_cast<float>(TimeClose);
		CheckInfos.Add(NewItem);
	}

	CheckInfos.Sort([](const FCheckModeInfo& Data1, const FCheckModeInfo& Data2)->bool {
		return Data1.StartTime < Data2.StartTime;
	});

	if (fb2r_CheckModeOpen->guild_battle_state == b2network::B2GuildBattleStateType::READY ||
		fb2r_CheckModeOpen->guild_battle_state == b2network::B2GuildBattleStateType::BATTLE)
	{
		FCheckModeInfo NewItem;
		NewItem.ModeType = EModeOpenType::EModeOpenType_Guild_Enter;
		CheckInfos.Add(NewItem);
	}
	RefreshCurrentTime();

	bActiveTick = true;
}

void UB2UIModeOpenNotice::RefreshCurrentTime()
{
	FDateTime DateNow = UB2GameInstance::GetKoreaTimeNow();
	int32 nNowHour = DateNow.GetHour();
	int32 TimeNow = (nNowHour * 3600) + (DateNow.GetMinute() * 60) + DateNow.GetSecond();

	CurrentTimeStemp = static_cast<float>(TimeNow);
}

void UB2UIModeOpenNotice::AddModeOpenSlot(EModeOpenType eModeType)
{
	if (P_ContentNotice.IsValid())
	{
		UB2UIModeOpenNoticeSlot* SlotItem = CreateWidget<UB2UIModeOpenNoticeSlot>(GetWorld(), ModeOpenNoticeSlotWidget);
		if (SlotItem)
		{
			SlotItem->Init();
			SlotItem->SetInfo(eModeType);
			SlotItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			P_ContentNotice->AddChild(SlotItem);
		}
	}
}

void UB2UIModeOpenNotice::DeleteModeOpenSlot(EModeOpenType eModeType)
{
	for (int32 i = 0; i < P_ContentNotice->GetChildrenCount(); i++)
	{
		if (P_ContentNotice->GetChildAt(i))
		{
			UB2UIModeOpenNoticeSlot* ChildWidget = Cast<UB2UIModeOpenNoticeSlot>(P_ContentNotice->GetChildAt(i));
			if (ChildWidget)
			{
				if (ChildWidget->GetModeType() == eModeType)
				{
					P_ContentNotice->RemoveChild(ChildWidget);
					ChildWidget->DestroySelf(NULL);
					return;
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////
//                         SlotItem
///////////////////////////////////////////////////////////////////////

UB2UIModeOpenNoticeSlot::UB2UIModeOpenNoticeSlot(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CurrentOpenType = EModeOpenType::EModeOpenType_Raid;
}

void UB2UIModeOpenNoticeSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_ModeName);
	GET_SLOT(UB2Button, BTN_ModeOpen);
	GET_SLOT(UWidgetSwitcher, SW_ModeImages);
}

void UB2UIModeOpenNoticeSlot::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_ModeOpen, &UB2UIModeOpenNoticeSlot::OnClickBTN_ModeOpen);
}

void UB2UIModeOpenNoticeSlot::Init()
{
	Super::Init();
}

void UB2UIModeOpenNoticeSlot::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
}

void UB2UIModeOpenNoticeSlot::OnClickBTN_ModeOpen()
{
	switch (CurrentOpenType)	//링커매니저 사용, 씬 등록하고 사용
	{
	case EModeOpenType::EModeOpenType_Raid:	//튜토리얼 오픈(레이드)
	{
		if (TutorialManager::GetInstance().IsLockTutorial(TutorialID_Raid, 0) == false)
		{
			FB2UILinkManager::LinkUIScene(EUIScene::RaidMain);
		}
		else
		{
			FText ftTitle = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ModeOpen_RaidUnlock1"));
			FText ftOpenCondition = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ModeOpen_RaidUnlock2"));

			FString DescString = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_SpanColor")), FText::FromString(FColor::Yellow.ToHex()), ftTitle).ToString();
			DescString.Append(TEXT("\n"));
			DescString.Append(ftOpenCondition.ToString());

			UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				FText::FromString(DescString),
				0.f, true, true, EUIMsgPopupButtonGroup::Confirm);
		}
	}break;
	case EModeOpenType::EModeOpenType_Occupy:	//튜토리얼 오픈(점령전)
	{
		if (TutorialManager::GetInstance().IsLockTutorial(TutorialID_Occupy, 0) == false)
		{
			FB2UILinkManager::LinkUIScene(EUIScene::ControlMain);
		}
		else
		{
			FText ftTitle = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ModeOpen_ControlUnlock1"));
			FText ftOpenCondition = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ModeOpen_ControlUnlock2"));

			FString DescString = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_SpanColor")), FText::FromString(FColor::Yellow.ToHex()), ftTitle).ToString();
			DescString.Append(TEXT("\n"));
			DescString.Append(ftOpenCondition.ToString());

			UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
				FText::FromString(DescString),
				0.f, true, true, EUIMsgPopupButtonGroup::Confirm);
		}
	}break;
	case EModeOpenType::EModeOpenType_Guild_Enter:
	case EModeOpenType::EModeOpenType_Guild_Request:
	{
		int64 iUserGuildID = BladeIIGameImpl::GetClientDataStore().GetUserGuildID();
		if (iUserGuildID)
		{
			data_trader::Retailer::GetInstance().RequestGuildDetailInfo(0);
		}
		else //길드전을 입장 못할시라도 길드메뉴창에 들어가지기 때문에 별도의 예외처리 없음(기획의도)
		{
		}

	}break;
	default:
		break;
	}
}

void UB2UIModeOpenNoticeSlot::SetInfo(EModeOpenType ModeNum)
{
	CurrentOpenType = ModeNum;
	SetIcon(ModeNum);
}

void UB2UIModeOpenNoticeSlot::SetIcon(EModeOpenType ModeNum)
{
	if (!SW_ModeImages.IsValid() || !TB_ModeName.IsValid())
		return;

	switch (ModeNum)
	{
	case EModeOpenType::EModeOpenType_Raid:
		SW_ModeImages->SetActiveWidgetIndex(0);
		TB_ModeName->SetText((BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_Raid"))));
		break;
	case EModeOpenType::EModeOpenType_Occupy:
		SW_ModeImages->SetActiveWidgetIndex(1);
		TB_ModeName->SetText((BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_Control"))));
		break;
	case EModeOpenType::EModeOpenType_Guild_Enter:
	case EModeOpenType::EModeOpenType_Guild_Request:
		SW_ModeImages->SetActiveWidgetIndex(2);
		TB_ModeName->SetText((BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ChallengeMod_GuildMatch"))));
		break;
	default:
		break;
	}
}


