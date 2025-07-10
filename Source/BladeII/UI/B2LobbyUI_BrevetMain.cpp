// Fill out your copyright notice in the Description page of Project Settings.
#include "B2LobbyUI_BrevetMain.h"

#include "B2LobbyUI_HeroMgmtCommon.h"
#include "B2LobbyGameMode.h"
#include "Retailer.h"
#include "B2UIBrevetAddStatus.h"
#include "B2UIBrevetRankUpSuccess.h"
#include "B2LobbyUI_BrevetNodeIcon.h"
#include "B2UIMsgPopupUseResources.h"
#include "B2LobbyUI_BrevetNodeIconDummy.h"
#include "B2ItemInfo.h"
#include "B2UIDocHelper.h"
#include "B2UIDocUser.h"

#include "B2UIShortagePopupManager.h"
#include "B2UIUpgradeStatNotify.h"
#include "B2AndroidBackManager.h"
#include "Global.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2BrevetInfo.h"

UB2LobbyUI_BrevetMain::UB2LobbyUI_BrevetMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentPCClass = EPCClass::EPC_End;
	//CachedLobbyGM = nullptr;
	//m_pCurrentBrevetNode = NULL;
	CurGaugeAnimationQueue.Empty();
	m_fStartTime = -1;
	m_fGaugePercentOld = -1;
	m_fDestGaugePercent = 0.0f;
	m_fStartBaseSpeed = 0.0f;
	m_fTotalTime = 0.0f;
	m_bIsAutoUpgradeState = false;
	m_bIsUpgrading = false;
	m_nOldRank = 0;
	m_nOldNodeIndex = 1;
}

void UB2LobbyUI_BrevetMain::StartFromLobby(class UB2UIManager_Lobby* InUIManager, class AB2LobbyGameMode* InGM)
{
	//Super::StartFromLobby(InUIManager, InGM);

	//CAPTURE_UOBJECT(UB2LobbyUI_BrevetMain);
	//DeliveryEnhanceRankNodeTicket = DeliveryEnhanceRankNodeClass<FB2EnhanceRankNode>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(DeliveryEnhanceRankNode, const FB2EnhanceRankNode& EnhanceRankNode)
	//		Capture->OnDeliveryEnhanceRankNode(EnhanceRankNode);
	//	END_CAPTURE_OBJECT()
	//);

	//LobbySetHeroMgmtModePCSelectionTicket = LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(LobbySetHeroMgmtModePCSelection, EPCClass PCClass)
	//	Capture->OnHeroMgmtPCChanged(PCClass);
	//END_CAPTURE_OBJECT()
	//	);

	//if (HeroMgmtBaseNRef)
	//{
	//	HeroMgmtBaseNRef->StartFromLobby(InUIManager, InGM); // 이건 LobbyUISwitcher 에서 모르니 직접 해 주어야..		
	//}

	//SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyHeroMgmt_BrevetButton")));

	//m_bIsAutoUpgradeState = false;
	//m_bIsUpgrading = false;

	//B2AndroidBackManager::GetInstance()->AddBackWidget(this);

	UpdateStaticText();
}

void UB2LobbyUI_BrevetMain::DestroySelf()
{
	if (GetWorld())
		GetWorld()->GetTimerManager().ClearTimer(DelayAutoUpgradeTimerHandle);

	//DeliveryEnhanceRankNodeClass<FB2EnhanceRankNode>::GetInstance().Unsubscribe(DeliveryEnhanceRankNodeTicket);
	//LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Unsubscribe(LobbySetHeroMgmtModePCSelectionTicket);

	//if (HeroMgmtBaseNRef)
	//	HeroMgmtBaseNRef->DestroySelf(); 

	//B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);

	//Super::DestroySelf();
}

void UB2LobbyUI_BrevetMain::BeginDestroy()
{
	////Editor 상황에서 문제가 좀 있어서 여기서도 unsubscribe
	//if (CachedLobbyGM)
	//{
	//	DeliveryEnhanceRankNodeClass<FB2EnhanceRankNode>::GetInstance().Unsubscribe(DeliveryEnhanceRankNodeTicket);
	//	LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Unsubscribe(LobbySetHeroMgmtModePCSelectionTicket);
	//}

	//if (GetWorld())
	//	GetWorld()->GetTimerManager().ClearTimer(DelayAutoUpgradeTimerHandle);

	Super::BeginDestroy();
}

void UB2LobbyUI_BrevetMain::UpdateStaticText()
{

	if (TB_TotalAbil.IsValid())
		TB_TotalAbil->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Brevet_TotalAbilities")));

	if (TB_ExAtk.IsValid())
		TB_ExAtk->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Brevet_Abilities1")));

	if (TB_ExDef.IsValid())
		TB_ExDef->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Brevet_Abilities2")));

	if (TB_ExHp.IsValid())
		TB_ExHp->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Brevet_Abilities3")));

	if (TB_ProgressGrade.IsValid())
		TB_ProgressGrade->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Brevet_ProgressGrade")));

	if (TB_Cost.IsValid())
		TB_Cost->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Brevet_Cost")));

	if (TB_Automatic.IsValid())
		TB_Automatic->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Brevet_Automatic")));

	if (TB_BtnUpgrade.IsValid())
		TB_BtnUpgrade->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Brevet_Currentprogress")));

}

void UB2LobbyUI_BrevetMain::OnHeroMgmtPCChanged(EPCClass InSelectedPCClass)
{
	StopAutoUpgrade();

	if (CurrentPCClass != InSelectedPCClass)
	{
		CurrentPCClass = InSelectedPCClass;

		RefreshBrevetUIScrollBoxInfo();
		RefreshBrevetUIInfo(true);
	}
}

