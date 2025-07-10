#include "B2UIMailSelectiveLotteryResult.h"
#include "B2DynItemIcon.h"
#include "B2DynItemIcon_ItemOpScene.h"
#include "B2LobbyUI_ItemDetailPostOp.h"


#include "B2UIManager.h"
#include "B2UIMail.h"

//대부분의 코드는 UB2LobbyUI_ItemLevelupProg 로부터 차용.

void UB2UIMailSelectiveLotteryResult::CacheAssets()
{
	GET_SLOT(UCanvasPanel, CP_TargetItemIconSet);
	GET_SLOT(UCanvasPanel, CP_EnhanceIngredIconPanel);
	GET_SLOT(UB2LobbyUI_ItemDetailPostOp, ItemDetailPopup);
}

void UB2UIMailSelectiveLotteryResult::BindDelegates()
{
	if (ItemDetailPopup.IsValid())
	{
		ItemDetailPopup->OnConfirmButtonClickedDelegate.RemoveAll(this);
		ItemDetailPopup->OnConfirmButtonClickedDelegate.AddDynamic(this, &UB2UIMailSelectiveLotteryResult::FinishAndProceedToResult);
	}
}

void UB2UIMailSelectiveLotteryResult::OnOpen(bool RightNow)
{
	Super::OnOpen(RightNow);

	if (CP_TargetItemIconSet.IsValid())
		HACKGetWidgetScreenCoord(CP_TargetItemIconSet.Get(), GetOwningPlayer(), TargetItemIconScreenPos, true);
}

const FName UB2UIMailSelectiveLotteryResult::Name_FxMID_ItemBGPanel(TEXT("MAT_ItemBGPanel"));
const FName UB2UIMailSelectiveLotteryResult::Name_FxMID_ItemIcon(TEXT("MAT_ItemIcon"));

UMaterialInstanceDynamic* UB2UIMailSelectiveLotteryResult::GetFxMID_ItemBGPanel(class UParticleSystemComponent* InPSComp)
{
	return GetFxNamedMID(InPSComp, Name_FxMID_ItemBGPanel);
}

UMaterialInstanceDynamic* UB2UIMailSelectiveLotteryResult::GetFxMID_ItemIcon(class UParticleSystemComponent* InPSComp)
{
	return GetFxNamedMID(InPSComp, Name_FxMID_ItemIcon);
}

void UB2UIMailSelectiveLotteryResult::OpenPopup(const FB2Item& InItem)
{
	//아이템 강화 카메라로 이동
	LobbyEnterItemOpDirectingViewClass<ELobbyInvenItemOpMode>::GetInstance().Signal(ELobbyInvenItemOpMode::EIVIOP_None);

	auto* UIManager = UB2UIManager::GetInstance();
	if (!UIManager)
		return;

	auto* MailUIScene = UIManager->GetUISceneData(EUIScene::Mail);
	if (MailUIScene)
	{
		for (FName& UIWidgetName : MailUIScene->WidgetFNames)
		{
			auto* UIWidgetData = UIManager->GetWidgetData(UIWidgetName);
			if (UIWidgetData && UIWidgetData->Widget)
				UIWidgetData->Widget->SetVisibility(ESlateVisibility::Hidden);
		}
	}		
	else
		return;

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	DestroyTargetItemIcon();

	if (ItemIconWidgetClass == NULL || !CP_TargetItemIconSet.IsValid() || InItem.InstanceUID <= 0 || InItem.ItemRefID <= 0) {
		return;
	}

	// Anchor 설정에 따라 GetSize 는 원하는 값이 안 나올 것이므로 주의.
	UCanvasPanelSlot* MainPanelSlot = Cast<UCanvasPanelSlot>(CP_TargetItemIconSet->Slot);
	FVector2D AllowedIconSize = MainPanelSlot ? MainPanelSlot->GetSize() : FVector2D(0.0f, 0.0f);

	UB2DynItemIcon* DynIconCDO = Cast<UB2DynItemIcon>(ItemIconWidgetClass->GetDefaultObject());

	CreatedTargetItemIcon = Cast<UB2DynItemIcon_ItemOpScene>(DynCreateInCanvasPanel(
		ItemIconWidgetClass, this, CP_TargetItemIconSet.Get(), DynIconCDO ? DynIconCDO->GetNonScaledSize() : FVector2D(100.0f, 100.0f), AllowedIconSize,
		FVector2D(0.0f, 0.0f), 0, false
	));

	if (CreatedTargetItemIcon)
	{		
		CreatedTargetItemIcon->SetIconUsageType(ELobbyInvenItemIconUsage::EEIIT_ItemOpModeMenuTarget);
		CreatedTargetItemIcon->UpdateItemData(InItem);
	}

	if (ItemDetailPopup.IsValid())
	{
		ItemDetailPopup->UpdateItemData(InItem, InItem);
		ItemDetailPopup->SetVisibility(ESlateVisibility::Hidden); // 처음에는 숨겨놓음. 연출 최종 단계로서 보여진다.
	}

	StartDirecting();
}

