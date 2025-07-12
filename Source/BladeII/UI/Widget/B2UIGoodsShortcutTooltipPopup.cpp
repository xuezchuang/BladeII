// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIGoodsShortcutTooltipPopup.h"
#include "B2ItemInfo.h"
#include "B2GoodsTypeInfo.h"
#include "B2UIManager.h"
#include "B2UIGoodsShortcutParts.h"
#include "B2LogManager.h"
#include "B2AndroidBackManager.h"
#include "BladeIIGameImpl.h"
#include "B2PlatformAPI.h"
#include "B2StageManager.h"

//B2GetLogger(UITooltipPopup);

UB2UIGoodsShortcutTooltipPopup::UB2UIGoodsShortcutTooltipPopup(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UB2UIGoodsShortcutTooltipPopup::CacheAssets()
{
	Super::CacheAssets();

#if !PLATFORM_MAC
	// 맥 에디터 환경일 경우 GET_SLOT_CHECK 빌드 실패로 인해 제외..
	GET_SLOT_CHECK(UOverlay, O_ShortcutTooltip)
	GET_SLOT_CHECK(UOverlay, O_InfoTooltip)
	GET_SLOT_CHECK(UImage, IMG_BGImage)
	GET_SLOT_CHECK(UImage, IMG_GoodsIcon)
	GET_SLOT_CHECK(UTextBlock, TB_Shortage)
	GET_SLOT_CHECK(UTextBlock, TB_GoodsName)
	GET_SLOT_CHECK(UTextBlock, TB_GoodsInfo)

	GET_SLOT_CHECK(UImage, IMG_InfoGoodsIcon)
	GET_SLOT_CHECK(UTextBlock, TB_InfoGoodsName)
	GET_SLOT_CHECK(UTextBlock, TB_InfoGoodsString)

	GET_SLOT_CHECK(UB2Button, BTN_Close)
	GET_SLOT_CHECK(UB2Button, BTN_Modal)

	GET_SLOT_CHECK(UCanvasPanel, CP_ShortcutPage)
	GET_SLOT_CHECK(UB2ScrollBox, SB_ShortcutPage)
	GET_SLOT_CHECK(UVerticalBox, VB_ShortcutPage)
#endif

#if 0 //WITH_EDITOR
	if (true == GIsEditor)
	{
		BindDelegates();
		UClass* getclass = GetClass();
		UWidgetBlueprintGeneratedClass* castClass = Cast<UWidgetBlueprintGeneratedClass>(getclass);
		TArray<UWidget*> localwidget;
		castClass->WidgetTree->GetAllWidgets(localwidget);

		for (auto& elem : localwidget)
		{
			//Subclass는 어쩌지??
			if (false == elem->GetClass()->IsChildOf(*m_CheckValid.Find(elem->GetName())))
			{
				//SubClass가 아니니 잘못 설정 한 것임
				//경고창 띄워주기
			}
			auto namecheck = m_CheckValid.Find(elem->GetName());
			if (namecheck != nullptr)
			{
				//이름이 바뀐거니 이름을 다시 넣어줘야 함
			}
			B2LOGMANAGER_ERROR(LoggerUITooltipPopup, "Name is [%s], Class Name is [%s]", TCHAR_TO_ANSI(*elem->GetName()), TCHAR_TO_ANSI(*elem->GetClass()->GetName()));
		}
	}
#endif //WITH_EDITOR
}
/*
void UB2UIGoodsShortcutTooltipPopup::PostLoad()
{
	Super::PostLoad();
#if WITH_EDITOR
	CacheAssets();
#endif //WITH_EDITOR
}
*/
void UB2UIGoodsShortcutTooltipPopup::Init()
{
	Super::Init();

	//ForcedCloseToolTipPopupClass<>::GetInstance().Subscribe([this]()
	//{
	//	this->OnForcedCloseToolTipPopup();
	//});
}

void UB2UIGoodsShortcutTooltipPopup::PreSave(FObjectPreSaveContext SaveContext)
{
//#if WITH_EDITOR
//	if (true == GIsEditor && !IsSavingForCookingOnPreSave(TargetPlatform))
//	{ // 에셋 검사용 코드. 쿠킹에서는 크래쉬를 일으키므로 제외. 혹은 검사 코드를 쿠킹에서도 작동하도록 보완..
//		CacheAssets();
//	}
//#endif //WITH_EDITOR
	Super::PreSave(SaveContext);
}

void UB2UIGoodsShortcutTooltipPopup::SetItemInfo(uint32 _IteminfoIdx, const EGoodsButtonType& _type)
{
//#if UE_BUILD_SHIPPING
//	//_IteminfoIdx가 0이하가 들어오면 팝업에 띄워주는 정보가 하나도 없으므로 자동 파괴
//	//단 디버그일때는 디버그를 위해 정보가 없는 상태로 남김
//	if (_IteminfoIdx < 1)
//	{
//		OnClickBtnClose();
//		return;
//	}
//#endif
//	ToolTipOpenClass<>::GetInstance().Signal();
//	m_IteminfoIdx = _IteminfoIdx;
//
//	FSingleItemInfoData* const singleData = StaticFindItemInfo() ? StaticFindItemInfo()->GetInfoData(m_IteminfoIdx) : nullptr;
//	
//	AllTextBoxHide();
//	if (singleData == nullptr) 
//	{
//		OnClickBtnClose();
//		return; //데이터가 없으면 Close 버튼만 보여줌
//	}
//		
//	const FSingleGoodsTypeInfoData* const goodsTypeInfo(GetGoodsTypeFromGoodsTypeInfoObject(singleData->FGoodsType));
//	
//	m_type = _type;
//	SetGoodsIcon(m_IteminfoIdx);
//	switch (_type)
//	{
//		case EGoodsButtonType::EGoodsButtonType_NONE:
//		default:
//			break;
//		case EGoodsButtonType::EGoodsButtonType_Info:
//			if (O_InfoTooltip.IsValid())
//				O_InfoTooltip->SetVisibility(ESlateVisibility::Visible);
//			SetGoodsNameText(m_IteminfoIdx);
//			SetGoodsInfoText(goodsTypeInfo->GoodsInfoStringID);
//			break;
//		case EGoodsButtonType::EGoodsButtonType_InfoShortcut:
//			if (BTN_Close.IsValid())
//				BTN_Close->SetVisibility(ESlateVisibility::Visible);
//			if (O_ShortcutTooltip.IsValid())
//				O_ShortcutTooltip->SetVisibility(ESlateVisibility::Visible);
//			SetGoodsNameText(m_IteminfoIdx);
//			SetGoodsInfoText(goodsTypeInfo->GoodsInfoStringID);
//			addRow(goodsTypeInfo);
//			break;
//		case EGoodsButtonType::EGoodsButtonType_ShortageShortcut:
//			if (BTN_Close.IsValid())
//				BTN_Close->SetVisibility(ESlateVisibility::Visible);
//			if (O_ShortcutTooltip.IsValid())
//				O_ShortcutTooltip->SetVisibility(ESlateVisibility::Visible);
//			SetGoodsNameText(m_IteminfoIdx);
//			//SetGoodsShortageText(goodsTypeInfo->ShortageInfoStringID);
//			SetGoodsInfoText(goodsTypeInfo->ShortageInfoStringID);
//			SetGoodsShortageTextColor(FSlateColor(FLinearColor(0.879623f, 0.03434f, 0.002428, 1.0f)));
//			addRow(goodsTypeInfo);
//			break;
//	}
}

void UB2UIGoodsShortcutTooltipPopup::SetOpenStageItemInfo(int32 _ItemRefID, TArray<bool> _IsContentsOpened, const EGoodsButtonType& _GoodsButtonType)
{
//#if UE_BUILD_SHIPPING
//	if (_ItemRefID < 1)
//	{
//		OnClickBtnClose();
//		return;
//	}
//#endif
//	ToolTipOpenClass<>::GetInstance().Signal();
//	AllTextBoxHide();
//
//	FSingleItemInfoData* const singleData = StaticFindItemInfo() ? StaticFindItemInfo()->GetInfoData(_ItemRefID) : nullptr;
//	if (singleData == nullptr)
//	{
//		OnClickBtnClose();
//		return;
//	}
//
//	const FSingleGoodsTypeInfoData* const goodsTypeInfo(GetGoodsTypeFromGoodsTypeInfoObject(singleData->FGoodsType));
//	
//	if (BTN_Close.IsValid())
//		BTN_Close->SetVisibility(ESlateVisibility::Visible);
//	if (O_ShortcutTooltip.IsValid())
//		O_ShortcutTooltip->SetVisibility(ESlateVisibility::Visible);
//	
//	SetGoodsIcon(_ItemRefID);
//	switch (_GoodsButtonType)
//	{
//	case EGoodsButtonType::EGoodsButtonType_Info:
//		if (O_InfoTooltip.IsValid())
//			O_InfoTooltip->SetVisibility(ESlateVisibility::Visible);
//		SetGoodsNameText(_ItemRefID);
//		SetGoodsInfoText(goodsTypeInfo->GoodsInfoStringID);
//		break;
//
//	case EGoodsButtonType::EGoodsButtonType_InfoShortcut:
//		if (BTN_Close.IsValid())
//			BTN_Close->SetVisibility(ESlateVisibility::Visible);
//		if (O_ShortcutTooltip.IsValid())
//			O_ShortcutTooltip->SetVisibility(ESlateVisibility::Visible);
//		SetGoodsNameText(_ItemRefID);
//		SetGoodsInfoText(goodsTypeInfo->GoodsInfoStringID);
//		addRowOpenStage(goodsTypeInfo, _IsContentsOpened);
//		break;
//
//	case EGoodsButtonType::EGoodsButtonType_ShortageShortcut:
//		if (BTN_Close.IsValid())
//			BTN_Close->SetVisibility(ESlateVisibility::Visible);
//		if (O_ShortcutTooltip.IsValid())
//			O_ShortcutTooltip->SetVisibility(ESlateVisibility::Visible);
//		SetGoodsNameText(_ItemRefID);
//		SetGoodsInfoText(goodsTypeInfo->ShortageInfoStringID);
//		SetGoodsShortageTextColor(FSlateColor(FLinearColor(0.879623f, 0.03434f, 0.002428, 1.0f)));
//		addRowOpenStage(goodsTypeInfo, _IsContentsOpened);
//		break;
//
//	case EGoodsButtonType::EGoodsButtonType_NONE:
//	default:
//		break;
//	}
}

FVector2D UB2UIGoodsShortcutTooltipPopup::GetUISize() const 
{
	const UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
	FVector2D sizeVector(FVector2D::ZeroVector);
	if (!UIMgrInst)	return sizeVector;

	if (O_ShortcutTooltip.IsValid())
	{
		const UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(O_ShortcutTooltip->Slot);
		if (MainPanelSlot)
			sizeVector = MainPanelSlot->GetSize();
	}
	return sizeVector;
	//return FVector2D(GetDesiredSize());
}

void UB2UIGoodsShortcutTooltipPopup::SetSlotPosition(const FVector2D& _Pos)
{
	if (O_ShortcutTooltip.IsValid())
	{
		if(O_ShortcutTooltip->IsVisible())
		{
			UCanvasPanelSlot* const MainPanelSlot = Cast<UCanvasPanelSlot>(O_ShortcutTooltip->Slot);
			if (MainPanelSlot)
				MainPanelSlot->SetPosition(_Pos);// +MainPanelSlot->GetPosition());
		}
	}
	if (O_InfoTooltip.IsValid())
	{
		if (O_InfoTooltip->IsVisible())
		{
			UCanvasPanelSlot* const MainPanelSlot = Cast<UCanvasPanelSlot>(O_InfoTooltip->Slot);
			if (MainPanelSlot)
				MainPanelSlot->SetPosition(_Pos);// +MainPanelSlot->GetPosition());
		}
	}
}

FVector2D UB2UIGoodsShortcutTooltipPopup::GetSlotAnchors() const
{
	if (O_ShortcutTooltip.IsValid())
	{
		UCanvasPanelSlot* const MainPanelSlot = Cast<UCanvasPanelSlot>(O_ShortcutTooltip->Slot);
		if (MainPanelSlot)
			return MainPanelSlot->GetAnchors().Minimum;
	}
	return FVector2D::ZeroVector;
}

void UB2UIGoodsShortcutTooltipPopup::SetGoodsIcon(int32 _GoodsIconID)
{
	//_GoodsIconID를 받아와서 바꿔줌
	UB2ItemInfo* const iteminfo = StaticFindItemInfo();
	if (iteminfo)
	{
		if(IMG_GoodsIcon.IsValid())
		{
			IMG_GoodsIcon->SetBrushFromMaterial(StaticFindItemInfo()->GetItemIcon(_GoodsIconID));
		}
		if(IMG_InfoGoodsIcon.IsValid())
		{
			IMG_InfoGoodsIcon->SetBrushFromMaterial(StaticFindItemInfo()->GetItemIcon(_GoodsIconID));
		}
	}
}

void UB2UIGoodsShortcutTooltipPopup::SetGoodsNameText(int32 _GoodsNameStringID)
{
	//_GoodsNameStringID를 받아와서 바꿔줌
	UB2ItemInfo* const iteminfo(StaticFindItemInfo());
	if (iteminfo)
	{
		if (TB_GoodsName.IsValid())
		{
			TB_GoodsName->SetVisibility(ESlateVisibility::Visible);
			TB_GoodsName->SetText(StaticFindItemInfo()->GetItemName(_GoodsNameStringID));
		}
		if (TB_InfoGoodsName.IsValid())
		{
			TB_InfoGoodsName->SetVisibility(ESlateVisibility::Visible);
			TB_InfoGoodsName->SetText(StaticFindItemInfo()->GetItemName(_GoodsNameStringID));
		}
	}
}

void UB2UIGoodsShortcutTooltipPopup::SetGoodsInfoText(int32 _GoodsInfoStringID)
{
	FText text = BladeIIGetLOCText(B2LOC_CAT_TOOLTIPTEXT, *(FString::FromInt(_GoodsInfoStringID)));
	if (TB_GoodsInfo.IsValid())
	{
		TB_GoodsInfo->SetVisibility(ESlateVisibility::Visible);
		TB_GoodsInfo->SetText(text);
	}
	if (TB_InfoGoodsString.IsValid())
	{
		TB_InfoGoodsString->SetVisibility(ESlateVisibility::Visible);
		TB_InfoGoodsString->SetText(text);
	}
}

void UB2UIGoodsShortcutTooltipPopup::SetGoodsShortageTextColor(const FSlateColor& color)
{
	if (TB_GoodsInfo.IsValid())
	{
		TB_GoodsInfo->SetColorAndOpacity(color);
	}
	if (TB_InfoGoodsString.IsValid())
	{
		TB_InfoGoodsString->SetColorAndOpacity(color);
	}
}

void UB2UIGoodsShortcutTooltipPopup::SetGoodsShortageText(int32 _GoodsInfoStringID)
{
	if (TB_Shortage.IsValid())
	{
		TB_Shortage->SetVisibility(ESlateVisibility::Visible);
		TB_Shortage->SetText(BladeIIGetLOCText(B2LOC_CAT_TOOLTIPTEXT, *(FString::FromInt(_GoodsInfoStringID))));
	}
}

//UIP_TooltipButton에 들어갈 정보
void UB2UIGoodsShortcutTooltipPopup::addRow(const struct FSingleGoodsTypeInfoData*  _GoodsShortcutInfo)
{
	// [@AKI, 170412] 일단 이렇게 마무리 하고 아래쪽을 다시 구현 하기로 함
	/*/
	SB_ShortcutPage->ClearChildren();
	const FUB2GoodsShortcutButtonInfo shortcutInfo = _GoodsShortcutInfo->ShortcutInfo[0];
	for(int i = 0; i < 1; i++)
	//for (auto& shortcutInfo : _GoodsShortcutInfo->ShortcutInfo)
	{
		UB2UIGoodsShortcutParts* const GoodsShortcutRowWidget = CreateWidget<UB2UIGoodsShortcutParts>(GetOwningPlayer(), UIP_Shortcut_Button);
		if (GoodsShortcutRowWidget != nullptr)
		{
			GoodsShortcutRowWidget->Init();
			//@AKI, For Test
			//GoodsShortcutRowWidget->SetGoodsShortcutInfo(0, EUIScene::Store, 0, 0);
			GoodsShortcutRowWidget->SetGoodsShortcutInfo(shortcutInfo.ShortcutUIPStringID, shortcutInfo.ShortcutSceneName, shortcutInfo.ShortcutScene_Option1, shortcutInfo.ShortcutScene_Option2);
			GoodsShortcutRowWidget->SetParentUI(this);

			MGR_Shortcut_Button.Add(GoodsShortcutRowWidget);
			if (SB_ShortcutPage.IsValid())
				SB_ShortcutPage->AddChild(GoodsShortcutRowWidget);
		}
	}
	/*/
	if(SB_ShortcutPage.IsValid())
		SB_ShortcutPage->ClearChildren();
	if (CP_ShortcutPage.IsValid())
		CP_ShortcutPage->SetVisibility(ESlateVisibility::Collapsed);
	if (VB_ShortcutPage.IsValid())
	{
		VB_ShortcutPage->ClearChildren();
		VB_ShortcutPage->SetVisibility(ESlateVisibility::Collapsed);
	}

	const uint8 shortcutinfoNum(_GoodsShortcutInfo->ShortcutInfo.Num());
	const uint8 MaxUIPNum(4);
	if(shortcutinfoNum > MaxUIPNum)
	{
		CP_ShortcutPage->SetVisibility(ESlateVisibility::Visible);

		for (auto& shortcutInfo : _GoodsShortcutInfo->ShortcutInfo)
		{
			UB2UIGoodsShortcutParts* const GoodsShortcutRowWidget = CreateWidget<UB2UIGoodsShortcutParts>(GetOwningPlayer(), UIP_Shortcut_Button);
			if (GoodsShortcutRowWidget != nullptr)
			{
				GoodsShortcutRowWidget->Init();
				GoodsShortcutRowWidget->SetGoodsShortcutInfo(shortcutInfo.ShortcutUIPStringID, 
					shortcutInfo.ShortcutSceneName, 
					shortcutInfo.ShortcutScene_Option1, 
					shortcutInfo.ShortcutScene_Option2);

				if (shortcutInfo.ShortcutSceneName == EUIScene::HallOfFame && B2P_IsInReview())
				{
					GoodsShortcutRowWidget->SetVisibleButton(false);
				}
				else
				{
					GoodsShortcutRowWidget->SetVisibleMoveButton(true);
				}
				GoodsShortcutRowWidget->SetParentUI(this);

				if (SB_ShortcutPage.IsValid())
					SB_ShortcutPage->AddChild(GoodsShortcutRowWidget);
			}
 		}
	}
	else
	{
		VB_ShortcutPage->SetVisibility(ESlateVisibility::Visible);

		for (auto& shortcutInfo : _GoodsShortcutInfo->ShortcutInfo)
		{
			UB2UIGoodsShortcutParts* const GoodsShortcutRowWidget = CreateWidget<UB2UIGoodsShortcutParts>(GetOwningPlayer(), UIP_Shortcut_Button);
			if (GoodsShortcutRowWidget != nullptr)
			{
				GoodsShortcutRowWidget->Init();
				GoodsShortcutRowWidget->SetGoodsShortcutInfo(shortcutInfo.ShortcutUIPStringID, 
					shortcutInfo.ShortcutSceneName, 
					shortcutInfo.ShortcutScene_Option1, 
					shortcutInfo.ShortcutScene_Option2);

				if (shortcutInfo.ShortcutSceneName == EUIScene::HallOfFame && B2P_IsInReview())
				{
					GoodsShortcutRowWidget->SetVisibleButton(false);
				}
				else
				{
					GoodsShortcutRowWidget->SetVisibleMoveButton(true);
				}

				GoodsShortcutRowWidget->SetParentUI(this);

				if (VB_ShortcutPage.IsValid())
					VB_ShortcutPage->AddChild(GoodsShortcutRowWidget);
			}
		}
	}
}

void UB2UIGoodsShortcutTooltipPopup::addRowOpenStage(const struct FSingleGoodsTypeInfoData*  _GoodsShortcutInfo, TArray<bool> _IsContentsOpened)
{
	if (SB_ShortcutPage.IsValid())
		SB_ShortcutPage->ClearChildren();
	if (CP_ShortcutPage.IsValid())
		CP_ShortcutPage->SetVisibility(ESlateVisibility::Collapsed);
	if (VB_ShortcutPage.IsValid())
	{
		VB_ShortcutPage->ClearChildren();
		VB_ShortcutPage->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	auto& StageDataStore = BladeIIGameImpl::GetStageDataStore();
	const uint8 shortcutinfoNum(_GoodsShortcutInfo->ShortcutInfo.Num());
	const uint8 MaxUIPNum(4);
	if (shortcutinfoNum > MaxUIPNum)
	{
		CP_ShortcutPage->SetVisibility(ESlateVisibility::Visible);

		for (int i = 0; i < _GoodsShortcutInfo->ShortcutInfo.Num(); i++)
		{
			FServerStageID ServerStageID = FServerStageID(_GoodsShortcutInfo->ShortcutInfo[i].ShortcutScene_Option1);
			if (ServerStageID != 0)
			{
				int32 Difficuly = static_cast<int32>(StageDataStore.GetStageDifficulty(ServerStageID));
				if (Difficuly > BladeIIGameImpl::GetClientDataStore().GetLimitOpenStageDifficulty())
					continue;
			}

			UB2UIGoodsShortcutParts* const GoodsShortcutRowWidget = CreateWidget<UB2UIGoodsShortcutParts>(GetOwningPlayer(), UIP_Shortcut_Button);
			if (GoodsShortcutRowWidget != nullptr)
			{
				GoodsShortcutRowWidget->Init();
				GoodsShortcutRowWidget->SetGoodsShortcutInfo(_GoodsShortcutInfo->ShortcutInfo[i].ShortcutUIPStringID, 
					_GoodsShortcutInfo->ShortcutInfo[i].ShortcutSceneName, 
					_GoodsShortcutInfo->ShortcutInfo[i].ShortcutScene_Option1,
					_GoodsShortcutInfo->ShortcutInfo[i].ShortcutScene_Option2);		

				if (_GoodsShortcutInfo->FGoodsType.Compare(GoodsTypeFName::GOLD) == 0 ||
					_GoodsShortcutInfo->FGoodsType.Compare(GoodsTypeFName::FRIENDSHIP_POINT) == 0)
				{
					GoodsShortcutRowWidget->SetVisibleMoveButton(true);
				}
				else
				{
					GoodsShortcutRowWidget->SetVisibleMoveButton(_IsContentsOpened.IsValidIndex(i) ? _IsContentsOpened[i] : false);
				}
				GoodsShortcutRowWidget->SetParentUI(this);

				if (SB_ShortcutPage.IsValid())
					SB_ShortcutPage->AddChild(GoodsShortcutRowWidget);
			}
		}		
	}
	else
	{
		VB_ShortcutPage->SetVisibility(ESlateVisibility::Visible);

		for (int i = 0; i < _GoodsShortcutInfo->ShortcutInfo.Num(); i++)
		{
			FServerStageID ServerStageID = FServerStageID(_GoodsShortcutInfo->ShortcutInfo[i].ShortcutScene_Option1);
			if (ServerStageID != 0)
			{
				int32 Difficuly = static_cast<int32>(StageDataStore.GetStageDifficulty(ServerStageID));
				if (Difficuly > BladeIIGameImpl::GetClientDataStore().GetLimitOpenStageDifficulty())
					continue;
			}

			UB2UIGoodsShortcutParts* const GoodsShortcutRowWidget = CreateWidget<UB2UIGoodsShortcutParts>(GetOwningPlayer(), UIP_Shortcut_Button);
			if (GoodsShortcutRowWidget != nullptr)
			{
				GoodsShortcutRowWidget->Init();
				GoodsShortcutRowWidget->SetGoodsShortcutInfo(_GoodsShortcutInfo->ShortcutInfo[i].ShortcutUIPStringID,
					_GoodsShortcutInfo->ShortcutInfo[i].ShortcutSceneName, 
					_GoodsShortcutInfo->ShortcutInfo[i].ShortcutScene_Option1, 
					_GoodsShortcutInfo->ShortcutInfo[i].ShortcutScene_Option2);

				if (_GoodsShortcutInfo->FGoodsType.Compare(GoodsTypeFName::GOLD) == 0 || 
					_GoodsShortcutInfo->FGoodsType.Compare(GoodsTypeFName::FRIENDSHIP_POINT) == 0)
				{
					GoodsShortcutRowWidget->SetVisibleMoveButton(true);
				}
				else
				{
					GoodsShortcutRowWidget->SetVisibleMoveButton(_IsContentsOpened.IsValidIndex(i) ? _IsContentsOpened[i] : false);
				}
				GoodsShortcutRowWidget->SetParentUI(this);

				if (VB_ShortcutPage.IsValid())
					VB_ShortcutPage->AddChild(GoodsShortcutRowWidget);
			}
		}
	}
}

void UB2UIGoodsShortcutTooltipPopup::AllTextBoxHide()
{
	if(O_ShortcutTooltip.IsValid())
		O_ShortcutTooltip->SetVisibility(ESlateVisibility::Collapsed);
	if(O_InfoTooltip.IsValid())
		O_InfoTooltip->SetVisibility(ESlateVisibility::Collapsed);
	if (BTN_Close.IsValid())
		BTN_Close->SetVisibility(ESlateVisibility::Collapsed);
	/*
	if (TB_Shortage.IsValid())
		TB_Shortage->SetVisibility(ESlateVisibility::Hidden);
	if (TB_GoodsName.IsValid())
		TB_GoodsName->SetVisibility(ESlateVisibility::Hidden);
	if (TB_GoodsInfo.IsValid())
		TB_GoodsInfo->SetVisibility(ESlateVisibility::Hidden);
	*/
}


FSingleGoodsTypeInfoData* UB2UIGoodsShortcutTooltipPopup::GetGoodsTypeFromGoodsTypeInfoObject(FName _GoodsTypeInfoID) const
{
	return StaticFindGoodsTypeInfoTable() != NULL ? StaticFindGoodsTypeInfoTable()->GetInfoData(_GoodsTypeInfoID) : nullptr;
}

void UB2UIGoodsShortcutTooltipPopup::DestroySelfUsingUIManager()
{
	//B2AndroidBackManager::GetInstance()->RemoveBackWidget(this);
	//ForcedCloseToolTipPopupClass<>::GetInstance().UnsubscribeAll();
	//DestroySelf(UB2UIManager::GetInstance());
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Delegate or Event

void UB2UIGoodsShortcutTooltipPopup::CloseWidgetDelegate()
{
	if (BTN_Close.IsValid() && BTN_Close->GetVisibility() == ESlateVisibility::Visible)
	{
		OnClickBtnClose();
	}
	else
	{
		OnClickBtnModal();
	}
}

void UB2UIGoodsShortcutTooltipPopup::BindDelegates()
{
	Super::BindDelegates();
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIGoodsShortcutTooltipPopup::OnClickBtnClose);
	BIND_CLICK_FUNC_TO_BTN(BTN_Modal, &UB2UIGoodsShortcutTooltipPopup::OnClickBtnModal);
}

void UB2UIGoodsShortcutTooltipPopup::OnClickBtnClose()
{
	//// [반복전투 / 연속전투] 에서 스테이지 진입에 필요한 재화가 부족하면 전투 및 카운트다운을 중단한다
	//if ( m_type == EGoodsButtonType::EGoodsButtonType_ShortageShortcut && AB2StageManager::GetCacheStageKeepEssentialData().GetRepeatBattleState().bIsOn )
	//{
	//	CancelOrStopRepeatBattleClass<>::GetInstance().Signal();
	//}

	//ToolTipCloseClass<>::GetInstance().Signal();
	//DestroySelfUsingUIManager();
}

void UB2UIGoodsShortcutTooltipPopup::OnClickBtnModal()
{
	//if(EGoodsButtonType::EGoodsButtonType_Info == m_type)
	//{
	//	ToolTipCloseClass<>::GetInstance().Signal();
	//	DestroySelfUsingUIManager();
	//}
}

void UB2UIGoodsShortcutTooltipPopup::OnForcedCloseToolTipPopup()
{
	OnClickBtnClose();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////