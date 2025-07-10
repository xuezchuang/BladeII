// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UICostumePageStore.h"
#include "B2UICostumeStoreItemSlot.h"
#include "B2ScrollBox.h"
#include "B2UIDocHelper.h"

#include "BladeIIGameImpl.h"
#include "B2UIStore.h"
#include "B2LobbyGameMode.h"
#include "B2UICostumePackageDetail.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "B2LobbyInventory.h"
#include "Slate/SceneViewport.h"
#include "B2GameInstance.h"
//#include "B2IAPGenericPlatform.h"
#include "B2UIUtil.h"
#include "B2RedDotManager.h"
#include "Retailer.h"
#include "B2ScrollBoxWithGridPanel.h"

void UB2UICostumePageStore::Init()
{
	Super::Init();

	//오픈때마다? init마다?
	data_trader::Retailer::GetInstance().RequestGetGeneralShop(false);
}
UMaterialInstance * UB2UICostumePageStore::GetProductIcon(int32 PackageID, int32 OptionalItemId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductIcon(PackageID, OptionalItemId);

	return nullptr;
}
FText UB2UICostumePageStore::GetProductName(int32 PackageID, int32 OptionalItemId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductName(PackageID, OptionalItemId);

	return FText::FromString(TEXT("Unknown Product"));
}

FText UB2UICostumePageStore::GetProductDescription(int32 PackageID)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductDescription(PackageID);

	return FText::FromString(TEXT("Unknown Product"));
}

void UB2UICostumePageStore::CacheAssets()
{
	GET_SLOT(UB2ScrollBoxWithGridPanel, UIP_ScrollBoxWithGridPanelBP);
	if (UIP_ScrollBoxWithGridPanelBP.IsValid())
	{
		UIP_ScrollBoxWithGridPanelBP->Init();
	}
}

void UB2UICostumePageStore::ConditionalCreateGuestWarningPopup()
{
	if (GuestWarningPopup == nullptr)
		GuestWarningPopup = B2UIUtil::OpenGuestWarningPopup(this);

	if (GuestWarningPopup != nullptr)
	{
		BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(GuestWarningPopup->GetSlot<UB2Button>(TEXT("BTN_Cancel"))), &UB2UICostumePageStore::CloseGuestWarningPopup);
		BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(GuestWarningPopup->GetSlot<UB2Button>(TEXT("BTN_Buy"))), &UB2UICostumePageStore::BuyGuestWarningPopup);
		GuestWarningPopup->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UB2UICostumePageStore::DestroyGuestWarningPopup()
{
	if (GuestWarningPopup)
	{
		GuestWarningPopup->ClosePopup();
		GuestWarningPopup->DestroySelf(UB2UIManager::GetInstance());
		GuestWarningPopup = nullptr;
	}
}

void UB2UICostumePageStore::OpenSuccessBuyPopUp()
{
	UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bought_ClosetProduct")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm
		);
}

void UB2UICostumePageStore::OpenFailBuyPopUp(bool bIsInvenFull)
{
	if (bIsInvenFull)
	{
		UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("EquipErrorInvenFull")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
			);
	}
	else
	{
		UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
			BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Failed_Product")),
			0.f,
			true,
			true,
			EUIMsgPopupButtonGroup::Confirm
			);
	}
}

void UB2UICostumePageStore::OpenGuestWarningPopup()
{
	ConditionalCreateGuestWarningPopup();

	if (GuestWarningPopup)
	{
		GuestWarningPopup->OpenPopup();
	}
	ClosePackageDetailPopup();
}

void UB2UICostumePageStore::CloseGuestWarningPopup()
{
	if (GuestWarningPopup)
	{
		GuestWarningPopup->ClosePopup();
	}
}

void UB2UICostumePageStore::BuyGuestWarningPopup()
{
	CloseGuestWarningPopup();
	RequestCashPurchase(StoreProductID);
}

void UB2UICostumePageStore::RemoveAllChild()
{
	if (UIP_ScrollBoxWithGridPanelBP.IsValid() == false)
	{
		return;
	}

	UIP_ScrollBoxWithGridPanelBP->SetUniformGridPanelClearChildren();
}