void UB2LobbyUI_BrevetMain::RefreshBrevetUIInfo(bool bReset)
{
	//int32 CurrentCharBrevetRank = GetCurrentBrevetRank();
	//int32 CurrentBrevetNodeIndex = GetCurrentBrevetNodeIndex();

	//int32 AppliedBrevetRank = GetAppliedBrevetRank();
	//int32 AppliedBrevetNodeIndex = GetAppliedBrevetNodeIndex();

	//FString RankNameKey = BladeIIGameImpl::GetClientDataStore().GetBrevetRankNameKey(CurrentCharBrevetRank);
	//TB_RankName->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), RankNameKey));

	//UMaterialInterface* pMI = BladeIIGameImpl::GetClientDataStore().GetBrevetRankIconMaterial(CurrentCharBrevetRank, true);
	//IMG_RankIcon->SetBrushFromMaterial(pMI);

	//UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
	//int32 CurrentBrevetGoodsID = BladeIIGameImpl::GetClientDataStore().GetBrevetGoodsID(CurrentCharBrevetRank);
	//FSingleItemInfoData* WantedBrevetGoodsItemInfo = ItemInfoTable ? ItemInfoTable->GetInfoData(CurrentBrevetGoodsID) : NULL;
	//if (WantedBrevetGoodsItemInfo)
	//{
	//	if (IMG_BrevetGoods.IsValid())
	//	{
	//		IMG_BrevetGoods->SetBrushFromMaterial(WantedBrevetGoodsItemInfo->GetIconMaterial(ItemInfoTable));
	//	}

	//	if (BTN_BrevetGoods.IsValid())
	//	{
	//		BTN_BrevetGoods->SetItemInfo(CurrentBrevetGoodsID);
	//	}
	//}
	//
	//// atk def hp
	//int32 TotalAtk = BladeIIGameImpl::GetClientDataStore().GetBrevetNodeTotalAtk(AppliedBrevetRank, AppliedBrevetNodeIndex);
	//int32 TotalDef = BladeIIGameImpl::GetClientDataStore().GetBrevetNodeTotalDef(AppliedBrevetRank, AppliedBrevetNodeIndex);
	//int32 TotalHp = BladeIIGameImpl::GetClientDataStore().GetBrevetNodeTotalHp(AppliedBrevetRank, AppliedBrevetNodeIndex);
	//TB_BrevetAtk->SetText(FText::AsNumber(TotalAtk));
	//TB_BrevetDef->SetText(FText::AsNumber(TotalDef));
	//TB_BrevetHp->SetText(FText::AsNumber(TotalHp));

	//m_fStartTime = -1;
	//SetCurrentNodeProgress(0);
	//UB2UIManager::GetInstance()->CloseUI(UIFName::BlockScreen);

	//if (const FBrevetNodeInfo* BrevetNodeData = GetCurrentBrevetNodeInfo())
	//{
	//	if (TB_HonerPoint.IsValid())
	//	{
	//		bool checkNormal = (CurrentCharBrevetRank <= BladeIIGameImpl::GetClientDataStore().GetMaximumNormalBrevetGade());

	//		if (checkNormal)
	//		{
	//			TB_HonerPoint->SetText(FText::AsNumber(BrevetNodeData->NeedRankPromotionPoint));

	//			if (BrevetNodeData->NeedRankPromotionPoint <= BladeIIGameImpl::GetClientDataStore().GetRankPromotionPoint())
	//				TB_HonerPoint->SetColorAndOpacity(FLinearColor::White);
	//			else
	//				TB_HonerPoint->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
	//		}
	//		else
	//		{
	//			TB_HonerPoint->SetText(FText::AsNumber(BrevetNodeData->NeedRankPromotionPoint));

	//			if (BrevetNodeData->NeedRankPromotionPoint <= BladeIIGameImpl::GetClientDataStore().GetAdvancedRankPromotionPoint())
	//				TB_HonerPoint->SetColorAndOpacity(FLinearColor::White);
	//			else
	//				TB_HonerPoint->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
	//		}
	//	}

	//	if (TB_GoldPoint.IsValid())
	//	{
	//		TB_GoldPoint->SetText(FText::AsNumber(BrevetNodeData->NeedActiveGold));

	//		if(BrevetNodeData->NeedActiveGold <= BladeIIGameImpl::GetClientDataStore().GetGoldAmount())
	//			TB_GoldPoint->SetColorAndOpacity(FLinearColor::White);
	//		else
	//			TB_GoldPoint->SetColorAndOpacity(UB2UIManager::GetInstance()->TextColor_Decrease);
	//	}
	//}

	//if (bReset)
	//{
	//	// init
	//	SB_BrevetNode->SetAnimateScroll(false);
	//	SB_BrevetNode->SetAdjustFromSlotIndex(CurrentBrevetNodeIndex);

	//	if (m_bIsAutoUpgradeState == false)
	//	{
	//		BlockInputBrevetMain(false);
	//		SetClose(true);
	//	}
	//}

	//UpdateVisibilityAutoUpgradeBtn();

	//if (HeroMgmtBaseNRef)
	//	HeroMgmtBaseNRef->DoMarkRedDot();
}

