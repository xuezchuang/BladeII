#include "B2UIStore.h"
#include "B2UIManager.h"
#include "B2UIHeader.h"
#include "B2UIDocHelper.h"
#include "B2ItemInfo.h"
#include "Retailer.h"
#include "B2UIRewardEventSet.h"
#include "B2RedDotManager.h"
#include "B2AndroidBackManager.h"
#include "BladeIIGameImpl.h"
#include "B2UIBackWidgetBase.h"
#include "B2UICommonTab.h"
#include "B2PlatformAPI.h"
////#include "B2IAPGenericPlatform.h"
#include "B2GameInstance.h"

FB2UIStoreProductIconElement* UB2UIStoreProductIconData::GetProductIconData(int32 ProductId)
{
	for (auto& Elem : ProductIconDataArray)
	{
		if (Elem.ProductId == ProductId)
			return &Elem;
	}

	return nullptr;
}

FB2UIStoreProductIconElement* UB2UIStoreProductIconData::GetProductIconData(EPACKAGE_TYPE PakageType)
{
	for (auto& Elem : ProductIconDataArray)
	{
		if (Elem.PackageType == PakageType)
			return &Elem;
	}

	return nullptr;
}

UMaterialInstance* UB2UIStoreProductIconData::GetProductIcon(int32 ProductId, int32 OptionalItemId)
{
	auto* ProdutctIconData = GetProductIconData(ProductId);
	if (ProdutctIconData)
		return ProdutctIconData->ProductIcon;
	else
	{
		auto* ItemInfoTable = StaticFindItemInfo();
		if (ItemInfoTable == nullptr)
			return nullptr;

		return Cast<UMaterialInstance>(ItemInfoTable->GetItemIcon(OptionalItemId));
	}
}

ESTORE_SLOT_EFFECT_TYPE UB2UIStoreProductIconData::GetProductIconEffect(int32 ProductId)
{
	auto* ProdutctIconData = GetProductIconData(ProductId);
	if (ProdutctIconData)
		return ProdutctIconData->IconEffect;

	return ESTORE_SLOT_EFFECT_TYPE::NONE;
}

ESTORE_SLOT_WIDGET_TYPE UB2UIStoreProductIconData::GetProductWidget(int32 ProductId)
{
	auto* ProdutctIconData = GetProductIconData(ProductId);
	if (ProdutctIconData)
		return ProdutctIconData->SlotWidget;

	return ESTORE_SLOT_WIDGET_TYPE::NONE;
}

FText UB2UIStoreProductIconData::GetProductName(int32 ProductId, int32 OptionalItemId)
{
	auto* ProdutctIconData = GetProductIconData(ProductId);
	if (ProdutctIconData)
	{
		switch (ProdutctIconData->LocCategory)
		{
		case EB2LOC_PRODUCTNAME_CAT::GENERAL:
			return BladeIIGetLOCText(B2LOC_CAT_GENERAL, ProdutctIconData->ProductNameKey);

		case EB2LOC_PRODUCTNAME_CAT::STORE:
			return BladeIIGetLOCText(B2LOC_CAT_STORE, ProdutctIconData->ProductNameKey);

		case EB2LOC_PRODUCTNAME_CAT::ITEM_NAME:
			return BladeIIGetLOCText(B2LOC_CAT_ITEMNAME, ProdutctIconData->ProductNameKey);
		}
	}
	else
	{
		auto* ItemInfoTable = StaticFindItemInfo();
		if (ItemInfoTable != nullptr)
			return ItemInfoTable->GetItemName(OptionalItemId);
	}

	return FText::FromString(TEXT("Unknown Product"));
}

FText UB2UIStoreProductIconData::GetProductDescription(int32 ProductId)
{
	auto* ProdutctIconData = GetProductIconData(ProductId);
	if (ProdutctIconData)
	{
		switch (ProdutctIconData->LocCategory)
		{
		case EB2LOC_PRODUCTNAME_CAT::GENERAL:
			return BladeIIGetLOCText(B2LOC_CAT_GENERAL, ProdutctIconData->ProductDescriptionKey);

		case EB2LOC_PRODUCTNAME_CAT::STORE:
			return BladeIIGetLOCText(B2LOC_CAT_STORE, ProdutctIconData->ProductDescriptionKey);

		case EB2LOC_PRODUCTNAME_CAT::ITEM_NAME:
			return BladeIIGetLOCText(B2LOC_CAT_ITEMNAME, ProdutctIconData->ProductDescriptionKey);
		}
	}
	return FText::FromString(TEXT("Unknown Product"));
}

void UB2UIStoreProductIconData::GetBonusIconAndName(EStoreItemType BonusType, UMaterialInstance*& OutBonusIcon, FText& OutBonusName, int32 OptionalItemTemplateId)
{
	for (auto& Elem : BonusIconDataArray)
	{
		if (Elem.BonusType == BonusType)
		{
			if (BonusType == EStoreItemType::Item)
			{
				UB2ItemInfo::GetItemIconAndName(OptionalItemTemplateId, OutBonusIcon, OutBonusName);
			}
			else
			{
				switch (Elem.LocCategory)
				{
				case EB2LOC_PRODUCTNAME_CAT::GENERAL:
					OutBonusName = BladeIIGetLOCText(B2LOC_CAT_GENERAL, Elem.BonusNameKey);
					break;

				case EB2LOC_PRODUCTNAME_CAT::STORE:
					OutBonusName = BladeIIGetLOCText(B2LOC_CAT_STORE, Elem.BonusNameKey);
					break;

				case EB2LOC_PRODUCTNAME_CAT::ITEM_NAME:
					OutBonusName = BladeIIGetLOCText(B2LOC_CAT_ITEMNAME, Elem.BonusNameKey);
					break;
				}

				OutBonusIcon = Elem.BonusIcon;
			}

			break;
		}
	}
}

ESTORE_BUYPOPUP_WIDGET_TYPE UB2UIStoreProductIconData::GetPopupWidgetType(int32 ProductId)
{
	auto* ProdutctIconData = GetProductIconData(ProductId);
	if (ProdutctIconData)
		return ProdutctIconData->PopupType;

	return ESTORE_BUYPOPUP_WIDGET_TYPE::END;
}

EPACKAGE_TYPE UB2UIStoreProductIconData::GetPackageType(int32 ProductId)
{
	auto* ProdutctIconData = GetProductIconData(ProductId);
	if (ProdutctIconData)
		return ProdutctIconData->PackageType;

	return EPACKAGE_TYPE::None;
}