void UB2UICostumePageStore::RequestCashPurchase(int32 ProductID)
{
	//B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(ProductID);
	//if (ProductInfo)
	//	B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->RequestPreparePurchase(ProductID, ProductInfo->MarketProductId);
}

void UB2UICostumePageStore::OpenPackageDetailPopUp(int32 ProductID)
{
	float fPoupSizeX = 0.f;

	const float fConvertScale = 1.f / UWidgetLayoutLibrary::GetViewportScale(this);

	FDisplayMetrics Metrics;
	FSlateApplication::Get().GetDisplayMetrics(Metrics);
	FVector2D vSafePaddingSize(Metrics.TitleSafePaddingSize.X, Metrics.ActionSafePaddingSize.Y);

	vSafePaddingSize *= fConvertScale;

	FVector2D vPopupPostion(vSafePaddingSize.X, 0.0f);

	if (UGameViewportClient* ViewportClient = this->GetWorld()->GetGameViewport())
	{
		if (FSceneViewport* SceneViewport = ViewportClient->GetGameViewport())
		{
			fPoupSizeX = SceneViewport->GetCachedGeometry().GetLocalSize().X * fConvertScale * 0.5;
			fPoupSizeX -= vSafePaddingSize.X;
		}
	}

	vPopupPostion.X += fPoupSizeX;
	
	if (UB2UIManager* pUIManager = UB2UIManager::GetInstance())
	{
		ClosePackageDetailPopup();
		CostumePacakageDetail = pUIManager->OpenUI<UB2UICostumePackageDetail>(UIFName::CostumePackageDetailPopup);
		if (CostumePacakageDetail != nullptr)
		{
			CostumePacakageDetail->SetPosition(vPopupPostion, fPoupSizeX);
			const FStoreProductData* StoreProductData = BladeIIGameImpl::GetClientDataStore().GetStoreProductData(ProductID);
			if (StoreProductData != nullptr)
			{
				int32 PackageID = StoreProductData->Product.ProductId;
				CostumePacakageDetail->SetPackageData(ProductID,GetProductName(PackageID, PackageID),GetProductDescription(PackageID));
			}
		}
	}
}

void UB2UICostumePageStore::ClosePackageDetailPopup()
{
	if (CostumePacakageDetail != nullptr)
	{
		CostumePacakageDetail->CloseSelf();
		CostumePacakageDetail = nullptr;
	}
}

void UB2UICostumePageStore::RefreshPackageDetailPopup()
{
	//if (CostumePacakageDetail != nullptr && !CostumePacakageDetail->IsPendingKill())
		//CostumePacakageDetail->RefreshData();
}

void UB2UICostumePageStore::ProcessPurchase(int32 ProductID)
{
	const FStoreProductData* StoreProductData = BladeIIGameImpl::GetClientDataStore().GetStoreProductData(ProductID);
	if (StoreProductData == nullptr)
	{
		ClosePackageDetailPopup();
		OpenFailBuyPopUp();
		return;
	}
		
	
	int32 PackageID = StoreProductData->Product.ProductId;
	TArray<FPackageProductData> PackageProductDatas;
	BladeIIGameImpl::GetClientDataStore().GetPackageProductList(PackageID, PackageProductDatas);
	
	StoreProductID = INDEX_NONE;

	//코스튬 인벤 개수 제한
	AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer()));
	auto* CachedInventory = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
	if (CachedInventory)
	{
		bool bIsFull = CachedInventory->GetStoredCostomeItemNumInUnitedSlot(SelectedPCClass) > BladeIIGameImpl::GetClientDataStore().GetMaxCostumeInventorySlotCount();
		if (bIsFull)
		{
			ClosePackageDetailPopup();
			OpenFailBuyPopUp(true);
			return;
		}
	}

	EStoreItemCost CurrentCostType = static_cast<EStoreItemCost>(StoreProductData->CostType);
	
	if (CurrentCostType == EStoreItemCost::Cash)
	{
		if (B2P_GetKakaoIDPCode() == EPlatformIDPCode::Guest)
		{
			StoreProductID = ProductID;
			OpenGuestWarningPopup();
		}
		else
		{
			RequestCashPurchase(ProductID);
		}
	}
	else
	{
		ClosePackageDetailPopup();

		if (B2UIUtil::CheckPayableStoreProduct(StoreProductData))
			data_trader::Retailer::GetInstance().RequestBuyShopProduct(ProductID);
		else
			B2UIUtil::OpenCostTypeErrorPopup(CurrentCostType);
	}
}

