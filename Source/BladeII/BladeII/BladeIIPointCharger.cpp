#include "BladeIIPointCharger.h"
//#include "Event.h"
////#include "Retailer.h"
//#include "B2UIDocHelper.h"
//
//
//
//#include "BladeIIGameImpl.h"
//
//FBladeIIChargeBladePointCharger& FBladeIIChargeBladePointCharger::GetBladeIIPointCharger()
//{
//	static FBladeIIChargeBladePointCharger BladeIIPointCharger;
//	BladeIIPointCharger.PointType = EPointChargeType::EPointChargeType_BladePoint;
//	return BladeIIPointCharger;
//}
//
//FBladeIIChargeBladePointCharger& FBladeIIChargeBladePointCharger::GetPvPMatchPointCharger()
//{
//	static FBladeIIChargeBladePointCharger PvpMatchPointCharger;
//	PvpMatchPointCharger.PointType = EPointChargeType::EPointChargeType_PvPMatchPoint;
//	return PvpMatchPointCharger;
//}
//
//FBladeIIChargeBladePointCharger& FBladeIIChargeBladePointCharger::GetTeamMatchPointCharger()
//{
//	static FBladeIIChargeBladePointCharger TeamMatchPointCharger;
//	TeamMatchPointCharger.PointType = EPointChargeType::EPointChargeType_TeamMatchPoint;
//	return TeamMatchPointCharger;
//}
//
//FBladeIIChargeBladePointCharger::FBladeIIChargeBladePointCharger()
//	:bInitialized(false), BeganRealTime(0.f)
//{
//	DeliveryCheckChargePointsClass<FB2CheckChargePointsPtr>::GetInstance().Subscribe([this](const FB2CheckChargePointsPtr& InChargePoints)
//	{
//		this->ResponseCheckChargePoints(InChargePoints);
//	});
//}
//
//
//FBladeIIChargeBladePointCharger::~FBladeIIChargeBladePointCharger()
//{
//	DeliveryCheckChargePointsClass<FB2CheckChargePointsPtr>::GetInstance().UnsubscribeAll();
//}
//
//void FBladeIIChargeBladePointCharger::Initialize()
//{
//	BeganRealTime = FApp::GetCurrentTime();
//
//	bInitialized = true;
//}
//
//void FBladeIIChargeBladePointCharger::ResponseCheckChargePoints(const FB2CheckChargePointsPtr& InChargePoints)
//{
//	//요청한 값이 아니면 -9999
//	//switch(PointType)
//	//{
//	//	case EPointChargeType::EPointChargeType_BladePoint:
//	//		if (InChargePoints->blade_point != -9999)
//	//		{
//	//			//Update data
//	//			BladeIIGameImpl::GetClientDataStore().BladePointCharge(InChargePoints->blade_point, InChargePoints->blade_point_max, InChargePoints->blade_point_charge_lefttime_second);
//	//			//Update relevant UI
//	//			ChangeStaminaChargeTimeClass<>::GetInstance().Signal();
//	//		}
//	//	break;
//	//	case EPointChargeType::EPointChargeType_PvPMatchPoint:
//	//		if (InChargePoints->pvp_match_point != -9999)
//	//		{
//	//			//Update data
//	//			if (auto* DocPvpMatch = UB2UIDocHelper::GetDocPVP1on1Rival())
//	//			{
//	//				DocPvpMatch->SetMatchPoint(InChargePoints->pvp_match_point);
//	//				DocPvpMatch->SetNextChargeSecond(InChargePoints->pvp_match_point_charge_lefttime_second);
//	//			}
//	//			//Update relevant UI
//	//			ChangePvPMatchPointChargeTimeClass<>::GetInstance().Signal();
//	//		}
//	//		break;
//	//	case EPointChargeType::EPointChargeType_TeamMatchPoint:
//	//		if (InChargePoints->team_match_point != -9999)
//	//		{
//	//			//Update data
//	//			if (auto* DocTeamMatch = UB2UIDocHelper::GetDocTeamMatch())
//	//			{
//	//				DocTeamMatch->SetTeamMatchPoint(InChargePoints->team_match_point);
//	//				DocTeamMatch->SetNextChargeSecond(InChargePoints->team_match_point_charge_lefttime_second);
//	//			}
//
//	//			//Update relevant UI
//	//			ChangeTeamMatchPointChargeTimeClass<>::GetInstance().Signal();
//	//		}
//	//		break;
//	//}
//	Initialize();
//}
//
//void FBladeIIChargeBladePointCharger::Tick(float DeltaTime)
//{
//	if (!bInitialized)
//		return;
//	
//	if(PointType == EPointChargeType::EPointChargeType_PvPMatchPoint)
//		UpdateRemainTime_PvPMatchPoint();
//	else if(PointType == EPointChargeType::EPointChargeType_TeamMatchPoint)
//		UpdateRemainTime_TeamMatchPoint();
//	else
//		UpdateRemainTime_BladePoint();
//}
//
//void FBladeIIChargeBladePointCharger::UpdateRemainTime_BladePoint()
//{
//	double LeftChargeTime = BladeIIGameImpl::GetClientDataStore().GetBladePointChargingLeftTime();
//	double ElapsedChargeTime = (FApp::GetCurrentTime() - BeganRealTime);
//	int32 CurrentLeftChargeTime = FMath::CeilToInt(LeftChargeTime - ElapsedChargeTime);
//
//	if (LeftChargeTime > 0.f && ElapsedChargeTime > LeftChargeTime)
//		RequestCheckBladePointCharge();
//
//	auto* DocUser = UB2UIDocHelper::GetDocUser();
//	if (DocUser)
//		DocUser->SetNextTimeStaminaGet(FMath::Clamp<int32>(CurrentLeftChargeTime, 0, LeftChargeTime));
//}
//
//void FBladeIIChargeBladePointCharger::UpdateRemainTime_PvPMatchPoint()
//{
//	auto* DocPvp = UB2UIDocHelper::GetDocPVP1on1Rival();
//	if (DocPvp)
//	{
//		double LeftChargeTime = DocPvp->GetNextChargeSecond();
//		double ElapsedChargeTime = (FApp::GetCurrentTime() - BeganRealTime);
//		int32 CurrentLeftChargeTime = FMath::CeilToInt(LeftChargeTime - ElapsedChargeTime);
//
//		if (LeftChargeTime > 0.f && ElapsedChargeTime > LeftChargeTime)
//			this->RequestCheckPvPMatchPointCharge();
//
//		DocPvp->SetNextTimeMatchPointGet(FMath::Clamp<int32>(CurrentLeftChargeTime, 0, LeftChargeTime));
//	}
//}
//
//void FBladeIIChargeBladePointCharger::UpdateRemainTime_TeamMatchPoint()
//{
//	auto* DocTeam = UB2UIDocHelper::GetDocTeamMatch();
//	if (DocTeam)
//	{
//		double LeftChargeTime = DocTeam->GetNextChargeSecond();
//		double ElapsedChargeTime = (FApp::GetCurrentTime() - BeganRealTime);
//		int32 CurrentLeftChargeTime = FMath::CeilToInt(LeftChargeTime - ElapsedChargeTime);
//
//		if (LeftChargeTime > 0.f && ElapsedChargeTime > LeftChargeTime)
//			this->RequestCheckTeamMatchPointCharge();
//
//		DocTeam->SetNextTimeTeamMatchPointGet(FMath::Clamp<int32>(CurrentLeftChargeTime, 0, LeftChargeTime));
//
//	}
//}
//
//void FBladeIIChargeBladePointCharger::RequestCheckBladePointCharge()
//{
//	bInitialized = false;
//
//	data_trader::Retailer::GetInstance().RequestCheckChargePoints(true, false, false);
//}
//
//void FBladeIIChargeBladePointCharger::RequestCheckPvPMatchPointCharge()
//{
//	bInitialized = false;
//
//	data_trader::Retailer::GetInstance().RequestCheckChargePoints(false, true, false);
//}
//
//void FBladeIIChargeBladePointCharger::RequestCheckTeamMatchPointCharge()
//{
//	bInitialized = false;
//	data_trader::Retailer::GetInstance().RequestCheckChargePoints(false, false, true);
//}
//
//TStatId FBladeIIChargeBladePointCharger::GetStatId() const
//{
//	RETURN_QUICK_DECLARE_CYCLE_STAT(FBladeIIChargeBladePointCharger, STATGROUP_Tickables);
//}
//
//int32 FBladeIIChargeBladePointCharger::GetLeftChargeTime()
//{
//	if (!bInitialized)
//		return 0;
//
//	int32 nLeftTime = 0;
//
//	if (PointType == EPointChargeType::EPointChargeType_PvPMatchPoint)
//	{
//		auto* DocPvp = UB2UIDocHelper::GetDocPVP1on1Rival();
//		if (DocPvp)
//			nLeftTime = DocPvp->GetNextChargeSecond();
//
//	}
//	else if (PointType == EPointChargeType::EPointChargeType_TeamMatchPoint)
//	{
//		auto* DocTeam = UB2UIDocHelper::GetDocTeamMatch();
//		if (DocTeam)
//			nLeftTime = DocTeam->GetNextChargeSecond();
//	}
//	else
//	{
//		nLeftTime = BladeIIGameImpl::GetClientDataStore().GetBladePointChargingLeftTime();
//	}
//
//	return nLeftTime - FMath::CeilToInt(FApp::GetCurrentTime() - BeganRealTime);
//}