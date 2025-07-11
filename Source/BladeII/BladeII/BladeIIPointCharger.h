#pragma once

//#include "EngineMinimal.h"
//#include "CommonStruct.h"
//#include "Tickable.h"
//
//enum class EPointChargeType : uint8
//{
//	EPointChargeType_BladePoint,
//	EPointChargeType_PvPMatchPoint,
//	EPointChargeType_TeamMatchPoint,
//	EPointChargeType_None
//};
//
//class FBladeIIChargeBladePointCharger : public FTickableGameObject
//{
//public:
//	FBladeIIChargeBladePointCharger();
//	~FBladeIIChargeBladePointCharger();
//
//	virtual void Tick(float DeltaTime);
//	virtual bool IsTickable() const { return bInitialized; }
//
//	virtual bool IsTickableWhenPaused() const { return true; }
//	virtual bool IsTickableInEditor() const { return true; }
//
//	virtual TStatId GetStatId() const;
//
//	void Initialize();
//
//	void UpdateRemainTime_BladePoint();
//	void UpdateRemainTime_PvPMatchPoint();
//	void UpdateRemainTime_TeamMatchPoint();
//
//	void ResponseCheckChargePoints(const FB2CheckChargePointsPtr& InChargePoints);
//
//	int32 GetLeftChargeTime();
//
//	void RequestCheckBladePointCharge();
//	void RequestCheckPvPMatchPointCharge();
//	void RequestCheckTeamMatchPointCharge();
//	FBladeIIChargeBladePointCharger& GetInstance();
//
//	static FBladeIIChargeBladePointCharger& GetBladeIIPointCharger();
//	static FBladeIIChargeBladePointCharger& GetPvPMatchPointCharger();
//	static FBladeIIChargeBladePointCharger& GetTeamMatchPointCharger();
//
//	void SetbInitialized(bool InVlaue) { bInitialized = InVlaue; }
//
//private:
//	bool bInitialized;
//
//	EPointChargeType PointType = EPointChargeType::EPointChargeType_None;
//
//	double BeganRealTime;
//
//};