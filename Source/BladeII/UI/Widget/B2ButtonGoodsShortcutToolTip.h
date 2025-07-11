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


	//�ٸ� �ൿ�� ���ϰ� ������ ����� Click Delegate����
	UFUNCTION()
	void StopButtonClicked();
	//�ٸ� �ൿ�� ���ϰ� ������ ����� Press Delegate����
	UFUNCTION()
	void StopButtonPressed();
	//Delay �������� ���� Delay���Ŀ� ���� �Ϸ��� �ߴ� ��ư �̺�Ʈ�� �߻���Ŵ
	UFUNCTION()
	void ClickedBroadCast();
	////Delay �������� ���� Delay���Ŀ� ���� �Ϸ��� �ߴ� ��ư �̺�Ʈ�� �߻���Ŵ
	UFUNCTION()
	void PressedBroadCast();
	UFUNCTION()
	void DelayPressed(bool _DelayPress = false) { m_DelayPress = _DelayPress; }; //Press�� Delay��Ű�� ���ٴ� Click�� Delay��Ű�� ���� ��õ. ������ ȣ�� ������ Ȯ�� �ϸ� ��.
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
	//������ ShortcutInfo���� ������ �ְ� �ϰ� �;�����....Wing���� ��� ���� �÷��� PCClass�� �־�� �Ǵµ� PCClass�� Shortcutinfo���� ������ ����� ���...���⼭ ������ ����.
	TMap<uint32, int32> m_GoodsAmountMap;	//uint8 EGoodsType, int32�� Amount

	UB2UnitedWidgetBase* m_InOwnerUnitedWidget;

	bool				m_isEnough;
	bool				m_DelayPress;
	bool				m_DelayClick;

	FOnButtonClickedEvent m_ReserveClicked;
	FOnButtonPressedEvent m_ReservePressed;
public:
	
};
