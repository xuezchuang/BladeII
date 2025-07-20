#pragma once

#include "B2UIWidget.h"
#include "B2UIDocBindable.h"
#include "B2UIBackWidget.h"
#include "../../InfoAsset/B2SkillInfo.h"
#include "B2UIRegistSkill.generated.h"

class FCachedResistSkillData
{
public:
	FCachedResistSkillData() {}
	virtual ~FCachedResistSkillData() {}

	void Init(int32 SlotMax, EItemPresetSlotType SlotType);
	void SetSkillID(int32 SlotIndex, int32 SkillID, bool CheckDirty = false);
	int32 GetSkillID(int32 SlotIndex);
	bool IsDirty() { return DitryData; }
private:
	bool DitryData;
	EItemPresetSlotType SkillSlotID;
	TArray<int32> SkillIDs;
};
using FCachedResistSkillDataPtr = std::shared_ptr<FCachedResistSkillData>;

UCLASS(BlueprintType)
class BLADEII_API UB2UIRegistSkill : public UB2UIWidget, public IB2UIBackWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	void SetPCClass(EPCClass InPCClass);
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
	virtual void CloseWidgetDelegate() override;

	void SubscribeEvents();
	void UnsubscribeEvents();

	void UpdateSkillSlotText();
	void UpdatePresetState(EItemPresetSlotType SelectSlotId);
private:
	UFUNCTION()
		void OnClickBtnClose();
	UFUNCTION()
		void OnClickBtnCancel();

	//void SetSkillInfo(TWeakObjectPtr<class UB2UIPlayerSkillInfo> WidgetPart, int32 SkillId, const class UB2SkillInfo* AllSkillInfo, const FLocalCharacterData& CharacterData, bool bRegistred);
	void CheckToRegistSkill();
	bool CheckToChangeSkillPosition(int32, int32);

	void CachedSlotSkillInfos(int32 SlotID, int32 Skill0, int32 skill1, int32 skill2);

	TWeakObjectPtr<class UB2UIPlayerSkillInfo> GetSkillSlot(int SkillId);

	void ShowAskSaveConfirmPopup();
	void ShowRegistCompletePopup();

	void CloseMe();

	bool IsDirtyAnySkillSlot();
private:
	TWeakObjectPtr<UButton> BTN_Cancel;
	TWeakObjectPtr<UButton> BTN_Close;
	
	TArray<TWeakObjectPtr<class UB2UIPlayerSkillInfo>> UsingSkills;
	TArray<TWeakObjectPtr<class UB2UIPlayerSkillInfo>> HavingSkills;

	TArray<TWeakObjectPtr<class UB2UIPHeroMgmtPreset>> SkillSlots;
	TWeakObjectPtr<UPanelWidget> HB_SkillPreset;

	EItemPresetSlotType CurrentSkillSlotID;
	TMap<int32, FCachedResistSkillDataPtr> CachedRegitSkills;

	EPCClass PCClass;

	bool  bChangeSkillSlot;
	int32 WaitingToUnregistSkillId;
	int32 WaitingToRegistSkillId;

	int32 SkillSlotMax;

	uint32 bEventSubscribed : 1;
	uint32 SelectPreviewPresetSlotTicket;
	uint32 DeliverySetUsingSkillTicket;
};