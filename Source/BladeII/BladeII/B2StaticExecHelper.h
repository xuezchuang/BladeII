// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
//#include "UObject/Object.h"
#include "B2StaticExecHelper.generated.h"

/**
 * To place various utility console command.
 */
UCLASS()
class BLADEII_API UB2StaticExecHelper : public UObject, public FSelfRegisteringExec
{
	GENERATED_BODY()
	
public:
	
	UB2StaticExecHelper(const FObjectInitializer& ObjectInitializer);

	virtual bool Exec(UWorld* Inworld, const TCHAR* Cmd, FOutputDevice& Ar) override;

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	// Not for dirty cheat stuff, some console command that needed for shipping build too.
	bool ExecFormal(UWorld* Inworld, const TCHAR* Cmd, FOutputDevice& Ar);
#endif

#if WITH_EDITOR
	bool ExecEditorOnly(UWorld* Inworld, const TCHAR* Cmd, FOutputDevice& Ar);
#endif

#if !UE_BUILD_SHIPPING
	/** Commands here works for mobile test version too. */
	bool ExecDev(UWorld* Inworld, const TCHAR* Cmd, FOutputDevice& Ar);
#endif
};
