#include "B2UIRegistSkill.h"
#include "B2UIManager.h"
#include "B2PCClassInfoBox.h"
#include "B2SkillInfo.h"
#include "B2UIPlayerSkillInfo.h"
#include "B2UIPHeroMgmtPreset.h"


#include "BladeIIGameImpl.h"

void FCachedResistSkillData::Init(int32 SlotMax, EItemPresetSlotType SlotType)
{
	DitryData = false;
	SkillIDs.SetNum(SlotMax);
	SkillSlotID = SlotType;
}

void FCachedResistSkillData::SetSkillID(int32 SlotIndex, int32 SkillID, bool CheckDirty /*= false*/)
{
	if (SkillIDs.IsValidIndex(SlotIndex))
	{
		DitryData = CheckDirty;
		SkillIDs[SlotIndex] = SkillID;
	}
}

int32  FCachedResistSkillData::GetSkillID(int32 SlotIndex)
{
	if (SkillIDs.IsValidIndex(SlotIndex))
		return SkillIDs[SlotIndex]; 
	return SKILL_INVALID_ID;
}

void UB2UIRegistSkill::Init()
{
	Super::Init();

	SubscribeEvents();

	WaitingToUnregistSkillId = WaitingToRegistSkillId = SKILL_INVALID_ID;
}

void UB2UIRegistSkill::SetPCClass(EPCClass InPCClass)
{
	/*PCClass = InPCClass;

	SkillSlotMax = BladeIIGameImpl::GetClientDataStore().GetSkillPresetMaxCount();

	auto& CharacterDataStore = BladeIIGameImpl::GetLocalCharacterData();


	auto* ClassInfoBox = StaticFindPCClassInfoBox();
	auto* AllSkillInfo = ClassInfoBox ? ClassInfoBox->GetAllSkillInfo() : nullptr;

	
	TArray<int32> UsingSkillIds;
	CharacterDataStore.GetCharacterRegistredSkills(PCClass, UsingSkillIds, false);
	BII_CHECK(UsingSkillIds.Num() == MAX_CLASS_SKILL_INPUT && UsingSkills.Num() == MAX_CLASS_SKILL_INPUT);
	for (int32 i = 0; i < UsingSkills.Num(); ++i)
	{
		SetSkillInfo(UsingSkills[i], UsingSkillIds[i], AllSkillInfo, CharacterDataStore, true);
	}

	TArray<int32> HavingSkillIds;
	CharacterDataStore.GetCharacterUnregistredSkills(PCClass, HavingSkillIds);
	for (int32 i = 0; i < HavingSkills.Num(); ++i)
	{
		SetSkillInfo(HavingSkills[i], HavingSkillIds.Num() > i ? HavingSkillIds[i] : SKILL_INVALID_ID, AllSkillInfo, CharacterDataStore, false);
	}

	UpdateSkillSlotText();

	for (int32 i = 0; i < SkillSlotMax; ++i)
	{
		TArray<int32> SlotSkillIds;
		CharacterDataStore.GetRegistredTargetSlotSkills(PCClass, (EItemPresetSlotType)i, SlotSkillIds);

		int32 Skill0 = SlotSkillIds.IsValidIndex(0) ? SlotSkillIds[0] : SKILL_INVALID_ID;
		int32 Skill1 = SlotSkillIds.IsValidIndex(1) ? SlotSkillIds[1] : SKILL_INVALID_ID;
		int32 Skill2 = SlotSkillIds.IsValidIndex(2) ? SlotSkillIds[2] : SKILL_INVALID_ID;

		CachedSlotSkillInfos(i, Skill0, Skill1, Skill2);
	}

	EItemPresetSlotType SlotType = BladeIIGameImpl::GetLocalCharacterData().GetPCClassSkillSlotID(PCClass);
	UpdatePresetState(SlotType);*/

}

void UB2UIRegistSkill::OnClickBtnClose()
{
/*	for (int32 SlotID = 0; SlotID < SkillSlotMax; ++SlotID)
	{
		if (CachedRegitSkills.Contains(SlotID))
		{
			auto &Each = CachedRegitSkills[SlotID];
			if (Each->IsDirty())
			{
				BladeIIGameImpl::GetClientDataStore().RequestSetUsingSkill(
					PCClass,
					SlotID,
					Each->GetSkillID(0),
					Each->GetSkillID(1),
					Each->GetSkillID(2)
				);
			}
		}
	}
	if (bChangeSkillSlot)
	{
		LobbyHeroTransientEffectBeginClass<const int32>::GetInstance().Signal(0);
		bChangeSkillSlot = false;
	}
	else
	{
		CloseMe();
	}*/	
}

