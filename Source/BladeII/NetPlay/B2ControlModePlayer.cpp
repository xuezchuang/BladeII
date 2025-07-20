
#include "B2ControlModePlayer.h"

#include "Event.h"
#include "Packet.h"
#include "PacketMaker.h"
#include "BladeIIPlayerAIController.h"
#include "B2ControlGameMode.h"

AB2ControlModePlayer::AB2ControlModePlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MyAIController = nullptr;
	bRequestStartQTE = false;
}

void AB2ControlModePlayer::BeginPlay()
{
	Super::BeginPlay();	
}

void AB2ControlModePlayer::Tick(float delta)
{
	Super::Tick(delta);
}

void AB2ControlModePlayer::StartQTE(bool bForce/* = false*/)
{
	// 霸烙葛靛 烹秦辑 龋胶飘茄抛 夸没窍备 倾啊罐栏搁 SuperStartQTE() 捞吧肺 角青
	if (IsAlive() && IsQTEStartAble() &&GetNetStatus() == NET_MASTER && !bRequestStartQTE)
	{
		GetControlGameMode()->NetSendRequestQTE();
		bRequestStartQTE = true;
	}
}

void AB2ControlModePlayer::StopQTEMount()
{
	// 畴萍颇捞肺 郴赴促绊 舅覆. 唱滴 畴萍罐绊 郴覆
	// 捞扒 蜡历涝仿栏肺 龋免登绰霸 酒聪聪瘪.. 吝汗 眉农 救窃
	if (GetNetStatus() == NET_MASTER)
	{
		GetControlGameMode()->NetSendNotifyEndQTE();
	}
}

float AB2ControlModePlayer::GetUnitySkillPointDeltaSecondsRate(float DeltaSeconds)
{
	// 痢飞傈篮 救蛮促.
	return 0.0f;
}

void AB2ControlModePlayer::AddWeaponSkillPoint(float ToAddPoint)
{
	//171206_YJ 痢飞傈俊辑绰 胶懦捞唱 馆拜借阑锭 器牢飘 盲况瘤瘤 臼档废 窃.
}


bool AB2ControlModePlayer::CanTakeDamage(class AActor* DamageCauser, struct FDamageEvent const& DamageEvent, const struct FDamageInfo* DamageType)
{
	return Super::CanTakeDamage(DamageCauser, DamageEvent, DamageType)
		&& !bRequestStartQTE;
}

void AB2ControlModePlayer::MoveForward(float InValue)
{
	if (FMath::IsNearlyZero(InValue) == false)
	{
		GetControlGameMode()->TouchPressed();
	}

	Super::MoveForward(InValue);
}

void AB2ControlModePlayer::MoveRight(float InValue)
{
	if (FMath::IsNearlyZero(InValue) == false)
	{
		GetControlGameMode()->TouchPressed();
	}

	Super::MoveRight(InValue);
}

void AB2ControlModePlayer::PromoteNetStatus()
{
	if (GetNetStatus() == NET_MASTER && MyAIController)
		return;

	UnsubscribeEvents();

	//Role = ROLE_Authority;
	SetNetStatus(NET_AIMASTER);

	// AI 傈券 抛胶飘 Off 阂啊
	FActorSpawnParameters SpawnInfo;
	//SpawnInfo.Instigator = Instigator;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save player controllers into a map
	SpawnInfo.bDeferConstruction = true;

	MyAIController = GetWorld()->SpawnActor<ABladeIIPlayerAIController>(ABladeIIPlayerAIController::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
	BII_CHECK(MyAIController);
	UGameplayStatics::FinishSpawningActor(MyAIController, FTransform());

	MyAIController->SetTeamNum(GetTeamNum());
	MyAIController->Possess(this);
	MyAIController->BeginAutoPlay();

	// 唱埃敲饭捞绢啊 呕巴鸥绊乐栏搁 郴妨淋
	if (IsQTEMounting())
	{
		GetControlGameMode()->NetSendNotifyEndQTE(GetNetId());
	}
}

class AB2ControlGameMode* AB2ControlModePlayer::GetControlGameMode()
{
	AB2ControlGameMode* pGM = Cast<AB2ControlGameMode>(GetGameMode());

	check(pGM);

	return pGM;
}
