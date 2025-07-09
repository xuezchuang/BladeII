// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "B2UILobbyNoticeSlot.h"
#include "B2UIManager.h"
#include "BladeIIUtil.h"
#include "BladeIIGameImpl.h"
#include "Retailer.h"
#include "B2UILinkManager.h"
#include "B2UIDocHelper.h"
#include "TutorialManager.h"
#include "B2UIMsgPopupSelectPCClass.h"
#include "B2UIMsgPopupSimple.h"
#include "B2LobbyGameMode.h"

UB2UILobbyNoticeSlot::UB2UILobbyNoticeSlot(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	m_eModeOpenType = EModeOpenType::EModeOpenType_Raid;
}

void UB2UILobbyNoticeSlot::Init()
{
	Super::Init();
}

void UB2UILobbyNoticeSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(LeftTimeHandler);
	}
}

void UB2UILobbyNoticeSlot::CacheAssets()
{
	Super::CacheAssets();

	GET_SLOT(UTextBlock, TB_Mode);
	GET_SLOT(UTextBlock, TB_Notice);
	GET_SLOT(UButton, BTN_ModeOpen);
	GET_SLOT(UTextBlock, TB_LeftTime);
}

void UB2UILobbyNoticeSlot::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_ModeOpen, &UB2UILobbyNoticeSlot::OnClickBtnModeOpen)
}

void UB2UILobbyNoticeSlot::RegisterUIMarkForRedDot()
{

}

void UB2UILobbyNoticeSlot::SetModeOpenType(int32 iType)
{
	SetModeIcon_BP(iType);

	m_eModeOpenType = (EModeOpenType)iType;

	bUsedInvitePopup = false;
}

void UB2UILobbyNoticeSlot::OnClickBtnModeOpen()
{
	switch (m_eModeOpenType)	//링커매니저 사용, 씬 등록하고 사용
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
			{ }
				
		}break;
	case EModeOpenType::EModeOpenType_Raid_Request:
		OpenRaidInviteCheckPopup();
		break;
	default:
		{

		}break;
	}
}

void UB2UILobbyNoticeSlot::OpenRaidInviteCheckPopup()
{
	auto* DocRaid = UB2UIDocHelper::GetDocRaid();
	auto UIManager = UB2UIManager::GetInstance();
	if (!DocRaid || !UIManager) return;
	/*
	const FRaidInviteInfo& InviteInfo = DocRaid->GetFirstRaidInviteInfo();

	FText InviterName = FText::FromString(InviteInfo.InviterName);
	if (InviterName.IsEmpty()) InviterName = FText::FromString(TEXT("Raid Player"));
	FText RaidName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, FString(TEXT("Raid_Name") + FString::FromInt(InviteInfo.RaidType)));
	FText RaidDifficultyStr = FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("counterDungeon_Step")), FText::AsNumber((int32)InviteInfo.RaidStep));

	UB2UIMsgPopupSimple* pPopupUI = UIManager->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::SimpleInvite,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
		FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_NotificationInvitieMessage")), InviterName, RaidName, RaidDifficultyStr),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::ConfirmOrCancel
		);


	// 추가코멘트
	pPopupUI->SetSubContent(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_AutoReject")), FText::AsNumber(nCurrentLeftTime)));
	pPopupUI->AddHandler(EUIMsgPopupButton::Positive, FMsgPopupOnClick::CreateLambda([this]() 
	{ 
		data_trader::Retailer::GetInstance().RequestGetChallengeModeInfo();  
		this->OpenRaidInviteSelectClassPopup(); 
	}));

	pPopupUI->AddHandler(EUIMsgPopupButton::Negative, FMsgPopupOnClick::CreateLambda([this]() { this->RemoveRaidInvite(); }));
	pPopupUI->SetButtonText(EUIMsgPopupButton::Positive, BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), FString(TEXT("General_Accept"))));

	bUsedInvitePopup = true;
	*/
}

void UB2UILobbyNoticeSlot::OpenRaidInviteSelectClassPopup()
{
	if (TutorialManager::GetInstance().IsLockTutorial(TutorialID_Raid, 0)) // 레이드 잠금시 팝업 호출
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

		return;
	}

	auto UIManager = UB2UIManager::GetInstance();
	if (!UIManager) return;

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
		TheWorld->GetTimerManager().ClearTimer(LeftTimeHandler);

	auto* RaidInvitePopupUI = UIManager->OpenMsgPopup<UB2UIMsgPopupSelectPCClass>(EUIMsgPopup::SelectPCClass,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")), FText::GetEmpty(),
		0.0f,
		true,
		true,
		EUIMsgPopupButtonGroup::AcceptOrReject);


	// RequestRaidStart 이름은 레이드 시작이지만, 서버팀에서 프로토콜 재사용으로
	// 레이드 초대시에 어떤 캐릭터를 선택했는지 알리고, 리스폰스를 받은 후에 룸조인.
	if (RaidInvitePopupUI)
	{
		RaidInvitePopupUI->SetAutoCloseOnClick(false); // 리스폰스를 처리한 후에 닫아야함
		RaidInvitePopupUI->AddHandler(EUIMsgPopupButton::Positive, FMsgPopupOnClick::CreateLambda([this, RaidInvitePopupUI]() {
			data_trader::Retailer::GetInstance().RequestRaidStart(CliToSvrPCClassType(RaidInvitePopupUI->GetSelectPCClass()));
			RemoveRaidInvite();
		}));
	}
}

void UB2UILobbyNoticeSlot::RemoveRaidInvite()
{
	bUsedInvitePopup = false;
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
		TheWorld->GetTimerManager().ClearTimer(LeftTimeHandler);

	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UB2UILobbyNoticeSlot::SetTimeText(int32 nSecond)
{
	if(TB_LeftTime.IsValid())
		TB_LeftTime->SetText(FText::AsNumber(nCurrentLeftTime));
}

void UB2UILobbyNoticeSlot::StartTimer(int32 nSecond)
{
	nCurrentLeftTime = nSecond;
	SetTimeText(nCurrentLeftTime);

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(LeftTimeHandler);
		TheWorld->GetTimerManager().SetTimer(LeftTimeHandler, this, &UB2UILobbyNoticeSlot::CheckLimittime, 1.0f, true);
	}
}

void UB2UILobbyNoticeSlot::CheckLimittime()
{
	/*
	nCurrentLeftTime -= 1;
	// 초대 팝업이 있다면... 팝업도 똑같이 갱신.
	UB2UIMsgPopupSimple* pPopupUI = bUsedInvitePopup ? UB2UIManager::GetInstance()->GetUI<UB2UIMsgPopupSimple>(UIFName::MsgPopupSimpleInvite) : nullptr;
	if (nCurrentLeftTime < 1)
	{
		if (UB2UIDocRaid* UIDocRaid = UB2UIDocHelper::GetDocRaid())
			UIDocRaid->ClearRaidInviteInfo();

		RemoveRaidInvite();

		if (pPopupUI)
			pPopupUI->CloseSelf();
	}
	else
	{
		SetTimeText(nCurrentLeftTime);

		if (pPopupUI)
			pPopupUI->SetSubContent(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Raid_AutoReject")), FText::AsNumber(nCurrentLeftTime)));
	}
	*/
}

