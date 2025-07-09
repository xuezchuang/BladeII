// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIMsgPopupSimple.h"
#include "B2UIMsgPopupSimpleAddComment.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMsgPopupSimpleAddComment : public UB2UIMsgPopupSimple
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;

public:
	virtual void SetAddComment(const FText& comment);

protected:
	TWeakObjectPtr<UB2RichTextBlock> TB_AddComment;
};