void UB2UIMailSelectiveLotteryResult::DestroyTargetItemIcon()
{
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->DestroySelf();
		CreatedTargetItemIcon = NULL;
	}
}

void UB2UIMailSelectiveLotteryResult::DestroyOverallBGFx()
{
	if (CreatedOverallBGFx)
	{
		CreatedOverallBGFx->DeactivateSystem();
		CreatedOverallBGFx->ConditionalBeginDestroy();
		CreatedOverallBGFx = NULL;
	}
}

void UB2UIMailSelectiveLotteryResult::DestroyEnhanceBlowupFx()
{
	if (CreatedEnhanceBlowupFx)
	{
		CreatedEnhanceBlowupFx->DeactivateSystem();
		CreatedEnhanceBlowupFx->ConditionalBeginDestroy();
		CreatedEnhanceBlowupFx = NULL;
	}
}

void UB2UIMailSelectiveLotteryResult::DestroyTargetItemBGFx()
{
	if (CreatedTargetItemBGFx)
	{
		CreatedTargetItemBGFx->DeactivateSystem();
		CreatedTargetItemBGFx->ConditionalBeginDestroy();
		CreatedTargetItemBGFx = NULL;
	}
}

void UB2UIMailSelectiveLotteryResult::DestroyFinalEnhanceSuccessFxComp()
{
	if (CreatedFinalEnhanceSuccessFx)
	{
		CreatedFinalEnhanceSuccessFx->DeactivateSystem();
		CreatedFinalEnhanceSuccessFx->ConditionalBeginDestroy();
		CreatedFinalEnhanceSuccessFx = NULL;
	}
}

void UB2UIMailSelectiveLotteryResult::ShowOverallBGFx()
{
	DestroyOverallBGFx();
	int32 ViewSizeX = (int32)DesignedRefViewSize.X;
	int32 ViewSizeY = (int32)DesignedRefViewSize.Y;
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		OwningPC->GetViewportSize(ViewSizeX, ViewSizeY); // 화면 중앙에 생성
		CreatedOverallBGFx = OverallBGFxSetup.CreateUnderScreen(OwningPC, FVector2D((float)(ViewSizeX / 2), (float)(ViewSizeY / 2)));
	}
}


void UB2UIMailSelectiveLotteryResult::ShowEnhanceBlowupFx()
{
	DestroyEnhanceBlowupFx();
	int32 ViewSizeX = (int32)DesignedRefViewSize.X;
	int32 ViewSizeY = (int32)DesignedRefViewSize.Y;
	APlayerController* OwningPC = GetOwningPlayer();
	if (OwningPC)
	{
		OwningPC->GetViewportSize(ViewSizeX, ViewSizeY); // 화면 중앙에 생성
		CreatedEnhanceBlowupFx = EnhanceBlowupFxSetup.CreateUnderScreen(OwningPC, FVector2D((float)(ViewSizeX / 2), (float)(ViewSizeY / 2)));
	}
}