int32 UB2UIStoreProductIconData::GetProductId(EPACKAGE_TYPE PakageType)
{
	auto* ProdutctIconData = GetProductIconData(PakageType);
	if (ProdutctIconData)
		return ProdutctIconData->ProductId;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FText UB2UIStore::GetStoreCategoryName(int32 StoreCategory)
{
	switch (StoreCategory)
	{
	case EStoreCategory::Package:
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("LobbyMain_Package"));

	case EStoreCategory::DarkCrystal:
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_IngredientItem"));

	case EStoreCategory::Goods:
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Item"));

	case EStoreCategory::Fame_Point:
		return BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_FamePoint"));

	default:
		return FText::FromString(TEXT("Unknown Product"));
	}
}

ESTORE_SLOT_WIDGET_TYPE UB2UIStore::GetProductWidget(int32 ProductId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductWidget(ProductId);

	return ESTORE_SLOT_WIDGET_TYPE::NONE;
}

UMaterialInstance* UB2UIStore::GetProductIcon(int32 ProductId, int32 OptionalItemId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductIcon(ProductId, OptionalItemId);

	return nullptr;
}

ESTORE_SLOT_EFFECT_TYPE UB2UIStore::GetProductIconEffect(int32 ProductId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductIconEffect(ProductId);

	return ESTORE_SLOT_EFFECT_TYPE::NONE;
}

FText UB2UIStore::GetProductName(int32 ProductId, int32 OptionalItemId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		return ProductData->GetProductName(ProductId, OptionalItemId);

	return FText::FromString(TEXT("Unknown Product"));
}

void UB2UIStore::GetBonusIconAndName(EStoreItemType BonusType, UMaterialInstance*& OutBonusIcon, FText& OutBonusName, int32 OptionalItemTemplateId)
{
	auto* ProductData = ProductIconData->GetDefaultObject<UB2UIStoreProductIconData>();
	if (ProductData)
		ProductData->GetBonusIconAndName(BonusType, OutBonusIcon, OutBonusName, OptionalItemTemplateId);
}

static UB2UIStoreProductCost* GetCostToBuyUIP(UB2UIWidgetBase* InCostToBuyOwnerUIP)
{
	if (InCostToBuyOwnerUIP)
	{
		// 네이티브 UIWidgetBase 클래스로 그냥 만들어서 쓰는 게 있음.. ㅡㅡ
		return InCostToBuyOwnerUIP->GetSlot<UB2UIStoreProductCost>(TEXT("UIP_CostToBuy"));
	}
	return nullptr;
}

UB2UIStore::UB2UIStore(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CurrentMileageType(0)
{
	Created_UIP_MileageRewardPopup = nullptr;
	Created_UIP_ConfirmToBuyPopup_Gem = nullptr;
	Created_UIP_ConfirmToBuyPopup_Normal = nullptr;
	Created_UIP_MileageGuidePopup = nullptr;
	Created_UIP_GuestWarningPopup = nullptr;
	RewardIconTemplate = nullptr;
}

void UB2UIStore::Init()
{
	CAPTURE_UOBJECT(UB2UIStore);

	SelectItemToBuyTicket = SelectItemToBuyClass<int32>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(SelectItemToBuy, int32 ProductId)
		Capture->OpenPopupConfirmToBuyItem(ProductId);
	END_CAPTURE_OBJECT()
		);

	DeliveryRewardShopMileageClassTicket = DeliveryRewardShopMileageClass<FB2ResponseRewardShopMileagePtr>::GetInstance().Subscribe(
		USE_CAPTURE_OBJECT_AND_TICKET(DeliveryRewardShopMileageClass, FB2ResponseRewardShopMileagePtr ResponseRewardShopMileage)
		Capture->OnResponseRewardShopMileage(ResponseRewardShopMileage);
	END_CAPTURE_OBJECT()
		);

	Super::Init();

	SetLobbyUIHeaderTitle(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_GoodsStore")));

	CreateStoreCategoryTab();
	SetGoodsTabEvent();

	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		SetTab();

		if (MileageChangedHandler.IsValid())
		{
			DocStore->OnMileageChanged.Remove(MileageChangedHandler);
		}
		MileageChangedHandler.Reset();

		MileageChangedHandler = DocStore->OnMileageChanged.AddUObject(this, &UB2UIStore::OnMileageChanged);
	}
}

void UB2UIStore::DestroySelf(class UB2UIManager* InUIManager)
{
	SelectItemToBuyClass<int32>::GetInstance().Unsubscribe(SelectItemToBuyTicket);
	DeliveryRewardShopMileageClass<FB2ResponseRewardShopMileagePtr>::GetInstance().Unsubscribe(DeliveryRewardShopMileageClassTicket);

	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		if (MileageChangedHandler.IsValid())
		{
			DocStore->OnMileageChanged.Remove(MileageChangedHandler);
		}
		MileageChangedHandler.Reset();

		DocStore->OnMileageChanged.Remove(MileageChangedHandler);
	}

	CloseMileageGuidePopup();
	CloseConfirmToBuyPopupGemPopup();
	CloseConfirmToBuyPopupNormalPopup();
	CloseGuestWarningPopup();

	DestroyMileageRewardPopup();
	DestroyConfirmToBuyPopup_Gem();
	DestroyConfirmToBuyPopup_Normal();
	DestroyMileageGuidePopup();
	DestroyGuestWarningPopup();

	Super::DestroySelf(InUIManager);
}

void UB2UIStore::CacheAssets()
{
	// Store Tab
	GET_SLOT(UVerticalBox, VB_TabList);

	// Goods Tab
	GET_SLOT(UPanelWidget, O_GoodsTab);
	GET_SLOT(UB2UICommonTab, GoodsTab_BladePoint);
	if (GoodsTab_BladePoint.IsValid())
		GoodsTab_BladePoint->Init();
	GET_SLOT(UB2UICommonTab, GoodsTab_Gem);
	if (GoodsTab_Gem.IsValid())
		GoodsTab_Gem->Init();
	GET_SLOT(UB2UICommonTab, GoodsTab_Gold);
	if (GoodsTab_Gold.IsValid())
		GoodsTab_Gold->Init();

	// List
	GET_SLOT(UHorizontalBox, HB_ItemList);

	// Mileage
	GET_SLOT(UB2UIStoreMileage, UIP_StoreMileage);
	if (UIP_StoreMileage.IsValid())
		UIP_StoreMileage->Init();

	GET_SLOT(UCanvasPanel, CP_Mileage);

	GET_SLOT(UCanvasPanel, X_CP_MileageRewardPopup_Panel);
	GET_SLOT(UCanvasPanel, X_CP_ConfirmToBuyPopup_Gem_Panel);
	GET_SLOT(UCanvasPanel, X_CP_ConfirmToBuyPopup_Normal_Panel);
	GET_SLOT(UCanvasPanel, X_CP_MileageGuidePopup_Panel);
	GET_SLOT(UCanvasPanel, X_CP_GuestWarningPopup_Panel);

	GET_SLOT(UB2Button, BTN_MileageGuide);
}

