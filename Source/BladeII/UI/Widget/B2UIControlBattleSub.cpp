// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIControlBattleSub.h"
#include "BladeIINetControlObject.h"
#include "B2UIControlTeamPlayerInfo.h"
#include "B2UIControlKillInfo.h"
#include "B2ControlGameMode.h"
#include "BladeIINetPlayer.h"
#include "B2UIDocHelper.h"
#include "B2UIControlChatPanel.h"
#include "Packet.h"
#include "PacketMaker.h"
#include "B2UIManager.h"
#include "B2UIBattleMain.h"


UB2UIControlBattleSub::UB2UIControlBattleSub(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	//ControlChatCoolTime = 5.0f;
	//ControlChatSendTimeSec = -1.0f;
	//ControlSerialKillTextureInfos.Empty();
	//vComboUIPos = FVector2D(0.0f, 70.0f);
	//m_arTeamPlayerInfoUI.Empty();
}

void UB2UIControlBattleSub::Init()
{
	Super::Init();

	SubscribeEvent();

	SetMsgSubmersion(false);
}

void UB2UIControlBattleSub::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	UnsubscribeEvent();
}

void UB2UIControlBattleSub::NativeConstruct()
{
	Super::NativeConstruct();

	InitPlayerInfos();

	SetTeamKillCount(0, 0);

	// 초기화
	// UI가 컨트롤 오브젝트보다 나중에 생성됨. 정보 요청
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));

	if (pGM)
		pGM->ApplyCurrentStateToUI();

	// 배틀 메인에 콤보UI위치 보정
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();
	if (pUIManager)
	{
		UB2UIBattleMain* pUI = pUIManager->GetUI<UB2UIBattleMain>(UIFName::BattleMain);

		if (pUI)
		{
			pUI->SetComboUIPosition(vComboUIPos);
		}
	}
}

void UB2UIControlBattleSub::UpdateStaticText()
{
	Super::UpdateStaticText();

	if (TB_MinoNotice.IsValid())
		TB_MinoNotice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlMinoNotice")));

	if (TB_SancNotice.IsValid())
		TB_SancNotice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlBuffNotice")));

	if (T_R.IsValid())
		T_R->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlRedTeam")));

	if (T_B.IsValid())
		T_B->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlBlueTeam")));

	if (TB_MyAssultNoticeR.IsValid())
		TB_MyAssultNoticeR->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlOccupation")));

	if (TB_MyAssultNoticeB.IsValid())
		TB_MyAssultNoticeB->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlOccupation")));

	if(TB_LeaveEnemy.IsValid())
		TB_LeaveEnemy->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("PVP1on1_Escape")));

	if (TB_SubmersionTitle.IsValid())
	{
		TB_SubmersionTitle->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Control_FirstWarningTitle")));
	}

	if (TB_SubmersionDesc.IsValid())
	{
		TB_SubmersionDesc->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Control_FirstWarningDesc")));
	}
}

void UB2UIControlBattleSub::BindDelegates()
{
	Super::BindDelegates();

	BIND_CLICK_FUNC_TO_BTN(BTN_Chat, &UB2UIControlBattleSub::OnClickBtnChat);
}