void UB2UIMailSelectiveLotteryResult::ShowTargetItemBGFx()
{
	DestroyTargetItemBGFx();
	CreatedTargetItemBGFx = TargetItemBGFxSetup.CreateUnderScreen(GetOwningPlayer(), TargetItemIconScreenPos);

	SetupNamedMIDForFxComp(CreatedTargetItemBGFx); // 필요한 MIC 들에서 MID 를 만들어서 사용 가능하도록 세팅.
												   // Fx 쪽에 MID 까지 준비가 되었다면 파트별로 파라미터 값을 가져올 UI 쪽의 MIC 를 가져와서 MID 에 파라미터를 세팅.
	{
		UMaterialInstanceDynamic* ItemBGPanelMID = GetFxMID_ItemBGPanel(CreatedTargetItemBGFx);
		UMaterialInstanceConstant* ItemBGPanelMIC_Ref = CreatedTargetItemIcon ? CreatedTargetItemIcon->GetBGPanelMIC() : NULL;
		SetupMIDNonAtlasParamFromUIMIC(ItemBGPanelMID, ItemBGPanelMIC_Ref);
	}
	{
		UMaterialInstanceDynamic* ItemIconMID = GetFxMID_ItemIcon(CreatedTargetItemBGFx);
		UMaterialInstanceConstant* ItemIconMIC_Ref = CreatedTargetItemIcon ? CreatedTargetItemIcon->GetMainIconMIC() : NULL;
		SetupMIDNonAtlasParamFromUIMIC(ItemIconMID, ItemIconMIC_Ref);
	}
}

void UB2UIMailSelectiveLotteryResult::ShowFinalEnhanceSuccessFx()
{
	DestroyEnhanceBlowupFx();

	DestroyFinalEnhanceSuccessFxComp();
	CreatedFinalEnhanceSuccessFx = FinalEnhanceSuccessFxSetup.CreateUnderScreen(GetOwningPlayer(), TargetItemIconScreenPos);
}

void UB2UIMailSelectiveLotteryResult::SetTargetItemIconVisible(bool bVisible)
{
	if (CreatedTargetItemIcon)
	{
		CreatedTargetItemIcon->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UB2UIMailSelectiveLotteryResult::ShowFinalResultPage()
{
	if (ItemDetailPopup.IsValid())
	{
		ItemDetailPopup->SetVisibility(ESlateVisibility::Visible);
		ItemDetailPopup->OnVisibleUpdate();
	}
}

FVector UB2UIMailSelectiveLotteryResult::GetTargetIconDesiredWorldPos(float Depth)
{
	return GetWorldPosBeneathScreen(GetOwningPlayer(), TargetItemIconScreenPos, Depth);
}

void UB2UIMailSelectiveLotteryResult::FinishAndProceedToResult()
{
	DestroyTargetItemIcon();
	DestroyOverallBGFx();
	DestroyEnhanceBlowupFx();
	DestroyTargetItemBGFx();
	DestroyFinalEnhanceSuccessFxComp();

	auto* UIManager = UB2UIManager::GetInstance();
	if (!UIManager)
		return;

	auto* MailUIScene = UIManager->GetUISceneData(EUIScene::Mail);
	if (MailUIScene)
	{
		for (FName& UIWidgetName : MailUIScene->WidgetFNames)
		{
			auto* UIWidgetData = UIManager->GetWidgetData(UIWidgetName);
			if (UIWidgetData && UIWidgetData->Widget)
				UIWidgetData->Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	SetVisibility(ESlateVisibility::Collapsed);

	LobbyQuitItemOpDirectingViewClass<>::GetInstance().Signal();
}