void UB2UIStore::UpdateStaticText()
{
	if (GoodsTab_BladePoint.IsValid())
		GoodsTab_BladePoint->SetTabName(BladeIIGetLOCText(FString(B2LOC_CAT_STORE), FString(TEXT("Store_TabCategory_SoulBlade"))));
	if (GoodsTab_Gem.IsValid())
		GoodsTab_Gem->SetTabName(BladeIIGetLOCText(FString(B2LOC_CAT_STORE), FString(TEXT("Store_TabCategory_Gem"))));
	if (GoodsTab_Gold.IsValid())
		GoodsTab_Gold->SetTabName(BladeIIGetLOCText(FString(B2LOC_CAT_STORE), FString(TEXT("Store_TabCategory_Gold"))));
}

void UB2UIStore::BindDelegates()
{
	// Goods Tab
	if (GoodsTab_BladePoint.IsValid())
		REGISTER_UITAB_BINDING(GoodsTab_BladePoint.Get(), &UB2UIStore::OnClickGoodsTabBladePoint);
	if (GoodsTab_Gem.IsValid())
		REGISTER_UITAB_BINDING(GoodsTab_Gem.Get(), &UB2UIStore::OnClickGoodsTabGem);
	if (GoodsTab_Gold.IsValid())
		REGISTER_UITAB_BINDING(GoodsTab_Gold.Get(), &UB2UIStore::OnClickGoodsTabGold);

	// Mileage
	if (UIP_StoreMileage.IsValid())
	{
		auto* BClassRewardBoxBTN = UIP_StoreMileage->GetBoxButtonPtr(EUIStoreMileageGrade::BClass);
		if (BClassRewardBoxBTN)
		{
			BClassRewardBoxBTN->OnClicked.RemoveAll(this);
			BClassRewardBoxBTN->OnClicked.AddDynamic(this, &UB2UIStore::SetOpenRewardPopupBClass);
		}

		auto* AClassRewardBoxBTN = UIP_StoreMileage->GetBoxButtonPtr(EUIStoreMileageGrade::AClass);
		if (AClassRewardBoxBTN)
		{
			AClassRewardBoxBTN->OnClicked.RemoveAll(this);
			AClassRewardBoxBTN->OnClicked.AddDynamic(this, &UB2UIStore::SetOpenRewardPopupAClass);
		}

		auto* SClassRewardBoxBTN = UIP_StoreMileage->GetBoxButtonPtr(EUIStoreMileageGrade::SClass);
		if (SClassRewardBoxBTN)
		{
			SClassRewardBoxBTN->OnClicked.RemoveAll(this);
			SClassRewardBoxBTN->OnClicked.AddDynamic(this, &UB2UIStore::SetOpenRewardPopupSClass);
		}
	}

	BIND_RELEASE_FUNC_TO_BTN(BTN_MileageGuide, &UB2UIStore::OpenMileageGuidePopup);
}

void UB2UIStore::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);

	DeliveryBuyShopProductTicket = DeliveryBuyShopProductClass<FB2ResponseBuyShopProductPtr>::GetInstance().Subscribe(
		[this](const FB2ResponseBuyShopProductPtr& BuyShopResult) {
		this->SetTab();
	});
}

void UB2UIStore::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose(RightNow);

	DeliveryBuyShopProductClass<FB2ResponseBuyShopProductPtr>::GetInstance().Unsubscribe(DeliveryBuyShopProductTicket);
}

void UB2UIStore::RegisterUIMarkForRedDot()
{
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(TEXT("SoulBladeSet")), &UB2UIStore::RedDotCondition_BladePoint);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(TEXT("GemSet")), &UB2UIStore::RedDotCondition_Gem);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(TEXT("GoldSet")), &UB2UIStore::RedDotCondition_Gold);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(TEXT("DarkCrystalSet")), &UB2UIStore::RedDotCondition_DarkCrystal);
	REGISTER_REDDOT_CONDITION(GetRedDotWidget(TEXT("PackageSet")), &UB2UIStore::RedDotCondition_Package);
}

void UB2UIStore::CreateStoreCategoryTab()
{
	if (VB_TabList.IsValid())
	{
		VB_TabList->ClearChildren();

		TMap<int32, int32> StoreCategoryOrder;
		BladeIIGameImpl::GetClientDataStore().GetStoreCategoryOrder(StoreCategoryOrder);

		for (auto& Elem : StoreCategoryOrder)
		{
			auto* CreatedWidget = CreateWidget<class UB2UICommonTab>(GetOwningPlayer(), DefaultStoreTab);
			if (CreatedWidget != NULL)
			{
				int32 CategoryOrderKey = Elem.Key;
				int32 CategoryOrderValue = Elem.Value;
				bool bIsSale = BladeIIGameImpl::GetClientDataStore().GetStoreFundsSaleStoreCategory(CategoryOrderValue);
				CreatedWidget->Init();

				CreatedWidget->SetTabSelected(false);
				CreatedWidget->SetIsTabEnable(true);
				CreatedWidget->SetTabName(GetStoreCategoryName(CategoryOrderValue));
				CreatedWidget->SetTabButtonIndex(CategoryOrderValue);
				CreatedWidget->SetTabEvent(bIsSale);

				//CreatedWidget->SetTabHeadImage();

				CreatedWidget->TabButtonDelegateBindLambda(
					[this, CategoryOrderValue]() { OnClickTabButtonDelegate(CategoryOrderValue);
				});

				VB_TabList->AddChild(CreatedWidget);
			}
		}
	}
}

// Store Tab
void UB2UIStore::OnClickTabButtonDelegate(int32 ButtonIndex)
{
	switch (ButtonIndex)
	{
	case EStoreCategory::Package:
		OnClickTabPackage();
		break;

	case EStoreCategory::DarkCrystal:
		OnClickTabDarkCrystal();
		break;

	case EStoreCategory::Goods:
		OnClickTabGoods();
		break;

	case EStoreCategory::Fame_Point:
		OnClickTabFamePoint();
		break;

	default:
		break;
	}
}

void UB2UIStore::OnClickTabPackage()
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		DocStore->SetStoreTabToOpen(EStoreCategory::Package);
		DocStore->SetStoreGoodsTabToOpen(-1);
	}

	SetTab();
}

void UB2UIStore::OnClickTabDarkCrystal()
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		DocStore->SetStoreTabToOpen(EStoreCategory::DarkCrystal);
		DocStore->SetStoreGoodsTabToOpen(-1);
	}

	SetTab();
}

void UB2UIStore::OnClickTabGoods()
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
		DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::BLADE_POINT);
	}

	SetTab();
}

