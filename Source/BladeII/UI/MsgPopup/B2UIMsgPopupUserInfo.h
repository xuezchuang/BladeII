// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once

#include "B2UIMsgPopupSimpleAddComment.h"
#include "B2UIMsgPopupUserInfo.generated.h"


UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2UIMsgPopupUserInfo : public UB2UIMsgPopupSimpleAddComment
{
	GENERATED_BODY()

protected:
	virtual void CacheAssets() override;

public:
	void SetUserInfo(UMaterialInstance* pIconMI, const FText& txtName, const FText& txtLevel, const FText& txtGuild);
	virtual void SetAddComment(const FText& comment) override;
protected:
	TWeakObjectPtr<UImage> IMG_Character;
	TWeakObjectPtr<UTextBlock> TB_Guild;
	TWeakObjectPtr<UTextBlock> TB_PlayerLevel;
	TWeakObjectPtr<UTextBlock> TB_PlayerName;
	TWeakObjectPtr<UB2RichTextBlock> TB_AddComment;

};
