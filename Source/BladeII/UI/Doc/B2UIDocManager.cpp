#include "B2UIDocManager.h"
#include "B2UIDocHelper.h"
//#include "GameDelegates.h"
//#include "BladeIIPlayer.h"	//咯扁 乐绰 enum锭巩俊 include. enum阑 臂肺国窍霸 历厘窍绰 镑阑 绝阑鳖?
//#include "B2ItemInfo.h"
//#include "B2PCClassInfo.h"
//#include "BladeIIGameMode.h"

UB2UIDocManager* UB2UIDocManager::Instance = nullptr;


UB2UIDocManager::~UB2UIDocManager()
{
}

void UB2UIDocManager::Init(ABladeIIGameMode* InGM)
{
////	// 咯扁肺 逞绢坷绰 GameMode 绰 Init 窜拌俊辑狼 曼绊甫 困窃. 弊府绊 概 GameMode 付促 Init 捞 阂赴促绰 巴阑 鞠矫且 荐档
////	// UIDocManager 啊 static 牢 茄, 捞 GameMode 甫 某教秦辑 荤侩窍瘤 富 巴. 窜瘤 咯扁辑父..
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
////		if (!FGameDelegates::Get().GetEndPlayMapDelegate().IsBoundToObject(this)) //吝酶 官牢爹 救登霸
////			FGameDelegates::Get().GetEndPlayMapDelegate().AddRaw(this, &UB2UIDocManager::Destroy);
////#endif
////		*/
////		UB2UIDocHelper::CreateDocSome(); // 局概茄 巴甸 持阑 芥蹿芥蹿
////
////		//敲饭捞绢刀 积己(榜靛鞍篮 臂肺国茄 单捞磐, Single)
////		UB2UIDocHelper::CreateDocUser();
////
////		//UI狼 磊龙备饭茄 惑怕甸阑 历厘 且 档钮刚飘 积己(Single)
////		UB2UIDocHelper::CreateDocUICondition();
////
////		//4疙狼 洒绢肺 档钮刚飘 积己
////		DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
////		UB2UIDocHelper::CreateDocHero(PCClassToInt(EPCClass::EPC_Gladiator));
////		UB2UIDocHelper::CreateDocHero(PCClassToInt(EPCClass::EPC_Assassin));
////		UB2UIDocHelper::CreateDocHero(PCClassToInt(EPCClass::EPC_Wizard));
////		UB2UIDocHelper::CreateDocHero(PCClassToInt(EPCClass::EPC_Fighter));
////		// 积己篮 葛滴 窍绊 PCClassInfo 肺爹捞 鞘夸茄 累诀篮 鞘夸茄 局甸父 葛酒辑 蝶肺 窃.
////		InitResourcesOnNecessaryHeroDoc(InGM);
////
////		//硅撇刀 积己(Single)
////		UB2UIDocHelper::CreateDocBattle();
////
////		//硅撇胶抛捞瘤(Single)
////		UB2UIDocHelper::CreateDocBattleStage();
////
////		//矫唱府坷 促捞舅肺弊(Single)
////		UB2UIDocHelper::CreateDocScenarioDialog();
////
////		//茅磐(Single)
////		UB2UIDocHelper::CreateDocChapter();
////
////		//葛靛 傍侩(1措1措傈, 评概摹, 痢飞傈, 馆拜带傈, 康旷狼 啪, 饭捞靛)
////		UB2UIDocHelper::CreateDocMode();
////
////		//1措1措傈
////		UB2UIDocHelper::CreateDocPVP1on1Rival();
////
////		//评概摹
////		UB2UIDocHelper::CreateDocTeamMatch();
////
////		//馆拜带傈
////		UB2UIDocHelper::CreateDocCounterAttack();
////
////		//饭捞靛
////		UB2UIDocHelper::CreateDocRaid();
////
////		//康旷狼 啪
////		UB2UIDocHelper::CreateDocHeroTower();
////
////		//辨靛
////		UB2UIDocHelper::CreateDocGuild();
////
////		//惑痢
////		UB2UIDocHelper::CreateDocStore();
////
////		//痢飞傈
////		UB2UIDocHelper::CreateDocControl();
////
////		//肺厚概莫
////		UB2UIDocHelper::CreateDocLobbyMatch();
////
////		//模备
////		UB2UIDocHelper::CreateDocFriend();
////
////		//扁何捞亥飘
////		UB2UIDocHelper::CreateDocDonation();
////
////		//瞒盔狼闭凯
////		UB2UIDocHelper::CreateDocDimension();
////
////		bInit = true;
////	}	
}

void UB2UIDocManager::InitResourcesOnNecessaryHeroDoc(class ABladeIIGameMode* InGM)
{
//	BII_CHECK(InGM);
//	// 酒贰 InitResources 俊辑 PCClassInfo 甫 肺爹窍霸 等促搁 鞘夸茄 局甸父 葛酒辑 HeroDoc 阑 积己且 鞘夸啊 乐促. 利绢档 PCClassInGameOnlyInfo 绰 肺爹 救窍搁 宝满阑 淀.
//	// 父老 鞘夸茄 局甸父 避绢葛酒具 且 版快 GetPCClassesToPreLoad 绰 粱 狐瘤绰 努贰胶啊 乐阑 啊瓷己捞 乐绢辑 咯扁俊 静搁 救凳.
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
	// check(Instance); GIsRequestingExit 悼救俊绰 GetTransientPackage 啊 绝绢辑 Instance 啊 NULL 老 荐 乐促.
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
	//唱吝俊 掉饭捞甸 拎辑 平矫懦巴(弥利拳)

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
