// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "B2UIEnum.h"
#include "Engine/DataTable.h"
#include "B2GoodsTypeInfo.generated.h"

USTRUCT(BlueprintType)
struct FUB2GoodsShortcutButtonInfo
{
	GENERATED_USTRUCT_BODY()

	FUB2GoodsShortcutButtonInfo()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EUIScene ShortcutSceneName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ShortcutScene_Option1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ShortcutScene_Option2;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ShortcutUIPStringID; //�ٷΰ��� UIP�� �� ���� ��Ʈ��
};
/**
* The actual data composing UB2GoodsTypeInfo.
*/
USTRUCT(BlueprintType)
struct FSingleGoodsTypeInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FSingleGoodsTypeInfoData()
	{
		FGoodsType = GoodsTypeFName::None;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName FGoodsType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GoodsInfoStringID;	//��ȭ�� ���� �˾��� �� ��Ʈ��
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ShortageInfoStringID; //��ȭ�� �����Ҷ� �ߴ� �˾��� �� ��Ʈ��

	//ItemInfo�� ���� �ʰ� ��� �ִٴ� �����Ͽ� iteminfo�� ID�� ���������� ���⼭�� ���̻� �ʿ� ���� ����
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//int32 ItemInfoDataTableID; //icon�� ������ ������ ������ ���̺��� LocalizeNameKey

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FUB2GoodsShortcutButtonInfo> ShortcutInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUB2GoodsShortcutButtonInfo UseShortcutInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 LockTutorialID;		//������ ���ó Ʃ�丮�� ���̵�
};
/**
 * 
 */
UCLASS()
class BLADEII_API UB2GoodsTypeInfo : public UObject
{
	GENERATED_BODY()
	
private:
protected:

	UPROPERTY()
	class UDataTable* TheData;

	TMap<FName, FSingleGoodsTypeInfoData*> m_TheDataMap;
public:

private:
protected:
public:
	UB2GoodsTypeInfo(const FObjectInitializer& ObjectInitializer);

	FSingleGoodsTypeInfoData* GetInfoData(FName _GoodsType);

	virtual void PostLoad() override;

#if WITH_EDITOR
	/** Check and pops up some message if something wrong. */
	void CheckInfoDataIntegrity();
#endif
	
	
};
