// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
#include "UI/Widget/B2Button.h"
#include "B2ButtonGoodsShortcutToolTip.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2ButtonGoodsShortcutToolTip : public UB2Button
{
	GENERATED_BODY()
	
//Function
private:
protected:
	struct FB2GoodsShortcutIndividualInfo* GetGoodsShortcutFromShortcutInfoObject(int32 _GoodsShortcutInfoID);
	bool IsGoodsShorcutCondition(FName& InFGoodsType) const;
	int32 GetHaveAmount(FName FGoodsType) const;
	
	UFUNCTION()
	virtual void RegisterDelayClickEvent();
	UFUNCTION()
	virtual void RegisterDelayPressEvent();


public:
	UB2ButtonGoodsShortcutToolTip(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void StartFromLobby(class UB2UIManager_Lobby* const InUIManager, class AB2LobbyGameMode* InGM);
	
	//UFUNCTION()
	virtual void RegisterExtraStuff(UB2UnitedWidgetBase* InOwnerUnitedWidget) override;

	//UFUNCTION()
	virtual void UnregisterExtraStuff(UB2UnitedWidgetBase* InOwnerUnitedWidget) override;
	//UFUNCTION()
	virtual void ExtraOnPress() override;
	//UFUNCTION()
	virtual void ExtraOnClick() override;
	//UFUNCTION()
	virtual void ExtraOnRelease() override;
	//UFUNCTION()
	virtual void ExtraOnHovered() override;


	//다른 행동을 못하게 기존에 등록한 Click Delegate삭제
	UFUNCTION()
	void StopButtonClicked();
	//다른 행동을 못하게 기존에 등록한 Press Delegate삭제
	UFUNCTION()
	void StopButtonPressed();
	//Delay 시켰을때 일정 Delay이후에 원래 하려고 했던 버튼 이벤트를 발생시킴
	UFUNCTION()
	void ClickedBroadCast();
	////Delay 시켰을때 일정 Delay이후에 원래 하려고 했던 버튼 이벤트를 발생시킴
	UFUNCTION()
	void PressedBroadCast();
	UFUNCTION()
	void DelayPressed(bool _DelayPress = false) { m_DelayPress = _DelayPress; }; //Press를 Delay시키기 보다는 Click을 Delay시키는 것을 추천. 이유는 호출 순서를 확인 하면 됨.
	UFUNCTION()
	void DelayClicked(bool _DelayClick = false) { m_DelayClick = _DelayClick; };
	UFUNCTION()
	void SetItemInfo(int32 _ItemInfoidx);
	UFUNCTION()
	void AddGoodsAmountMap(uint32 _type, int32 _amount);
	UFUNCTION()
	void ClearInfo() { m_ShortcutInfoDocID.Empty(); };
	UFUNCTION()
	FORCEINLINE bool IsEnough() const { return m_isEnough; };
	
	
//Variable
private:
protected:
	/** Probably need this for some item operation. */
	UPROPERTY(Transient)
	class UB2LobbyInventory* CachedInventory;
	UPROPERTY(Transient)
	class AB2LobbyGameMode* CachedLobbyGM;
	
	TArray<int32> m_ShortcutInfoDocID;
	//원래는 ShortcutInfo에서 가지고 있게 하고 싶었으나....Wing같은 경우 가져 올려면 PCClass가 있어야 되는데 PCClass를 Shortcutinfo에서 가져올 방법이 없어서...여기서 가지고 있음.
	TMap<uint32, int32> m_GoodsAmountMap;	//uint8 EGoodsType, int32는 Amount

	UB2UnitedWidgetBase* m_InOwnerUnitedWidget;

	bool				m_isEnough;
	bool				m_DelayPress;
	bool				m_DelayClick;

	FOnButtonClickedEvent m_ReserveClicked;
	FOnButtonPressedEvent m_ReservePressed;
public:
	
};
