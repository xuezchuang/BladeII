// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "UObject/Object.h"
#include "B2UIEnum.h"
#include "B2UIDocBase.h"
#include "Tickable.h"
#include "B2UIDocManager.generated.h"

/*
* Unique key of the document.
* It is made By Type and ID.
*/
class BLADEII_API FB2UIDocKey
{
	friend class UB2UIDocManager;

public:
	FB2UIDocKey() {}
	FB2UIDocKey(const FB2UIDocKey& Other)								{ KeyValue = Other.KeyValue; }
	FB2UIDocKey(EUIDocType InType, int32 InId = INDEX_NONE)
	{
		*((int32*)&KeyValue) = (int32)InType;	//The upper 32-int type.
		*((int32*)&KeyValue + 1) = InId;		//The lower 32 int ID.
	}

public:
	FB2UIDocKey& operator=(const FB2UIDocKey& Other) { KeyValue = Other.KeyValue; return *this; }
	bool operator==(const FB2UIDocKey& rhs) { return KeyValue == rhs.KeyValue; }
	bool operator!=(const FB2UIDocKey& rhs) { return KeyValue != rhs.KeyValue; }
	friend bool operator==(const FB2UIDocKey& A, const FB2UIDocKey& B) { return A.KeyValue == B.KeyValue; }

public:
	EUIDocType GetType() { return (EUIDocType)(*((int32*)&KeyValue));; }
	int32 GetId() { return *((int32*)&KeyValue + 1); }

private:
	void SetId(int32 InValue) { *((int32*)&KeyValue + 1) = InValue; }

private:
	int64 KeyValue;
};


/*
* Manager of UIDocs. (Singleton)
*/

UCLASS(Transient)
class BLADEII_API UB2UIDocManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	/* The interface of the singleton pattern. 
	 * !!! NEVER assume that it will always return a valid object. */
	static class UB2UIDocManager* GetInstance();
	
	void Init(class ABladeIIGameMode* InGM);
	/** A part of Init. It is about load only necessary PCClassInfo to get benefit of TAsset lazy-loading */
	void InitResourcesOnNecessaryHeroDoc(class ABladeIIGameMode* InGM);
	void Shutdown();

	/* interface of FTickableGameObject. */
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickableWhenPaused() const override;
	virtual bool IsTickableInEditor() const override;


	template <typename T>
	T* CreateDoc(FB2UIDocKey& DocKey)
	{
		auto NewDoc = NewObject<T>(GetTransientPackage());

		if (NewDoc->bMultiple && !NewDoc->bAnonymous)
		{
			auto Found = MultipleDocs.Find(DocKey.KeyValue);
			if (Found)
			{
				(*Found)->Init();
				return  Cast<T>(*Found);
			}
		}
		else
		{
			if (!IsExistSingleDoc(DocKey.GetType()))
				return nullptr;
		}

		if (!Cast<class UB2UIDocBase>(NewDoc))
			return nullptr;

		if (NewDoc->bMultiple)
		{
			if (NewDoc->bAnonymous)
				DocKey.SetId(++AnonymousIdCount);

			MultipleDocs.Add(DocKey.KeyValue, NewDoc);
		}
		else
		{
			SingleDoc.Add(DocKey.KeyValue, NewDoc);
		}

		NewDoc->DocId = DocKey.GetId();
		NewDoc->Init();
		return NewDoc;
	}

	template <typename T>
	T* GetDoc(const FB2UIDocKey& DocKey)
	{
		auto Found = SingleDoc.Find(DocKey.KeyValue);
		if (Found)
			return Cast<T>(*Found);

		auto Found2 = MultipleDocs.Find(DocKey.KeyValue);
		if (Found2)
			return Cast<T>(*Found2);

		return nullptr;
	}
	


private:
	UB2UIDocManager(){}
	~UB2UIDocManager();
	bool IsExistSingleDoc(EUIDocType Type);

private:
	/* The Instance of singleton. */
	static class UB2UIDocManager* Instance;

	/* It is needed to prevent duplicate initialization. */
	bool bInit = false;

	/* Counters for the documents in the case anonymous. */
	int32 AnonymousIdCount = INDEX_NONE;

	/* This is a document that exists in the container one. */
	UPROPERTY(Transient)
	TMap<int64, class UB2UIDocBase*> SingleDoc;

	/* This is a container for documents available several are present. */
	UPROPERTY(Transient)
	TMap<int64, class UB2UIDocBase*> MultipleDocs;
};
