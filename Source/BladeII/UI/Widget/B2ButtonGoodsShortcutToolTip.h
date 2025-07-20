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


	//促弗 青悼阑 给窍霸 扁粮俊 殿废茄 Click Delegate昏力
	UFUNCTION()
	void StopButtonClicked();
	//促弗 青悼阑 给窍霸 扁粮俊 殿废茄 Press Delegate昏力
	UFUNCTION()
	void StopButtonPressed();
	//Delay 矫淖阑锭 老沥 Delay捞饶俊 盔贰 窍妨绊 沁带 滚瓢 捞亥飘甫 惯积矫糯
	UFUNCTION()
	void ClickedBroadCast();
	////Delay 矫淖阑锭 老沥 Delay捞饶俊 盔贰 窍妨绊 沁带 滚瓢 捞亥飘甫 惯积矫糯
	UFUNCTION()
	void PressedBroadCast();
	UFUNCTION()
	void DelayPressed(bool _DelayPress = false) { m_DelayPress = _DelayPress; }; //Press甫 Delay矫虐扁 焊促绰 Click阑 Delay矫虐绰 巴阑 眠玫. 捞蜡绰 龋免 鉴辑甫 犬牢 窍搁 凳.
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
	//盔贰绰 ShortcutInfo俊辑 啊瘤绊 乐霸 窍绊 酵菌栏唱....Wing鞍篮 版快 啊廉 棵妨搁 PCClass啊 乐绢具 登绰单 PCClass甫 Shortcutinfo俊辑 啊廉棵 规过捞 绝绢辑...咯扁辑 啊瘤绊 乐澜.
	TMap<uint32, int32> m_GoodsAmountMap;	//uint8 EGoodsType, int32绰 Amount

	UB2UnitedWidgetBase* m_InOwnerUnitedWidget;

	bool				m_isEnough;
	bool				m_DelayPress;
	bool				m_DelayClick;

	FOnButtonClickedEvent m_ReserveClicked;
	FOnButtonPressedEvent m_ReservePressed;
public:
	
};