bool UB2UICostumePageStore::CheckPackageInClass(EPCClass CurrentClass, const FStoreProductData & StoreProductData)
{
	TArray<FPackageProductData> OutList;
	BladeIIGameImpl::GetClientDataStore().GetPackageProductList(StoreProductData.Product.ProductId, OutList);

	if (OutList.Num() > 0)
	{
		EPCClass ItemPCClass = FItemRefIDHelper::ExtractItemAllowedPCClassFromRefID(OutList[0].PackageProductId);
		return ItemPCClass == CurrentClass;
	}
	return true;
}

void UB2UICostumePageStore::FilterAndSortPackageList(EPCClass InCurrentClass, TArray<FStoreProductData>& FilterList)
{
	//Filter Class
	FilterList.RemoveAll([this, InCurrentClass](const FStoreProductData& ProductData)
	{
		return (this->CheckPackageInClass(InCurrentClass, ProductData) == false);
	});

	////Filter Market
	//FilterList.RemoveAll([this](const FStoreProductData& ProductData)
	//{
	//	if (ProductData.CostType == static_cast<int32>(EStoreItemCost::Gem))
	//		return false;
	//	else
	//		return (B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(ProductData.StoreProductId) == nullptr);
	//});

	//Filter Period
	UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore != nullptr)
	{
		FilterList.RemoveAll([this, &DocStore](const FStoreProductData& ProductData)
		{
			if (DocStore->IsPeriodicPackage(ProductData.Product.ProductId))
			{
				int64 EndTime = DocStore->GetReceivedPeriodicPackageLeftTime(ProductData.Product.ProductId);
				FTimespan RemainTime = FDateTime::FromUnixTimestamp(EndTime / 1000) - UB2GameInstance::GetUTCNow();
				return RemainTime.GetSeconds() <= 0;
			}
			return false;
		});
	}

	//Sort
	FilterList.Sort([](const FStoreProductData& Data1, const FStoreProductData& Data2)->bool {
		return Data1.GoodsCategoryOrder < Data2.GoodsCategoryOrder;
	});

	//limit
	if (DocStore != nullptr)
	{
		FilterList.StableSort([&DocStore](const FStoreProductData& Data1, const FStoreProductData& Data2)->bool {
			bool LimitData1 = DocStore->IsPeriodicPackage(Data1.Product.ProductId);
			bool LimitData2 = DocStore->IsPeriodicPackage(Data2.Product.ProductId);
			return !LimitData1 && LimitData2;
		});
	}
}

void UB2UICostumePageStore::CompletedPurchaseBuyCash(bool success, const FB2ResponseBuyShopResultInfoPtr GetProduct)
{
	if (success)
	{
		BladeIIGameImpl::GetClientDataStore().ResponseBuyShopResultInfo(GetProduct);
		RefreshPage();
		OpenSuccessBuyPopUp();
		RefreshPackageDetailPopup();
		BladeIIGameImpl::GetRedDotManager().RequestCheckRedDot({ RedDotHint::RED_DOT_EQUIP });
	}
	else
	{
		OpenFailBuyPopUp();
	}
}

void UB2UICostumePageStore::OnOpen(bool RightNow)
{
	BindDocAuto();
	Super::OnOpen(RightNow);

	Issues.Add(LobbySetHeroMgmtModePCSelectionClass<EPCClass>::GetInstance().Subscribe2(
		[this](EPCClass SelectionPCClass)
	{
		ClosePackageDetailPopup();
		this->RefreshPage();
	}
	));

	Issues.Add(SelectCostumePackageClass<int32>::GetInstance().Subscribe2(
		[this](int32 ProductID)
	{
		this->OpenPackageDetailPopUp(ProductID);
	}
	));

	Issues.Add(SelectItemToBuyClass<int32>::GetInstance().Subscribe2(
		[this](int32 ProductID)
	{
		this->ProcessPurchase(ProductID);
	}
	));
	
	Issues.Add(DeliveryBuyShopProductClass<FB2ResponseBuyShopProductPtr>::GetInstance().Subscribe2(
		[this](const FB2ResponseBuyShopProductPtr& BuyShopResult) 
	{
		this->RefreshPage();
		OpenSuccessBuyPopUp();
		RefreshPackageDetailPopup();
		BladeIIGameImpl::GetRedDotManager().RequestCheckRedDot({ RedDotHint::RED_DOT_EQUIP });
	}
	));

	//B2InAppPurchase::IB2IAPGenericPlatform::OnPurchaseResultDelegate.BindLambda([this](bool success, b2network::B2BuyShopResultInfoPtr GetProduct) {
	//	//UE_LOG(LogBladeII, Warning, TEXT("@@@@@@@ OnPurchaseResultDelegate @@@@@@@>> %d"), success);
	//	CompletedPurchaseBuyCash(success, GetProduct);
	//});

	RefreshPage();
}