void UB2LobbyUI_BrevetMain::RefreshBrevetUIScrollBoxInfo()
{
	//if (SB_BrevetNode.IsValid() && NodeItemClass)
	//{
	//	for (auto* BrevetNode : m_arBrevetNodes)
	//	{
	//		if (BrevetNode)
	//			BrevetNode->DestroySelf();
	//	}

	//	int32 CurrentCharBrevetRank = GetCurrentBrevetRank();
	//	int32 BrevetNodeIndex = GetCurrentBrevetNodeIndex();
	//	bool IsCompleteBrevet = IsCompletedCurrentBrevetNode();
	//	int32 BrevetNodeMaxCount = BladeIIGameImpl::GetClientDataStore().GetBrevetNodeCount(CurrentCharBrevetRank);

	//	m_arBrevetNodes.Empty();		
	//	// 현재 계급의 랭크 노드들
	//	for (int32 nCnt = 0; nCnt < BrevetNodeMaxCount; ++nCnt)
	//	{
	//		//아이콘 셋팅, 계급, 랭크노드번호
	//		const FBrevetNodeInfo* BrevetNodeInfo = BladeIIGameImpl::GetClientDataStore().GetBrevetNodeInfo(CurrentCharBrevetRank == 0 ? 1 : CurrentCharBrevetRank, nCnt + 1);
	//		if (!BrevetNodeInfo)
	//			continue;

	//		// 왼쪽 끝 더미
	//		if (nCnt == 0)
	//		{
	//			UB2LobbyUserWidget* DummyLeft = CreateWidget<UB2LobbyUserWidget>(GetOwningPlayer(), NodeItemClass_DummyL);
	//			SB_BrevetNode->AddChild(DummyLeft);
	//			m_arBrevetNodes.Add(DummyLeft);

	//			// 조건 안되면 텍스트 설정(첫번째꺼만 검사하면됨)
	//			if (BrevetNodeInfo->OpenLevel > BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(CurrentPCClass))
	//			{
	//				UB2LobbyUI_BrevetNodeIconDummy* pUI = Cast<UB2LobbyUI_BrevetNodeIconDummy>(DummyLeft);

	//				if (pUI)
	//					pUI->SetUnLockConditionText(BrevetNodeInfo->OpenLevel);
	//			}
	//		}

	//		UB2LobbyUI_BrevetNodeIcon* NewBrevetNodeIcon;
	//		NewBrevetNodeIcon = CreateWidget<UB2LobbyUI_BrevetNodeIcon>(GetOwningPlayer(), NodeItemClass);
	//		SB_BrevetNode->AddChild(NewBrevetNodeIcon);

	//		// 노드번호 설정
	//		NewBrevetNodeIcon->SetBreverNodeNum(nCnt + 1);

	//		// 노드 상태
	//		EBrevetNodeState BrevetNodeState = EBrevetNodeState::EBNS_None;

	//		if (IsCompleteBrevet)
	//		{
	//			BrevetNodeState = EBrevetNodeState::EBNS_Complete;
	//		}
	//		else
	//		{
	//			if (nCnt < BrevetNodeIndex - 1)
	//			{
	//				BrevetNodeState = EBrevetNodeState::EBNS_Complete;
	//			}
	//			else if (nCnt == BrevetNodeIndex - 1)
	//			{
	//				if (BrevetNodeInfo)
	//				{
	//					if (BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(CurrentPCClass) < BrevetNodeInfo->OpenLevel)
	//						BrevetNodeState = EBrevetNodeState::EBNS_Locked_Current;
	//					else
	//						BrevetNodeState = EBrevetNodeState::EBNS_UnLocked;
	//				}
	//			}
	//			else if (nCnt > BrevetNodeIndex - 1)
	//			{
	//				BrevetNodeState = EBrevetNodeState::EBNS_Locked;
	//			}
	//		}

	//		NewBrevetNodeIcon->InitBrevetNodeIcon(this, BrevetNodeInfo, BrevetNodeState);

	//		// 앞노드 활성화 안내 문구
	//		if (nCnt != 0)
	//		{
	//			UB2LobbyUI_BrevetNodeIcon* pUI = Cast<UB2LobbyUI_BrevetNodeIcon>(m_arBrevetNodes[m_arBrevetNodes.Num() - 1]);

	//			// 첫번째랑 끝에 더미노드가 들어가니 1빼줌
	//			if (pUI && pUI->GetBrevetNodeState() == EBrevetNodeState::EBNS_UnLocked)
	//				NewBrevetNodeIcon->SetNotOpenText(m_arBrevetNodes.Num() - 1);
	//		}

	//		// 아이콘 머터리얼 세팅
	//		SetNodeIconFromStatusType(NewBrevetNodeIcon, EBrevetStatusType(BrevetNodeInfo->StatusType));

	//		// 게이지 초기화
	//		NewBrevetNodeIcon->SetProgressGauge(0);
	//		
	//		m_arBrevetNodes.Add(Cast<UB2LobbyUserWidget>(NewBrevetNodeIcon));

	//		// 오른쪽 끝 더미
	//		if (nCnt == BrevetNodeMaxCount - 1)
	//		{
	//			UB2LobbyUserWidget* DummyRight = CreateWidget<UB2LobbyUserWidget>(GetOwningPlayer(), NodeItemClass_DummyR);
	//			SB_BrevetNode->AddChild(DummyRight);
	//			m_arBrevetNodes.Add(DummyRight);

	//			if (StaticFindBrevetInfo() && CurrentCharBrevetRank == StaticFindBrevetInfo()->GetDataCount())
	//			{
	//				if (auto* EndDummy = Cast<UB2LobbyUI_BrevetNodeIconDummy>(DummyRight))
	//				{
	//					EndDummy->SetEndRanking();
	//				}
	//			}
	//			else
	//			{
	//				// 다음 계급 아이콘 넣어주기
	//				if (UB2LobbyUI_BrevetNodeIconDummy* pUI = Cast<UB2LobbyUI_BrevetNodeIconDummy>(DummyRight))
	//				{
	//					UMaterialInterface* pMI = BladeIIGameImpl::GetClientDataStore().GetBrevetRankIconMaterial(CurrentCharBrevetRank + 1, true);
	//					FText txtCurRank = BladeIIGetLOCText(B2LOC_CAT_GENERAL, BladeIIGameImpl::GetClientDataStore().GetBrevetRankNameKey(CurrentCharBrevetRank));

	//					pUI->SetNextRankInfo(pMI, txtCurRank);
	//				}
	//			}
	//		}
	//	}
	//}

	////현재노드 설정
	//SetCurrentNode();
	//SetCurrentNodeProgress(0.0f);
	//
	//m_bIsUpgrading = false;
}

bool UB2LobbyUI_BrevetMain::CheckNeedNodeOpenAnimation()
{
	bool bIsOpenAnimPlayed = false;

	if (!GConfig->GetBool(TEXT("BrevetNodeAnim"), *FString::FromInt((int32)CurrentPCClass), bIsOpenAnimPlayed, GB2UserSavedStateIni))
		bIsOpenAnimPlayed = false;

	if (!bIsOpenAnimPlayed)
	{
		// true 리턴할떄 호출
		GConfig->SetBool(TEXT("BrevetNodeAnim"), *FString::FromInt((int32)CurrentPCClass), true, GB2UserSavedStateIni);

		return true;
	}

	return false;
}

int32 UB2LobbyUI_BrevetMain::GetCurrentBrevetRank()
{
	int32 BrevetRank = BladeIIGameImpl::GetLocalCharacterData().GetCharBrevetRank(CurrentPCClass);
	return (BrevetRank != 0) ? BrevetRank : 1;
}

int32 UB2LobbyUI_BrevetMain::GetCurrentBrevetNodeIndex()
{
	int32 nNodeIndex = BladeIIGameImpl::GetLocalCharacterData().GetCharBrevetNodeIndex(CurrentPCClass);
	return (nNodeIndex != 0) ? nNodeIndex : 1;
}

int32 UB2LobbyUI_BrevetMain::GetAppliedBrevetRank()
{
	return BladeIIGameImpl::GetLocalCharacterData().GetAppliedCharBrevetRank(CurrentPCClass);
}

int32 UB2LobbyUI_BrevetMain::GetAppliedBrevetNodeIndex()
{
	return BladeIIGameImpl::GetLocalCharacterData().GetAppliedCharBrevetNodeIndex(CurrentPCClass);
}

int32 UB2LobbyUI_BrevetMain::GetOldBrevetRank()
{
	return m_nOldRank;
}

int32 UB2LobbyUI_BrevetMain::GetOldBrevetNodeIndex()
{
	return m_nOldNodeIndex;
}

bool UB2LobbyUI_BrevetMain::IsCompletedCurrentBrevetNode()
{
	FClientDataStore& cDataStore = BladeIIGameImpl::GetClientDataStore();

	const int32 CurrentCharBrevetRank = GetCurrentBrevetRank();
	const int32 BrevetRankMax = cDataStore.GetMaxBrevetRank();
	const bool IsFinalBrevetRank = CurrentCharBrevetRank >= BrevetRankMax;

	const int32 CurrentCharBrevetNode = GetCurrentBrevetNodeIndex();
	const int32 BrevetNodeMax = cDataStore.GetBrevetNodeCount(CurrentCharBrevetRank);
	const bool IsFinalBrevetNode = CurrentCharBrevetNode >= BrevetNodeMax;

	const bool IsCompletedBrevetByClientCheck = IsFinalBrevetRank && IsFinalBrevetNode;
	const bool IsCompletedBrevetByServerCheck = cDataStore.GetLocalCharacterData().GetCharBrevetNodeState(CurrentPCClass);

	return IsCompletedBrevetByClientCheck && IsCompletedBrevetByServerCheck;
}

