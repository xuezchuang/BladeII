// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventSubsystem.h"
#include "UObject/WeakObjectPtr.h"



/**
*
*/
enum class EAndroidBackState : uint8
{
	NONE = 0,
	INDICATOR,
	CLOSEBACKWIDGET,
	CLOSEHEADER,
	CLOSEBATTLEMAIN,
	CLOSECOMBINEMENU,
	EXIT,
};

enum class EBackWidgetType : uint8
{
	BACKWIDGET = 0,
	HEADER,
	BATTLEMAIN,
	COMBINEMENU
};

class BLADEII_API B2AndroidBackManager
{

public:
	static B2AndroidBackManager*		GetInstance();

	void SubscribeEvents();
	void UnsubscribeEvents();
	void OnPlatformPausePress();

	void OnClickBackButton();
	void AddBackWidget(UObject* Value, FName UIName = FName(TEXT("Popup")));
	void RemoveBackWidget(UObject* Value, FName UIName = FName(TEXT("Popup")));
	void RemoveBackWidget(FName UIName);
	void RemoveAllPopup();

	bool HasHaderUI();


private:
	B2AndroidBackManager();
	B2AndroidBackManager(const B2AndroidBackManager& other);
	~B2AndroidBackManager();

	EAndroidBackState					GetAndroidBackState();
	EBackWidgetType						GetBackWidgetType(FName& UIName);

	TArray<TWeakObjectPtr<UObject>>		BackWidgetHistory;
	//纠俊 窍唱父 乐绢具 窃
	TWeakObjectPtr<UObject>					HeaderWidget;
	TWeakObjectPtr<UObject>					BattleMainWidget;
	TWeakObjectPtr<UObject>					CombineMenuWidget;

private:
	TArray<std::shared_ptr<issue>> Issues;

};