void UB2UIControlBattleSub::SetControlObjectState(uint8 ControlObjectState, float fConquestGageRed, float fConquestGageBlue)
{
	UMaterialInterface *pMarkMI = NULL;
	UMaterialInterface *pGageBGMI = NULL;

	uint8 orgControlObjectState = m_ControlObjectState;

	m_ControlObjectState = ControlObjectState;

	switch (ControlObjectState)
	{
	case CONTROL_STATE_NEUTRAL:
		pMarkMI = ControlAreaMarkNeutral.LoadSynchronous();
		// 일단 초기화하고
		SetProgressGage(0.0f);		
		break;
	case CONTROL_STATE_RED:
		pMarkMI = ControlAreaMarkRed.LoadSynchronous();
		pGageBGMI = ConquestGageRed.LoadSynchronous();
		break;
	case CONTROL_STATE_BLUE:
		pMarkMI = ControlAreaMarkBlue.LoadSynchronous();
		pGageBGMI = ConquestGageBlue.LoadSynchronous();
		break;
	}
	
	BII_CHECK(fConquestGageRed == 0.0f || fConquestGageBlue == 0.0f)

	// 둘다 세팅한다. 둘중하나는 0이어야 정상
	if (fConquestGageRed > 0.0f)
		SetConquestGageInfo(true, fConquestGageRed);

	if (fConquestGageBlue > 0.0f)
		SetConquestGageInfo(false, fConquestGageBlue);

	SetControlStateImage(pMarkMI);
	SetProgressBackImage(pGageBGMI);

	// BG용 게이지 중립일땐 안보이게
	if (IMG_PrograssBG.IsValid())
		IMG_PrograssBG->SetVisibility(ControlObjectState == CONTROL_STATE_NEUTRAL ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	// 지금막 점령지를 뺏거나 뺏긴 경우
	if (ControlObjectState != CONTROL_STATE_NEUTRAL &&
		orgControlObjectState != m_ControlObjectState &&
		fConquestGageRed == 0.0f &&
		fConquestGageBlue == 0.0f)
	{
		AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
		if (pGM)
		{
			ETeamType MyTeamType = ETeamType(pGM->GetMyTeamNum());

			if ((MyTeamType == ETeamType::ETT_Red &&  ControlObjectState == CONTROL_STATE_RED)
				|| (MyTeamType == ETeamType::ETT_Blue &&  ControlObjectState == CONTROL_STATE_BLUE))
			{
				//성공
				ViewControlGameNotice(true);
			}
			else
			{
				//실패
				ViewControlGameNotice(false);
			}
		}
	}
}

void UB2UIControlBattleSub::SetConquestGageInfo(bool bRed, float fRate)
{
	UMaterialInterface *pGageMI = NULL;
	if (bRed)
		pGageMI = ConquestGageRed.LoadSynchronous();
	else
		pGageMI = ConquestGageBlue.LoadSynchronous();

	SetProgressImage(pGageMI);
	SetProgressGage(fRate);
}

void UB2UIControlBattleSub::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UImage, IMG_PrograssBG);
	GET_SLOT(UImage, IMG_Prograss);
	GET_SLOT(UImage, IMG_Stat);

	GET_SLOT(UProgressBar, PB_ControlPointRed);
	GET_SLOT(UProgressBar, PB_ControlPointBlue);

	GET_SLOT(UOverlay, O_Red1);
	if (O_Red1.IsValid())
		arRedPlayerOverlays.Add(O_Red1.Get());

	GET_SLOT(UOverlay, O_Red2);
	if (O_Red2.IsValid())
		arRedPlayerOverlays.Add(O_Red2.Get());

	GET_SLOT(UOverlay, O_Red3);
	if (O_Red3.IsValid())
		arRedPlayerOverlays.Add(O_Red3.Get());

	GET_SLOT(UOverlay, O_Red4);
	if (O_Red4.IsValid())
		arRedPlayerOverlays.Add(O_Red4.Get());

	GET_SLOT(UOverlay, O_Blue1);
	if (O_Blue1.IsValid())
		arBluePlayerOverlays.Add(O_Blue1.Get());

	GET_SLOT(UOverlay, O_Blue2);
	if (O_Blue2.IsValid())
		arBluePlayerOverlays.Add(O_Blue2.Get());

	GET_SLOT(UOverlay, O_Blue3);
	if (O_Blue3.IsValid())
		arBluePlayerOverlays.Add(O_Blue3.Get());

	GET_SLOT(UOverlay, O_Blue4);
	if (O_Blue4.IsValid())
		arBluePlayerOverlays.Add(O_Blue4.Get());

	GET_SLOT(UVerticalBox, VB_Kill_Infos);

	GET_SLOT(UTextBlock, TB_PointRed);
	GET_SLOT(UTextBlock, TB_PointBlue);
	GET_SLOT(UTextBlock, TB_KillCountRed);
	GET_SLOT(UTextBlock, TB_KillCountBlue);
	GET_SLOT(UTextBlock, TB_LeaveEnemy);

	GET_SLOT(UImage, IMG_SerialKillCount);
	GET_SLOT(UTextBlock, TB_ControlGameNotice);
	GET_SLOT(UTextBlock, TB_RemainGameTime);
		
	GET_SLOT(UOverlay, O_MyAssultNotice);
	GET_SLOT(UTextBlock, TB_MyAssultNoticeR);
	GET_SLOT(UTextBlock, TB_MyAssultNoticeB);
	GET_SLOT(UTextBlock, TB_ResultNotice);
	GET_SLOT(UB2RichTextBlock, RTB_JoinNotice);

	GET_SLOT(UOverlay, O_Inside_Conlict);
	GET_SLOT(UOverlay, O_Inside_B);
	GET_SLOT(UOverlay, O_Inside_R);

	GET_SLOT(UOverlay, O_Msg_Time);

	GET_SLOT(UB2Button, BTN_Chat);
	GET_SLOT(UB2UIControlChatPanel, UI_Control_Chat);

	GET_SLOT(UB2Image, IMG_ChatDis);
	GET_SLOT(UB2Image, IMG_ChatCoolTime);	

	GET_SLOT(UTextBlock, TB_MinoNotice);
	GET_SLOT(UTextBlock, TB_SancNotice);
	GET_SLOT(UTextBlock, T_R);
	GET_SLOT(UTextBlock, T_B);
	GET_SLOT(UTextBlock, TB_MyAssultNoticeR);
	GET_SLOT(UTextBlock, TB_MyAssultNoticeB);

	GET_SLOT(UTextBlock, TB_SubmersionTitle);
	GET_SLOT(UTextBlock, TB_SubmersionDesc);
	GET_SLOT(UTextBlock, TB_SubmersionTime);
	GET_SLOT(UPanelWidget, O_Msg_Submersion);

	// 채팅쿨타임이미지 보이는걸로 초기화
	SetChatImageCoolTimeProgress(1.0f);

	// 채팅 패널 초기화
	InitControlChatPanel();
	
	// 점령상태 게이지 초기화
	SetProgressGage(0.0f);
}