void UB2UIStore::OnClickTabFamePoint()
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		DocStore->SetStoreTabToOpen(EStoreCategory::Fame_Point);
		DocStore->SetStoreGoodsTabToOpen(-1);
	}

	SetTab();
}

// Goods Tab
void UB2UIStore::SetGoodsTabEvent()
{
	bool bFundsSaleBladePoint = BladeIIGameImpl::GetClientDataStore().GetStoreFundsSaleGoodsCategory(EGoodsCategory::BLADE_POINT);
	bool bFundsSaleGold = BladeIIGameImpl::GetClientDataStore().GetStoreFundsSaleGoodsCategory(EGoodsCategory::GOLD);

	if (GoodsTab_BladePoint.IsValid())
		GoodsTab_BladePoint->SetTabEvent(bFundsSaleBladePoint);
	if (GoodsTab_Gold.IsValid())
		GoodsTab_Gold->SetTabEvent(bFundsSaleGold);

	bool bCashBonusGem = BladeIIGameImpl::GetClientDataStore().GetStoreCashBonusGoodsCategory(EGoodsCategory::GEM);

	if (GoodsTab_Gem.IsValid())
		GoodsTab_Gem->SetTabEvent(bCashBonusGem);
}

void UB2UIStore::OnClickGoodsTabBladePoint()
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
		DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::BLADE_POINT);
	}

	SetTab();
}

void UB2UIStore::OnClickGoodsTabGem()
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
		DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::GEM);
	}

	SetTab();
}

void UB2UIStore::OnClickGoodsTabGold()
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		DocStore->SetStoreTabToOpen(EStoreCategory::Goods);
		DocStore->SetStoreGoodsTabToOpen(EGoodsCategory::GOLD);
	}

	SetTab();
}

// Mileage
void UB2UIStore::OpenMileageGuidePopup()
{
	ConditionalCreateMileageGuidePopup();

	if (Created_UIP_MileageGuidePopup)
	{
		Created_UIP_MileageGuidePopup->OpenPopup();
	}
	SetForcedBlockHeader(true);
}

void UB2UIStore::CloseMileageGuidePopup()
{
	if (Created_UIP_MileageGuidePopup)
	{
		Created_UIP_MileageGuidePopup->ClosePopup();
	}
	SetForcedBlockHeader(false);
}

void UB2UIStore::CloseConfirmToBuyPopupGemPopup()
{
	if (Created_UIP_ConfirmToBuyPopup_Gem)
	{
		Created_UIP_ConfirmToBuyPopup_Gem->ClosePopup();
	}
	SetForcedBlockHeader(false);
}

void UB2UIStore::CloseConfirmToBuyPopupNormalPopup()
{
	if (Created_UIP_ConfirmToBuyPopup_Normal)
	{
		Created_UIP_ConfirmToBuyPopup_Normal->ClosePopup();
	}
	SetForcedBlockHeader(false);
}

void UB2UIStore::SetTab()
{
	auto* DocStore = UB2UIDocHelper::GetDocStore();
	if (DocStore)
	{
		CurrentStoreCategory = DocStore->GetStoreTabToOpen();
		CurrentGoodsCategory = DocStore->GetStoreGoodsTabToOpen();
	}

	SetGoodsCategory(CurrentGoodsCategory);

	SetSelectedStoreCategory(CurrentStoreCategory);
	SetSelectedGoodsCategory(CurrentGoodsCategory);

	SetTabStoreMileage();

	ItemSlots.Empty();

	/*if (HB_ItemList.IsValid())
	{
		HB_ItemList->ClearChildren();

		TArray<FStoreProductData> ProductList;
		BladeIIGameImpl::GetClientDataStore().GetStoreProductList(CurrentStoreCategory, CurrentGoodsCategory, ProductList);

		if (CurrentGoodsCategory == EGoodsCategory::GEM)
		{
			for (auto& Elem : ProductList)
			{
				B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(Elem.StoreProductId);
				if (ProductInfo)
				{
					auto* ProductData = BladeIIGameImpl::GetClientDataStore().GetStoreProductData(ProductInfo->ShopProductId);
					if (ProductData)
						SetProductListItem(ProductData);
				}
			}
		}
		else
		{
			for (int Idx = 0; Idx < ProductList.Num(); Idx++)
			{
				const FStoreProductData* ProductData = &ProductList[Idx];
				SetProductListItem(ProductData);
			}
		}
	}*/

	DoMarkRedDot();
}

void UB2UIStore::SetProductListItem(const FStoreProductData* ProductItem)
{
	auto* CreatedWidget = CreateWidget<UB2UIStoreItemSlot>(GetOwningPlayer(), DefaultItemSlot);
	if (CreatedWidget != NULL)
	{	
		CreatedWidget->Init();

		// Limit Sale
		int64 StartTime = 0;
		int64 EndTime = 0;

		if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
		{
			bool IsLimited = DocStore->GetProductSalePeriod(ProductItem->StoreProductId, StartTime, EndTime);

			if (IsLimited && !UB2GameInstance::IncludeTimeNow(StartTime, EndTime))
				return;

			CreatedWidget->SetSalePeriodData(IsLimited, EndTime);
		}

		// Product
		int32 SlotWidgetNum = static_cast<uint32>(GetProductWidget(ProductItem->StoreProductId));
		int32 SlotIconEffectNum = static_cast<uint32>(GetProductIconEffect(ProductItem->StoreProductId));
		CreatedWidget->CreateSlotWidget(SlotWidgetNum, GetProductIcon(ProductItem->StoreProductId, ProductItem->Product.ProductId), SlotIconEffectNum);
		CreatedWidget->SetData(ProductItem->StoreProductId,
			GetProductIcon(ProductItem->StoreProductId, ProductItem->Product.ProductId),
			GetProductName(ProductItem->StoreProductId, ProductItem->Product.ProductId),
			ProductItem->Product.ProductCount);

		// Bonus
		UMaterialInstance* BonusIcon = nullptr;
		FText BonusName;
		GetBonusIconAndName(static_cast<EStoreItemType>(ProductItem->Bonus.ProductType), BonusIcon, BonusName, ProductItem->Bonus.ProductId);
		CreatedWidget->SetBonusData(ProductItem->Bonus.ProductCount > 0, BonusIcon, BonusName, ProductItem->Bonus.ProductCount, ProductItem->CalculatedBonusCount);

		// Restrict
		CreatedWidget->SetRestrictData(ProductItem->StoreProductId, ProductItem->RestrictType, ProductItem->RestrictCount);

		const FStoreFundsSale* FundsSale = BladeIIGameImpl::GetClientDataStore().GetStoreFundsSaleData(ProductItem->StoreProductId);
		if (FundsSale)
		{
			// Funds Sale
			bool bIsInclude = UB2GameInstance::IncludeTimeNow(FundsSale->StartTime, FundsSale->EndTime);
			if (FundsSale->DiscountRate > 0 && bIsInclude)
				CreatedWidget->SetFundsSale(FundsSale->DiscountRate);
			else
				// Product Mark
				CreatedWidget->SetProductMark(ProductItem->StoreProductId);
		}
		else
		{
			// Product Mark
			CreatedWidget->SetProductMark(ProductItem->StoreProductId);
		}

		// Cash Bonus
		const FStoreCashBonusData* CashBonus = BladeIIGameImpl::GetClientDataStore().GetStoreCashBonusData(ProductItem->StoreProductId);
		if (CashBonus)
		{
			bool bIsInclude = UB2GameInstance::IncludeTimeNow(CashBonus->StartTime, CashBonus->EndTime);
			if (CashBonus->IncreaseRate > 0 && bIsInclude)
				CreatedWidget->SetCashBonus(CashBonus->IncreaseRate);
		}

		// Cost
		CreatedWidget->SetCost(static_cast<EStoreItemCost>(ProductItem->CostType), ProductItem->Cost, ProductItem->CalculatedPrice);

		auto* AddedSlot = Cast<UHorizontalBoxSlot>(HB_ItemList->AddChild(CreatedWidget));
		check(AddedSlot);

		ItemSlots.Add(CreatedWidget);
	}
}

