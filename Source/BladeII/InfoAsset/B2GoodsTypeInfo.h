// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "BladeII.h"
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
	int32 ShortcutUIPStringID; //官肺啊扁 UIP俊 甸绢哎 汲疙 胶飘傅
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
	int32 GoodsInfoStringID;	//犁拳啊 汲疙 扑诀俊 甸绢哎 胶飘傅
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ShortageInfoStringID; //犁拳啊 何练且锭 哆绰 扑诀俊 甸绢哎 胶飘傅

	//ItemInfo啊 磷瘤 臼绊 混酒 乐促绰 啊沥窍俊 iteminfo狼 ID甫 啊廉咳栏肺 咯扁辑绰 歹捞惑 鞘夸 窍瘤 臼澜
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//int32 ItemInfoDataTableID; //icon阑 啊廉棵 酒捞袍 单捞磐 抛捞喉狼 LocalizeNameKey

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FUB2GoodsShortcutButtonInfo> ShortcutInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUB2GoodsShortcutButtonInfo UseShortcutInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 LockTutorialID;		//酒捞袍 荤侩贸 譬配府倔 酒捞叼
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