void UB2LobbyUI_BrevetMain::OnSelectedCurrentBrevetNode()
{
	// 현재 슬롯으로 이동 후
	OnToCenterBtnClick();

	// 업그레이드 진행
	OnUpgradeBtnClick();
}

//void UB2LobbyUI_BrevetMain::SetNodeIconFromStatusType(UB2LobbyUI_BrevetNodeIcon* TargetUI, EBrevetStatusType StatusType)
//{
//	//if (!TargetUI)
//	//	return;
//
//	//UMaterialInterface* pMi;
//	//switch (StatusType)
//	//{
//	//case EBrevetStatusType::EBST_Atk:
//	//	
//	//	pMi = NodeIconAtk.LoadSynchronous();
//	//	if (pMi)
//	//		TargetUI->SetIconImage(pMi);
//	//	
//	//	pMi = NodeIconAtk_Light.LoadSynchronous();
//	//	if (pMi)
//	//		TargetUI->SetIconLightImage(pMi);
//
//	//	pMi = NodeIconAtk_Progress.LoadSynchronous();
//	//	if (pMi)
//	//		TargetUI->SetProgressImage(pMi);
//
//	//	pMi = NodeIconAtk_ProgressBack.LoadSynchronous();
//	//	if (pMi)
//	//		TargetUI->SetProgressBackImage(pMi);
//
//	//	break;
//	//case EBrevetStatusType::EBST_Def:
//	//	
//	//	pMi = NodeIconDef.LoadSynchronous();
//	//	if (pMi)
//	//		TargetUI->SetIconImage(pMi);
//
//	//	pMi = NodeIconDef_Light.LoadSynchronous();
//	//	if (pMi)
//	//		TargetUI->SetIconLightImage(pMi);
//
//	//	pMi = NodeIconDef_Progress.LoadSynchronous();
//	//	if (pMi)
//	//		TargetUI->SetProgressImage(pMi);
//
//	//	pMi = NodeIconDef_ProgressBack.LoadSynchronous();
//	//	if (pMi)
//	//		TargetUI->SetProgressBackImage(pMi);
//
//	//	break;
//	//case EBrevetStatusType::EBST_Hp:
//	//	
//	//	pMi = NodeIconHp.LoadSynchronous();
//	//	if (pMi)
//	//		TargetUI->SetIconImage(pMi);
//
//	//	pMi = NodeIconHp_Light.LoadSynchronous();
//	//	if (pMi)
//	//		TargetUI->SetIconLightImage(pMi);
//
//	//	pMi = NodeIconHp_Progress.LoadSynchronous();
//	//	if (pMi)
//	//		TargetUI->SetProgressImage(pMi);
//
//	//	pMi = NodeIconHp_ProgressBack.LoadSynchronous();
//	//	if (pMi)
//	//		TargetUI->SetProgressBackImage(pMi);
//
//	//	break;
//	//default:
//	//	break;
//	//}
//}

void UB2LobbyUI_BrevetMain::CacheAssets()
{
	//Super::CacheAssets();
//
//	GET_SLOT(UImage, IMG_RankIcon);
//	GET_SLOT(UTextBlock, TB_RankName);
//	GET_SLOT(UTextBlock, TB_BrevetAtk);
//	GET_SLOT(UTextBlock, TB_BrevetDef);
//	GET_SLOT(UTextBlock, TB_BrevetHp);
//	GET_SLOT(UTextBlock, TB_HonerPoint);
//	GET_SLOT(UTextBlock, TB_GoldPoint);
//	GET_SLOT(UTextBlock, TB_BtnAutoUpgrade);
//	GET_SLOT(UB2Button, BTN_AutoUpgrade);
//	GET_SLOT(UB2Button, BTN_ToCenter);
//	GET_SLOT(UB2Button, BTN_HelpBrevetRank);
//	GET_SLOT(UB2Button, BTN_HelpHonerPoint);	
//	GET_SLOT(UB2ScrollBox, SB_BrevetNode);
//	GET_SLOT(UB2Button, BTN_Block);
//	GET_SLOT(UB2Button, BTN_Block2);
//	GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_BrevetGoods);
//	GET_SLOT(UImage, IMG_BrevetGoods);
//	//GET_SLOT(UB2ButtonGoodInfoToolTip, BTN_GoldGoods);
//
//
//	GET_SLOT(UTextBlock, TB_TotalAbil);
//	GET_SLOT(UTextBlock, TB_ExAtk);
//	GET_SLOT(UTextBlock, TB_ExDef);
//	GET_SLOT(UTextBlock, TB_ExHp);
//	GET_SLOT(UTextBlock, TB_ProgressGrade);
//	GET_SLOT(UTextBlock, TB_Cost);
//	GET_SLOT(UTextBlock, TB_Automatic);
//	GET_SLOT(UTextBlock, TB_BtnUpgrade);
//
//	int32 CurrentCharBrevetRank = GetCurrentBrevetRank();
//	UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
//	int32 CurrentBrevetGoodsID = BladeIIGameImpl::GetClientDataStore().GetBrevetGoodsID(CurrentCharBrevetRank);
//	FSingleItemInfoData* WantedBrevetGoodsItemInfo = ItemInfoTable ? ItemInfoTable->GetInfoData(CurrentBrevetGoodsID) : NULL;
//	if (WantedBrevetGoodsItemInfo)
//	{
//		if (IMG_BrevetGoods.IsValid())
//		{
//			IMG_BrevetGoods->SetBrushFromMaterial(WantedBrevetGoodsItemInfo->GetIconMaterial(ItemInfoTable));
//		}
//
//		if (BTN_BrevetGoods.IsValid())
//		{
//			BTN_BrevetGoods->SetItemInfo(CurrentBrevetGoodsID);
//		}
//// 		if(BTN_BrevetGoods.IsValid())
//// 		{
//// 			BTN_BrevetGoods->SetItemInfo(FItemRefIDHelper::ITEM_REF_ID_BREVET_STONE);
//// 		}
//	}
//	//if (BTN_GoldGoods.IsValid())
//	//{
//	//	BTN_GoldGoods->SetItemInfo(FItemRefIDHelper::GetGoodsID_Gold());
//	//}
//	
//	BIND_CLICK_FUNC_TO_BTN(BTN_ToCenter, &UB2LobbyUI_BrevetMain::OnToCenterBtnClick);	
//	BIND_CLICK_FUNC_TO_BTN(BTN_AutoUpgrade, &UB2LobbyUI_BrevetMain::OnAutoUpgradeBtnClick);
//	BIND_CLICK_FUNC_TO_BTN(BTN_HelpHonerPoint, &UB2LobbyUI_BrevetMain::OnHelpHonerPoint);
//	BIND_CLICK_FUNC_TO_BTN(BTN_HelpBrevetRank, &UB2LobbyUI_BrevetMain::OnHelpBrevetRank);
}