void UB2UIStore::SetGoodsCategory(int32 GoodsCategory)
{
	if (O_GoodsTab.IsValid())
		O_GoodsTab->SetVisibility(CurrentStoreCategory == EStoreCategory::Goods ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UB2UIStore::SetSelectedStoreCategory(int StoreCategory)
{
	if (VB_TabList.IsValid())
	{
		for (int Idx = 0; Idx < VB_TabList->GetChildrenCount(); Idx++)
		{
			UB2UICommonTab* ChildrenTab = static_cast<UB2UICommonTab*>(VB_TabList->GetChildAt(Idx));

			if (ChildrenTab)
			{
				if (ChildrenTab->GetTabButtonIndex() == StoreCategory)
					ChildrenTab->SetTabSelected(true);
				else
					ChildrenTab->SetTabSelected(false);
			}
		}
	}
}

void UB2UIStore::SetSelectedGoodsCategory(int32 GoodsCategory)
{
	if (GoodsTab_BladePoint.IsValid())
	{
		GoodsTab_BladePoint->SetTabSelected(false);
	}
	if (GoodsTab_Gem.IsValid())
	{
		GoodsTab_Gem->SetTabSelected(false);
	}
	if (GoodsTab_Gold.IsValid())
	{
		GoodsTab_Gold->SetTabSelected(false);
	}

	switch (GoodsCategory)
	{
	case EGoodsCategory::BLADE_POINT:
		if (GoodsTab_BladePoint.IsValid())
		{
			GoodsTab_BladePoint->SetTabSelected(true);
		}
		break;

	case EGoodsCategory::GEM:
		if (GoodsTab_Gem.IsValid())
		{
			GoodsTab_Gem->SetTabSelected(true);
		}
		break;

	case EGoodsCategory::GOLD:
		if (GoodsTab_Gold.IsValid())
		{
			GoodsTab_Gold->SetTabSelected(true);
		}
		break;

	default:
		break;
	}
}

void UB2UIStore::SetTabStoreMileage()
{
	if (UIP_StoreMileage.IsValid())
		UIP_StoreMileage->SetVisibility(CurrentStoreCategory == EStoreCategory::Goods ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (BTN_MileageGuide.IsValid())
		BTN_MileageGuide->SetIsEnabled(CurrentStoreCategory == EStoreCategory::Goods ? true : false);

	if (UIP_StoreMileage.IsValid())
	{
		switch (CurrentStoreCategory)
		{
		case EStoreCategory::Package:
			CurrentMileageType = EStoreMileageType::Package;
			break;

		case EStoreCategory::DarkCrystal:
			CurrentMileageType = EStoreMileageType::DarkCrystal;
			break;

		case EStoreCategory::Goods:
		{
			if (CurrentGoodsCategory == EGoodsCategory::BLADE_POINT)
			{
				CurrentMileageType = EStoreMileageType::BladePoint;
			}
			else if (CurrentGoodsCategory == EGoodsCategory::GEM)
			{
				CurrentMileageType = EStoreMileageType::Gem;
			}
			else if (CurrentGoodsCategory == EGoodsCategory::GOLD)
			{
				CurrentMileageType = EStoreMileageType::Gold;
			}
		}
		break;

		default:
			break;
		}

		UIP_StoreMileage->SetMileageData(CurrentMileageType);
	}
}


void UB2UIStore::SetOpenRewardPopupBClass()
{
	OpenRewardPopup(EUIStoreMileageGrade::BClass);
}

void UB2UIStore::SetOpenRewardPopupAClass()
{
	OpenRewardPopup(EUIStoreMileageGrade::AClass);
}

void UB2UIStore::SetOpenRewardPopupSClass()
{
	OpenRewardPopup(EUIStoreMileageGrade::SClass);
}

#include "B2ItemInfo.h"
void UB2UIStore::OpenRewardPopup(EUIStoreMileageGrade BoxGrade)
{
	ConditionalCreateMileageRewardPopup();
	if (Created_UIP_MileageRewardPopup)
	{
		Created_UIP_MileageRewardPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (UIP_StoreMileage.IsValid())
			Created_UIP_MileageRewardPopup->SetRewardPopup(CurrentMileageType);

		B2AndroidBackManager::GetInstance()->AddBackWidget(Created_UIP_MileageRewardPopup);
	}

	SetForcedBlockHeader(true);
}

void UB2UIStore::OpenPopupConfirmToBuyItem(int32 ProductId)
{
	SelectedProductIdToBuy = ProductId;

	auto* ProductData = BladeIIGameImpl::GetClientDataStore().GetStoreProductData(ProductId);
	check(ProductData);
	if (ProductData->Product.ProductType != static_cast<int32>(EStoreItemType::Gem))
	{
		ConditionalCreateConfirmToBuyPopup_Normal();
		if (Created_UIP_ConfirmToBuyPopup_Normal)
		{
			auto* Icon = Created_UIP_ConfirmToBuyPopup_Normal->GetSlot<UImage>(TEXT("IMG_ProductIcon"));
			if (Icon)
			{
				Icon->SetBrushFromMaterial(GetProductIcon(ProductData->StoreProductId, ProductData->Product.ProductId));
			}
			auto* ContentText = Created_UIP_ConfirmToBuyPopup_Normal->GetSlot<UB2RichTextBlock>(TEXT("TB_Content"));
			if (ContentText)
			{
				ContentText->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Confirm_Buy_Item")), GetProductName(ProductData->StoreProductId, ProductData->Product.ProductId), FText::AsNumber(ProductData->Product.ProductCount)));
			}

			SetCostData_ConfirmBuy(static_cast<EStoreItemCost>(ProductData->CostType), ProductData->Cost, ProductData->CalculatedPrice);
			SetBonusData_ConfirmBuy(static_cast<EStoreItemCost>(ProductData->CostType), ProductData->Bonus.ProductCount > 0, static_cast<EStoreItemType>(ProductData->Bonus.ProductType), ProductData->Bonus.ProductCount, ProductData->CalculatedBonusCount);
			Created_UIP_ConfirmToBuyPopup_Normal->OpenPopup();
		}
	}
	else
	{
		ConditionalCreateConfirmToBuyPopup_Gem();
		if (Created_UIP_ConfirmToBuyPopup_Gem)
		{
			auto* Icon = Created_UIP_ConfirmToBuyPopup_Gem->GetSlot<UImage>(TEXT("IMG_ProductIcon"));
			if (Icon)
			{
				Icon->SetBrushFromMaterial(GetProductIcon(ProductData->StoreProductId, ProductData->Product.ProductId));
			}

			auto* ContentText = Created_UIP_ConfirmToBuyPopup_Gem->GetSlot<UB2RichTextBlock>(TEXT("TB_Content"));
			if (ContentText)
			{
				ContentText->SetText(FText::Format(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Confirm_Buy_Item")), GetProductName(ProductData->StoreProductId, ProductData->Product.ProductId), FText::AsNumber(ProductData->Product.ProductCount)));
			}

			auto* DetailText = Created_UIP_ConfirmToBuyPopup_Gem->GetSlot<UTextBlock>(TEXT("STB_Detail"));
			if (DetailText)
			{
				DetailText->SetText(BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_PackageText2")));
			}

			SetCostData_ConfirmBuy(static_cast<EStoreItemCost>(ProductData->CostType), ProductData->Cost, ProductData->CalculatedPrice);
			SetBonusData_ConfirmBuy(static_cast<EStoreItemCost>(ProductData->CostType), ProductData->Bonus.ProductCount > 0, static_cast<EStoreItemType>(ProductData->Bonus.ProductType), ProductData->Bonus.ProductCount, ProductData->CalculatedBonusCount);
			Created_UIP_ConfirmToBuyPopup_Gem->OpenPopup();
		}
	}

	SetForcedBlockHeader(true);
}

void UB2UIStore::SetCostData_ConfirmBuy(EStoreItemCost CostType, int32 Cost, int32 CalculatedCost)
{
	if (CostType == EStoreItemCost::Cash)
	{
		ConditionalCreateConfirmToBuyPopup_Gem();
		if (Created_UIP_ConfirmToBuyPopup_Gem)
		{
			//B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(SelectedProductIdToBuy);
			//TWeakObjectPtr<UB2UIStoreProductCost> CostToBuyUIP = GetCostToBuyUIP(Created_UIP_ConfirmToBuyPopup_Gem);
			//if (CostToBuyUIP.IsValid())
			//{
			//	CostToBuyUIP->SetCashCost(ProductInfo->Price, ProductInfo->CurrencyCode);
			//}
		}
	}
	else
	{
		ConditionalCreateConfirmToBuyPopup_Normal();
		if (Created_UIP_ConfirmToBuyPopup_Normal)
		{
			TWeakObjectPtr<UB2UIStoreProductCost> CostToBuyUIP = GetCostToBuyUIP(Created_UIP_ConfirmToBuyPopup_Normal);
			if (CostToBuyUIP.IsValid())
			{
				CostToBuyUIP->SetCost(CostType, Cost, CalculatedCost);
			}
		}
	}

	SetForcedBlockHeader(true);
}

void UB2UIStore::SetBonusData_ConfirmBuy(EStoreItemCost CostType, bool bShowBonus, EStoreItemType BonusItemType, int32 InItemCount, int32 InBonusCount)
{
	int32 CalculatedBonusCount = InBonusCount > 0 ? InBonusCount : InItemCount;

	if (CostType == EStoreItemCost::Cash)
	{
		ConditionalCreateConfirmToBuyPopup_Gem();
		if (Created_UIP_ConfirmToBuyPopup_Gem)
		{
			// UIWidgetBase 네이티브로 블루프린트에만 존재하는 거라 이렇게 가져옴. 옛날 한때나 이렇게 만들었던 거 같은데. 따라하지 맙시다.
			// 원래 아래 코드가 블루프린트에 있었음.
			UB2RichTextBlock* TB_Bonus = Created_UIP_ConfirmToBuyPopup_Gem->GetSlot<UB2RichTextBlock>(TEXT("TB_Bonus"));
			check(TB_Bonus);
			if (TB_Bonus)
			{
				if (bShowBonus)
				{
					TB_Bonus->SetVisibility(ESlateVisibility::HitTestInvisible);

					if (BonusItemType == EStoreItemType::BladePoint)
					{
						TB_Bonus->SetText(FText::Format(
							BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bonus_Notification")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Bladepoint")), FText::AsNumber(CalculatedBonusCount)
						));
					}
					else if (BonusItemType == EStoreItemType::Gem)
					{
						TB_Bonus->SetText(FText::Format(
							BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bonus_Notification")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Gem")), FText::AsNumber(CalculatedBonusCount)
						));
					}
					else if (BonusItemType == EStoreItemType::Gold)
					{
						TB_Bonus->SetText(FText::Format(
							BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bonus_Notification")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Gold")), FText::AsNumber(CalculatedBonusCount)
						));
					}
					else if (BonusItemType == EStoreItemType::DarkCrystal)
					{
						TB_Bonus->SetText(FText::Format(
							BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bonus_Notification")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_DarkCrystal")), FText::AsNumber(CalculatedBonusCount)
						));
					}
				}
				else
				{
					TB_Bonus->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
	}
	else
	{
		ConditionalCreateConfirmToBuyPopup_Normal();
		if (Created_UIP_ConfirmToBuyPopup_Normal)
		{
			UB2RichTextBlock* TB_Bonus = Created_UIP_ConfirmToBuyPopup_Normal->GetSlot<UB2RichTextBlock>(TEXT("TB_Bonus"));
			check(TB_Bonus);
			if (TB_Bonus)
			{
				if (bShowBonus)
				{
					TB_Bonus->SetVisibility(ESlateVisibility::HitTestInvisible);

					if (BonusItemType == EStoreItemType::BladePoint)
					{
						TB_Bonus->SetText(FText::Format(
							BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bonus_Notification")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Bladepoint")), FText::AsNumber(CalculatedBonusCount)
						));
					}
					else if (BonusItemType == EStoreItemType::Gem)
					{
						TB_Bonus->SetText(FText::Format(
							BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bonus_Notification")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Gem")), FText::AsNumber(CalculatedBonusCount)
						));
					}
					else if (BonusItemType == EStoreItemType::Gold)
					{
						TB_Bonus->SetText(FText::Format(
							BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bonus_Notification")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Gold")), FText::AsNumber(CalculatedBonusCount)
						));
					}
					else if (BonusItemType == EStoreItemType::DarkCrystal)
					{
						TB_Bonus->SetText(FText::Format(
							BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_Bonus_Notification")), BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_DarkCrystal")), FText::AsNumber(CalculatedBonusCount)
						));
					}
				}
				else
				{
					TB_Bonus->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}
	}

	SetForcedBlockHeader(true);
}

