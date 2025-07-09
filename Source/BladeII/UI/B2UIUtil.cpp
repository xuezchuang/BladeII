#include "BladeII.h"
#include "B2UIUtil.h"
#include "BladeIIGameImpl.h"
#include "B2UIManager.h"
#include "B2UIShortagePopupManager.h"
#include "B2UIMsgPopupGuestWarning.h"
#include "B2UIDocHelper.h"
#include "B2AssetLoader.h"
#include "B2UIMain.h"
#include "B2UIMsgPopupInventoryFull.h"

bool B2UIUtil::IsLimitedBuySummonSlot(int32 SummonItemSlotHashKey)
{
	//auto* Data = BladeIIGameImpl::GetClientDataStore().GetSummonItemData(SummonItemSlotHashKey);
	//if (Data)
	//{
	//	int32 BuyMaxCount = Data->GetLimitdBuyCount();
	//	int32 BuyCurrentCount = Data->GetBuyCount();
	//	if (BuyMaxCount && (BuyCurrentCount - BuyMaxCount) == 0)
	//	{
	//		UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupSimple>(EUIMsgPopup::Simple,
	//			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SensitiveNoti_Notification")),
	//			BladeIIGetLOCText(B2LOC_CAT_STORE, TEXT("Store_BuyError")),
	//			0.f,
	//			true,
	//			true,
	//			EUIMsgPopupButtonGroup::Confirm
	//			);
	//		return true;
	//	}
	//}

	return false;
}
EItemInvenType B2UIUtil::GetItemEquipPlaceToItemInventoryType(EItemEquipPlace EquipPlace)
{
	switch (EquipPlace)
	{
	case EItemEquipPlace::EIEP_Weapon:
		return EItemInvenType::EIIVT_Weapon;

	case EItemEquipPlace::EIEP_Helmet:
	case EItemEquipPlace::EIEP_BodyArmor:
	case EItemEquipPlace::EIEP_Glove:
	case EItemEquipPlace::EIEP_Shoes:
		return EItemInvenType::EIIVT_Protection;

	case EItemEquipPlace::EIEP_Belt:
	case EItemEquipPlace::EIEP_Necklace:
	case EItemEquipPlace::EIEP_Ring:
		return EItemInvenType::EIIVT_Accessory;

	case EItemEquipPlace::EIEP_End:
		return EItemInvenType::EIIVT_End;
	}
	return EItemInvenType::EIIVT_End;
}

TWeakObjectPtr<UB2UIMsgPopupInventoryFull> B2UIUtil::OpenInventoryFullPopup(EPCClass PCClass, const FMsgPopupOnClick& OverrideMoveInven, bool ShowExtension)
{
	auto* Popup = UB2UIManager::GetInstance()->OpenMsgPopup<UB2UIMsgPopupInventoryFull>(
		EUIMsgPopup::InventoryFull,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("StageStartErrorTitle")),
		FText::GetEmpty()
	);

	if (Popup != nullptr)
	{
		Popup->SetPCClass(PCClass);
		Popup->OverrideMoveInventoryLogic(OverrideMoveInven);
		Popup->ShowExtensionBtn(ShowExtension);
	}


	return TWeakObjectPtr<UB2UIMsgPopupInventoryFull>(Popup);
}

int32 B2UIUtil::DifferenceItemPrimaryPoint(const FB2Item & Origin, const FB2Item & Comparer)
{
	return (int32)Origin.GetPrimaryPoint() - (int32)Comparer.GetPrimaryPoint();
}

bool B2UIUtil::HasLevelUpSkillPoint(int32 InSkillID, int32 InSkillLevel)
{
	const EPCClass SkillClass = BladeIIGameImpl::GetClientDataStore().GetSkillClass(InSkillID);
	const int32 LeftSkillPoint = BladeIIGameImpl::GetLocalCharacterData().GetCharacterLeftSkillPoint(SkillClass);

	return LeftSkillPoint >= BladeIIGameImpl::GetLocalCharacterData().GetCharacterSkillLevelupPoint(InSkillID, InSkillLevel);
}

