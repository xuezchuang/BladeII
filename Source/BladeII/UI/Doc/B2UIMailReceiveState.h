// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "B2MailTimer.h"
/**
*
*/
class  B2UIMailReceiveState 
{
public:
	B2UIMailReceiveState() {}
	virtual ~B2UIMailReceiveState() {}
	virtual void OpenMail(int64 MailId, const FB2ResponseOpenMailPtr& MailInfo);
	virtual void OpenMailError(const int32 ErrorCode, const FB2ResponseOpenMailPtr MailInfo);
	virtual void OpenAllMail(const FB2OpenAllMail& OpenedAllMail);
	virtual void OpenAllMailError(const int32 ErrorCode);
	virtual void ReceiveAllBTN(const TArray<TWeakObjectPtr<class UB2UIMailContent>>& InMailContents, MailTabIndex InCurrentTabIndex);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class  B2UIMailReceiveBasicState : public B2UIMailReceiveState
{
public:
	virtual void OpenAllMail(const FB2OpenAllMail& OpenedAllMail) override;
	virtual void ReceiveAllBTN(const TArray<TWeakObjectPtr<class UB2UIMailContent>>& InMailContents, MailTabIndex	InCurrentTabIndex) override;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class  B2UIMailReceiveAllState : public B2UIMailReceiveState
{
public:
	B2UIMailReceiveAllState();

	virtual void OpenMail(int64 MailId, const FB2ResponseOpenMailPtr& MailInfo) override;
	virtual void OpenMailError(const int32 ErrorCode, const FB2ResponseOpenMailPtr MailInfo) override;

protected:
	virtual void OpenAllMailReceiveData() = 0;
	
protected:
	UB2MailTimer MailTimer;
	TArray<int64> HaveItemMailID;
	FB2ResponseOpenAllMail AllMailReceiveData;
	bool IsAllReceive;
	EPCClass SelectedEPCClass;
	int32 SelectedItemRefId;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class  B2UIMailReceiveGiftState : public B2UIMailReceiveAllState
{
public:
	virtual void OpenAllMail(const FB2OpenAllMail& OpenedAllMail) override;
	virtual void OpenAllMailError(const int32 ErrorCode) override;
	virtual void ReceiveAllBTN(const TArray<TWeakObjectPtr<class UB2UIMailContent>>& InMailContents, MailTabIndex InCurrentTabIndex) override;

protected:
	virtual void OpenAllMailReceiveData() override;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class B2UIMailReceiveTicketState : public B2UIMailReceiveAllState
{
public:
	B2UIMailReceiveTicketState();
	~B2UIMailReceiveTicketState() override;
	virtual void ReceiveAllBTN(const TArray<TWeakObjectPtr<class UB2UIMailContent>>& InMailContents, MailTabIndex InCurrentTabIndex) override;

protected:
	virtual void OpenAllMailReceiveData() override;
};