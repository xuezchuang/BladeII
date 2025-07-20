// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UObject/Object.h"
#include "UObject/Interface.h"

#include "B2UIBackWidget.generated.h"

UINTERFACE(MinimalAPI)
class UB2UIBackWidget : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IB2UIBackWidget
{
	GENERATED_IINTERFACE_BODY()

public:

	void				CloseWidget();
	void				SetClose(bool Value);
	void				OpenBattleMainPause();

protected:
	virtual void		CloseWidgetDelegate() = 0;

private:
	bool				IsClose = true;
};