void UB2UIStore::OnResponseRewardShopMileage(FB2ResponseRewardShopMileagePtr ResponseRewardShopMileage)
{
	if (Created_UIP_MileageRewardPopup && UIP_StoreMileage.IsValid())
		Created_UIP_MileageRewardPopup->SetRewardPopup(CurrentMileageType);

	DoMarkRedDot();
	FB2RedDotManager::RequestCheckRedDot({ RedDotHint::RED_DOT_SHOP });
}

void UB2UIStore::OnMileageChanged()
{
	DoMarkRedDot();
	FB2RedDotManager::RequestCheckRedDot({ RedDotHint::RED_DOT_SHOP });
}

void UB2UIStore::ClosePopupConfirmToBuyItem()
{
	// 닫는 곳에서 ConditionalCreate~~ 를 할 필요가 없다.. 이런 코드는 아직 해당 파트가 사용이 안되는 상황에서도 불릴 수 있으므로 생성을 시도하면 낭비가 될 수 있음.

	if (Created_UIP_ConfirmToBuyPopup_Gem)
		Created_UIP_ConfirmToBuyPopup_Gem->ClosePopup();

	if (Created_UIP_ConfirmToBuyPopup_Normal)
		Created_UIP_ConfirmToBuyPopup_Normal->ClosePopup();

	SetForcedBlockHeader(false);
}

