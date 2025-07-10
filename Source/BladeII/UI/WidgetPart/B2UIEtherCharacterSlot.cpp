// Fill out your copyright notice in the Description page of Project Settings.

#include "B2UIEtherCharacterSlot.h"
#include "B2UIEtherScrollSlot.h"
#include "B2EtherManager.h"
#include "B2UIEther.h"
#include "B2EtherInfo.h"
#include "B2ItemInfo.h"
#include "B2UIEtherInfoNotify.h"
#include "B2UIManager.h"
#include "TutorialManager.h"
#include "B2GameDefine.h"
#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"


void FEtherSlot::OnVisible()
{
	if (IMG_BG.IsValid())
	{
		IMG_BG->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (IMG_SetImage.IsValid())
	{
		IMG_SetImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (BTN_Slot.IsValid())
	{
		BTN_Slot->SetVisibility(ESlateVisibility::Visible);
	}

	if (PW_SlotInfo.IsValid())
	{
		PW_SlotInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	IsOn = true;
}

void FEtherSlot::OffVisible()
{
	if (IMG_BG.IsValid())
	{
		IMG_BG->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IMG_SetImage.IsValid())
	{
		IMG_SetImage->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (BTN_Slot.IsValid())
	{
		BTN_Slot->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (PW_SlotInfo.IsValid())
	{
		PW_SlotInfo->SetVisibility(ESlateVisibility::Collapsed);
	}

	IsOn = false;
}

void FEtherSlot::SetSetImage(int32 InType, int32 InTier)
{
	if (StaticFindEtherInfoTable() != NULL && EtherType != InType)
	{
		IMG_SetImage->SetBrushFromMaterial(StaticFindEtherInfoTable()->GetEtherIcon(InType));
		EtherType = InType;
	}

	if (StaticFindItemMiscInfo() != NULL && EtherTier != InTier)
	{
		FSlateColor Color = StaticFindItemMiscInfo()->GetEtherTierTextColor(InTier);
		IMG_SetImage->SetColorAndOpacity(Color.GetSpecifiedColor());
		EtherTier = InTier;
	}
}

void FEtherSlot::SetTextInfo(int32 InStar, int32 InEnhanceLevel)
{
	if (RTB_Enhance.IsValid())
	{
		if (InEnhanceLevel > 0)
		{
			RTB_Enhance->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			RTB_Enhance->SetText(FText::Format(FText::FromString(TEXT("+{0}")), FText::AsNumber(InEnhanceLevel)));
		}
		else
		{
			RTB_Enhance->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (RTB_Star.IsValid())
	{
		RTB_Star->SetText(FText::AsNumber(InStar));
	}
}

void FEtherSlot::SetSetEffect(int32 SetState)
{
	switch (SetState)
	{
	case EtherSetType::OFF:
		if (IMG_Defence.IsValid())
		{
			IMG_Defence->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (IMG_Offense.IsValid())
		{
			IMG_Offense->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	case EtherSetType::DEFENCE:
		if (IMG_Defence.IsValid())
		{
			IMG_Defence->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		if (IMG_Offense.IsValid())
		{
			IMG_Offense->SetVisibility(ESlateVisibility::Collapsed);
		}
		break;
	case EtherSetType::OFFENCE:
		if (IMG_Defence.IsValid())
		{
			IMG_Defence->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (IMG_Offense.IsValid())
		{
			IMG_Offense->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		break;
	}
}

void UB2UIEtherCharacterSlot::EndCombineAnim_BP()
{
	SetEtherSetEffect(true);
}

void UB2UIEtherCharacterSlot::Init()
{
	DefenceSetID = 0;
	OffenseSetID = 0;
	Super::Init();
}

void UB2UIEtherCharacterSlot::DestroySelf(class UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);
	DeliverySelectEtherDefenseOptionClass<FB2SelectEtherDefenseOptionPtr>::GetInstance().Unsubscribe(SelectEtherDefenseOptionEventID);
	OnEtherSlotTouch.Unbind();
}

void UB2UIEtherCharacterSlot::SetInfo(UB2UIEther* InReceiver, EPCClass InPCClass, bool IsEquip)
{
	OnEtherSlotTouch.BindUObject(InReceiver, &UB2UIEther::GetEquipDetailPopup);
	PCClass = InPCClass;

	for (TPair<int32, FEtherSlot>& Elem : EtherSlots)
	{
		Elem.Value.OffVisible();
		Elem.Value.SetSetEffect(EtherSetType::OFF);
	}

	for (int i = 0; i < MAXOFFENSECOUNT; ++i)
	{
		ActiveOffense[i]->SetVisibility(ESlateVisibility::Collapsed);
		KeepOffense[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (int i = 0; i < MAXDEFENSECOUNT; ++i)
	{
		ActiveDefense[i]->SetVisibility(ESlateVisibility::Collapsed);
		keepDefense[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	TMap<int64, FB2Ether>& Ethers = BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetEquippedEthers(PCClass);
	for (TPair<int64, FB2Ether>& Elem : Ethers)
	{
		int32 Part = Elem.Value.Part;
		if (EtherSlots.Contains(Part))
		{
			if (EtherSlots[Part].EtherID != Elem.Value.ID)
			{
				EtherSlots[Part].EtherID = Elem.Value.ID;
				
				if (IsEquip)
				{
					EquipEtherAnimation(Part);
				}
			}

			EtherSlots[Part].SetSetImage(Elem.Value.Type, Elem.Value.Tier);
			EtherSlots[Part].SetTextInfo(Elem.Value.Star, Elem.Value.EnhanceLevel);
			EtherSlots[Part].OnVisible();
		}
	}

	for (TPair<int32, FEtherSlot>& Elem : EtherSlots)
	{
		if (Elem.Value.IsOn == false)
		{
			Elem.Value.EtherID = -1;
			Elem.Value.EtherTier = -1;
			Elem.Value.EtherType = -1;
		}
	}
	
	if (IsEquip == false)
	{
		FLocalCharacterData& CData = BladeIIGameImpl::GetLocalCharacterData();
		DefenceSetID = CData.GetEtherDefenceSetID(PCClass);
		OffenseSetID = CData.GetEtherOffenseSetID(PCClass);
		
		SetEtherSetEffect(false);
	}
}

#define SET_ETHER_SLOT(Part, Index); \
EtherSlots.Add(ETHER::PART::M##Part); \
GET_SLOT_BYNAME(UImage, EtherSlots[ETHER::PART::M##Part].IMG_Offense, X_IMG_Offense##Index); \
GET_SLOT_BYNAME(UImage, EtherSlots[ETHER::PART::M##Part].IMG_Defence, X_IMG_Defence##Index); \
GET_SLOT_BYNAME(UImage, EtherSlots[ETHER::PART::M##Part].IMG_SetImage, X_IMG_Slot##Index); \
GET_SLOT_BYNAME(UB2Button, EtherSlots[ETHER::PART::M##Part].BTN_Slot, X_BTN_Slot##Index); \
GET_SLOT_BYNAME(UImage, EtherSlots[ETHER::PART::M##Part].IMG_BG, X_IMG_SlotBG##Index); \
GET_SLOT_BYNAME(UB2RichTextBlock, EtherSlots[ETHER::PART::M##Part].RTB_Enhance, X_RTB_Enhance##Index); \
GET_SLOT_BYNAME(UB2RichTextBlock, EtherSlots[ETHER::PART::M##Part].RTB_Star, X_RTB_Star##Index); \
GET_SLOT_BYNAME(UPanelWidget, EtherSlots[ETHER::PART::M##Part].PW_SlotInfo, X_PW_Info##Index); \
EtherSlots[ETHER::PART::M##Part].OnPlayAnim.BindUObject(this, &UB2UIEtherCharacterSlot::PlayEquipAnim##Part##_BP);

void UB2UIEtherCharacterSlot::CacheAssets()
{
	SET_ETHER_SLOT(ONE, 1);
	SET_ETHER_SLOT(TWO, 2);
	SET_ETHER_SLOT(THREE, 3);
	SET_ETHER_SLOT(FOUR, 4);
	SET_ETHER_SLOT(FIVE, 5);
	SET_ETHER_SLOT(SIX, 6);

	GET_SLOT(UPanelWidget, X_PW_Combine);

	ActiveOffense.SetNum(MAXOFFENSECOUNT);
	GET_SLOT_BYNAME(UPanelWidget, ActiveOffense[0], X_PW_ActiveOffense01);
	GET_SLOT_BYNAME(UPanelWidget, ActiveOffense[1], X_PW_ActiveOffense02);
	GET_SLOT_BYNAME(UPanelWidget, ActiveOffense[2], X_PW_ActiveOffense03);
	GET_SLOT_BYNAME(UPanelWidget, ActiveOffense[3], X_PW_ActiveOffense04);

	KeepOffense.SetNum(MAXOFFENSECOUNT);
	GET_SLOT_BYNAME(UPanelWidget, KeepOffense[0], X_PW_KeepOffense01);
	GET_SLOT_BYNAME(UPanelWidget, KeepOffense[1], X_PW_KeepOffense02);
	GET_SLOT_BYNAME(UPanelWidget, KeepOffense[2], X_PW_KeepOffense03);
	GET_SLOT_BYNAME(UPanelWidget, KeepOffense[3], X_PW_KeepOffense04);

	ActiveDefense.SetNum(MAXDEFENSECOUNT);
	GET_SLOT_BYNAME(UPanelWidget, ActiveDefense[0], X_PW_ActiveDefense01);
	GET_SLOT_BYNAME(UPanelWidget, ActiveDefense[1], X_PW_ActiveDefense02);

	keepDefense.SetNum(MAXDEFENSECOUNT);
	GET_SLOT_BYNAME(UPanelWidget, keepDefense[0], X_PW_KeepDefense01);
	GET_SLOT_BYNAME(UPanelWidget, keepDefense[1], X_PW_KeepDefense02);
}

void UB2UIEtherCharacterSlot::BindDelegates()
{
	SelectEtherDefenseOptionEventID = DeliverySelectEtherDefenseOptionClass<FB2SelectEtherDefenseOptionPtr>::GetInstance().Subscribe([this](const FB2SelectEtherDefenseOptionPtr EtherPtr)
	{
		FLocalCharacterData& CData = BladeIIGameImpl::GetLocalCharacterData();
		this->SetEtherSetEffect(this->PCClass, EtherPtr->selected_defense_set_option_id, CData.GetEtherOffenseSetID(this->PCClass), false);
	});

	BIND_CLICK_FUNC_TO_BTN(EtherSlots[ETHER::PART::MONE].BTN_Slot, &UB2UIEtherCharacterSlot::OnClickOneSlot);
	BIND_CLICK_FUNC_TO_BTN(EtherSlots[ETHER::PART::MTWO].BTN_Slot, &UB2UIEtherCharacterSlot::OnClickTwoSlot);
	BIND_CLICK_FUNC_TO_BTN(EtherSlots[ETHER::PART::MTHREE].BTN_Slot, &UB2UIEtherCharacterSlot::OnClickThreeSlot);
	BIND_CLICK_FUNC_TO_BTN(EtherSlots[ETHER::PART::MFOUR].BTN_Slot, &UB2UIEtherCharacterSlot::OnClickFourSlot);
	BIND_CLICK_FUNC_TO_BTN(EtherSlots[ETHER::PART::MFIVE].BTN_Slot, &UB2UIEtherCharacterSlot::OnClickFiveSlot);
	BIND_CLICK_FUNC_TO_BTN(EtherSlots[ETHER::PART::MSIX].BTN_Slot, &UB2UIEtherCharacterSlot::OnClickSixSlot);
}

void UB2UIEtherCharacterSlot::OnClickOneSlot()
{
	if (EtherSlots.Contains(ETHER::PART::MONE))
	{
		OnEtherSlotTouch.ExecuteIfBound(EtherSlots[ETHER::PART::MONE].EtherID);
	}
}

void UB2UIEtherCharacterSlot::OnClickTwoSlot()
{
	if (EtherSlots.Contains(ETHER::PART::MTWO))
	{
		OnEtherSlotTouch.ExecuteIfBound(EtherSlots[ETHER::PART::MTWO].EtherID);
	}
}

void UB2UIEtherCharacterSlot::OnClickThreeSlot()
{
	if (EtherSlots.Contains(ETHER::PART::MTHREE))
	{
		OnEtherSlotTouch.ExecuteIfBound(EtherSlots[ETHER::PART::MTHREE].EtherID);
	}
}

void UB2UIEtherCharacterSlot::OnClickFourSlot()
{
	if (EtherSlots.Contains(ETHER::PART::MFOUR))
	{
		OnEtherSlotTouch.ExecuteIfBound(EtherSlots[ETHER::PART::MFOUR].EtherID);
	}
}

void UB2UIEtherCharacterSlot::OnClickFiveSlot()
{
	if (EtherSlots.Contains(ETHER::PART::MFIVE))
	{
		OnEtherSlotTouch.ExecuteIfBound(EtherSlots[ETHER::PART::MFIVE].EtherID);
	}
}

void UB2UIEtherCharacterSlot::OnClickSixSlot()
{
	if (EtherSlots.Contains(ETHER::PART::MSIX))
	{
		OnEtherSlotTouch.ExecuteIfBound(EtherSlots[ETHER::PART::MSIX].EtherID);
	}
}

void UB2UIEtherCharacterSlot::SetEtherSetEffect(EPCClass InPCClass, int32 InDefenceSetID, int32 InOffenseSetID, bool IsEquip)
{
	//bool IsDefenseSetChange = false;
	//bool IsOffenseSetChange = false;

	//if (InDefenceSetID != 0 && DefenceSetID != InDefenceSetID)
	//{
	//	IsDefenseSetChange = true;
	//}

	//if (InOffenseSetID != 0 && OffenseSetID != InOffenseSetID)
	//{
	//	IsOffenseSetChange = true;
	//}

	//DefenceSetID = InDefenceSetID;
	//OffenseSetID = InOffenseSetID;

	//TMap<int64, FB2Ether>& Ethers = BladeIIGameImpl::GetLocalCharacterData().GetAllEthers().GetEquippedEthers(InPCClass);
	//TArray<FB2Ether> SortEthers;
	//Ethers.GenerateValueArray(SortEthers);
	//SortEthers.Sort([](const FB2Ether& A, const FB2Ether& B)
	//{
	//	//Æ¼¾î > ÆÄÃ÷ ¼ø
	//	bool Result = false;
	//	if (A.Tier > B.Tier)
	//	{
	//		Result = true;
	//	}
	//	else if (A.Tier == B.Tier)
	//	{
	//		if (A.Part > B.Part)
	//		{
	//			Result = true;
	//		}
	//		else
	//		{
	//			Result = false;
	//		}

	//	}
	//	return Result;
	//});

	//int DefenceCount = 0;
	//int OffenseCount = 0;

	//for (const FB2Ether& Elem : SortEthers)
	//{
	//	int32 Part = Elem.Part;
	//	if (EtherSlots.Contains(Part))
	//	{
	//		EtherSlots[Part].SetSetEffect(EtherSetType::OFF);

	//		b2network::B2mdAetherSetOptionInfoPtr SetOptionPtr = B2EtherManager::GetInstance().GetEtherSetOptionInfoPtrByType(Elem.Type, Elem.Tier);
	//		if (SetOptionPtr != nullptr)
	//		{
	//			if (SetOptionPtr->set_option_id == DefenceSetID && SetOptionPtr->min_apply_count > DefenceCount)
	//			{
	//				EtherSlots[Part].SetSetEffect(EtherSetType::DEFENCE);
	//				
	//				if (MAXDEFENSECOUNT > DefenceCount && ActiveDefense[DefenceCount].IsValid() && keepDefense[DefenceCount].IsValid() && ConvertDefenseMap.Contains(Part))
	//				{
	//					//keepDefense[DefenceCount]->SetRenderAngle(ConvertDefenseMap[Part]);
	//					keepDefense[DefenceCount]->SetVisibility(ESlateVisibility::HitTestInvisible);
	//					if (IsEquip == true)
	//					{
	//						//ActiveDefense[DefenceCount]->SetRenderAngle(ConvertDefenseMap[Part]);
	//						ActiveDefense[DefenceCount]->SetVisibility(ESlateVisibility::HitTestInvisible);
	//					}
	//				}
	//				DefenceCount++;
	//			}
	//			else if (SetOptionPtr->set_option_id == OffenseSetID && SetOptionPtr->min_apply_count > OffenseCount)
	//			{
	//				EtherSlots[Part].SetSetEffect(EtherSetType::OFFENCE);

	//				if (MAXOFFENSECOUNT > OffenseCount && ActiveOffense[OffenseCount].IsValid() && KeepOffense[OffenseCount].IsValid() && ConvertOffenseMap.Contains(Part))
	//				{
	//					KeepOffense[OffenseCount]->SetRenderAngle(ConvertOffenseMap[Part]);
	//					KeepOffense[OffenseCount]->SetVisibility(ESlateVisibility::HitTestInvisible);
	//					if (IsEquip == true)
	//					{
	//						//ActiveOffense[OffenseCount]->SetRenderAngle(ConvertOffenseMap[Part]);
	//						ActiveOffense[OffenseCount]->SetVisibility(ESlateVisibility::HitTestInvisible);
	//					}
	//				}
	//				OffenseCount++;
	//			}
	//		}
	//	}
	//}

	//if (IsEquip == true && IsOffenseSetChange == true)
	//{
	//	PlayActiveOffense_BP();
	//	UB2UIEtherInfoNotify* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UIEtherInfoNotify>(UIFName::EtherInfoNotify);
	//	if (Popup)
	//	{
	//		Popup->SetInfoNotify(FText::Format(FText::FromString(TEXT("{0} {1}")),
	//			B2EtherManager::GetInstance().GetEtherSetOptionText(OffenseSetID),
	//			BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_SetOption_Activation_Notice")))));
	//	}
	//}

	//if (IsEquip == true && IsDefenseSetChange == true)
	//{
	//	PlayActiveDefense_BP();
	//	UB2UIEtherInfoNotify* Popup = UB2UIManager::GetInstance()->OpenUI<UB2UIEtherInfoNotify>(UIFName::EtherInfoNotify);
	//	if (Popup)
	//	{
	//		Popup->SetInfoNotify(FText::Format(FText::FromString(TEXT("{0} {1}")),
	//			B2EtherManager::GetInstance().GetEtherSetOptionText(DefenceSetID),
	//			BladeIIGetLOCText(FString(B2LOC_CAT_ETHER), FString(TEXT("Ether_SetOption_Activation_Notice")))));
	//	}
	//}

	//if (IsEquip == false)
	//{
	//	if (OffenseSetID != 0)
	//	{
	//		PlayKeepOffense_BP();
	//	}
	//	
	//	if (DefenceSetID != 0)
	//	{
	//		PlayKeepDefense_BP();
	//	}
	//}
}

void UB2UIEtherCharacterSlot::SetEtherSetEffect(bool IsEquip)
{
	FLocalCharacterData& CData = BladeIIGameImpl::GetLocalCharacterData();
	SetEtherSetEffect(PCClass, CData.GetEtherDefenceSetID(PCClass), CData.GetEtherOffenseSetID(PCClass), IsEquip);
}

void UB2UIEtherCharacterSlot::EquipAnimOneEnd_BP()
{
	if (TutorialManager::GetInstance().IsCurrentTutorial(TutorialID_Ether, 14))
		TutorialManager::GetInstance().AtherEquipAnimEnd();
}

void UB2UIEtherCharacterSlot::EquipEtherAnimation(int32 InPart)
{
	/*if (EtherSlots.Contains(InPart))
	{
		if (X_PW_Combine.IsValid())
		{
			if (ConvertComBineMap.Contains(InPart))
			{
				X_PW_Combine->SetVisibility(ESlateVisibility::HitTestInvisible);
				X_PW_Combine->SetRenderAngle(ConvertComBineMap[InPart]);
			}
			else
			{
				X_PW_Combine->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		EtherSlots[InPart].OnPlayAnim.ExecuteIfBound();
	}*/
}