void UB2LobbyUI_BrevetMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateCurrentNodeGauge(InDeltaTime);

	CheckCurrentScrollViewState();
}

void UB2LobbyUI_BrevetMain::CloseWidgetDelegate()
{
	//if (m_bIsAutoUpgradeState == true)
	//{//자동 강화중
	//	SetToggleAutoUpgradeBtn(true);
	//}
	//else if (HeroMgmtBaseNRef)
	//{
	//	HeroMgmtBaseNRef->HeaderBackButton();
	//}
}

void UB2LobbyUI_BrevetMain::UpdateCurrentNodeGauge(float InDeltaTime)
{
	//if (!m_pCurrentBrevetNode || m_fStartTime == -1 || m_fTotalTime == 0.0f)
	//	return;

	//m_bIsUpgrading = true;

	//// 가속도와 시간을 기반으로 이동한 위치를 계산
	//float fElapseTime = (GetWorld()->GetTimeSeconds() - m_fStartTime);	// 경과 시간
	//if (fElapseTime > m_fTotalTime)
	//	fElapseTime = m_fTotalTime;

	//float fNewGaugePosition = (m_fGaugePercentOld + (m_fStartBaseSpeed + (m_fAcceleration * fElapseTime)) * fElapseTime);

	//
	//SetCurrentNodeProgress(fNewGaugePosition);

	//if (fElapseTime >= m_fTotalTime)
	//{
	//	SetCurrentNodeProgress(m_fDestGaugePercent);

	//	bool IsOpenResultNotice = false;
	//	bool IsEnhanceSuccess = false;
	//	float CurVelocity = m_fStartBaseSpeed + (m_fAcceleration * m_fTotalTime);
	//	UpdateNextGaugeAnimation(CurVelocity, IsOpenResultNotice, IsEnhanceSuccess);

	//	
	//	if (IsOpenResultNotice)
	//		OpenResultUI(IsEnhanceSuccess);
	//}
}

float UB2LobbyUI_BrevetMain::GetCurGaugePercent()
{
	//if (m_pCurrentBrevetNode == nullptr)
		return -1;

	//return m_pCurrentBrevetNode->GetProgressGauge();
}

void UB2LobbyUI_BrevetMain::SetCurrentNodeProgress(float fGaugePercent)
{
	//if (m_pCurrentBrevetNode == nullptr)
	//	return;

	//m_pCurrentBrevetNode->SetProgressGauge(fGaugePercent);
}

void UB2LobbyUI_BrevetMain::CheckCurrentScrollViewState()
{
	//// 블록UI있을땐 노체크	
	//if (UB2UIManager::GetInstance()->GetUI<UB2UIWidget>(UIFName::BlockScreen) != NULL)
	//	return;

	//int32 LastAdjustIndex = SB_BrevetNode->LastAdjustItemIndex;
	//int32 BrevetNodeIndex = GetCurrentBrevetNodeIndex();
	//if (LastAdjustIndex == BrevetNodeIndex || IsUpgrading())
	//{
	//	if (BTN_ToCenter.IsValid())
	//		BTN_ToCenter->SetVisibility(ESlateVisibility::Hidden);
	//}
	//else
	//{
	//	if (BTN_ToCenter.IsValid() && BTN_ToCenter->GetVisibility() == ESlateVisibility::Hidden)
	//		BTN_ToCenter->SetVisibility(ESlateVisibility::Visible);
	//}
}

void UB2LobbyUI_BrevetMain::CheckAutoUpgrade(float fDelayTime /*= 0.0f*/)
{
	if (m_bIsAutoUpgradeState == false)
		return;

	if (IsUpgrading())
		return;

	// 현재 강화 에니메이션이 나오고있다면 
	if (m_fStartTime != -1 && m_fTotalTime != 0.0f)
		return;

	if (IsCompletedCurrentBrevetNode())
		return;

	if (GetWorld())
		GetWorld()->GetTimerManager().ClearTimer(DelayAutoUpgradeTimerHandle);

	// 업그레이드 진행
	if (fDelayTime == 0.0f)
	{
		OnSelectedCurrentBrevetNode();
	}
	else
	{
		if (GetWorld())
			GetWorld()->GetTimerManager().SetTimer(DelayAutoUpgradeTimerHandle, this, &UB2LobbyUI_BrevetMain::OnSelectedCurrentBrevetNode, fDelayTime, false);
	}

}

void UB2LobbyUI_BrevetMain::SetToggleAutoUpgradeBtn(bool IsOpenNoticePopupWindow /*= false*/)
{
	//m_bIsAutoUpgradeState = !m_bIsAutoUpgradeState;


	//BlockInputBrevetMain(m_bIsAutoUpgradeState);

	//if (TB_BtnAutoUpgrade.IsValid())
	//	TB_BtnAutoUpgrade->SetText(BladeIIGetLOCText(FString(B2LOC_CAT_GENERAL), m_bIsAutoUpgradeState ? TEXT("LobbyInvenText_AutoEnhancementCancel") : TEXT("LobbyInvenText_AutoEnhancement")));

	//if (IsOpenNoticePopupWindow && m_bIsAutoUpgradeState == false)
	//{
	//	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("AutoEnhanceCancel")),
	//		0.f,
	//		true,
	//		true,
	//		EUIMsgPopupButtonGroup::Confirm
	//	);
	//}

	//if (BTN_BrevetGoods.IsValid())
	//{
	//	BTN_BrevetGoods->SetVisibility(m_bIsAutoUpgradeState ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);
	//}

	////if (BTN_GoldGoods.IsValid())
	////{
	////	BTN_GoldGoods->SetVisibility(m_bIsAutoUpgradeState ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);
	////}

	//if(m_bIsAutoUpgradeState)
	//	TB_Automatic->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyInvenText_AutoEnhancementCancel")));
	//else
	//	TB_Automatic->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Brevet_Automatic")));
}

void UB2LobbyUI_BrevetMain::UpdateVisibilityAutoUpgradeBtn()
{
	bool IsMaxBrevet = IsCompletedCurrentBrevetNode();
	if (BTN_AutoUpgrade.IsValid())
		BTN_AutoUpgrade->SetVisibility(IsMaxBrevet ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);	
}

void UB2LobbyUI_BrevetMain::StopAutoUpgrade(bool IsOpenNoticePopupWindow /*= false*/)
{
	if (m_bIsAutoUpgradeState == false)
		return;

	if (GetWorld())
		GetWorld()->GetTimerManager().ClearTimer(DelayAutoUpgradeTimerHandle);

	SetToggleAutoUpgradeBtn(IsOpenNoticePopupWindow);
}

bool UB2LobbyUI_BrevetMain::IsUpgrading()
{
	return m_bIsUpgrading;
}