void UB2UIControlBattleSub::SetLeaveEnemyText(ESlateVisibility InVisible)
{
	if (TB_LeaveEnemy.IsValid())
	{
		TB_LeaveEnemy->SetVisibility(InVisible);
	}
}

void UB2UIControlBattleSub::SetControlStateImage(UMaterialInterface* pMI)
{
	if (pMI && IMG_Stat.IsValid())
	{
		IMG_Stat->SetBrushFromMaterial(pMI);
	}
}

void UB2UIControlBattleSub::SetProgressImage(UMaterialInterface* pMI)
{
	if (pMI && IMG_Prograss.IsValid())
	{
		IMG_Prograss->SetBrushFromMaterial(pMI);
	}
}

void UB2UIControlBattleSub::SetProgressBackImage(UMaterialInterface* pMI)
{
	if (pMI && IMG_PrograssBG.IsValid())
	{
		IMG_PrograssBG->SetBrushFromMaterial(pMI);

		UMaterialInstanceDynamic* DynamicMI = IMG_PrograssBG->GetDynamicMaterial();

		// 배경은 항시 꽉차있다.
		if (DynamicMI)
			DynamicMI->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, 1.0f);
	}
}

void UB2UIControlBattleSub::SetProgressGage(float fProgress)
{
	if (IMG_Prograss.IsValid())
	{
		UMaterialInstanceDynamic* DynamicMI = IMG_Prograss->GetDynamicMaterial();

		if (DynamicMI)
			DynamicMI->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, fProgress);
	}
}

void UB2UIControlBattleSub::InitPlayerInfos()
{
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));

	if (pGM)
	{
		TArray<ABladeIINetPlayer*> arPlayers = pGM->GetPlayerArray();

		for (ABladeIINetPlayer* pPlayer : arPlayers)
			AddTeamPlayerInfoUI(pPlayer);
	}
}

void UB2UIControlBattleSub::SetTeamKillCount(int32 RedKillCount, int32 BlueKillCount)
{
	if (TB_KillCountRed.IsValid())		
		TB_KillCountRed->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlTeamKillPoint")), FText::AsNumber(RedKillCount)));

	if (TB_KillCountBlue.IsValid())		
		TB_KillCountBlue->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlTeamKillPoint")), FText::AsNumber(BlueKillCount)));
}

