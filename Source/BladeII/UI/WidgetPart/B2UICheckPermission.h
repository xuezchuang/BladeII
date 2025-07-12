// Fill out your copyright notice in the Description page of Project Settings.
#pragma once


#include "B2UIDLCFrontBase.h"
#include "../Widget/B2Button.h"
#include "B2UICheckPermission.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UICheckPermission : public UB2UIDLCFrontBase
{
	GENERATED_BODY()
	friend class UB2UIPatch;
protected:
	virtual void CacheAssets() override;
	virtual void BindDelegates() override;
public:
	UB2UICheckPermission(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, Category = "ContentsSub")
		void GotoPermissionSelect();


	class UB2UIActionLogo* owner_;
	void SetOwnerUI(class  UB2UIActionLogo* owner);
	
private:
	TWeakObjectPtr<UB2Button> B2Button_0;	
	
};