void UB2UIRegistSkill::OnClickBtnCancel()
{
	if (IsDirtyAnySkillSlot())
	{
		ShowAskSaveConfirmPopup();
	}
	else
	{
		CloseMe();
	}
}

void UB2UIRegistSkill::CacheAssets()
{
	GET_SLOT(UButton, BTN_Cancel);
	GET_SLOT(UButton, BTN_Close);

	UsingSkills.AddDefaulted(3);
	GET_SLOT_BYNAME(UB2UIPlayerSkillInfo, UsingSkills[0], UsingSkill01);
	GET_SLOT_BYNAME(UB2UIPlayerSkillInfo, UsingSkills[1], UsingSkill02);
	GET_SLOT_BYNAME(UB2UIPlayerSkillInfo, UsingSkills[2], UsingSkill03);

	HavingSkills.AddDefaulted(3);
	GET_SLOT_BYNAME(UB2UIPlayerSkillInfo, HavingSkills[0], HavingSkill01);
	GET_SLOT_BYNAME(UB2UIPlayerSkillInfo, HavingSkills[1], HavingSkill02);
	GET_SLOT_BYNAME(UB2UIPlayerSkillInfo, HavingSkills[2], HavingSkill03);

	for (auto& UIP_Skill : UsingSkills)
	{
		if (UIP_Skill.IsValid())
			UIP_Skill->Init();
	}

	for (auto& UIP_Skill : HavingSkills)
	{
		if (UIP_Skill.IsValid())
			UIP_Skill->Init();
	}

	GET_SLOT(UPanelWidget, HB_SkillPreset)

	const int32 MaxPresetSlot = static_cast<uint8>(EItemPresetSlotType::EIPS_End);
	SkillSlots.SetNum(MaxPresetSlot);
	GET_SLOT_BYNAME(UB2UIPHeroMgmtPreset, SkillSlots[static_cast<uint8>(EItemPresetSlotType::EIPS_First)], SkillPreset_0)
	GET_SLOT_BYNAME(UB2UIPHeroMgmtPreset, SkillSlots[static_cast<uint8>(EItemPresetSlotType::EIPS_Second)], SkillPreset_1)
	GET_SLOT_BYNAME(UB2UIPHeroMgmtPreset, SkillSlots[static_cast<uint8>(EItemPresetSlotType::EIPS_Third)], SkillPreset_2)

	for (auto& UIP_Skill : SkillSlots)
	{
		if (UIP_Skill.IsValid())
		{
			UIP_Skill->Init();
			UIP_Skill->SetUsageType(EPresetUsageType::Preview);
		}
	}

	if (HB_SkillPreset.IsValid())
	{
		int32 MinPreset = 1;
		bool HasPresets = BladeIIGameImpl::GetClientDataStore().GetSkillPresetMaxCount() > MinPreset;
		HB_SkillPreset->SetVisibility(HasPresets ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UB2UIRegistSkill::BindDelegates()
{
	BIND_CLICK_FUNC_TO_BTN(BTN_Cancel, &UB2UIRegistSkill::OnClickBtnCancel);
	BIND_CLICK_FUNC_TO_BTN(BTN_Close, &UB2UIRegistSkill::OnClickBtnClose);
}

void UB2UIRegistSkill::DestroySelf(class UB2UIManager* InUIManager)
{
	for (auto& UIP_Skill : UsingSkills)
	{
		if (UIP_Skill.IsValid())
			UIP_Skill->DestroySelf(InUIManager);
	}

	for (auto& UIP_Skill : HavingSkills)
	{
		if (UIP_Skill.IsValid())
			UIP_Skill->DestroySelf(InUIManager);
	}

	bChangeSkillSlot = false;
	UsingSkills.Empty();
	HavingSkills.Empty();

	UnsubscribeEvents();
}

void UB2UIRegistSkill::SubscribeEvents()
{
	//if (bEventSubscribed)
	//	return;

	//CAPTURE_UOBJECT(UB2UIRegistSkill);

	//SelectPreviewPresetSlotTicket = SelectPreviewPresetSlotClass<EItemPresetSlotType>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(SelectPreviewPresetSlot, const EItemPresetSlotType InPresetSlot)
	//	Capture->UpdatePresetState(InPresetSlot);
	//	END_CAPTURE_OBJECT()
	//);
	//DeliverySetUsingSkillTicket = DeliverySetUsingSkillClass<FB2SetUsingSkillInfo>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(DeliverySetUsingSkill, const FB2SetUsingSkillInfo& SetUsingSkillInfo)
	//	Capture->ShowRegistCompletePopup();
	//	END_CAPTURE_OBJECT()
	//);
	
	bEventSubscribed = true;
}

void UB2UIRegistSkill::UnsubscribeEvents()
{
	//if (!bEventSubscribed)
	//	return;

	//SelectPreviewPresetSlotClass<EItemPresetSlotType>::GetInstance().Unsubscribe(SelectPreviewPresetSlotTicket);
	//DeliverySetUsingSkillClass<FB2SetUsingSkillInfo>::GetInstance().Unsubscribe(DeliverySetUsingSkillTicket);

	//bEventSubscribed = false;
}

void UB2UIRegistSkill::CloseWidgetDelegate()
{
	OnClickBtnCancel();
}

TWeakObjectPtr<UB2UIPlayerSkillInfo> UB2UIRegistSkill::GetSkillSlot(int SkillId)
{
	for (auto& UIP_Skill : UsingSkills)
	{
		if (UIP_Skill.IsValid() && UIP_Skill->GetSkillId() == SkillId)
			return UIP_Skill;
	}

	for (auto& UIP_Skill : HavingSkills)
	{
		if (UIP_Skill.IsValid() && UIP_Skill->GetSkillId() == SkillId)
			return UIP_Skill;
	}

	return TWeakObjectPtr<UB2UIPlayerSkillInfo>(nullptr);
}

//void UB2UIRegistSkill::SetSkillInfo(TWeakObjectPtr<UB2UIPlayerSkillInfo> WidgetPart, int32 SkillId, const UB2SkillInfo* AllSkillInfo, const FLocalCharacterData& CharacterData, bool bRegistred)
//{
//	if (WidgetPart.IsValid())
//	{
//		auto* SkillSingleInfo = AllSkillInfo->GetSingleInfoOfID(SkillId);
//
//		if (SkillSingleInfo)
//		{
//			WidgetPart->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//
//			WidgetPart->SetSkillId(SkillId);
//			// GetButtonIconStyle 에 로딩이 있어서 const 를 빼야 함..
//			UB2SkillInfo* CastedNonConstSkillInfo = const_cast<UB2SkillInfo*>(AllSkillInfo);
//			if (CastedNonConstSkillInfo){
//				WidgetPart->SetButtonStyle(CastedNonConstSkillInfo->GetButtonIconStyle(SkillId));
//			}
//			WidgetPart->SetSkillType(SkillSingleInfo->SkillType);
//			WidgetPart->SetSkillLevel(CharacterData.GetCharacterSkillLevel(SkillId),true);
//						
//			int32* HandleSkillId = bRegistred ? &WaitingToUnregistSkillId : &WaitingToRegistSkillId;
//
//			WidgetPart->SetSkillIconHandler(FSkillInfoOnClick::CreateLambda([this, WidgetPart, HandleSkillId]()
//			{
//				if (WidgetPart.IsValid() && *HandleSkillId != WidgetPart->GetSkillId())
//				{
//					if (*HandleSkillId != SKILL_INVALID_ID)
//					{
//						auto SkillSlot = GetSkillSlot(*HandleSkillId);
//						if (SkillSlot.IsValid())
//							SkillSlot->SetSelected(false);
//					}
//
//					if (false == this->CheckToChangeSkillPosition(*HandleSkillId, WidgetPart->GetSkillId()))
//					{
//						*HandleSkillId = WidgetPart->GetSkillId();
//						WidgetPart->SetSelected(true);
//					}					
//				}	
//				else 
//				{
//					if (WidgetPart.IsValid() && *HandleSkillId == WidgetPart->GetSkillId())
//					{
//						auto SkillSlot = GetSkillSlot(*HandleSkillId);
//						if (SkillSlot.IsValid())
//							SkillSlot->SetSelected(false);
//
//						WaitingToRegistSkillId = WaitingToUnregistSkillId = SKILL_INVALID_ID;
//					}
//				}
//				this->CheckToRegistSkill();
//			}
//			));
//		}
//		else
//		{
//			WidgetPart->SetVisibility(ESlateVisibility::Hidden);
//		}
//	}
//}

#define SUGGEST_SKILL(SkillSlot, bSuggest)								\
if (SkillSlot.IsValid() && SkillSlot->GetSkillId() != SKILL_INVALID_ID)	\
{																		\
	SkillSlot->SetSuggested(bSuggest);									\
}

#define SUGGEST_SETTING(bSetUsingSlots, bSetHavingSlots)	\
	for (auto& UIP_Skill : UsingSkills)						\
	{														\
		SUGGEST_SKILL(UIP_Skill, bSetUsingSlots);			\
	}														\
	for (auto& UIP_Skill : HavingSkills)					\
	{														\
		SUGGEST_SKILL(UIP_Skill, bSetHavingSlots);			\
	}

void UB2UIRegistSkill::CheckToRegistSkill()
{
	//if (WaitingToRegistSkillId != SKILL_INVALID_ID && WaitingToUnregistSkillId != SKILL_INVALID_ID)
	//{
	//	auto& CharacterData = BladeIIGameImpl::GetLocalCharacterData();
	//	auto* ClassInfoBox = StaticFindPCClassInfoBox();
	//	auto* AllSkillInfo = ClassInfoBox ? ClassInfoBox->GetAllSkillInfo() : nullptr;

	//	if (AllSkillInfo == nullptr)
	//		return;

	//	auto RegistredSkillSlot = GetSkillSlot(this->WaitingToUnregistSkillId);
	//	auto UnregistredSkillSlot = GetSkillSlot(this->WaitingToRegistSkillId);

	//	if (RegistredSkillSlot.IsValid())
	//	{
	//		SetSkillInfo(RegistredSkillSlot, WaitingToRegistSkillId, AllSkillInfo, CharacterData, true);
	//		RegistredSkillSlot->SetSelected(false);
	//	}		
	//	if (UnregistredSkillSlot.IsValid())
	//	{
	//		SetSkillInfo(UnregistredSkillSlot, WaitingToUnregistSkillId, AllSkillInfo, CharacterData, false);
	//		UnregistredSkillSlot->SetSelected(false);
	//	}
	//	
	//	WaitingToUnregistSkillId = WaitingToRegistSkillId = SKILL_INVALID_ID;

	//	bChangeSkillSlot = true;
	//	SUGGEST_SETTING(false, false);
	//}
	//else if (WaitingToRegistSkillId != SKILL_INVALID_ID)
	//{
	//	SUGGEST_SETTING(true, false);
	//}
	//else if (WaitingToUnregistSkillId != SKILL_INVALID_ID)
	//{
	//	bChangeSkillSlot = true;
	//	SUGGEST_SETTING(UIP_Skill.IsValid() && UIP_Skill->GetSkillId() != WaitingToUnregistSkillId, true);
	//}
	//else
	//{
	//	SUGGEST_SETTING(false, false);
	//}

	//CachedSlotSkillInfos(static_cast<int32>(CurrentSkillSlotID), 
	//	UsingSkills[0].IsValid() ? UsingSkills[0]->GetSkillId() : SKILL_INVALID_ID,
	//	UsingSkills[1].IsValid() ? UsingSkills[1]->GetSkillId() : SKILL_INVALID_ID,
	//	UsingSkills[2].IsValid() ? UsingSkills[2]->GetSkillId() : SKILL_INVALID_ID);
}

bool UB2UIRegistSkill::CheckToChangeSkillPosition(int32 SrcId, int32 DestId)
{
	//if (SrcId == SKILL_INVALID_ID || DestId == SKILL_INVALID_ID)
	//	return false;

	//if (SrcId != WaitingToUnregistSkillId)
	//	return false;
	//
	//TWeakObjectPtr<UB2UIPlayerSkillInfo> SrcSlot, DestSlot;

	//for (int32 i = 0; i < UsingSkills.Num(); ++i)
	//{
	//	if (UsingSkills[i].IsValid() && UsingSkills[i]->GetSkillId() == SrcId)
	//		SrcSlot = UsingSkills[i];		
	//	else if (UsingSkills[i].IsValid() && UsingSkills[i]->GetSkillId() == DestId)
	//		DestSlot = UsingSkills[i];
	//}

	//if (!SrcSlot.IsValid() || !DestSlot.IsValid())
	//	return false;

	//auto& CharacterData = BladeIIGameImpl::GetLocalCharacterData();
	//auto* ClassInfoBox = StaticFindPCClassInfoBox();
	//auto* AllSkillInfo = ClassInfoBox ? ClassInfoBox->GetAllSkillInfo() : nullptr;

	//if (AllSkillInfo == nullptr)
	//	return false;

	//SetSkillInfo(DestSlot, SrcId, AllSkillInfo, CharacterData, true);
	//SetSkillInfo(SrcSlot, DestId, AllSkillInfo, CharacterData, true);
	//
	//WaitingToUnregistSkillId = SKILL_INVALID_ID;

	return true;
}

void UB2UIRegistSkill::UpdateSkillSlotText()
{
	const int32 MaxPresetSlot = static_cast<uint8>(EItemPresetSlotType::EIPS_End);
	for (int i = 0; i < MaxPresetSlot; ++i)
	{
		if (SkillSlots[i].IsValid())
		{
			SkillSlots[i]->SetSlotNumber(PCClass, (EItemPresetSlotType)i);
		}
	}
}

void UB2UIRegistSkill::UpdatePresetState(EItemPresetSlotType SelectSlotId)
{
	//if (WaitingToRegistSkillId != SKILL_INVALID_ID || WaitingToUnregistSkillId != SKILL_INVALID_ID)
	//	return;

	//const int32 MaxPresetSlot = static_cast<uint8>(EItemPresetSlotType::EIPS_End);
	//const int32 SlotID = (int32)SelectSlotId;
	//CurrentSkillSlotID = SelectSlotId;

	//for (int i = 0; i < MaxPresetSlot; ++i)
	//{
	//	if (SkillSlots[i].IsValid())
	//	{
	//		SkillSlots[i]->SwitchState(i == SlotID);
	//	}
	//}

	//auto& ClientDataStore = BladeIIGameImpl::GetLocalCharacterData();
	//
	//auto* ClassInfoBox = StaticFindPCClassInfoBox();
	//auto* AllSkillInfo = ClassInfoBox ? ClassInfoBox->GetAllSkillInfo() : nullptr;

	//TArray<int32> UsingSkillIds;
	//UsingSkillIds.SetNum(SkillSlotMax);

	//if (CachedRegitSkills.Contains(SlotID))
	//{
	//	auto &Each = CachedRegitSkills[SlotID];
	//	for (int32 i = 0; i < SkillSlotMax; ++i)
	//	{
	//		UsingSkillIds[i] = Each->GetSkillID(i);
	//	}
	//}

	//BII_CHECK(UsingSkillIds.Num() == MAX_CLASS_SKILL_INPUT && UsingSkills.Num() == MAX_CLASS_SKILL_INPUT);
	//for (int32 i = 0; i < UsingSkills.Num(); ++i)
	//{
	//	SetSkillInfo(UsingSkills[i], UsingSkillIds[i], AllSkillInfo, ClientDataStore, true);
	//}

	//TArray<int32> HavingSkillIds;
	//ClientDataStore.GetUnregistredTargetSlotSkills(PCClass, CurrentSkillSlotID, HavingSkillIds);
	//for (int32 i = 0; i < HavingSkills.Num(); ++i)
	//{
	//	SetSkillInfo(HavingSkills[i], HavingSkillIds.Num() > i ? HavingSkillIds[i] : SKILL_INVALID_ID, AllSkillInfo, ClientDataStore, false);
	//}

	//UpdateSkillSlotText();
}

void UB2UIRegistSkill::CachedSlotSkillInfos(int32 SlotID, int32 Skill0, int32 Skill1, int32 Skill2)
{
	if (CachedRegitSkills.Contains(SlotID))
	{
		auto &Each = CachedRegitSkills[SlotID];
		Each->SetSkillID(0, Skill0, true);
		Each->SetSkillID(1, Skill1, true);
		Each->SetSkillID(2, Skill2, true);
	}
	else
	{
		FCachedResistSkillDataPtr NewData = FCachedResistSkillDataPtr(new FCachedResistSkillData());
		NewData->Init(SkillSlotMax, static_cast<EItemPresetSlotType>(SlotID));
		NewData->SetSkillID(0, Skill0);
		NewData->SetSkillID(1, Skill1);
		NewData->SetSkillID(2, Skill2);

		CachedRegitSkills.Add(SlotID, NewData);
	}
}

bool UB2UIRegistSkill::IsDirtyAnySkillSlot()
{
	for (int32 SlotID = 0; SlotID < SkillSlotMax; ++SlotID)
	{
		if (CachedRegitSkills.Contains(SlotID))
		{
			auto &Each = CachedRegitSkills[SlotID];
			if (Each->IsDirty())
			{
				return true;
			}
		}
	}

	return false;
}

void UB2UIRegistSkill::ShowAskSaveConfirmPopup()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_ConfirmSkillSetting")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::YesOrNo,
		FMsgPopupOnClick::CreateLambda([this]() {
			CloseMe();
		})
	);
}

void UB2UIRegistSkill::ShowRegistCompletePopup()
{
	UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("General_Notification")),
		BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("SkillInfo_SaveSkillSetting")),
		0.f,
		true,
		true,
		EUIMsgPopupButtonGroup::Confirm,
		FMsgPopupOnClick::CreateLambda([this]() {
			CloseMe();
		})
	);
}

void UB2UIRegistSkill::CloseMe()
{
	UB2UIManager::GetInstance()->CloseUI(UIFName::RegistSkill);
}