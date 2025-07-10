#include "B2UIDocPVP1on1Rival.h"
//#include "BladeIIPointCharger.h"
#include "Event.h"


UB2UIDocPVP1on1Rival::UB2UIDocPVP1on1Rival(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetCanViewModRewardBoxConfirm(true);
}

UB2UIDocPVP1on1Rival::~UB2UIDocPVP1on1Rival()
{

}

void UB2UIDocPVP1on1Rival::Init()
{
	CAPTURE_UOBJECT(UB2UIDocPVP1on1Rival);

	//ChangePvPMatchPointChargeTimeTicket = ChangePvPMatchPointChargeTimeClass<>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(ChangePvPMatchPointChargeTime)
	//	Capture->UpdateTagMatchData();
	//END_CAPTURE_OBJECT()
	//	);

	UpdateTagMatchData();
}

void UB2UIDocPVP1on1Rival::Destroy()
{
	//ChangePvPMatchPointChargeTimeClass<>::GetInstance().Unsubscribe(ChangePvPMatchPointChargeTimeTicket);
}

void UB2UIDocPVP1on1Rival::UpdateTagMatchData()
{
	//SetNextTimeMatchPointGet(FBladeIIChargeBladePointCharger::GetPvPMatchPointCharger().GetLeftChargeTime());
}

void UB2UIDocPVP1on1Rival::ClearInvitationData()
{
	IsFriendshipGame = false;
	IsInviteeReady = false;
	IsInviterReady = false;
	IsFrienshipGameValidSet = false;
	IsInvitee = false;
}
