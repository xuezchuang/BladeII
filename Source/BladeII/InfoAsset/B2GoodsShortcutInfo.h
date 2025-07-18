// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UObject/NoExportTypes.h"
#include "B2UIEnum.h"
#include "BladeII.h"
#include "UObject/ObjectSaveContext.h"
#include "B2GoodsShortcutInfo.generated.h"

/*
USTRUCT()
struct FB2GoodsShortcutPopupConditionInfo
{
	GENERATED_USTRUCT_BODY()

	FB2GoodsShortcutPopupConditionInfo()
	//	:GoodsType(EGoodsType::EGoodsType_None)
	{}
	
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//EGoodsType GoodsType;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//int32 GoodsAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GoodsPopupStringID; //재화가 부족할때 뜨는 팝업에 들어갈 스트링
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ItemInfoDataTableID; //icon을 가져올 아이템 데이터 테이블의 LocalizeNameKey
};
*/

USTRUCT(BlueprintType)
struct FB2GoodsShortcutIndividualInfo
{
	GENERATED_USTRUCT_BODY()

	FB2GoodsShortcutIndividualInfo()
	: ShortcutInfoID(0)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FName> FGoodsType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ShortcutInfoID;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BLADEII_API UB2GoodsShortcutInfo : public UObject
{
	GENERATED_BODY()
	
//Function
private:
protected:
public:
	UB2GoodsShortcutInfo(const FObjectInitializer& ObjectInitializer);
	
	virtual void PostLoad() override;
	FORCEINLINE FB2GoodsShortcutIndividualInfo* GetInfoData(int32 _shortcutInfoID) { return m_GoodsShortcutInfoMap.Find(_shortcutInfoID); };
/////////////////////////////////////////////////////////////////////////////////////////////
//[@AKI, 170331] 이 아래 것들은 B2SomeInfo것을 가지고 온 것임.
	//
	// To be added..
	//

	/** Call this whenever a stage is about to be unloaded */
	void UnloadAll();
#if WITH_EDITOR
	/** For safe use of TSoftClassPtr in editor. */
	void EditorLoadAll();
#endif

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP // Clean-up loaded TAsset references. They are temporary code until formally fix wrong handling of lazy-loaded info asset.
	void CleanupOnPreSave();
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif
/////////////////////////////////////////////////////////////////////////////////////////////

//Variable
private:
protected:
	UPROPERTY(EditAnywhere)
	TArray<FB2GoodsShortcutIndividualInfo> ShortcutInfoArray;

	TMap<int32, FB2GoodsShortcutIndividualInfo> m_GoodsShortcutInfoMap;
public:

};
