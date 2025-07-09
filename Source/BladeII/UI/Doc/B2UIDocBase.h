// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square

#pragma once
//#include "UObject/Object.h"
#include "B2UIEnum.h"
#include "B2UIDocBase.generated.h"

/*This is a macro for creating the document data field.
* It has a built-Getter/Setter and Delegate for Broadcast.
* When the data is changed to the side of the ->SetName(Value), and the reception is handled using AddUObject.
* 
* Send Side		:  Doc->SetName(Value);
* Receive Side	:  Doc->OnNameChanged.AddUObject(this, &ThisObj::OnNameHandler);	
*																	--> (UB2UIDocBase* Sender, Type Value, Type PreValue)
*/
#define DECLARE_DOC_FIELD(CLASSPOINTER, TYPE,NAME)														\
		public:																							\
		DECLARE_MULTICAST_DELEGATE_ThreeParams(FOn##NAME##Changed, CLASSPOINTER, TYPE ,TYPE);			\
		FOn##NAME##Changed On##NAME##Changed;															\
		void Set##NAME( TYPE Value )																	\
		{																								\
			TYPE PreValue = NAME;																		\
			NAME = Value;																				\
			On##NAME##Changed.Broadcast(this, Value, PreValue);											\
		}																								\
		TYPE Get##NAME() const																			\
		{																								\
			return NAME;																				\
		}																								\
		protected:																						\
		TYPE NAME;

#define DECLARE_DOC_STRUCT_FIELD(CLASSPOINTER, TYPE,NAME)														\
public:																											\
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOn##NAME##Changed, CLASSPOINTER, const TYPE&, const TYPE&);			\
		FOn##NAME##Changed On##NAME##Changed;																	\
		void Set##NAME(const TYPE& Value )																		\
		{																										\
			TYPE PreValue = NAME;																				\
			NAME = Value;																						\
			On##NAME##Changed.Broadcast(this, Value, PreValue);													\
		}																										\
		const TYPE& Get##NAME() const																			\
		{																										\
			return NAME;																						\
		}																										\
		protected:																								\
		TYPE NAME;

#define DECLARE_UIDOC_FIELD(TYPE,NAME)				DECLARE_DOC_FIELD(class UB2UIDocBase*, TYPE,NAME)
#define DECLARE_UIDOC_STRUCT_FIELD(TYPE,NAME)		DECLARE_DOC_STRUCT_FIELD(class UB2UIDocBase*, TYPE,NAME)

// ----------------------------------------------------------------------------------
// UIDOCFIELD Multi Field
// 
// UIDOCFIELD TwoField
#define DECLARE_UIDOC_FIELD_TwoField(TYPE,DECLARENAME,DEFAULTVALUE)				\
	DECLARE_UIDOC_FIELD(TYPE, DECLARENAME##0)									\
	DECLARE_UIDOC_FIELD(TYPE, DECLARENAME##1)									\
	public:																		\
	TYPE Get##DECLARENAME##ByIndex(int32 Idx)									\
	{																			\
		switch (Idx)															\
		{																		\
		case 0: return Get##DECLARENAME##0();									\
		case 1: return Get##DECLARENAME##1();									\
		}																		\
		return DEFAULTVALUE;													\
	}																			\
	void Set##DECLARENAME##ByIndex(int32 Idx, TYPE Value)						\
	{																			\
		switch (Idx)															\
		{																		\
			case 0: Set##DECLARENAME##0(Value);	break;							\
			case 1: Set##DECLARENAME##1(Value);	break;							\
		}																		\
	}

// UIDOCFIELD Three Field
#define DECLARE_UIDOC_FIELD_ThreeField(TYPE, DECLARENAME, DEFAULTVALUE)			\
	DECLARE_UIDOC_FIELD(TYPE, DECLARENAME##0)									\
	DECLARE_UIDOC_FIELD(TYPE, DECLARENAME##1)									\
	DECLARE_UIDOC_FIELD(TYPE, DECLARENAME##2)									\
	public:																		\
	TYPE Get##DECLARENAME##ByIndex(int32 Idx)									\
	{																			\
		switch (Idx)															\
		{																		\
		case 0: return Get##DECLARENAME##0();									\
		case 1: return Get##DECLARENAME##1();									\
		case 2: return Get##DECLARENAME##2();									\
		}																		\
		return DEFAULTVALUE;													\
	}																			\
	void Set##DECLARENAME##ByIndex(int32 Idx, TYPE Value)						\
	{																			\
		switch (Idx)															\
		{																		\
			case 0: Set##DECLARENAME##0(Value);	break;							\
			case 1: Set##DECLARENAME##1(Value);	break;							\
			case 2: Set##DECLARENAME##2(Value);	break;							\
		}																		\
	}

// UIDOCFIELD FourField
#define DECLARE_UIDOC_FIELD_FourField(TYPE,DECLARENAME,DEFAULTVALUE)			\
	DECLARE_UIDOC_FIELD(TYPE, DECLARENAME##0)									\
	DECLARE_UIDOC_FIELD(TYPE, DECLARENAME##1)									\
	DECLARE_UIDOC_FIELD(TYPE, DECLARENAME##2)									\
	DECLARE_UIDOC_FIELD(TYPE, DECLARENAME##3)									\
	public:																		\
	TYPE Get##DECLARENAME##ByIndex(int32 Idx)									\
	{																			\
		switch (Idx)															\
		{																		\
		case 0: return Get##DECLARENAME##0();									\
		case 1: return Get##DECLARENAME##1();									\
		case 2: return Get##DECLARENAME##2();									\
		case 3: return Get##DECLARENAME##3();									\
		}																		\
		return DEFAULTVALUE;													\
	}																			\
	void Set##DECLARENAME##ByIndex(int32 Idx, TYPE Value)						\
	{																			\
		switch (Idx)															\
		{																		\
			case 0: Set##DECLARENAME##0(Value);	break;							\
			case 1: Set##DECLARENAME##1(Value);	break;							\
			case 2: Set##DECLARENAME##2(Value);	break;							\
			case 3: Set##DECLARENAME##3(Value);	break;							\
		}																		\
	}

#define SET_UIDOC_FIELD_TwoField(DECLARENAME,DEFAULTVALUE)						\
	Set##DECLARENAME##0(DEFAULTVALUE);											\
	Set##DECLARENAME##1(DEFAULTVALUE);

#define SET_UIDOC_FIELD_ThreeField(DECLARENAME,DEFAULTVALUE)					\
	Set##DECLARENAME##0(DEFAULTVALUE);											\
	Set##DECLARENAME##1(DEFAULTVALUE);											\
	Set##DECLARENAME##2(DEFAULTVALUE);

#define SET_UIDOC_FIELD_FourField(DECLARENAME,DEFAULTVALUE)						\
	Set##DECLARENAME##0(DEFAULTVALUE);											\
	Set##DECLARENAME##1(DEFAULTVALUE);											\
	Set##DECLARENAME##2(DEFAULTVALUE);											\
	Set##DECLARENAME##3(DEFAULTVALUE);


/*
* Base class of UIDocument.
*/

UCLASS(Transient)
class BLADEII_API UB2UIDocBase : public UObject
{
	GENERATED_BODY()

	friend class UB2UIDocManager;

public:
	UB2UIDocBase(const FObjectInitializer& ObjectInitializer);
	~UB2UIDocBase();

	virtual void Init(){}
	virtual void Destroy();

	int32 GetDocId() const { return DocId; }

protected:
	/* Single document or not.(like Singleton) */
	bool bMultiple = false;

	/* Anonymous document or not.(not use a specific identity) */
	bool bAnonymous = false;

	EUIDocType DocType;
	int32 DocId;
};