void UB2UIControlBattleSub::UpdateMyAssultNotice()
{
	if (!O_MyAssultNotice.IsValid() || !TB_MyAssultNoticeR.IsValid() || !TB_MyAssultNoticeB.IsValid())
		return;
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));

	O_MyAssultNotice->SetVisibility(ESlateVisibility::Hidden);
	TB_MyAssultNoticeR->SetVisibility(ESlateVisibility::Hidden);
	TB_MyAssultNoticeB->SetVisibility(ESlateVisibility::Hidden);
	
	if (pGM && pGM->IsMyAssaultState())
	{
		ETeamType myTeamType = ETeamType(Cast<ABladeIICharacter>(pGM->GetLocalController()->GetPawn())->GetTeamNum());
		
		// 이미 우리편이 점령했을땐 안뜬다.
		if (
			(myTeamType == ETeamType::ETT_Red && m_ControlObjectState == CONTROL_STATE_RED)
			|| (myTeamType == ETeamType::ETT_Blue && m_ControlObjectState == CONTROL_STATE_BLUE)
			)
			return;

		switch (myTeamType)
		{
		case ETeamType::ETT_Red:
			O_MyAssultNotice->SetVisibility(ESlateVisibility::Visible);
			TB_MyAssultNoticeR->SetVisibility(ESlateVisibility::Visible);
			break;
		case ETeamType::ETT_Blue:
			O_MyAssultNotice->SetVisibility(ESlateVisibility::Visible);
			TB_MyAssultNoticeB->SetVisibility(ESlateVisibility::Visible);
			break;
		}		
	}
}