void UB2UIStore::SubmitConfirmToBuyItem()
{
	data_trader::Retailer::GetInstance().RequestBuyShopProduct(SelectedProductIdToBuy);

	ClosePopupConfirmToBuyItem();
}

void UB2UIStore::SubmitConfirmToBuyGem()
{
	CloseConfirmToBuyPopupGemPopup();

	if (B2P_GetKakaoIDPCode() == EPlatformIDPCode::Guest)
	{
		OpenGuestWarningPopup();
	}
	else
	{
		ConfirmToBuyGem();
	}
}

void UB2UIStore::DetailConfirmToBuyItem()
{
	B2P_ShowOpenURL(B2P_GetKakaoCustomData(TEXT("withdSubscUrl")));
}

void UB2UIStore::ConfirmToBuyGem()
{
	//B2InAppPurchase::FB2CashProductInfo* ProductInfo = B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->GetMarketProduct(SelectedProductIdToBuy);
	//if (ProductInfo)
	//	B2InAppPurchase::IB2IAPGenericPlatform::GetInstance()->RequestPreparePurchase(SelectedProductIdToBuy, ProductInfo->MarketProductId);
}

void UB2UIStore::CompletedPurchaseBuyGem(bool success, const FB2ResponseBuyShopResultInfoPtr GetProduct)
{
	if (success)
	{
		BladeIIGameImpl::GetClientDataStore().ResponseBuyShopResultInfo(GetProduct);
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

void UB2UIStore::OpenGuestWarningPopup()
{
	ConditionalCreateGuestWarningPopup();

	if (Created_UIP_GuestWarningPopup)
	{
		Created_UIP_GuestWarningPopup->OpenPopup();
	}

	SetForcedBlockHeader(true);
}

void UB2UIStore::CloseGuestWarningPopup()
{
	if (Created_UIP_GuestWarningPopup)
	{
		Created_UIP_GuestWarningPopup->ClosePopup();
	}

	SetForcedBlockHeader(false);
}

void UB2UIStore::BuyGuestWarningPopup()
{
	if (Created_UIP_GuestWarningPopup)
	{
		Created_UIP_GuestWarningPopup->ClosePopup();
	}

	SetForcedBlockHeader(false);

	ConfirmToBuyGem();
}

bool UB2UIStore::RedDotCondition_BladePoint()
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
		return DocStore->HasUnRewardeMileage(EStoreMileageType::BladePoint);

	return false;
}

bool UB2UIStore::RedDotCondition_Gem()
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
		return DocStore->HasUnRewardeMileage(EStoreMileageType::Gem);

	return false;
}

bool UB2UIStore::RedDotCondition_Gold()
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
		return DocStore->HasUnRewardeMileage(EStoreMileageType::Gold);

	return false;
}

bool UB2UIStore::RedDotCondition_DarkCrystal()
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
		return DocStore->HasUnRewardeMileage(EStoreMileageType::DarkCrystal);

	return false;
}

bool UB2UIStore::RedDotCondition_Package()
{
	if (UB2UIDocStore* DocStore = UB2UIDocHelper::GetDocStore())
		return DocStore->HasUnRewardeMileage(EStoreMileageType::Package);

	return false;
}