void UB2UICostumePageStore::OnClose(bool RightNow)
{
	UnbindDoc();
	Issues.Empty();

	OnClose_BP();
}

void UB2UICostumePageStore::BindDocAuto()
{
	UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore != nullptr)
	{
		SetDoc(DocStore);
		RefreshHandler = DocStore->OnGeneralShopChanged.AddUObject(this, &UB2UICostumePageStore::RefreshPage);
	}
}

void UB2UICostumePageStore::UnbindDoc()
{
	UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore != nullptr)
		DocStore->OnGeneralShopChanged.Remove(RefreshHandler);
}

void UB2UICostumePageStore::RefreshPage()
{
	if (!IsVisible())
		return;

	UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore == nullptr)
		return;

	if (!UIP_ScrollBoxWithGridPanelBP.IsValid())
		return;

	RemoveAllChild();

	SelectedPCClass = EPCClass::EPC_End;
	if (AB2LobbyGameMode* pLobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetOwningPlayer())))
		SelectedPCClass = pLobbyGameMode->GetHeroMgmtModePCClass();

	UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(UIP_ScrollBoxWithGridPanelBP->GetGridPanelSlot());
	if (ScrollBoxSlot != nullptr)
		ScrollBoxSlot->SetPadding(FMargin(SlotPadding.X, SlotPadding.Y, 0.0f, 0.0f));
	UIP_ScrollBoxWithGridPanelBP->SetUniformGridPanelSlotPadding(FMargin(0.0f, 0.0f, SlotPadding.X, SlotPadding.Y));
	UIP_ScrollBoxWithGridPanelBP->SetUniformGridPanelMinDesiredSlotHeight(IconSizeY);

	TArray<FStoreProductData> PackageProductDatas;
	BladeIIGameImpl::GetClientDataStore().GetPackageList(b2network::B2ShopPackageTabType::COSTUME, PackageProductDatas);

	FilterAndSortPackageList(SelectedPCClass, PackageProductDatas);

	int32 SlotIndex = 0;
	int32 count = 0;
	for (; count < PackageProductDatas.Num(); count++)
	{
		const FStoreProductData& ProductData = PackageProductDatas[count];

		int64 StartTime = 0;
		int64 EndTime = 0;
		bool IsLimitedPackage = DocStore->GetProductSalePeriod(ProductData.StoreProductId, StartTime, EndTime);

		FDateTime StartDateTime = FDateTime::FromUnixTimestamp(StartTime / 1000);
		FDateTime EndDateTime = FDateTime::FromUnixTimestamp(EndTime / 1000);

		if(IsLimitedPackage && !UB2GameInstance::IncludeTimeNow(StartTime, EndTime))
			continue;

		UB2UICostumeStoreItemSlot* NewCostumeItemSlot = CreateWidget<UB2UICostumeStoreItemSlot>(GetWorld(), CostumeSlotClass);
		UScaleBox* NewScaleBox = NewObject<UScaleBox>(this, NAME_None, RF_Transient);
		if(NewCostumeItemSlot == nullptr || NewScaleBox == nullptr)
			continue;

		int32 PackageID = ProductData.Product.ProductId;

		NewCostumeItemSlot->Init();
		NewCostumeItemSlot->SetCostumPackageData(ProductData, GetProductName(PackageID, PackageID));
		NewCostumeItemSlot->SetIcon(GetProductIcon(PackageID, PackageID));

		UScaleBoxSlot* ScaleBoxSlot = Cast<UScaleBoxSlot>(NewScaleBox->AddChild(NewCostumeItemSlot));
		if (ScaleBoxSlot != nullptr)
		{
			ScaleBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			ScaleBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		}

		UUniformGridSlot* UniformGridSlot = UIP_ScrollBoxWithGridPanelBP->AddChildToUniformGridSlot(NewScaleBox);
		if (UniformGridSlot != nullptr)
		{
			UniformGridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			UniformGridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			UniformGridSlot->SetRow((int32)(SlotIndex / RowSlotCount));
			UniformGridSlot->SetColumn(SlotIndex % RowSlotCount);
		}
		
		SlotIndex += 1;

	}
	//그냥 아무거나 채워서 칸맞춤...
	while (++count < RowSlotCount)
	{
		UB2UIWidget* DummyWidget = CreateWidget<UB2UIWidget>(GetWorld());
		UUniformGridSlot* UniformGridSlot = UIP_ScrollBoxWithGridPanelBP->AddChildToUniformGridSlot(DummyWidget);
		if (UniformGridSlot != nullptr)
		{
			UniformGridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			UniformGridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			UniformGridSlot->SetRow((int32)(count / RowSlotCount));
			UniformGridSlot->SetColumn(count % RowSlotCount);
		}
	}
}