// 격돌중인지 그냥 점령상태인지
void UB2UIControlBattleSub::UpdateAssultState()
{
	if (!O_Inside_Conlict.IsValid() || !O_Inside_B.IsValid() || !O_Inside_R.IsValid())
		return;

	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));

	O_Inside_Conlict->SetVisibility(ESlateVisibility::Hidden);
	O_Inside_B->SetVisibility(ESlateVisibility::Hidden);
	O_Inside_R->SetVisibility(ESlateVisibility::Hidden);

	// 격돌중
	if (pGM && pGM->IsAssaultState())
	{
		O_Inside_Conlict->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if (m_ControlObjectState == CONTROL_STATE_RED)
		{
			O_Inside_R->SetVisibility(ESlateVisibility::Visible);
		}

		if (m_ControlObjectState == CONTROL_STATE_BLUE)
		{
			O_Inside_B->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UB2UIControlBattleSub::UpdateControlChatCoolTime()
{
	if (ControlChatSendTimeSec != -1.0f)
	{
		float ElapseTime = GetWorld()->GetTimeSeconds() - ControlChatSendTimeSec;

		// 버튼 이미지 프로그래스 설정.
		float ProgressRate = ElapseTime / ControlChatCoolTime;
		SetChatImageCoolTimeProgress(ProgressRate);

		// 쿨 다돌았을때
		if (ElapseTime >= ControlChatCoolTime)
		{
			ControlChatSendTimeSec = -1.0f;
			// 버튼 이미지 프로그래스 설정. 1로 설정.
			SetChatImageCoolTimeProgress(1.0f);

			if (BTN_Chat.IsValid())
			{
				BTN_Chat->SetIsEnabled(true);
			}
		}
	}
}

void UB2UIControlBattleSub::SubscribeEvent()
{
	//CAPTURE_UOBJECT(UB2UIControlBattleSub);

	//NotifyInputTouchBeganInViewPortTicket = NotifyInputTouchBeganInViewPortClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(NotifyInputTouchBeganInViewPort)
	//	Capture->CloseChatPanel();
	//END_CAPTURE_OBJECT()
	//	);

}

void UB2UIControlBattleSub::UnsubscribeEvent()
{
	//NotifyInputTouchBeganInViewPortClass<>::GetInstance().Unsubscribe(NotifyInputTouchBeganInViewPortTicket);
}

void UB2UIControlBattleSub::OnClickBtnChat()
{
	if (UI_Control_Chat.IsValid())
	{
		if (UI_Control_Chat->GetVisibility() == ESlateVisibility::Hidden)
		{
			UI_Control_Chat->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UI_Control_Chat->SetChatTab(EControlChatType::Emoticon);

			// 버튼셋은 숨김
			SetVisibilityChatButton(ESlateVisibility::Hidden);
		}
	}
		
}

void UB2UIControlBattleSub::InitControlChatPanel()
{
	if (!UI_Control_Chat.IsValid())
		return;

	UI_Control_Chat->Init();

	// 메세지 항목 채움
	for (TPair<int32, FControlChatMsgInfo> ChatMsgInfo : MessageInfos)
	{
		UMaterialInterface* pMI = ChatMsgInfo.Value.MessageIconMaterial.LoadSynchronous();
		UI_Control_Chat->AddControlChatPanelMessageItem(EControlChatType::Message, ChatMsgInfo.Key, ChatMsgInfo.Value.MessageLocTextKey, pMI);
	}

	// 이모티콘 항목 채움
	for (TPair<int32, FControlChatMsgInfo> ChatMsgInfo : EmoticonInfos)
	{
		UMaterialInterface* pMI = ChatMsgInfo.Value.MessageIconMaterial.LoadSynchronous();
		UI_Control_Chat->AddControlChatPanelMessageItem(EControlChatType::Emoticon, ChatMsgInfo.Key, ChatMsgInfo.Value.MessageLocTextKey, pMI);
	}
}

void UB2UIControlBattleSub::SetChatImageCoolTimeProgress(float fProgress)
{
	if (IMG_ChatCoolTime.IsValid())
	{
		UMaterialInstanceDynamic* DynamicMI = IMG_ChatCoolTime->GetDynamicMaterial();

		if (DynamicMI)
			DynamicMI->SetScalarParameterValue(MtrlParamName_CoolTimeRingProgress, fProgress);
	}
}

class UTexture2D* UB2UIControlBattleSub::GetSerialKillTexture(int32 nKillCount)
{
	int32 nMaxKillCount = 0;
	UTexture2D* pMaxKillCountTexture = NULL;

	for (FControlSerialKillTextureInfo& KillTextureInfo : ControlSerialKillTextureInfos)
	{
		if (KillTextureInfo.KillCount == nKillCount)
		{
			UTexture2D* pTexture = KillTextureInfo.KillTexture.LoadSynchronous();

			if (pTexture)
				return pTexture;
		}

		// 이터레이팅한거중에 킬젤높은거 저장.
		if (nMaxKillCount < KillTextureInfo.KillCount)
		{
			nMaxKillCount = KillTextureInfo.KillCount;
			pMaxKillCountTexture = KillTextureInfo.KillTexture.LoadSynchronous();
		}
	}

	return pMaxKillCountTexture;
}

void UB2UIControlBattleSub::SetVisibilityChatButton(ESlateVisibility NewVisibility)
{
	if (IMG_ChatDis.IsValid())
		IMG_ChatDis->SetVisibility(NewVisibility);

	if (IMG_ChatCoolTime.IsValid())
		IMG_ChatCoolTime->SetVisibility(NewVisibility);

	if (BTN_Chat.IsValid())
		BTN_Chat->SetVisibility(NewVisibility);
}

class UOverlay* UB2UIControlBattleSub::GetPlayerInfoOverlay(ETeamType TeamType)
{
	int32 nCurTeamCount = 0;

	for (UB2UIControlTeamPlayerInfo* pCheckUI : m_arTeamPlayerInfoUI)
	{
		if (pCheckUI)
		{
			ABladeIIPlayer* pPlayer = pCheckUI->GetTargetPlayer();

			if (pPlayer && ETeamType(pPlayer->GetTeamNum()) == TeamType)
				nCurTeamCount++;
		}
	}

	if (TeamType == ETeamType::ETT_Red && arRedPlayerOverlays.Num() > nCurTeamCount)
		return arRedPlayerOverlays[nCurTeamCount];

	if (TeamType == ETeamType::ETT_Blue && arBluePlayerOverlays.Num() > nCurTeamCount)
		return arBluePlayerOverlays[nCurTeamCount];

	return nullptr;
}

void UB2UIControlBattleSub::AddTeamPlayerInfoUI(ABladeIINetPlayer* pPlayer)
{
	if (!pPlayer)
		return;

	for (UB2UIControlTeamPlayerInfo* pCheckUI : m_arTeamPlayerInfoUI)
	{
		if (pCheckUI->GetTargetPlayerNetId() == pPlayer->GetNetId())
			return;
	}

	UOverlay* pTargetOverlay = GetPlayerInfoOverlay(ETeamType(pPlayer->GetTeamNum()));
	
	if (pTargetOverlay)
	{
		UB2UIControlTeamPlayerInfo* pUI = CreateWidget<UB2UIControlTeamPlayerInfo>(GetWorld(), PlayerInfoClass);
		if (pUI)
		{
			pTargetOverlay->AddChild(Cast<UWidget>(pUI));
			pUI->Init();
			pUI->SetPlayer(pPlayer);

			// 일반 상태 애니메이션 재생
			pUI->PlayAnimationEvent_NormalState();

			if (pPlayer->IsLocalPlayer())
			{
				pUI->PlayAnimationEvent_LocalPlayerState();
			}

			m_arTeamPlayerInfoUI.Add(pUI);
		}
	}
}

void UB2UIControlBattleSub::UpdateKillInfo(class ABladeIINetPlayer* pKillerPlayer, class ABladeIINetPlayer* pDeadPlayer)
{
	// 우리편꺼 UI에 킬 업데이트
	//SetUserKillCountToPlayerInfoUI(pKillerPlayer->GetNetId(), pKillerPlayer->GetKillCount());
			
	// 우측에 킬알림 추가
	if (VB_Kill_Infos.IsValid())
	{
		UB2UIControlKillInfo* pUI = CreateWidget<UB2UIControlKillInfo>(GetWorld(), KillInfoClass);
		if (pUI)
		{
			VB_Kill_Infos->AddChild(Cast<UWidget>(pUI));
			pUI->Init();

			FText KillerNickName = FText::FromString(pKillerPlayer->GetCharacterDataStore()->GetUserNickName());
			FText DeadManNickName = FText::FromString(pDeadPlayer->GetCharacterDataStore()->GetUserNickName());
			pUI->SetKillInfo(ETeamType(pKillerPlayer->GetTeamNum()), KillerNickName, DeadManNickName, pKillerPlayer->GetCharacterDataStore()->GetMainPlayerClass(), pDeadPlayer->GetCharacterDataStore()->GetMainPlayerClass());
		}
	}
}

void UB2UIControlBattleSub::SetUserKillInfos(TMap<uint32, int32> KillInfos)
{
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));

	for (TPair<uint32, int32>& KillInfo : KillInfos)
	{
		uint32 UserNetId = KillInfo.Key;
		int32 KillCount = KillInfo.Value;

		SetUserKillCountToPlayerInfoUI(UserNetId, KillCount);
	}
}

