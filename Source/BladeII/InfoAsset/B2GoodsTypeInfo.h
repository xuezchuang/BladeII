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
	int32 ShortcutUIPStringID; //바로가기 UIP에 들어갈 설명 스트링
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
	int32 GoodsInfoStringID;	//재화가 설명 팝업에 들어갈 스트링
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ShortageInfoStringID; //재화가 부족할때 뜨는 팝업에 들어갈 스트링

	//ItemInfo가 죽지 않고 살아 있다는 가정하에 iteminfo의 ID를 가져옴으로 여기서는 더이상 필요 하지 않음
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//int32 ItemInfoDataTableID; //icon을 가져올 아이템 데이터 테이블의 LocalizeNameKey

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FUB2GoodsShortcutButtonInfo> ShortcutInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUB2GoodsShortcutButtonInfo UseShortcutInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 LockTutorialID;		//아이템 사용처 튜토리얼 아이디
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
