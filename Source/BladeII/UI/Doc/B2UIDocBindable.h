// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
#include "UObject/Object.h"
#include "B2UIDocBase.h"
#include "B2UIDocBindable.generated.h"

/* This interface represents a document that explicitly binds available. 
 * and If the document is bound to the convenience of Get / Set. */
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UB2UIDocBindable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IB2UIDocBindable
{
	GENERATED_IINTERFACE_BODY()

public:
	/* This will be referred to later in B2UIManager in batches. */
	virtual void BindDocAuto(){}

	/* If you bind a document to prevent the release later. */
	virtual void UnbindDoc() = 0;

public:
	/* If the document is bound to the convenience of Get / Set. */
	void SetDoc(UB2UIDocBase* Doc) { BindedDoc = Doc; }
	UB2UIDocBase* GetDoc() const { return (BindedDoc.IsValid()) ? BindedDoc.Get() : nullptr; }
	int32 GetDocId() const	{ return (BindedDoc.IsValid()) ? BindedDoc->GetDocId() : -1; }

private:
	TWeakObjectPtr<UB2UIDocBase> BindedDoc;
};