void UB2LobbyUI_BrevetMain::OpenResultUI(bool IsSuccess)
{
	/*if (IsSuccess)
	{
		OpenAddStatusUI();
		RefreshBrevetUIInfo(false);
		LobbyHeroTransientEffectBeginClass<const int32>::GetInstance().Signal(0);
	}
	else
	{
		if (m_pCurrentBrevetNode)
			m_pCurrentBrevetNode->PlayAnimationEvent_Anim_FailNotice_BP();
	}*/

	//PlaySoundResultUpgradeNode_BP(IsSuccess);
}

const FBrevetNodeInfo* UB2LobbyUI_BrevetMain::GetCurrentBrevetNodeInfo()
{
	int32 BrevetRank = GetCurrentBrevetRank();
	int32 BrevetNodeIndex = GetCurrentBrevetNodeIndex();
	return BladeIIGameImpl::GetClientDataStore().GetBrevetNodeInfo(BrevetRank, BrevetNodeIndex);
}

void UB2LobbyUI_BrevetMain::OpenAddStatusUI()
{
	//const FBrevetNodeInfo* BrevetNodeData = BladeIIGameImpl::GetClientDataStore().GetBrevetNodeInfo(GetOldBrevetRank(), GetOldBrevetNodeIndex());

	//if (!BrevetNodeData)
	//	return;

	////연출용 UI띄움 스텟증가!
	//int32 nStatusType = BrevetNodeData->StatusType;
	//int32 nStatusValue = BrevetNodeData->StatusValue;

	//// UI리뉴얼로 인한 코드 주석처리 170512 YJ
	////UI만들고 위값 설정해줌
	///*if (UB2UIBrevetAddStatus* pUI = UB2UIManager::GetInstance()->OpenUI<UB2UIBrevetAddStatus>(UIFName::BrevetAddStatus))
	//{
	//	UMaterialInterface* pGaugeMI = NULL;
	//	UMaterialInterface* pGaugeBackMI = NULL;
	//	UMaterialInterface* pIconMIl = NULL;

	//	switch (static_cast<EBrevetStatusType>(BrevetNodeData->StatusType))
	//	{
	//	case EBrevetStatusType::EBST_Atk:
	//		pGaugeMI = NodeIconAtk_Progress.LoadSynchronous();
	//		pGaugeBackMI = NodeIconAtk_ProgressBack.LoadSynchronous();
	//		pIconMIl = NodeIconAtk.LoadSynchronous();
	//		break;
	//	case EBrevetStatusType::EBST_Def:
	//		pGaugeMI = NodeIconDef_Progress.LoadSynchronous();
	//		pGaugeBackMI = NodeIconDef_ProgressBack.LoadSynchronous();
	//		pIconMIl = NodeIconDef.LoadSynchronous();
	//		break;
	//	case EBrevetStatusType::EBST_Hp:
	//		pGaugeMI = NodeIconHp_Progress.LoadSynchronous();
	//		pGaugeBackMI = NodeIconHp_ProgressBack.LoadSynchronous();
	//		pIconMIl = NodeIconHp.LoadSynchronous();
	//		break;
	//	}

	//	if (pGaugeMI && pGaugeBackMI && pIconMIl)
	//	{
	//		pUI->InitBrevetAddStatus(this, BrevetNodeData, pGaugeBackMI, pGaugeMI, pIconMIl, CurrentPCClass);
	//	}
	//}*/

	//if (UB2UIUpgradeStatNotify* pUI = UB2UIManager::GetInstance()->OpenUI<UB2UIUpgradeStatNotify>(UIFName::UpgradeStatNotify))
	//{
	//	//pUI->SetAutoClose();
	//	pUI->ClearList();
	//	FText StatNameText;
	//	switch (static_cast<EBrevetStatusType>(nStatusType))
	//	{
	//	case EBrevetStatusType::EBST_Atk:
	//		StatNameText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("IncreaseStatAttack"));
	//		break;
	//	case EBrevetStatusType::EBST_Def:
	//		StatNameText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("IncreaseStatDefense"));
	//		break;
	//	case EBrevetStatusType::EBST_Hp:
	//		StatNameText = BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("IncreaseStatHealth"));
	//		break;
	//	}
	//	pUI->AddListItem(StatNameText, FText::FromString("+"+FString::FromInt(nStatusValue)),true);
	//	ShowBattleScoreNotifyClass<EPCClass>::GetInstance().Signal(CurrentPCClass);
	//	OnFinishAnimationEvent_BrevetAddStatusInitAnimation();
	//}
}

void UB2LobbyUI_BrevetMain::OpenRankUpSuccessUI()
{
	//if (UB2UIBrevetRankUpSuccess* pUI = UB2UIManager::GetInstance()->OpenUI<UB2UIBrevetRankUpSuccess>(UIFName::BrevetRankUpSuccess))
	//{
	//	if (IMG_RankIcon.IsValid())
	//	{
	//		IMG_RankIcon->SetVisibility(ESlateVisibility::Hidden);
	//	}

	//	pUI->InitBrevetRankUpSuccess(this, GetOldBrevetRank(), GetCurrentBrevetRank());
	//}
}

void UB2LobbyUI_BrevetMain::SetCurrentNode()
{
	//int32 BrevetNodeIndex = GetCurrentBrevetNodeIndex();
	//// 현재노드 저장(m_arBrevetNodes 첫번째랑 끝 아이템은 장식용 더미임)
	//if (BrevetNodeIndex != -1 && m_arBrevetNodes.Num()-2 >= BrevetNodeIndex)
	//	m_pCurrentBrevetNode = Cast<UB2LobbyUI_BrevetNodeIcon>(m_arBrevetNodes[BrevetNodeIndex]);
}
//
//UB2LobbyUI_BrevetNodeIcon* UB2LobbyUI_BrevetMain::GetNextNode()
//{
//	int32 BrevetNodeIndex = GetCurrentBrevetNodeIndex();
//
//	//현재꺼 다음꺼
//	BrevetNodeIndex += 1;
//
//	// 현재노드 저장(m_arBrevetNodes 첫번째랑 끝 아이템은 장식용 더미임)
//	if (BrevetNodeIndex != -1 && m_arBrevetNodes.Num()-2 >= BrevetNodeIndex)
//		return Cast<UB2LobbyUI_BrevetNodeIcon>(m_arBrevetNodes[BrevetNodeIndex]);
//
//	return NULL;
//}

