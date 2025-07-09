#include "BladeII.h"
#include "B2UIDocManager.h"
//#include "B2UIDocHelper.h"
//#include "GameDelegates.h"
//#include "BladeIIPlayer.h"	//���� �ִ� enum������ include. enum�� �۷ι��ϰ� �����ϴ� ���� ������?
//#include "B2ItemInfo.h"
//#include "B2PCClassInfo.h"
//#include "BladeIIGameMode.h"

UB2UIDocManager* UB2UIDocManager::Instance = nullptr;


UB2UIDocManager::~UB2UIDocManager()
{
}

void UB2UIDocManager::Init(ABladeIIGameMode* InGM)
{
////	// ����� �Ѿ���� GameMode �� Init �ܰ迡���� ���� ����. �׸��� �� GameMode ���� Init �� �Ҹ��ٴ� ���� �Ͻ��� ����
////	// UIDocManager �� static �� ��, �� GameMode �� ĳ���ؼ� ������� �� ��. ���� ���⼭��..
////	BII_CHECK(InGM);
////
////	if (bInit)
////	{
////		InitResourcesOnNecessaryHeroDoc(InGM);
////	}
////	else
////	{
////		/*
////#if WITH_EDITOR
////		if (!FGameDelegates::Get().GetEndPlayMapDelegate().IsBoundToObject(this)) //��ø ���ε� �ȵǰ�
////			FGameDelegates::Get().GetEndPlayMapDelegate().AddRaw(this, &UB2UIDocManager::Destroy);
////#endif
////		*/
////		UB2UIDocHelper::CreateDocSome(); // �ָ��� �͵� ���� ��ڽ��
////
////		//�÷��̾ ����(��尰�� �۷ι��� ������, Single)
////		UB2UIDocHelper::CreateDocUser();
////
////		//UI�� ���������� ���µ��� ���� �� ��ť��Ʈ ����(Single)
////		UB2UIDocHelper::CreateDocUICondition();
////
////		//4���� ����� ��ť��Ʈ ����
////		DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
////		UB2UIDocHelper::CreateDocHero(PCClassToInt(EPCClass::EPC_Gladiator));
////		UB2UIDocHelper::CreateDocHero(PCClassToInt(EPCClass::EPC_Assassin));
////		UB2UIDocHelper::CreateDocHero(PCClassToInt(EPCClass::EPC_Wizard));
////		UB2UIDocHelper::CreateDocHero(PCClassToInt(EPCClass::EPC_Fighter));
////		// ������ ��� �ϰ� PCClassInfo �ε��� �ʿ��� �۾��� �ʿ��� �ֵ鸸 ��Ƽ� ���� ��.
////		InitResourcesOnNecessaryHeroDoc(InGM);
////
////		//��Ʋ�� ����(Single)
////		UB2UIDocHelper::CreateDocBattle();
////
////		//��Ʋ��������(Single)
////		UB2UIDocHelper::CreateDocBattleStage();
////
////		//�ó����� ���̾˷α�(Single)
////		UB2UIDocHelper::CreateDocScenarioDialog();
////
////		//é��(Single)
////		UB2UIDocHelper::CreateDocChapter();
////
////		//��� ����(1��1����, ����ġ, ������, �ݰݴ���, ������ ž, ���̵�)
////		UB2UIDocHelper::CreateDocMode();
////
////		//1��1����
////		UB2UIDocHelper::CreateDocPVP1on1Rival();
////
////		//����ġ
////		UB2UIDocHelper::CreateDocTeamMatch();
////
////		//�ݰݴ���
////		UB2UIDocHelper::CreateDocCounterAttack();
////
////		//���̵�
////		UB2UIDocHelper::CreateDocRaid();
////
////		//������ ž
////		UB2UIDocHelper::CreateDocHeroTower();
////
////		//���
////		UB2UIDocHelper::CreateDocGuild();
////
////		//����
////		UB2UIDocHelper::CreateDocStore();
////
////		//������
////		UB2UIDocHelper::CreateDocControl();
////
////		//�κ��Ī
////		UB2UIDocHelper::CreateDocLobbyMatch();
////
////		//ģ��
////		UB2UIDocHelper::CreateDocFriend();
////
////		//����̺�Ʈ
////		UB2UIDocHelper::CreateDocDonation();
////
////		//�����Ǳտ�
////		UB2UIDocHelper::CreateDocDimension();
////
////		bInit = true;
////	}	
}

void UB2UIDocManager::InitResourcesOnNecessaryHeroDoc(class ABladeIIGameMode* InGM)
{
//	BII_CHECK(InGM);
//	// �Ʒ� InitResources ���� PCClassInfo �� �ε��ϰ� �ȴٸ� �ʿ��� �ֵ鸸 ��Ƽ� HeroDoc �� ������ �ʿ䰡 �ִ�. ��� PCClassInGameOnlyInfo �� �ε� ���ϸ� ������ ��.
//	// ���� �ʿ��� �ֵ鸸 �ܾ��ƾ� �� ��� GetPCClassesToPreLoad �� �� ������ Ŭ������ ���� ���ɼ��� �־ ���⿡ ���� �ȵ�.
//	{		
//		for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//		{
//			UB2UIDocHero* DocHero = UB2UIDocHelper::GetDocHero(PCI);
//			if (DocHero)
//			{
//				DocHero->InitResources();
//			}
//		}
//	}
}

UB2UIDocManager* UB2UIDocManager::GetInstance()
{
	if (Instance == nullptr)
	{
		UPackage* NewObjOuter = GetTransientPackage();
		if (NewObjOuter)
		{
			Instance = NewObject<UB2UIDocManager>(NewObjOuter, UB2UIDocManager::StaticClass());
			if (Instance){
				Instance->AddToRoot();
			}
		}
	}	
	// check(Instance); GIsRequestingExit ���ȿ��� GetTransientPackage �� ��� Instance �� NULL �� �� �ִ�.
	return Instance;
}

void UB2UIDocManager::Shutdown()
{
	for (auto each : SingleDoc)
	{
		if (each.Value)
		{
			each.Value->Destroy();
			each.Value = nullptr;
		}
	}

	for (auto each : MultipleDocs)
	{
		if (each.Value)
		{
			each.Value->Destroy();
			each.Value = nullptr;
		}
	}

	SingleDoc.Empty();
	MultipleDocs.Empty();

	if (Instance)
	{
		Instance->RemoveFromRoot();
		Instance = nullptr;
	}

	CollectGarbage(RF_NoFlags);

	bInit = false;
}

void UB2UIDocManager::Tick(float DeltaTime)
{
	//���߿� �����̵� �༭ ƽ��ų��(����ȭ)

}

bool UB2UIDocManager::IsTickable() const
{
	return true;
}

TStatId UB2UIDocManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UB2UIDocManager, STATGROUP_Tickables);
}

bool UB2UIDocManager::IsTickableWhenPaused() const
{
	return true;
}

bool UB2UIDocManager::IsTickableInEditor() const
{
	return true;
}

bool UB2UIDocManager::IsExistSingleDoc(EUIDocType Type)
{
	for (auto each : SingleDoc)
	{
		if (each.Value->DocType == Type)
			return false;
	}

	return true;
}
