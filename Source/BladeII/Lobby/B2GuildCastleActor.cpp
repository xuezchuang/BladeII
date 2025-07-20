
#include "B2GuildCastleActor.h"
//#include "BladeII.h"
//#include "B2UIManager.h"
//#include "B2UIGuildMapMain.h"
#include "B2LobbyGameMode.h"


#include "BladeIIGameImpl.h"

//3俺挥捞扼 抛捞喉父甸扁 弊贰辑 快急 窍靛窍霸 ..
#define GUILDATTPOINT_LOW	250000
#define GUILDATTPOINT_MIDDLE 650000
#define GUILDATTPOINT_HIGH	1200000


AB2GuildCastleActor::AB2GuildCastleActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void AB2GuildCastleActor::BeginPlay()
{
	Super::BeginPlay();
	//SetBindInputKey();
}

void AB2GuildCastleActor::OnClickCastleActer()
{

	//if (this->bHidden == true) //见败柳 惑怕牢 版快 急琶 x
	//	return;

	//ClickPlaySound();

	//auto& ClientData = BladeIIGameImpl::GetClientDataStore();
	//b2network::B2GuildBattleGuildPtr GuildInfo;

	//MyGuildInfo ? GuildInfo = ClientData.GetMyGuildBattleInfo() : GuildInfo = ClientData.GetOtherGuildBattleInfo();

	//UB2UIGuildMapMain* GuildUI = UB2UIManager::GetInstance()->GetUI<UB2UIGuildMapMain>(UIFName::GuildMapUI);

	//if (!GuildUI)
	//	return;

	//TArray<b2network::B2GuildBattleAttackPtr> AttackInfo;

	//for (auto El : GuildInfo->attacks)
	//{
	//	if (El->account_id == GuildInfo->members[InfoIndex]->account_id)
	//	{
	//		AttackInfo.Add(El);
	//	}
	//}


	//GuildUI->OnGuildRightGuildInfo(MyGuildInfo, GuildInfo->members[InfoIndex], AttackInfo, InfoIndex);

	//auto* Gamemode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//if (!Gamemode)
	//	return;

	//Gamemode->SetGuildEffectsLocation(static_cast<int32>(MyGuildInfo ? EUIGuildSelectEffect::Team : EUIGuildSelectEffect::Rival), this->GetActorLocation());

	
}

void AB2GuildCastleActor::SetCastleStaticMesh(UStaticMeshComponent* MeshComponent,UStaticMesh* Castle1, UStaticMesh* Castle2, UStaticMesh* Castle3, UStaticMesh* Castle4)
{
	CastleMesh.SetNum(4);

	CastleMesh[0] = Castle1;
	CastleMesh[1] = Castle2;
	CastleMesh[2] = Castle3;
	CastleMesh[3] = Castle4;

	CastleMeshComponent = MeshComponent;
}

void AB2GuildCastleActor::SetCastleEffect(UParticleSystemComponent* Effect1, UParticleSystemComponent* Effect2)
{
	NoMedalEffect = Effect1;
	BattleEffect = Effect2;
}





void AB2GuildCastleActor::NotifyActorOnReleased(FKey ButtonReleased /*= EKeys::LeftMouseButton*/)
{
	Super::NotifyActorOnReleased(ButtonReleased);

	//UB2UIGuildMapMain* GuildUI = UB2UIManager::GetInstance()->GetUI<UB2UIGuildMapMain>(UIFName::GuildMapUI);

	//if (!GuildUI)
	//	return;

	//if (GuildUI->IsPagePopup())						//扑诀捞 栋乐栏搁 菩胶菩胶
	//	return;


	OnClickCastleActer();
}

void AB2GuildCastleActor::NotifyActorOnInputTouchEnd(const ETouchIndex::Type FingerIndex)
{

	Super::NotifyActorOnInputTouchEnd(FingerIndex);

	//UB2UIGuildMapMain* GuildUI = UB2UIManager::GetInstance()->GetUI<UB2UIGuildMapMain>(UIFName::GuildMapUI);

	//if (!GuildUI)
	//	return;

	//if (GuildUI->IsPagePopup())						//扑诀捞 栋乐栏搁 菩胶菩胶
	//	return;

	OnClickCastleActer();
}

void AB2GuildCastleActor::SetGuildInfo(int32 Index, bool GuildInfoState /*= true*/)
{
	InfoIndex = Index;
	MyGuildInfo = GuildInfoState;
	
	auto& ClientData = BladeIIGameImpl::GetClientDataStore();
	b2network::B2GuildBattleGuildPtr GuildInfo;

	MyGuildInfo ? GuildInfo = ClientData.GetMyGuildBattleInfo() : GuildInfo = ClientData.GetOtherGuildBattleInfo();

	NoMedalEffect->SetVisibility(false);

	if (GuildInfo->members[InfoIndex]->left_medal == 0)
	{
		CastleMeshComponent->SetStaticMesh(CastleMesh[3]);
		NoMedalEffect->SetVisibility(true);
	}
	else if (GuildInfo->members[InfoIndex]->total_power > GUILDATTPOINT_HIGH)		// jwyim power
	{
		CastleMeshComponent->SetStaticMesh(CastleMesh[2]);
	}
	else if (GuildInfo->members[InfoIndex]->total_power > GUILDATTPOINT_MIDDLE)		// jwyim power
	{
		CastleMeshComponent->SetStaticMesh(CastleMesh[1]);
	}
	else
	{
		CastleMeshComponent->SetStaticMesh(CastleMesh[0]);
	}

	BattleEffect->SetVisibility(GuildInfo->members[InfoIndex]->is_attacked ? true : false);

	//GetTransform();
	this->SetActorRotation(FRotator(0, MyGuildInfo ? 40 : -40, 0));
	/*FTransform
	GetWorld()->spawn*/

}