FText B2UIUtil::TwoStepTimeSpanToString(FTimespan InTimeSpan)
{
	FString Format = TEXT("{0} {1}");
	
	if (InTimeSpan.GetTotalDays() >= 1)
	{
		return FText::Format(FText::FromString(Format),
			FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Day")), (int32)InTimeSpan.GetTotalDays()),
			FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Hour")), InTimeSpan.GetHours()));
	}
	else if (InTimeSpan.GetTotalHours() >= 1)
	{
		return FText::Format(FText::FromString(Format),
			FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Hour")), InTimeSpan.GetHours()),
			FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Min")), InTimeSpan.GetMinutes()));
	}
	else if (InTimeSpan.GetTotalMinutes() >= 1)
	{
		return FText::Format(FText::FromString(Format),
			FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Min")), InTimeSpan.GetMinutes()),
			FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Sconds")), InTimeSpan.GetSeconds()));
	}
	else
	{
		return FText::Format(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Sconds")), InTimeSpan.GetSeconds());
	}
}

UB2UIMsgPopupGuestWarning* B2UIUtil::OpenGuestWarningPopup(UB2UnitedWidgetBase* OwnerUserWidget)
{
	UB2UIUtilAssetContainer* UtilAssetContainer = Cast<UB2UIUtilAssetContainer>(UB2UIUtilAssetContainer::StaticClass()->GetDefaultObject());

	if (UtilAssetContainer != nullptr)
	{
		if (UtilAssetContainer->GuestWarningPopupAsset.IsValid())
		{
			TSoftClassPtr<UB2UIMsgPopupGuestWarning> Subclassof(UtilAssetContainer->GuestWarningPopupAsset);
			TSubclassOf<UB2UIMsgPopupGuestWarning> LoadedClass = UB2AssetLoader::Get()->LoadSynchronous(Subclassof);
			UB2UIMsgPopupGuestWarning* CreatedWidget = CreateWidget<UB2UIMsgPopupGuestWarning>(OwnerUserWidget->GetOwningPlayer(), LoadedClass);

			UB2UIMain* MainWidget = UB2UIManager::GetInstance()->GetMainWidget();
			UPanelWidget* LayerWidget = MainWidget != nullptr ? MainWidget->GetParentLayer(EUILayer::Popups) : nullptr;

			if (CreatedWidget && LayerWidget)
			{
				CreatedWidget->Init();
				LayerWidget->AddChild(CreatedWidget);
				auto OverlaySlot = Cast<UOverlaySlot>(CreatedWidget->Slot);
				if (OverlaySlot)
				{
					OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
					OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				}
				return CreatedWidget;
			}
		}
	}
	return nullptr;
}

bool B2UIUtil::CheckPayableStoreProduct(const FStoreProductData * CheckProductData)
{
	if (CheckProductData == nullptr)
		return false;

	bool IsBuy = true;

	EStoreItemCost CurrentCostType = static_cast<EStoreItemCost>(CheckProductData->CostType);

	if (CheckProductData->Cost != 0)
	{
		int32 UserCost = 0;
		if (auto UserInfo = UB2UIDocHelper::GetDocUser())
		{
			switch (CurrentCostType)
			{
			case EStoreItemCost::Cash:
				return true;
			case EStoreItemCost::Gem:
				UserCost = UserInfo->GetGem();
				break;
			case EStoreItemCost::DarkCrystal:
				UserCost = UserInfo->GetDarkCrystal();
				break;
			case EStoreItemCost::FriendshipPoint:
				UserCost = UserInfo->GetSocialPoint();
				break;
			case EStoreItemCost::TenLotteryTicket:
				UserCost = BladeIIGameImpl::GetClientDataStore().GetShopTenLotteryTicket();
				break;
			case EStoreItemCost::Gold:
				UserCost = BladeIIGameImpl::GetClientDataStore().GetGoldAmount();
			default:
				break;
			}
		}

		if (CheckProductData->CalculatedPrice > 0 && CheckProductData->CalculatedPrice > UserCost)
		{
			IsBuy = false;
		}
		else if (
			(CurrentCostType == EStoreItemCost::DarkCrystal ||
			 CurrentCostType == EStoreItemCost::FriendshipPoint ||
			 CurrentCostType == EStoreItemCost::Gem)
			 &&
			 CheckProductData->Cost > UserCost)
		{
			IsBuy = false;
		}
	}
	return IsBuy;
}

void B2UIUtil::OpenCostTypeErrorPopup(EStoreItemCost InCostType)
{
	//switch (InCostType)
	//{
	//case EStoreItemCost::DarkCrystal:
	//	UB2UIManager::GetInstance()->GetShortagePopupMGR()->PopupGotoShopForDarkCrystalMessage();
	//	break;
	//case EStoreItemCost::FriendshipPoint:
	//	UB2UIManager::GetInstance()->GetShortagePopupMGR()->PopupGotoFriendForFriendShip();
	//	break;
	//case EStoreItemCost::Gem:
	//	UB2UIManager::GetInstance()->GetShortagePopupMGR()->PopupGoToShopForGemMessage();
	//	break;
	//default:
	//	break;
	//}
}

static TMap<EItemOption, int32> InitGuildBuffSortIndex()
{
	TMap<EItemOption, int32> retVal;

	retVal.Add(EItemOption::EIO_Offense_IncAttack, 3);
	retVal.Add(EItemOption::EIO_Defense_IncDefense, 2);
	retVal.Add(EItemOption::EIO_Health_IncHealth, 1);

	return retVal;
}

static TMap<EItemOption, int32> GuildBuffSortIndex = InitGuildBuffSortIndex();

int32 B2UIUtil::GetGuildBuffSortValue(EItemOption InItemOption)
{
	if (GuildBuffSortIndex.Contains(InItemOption))
		return GuildBuffSortIndex[InItemOption];

	return INDEX_NONE;
}

UMaterialInterface * FB2ClassPortraitImage::GetCharImage(const TArray<FB2ClassPortraitImage>& ImageInfos, EPCClass PCClass)
{
	for (auto& ImageInfo : ImageInfos)
	{
		if (ImageInfo.CharClass == PCClass)
			return ImageInfo.Portrait;
	}

	return nullptr;
}

B2SlotSliderLogic::B2SlotSliderLogic()
{

}

void B2SlotSliderLogic::SetData(int32 InCurrentSlotCount, int32 InSlotCountPerSingleExtend, int32 InMaxAllowedSlotCount)
{
	CurrentSlotCount = InCurrentSlotCount;
	SlotCountPerSingleExtend = InSlotCountPerSingleExtend;
	MaxAllowedSlotCount = InMaxAllowedSlotCount;

	SliderMinSlotCount = FMath::Min(MaxAllowedSlotCount, CurrentSlotCount + SlotCountPerSingleExtend);
	MaxExtendCount = (MaxAllowedSlotCount - SliderMinSlotCount) / SlotCountPerSingleExtend;
	if (MaxAllowedSlotCount == SliderMinSlotCount)
		SliderStepSize = 1.0f;
	else
		SliderStepSize = static_cast<float>(SlotCountPerSingleExtend) / static_cast<float>(MaxAllowedSlotCount - SliderMinSlotCount);
}

int32 B2SlotSliderLogic::SliderValueToExtendCount(float SliderValue)
{
	return FMath::RoundToInt(SliderValue / SliderStepSize);
}

float B2SlotSliderLogic::ExtendCountToSliderValue(int32 ExtendCount)
{
	return MaxAllowedSlotCount <= SliderMinSlotCount ? 1.0f : FMath::Min(static_cast<float>(DomainExtendCount(ExtendCount)) * SliderStepSize, 1.0f);
}

int32 B2SlotSliderLogic::GetRealExtendCount(float SliderValue)
{
	int32 CorrectionValue = SliderMinSlotCount < MaxAllowedSlotCount ? 1 : 0;
	return SliderValueToExtendCount(SliderValue) + CorrectionValue;
}

int32 B2SlotSliderLogic::GetSlotCount(int32 ExtendCount)
{
	return FMath::Min(MaxAllowedSlotCount, (DomainExtendCount(ExtendCount) * SlotCountPerSingleExtend) + SliderMinSlotCount);
}