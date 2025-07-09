// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "Event.h"
//#include "B2StageEventDirector.h"
//#include "B2AutoWayPoint.h"
#include "B2JumpEventZone.h"




void AB2JumpEventZone::BeginPlay()
{
	Super::BeginPlay();
	CurrentInteractActor = nullptr;

	/** [�����÷��̾���] �� [��Ƽ�Ͼ���] �� Ʈ���� �켱������ ���� ���� ���� ������, ��������Ʈ ���� ��ŷ ����� �������� �̺�Ʈ ���ͷ� ����.
	SubscribeStageEventDirector();
	*/
}

void AB2JumpEventZone::EndInteract()
{
	Super::EndInteract();
	CurrentInteractActor = nullptr;

	/** [�����÷��̾���] �� [��Ƽ�Ͼ���] �� Ʈ���� �켱������ ���� ���� ���� ������, ��������Ʈ ���� ��ŷ ����� �������� �̺�Ʈ ���ͷ� ����.
	UnSubscribeStageEventDirector();
	*/
}

void AB2JumpEventZone::SubscribeStageEventDirector()
{
	CAPTURE_UOBJECT(AB2JumpEventZone);

	//StageEventSceneBeginTicket = StageEventSceneBeginClass<AB2StageEventDirector*, EStageEvent>::GetInstance().Subscribe(
	//	USE_CAPTURE_OBJECT_AND_TICKET(StageEventSceneBegin, AB2StageEventDirector* ed, EStageEvent et)
	//	Capture->NotifyStageEventSceneBegin(ed, et);
	//	END_CAPTURE_OBJECT()
	//	);
}

void AB2JumpEventZone::UnSubscribeStageEventDirector()
{
	//StageEventSceneBeginClass<AB2StageEventDirector*, EStageEvent>::GetInstance().Unsubscribe(StageEventSceneBeginTicket);
	//StageEventSceneBeginTicket = 0;
}

void AB2JumpEventZone::BeginInteract(class AActor* InInteractingActor)
{
	CurrentInteractActor = InInteractingActor;

	Super::BeginInteract(InInteractingActor);
}

void AB2JumpEventZone::NotifyStageEventSceneBegin(class AB2StageEventDirector* BegunDirector, EStageEvent EventType)
{
	//if (BegunDirector && OwnerEventDirectors.Find(BegunDirector) != INDEX_NONE)
	//{
	//	BeginInteract(BegunDirector->GetB2LocalPlayerChar());
	//}
}

void AB2JumpEventZone::InteractAction()
{
	/** [�����÷��̾���] �� [��Ƽ�Ͼ���] �� Ʈ���� �켱������ ���� ���� ���� ������, ��������Ʈ ���� ��ŷ ����� �������� �̺�Ʈ ���ͷ� ����.
	if (auto* LocalPlayer = Cast<ABladeIIPlayer>(CurrentInteractActor))
		if (LocalPlayer && TriggerShareWayPoint != nullptr)
			TriggerShareWayPoint->ForceMarkByJumpEvent(this, LocalPlayer);
	*/

	Super::InteractAction();
}