void UB2UIStore::ConditionalCreateMileageRewardPopup()
{
	if (!Created_UIP_MileageRewardPopup && X_CP_MileageRewardPopup_Panel.IsValid())
	{
		Created_UIP_MileageRewardPopup = DynLoadAndCreateInCanvasPanelFull<UB2UIStoreMileageRewardPopup>(UIP_MileageRewardPopup_Class, this, X_CP_MileageRewardPopup_Panel.Get());
		if (Created_UIP_MileageRewardPopup)
		{
			if (RewardIconTemplate) {
				Created_UIP_MileageRewardPopup->SetRewardTemplateClass(RewardIconTemplate); // RewardIconTemplate 까지는 동적 로딩은 아니다. 
			}
			Created_UIP_MileageRewardPopup->Init();
		}
	}
}
void UB2UIStore::ConditionalCreateConfirmToBuyPopup_Gem()
{
	//if (!Created_UIP_ConfirmToBuyPopup_Gem && X_CP_ConfirmToBuyPopup_Gem_Panel.IsValid())
	//{
	//	Created_UIP_ConfirmToBuyPopup_Gem = DynLoadAndCreateInCanvasPanelFull<UB2UIBackWidgetBase>(UIP_ConfirmToBuyPopup_Gem_Class, this, X_CP_ConfirmToBuyPopup_Gem_Panel.Get());
	//	if (Created_UIP_ConfirmToBuyPopup_Gem)
	//	{
	//		Created_UIP_ConfirmToBuyPopup_Gem->Init();

	//		// BindDelegate 도 여기서
	//		BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_ConfirmToBuyPopup_Gem->GetSlot<UB2Button>(TEXT("BTN_Negative"))), &UB2UIStore::CloseConfirmToBuyPopupGemPopup);
	//		BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_ConfirmToBuyPopup_Gem->GetSlot<UB2Button>(TEXT("BTN_Positive"))), &UB2UIStore::SubmitConfirmToBuyGem);
	//		BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_ConfirmToBuyPopup_Gem->GetSlot<UB2Button>(TEXT("BTN_Detail"))), &UB2UIStore::DetailConfirmToBuyItem);

	//		B2InAppPurchase::IB2IAPGenericPlatform::OnPurchaseResultDelegate.BindLambda([this](bool success, b2network::B2BuyShopResultInfoPtr GetProduct) {
	//			UE_LOG(LogBladeII, Warning, TEXT("@@@@@@@ OnPurchaseResultDelegate @@@@@@@>> %d"), success);
	//			CompletedPurchaseBuyGem(success, GetProduct);
	//		});

	//		TWeakObjectPtr<UB2UIStoreProductCost> Cost = GetCostToBuyUIP(Created_UIP_ConfirmToBuyPopup_Gem);
	//		if (Cost.IsValid())
	//			Cost->Init();
	//	}
	//}
}
void UB2UIStore::ConditionalCreateConfirmToBuyPopup_Normal()
{
	if (!Created_UIP_ConfirmToBuyPopup_Normal && X_CP_ConfirmToBuyPopup_Normal_Panel.IsValid())
	{
		Created_UIP_ConfirmToBuyPopup_Normal = DynLoadAndCreateInCanvasPanelFull<UB2UIBackWidgetBase>(UIP_ConfirmToBuyPopup_Normal_Class, this, X_CP_ConfirmToBuyPopup_Normal_Panel.Get());
		if (Created_UIP_ConfirmToBuyPopup_Normal)
		{
			Created_UIP_ConfirmToBuyPopup_Normal->Init();
			// BindDelegate 도 여기서
			BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_ConfirmToBuyPopup_Normal->GetSlot<UB2Button>(TEXT("BTN_Negative"))), &UB2UIStore::CloseConfirmToBuyPopupNormalPopup);
			BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_ConfirmToBuyPopup_Normal->GetSlot<UB2Button>(TEXT("BTN_Positive"))), &UB2UIStore::SubmitConfirmToBuyItem);

			TWeakObjectPtr<UB2UIStoreProductCost> Cost = GetCostToBuyUIP(Created_UIP_ConfirmToBuyPopup_Normal);
			if (Cost.IsValid())
				Cost->Init();
		}
	}
}

void UB2UIStore::ConditionalCreateMileageGuidePopup()
{
	if (!Created_UIP_MileageGuidePopup && X_CP_MileageGuidePopup_Panel.IsValid())
	{
		Created_UIP_MileageGuidePopup = DynLoadAndCreateInCanvasPanelFull<UB2UIBackWidgetBase>(UIP_MileageGuidePopup_Class, this, X_CP_MileageGuidePopup_Panel.Get());
		if (Created_UIP_MileageGuidePopup)
		{
			Created_UIP_MileageGuidePopup->Init();
			Created_UIP_MileageGuidePopup->SetVisibility(ESlateVisibility::Collapsed);
			BTN_MileageGuidePopupClose = Created_UIP_MileageGuidePopup->GetSlot<UB2Button>(FName(TEXT("BTN_Confirm")));
			BIND_RELEASE_FUNC_TO_BTN(BTN_MileageGuidePopupClose, &UB2UIStore::CloseMileageGuidePopup);
		}
	}
}

void UB2UIStore::ConditionalCreateGuestWarningPopup()
{
	if (!Created_UIP_GuestWarningPopup && X_CP_GuestWarningPopup_Panel.IsValid())
	{
		Created_UIP_GuestWarningPopup = DynLoadAndCreateInCanvasPanelFull<UB2UIMsgPopupGuestWarning>(UIP_GuestWarningPopup_Class, this, X_CP_GuestWarningPopup_Panel.Get());
		if (Created_UIP_GuestWarningPopup)
		{
			Created_UIP_GuestWarningPopup->Init();

			BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_GuestWarningPopup->GetSlot<UB2Button>(TEXT("BTN_Cancel"))), &UB2UIStore::CloseGuestWarningPopup);
			BIND_RELEASE_FUNC_TO_BTN(TWeakObjectPtr<UB2Button>(Created_UIP_GuestWarningPopup->GetSlot<UB2Button>(TEXT("BTN_Buy"))), &UB2UIStore::BuyGuestWarningPopup);
		}
	}
}

void UB2UIStore::DestroyMileageRewardPopup()
{
	if (Created_UIP_MileageRewardPopup)
	{
		if (Created_UIP_MileageRewardPopup->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
		{
			B2AndroidBackManager::GetInstance()->RemoveBackWidget(Created_UIP_MileageRewardPopup);
		}
		Created_UIP_MileageRewardPopup->DestroySelf(UB2UIManager::GetInstance());
		Created_UIP_MileageRewardPopup = nullptr;
	}
}
void UB2UIStore::DestroyConfirmToBuyPopup_Gem()
{
	if (Created_UIP_ConfirmToBuyPopup_Gem)
	{
		Created_UIP_ConfirmToBuyPopup_Gem->DestroySelf(UB2UIManager::GetInstance());
		Created_UIP_ConfirmToBuyPopup_Gem = nullptr;
	}
}
void UB2UIStore::DestroyConfirmToBuyPopup_Normal()
{
	if (Created_UIP_ConfirmToBuyPopup_Normal)
	{
		Created_UIP_ConfirmToBuyPopup_Normal->DestroySelf(UB2UIManager::GetInstance());
		Created_UIP_ConfirmToBuyPopup_Normal = nullptr;
	}
}

void UB2UIStore::DestroyMileageGuidePopup()
{
	if (Created_UIP_MileageGuidePopup)
	{
		Created_UIP_MileageGuidePopup->DestroySelf(UB2UIManager::GetInstance());
		Created_UIP_MileageGuidePopup = nullptr;
	}
}

void UB2UIStore::DestroyGuestWarningPopup()
{
	if (Created_UIP_GuestWarningPopup)
	{
		Created_UIP_GuestWarningPopup->DestroySelf(UB2UIManager::GetInstance());
		Created_UIP_GuestWarningPopup = nullptr;
	}
}

void UB2UIStore::SetForcedBlockHeader(bool IsBlock)
{
	UB2UIHeader* HeaderUI = UB2UIManager::GetInstance()->GetUI<UB2UIHeader>(UIFName::Header);
	if (HeaderUI != nullptr)
		HeaderUI->SetForcedBlockHeader(IsBlock);
}