#if WITH_EDITOR
void UB2UICostumePageStore::AddDevSlot(int32 AddCount)
{
	TArray<FStoreProductData> PackageProductDatas;
	BladeIIGameImpl::GetClientDataStore().GetPackageList(b2network::B2ShopPackageTabType::COSTUME, PackageProductDatas);

	if (PackageProductDatas.Num() == 0)
		return;

	if (!UIP_ScrollBoxWithGridPanelBP.IsValid())
		return;

	int32 CurrentCount = UIP_ScrollBoxWithGridPanelBP->GetChildrenCount();
	AddCount += CurrentCount;

	for (; CurrentCount < AddCount; ++CurrentCount)
	{
		const FStoreProductData& ProductData = PackageProductDatas[0];

		UB2UICostumeStoreItemSlot* NewCostumeItemSlot = CreateWidget<UB2UICostumeStoreItemSlot>(GetWorld(), CostumeSlotClass);
		UScaleBox* NewScaleBox = NewObject<UScaleBox>(this, NAME_None, RF_Transient);
		if (NewCostumeItemSlot == nullptr || NewScaleBox == nullptr)
			continue;

		int32 PackageID = ProductData.Product.ProductId;

		NewCostumeItemSlot->Init();
		NewCostumeItemSlot->SetCostumPackageData(ProductData, GetProductName(PackageID, PackageID));
		NewCostumeItemSlot->SetIcon(GetProductIcon(PackageID, PackageID));

		UScaleBoxSlot* ScaleBoxSlot = Cast<UScaleBoxSlot>(NewScaleBox->AddChild(NewCostumeItemSlot));
		if (ScaleBoxSlot != nullptr)
		{
			ScaleBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			ScaleBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
		}

		UUniformGridSlot* UniformGridSlot = UIP_ScrollBoxWithGridPanelBP->AddChildToUniformGridSlot(NewScaleBox);
		if (UniformGridSlot != nullptr)
		{
			UniformGridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			UniformGridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			UniformGridSlot->SetRow((int32)(CurrentCount / RowSlotCount));
			UniformGridSlot->SetColumn(CurrentCount % RowSlotCount);
		}
	}
	//그냥 아무거나 채워서 칸맞춤...
	while (++CurrentCount < RowSlotCount)
	{
		UB2UIWidget* DummyWidget = CreateWidget<UB2UIWidget>(GetWorld());
		UUniformGridSlot* UniformGridSlot = UIP_ScrollBoxWithGridPanelBP->AddChildToUniformGridSlot(DummyWidget);
		if (UniformGridSlot != nullptr)
		{
			UniformGridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			UniformGridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			UniformGridSlot->SetRow((int32)(CurrentCount / RowSlotCount));
			UniformGridSlot->SetColumn(CurrentCount % RowSlotCount);
		}
	}
}
#endif