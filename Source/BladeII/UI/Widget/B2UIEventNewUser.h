// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "B2UIWidget.h"
#include "../../BladeII/BladeIIGameMode.h"
#include "B2UIEventNewUser.generated.h"


UCLASS()
class BLADEII_API UB2UIEventNewUser : public UB2UIWidget
{
	GENERATED_BODY()

private:
	void ReceiveEventNewUserAttendance(FB2ResponseBeginnerAttendancePtr InNewUserInfo);

	void SetEventSpecialDay();
	void SetEventNewParts();
	void SubscribeEvents();
	void UnsubscribeEvents();

protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;

public:
	virtual void Init() override;
	virtual void DestroySelf(class UB2UIManager* InUIManager) override;

	bool IsEndEvent() const;
	bool IsNewUserEvent() const;

private:
	TArray<TWeakObjectPtr<class UB2UIEventAttendacnePart>> UIP_EventNewUserParts;
	TArray<issue_ptr>	Issues;

	TWeakObjectPtr<UTextBlock> TB_RemainDay;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpecialDay)
		TArray<int32> SpecialDay;

};