void UB2LobbyUI_BrevetMain::StartGaugeAnimation(bool IsSuccessEnhance)
{
	if (BaseGaugeAnimationList.Num() == 0 || SuccessGaugeAnimationList.Num() == 0 || FailGaugeAnimationList.Num() == 0)
		return;

	CurGaugeAnimationQueue.Empty();
	for (auto& BaseAni : BaseGaugeAnimationList)
		CurGaugeAnimationQueue.Enqueue(BaseAni);

	for (auto& FinishAni : (IsSuccessEnhance ? SuccessGaugeAnimationList : FailGaugeAnimationList))
		CurGaugeAnimationQueue.Enqueue(FinishAni);

	if (CurGaugeAnimationQueue.IsEmpty() == false)
	{
		SetCurrentNodeProgress(0.0f);

		bool OutIsOpenResultNotice, OutIsEnhanceSuccess;
		UpdateNextGaugeAnimation(0.0f, OutIsOpenResultNotice, OutIsEnhanceSuccess);
		
		//PlaySoundStartUpgradeNode_BP();
	}
}

void UB2LobbyUI_BrevetMain::UpdateNextGaugeAnimation(float CurVelocity, bool& OutIsOpenResultNotice, bool& OutIsEnhanceSuccess)
{
	FGaugeAnimationEle Ele;
	if (CurGaugeAnimationQueue.Dequeue(Ele))
	{
		OutIsOpenResultNotice = false;
		OutIsEnhanceSuccess = false;

		m_fStartTime = GetWorld()->GetTimeSeconds();
		m_fStartBaseSpeed = CurVelocity;
		m_fTotalTime = Ele.TotalGaugeTime;
		m_fGaugePercentOld = m_fDestGaugePercent;
		m_fDestGaugePercent = Ele.DestGaugePosition;

		if (m_fTotalTime != 0)
			m_fAcceleration = (((m_fDestGaugePercent - m_fGaugePercentOld) / m_fTotalTime) - m_fStartBaseSpeed) / m_fTotalTime;
	}
	else
	{
		OutIsOpenResultNotice = true;
		OutIsEnhanceSuccess = (m_fDestGaugePercent == 1.0f);

		m_fAcceleration = 1.0f;
		m_fStartBaseSpeed = 0.0f;
		m_fStartTime = -1;
		m_fGaugePercentOld = -1;
		m_fDestGaugePercent = 0.0f;
		m_fTotalTime = 0.0f;
	}
}

void UB2LobbyUI_BrevetMain::BlockInputBrevetMain(bool bCheck)
{
	//if (BTN_Block.IsValid())
	//	BTN_Block->SetVisibility(bCheck ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	//if (BTN_Block2.IsValid())
	//	BTN_Block2->SetVisibility(bCheck ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	//if (HeroMgmtBaseNRef)
	//{ // 공통 파트에도 버튼이 많으니..
	//	HeroMgmtBaseNRef->SetExtraDisableButtons(bCheck);
	//}
}

void UB2LobbyUI_BrevetMain::OnUpgradeBtnClick()
{
	//if (IsUpgrading())
	//	return;

	//if (m_fStartTime != -1 && m_fTotalTime != 0.0f)
	//	return;

	//// 강화를 끝까지 진행했는지
	//if (IsCompletedCurrentBrevetNode())
	//{
	//	StopAutoUpgrade();
	//	return;
	//}

	//const FBrevetNodeInfo* BrevetNodeData = GetCurrentBrevetNodeInfo();
	//if (!BrevetNodeData)
	//	return;

	//if (BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(CurrentPCClass) < BrevetNodeData->OpenLevel)
	//{
	//	StopAutoUpgrade();

	//	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//		FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LevelUpForUnrock")), FText::AsNumber(BrevetNodeData->OpenLevel)),
	//		0.f,
	//		true,
	//		true,
	//		EUIMsgPopupButtonGroup::Confirm
	//		);

	//	return;
	//}

	//bool checkNormal = (GetCurrentBrevetRank() <= BladeIIGameImpl::GetClientDataStore().GetMaximumNormalBrevetGade());
	//int32 CurrentHonerPoint;

	//if (checkNormal)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Mate it's normal"));
	//	CurrentHonerPoint = BladeIIGameImpl::GetClientDataStore().GetRankPromotionPoint();
	//	if (CurrentHonerPoint == 0 || CurrentHonerPoint < BrevetNodeData->NeedRankPromotionPoint)
	//	{
	//		StopAutoUpgrade();
	//		//*/
	//		ShortageMGR->CheckContentsShortageShortcut(FItemRefIDHelper::ITEM_REF_ID_BREVET_STONE);
	//		/*/
	//		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::GoTeamMatch,
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("CollectHonerPointInTeamMatch"))
	//		);
	//		/*/

	//		return;
	//	}
	//}
	//else
	//{
	//	CurrentHonerPoint = BladeIIGameImpl::GetClientDataStore().GetAdvancedRankPromotionPoint();
	//	if (CurrentHonerPoint == 0 || CurrentHonerPoint < BrevetNodeData->NeedRankPromotionPoint)
	//	{
	//		StopAutoUpgrade();
	//		ShortageMGR->CheckContentsShortageShortcut(FItemRefIDHelper::ITEM_REF_ID_ADVANCED_BREVET_STONE);
	//		return;
	//	}
	//}

	//if (BrevetNodeData->NeedActiveGold > BladeIIGameImpl::GetClientDataStore().GetGoldAmount())
	//{
	//	//*/
	//	ShortageMGR->PopupGoToShopForGoldMessage();
	//	/*/
	//	PopupGoToShopForGoldMessage();
	//	//*/

	//	StopAutoUpgrade();
	//	return;
	//}

	//// Old Gauge Percent
	//m_bIsUpgrading = true;
	//m_nOldRank = GetCurrentBrevetRank();
	//m_nOldNodeIndex = GetCurrentBrevetNodeIndex();
	//data_trader::Retailer::GetInstance().RequestEnhanceRankNode(CliToSvrPCClassType(CurrentPCClass));

	//BlockInputBrevetMain(true);
	//SetClose(m_bIsAutoUpgradeState);
}

void UB2LobbyUI_BrevetMain::OnAutoUpgradeBtnClick()
{


	SetToggleAutoUpgradeBtn(true);
	CheckAutoUpgrade();
}

void UB2LobbyUI_BrevetMain::OnToCenterBtnClick()
{
	//SB_BrevetNode->SetAdjustFromSlotIndex(GetCurrentBrevetNodeIndex());
}

void UB2LobbyUI_BrevetMain::OnHelpHonerPoint()
{
}

void UB2LobbyUI_BrevetMain::OnHelpBrevetRank()
{
}

