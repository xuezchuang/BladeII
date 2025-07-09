// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/B2UIWidget.h"
#include "B2UIGoodsShortcutParts.generated.h"

/**
 * 
 */
UCLASS()
class BLADEII_API UB2UIGoodsShortcutParts : public UB2UIWidget
{
	GENERATED_BODY()
	
	
//Functions
public:
	UB2UIGoodsShortcutParts(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION()
	void CacheAssets();
	UFUNCTION()
	void SetGoodsShortcutInfo(uint32 _InfoStringID, EUIScene _LinkScene, int32 _Optional1 = 0, int32 _Optional2 = 0);
	void SetVisibleMoveButton(bool IsMovable = false);
	void SetVisibleButton(bool IsVisible);

	//�θ� Ŭ������ ���� �� ���� ������....���� ��� �θ� �����Դ� ��� ġ�� ����...�׳� �Ѱ� ����
	FORCEINLINE void SetParentUI(class UB2UIGoodsShortcutTooltipPopup* const _parentUI) { m_ParentUI = _parentUI; };
protected:
	//FSingleGoodsTypeInfoData* GetGoodsInfoStringFromGoodsInfoStringTableObject(int32 _InfoStringID);

	virtual void BindData();


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Delegate or Event
	virtual void BindDelegates() override;

	UFUNCTION()
	void OnClickBtnMove();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:
	
//Variable
public:
protected:
	TWeakObjectPtr<UButton> BTN_Positive;
	TWeakObjectPtr<UButton> BTN_Lock;

	TWeakObjectPtr<UB2RichTextBlock> TB_AddComment;

	uint32		m_InfoStringID;
	EUIScene	m_LinkScene;
	int32		m_Optional1;
	int32		m_Optional2;

	class UB2UIGoodsShortcutTooltipPopup* m_ParentUI;
private:
	
	
};