// UI에 킬카운트 입력. 난입자는 여기통해서 초기화
void UB2UIControlBattleSub::SetUserKillCountToPlayerInfoUI(uint32 NetId, int32 KillCount)
{
	for (UB2UIControlTeamPlayerInfo* pUI : m_arTeamPlayerInfoUI)
	{
		if (pUI && pUI->GetTargetPlayer() && pUI->GetTargetPlayer()->GetNetId() == NetId)
		{
			pUI->SetKillCount(KillCount);
		}
	}
}

void UB2UIControlBattleSub::SetSerialKillCount(int32 nKillCount)
{
	if (IMG_SerialKillCount.IsValid())
	{
		UTexture2D* pTexture = GetSerialKillTexture(nKillCount);
		
		if(pTexture)
			IMG_SerialKillCount->SetBrushFromTexture(pTexture);

		PlayAnimationEvent_AnimSingleKillNotice(nKillCount);
	}		
}

void UB2UIControlBattleSub::ViewPlayerJoinUI(class ABladeIINetPlayer* pPlayer)
{
	if (pPlayer && RTB_JoinNotice.IsValid())
	{
		FString strLocTextKey;

		if (ETeamType(pPlayer->GetTeamNum()) == ETeamType::ETT_Red)
		{
			strLocTextKey = TEXT("ControlJoinUserNoticeRed");
		}

		if (ETeamType(pPlayer->GetTeamNum()) == ETeamType::ETT_Blue)
		{
			strLocTextKey = TEXT("ControlJoinUserNoticeBlue");
		}

		RTB_JoinNotice->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, strLocTextKey), FText::FromString(pPlayer->GetCharacterDataStore()->GetUserNickName())));

		PlayAnimationEvent_AnimJoinNotice();
	}
}

void UB2UIControlBattleSub::ViewControlGameNotice(bool bSuccess)
{
	if (TB_ControlGameNotice.IsValid())
	{
		FText txtControlGameNotice = bSuccess ? BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlSuccessConquest")) : BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("ControlFailConquest"));
		TB_ControlGameNotice->SetText(txtControlGameNotice);
	}

	PlayAnimationEvent_AnimControlGameNotice(bSuccess);
}

void UB2UIControlBattleSub::PlayResultAction(ETeamType WinTeam, ETeamType MyTeam)
{
	if (WinTeam == ETeamType::ETT_End)
	{
		// 무승부
		//TB_ResultNotice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("MatchResult_Draw")));
	}
	else if (WinTeam == MyTeam)
	{
		// 승리
		//TB_ResultNotice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("MatchResult_Win")));
		PlayAnimationEvent_AnimResultAction(true);
	}
	else
	{
		// 패배
		//TB_ResultNotice->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("MatchResult_Lose")));
		PlayAnimationEvent_AnimResultAction(false);
	}

	
}