void UB2LobbyUI_BrevetMain::OnDeliveryEnhanceRankNode(FB2EnhanceRankNode EnhanceRankNode)
{

	//int32 nCharacter_type = GET_TUPLE_DATA(FB2ResponseEnhanceRankNode::character_type_index, EnhanceRankNode);
	//int32 nRank_index = GET_TUPLE_DATA(FB2ResponseEnhanceRankNode::rank_index, EnhanceRankNode);
	//int32 nNode_index = GET_TUPLE_DATA(FB2ResponseEnhanceRankNode::rank_node_index_index, EnhanceRankNode);
	//bool nNode_state = GET_TUPLE_DATA(FB2ResponseEnhanceRankNode::rank_node_state_index, EnhanceRankNode);
	//int32 current_gold = GET_TUPLE_DATA(FB2ResponseEnhanceRankNode::current_gold_index, EnhanceRankNode);
	//bool is_success = GET_TUPLE_DATA(FB2ResponseEnhanceRankNode::is_success_index, EnhanceRankNode);

	//bool checkNormal = (GetCurrentBrevetRank() <= BladeIIGameImpl::GetClientDataStore().GetMaximumNormalBrevetGade());	//이 부분 검증 필요
	//if (checkNormal)
	//{
	//	BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountDecrease(FItemRefIDHelper::ITEM_REF_ID_BREVET_STONE, -1,
	//		GET_TUPLE_DATA(FB2ResponseEnhanceRankNode::changed_rank_promotion_points_index, EnhanceRankNode),
	//		GET_TUPLE_DATA(FB2ResponseEnhanceRankNode::deleted_rank_promotion_point_ids_index, EnhanceRankNode)
	//	);
	//}
	//else
	//{
	//	BladeIIGameImpl::GetClientDataStore().OnResponseConsumableAmountDecrease(FItemRefIDHelper::ITEM_REF_ID_ADVANCED_BREVET_STONE, -1,
	//		GET_TUPLE_DATA(FB2ResponseEnhanceRankNode::changed_rank_promotion_points_index, EnhanceRankNode),
	//		GET_TUPLE_DATA(FB2ResponseEnhanceRankNode::deleted_rank_promotion_point_ids_index, EnhanceRankNode)
	//	);
	//}
	//BladeIIGameImpl::GetClientDataStore().SetGoldAmount(current_gold);
	//BladeIIGameImpl::GetLocalCharacterData().SetCharBrevetInfo(SvrToCliPCClassType(nCharacter_type), nRank_index, nNode_index, nNode_state);

	//if (auto* DocUser = UB2UIDocHelper::GetDocUser())
	//{
	//	DocUser->UpdateUserData();
	//	UpdateCombatPowerClass<EPCClass, bool>::GetInstance().Signal(SvrToCliPCClassType(nCharacter_type), false);
	//}

	////RefreshBrevetUIInfo();

	//UB2UIManager::GetInstance()->OpenUI(UIFName::BlockScreen);	//블록UI 생성

	//StartGaugeAnimation(is_success);
	//

}

void UB2LobbyUI_BrevetMain::OnFinishAnimationEvent_BrevetAddStatusInitAnimation()
{
	//if (m_pCurrentBrevetNode == nullptr)
	//	return;

	//m_pCurrentBrevetNode->PlayAnimationEvent_Anim_Current_Close();
	//m_pCurrentBrevetNode->SetBrevetNodeState(EBrevetNodeState::EBNS_Complete);
}

void UB2LobbyUI_BrevetMain::OnFinishAnimationEvent_Anim_Current_Close()
{	

	//if(m_bIsAutoUpgradeState == false)
	//	m_bIsUpgrading = false;

	//if (IsCompletedCurrentBrevetNode())
	//{
	//	RefreshBrevetUIInfo(true);
	//	StopAutoUpgrade();

	//	return;
	//}

	//// 랭크업했나 체크 / 초기(첫노드 활성화 이전)엔 표기상 1랭크지만 데이터는 0랭크임 고로 0랭크 무시
	//const int32 OldRank = GetOldBrevetRank();
	//if (OldRank != 0 && OldRank != GetCurrentBrevetRank())
	//{
	//	OpenRankUpSuccessUI();
	//	return;
	//}

	//// 현재 노드설정
	//SetCurrentNode();

	//// 스크롤 셋팅
	//int32 BrevetNodeIndex = GetCurrentBrevetNodeIndex();
	//
	//SB_BrevetNode->SetAnimateScroll(true);
	//SB_BrevetNode->SetAdjustFromSlotIndex(BrevetNodeIndex);
	//
	//if (!m_pCurrentBrevetNode)
	//	return;

	//// 노드 애니메이션
	//if ( BladeIIGameImpl::GetLocalCharacterData().GetCharacterLevel(CurrentPCClass) < m_pCurrentBrevetNode->m_pBrevetNodeInfo->OpenLevel)
	//{
	//	//아마도 이젠 여기를 탈일이 없을것이다.
	//	m_pCurrentBrevetNode->PlayAnimationEvent_Anim_Current_Lock();
	//	m_pCurrentBrevetNode->SetBrevetNodeState(EBrevetNodeState::EBNS_Locked_Current);

	//	// 오픈 애니메이션 못봣다고 저장 CheckNeexNodeOpenAnimation() 에서 설정
	//	GConfig->SetBool(TEXT("BrevetNodeAnim"), *FString::FromInt((int32)CurrentPCClass), false, GB2UserSavedStateIni);
	//}
	//else
	//{
	//	m_pCurrentBrevetNode->PlayAnimationEvent_Anim_Current_Open();
	//	m_pCurrentBrevetNode->SetBrevetNodeState(EBrevetNodeState::EBNS_UnLocked);

	//	// 다음노드에 xx활성화시 오픈 텍스트 보이기설정
	//	if (UB2LobbyUI_BrevetNodeIcon* pUI = GetNextNode())
	//		pUI->SetNotOpenText(m_pCurrentBrevetNode->GetBrevetNodeNum());
	//}

	//// 활성화로 만든 블록UI 이쯤에서 제거해줘도 될듯
	//UB2UIManager::GetInstance()->CloseUI(UIFName::BlockScreen);

	//if (m_bIsAutoUpgradeState == false)
	//{
	//	BlockInputBrevetMain(false);
	//	SetClose(true);
	//}
}

void UB2LobbyUI_BrevetMain::OnFinishAnimationEvent_Anim_RankUp()
{
	if (IMG_RankIcon.IsValid())
	{
		IMG_RankIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	RefreshBrevetUIScrollBoxInfo();
	RefreshBrevetUIInfo(true);

	StopAutoUpgrade();
}

void UB2LobbyUI_BrevetMain::OnFinishAnimationEvent_Anim_UnLock_BP()
{
	m_bIsUpgrading = false;

	CheckAutoUpgrade();

	UpdateVisibilityAutoUpgradeBtn();

	if (m_bIsAutoUpgradeState == false)
	{
		BlockInputBrevetMain(false);
		//SetClose(true);
	}
}

void UB2LobbyUI_BrevetMain::OnFinishAnimationEvent_Anim_FailNotice_BP()
{
	m_bIsUpgrading = false;

	CheckAutoUpgrade(0.5f);

	if (m_bIsAutoUpgradeState == false)
	{
		BlockInputBrevetMain(false);
		//SetClose(true);
	}
}
