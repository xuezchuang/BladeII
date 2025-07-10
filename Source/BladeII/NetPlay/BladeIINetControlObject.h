#pragma once
//#include "BladeII.h"
#include "GameFramework/Actor.h"
#include "BladeIINetControlObject.generated.h"

class IState;

UENUM()
enum class EConquestAreaState : uint8
{
	None,
	Assault,
	OwnerRed,
	OwnerBlue,
};

UCLASS()
class BLADEII_API ABladeIINetControlObject : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DefaultCollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ViewLimitLength;
	
public:
	ABladeIINetControlObject(const FObjectInitializer& ObjectInitializer);

	// AActor interfaces...
	virtual void BeginPlay() final;
	virtual void Tick(float) final;
	virtual void BeginDestroy() final;
	virtual void Destroyed() final;

	// My interfaces...
	void DeleteInternal();
	void ChangeState(uint8 NewStateKey);
	void ApplyChangeState(uint8 NewStateKey, float fControlPointGageRed, float fControlPointGageBlue, bool bResetConquestGage);
	void ApplyCurrentStateToUI();

	void AddConquestGage(bool bRed, float fAmount);
	void AddControlPointGage(bool bRed, float fAmount);

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	float GetConquestGage(bool bRed){ return bRed ? m_fConquestGageRed : m_fConquestGageBlue; }

	float GetControlPointGage(bool bRed){ return bRed ? m_fControlPointGageRed : m_fControlPointGageBlue; }

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	EConquestAreaState GetConquestAreaState(){ return ConquestAreaState; }

	void SetConquestAreaState(EConquestAreaState NewState);
	void ApplyConquestAreaState(uint8 NewState, float fConquestGageRed, float fConquestGageBlue);
	
	void BroadCastCurrentState();

	void CheckToNotifyControlPoint(float DeltaSeconds);

	//void OnCharacterDead();

	float GetViewLimitLength(){ return ViewLimitLength; }

	bool IsOverlap(AActor* pTarget);

	bool IsMatchingConquestAreaTeam(ETeamType teamType);

	bool IsAssaultState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float ConquestGageMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float ControlPointGageMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float BonusTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float GageRateForTutorial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float NotifyControlPointTerm;

	void SetActiveControlObject(bool NewActive);

	bool IsActiveControlObject() { return bActiveControlObject; }

	bool IsObjectUpdateable();

	UFUNCTION(BlueprintCallable, Category = "BladeII")
	bool IsNeutralState();

	float GetBonusTimeGage();

	bool IsinBonusTime();

	ETeamType GetBonueTimeTeam() { return BonueTimeTeam; }

	float GetAreaRadius();

private:
	//UFUNCTION()
	//void OnInCheckConquestArea(UPrimitiveComponent* OverlappedComponent, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//
	//UFUNCTION()
	//void OnOutCheckConquestArea(UPrimitiveComponent* OverlappedComponent, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckOverlap();

	void SetBonusTime(ETeamType OwnerTeam);
	
	void UpdateBonusTimeGage(float DeltaSeconds);

private:
	IState*		ControledState;

	TMap<uint8, IState*>	StateMaps;

	EConquestAreaState ConquestAreaState;

	ETeamType BonueTimeTeam;

	float BonusTimeGage;

	float m_fConquestGageRed;
	float m_fConquestGageBlue;

	float m_fControlPointGageRed;
	float m_fControlPointGageBlue;

	float LimitControlPointForBonusTimeRed;
	float LimitControlPointForBonusTimeBlue;

	bool bWaitSendChangeControlState;
	bool bWaitSendSetConquestAreaState;

	bool bActiveControlObject;

	float m_fNotifyControlPointRemainTime;

public:
	bool bEndMatch;
};

const uint8 CONTROL_STATE_NEUTRAL		=	1;
const uint8 CONTROL_STATE_RED			=	2;
const uint8 CONTROL_STATE_BLUE			=	3;

class IState
{
public:
	virtual ~IState() {};
	virtual uint8 GetState() const = 0;
	virtual void Tick(ABladeIINetControlObject*, float) = 0;
};

class NeutralState : public IState
{
public:
	virtual uint8 GetState() const
	{
		return CONTROL_STATE_NEUTRAL;
	}

	virtual void Tick(ABladeIINetControlObject*, float) override;
};

class RedState : public IState
{
public:
	virtual uint8 GetState() const
	{
		return CONTROL_STATE_RED;
	}

	virtual void Tick(ABladeIINetControlObject*, float) override;
};

class BlueState : public IState
{
public:
	virtual uint8 GetState() const
	{
		return CONTROL_STATE_BLUE;
	}

	virtual void Tick(ABladeIINetControlObject*, float) override;
};