void UB2UIControlBattleSub::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateMyAssultNotice(); 
	UpdateAssultState();
	UpdateControlChatCoolTime();
}

void UB2UIControlBattleSub::SetHiddneTimerUI(bool bHiddne)
{
	// 에디터 초기값은 ESlateVisibility::Hidden으로 설정

	// 보이는 상태면 숨김
	if (bHiddne && O_Msg_Time->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		O_Msg_Time->SetVisibility(ESlateVisibility::Hidden);
	}

	// 안보이는 상태면 숨김해제
	if (!bHiddne && O_Msg_Time->GetVisibility() == ESlateVisibility::Hidden)
	{
		O_Msg_Time->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UB2UIControlBattleSub::UpdateTimerUI(FText& TextContent)
{
	// 경과 시간 텍스트 셋팅
	if (TB_RemainGameTime.IsValid())
	{
		TB_RemainGameTime->SetText(TextContent);
	}
}

void UB2UIControlBattleSub::CloseChatPanel()
{
	if (UI_Control_Chat.IsValid())
	{
		if (UI_Control_Chat->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
		{
			UI_Control_Chat->SetVisibility(ESlateVisibility::Hidden);

			SetVisibilityChatButton(ESlateVisibility::Visible);
		}
	}
}

void UB2UIControlBattleSub::NotifySendControlChat()
{
	CloseChatPanel();

	if (BTN_Chat.IsValid())
	{
		BTN_Chat->SetIsEnabled(false);

		// 보낸 시간 저장.
		ControlChatSendTimeSec = GetWorld()->GetTimeSeconds();
	}
}

void UB2UIControlBattleSub::NotifyReceiveControlChat(uint32 NetId, EControlChatType ChatType, int32 MessageIndex)
{
	FText ContentText;
	UMaterialInterface* pMI;

	switch (ChatType)
	{
	case EControlChatType::Message:
		if (!MessageInfos.Contains(MessageIndex))
			return;

		ContentText = BladeIIGetLOCText(B2LOC_CAT_CONTROLCHAT, MessageInfos[MessageIndex].MessageLocTextKey);
		pMI = MessageInfos[MessageIndex].MessageIconMaterial.LoadSynchronous();
		break;
	case EControlChatType::Emoticon:
		if (!EmoticonInfos.Contains(MessageIndex))
			return;

		ContentText = BladeIIGetLOCText(B2LOC_CAT_CONTROLCHAT, EmoticonInfos[MessageIndex].MessageLocTextKey);
		pMI = EmoticonInfos[MessageIndex].MessageIconMaterial.LoadSynchronous();
		break;
	default:
		return;
		break;
	}

	// 대상플레이어 찾아옴
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(UGameplayStatics::GetGameMode(this));
	ABladeIIPlayer* pPlayer = NULL;

	if (pGM)
		pPlayer = Cast<ABladeIIPlayer>(pGM->FindPawnByNetId(NetId));

	if (!pPlayer)
		return;

	// 메세지형태 채팅은 아군꺼만
	if (ChatType == EControlChatType::Message && !pPlayer->GetIsLocalPlayerTeam())
	{
		return;
	}

	CreateChatMessageWidget(ChatType, ContentText, pMI, pPlayer);
}

void UB2UIControlBattleSub::SetPlayerResurrectTime(uint32 NetId, float RemainTime)
{
	for (UB2UIControlTeamPlayerInfo* pCheckUI : m_arTeamPlayerInfoUI)
	{
		if (pCheckUI && pCheckUI->GetTargetPlayerNetId() == NetId)
		{
			pCheckUI->SetRemainResurrectTime(RemainTime);
		}
	}
}

void UB2UIControlBattleSub::SetMsgSubmersion(bool InValue)
{
	if (O_Msg_Submersion.IsValid())
	{
		if (InValue)
		{
			O_Msg_Submersion->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			O_Msg_Submersion->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UB2UIControlBattleSub::SetMsgSubmersionTime(int32 InTime)
{
	if (TB_SubmersionTime.IsValid())
	{
		TB_SubmersionTime->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Control_FirstWarningCount")), FText::AsNumber(InTime)));
	}
}

