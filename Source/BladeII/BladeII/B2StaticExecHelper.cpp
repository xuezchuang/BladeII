// Fill out your copyright notice in the Description page of Project Settings.


#include "B2StaticExecHelper.h"
#if WITH_EDITOR || !UE_BUILD_SHIPPING
#include "Engine.h"
#if WITH_EDITOR
#include "Editor.h"
//#include "DeclarativeSyntaxSupport.h"
//#include "BladeIIEditor/Source/BladeIIEditor.h"
//#include "Button.h"
//#include "B2Button.h"
//#include "UMG.h"
//#include "Editor/UMGEditor/Public/WidgetBlueprint.h"
//#include "Misc/Paths.h"
//#include "PaperSprite.h"
//#include "BladeIIEditor/Source/Commandlets/B2MatineeRefCheckCommandlet.h"
//#include "BladeIIEditor/Source/Commandlets/B2MaterialParamCheckCommandlet.h"
//#include "BladeIIEditor/Source/Commandlets/B2AssetOptCheckCommandlet.h"
//#include "AssetToolsModule.h"
//#include "Factories/Factory.h"
//#include "Factories/BlueprintFactory.h"
#endif
#include "Materials/MaterialInstanceConstant.h"
#include "B2NPCClassInfo.h"
#include "B2StageGameMode.h"
#include "B2StageManager.h"
#include "B2UIManager_InGameHUDChar.h"
#include "B2MonsterSpawnPool.h"
#include "B2LevelDoorBase.h"
#include "B2DestructibleLevelObjBase.h"
#include "B2Inventory.h"
#include "B2ItemInfo.h"
#include "B2DropItem.h"
#include "B2LobbyInventory.h"
#include "B2ScenarioDialogInfo.h"
#include "B2LobbyUI_CharStatDetail.h"
#include "Sound/SoundNodeWavePlayer.h"
#include "B2LobbyNPCSkeletalMeshActor.h"
#endif
#include "../UI/DJLegacy/B2UIManager_InGameHUDChar.h"
#include "BladeIIPlayerController.h"
#include "BladeIITestDummyPlayer.h"
#include "BladeIITestDummyNPC.h"
#include "B2LobbyGameMode.h"
#include "BladeIIPlayer.h"
#include "B2SkillRTManager.h"
#include "BladeIIBlockToSyncNetwork.h"
#include "B2GameInstance.h"
#include "BladeIIGameMode.h"

#include "B2NetworkBridge.h"
#include "OnlineWholesaler.h"
//#include "Message.h"
#include "TypeList.h"
#include "B2AreaDamageActorBase.h" 
#include "B2UIManager.h"
#include "SkeletalMeshMerge.h"
#include "B2PCClassInfoBox.h"
#include "B2UIDocHelper.h"
#include "B2UIBattleMain.h"
#include "BladeIINetPlayer.h"
#include "B2UIStageResult.h"
#include "B2Airport.h"
#include "B2BuffModeEffectInfo.h"
#include "B2UIManager_Lobby.h"
#include "B2UIMsgPopupReward.h"
#include "B2LogManager.h"
#include "B2UIPVPResult.h"
#include "BladeIIScalabilityControl.h"
#include "B2GameLoadingProgress.h"
#include "B2PreRenderer.h"
#include "B2BTTask_PCAutoAttack.h"
#include "Camera/CameraComponent.h"
#include "TutorialManager.h"
#include "B2TutorialGameMode.h"
#include "Animation/AnimNotifies/AnimNotifyState_TimedParticleEffect.h"
#include "Animation/AnimNotifies/AnimNotify_PlayParticleEffect.h"
#include "Animation/AnimNotifies/AnimNotify_PlaySound.h"
#include "B2DamageEffectInfo.h"
#include "B2UIChapterFrame.h"
#include "B2UITutorial.h"
#include "B2CompositeMeshCache.h"
#include "B2AssetLoader.h"
#include "B2RaidGameMode.h"
#include "B2GuildGameMode.h"
#include "B2StageInfo.h"
#include "Global.h"
#include "FB2ErrorMessage.h"
#include "FakeConfigure.h"
#include "BladeIIGameImpl.h"
#include "B2GuildNPCInfo.h"
#include "B2GuildNPCAIController.h"
#include "BladeIIMercenaryPlayer.h"
#include "DeviceProfiles/DeviceProfileManager.h"
#include "IDeviceProfileSelectorModule.h"
#include "B2UICostumePageStore.h"
#include "B2CombatStatEvaluator.h"

#include "ChannelManager.h"
#include "MessageRecorder.h"

//#include "B2IAPGenericPlatform.h"
#include "CommonStruct.h"

//#define B2_PROFILE_LEVEL 9

#include <memory>
#include <cstring>
#include "../../../../down/blade_dev_client/BladeII/Source/BladeII/Public/BladeIIUtil.h"
//#include "B2UITotemRaffle.h"



UB2StaticExecHelper::UB2StaticExecHelper(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if UE_BUILD_SHIPPING
	bIsAllowedForShipping = true; // �츮 Ŀ�ǵ常 ���κ��弭 ���.. ���� ���� ���� ���¹������� BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1 �� 0 ���� �Ǹ� ©���� �� ����.
#endif
}

bool UB2StaticExecHelper::Exec(UWorld* Inworld, const TCHAR* Cmd, FOutputDevice& Ar)
{
#if B2_BUILD_LEVEL == B2_BUILD_LEVEL_LIVE // B2_BUILD_LEVEL_LIVE
	// ���� �������� �ܼ�â�� ������ ���� (�ÿ� ��) �� ���Ϸ� ���� Ŀ�ǵ�� ������� �ʵ��� �Ѵ�. ���� Ŀ�ǵ� ����.
	return true;
#endif

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA
	if (ExecFormal(Inworld, Cmd, Ar) == true)
	{
		return true;
	}
#endif

#if WITH_EDITOR
	if (ExecEditorOnly(Inworld, Cmd, Ar) == true)
	{
		return true;
	}
#endif

#if !UE_BUILD_SHIPPING
	if (ExecDev(Inworld, Cmd, Ar) == true)
	{
		return true;
	}
#endif

	return false;
}

#if B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA
bool UB2StaticExecHelper::ExecFormal(UWorld* Inworld, const TCHAR* Cmd, FOutputDevice& Ar)
{
//	// ���� ������ shipping ������ ���忡�� ���Ե� �ʿ䰡 �ִ� ���� ���� ����� ������ �� ���.
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	FString CultureName;
//#endif
//
//	if (FParse::Command(&Cmd, TEXT("ServerCmd")))
//	{
//		return data_trader::ExecHandler::GetInstance().ExecuteShellCommand(Cmd);
//	}
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // ServerCmd �ܿ��� LV2 �� ������
//	else if (FParse::Command(&Cmd, TEXT("CurrentMemStat")) || FParse::Command(&Cmd, TEXT("CMS"))) // Shipping ���忡�� üũ�� ���� �־ ����� �Űܿ´�.
//	{
//		FGenericPlatformMemory::DumpStats(Ar);
//
//		// FGenericPlatformMemory::DumpStats �ڵ� �����ͼ� ������ ������ ȭ�鿡�� �ѷ���
//		const float InvMB = 1.0f / 1024.0f / 1024.0f;
//		FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
//		const float DrawTextTime = 20.0f;
//		const int32 FontSize = 14;
//		// ���� ���ϴ� �������� �ִ´�. �� ������ ���� ���� �ɷ��� ���� �� �ٸ���
//		BII_SCREEN_LOG(FString::Printf(TEXT("Virtual Memory: %.2f MB used,  %.2f MB free, %.2f MB total"), (MemoryStats.TotalVirtual - MemoryStats.AvailableVirtual)*InvMB, MemoryStats.AvailablePhysical*InvMB, MemoryStats.TotalVirtual*InvMB), FLinearColor(0.0f, 1.0f, 0.0f), FontSize, DrawTextTime);
//		BII_SCREEN_LOG(FString::Printf(TEXT("Physical Memory: %.2f MB used,  %.2f MB free, %.2f MB total"), (MemoryStats.TotalPhysical - MemoryStats.AvailablePhysical)*InvMB, MemoryStats.AvailablePhysical*InvMB, MemoryStats.TotalPhysical*InvMB), FLinearColor(0.0f, 0.8f, 0.0f), FontSize, DrawTextTime);
//		BII_SCREEN_LOG(FString::Printf(TEXT("Process Virtual Memory: %.2f MB used, %.2f MB peak"), MemoryStats.UsedVirtual*InvMB, MemoryStats.PeakUsedVirtual*InvMB), FLinearColor(0.0f, 1.0f, 0.0f), FontSize, DrawTextTime);
//		BII_SCREEN_LOG(FString::Printf(TEXT("Process Physical Memory: %.2f MB used, %.2f MB peak"), MemoryStats.UsedPhysical*InvMB, MemoryStats.PeakUsedPhysical*InvMB), FLinearColor(0.0f, 0.8f, 0.0f), FontSize, DrawTextTime);
//		BII_SCREEN_LOG(FString::Printf(TEXT("[[ Platform Memory Stats for %s ]]"), ANSI_TO_TCHAR(FPlatformProperties::PlatformName())), FLinearColor(0.0f,1.0f,0.0f), FontSize, DrawTextTime);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("StatCollect")))
//	{
//#if BII_STATS
//		FString FirstToken;
//		FParse::Token(Cmd, FirstToken, true);
//		if (FirstToken == TEXT("SkipEvent")) {
//			FStatisticalPerfStatCollector::bSkipEventCinematics = !FStatisticalPerfStatCollector::bSkipEventCinematics;
//		}
//		else { // �ƹ��͵� ���� ���ϸ� �׳� On/Off 
//			FStatisticalPerfStatCollector::bDoStatCollecting = !FStatisticalPerfStatCollector::bDoStatCollecting;
//		}
//		UE_LOG(LogBladeII, Display, TEXT("FStatisticalPerfStatCollector IsOn : %d, SkipEvent : %d"), (int32)FStatisticalPerfStatCollector::bDoStatCollecting, (int32)FStatisticalPerfStatCollector::bSkipEventCinematics);
//		BII_SCREEN_LOG(FString::Printf(TEXT("FStatisticalPerfStatCollector IsOn : %d, SkipEvent : %d"), (int32)FStatisticalPerfStatCollector::bDoStatCollecting, (int32)FStatisticalPerfStatCollector::bSkipEventCinematics), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//#endif
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("B2Standalone")))
//	{
//		FBladeIIBlockToSyncNetwork::GetInstance().DevSetStandaloneMode(!FBladeIIBlockToSyncNetwork::GetInstance().DevIsStandaloneMode());
//		UE_LOG(LogBladeII, Display, TEXT("FBladeIIBlockToSyncNetwork StandaloneMode %d"), (int32)FBladeIIBlockToSyncNetwork::GetInstance().DevIsStandaloneMode());
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("BladeIIBlockToSyncNetwork StandaloneMode %s"), (int32)FBladeIIBlockToSyncNetwork::GetInstance().DevIsStandaloneMode() ? TEXT("ON") : TEXT("OFF")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SuperHero"))) // �������� �ĺμ��� �׽�Ʈ�ϰ� ���� �� �����ϰ�..
//	{
//		//ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		//if (B2Player)
//		//{
//		//	bool bCurrentlyInSuperHero = (B2Player->bCheatHeroMode && B2Player->bCheatImmortalMode); // Zombie �� Hero �� ���ÿ� �����Ǿ� ������ On ������ �ɷ� �����ϰ� ����.
//
//		//	bCurrentlyInSuperHero = !bCurrentlyInSuperHero;
//
//		//	// ���⼭���� ���� ����. Hero, Zombie, Cooltime �˴�.
//		//	B2Player->bCheatHeroMode = bCurrentlyInSuperHero;
//		//	B2Player->bCheatImmortalMode = bCurrentlyInSuperHero;
//
//		//	UB2SkillRTManager* SkillRTManager = B2Player->GetSkillRTManager();
//		//	if (SkillRTManager)
//		//	{ // ġƮ�� 1��, �ƴϸ� �⺻��
//		//		SkillRTManager->Cheat_ForceSetCooltime(true, bCurrentlyInSuperHero ? 1.0f : -1.0f);
//		//		SkillRTManager->Cheat_ForceSetCooltime(false, bCurrentlyInSuperHero ? 1.0f : -1.0f);
//		//	}
//
//		//	UE_LOG(LogBladeII, Log, TEXT("Super Hero mode : One-hit kill + immortality + Quick cool-time %s"), bCurrentlyInSuperHero ? TEXT("ON") : TEXT("OFF"));
//		//}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("Hero")))
//	{
//		//ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		//if (B2Player)
//		//{
//		//	B2Player->bCheatHeroMode = !B2Player->bCheatHeroMode;
//		//	UE_LOG(LogBladeII, Log, TEXT("Hero mode : Player kills all monsters at one hit %s"), B2Player->bCheatHeroMode ? TEXT("ON") : TEXT("OFF"));
//		//}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("Zombie")))
//	{
//		//ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		//if (B2Player)
//		//{
//		//	B2Player->bCheatImmortalMode = !B2Player->bCheatImmortalMode;
//		//	UE_LOG(LogBladeII, Log, TEXT("Zombie mode : Player doesn't die %s"), B2Player->bCheatImmortalMode ? TEXT("ON") : TEXT("OFF"));
//		//}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ForcedDummyPart")))
//	{
//		extern int32 gForcedPlayerDummyPartGrade;
//		FString StarGradeStr;
//		FParse::Token(Cmd, StarGradeStr, true);
//		gForcedPlayerDummyPartGrade = FCString::Atoi(*StarGradeStr);
//
//		for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//		{ // �κ� �޽��� ��������. ���� LobbyUpdateCharacterPartClass �ñ׳��� ���� ������Ʈ�� �ƴϹǷ� �� ġƮ�� �ٷ� �� ���� �� �ִµ� �� ġƮ ��� �� ���ʿ��� ó���ϵ��� �Ͽ���.
//			LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(PCI));
//		}
//		// �ΰ����̸� �±׸� �ٽ� �ϴ���.. �� �޽��� ĳ���� �Ǿ����� �׳� ���� �ٽ� �ε��ؾ߰ڴ�.
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CombatStatScale")))
//	{ // ���� �÷��̾� ĳ���Ϳ��� ������ ��
//		//extern float gLocalPCCombatStatScale;
//		//FString ScaleStr;
//		//FParse::Token(Cmd, ScaleStr, true);
//		//gLocalPCCombatStatScale = FCString::Atof(*ScaleStr);
//		//ABladeIIPlayer* LocalPC = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		//if (LocalPC) {
//		//	LocalPC->InitializeCombatStats();
//		//}
//		return true;
//	}
//#if B2_BUILD_LEVEL <= B2_BUILD_LEVEL_QA
//	else if (FParse::Command(&Cmd, TEXT("AddIncAdditionalDamage")))
//	{ 
//		//FString AdditionalDamStr;
//		//FParse::Token(Cmd, AdditionalDamStr, true);
//		//float AdditionalDamage = FCString::Atof(*AdditionalDamStr);
//		//ABladeIIPlayer* LocalPC = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		//for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//		//{
//		//	ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(*ActorIt);
//		//	if (B2Player)
//		//		B2Player->SetIncAdditionalDamage(AdditionalDamage);
//		//}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("AddDecAdditionalDamage")))
//	{ 
//		FString AdditionalDamStr;
//		FParse::Token(Cmd, AdditionalDamStr, true);
//		float AdditionalDamage = FCString::Atof(*AdditionalDamStr);
//		for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//		{
//			ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(*ActorIt);
//			if (B2Player)
//				B2Player->SetDecAdditionalDamage(AdditionalDamage);
//		}
//		return true;
//	}
//#endif
//	else if (FParse::Command(&Cmd, TEXT("SinglePlayQuick")))
//	{ // �ܵ� �÷��� ��带 ���� ġƮŰ ����
//		//if (!FBladeIIBlockToSyncNetwork::GetInstance().DevIsStandaloneMode())
//		//{
//		//	GEngine->DeferredCommands.Add(TEXT("B2Standalone"));
//		//}
//		//GEngine->DeferredCommands.Add(TEXT("CombatStatScale 100000000"));
//		//GEngine->DeferredCommands.Add(TEXT("ForcedDummyPart 6"));
//
//		//extern bool gCheatUseDummySkills;
//		//if (!gCheatUseDummySkills)
//		//{
//		//	GEngine->DeferredCommands.Add(TEXT("FixedSkillMode"));
//		//}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PreRender")))
//	{
//		FString FirstTokenStr;
//		FParse::Token(Cmd, FirstTokenStr, true);
//
//		if (FirstTokenStr == TEXT("Clear"))
//		{
//			UB2PreRenderer::ClearCompleteState(); // �ٽ� Pre-render ����
//			BII_SCREEN_LOG(FString::Printf(TEXT("PreRenderState Clear")), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstTokenStr == TEXT("FarAway")) // ó�� �� FarAway �� �ָ� �� �̰Ÿ� ����
//		{
//			UB2PreRenderer::bPreRenderAtFarAway = !UB2PreRenderer::bPreRenderAtFarAway;
//			UE_LOG(LogBladeII, Display, TEXT("PreRenderAtFarAway %d"), (int32)UB2PreRenderer::bPreRenderAtFarAway);
//			BII_SCREEN_LOG(FString::Printf(TEXT("PreRenderAtFarAway %d"), (int32)UB2PreRenderer::bPreRenderAtFarAway), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstTokenStr == TEXT("All")) // ��� ������Ƽ�� ������Ʈ���� Pre-render �ϴ� �ɼ�
//		{
//			UB2PreRenderer::bPreRenderAllPrimitives = !UB2PreRenderer::bPreRenderAllPrimitives;
//			UE_LOG(LogBladeII, Display, TEXT("bPreRenderAllPrimitives %d"), (int32)UB2PreRenderer::bPreRenderAllPrimitives);
//			BII_SCREEN_LOG(FString::Printf(TEXT("bPreRenderAllPrimitives %d"), (int32)UB2PreRenderer::bPreRenderAllPrimitives), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstTokenStr == TEXT("SpawnDist")) // ó�� �� SpawnDist �� �ָ� ���̵� spawn �� �Ÿ� ����
//		{
//			FString DistStr;
//			FParse::Token(Cmd, DistStr, true);
//			UB2PreRenderer::PreRenderSpawnDistFromView = FCString::Atof(*DistStr);
//			UE_LOG(LogBladeII, Display, TEXT("PreRenderSpawnDistFromView %f"), UB2PreRenderer::PreRenderSpawnDistFromView);
//			BII_SCREEN_LOG(FString::Printf(TEXT("PreRenderSpawnDistFromView %d"), UB2PreRenderer::PreRenderSpawnDistFromView), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else
//		{
//			// �ƴ� ���� ������ ���ڴ�� ���� Ƚ���� ����
//			UB2PreRenderer::MaxPreRenderCount = FCString::Atoi(*FirstTokenStr);
//
//			FString OverallLimitCountStr;
//			FParse::Token(Cmd, OverallLimitCountStr, true);
//			UB2PreRenderer::OverallPreRenderLimit = FMath::Max(FCString::Atoi(*OverallLimitCountStr), UB2PreRenderer::MaxPreRenderCount);
//
//			FString HardLimitCountStr;
//			FParse::Token(Cmd, HardLimitCountStr, true);
//			UB2PreRenderer::PreRenderHardLimit = FMath::Max(FCString::Atoi(*HardLimitCountStr), UB2PreRenderer::OverallPreRenderLimit);
//
//			FString AllPrimLit2PassTickLimitStr;
//			FParse::Token(Cmd, AllPrimLit2PassTickLimitStr, true);
//			UB2PreRenderer::PreRenderAllPrimLit2PassTickLimit = FCString::Atoi(*AllPrimLit2PassTickLimitStr);
//
//			FString AllPrimLit1PassTickLimitStr;
//			FParse::Token(Cmd, AllPrimLit1PassTickLimitStr, true);
//			UB2PreRenderer::PreRenderAllPrimLit1PassTickLimit = FCString::Atoi(*AllPrimLit1PassTickLimitStr);
//
//			FString AllPrimUnllitPassTickLimitStr;
//			FParse::Token(Cmd, AllPrimUnllitPassTickLimitStr, true);
//			UB2PreRenderer::PreRenderAllPrimUnlitPassTickLimit = FCString::Atoi(*AllPrimUnllitPassTickLimitStr);
//
//			UE_LOG(LogBladeII, Display, TEXT("MaxPreRenderCount %d, OverallLimit %d, HardLimit %d, AllPrimLit2Limit %d, AllPrimLit1Limit %d, AllPrimUnlitLimit %d"), 
//				UB2PreRenderer::MaxPreRenderCount, UB2PreRenderer::OverallPreRenderLimit, UB2PreRenderer::PreRenderHardLimit, 
//				UB2PreRenderer::PreRenderAllPrimLit2PassTickLimit, UB2PreRenderer::PreRenderAllPrimLit1PassTickLimit, UB2PreRenderer::PreRenderAllPrimUnlitPassTickLimit);
//			BII_SCREEN_LOG(FString::Printf(TEXT("MaxPreRenderCount %d, OverallLimit %d, HardLimit %d, AllPrimLit2Limit %d, AllPrimLit1Limit %d, AllPrimUnlitLimit %d"),
//				UB2PreRenderer::MaxPreRenderCount, UB2PreRenderer::OverallPreRenderLimit, UB2PreRenderer::PreRenderHardLimit, 
//				UB2PreRenderer::PreRenderAllPrimLit2PassTickLimit, UB2PreRenderer::PreRenderAllPrimLit1PassTickLimit, UB2PreRenderer::PreRenderAllPrimUnlitPassTickLimit),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CheckLT")))
//	{
//		extern bool gDisplayLoadingTimeTaken;
//		gDisplayLoadingTimeTaken = !gDisplayLoadingTimeTaken;
//		BII_SCREEN_LOG(FString::Printf(TEXT("Checking Loading Time %s"), gDisplayLoadingTimeTaken ? TEXT("ON") : TEXT("OFF")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("AddCostume")))
//	{
//		FString RefStr;
//		FParse::Token(Cmd, RefStr, true);
//		int32 RefID = FCString::Atoi(*RefStr);
//
//		FString CostumeEquipTypeStr;
//		FParse::Token(Cmd, CostumeEquipTypeStr, true);
//
//		int32 CostumeEquipValue = FCString::Atoi(*CostumeEquipTypeStr);
//		ECostumeEquipPlace CostumeEquipPlace = static_cast<ECostumeEquipPlace>(CostumeEquipValue);
//
//		extern TArray<FB2Item> gCheatCostumeItem;
//
//		FB2Item AddItem;
//		AddItem.ItemRefID = RefID;
//		AddItem.CostumeEquipPlace = CostumeEquipPlace;
//		gCheatCostumeItem.Add(AddItem);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("RemoveAllCostume")))
//	{
//		extern TArray<FB2Item> gCheatCostumeItem;
//		gCheatCostumeItem.SetNum(0);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PCSectionMerge"))) // �׽�Ʈ ���߿��� ����.. ������ �������� ������ ���Ŀ��� ���� �׽�Ʈ�� ���� On/Off �÷��װ� �� ����.
//	{
//		++UB2PCMeshSectionMergeInfo::SectionMergeTestFlag; // 1, 2 �� ���� ��� Ȥ�� Off
//		if (UB2PCMeshSectionMergeInfo::SectionMergeTestFlag > 2) {
//			UB2PCMeshSectionMergeInfo::SectionMergeTestFlag = 0;
//		}
//
//		UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
//		// �� ġƮ�� ���� ����Ǵ� �� �� �����̶� ĳ�� ���¸� ����� �Ѵ�.
//		if (MeshCacheManager)
//		{
//			MeshCacheManager->DevResetCompositMeshCache();
//		}
//
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		if (LobbyGM)
//		{
//			for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//			{
//				// ������ CompoiteMeshCache �� ���� LobbyUpdateCharacterPartClass �ε� ��� ���������� ���� ������Ʈ�� �Ǵµ� �ȵɶ��� �ִ�.
//				// �̷� ��Ȳ���� �� �̺�Ʈ�� ���� ������ ���ϴ� �׳� LobbyGameMode �����ؼ� ��������..
//				//LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(PCI));
//				if (LobbyGM)
//				{
//					LobbyGM->DevForceUpdateCharacterParts(IntToPCClass(PCI));
//				}
//			}
//		}
//
//		for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//		{
//			ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(*ActorIt);
//			if (B2Player)
//			{
//				if (UB2PCMeshSectionMergeInfo::ShouldForceUseSectionMerge()){
//					B2Player->SetUseSectionMergedMesh(true);
//				}
//				else if (UB2PCMeshSectionMergeInfo::ShouldForceUnuseSectionMerge()) {
//					B2Player->SetUseSectionMergedMesh(false);
//				}
//				else {
//					B2Player->SetUseSectionMergedMesh(UB2PCMeshSectionMergeInfo::ShouldUseSectionMergeByGameContext(Inworld));
//				}
//			}
//		}
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("PCMeshSectionMerge %s"),
//			(UB2PCMeshSectionMergeInfo::SectionMergeTestFlag == 1) ? TEXT("Force ON") :
//			( (UB2PCMeshSectionMergeInfo::SectionMergeTestFlag == 2) ? TEXT("Force OFF") : TEXT("Default") ) ),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("KillAllEnemy"))) // ��������
//	{
//		ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(Inworld));
//
//		if (B2GM)
//			B2GM->KillAllEnemy();
//	}
//	else if (FParse::Command(&Cmd, TEXT("AutoComboOnly")))
//	{
//		UB2BTTask_PCAutoAttack::bDevAllowComboAttackOnly = !UB2BTTask_PCAutoAttack::bDevAllowComboAttackOnly;
//		BII_SCREEN_LOG(FString::Printf(TEXT("PCAutoAttack Combo Only %s"), UB2BTTask_PCAutoAttack::bDevAllowComboAttackOnly ? TEXT("ON") : TEXT("OFF")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SpawnDummyPlayer")))
//	{
//		FString TypeStr;
//		FParse::Token(Cmd, TypeStr, true);
//		EPCClass SpawnClass = IntToPCClass(FCString::Atoi(*TypeStr));
//
//		FString EquipGradeStr;
//		FParse::Token(Cmd, EquipGradeStr, true);
//		int32 EquipGrade = FMath::Clamp(FCString::Atoi(*EquipGradeStr), 1, FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE);
//
//		FString ParsedAnimStateString;
//		FParse::Value(Cmd, TEXT("Anim="), ParsedAnimStateString);
//
//		float ParsedAnimCooltime = 0.0f;
//		FString ParsedAnimCooltimeString;
//		if (FParse::Value(Cmd, TEXT("AnimCool="), ParsedAnimCooltimeString))
//		{
//			ParsedAnimCooltime = FCString::Atof(*ParsedAnimCooltimeString);
//		}
//
//		float ParsedSpawnFloatingInterval = 0.0f;
//		FString ParsedSpawnFloatingIntervalStr;
//		if (FParse::Value(Cmd, TEXT("FUI="), ParsedSpawnFloatingIntervalStr))
//		{
//			ParsedSpawnFloatingInterval = FCString::Atof(*ParsedSpawnFloatingIntervalStr);
//		}
//
//		float ParsedSpawnDamageFxInterval = 0.0f;
//		FString ParsedSpawnDamageFxIntervalStr;
//		if (FParse::Value(Cmd, TEXT("DFX="), ParsedSpawnDamageFxIntervalStr))
//		{
//			ParsedSpawnDamageFxInterval = FCString::Atof(*ParsedSpawnDamageFxIntervalStr);
//		}
//
//		bool bHasWing = FParse::Param(Cmd, TEXT("Wing"));
//
//		//if (SpawnClass != EPCClass::EPC_End)
//		//{
//		//	FTransform NewTransform(
//		//		FRotator(0.0f, 0.0f, 0.0f),
//		//		FVector(0.0f, 0.0f, 0.0f),
//		//		FVector(1.0f, 1.0f, 1.0f)
//		//	);
//		//	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		//	if (LocalB2Player)
//		//	{
//		//		NewTransform = LocalB2Player->GetActorTransform();
//		//		NewTransform.AddToTranslation(FVector(100.0f, 100.0f, 0.0f));
//		//	}
//
//		//	// �Ľ��� ���ڵ�� �Բ� dummy player spawn.
//		//	ABladeIITestDummyPlayer* SpawnedDummy = ABladeIITestDummyPlayer::SpawnWithStandardEquipment(
//		//		Inworld, SpawnClass, NewTransform, EquipGrade, ParsedSpawnFloatingInterval, ParsedSpawnDamageFxInterval,
//		//		bHasWing, MAX_WING_EVOLUTION_GRADE, true // ���� ����� �� ���⼭..
//		//	);
//
//		//	if (SpawnedDummy)
//		//	{
//		//		if (ParsedAnimStateString == TEXT("Rand") || ParsedAnimStateString == TEXT("Random"))
//		//		{ // �����ϰ� �÷���
//		//			SpawnedDummy->SetUseRandomAttackState(true);
//		//		}
//		//		else
//		//		{ // ���س��� �ϳ� �÷���
//		//			SpawnedDummy->SetUseRandomAttackState(false);
//		//			SpawnedDummy->SetDirectAttackStateByKeyword(ParsedAnimStateString);
//		//		}
//		//		SpawnedDummy->SetCooltimeBetweenAnim(ParsedAnimCooltime);
//		//	}
//		//}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TutorialTest")))
//	{
//		TutorialManager::GetInstance().Init();
//
//		FString OnOrOff;
//		FParse::Token(Cmd, OnOrOff, true);
//
//		// Off��� ������������� �׳� ����
//		bool bTestAvailable = (OnOrOff == TEXT("Off")) ? false : true;
//		TutorialManager::GetInstance().SetTestTutorialAvailable(bTestAvailable);
//		TutorialManager::GetInstance().SetTutorialAvailable(bTestAvailable);
//		TutorialManager::GetInstance().RefreshTutorialTestUI();
//	}
//	else if (FParse::Command(&Cmd, TEXT("TutorialBegin")))
//	{
//		FString StrTutorialID;
//		FParse::Token(Cmd, StrTutorialID, true);
//		int32 TutorialID = FCString::Atoi(*StrTutorialID);
//
//		if (TutorialManager::GetInstance().IsTutorialAvailable())
//		{
//			TutorialManager::GetInstance().Init();
//			TutorialManager::GetInstance().SetTestTutorial(TutorialID);
//
//			FString CommandString = FString::Printf(TEXT("SetTutorialProgress %d %d %d %d"), TutorialID, 0, 0, false);
//			data_trader::ExecHandler::GetInstance().ExecuteShellCommand(*CommandString);
//		}
//	}
//	else if (FParse::Command(&Cmd, TEXT("StartNewTutorial")))
//	{
//		SetClearNewTutorial(false);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SkipNewTutorial")))
//	{
//		AB2TutorialGameMode* B2TGM = Cast<AB2TutorialGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		if (B2TGM)
//		{
//			B2TGM->SetFinishTutorial();
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("Back")))
//	{
//		FPlatformMisc::SetBackKeyEvent(true);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("OpenCostume")))
//	{
//		FString OnOrOff;
//		FParse::Token(Cmd, OnOrOff, true);
//
//		bool bIsOpen = (OnOrOff == TEXT("Off")) ? false : true;
//
//		OpenCostumeClass<bool>::GetInstance().Signal(bIsOpen);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SkipCurrentTutorial")) || FParse::Command(&Cmd, TEXT("SkipCurrTtr")))
//	{
//		// ġƮ�� �ᰡ�鼭 �÷��� �ϴ� ���� �������� ���� ��� �ص� Ʃ�丮�� â�� �� ���� ���� �ִ�. ���� ��ġ
//		UB2UITutorial* TutorialUI = UB2UIManager::GetInstance()->GetUI<UB2UITutorial>(UIFName::Tutorial);
//		if (TutorialUI)
//		{
//			TutorialUI->OnClickBTN_SkipConfirm();
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("AsyncPreLoad")))
//	{
//		BladeIIGameImpl::bAllowInfoAssetAsyncLoading = !BladeIIGameImpl::bAllowInfoAssetAsyncLoading;
//		BII_SCREEN_LOG(FString::Printf(TEXT("InfoAsset Async Pre-load %s"), BladeIIGameImpl::bAllowInfoAssetAsyncLoading ? TEXT("ON") : TEXT("OFF")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("AsyncUILoad")))
//	{
//		BladeIIGameImpl::bAllowUIAsyncLoading = !BladeIIGameImpl::bAllowUIAsyncLoading;
//		BII_SCREEN_LOG(FString::Printf(TEXT("UI Async Pre-load %s"), BladeIIGameImpl::bAllowUIAsyncLoading ? TEXT("ON") : TEXT("OFF")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("OnDemandAsyncUILoad")))
//	{
//		BladeIIGameImpl::bAllowUIOnDemandAsyncLoading = !BladeIIGameImpl::bAllowUIOnDemandAsyncLoading;
//		BII_SCREEN_LOG(FString::Printf(TEXT("On-demand UI Async load %s"), BladeIIGameImpl::bAllowUIOnDemandAsyncLoading ? TEXT("ON") : TEXT("OFF")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CompositeMeshRootset")))
//	{
//		UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
//		if (MeshCacheManager)
//		{
//			MeshCacheManager->SetAllowLocalPCMeshRootSet(!MeshCacheManager->IsLocalPCMeshRootSetAllowed());
//			BII_SCREEN_LOG(FString::Printf(TEXT("CompositeMeshCache Local PC RootSet %s"), MeshCacheManager->IsLocalPCMeshRootSetAllowed() ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Value(Cmd, TEXT("Culture="), CultureName))
//	{
//		FInternationalization::Get().SetCurrentCulture(CultureName);
//	}
//	else if (FParse::Command(&Cmd, TEXT("ALLog"))) // AssetLoaderLog Ȥ�� AsyncLoadLog ����� �ؼ��ϵ� ��
//	{
//		UB2AssetLoader::bEnableDevLog = !UB2AssetLoader::bEnableDevLog;
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("B2AssetLoader Dev Logging %s"), UB2AssetLoader::bEnableDevLog ? TEXT("ON") : TEXT("OFF")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("NPCAsyncLimit"))) // �̹� r.PostGameLoadNPCAsyncLoadLimit �ָܼ�������� Shipping ���� �׽�Ʈ�Ϸ���.
//	{
//		IConsoleVariable* CVarAsyncLoadLimit = IConsoleManager::Get().FindConsoleVariable(TEXT("r.PostGameLoadNPCAsyncLoadLimit"));
//		if (CVarAsyncLoadLimit)
//		{
//			FString NewLimitStr;
//			FParse::Token(Cmd, NewLimitStr, true);
//			int32 NewLimit = FCString::Atoi(*NewLimitStr);
//
//			CVarAsyncLoadLimit->Set(NewLimit, ECVF_SetByCommandline);
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("PostGameLoadNPCAsyncLoadLimit %d"), CVarAsyncLoadLimit->GetInt()),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ForcedCSMDist"))) // �̹� r.Shadow.EnforcedStationaryCSMDistance �ָܼ�������� Shipping ���� �׽�Ʈ�Ϸ���. �ٷ� ���� �ʿ䵵 �ְ�.
//	{
//		IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Shadow.EnforcedStationaryCSMDistance"));
//		if (CVar)
//		{
//			FString NewDistStr;
//			FParse::Token(Cmd, NewDistStr, true);
//			float NewDist = FCString::Atof(*NewDistStr);
//
//			CVar->Set(NewDist, ECVF_SetByCommandline);
//
//			// �̰� �ٷ� ����Ƿ��� DirectionalLightComponent ���� ������ ������Ʈ �� �־��..
//			for (TObjectIterator<UDirectionalLightComponent> CompIT; CompIT; ++CompIT)
//			{
//				UDirectionalLightComponent* ThisComp = Cast<UDirectionalLightComponent>(*CompIT);
//				if (ThisComp)
//				{
//					ThisComp->MarkRenderStateDirty();
//				}
//			}
//			BII_SCREEN_LOG(FString::Printf(TEXT("ForcedCSMDist %f"), CVar->GetFloat()),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestModShadowColor"))) // �̰� ��� �ְ� �ٷ� ������Ʈ �Ǵ� �� ������..
//	{
//		IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.TestModulatedShadowColorScale"));
//		if (CVar)
//		{
//			FString NewColorStr;
//			FParse::Token(Cmd, NewColorStr, true);
//			float NewColorScale = FCString::Atof(*NewColorStr);
//
//			CVar->Set(NewColorScale, ECVF_SetByCommandline);
//
//			// �̰� �ٷ� ����Ƿ��� DirectionalLightComponent ���� ������ ������Ʈ �� �־��..
//			for (TObjectIterator<UDirectionalLightComponent> CompIT; CompIT; ++CompIT)
//			{
//				UDirectionalLightComponent* ThisComp = Cast<UDirectionalLightComponent>(*CompIT);
//				if (ThisComp)
//				{
//					ThisComp->MarkRenderStateDirty();
//				}
//			}
//			BII_SCREEN_LOG(FString::Printf(TEXT("TestModulatedShadowColorScale %f"), CVar->GetFloat()),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ModShadowFog")))
//	{
//		IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Shadow.FoggedModulatedShadow"));
//		if (CVar)
//		{
//			CVar->Set( (CVar->GetInt() == 0) ? 1 : 0, ECVF_SetByCommandline);
//			BII_SCREEN_LOG(FString::Printf(TEXT("FoggedModulatedShadow %d"), CVar->GetInt()),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("BackToDefaultScalability")))
//	{
//		// ��⿡ ���� ������ ������ ���� �ɼ� ����
//		B2Scalability::ResetAllScalabilitySettings();
//		GConfig->Flush(false, GB2GeneralSavedStateIni);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ShowVersionInfo")))
//	{
//		FString OnOrOff;
//		FParse::Token(Cmd, OnOrOff, true);
//		bool IsShow = (OnOrOff == TEXT("Off")) ? false : true;
//
//		if (auto* pDocUser = UB2UIDocHelper::GetDocUser())
//		{
//			pDocUser->SetShowVersion(IsShow);
//		}
//	}
//	else if (FParse::Command(&Cmd, TEXT("ShowDeviceProfile")))
//	{
//		const FString DeviceProfileNameStr = UDeviceProfileManager::GetActiveProfileName();
//
//		FString LocalDeviceProfileStateIni;
//		FConfigCacheIni::LoadGlobalIniFile(LocalDeviceProfileStateIni, TEXT("LocalDeviceProfileState"));
//		bool bWasDetectedAsEmulator = false;
//		GConfig->GetBool(TEXT("UserDevice"), TEXT("DetectedAsAndroidEmulator"), bWasDetectedAsEmulator, LocalDeviceProfileStateIni);
//		bool bSupportASTC =
//#if PLATFORM_ANDROID
//			FAndroidMisc::SupportsASTC();
//#else
//			false;
//#endif
//		// GetActiveProfileName �߰� ����鵵 ����ϸ� ���� ������ �� ��.
//		BII_SCREEN_LOG(FString::Printf(TEXT("ActiveDeviceProfile : %s, EmulDetectHistory %d, ASTC? %d"), 
//			*DeviceProfileNameStr, (int32)bWasDetectedAsEmulator, (int32)bSupportASTC),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ShowDeviceInfo")))
//	{
//#if PLATFORM_ANDROID
//		IDeviceProfileSelectorModule* AndroidDeviceProfileSelector = FModuleManager::LoadModulePtr<IDeviceProfileSelectorModule>("AndroidDeviceProfileSelector");
//		if (AndroidDeviceProfileSelector)
//		{
//			FString GPUFamily = FAndroidMisc::GetGPUFamily();
//			FString GLVersion = FAndroidMisc::GetGLVersion();
//			//FString VulkanVersion = FAndroidMisc::GetVulkanVersion(); �ư� ��
//			FString AndroidVersion = FAndroidMisc::GetAndroidVersion();
//			FString DeviceMake = FAndroidMisc::GetDeviceMake();
//			FString DeviceModel = FAndroidMisc::GetDeviceModel();
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("  AndroidVersion %s, GLVersion %s"), *AndroidVersion, *GLVersion),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 10.0f);
//			BII_SCREEN_LOG(FString::Printf(TEXT("  GPUFaily : %s"), *GPUFamily),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 10.0f);
//			BII_SCREEN_LOG(FString::Printf(TEXT("  Device Make and Model : %s, %s"), *DeviceMake, *DeviceModel),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 10.0f);
//			BII_SCREEN_LOG(FString::Printf(TEXT("[ShowDeviceInfo] Android Device")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 10.0f);
//		}
//#elif PLATFORM_IOS
//		// FIOSPlatformMisc �� �ʿ��� ������ ���� �� ������.. ���� ���� �� ��� Ȯ�� �ϱ� ��������..
//		BII_SCREEN_LOG(FString::Printf(TEXT("[ShowDeviceInfo] Need implementation on IOS")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 10.0f);
//#else
//		BII_SCREEN_LOG(FString::Printf(TEXT("[ShowDeviceInfo] Not implemented for current device!!")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 10.0f);
//#endif
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("LoadTest")))
//	{
//		// ������ InfoAsset �� �ε��ϴ� ���� �׽�Ʈ
//		// �ε��� �� �� ���¿��� �ؾ� ��.
//
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		if (FirstArg == TEXT("UI"))
//		{
//			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
//
//			//UIMgrInst->UnloadAllTAssets(); �̰� �ϰ� �ٷ� GC ���� �� ������ �ϳ� ������ ������ �ε��ϴ� ���� �� ����..
//
//			double LoadBeginTime = FPlatformTime::Seconds();
//
//			// �ε� �� ��Ʈ�¿� �־��ٰų� ĳ���� �� �� �̰ɷ� ��Ȯ�� �׽�Ʈ�� �Ұ���. ����� ������ ����..
//			
//			TArray<FName> AllLoadUIs;
//			AllLoadUIs.Add(UIFName::StageResult);
//			AllLoadUIs.Add(UIFName::ArinConsult_PostDefeat);
//			AllLoadUIs.Add(UIFName::GuildMain);
//			AllLoadUIs.Add(UIFName::RaidResult);
//
//			int32 LoadedCount = 0;
//			for (FName ThisLoadName : AllLoadUIs)
//			{
//				FB2UIWidgetData* DataSetToLoad = UIMgrInst->GetWidgetData(ThisLoadName);
//				if (DataSetToLoad)
//				{
//					TSubclassOf<UB2UIWidget> LoadedClass = UIMgrInst->GetWidgetClass(DataSetToLoad);
//					if (LoadedClass)
//					{
//						++LoadedCount;
//					}
//				}
//			}
//			double TimeTaken = FPlatformTime::Seconds() - LoadBeginTime;
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("Took %.4f s to load %d UI WidgetClasses."), TimeTaken, LoadedCount),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 10.0f);
//		}
//		else
//		{
//			UB2PCClassInfoBox* PCInfoBox = StaticFindPCClassInfoBox(Inworld);
//			if (PCInfoBox)
//			{
//				double LoadBeginTime = FPlatformTime::Seconds();
//				int32 LoadedCount = 0;
//				for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//				{
//					EPCClass ThisClassToLoad = IntToPCClass(PCI);
//
//					UB2PCClassInfo* ThisInfo = PCInfoBox->GetSingleClassInfo(ThisClassToLoad);
//					if (ThisInfo)
//					{
//						ThisInfo->SyncLoadInGameOnlyAssets();
//						++LoadedCount;
//					}
//				}
//
//				double TimeTaken = FPlatformTime::Seconds() - LoadBeginTime;
//
//				BII_SCREEN_LOG(FString::Printf(TEXT("Took %.4f s to load %d PCClassInfo."), TimeTaken, LoadedCount),
//					FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 10.0f);
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("NonCBTContent")))
//	{
//		/** CBTVersionReady �� true �� ���·� ��ŷ �׽�Ʈ�� �� �������� ������ ��Ȳ������ ��Ȳ ���. */
//		extern bool gbAllowNonCBTVersionContentOverride;
//		gbAllowNonCBTVersionContentOverride = !gbAllowNonCBTVersionContentOverride;
//
//		UB2StageInfo* StageInfoTable = StaticFindStageInfo();
//		if (StageInfoTable)
//		{ // ������ �־�� �ϴ� �� �־..
//			StageInfoTable->SetDataByCBTVersionContent();
//		}
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("AllowNonCBTVersionContentOverride %s."), gbAllowNonCBTVersionContentOverride ? TEXT("On") : TEXT("Off")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TAATest"))) // TemporalAA On/Off �׽�Ʈ ��
//	{
//		static bool bTAAOn = false;
//		bTAAOn = !bTAAOn;
//		static IConsoleVariable* CVarDefaultAntiAliasing = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DefaultFeature.AntiAliasing"));
//		if (CVarDefaultAntiAliasing)
//		{
//			CVarDefaultAntiAliasing->Set(bTAAOn ? (int32)EAntiAliasingMethod::AAM_TemporalAA : (int32)EAntiAliasingMethod::AAM_None, ECVF_SetByCommandline);
//		}
//
//		static IConsoleVariable* CVarPostProcessAAQuality = IConsoleManager::Get().FindConsoleVariable(TEXT("r.PostProcessAAQuality"));
//		if (CVarPostProcessAAQuality)
//		{
//			CVarPostProcessAAQuality->Set(bTAAOn ? 4 : 0);
//		}
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("TemporalAA %s."), bTAAOn ? TEXT("On") : TEXT("Off")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("FXAATest"))) // FXAA On/Off �׽�Ʈ ��.. �ٵ� �̰� ����ϼ� �� ����?
//	{
//		static bool bFXAAOn = false;
//		bFXAAOn = !bFXAAOn;
//		static IConsoleVariable* CVarDefaultAntiAliasing = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DefaultFeature.AntiAliasing"));
//		if (CVarDefaultAntiAliasing)
//		{
//			CVarDefaultAntiAliasing->Set(bFXAAOn ? (int32)EAntiAliasingMethod::AAM_FXAA: (int32)EAntiAliasingMethod::AAM_None, ECVF_SetByCommandline);
//		}
//
//		static IConsoleVariable* CVarPostProcessAAQuality = IConsoleManager::Get().FindConsoleVariable(TEXT("r.PostProcessAAQuality"));
//		if (CVarPostProcessAAQuality)
//		{
//			CVarPostProcessAAQuality->Set(bFXAAOn ? 2 : 0);
//		}
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("FXAA %s."), bFXAAOn ? TEXT("On") : TEXT("Off")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("MSAATest")))
//	{
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		bool bHasFirstArg = !FirstArg.IsEmpty();
//		
//		// ���� ������ �� �� �ʿ���� r.MobileMSAA �̰� �ϳ��� �� ������ �Ǿ� ����.
//		static IConsoleVariable* _CVarMobileMSAA = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileMSAA"));
//		if (_CVarMobileMSAA)
//		{
//			const int32 CurMSAAValue = _CVarMobileMSAA->GetInt();
//			int32 NewMSAAValue = FCString::Atoi(*FirstArg);
//			if (!bHasFirstArg)
//			{
//				// �������� �ʾҴٸ� On/Off ��������.. �⺻ ���ð��� 4�� ��.
//				NewMSAAValue = ((CurMSAAValue <= 1) ? 4 : 1);
//			}
//			else if (NewMSAAValue != 1 && NewMSAAValue != 2 && NewMSAAValue != 4 && NewMSAAValue != 8)
//			{ // ����ó��.
//				NewMSAAValue = 1; // 1 �� ���� ��.
//			}
//
//			// %%%%%%%%%%%%%%%%%%%%%
//			// ���⼭ ������ �� B2Scalability �� ��Ʈ�ѷ� ���� ���� �ε� ���� �Ѿ���� ���ӵ��� ���� ���̴�.
//			// �� Ŀ�ǵ�� ���� �׽�Ʈ�� �Ϸ��� B2Scalability �ʿ����� ��� �� ������.
//			// %%%%%%%%%%%%%%%%%%%%%
//
//			_CVarMobileMSAA->Set(NewMSAAValue);
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("MSAA %d. (On if same or bigger than 2.)"), _CVarMobileMSAA->GetInt()),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("B2ResLevel")))
//	{
//		// EB2ResolutionLevel �� UI ���� ���� �������̽��̰� EB2GraphicsRQType �� ���� ��� ���� ������� ������ �����ϴ� ����� ������ ���� ��.
//
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//
//		if (FirstArg == TEXT("Low") || FirstArg == TEXT("Mid") || FirstArg == TEXT("High"))
//		{
//			const EB2ResolutionLevel NewResLevel =
//				(FirstArg == TEXT("High")) ? EB2ResolutionLevel::High : (
//					(FirstArg == TEXT("Low")) ? EB2ResolutionLevel::Low : EB2ResolutionLevel::Mid
//					);
//			
//			// AdjustScalabilityBySavedUserSelection ���� ������ ���� �ҷ��� �״� ������ �ʿ�..
//			SaveGameSetting_Resolution(B2ResolutionLevelToInt(NewResLevel));
//
//			B2Scalability::AdjustScalabilityBySavedUserSelection(Inworld);
//			BII_SCREEN_LOG(FString::Printf(TEXT("ResLevel %d, GraphicsRQTypeSelection %d"), (int32)NewResLevel, (int32)B2Scalability::GetMappedRQTypeOfResolutionLevel(NewResLevel)),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ForcedRQType")))
//	{
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		int32 FirstArgInt = FCString::Atoi(*FirstArg);
//
//		static IConsoleVariable* DevForcedGraphicsRQTypeCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DevForcedGraphicsRQType"));
//		check(DevForcedGraphicsRQTypeCVar);
//		if (DevForcedGraphicsRQTypeCVar)
//		{
//			DevForcedGraphicsRQTypeCVar->Set(FirstArgInt);
//			B2Scalability::AdjustScalabilityBySavedUserSelection(Inworld);
//			BII_SCREEN_LOG(FString::Printf(TEXT("DevForcedGraphicsRQTypeCVar %d"), FirstArgInt),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("MCSF")))
//	{
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		float NewValue = FCString::Atof(*FirstArg);
//
//		static IConsoleVariable* CVarMCSF = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileContentScaleFactor"));
//		check(CVarMCSF);
//		if (CVarMCSF && NewValue > 0.0f)
//		{
//			CVarMCSF->Set(NewValue);
//			BII_SCREEN_LOG(FString::Printf(TEXT("MobileContentScaleFactor %f"), NewValue),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("DOFQ")))
//	{
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		int32 NewValue = FCString::Atoi(*FirstArg);
//
//		static IConsoleVariable* CVarDOF = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DepthOfFieldQuality"));
//		check(CVarDOF);
//		if (CVarDOF)
//		{
//			CVarDOF->Set(NewValue);
//			BII_SCREEN_LOG(FString::Printf(TEXT("DepthOfFieldQuality %d"), CVarDOF->GetInt()),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("BloomQ")))
//	{
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		int32 NewValue = FCString::Atoi(*FirstArg);
//
//		static IConsoleVariable* CVarBloom = IConsoleManager::Get().FindConsoleVariable(TEXT("r.BloomQuality"));
//		check(CVarBloom);
//		if (CVarBloom)
//		{
//			CVarBloom->Set(NewValue);
//			BII_SCREEN_LOG(FString::Printf(TEXT("BloomQuality %d"), CVarBloom->GetInt()),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TonemapperF")))
//	{
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		int32 NewValue = FCString::Atoi(*FirstArg);
//		// PC ���� CVar �� ���� ����. �̰� ����� ���� �׽�Ʈ �������� ���� ����.
//		static IConsoleVariable* CVarTonemapperF = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Mobile.TonemapperFilm"));
//		check(CVarTonemapperF);
//		if (CVarTonemapperF)
//		{
//			CVarTonemapperF->Set(NewValue);
//			BII_SCREEN_LOG(FString::Printf(TEXT("Mobile TonemapperFilm %d"), CVarTonemapperF->GetInt()),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("LensFlareQ")))
//	{
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		int32 NewValue = FCString::Atoi(*FirstArg);
//
//		static IConsoleVariable* CVarLFQ = IConsoleManager::Get().FindConsoleVariable(TEXT("r.LensFlareQuality"));
//		check(CVarLFQ);
//		if (CVarLFQ)
//		{
//			CVarLFQ->Set(NewValue);
//			BII_SCREEN_LOG(FString::Printf(TEXT("LensFlareQuality %d"), CVarLFQ->GetInt()),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("LFTest")))
//	{
//		static float LFTest_Intensity = -1.0f;
//		static float LFTest_BokehSize = -1.0f;
//		static float LFTest_Threshold = -1.0f;
//
//		FParse::Value(Cmd, TEXT("IT="), LFTest_Intensity);
//		FParse::Value(Cmd, TEXT("BS="), LFTest_BokehSize);
//		FParse::Value(Cmd, TEXT("TH="), LFTest_Threshold);
//
//		UCameraComponent* CurrCamComp = GetCurrentActiveCamComp(Inworld);
//		if (CurrCamComp)
//		{
//			if (LFTest_Intensity >= 0.0f)
//			{
//				CurrCamComp->PostProcessSettings.bOverride_LensFlareIntensity = true;
//				CurrCamComp->PostProcessSettings.LensFlareIntensity = LFTest_Intensity;
//			}
//			if (LFTest_BokehSize >= 0.0f)
//			{
//				CurrCamComp->PostProcessSettings.bOverride_LensFlareBokehSize = true;
//				CurrCamComp->PostProcessSettings.LensFlareBokehSize = LFTest_BokehSize;
//			}
//			if (LFTest_Threshold >= 0.0f)
//			{ // ����Ͽ��� ������ ���� ��..
//				CurrCamComp->PostProcessSettings.bOverride_LensFlareThreshold = true;
//				CurrCamComp->PostProcessSettings.LensFlareThreshold = LFTest_Threshold;
//			}
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("Applied LensFlare Test Param. Intensity %.2f, BokehSize %.2f, Threshold %.2f"),
//				LFTest_Intensity, LFTest_BokehSize, LFTest_Threshold),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("LVObjDM")))
//	{
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		int32 NewValue = FCString::Atoi(*FirstArg);
//
//		static IConsoleVariable* CVarLVObjDM = IConsoleManager::Get().FindConsoleVariable(TEXT("r.LevelObjectDetailMode"));
//		check(CVarLVObjDM);
//		if (CVarLVObjDM)
//		{
//			CVarLVObjDM->Set(NewValue);
//			BII_SCREEN_LOG(FString::Printf(TEXT("LevelObjectDetailMode %d"), CVarLVObjDM->GetInt()),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SMFrame")))
//	{
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		float fFirstValue = FCString::Atof(*FirstArg);
//
//		FString SecondArg;
//		FParse::Token(Cmd, SecondArg, true);
//		float fSecondValue = FCString::Atof(*SecondArg);
//
//		// ���� �� �� ����ϸ� Lower / Upper ��, �� ���� ����ϸ� Upper ��
//		float SpecifiedUpperBound = (fSecondValue > 0.0f) ? fSecondValue : fFirstValue;
//		float SpecifiedLowerBound = (fSecondValue > 0.0f) ? fFirstValue : 0.0f;
//
//		if (GEngine && SpecifiedUpperBound > 0.0f)
//		{
//			// �׽�Ʈ ���̴� �׳� �� �Ȱ��� �ٲ�ħ. �̷��� �ϰ� ���� r.CurrentMobileFrameLimitLevel �� �ǹ̰� ������ ��
//
//			// ���� �غ�Ǿ� �ִ� SmoothedFrameRateRange_Mobile ���ú��� ū �ε��������� ���� ū �� ����ϹǷ� Ȯ�� ���ɼ��� �����ؼ� ������ ū ���ڱ��� iteration ��.
//			for (int32 SFI = 0; SFI < 10; ++SFI) 
//			{
//				// ���������� 0 �̳� ���������� ���� ���� �����ϰ� �� ��.
//				GEngine->SetMobileSmoothFrameRangeOfLimitLevel(SFI, SpecifiedLowerBound, SpecifiedUpperBound);
//			}
//		}
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("SmoothedFrameRate Lower %.2f / Upper %.2f"), 
//			GEngine->SmoothedFrameRateRange.GetLowerBoundValue(), GEngine->SmoothedFrameRateRange.GetUpperBoundValue()),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		BII_SCREEN_LOG(FString::Printf(TEXT("SmoothedFrameRate_Mobile Lower %.2f / Upper %.2f"),
//			GEngine->GetCurrentMobileSmoothFrameRange().GetLowerBoundValue(), GEngine->GetCurrentMobileSmoothFrameRange().GetUpperBoundValue()),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("FrameLimitLevel")))
//	{ // GEngine->SmoothedFrameRateRange_Mobile** ���� ���� ���� �ǵ帮�� �ʰ� 
//		// ���� �ǵ��� ��ɴ�� SmoothedFrameRateRange_Mobile** �� �ϳ��� �����ϴ� ���� ������ ����.
//		IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.CurrentMobileFrameLimitLevel"));
//		if (CVar)
//		{
//			FString FirstArg;
//			FParse::Token(Cmd, FirstArg, true);
//
//			if (FirstArg == TEXT("Low") || FirstArg == TEXT("Mid") || FirstArg == TEXT("High"))
//			{ // ���� �޴����� ����� �ǵ��� �����Ǵ� ������� �������� ���� �������̽�
//				const EB2FrameLimitLevel NewRelativeLevel =
//					(FirstArg == TEXT("High")) ? EB2FrameLimitLevel::High :
//					(FirstArg == TEXT("Low") ? EB2FrameLimitLevel::Low : EB2FrameLimitLevel::Mid);
//
//				B2Scalability::SetRelativeFrameLimitLevel(NewRelativeLevel);
//
//				BII_SCREEN_LOG(FString::Printf(TEXT("RelativeFrameLimitLevel %d, r.CurrentMobileFrameLimitLevel %d"),
//					(int32)B2Scalability::GetCurrentRelativeFrameLimitLevel(), CVar->GetInt()),
//					FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//			}
//			else
//			{ // ���� �� ����
//				int32 NewValue = FCString::Atoi(*FirstArg);
//				CVar->Set(NewValue);
//				BII_SCREEN_LOG(FString::Printf(TEXT("r.CurrentMobileFrameLimitLevel %d"), CVar->GetInt()),
//					FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ForceRRTHack"))) 
//	{
//		// ��Ȥ ���� ��� ������ ���� �׽�Ʈ �����ε�.. �̰� �ذ� ����� �Ǵ� �� ������ ���� ä��.
//		IConsoleVariable* CVarSendSignal = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ReallocateRenderTargetsSignal"));
//		if (CVarSendSignal)
//		{
//			int32 ValueBefore = CVarSendSignal->GetInt();
//			ForceSendReallocateRenderTargetsSignal();
//			int32 ValueAfter = CVarSendSignal->GetInt();
//			BII_SCREEN_LOG(FString::Printf(TEXT("Called ReallocateRenderTargetsSignal.. %d -> %d. May the God help us!"), ValueBefore, ValueAfter),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GoToLobbyCharSel")))
//	{ // ��Ʈ��ũ ���� �ȵ� ���¿��� �κ� Ư�� ������� �̵��ϱ� ���� ����.
//		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::CharacterSelectPage);
//		return true;
//	}	
//	else if (FParse::Command(&Cmd, TEXT("GoToLobbyMain")))
//	{ // ��Ʈ��ũ ���� �ȵ� ���¿��� �κ� Ư�� ������� �̵��ϱ� ���� ����.
//		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::LobbyMain);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GoToLobbyShop")))
//	{ // ��Ʈ��ũ ���� �ȵ� ���¿��� �κ� Ư�� ������� �̵��ϱ� ���� ����.
//		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_SummonItem);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GoToLobbyInventory")))
//	{ // ��Ʈ��ũ ���� �ȵ� ���¿��� �κ� Ư�� ������� �̵��ϱ� ���� ����.
//		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PrepareLobbyHallOfFame")))
//	{ // �׳��� �������µ� ������ �� ä���־��.. �̰� Ŭ�� �ܵ��� �ƴϰ� ���� ���� ��Ȳ����.
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddRankingStatus 2 3 10 100"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddRankingStatus 3 3 10 100"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddRankingStatus 5 3 10 100"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("forceSettlement 2"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("forceSettlement 3"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("forceSettlement 5"));
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestStage")))
//	{ // Standalone �������� �÷��̸� �� �� ������ �ϱ� ����. �ܺ� ���� �׽�Ʈ ���忡���� �ʿ��ϱ⵵ �ϰ�..
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		const int32 WantedStageNum = FCString::Atoi(*FirstArg);
//
//		FString SecondArg;
//		FParse::Token(Cmd, SecondArg, true);
//		const bool bScenarioModeOn = (SecondArg == TEXT("-SM") || SecondArg == TEXT("-Scenario"));
//
//		// ScenarioMode �� �����ϸ� ���̵��� ������ Normal �� ��. ���̵��� �����ϰ� ���� ��쵵 �ι�° ���ڷ�.
//		EStageDifficulty WantedDifficulty = bScenarioModeOn ? EStageDifficulty::ESD_Normal :
//			(SecondArg == TEXT("-Hard") ? EStageDifficulty::ESD_Hard : 
//			(SecondArg == TEXT("-VeryHard") ? EStageDifficulty::ESD_VeryHard : 
//			(SecondArg == TEXT("-Hell") ? EStageDifficulty::ESD_Hell : EStageDifficulty::ESD_Normal)));
//
//		FString OpenStageCommand;
//		UB2StageInfo* StageInfoTable = StaticFindStageInfo();
//		if (StageInfoTable)
//		{
//			FSingleStageInfoData* WantedStageInfoData = StageInfoTable->GetInfoData(WantedStageNum, WantedDifficulty);
//			if (WantedStageInfoData)
//			{ // ��ǻ� Open ������� �� ���� �ִ�. �ٷ�� ������ ��ٷο��� ���� ������ ����� �����Ϸ��� ��.
//				OpenStageCommand = FString::Printf(TEXT("Open %s?StageNum=%d?SM=%d?DF=%d?Standalone") ,
//					*WantedStageInfoData->MainMap.ToString(),
//					WantedStageNum, (int32)bScenarioModeOn, (int32)WantedDifficulty
//				);
//			}
//		}
//
//		GlobalSetPureMapLoadingShareValue(0.5f); // ���� ���.
//
//		if (OpenStageCommand.Len() > 0)
//		{
//			GEngine->DeferredCommands.Add(OpenStageCommand);
//		}
//		else
//		{
//			BII_SCREEN_LOG(FString::Printf(TEXT("TestStage failed. Possibly due to wrong stage number.")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SetPlayerClass")))
//	{ // Standalone �÷��� ���� ��Ȳ���� ĳ���� �����ϰ��� �� ��
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		const EPCClass MainClass = IntToPCClass(FCString::Atoi(*FirstArg));
//		
//		FString SecondArg;
//		FParse::Token(Cmd, SecondArg, true);
//		const EPCClass SubClass = IntToPCClass(FCString::Atoi(*SecondArg));
//
//		if (SubClass != EPCClass::EPC_End && SubClass != MainClass) 
//		{ // Sub �� ���� �����ϴ� ������ ȭ�鿡 �α� �Ѹ� �� �̰� �Ʒ��� �и��� �Ϸ���
//			BladeIIGameImpl::GetLocalCharacterData().SetSubPlayerClass(SubClass);
//			BII_SCREEN_LOG(FString::Printf(TEXT("Set Sub Class to %s"), *UB2PCClassInfo::GetTrimmedNameOfPCClassEnum(SubClass)),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		if (MainClass != EPCClass::EPC_End)
//		{
//			BladeIIGameImpl::GetLocalCharacterData().SetMainPlayerClass(MainClass);
//			BII_SCREEN_LOG(FString::Printf(TEXT("Set Main Player Class to %s"), *UB2PCClassInfo::GetTrimmedNameOfPCClassEnum(MainClass)),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("MobileBloomDirt")))
//	{
//		IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileBloomDirt"));
//		if (CVar)
//		{
//			CVar->Set((CVar->GetInt() == 0) ? 1 : 0);
//			BII_SCREEN_LOG(FString::Printf(TEXT("MobileBloomDirt %d"), CVar->GetInt()),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 3.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestIapSleep")))
//	{
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		bool bIsOn = (FirstArg == TEXT("On")) ? true : false;
//
//		FString SecondArg;
//		FParse::Token(Cmd, SecondArg, true);
//		float fSecondValue = FCString::Atof(*SecondArg);
//
// 		//B2InAppPurchase::IB2IAPGenericPlatform::FakeSleepMode(bIsOn);
// 		//B2InAppPurchase::IB2IAPGenericPlatform::SetDelaySeconds(fSecondValue);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CamDistAlpha")))
//	{
//		IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.B2CameraDistanceAlpha"));
//		if (CVar)
//		{
//			FString FirstArg;
//			FParse::Token(Cmd, FirstArg, true);
//			float NewAmount = FCString::Atof(*FirstArg);
//			CVar->Set(NewAmount);
//
//			ApplyNormallyExpectedInGameCamera(Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(Inworld)), Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(Inworld)), 0.0f);
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("B2CameraDistanceAlpha %f"), CVar->GetFloat()),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 3.0f);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestCamDistScale")))
//	{
//		extern float GDevTestCameraDistanceScale;
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		GDevTestCameraDistanceScale = FCString::Atof(*FirstArg);
//
//		ApplyNormallyExpectedInGameCamera(Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(Inworld)), Cast<ABladeIIPlayerController>(UGameplayStatics::GetLocalPlayerController(Inworld)), 0.0f);
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("TestCameraDistanceScale %f"), GDevTestCameraDistanceScale),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 3.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SkillCoolTime")))
//	{
//		FString CoolTimeString;
//		FParse::Token(Cmd, CoolTimeString, true);
//
//		float NewCoolTime = FCString::Atof(*CoolTimeString);
//		UB2SkillRTManager::Cheat_ForceSetCooltime(false, NewCoolTime);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TagCoolTime")))
//	{
//		// "SkillCoolTime" �� �ڵ尡 ��ġ���� ���� ġƮ�̰� ���⿡ ���� �Լ� ����� �͵� ���������ϴ� �� �Ѿ��
//		FString CoolTimeString;
//		FParse::Token(Cmd, CoolTimeString, true);
//
//		float NewCoolTime = FCString::Atof(*CoolTimeString);
//		UB2SkillRTManager::Cheat_ForceSetCooltime(true, NewCoolTime);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("FixedSkillMode")))
//	{
//		// ���� ���� �׽�Ʈ �뵵�μ� ��ų ������ ��Ÿ�� ���س��� ���� ��.		
//		extern bool gCheatUseDummySkills;
//		gCheatUseDummySkills = !gCheatUseDummySkills;
//		
//		const float FixedSkillModeCoolTime = 15.0f;
//		UB2SkillRTManager::Cheat_ForceSetCooltime(false, gCheatUseDummySkills ? FixedSkillModeCoolTime : -1.0f);
//		UB2SkillRTManager::Cheat_ForceSetCooltime(true, gCheatUseDummySkills ? FixedSkillModeCoolTime : -1.0f);
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("FixedSkillMode %s"), gCheatUseDummySkills ? TEXT("ON") : TEXT("OFF")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 4.0f);
//
//		return true;
//	}
//
//	else if (FParse::Command(&Cmd, TEXT("PhysicsButtonPause")))
//	{
//#if PLATFORM_ANDROID
//#elif PLATFORM_IOS
//#else
//		UB2GameInstance* _B2GameInstance = Inworld->GetGameInstance<UB2GameInstance>();
//		if (_B2GameInstance)
//			_B2GameInstance->PlatformPauseDelegate();
//#endif
//		return true;
//	}
//
//	else if (FParse::Command(&Cmd, TEXT("PhysicsButtonResume")))
//	{
//#if PLATFORM_ANDROID
//#elif PLATFORM_IOS
//#else
//		UB2GameInstance* _B2GameInstance = Inworld->GetGameInstance<UB2GameInstance>();
//		if(_B2GameInstance)
//			_B2GameInstance->PlatformResumeDelegate();
//#endif
//		return true;
//	}
//#endif // BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2

	return false;
}
#endif //B2_BUILD_LEVEL < B2_BUILD_LEVEL_QA

#if WITH_EDITOR
bool UB2StaticExecHelper::ExecEditorOnly(UWorld* Inworld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	// ���߿� Ŀ�ǵ������� ����� �׽�Ʈ���� ����� �Ŷ�� �Ʒ� ExecDev �� �����մϴ�.

	// .paper2dsprites ������ ��������Ʈ �ҽ� ���Ͽ��� �� ��������Ʈ ���� ���� �����ͼ� ������ �̸��� MIC �� �ִ� atlas �Ķ���Ϳ� �־� �ִ� ���.
	// ��� ���� : B2UIAtlasParam Sprite=D:\DepotWS\BII-Dev\B2_TexAtlas_InGameHUD_PC.paper2dsprites MICPath=HUD/Materials AtlasTexture=/Game/HUD/Texture/PCHUDAtlas/Textures/B2_TexAtlas_InGameHUD_PC.B2_TexAtlas_InGameHUD_PC 
	// AtlasTexture_PCPreview=/Game/DevOnly/B2_TexAtlas_InGameHUD_PC_sRGB_PCTemp.B2_TexAtlas_InGameHUD_PC_sRGB_PCTemp
//	if (FParse::Command(&Cmd, TEXT("B2UIAtlasParam")))
//	{
//		FString SpriteName;
//		if (FParse::Value(Cmd, TEXT("Sprite="), SpriteName)) // ��������Ʈ ���� �ҽ� ���� ��ġ. **.paper2dsprites
//		{
//			FString MICPath;
//			FParse::Value(Cmd, TEXT("MICPath="), MICPath); // /Game/.../MIC�����̸� ������ ���
//
//			FString OptionalAtlasTexturePath; // �ؽ��Ĵ� �⺻ ������ �� �� ������, �ƴϸ� ���� ���� ���� �ְڴ�.
//			FParse::Value(Cmd, TEXT("AtlasTexture="), OptionalAtlasTexturePath);
//			UTexture2D* OptionalAtlasTexture = LoadObject<UTexture2D>(NULL, *OptionalAtlasTexturePath);
//
//			TArray<FString> SpriteFileContent;
//			FFileHelper::LoadANSITextFileToStrings(*SpriteName, &IFileManager::Get(), SpriteFileContent); // ���� ��ƿ��Ƽ�α� ����
//
//			// ��ü ���������
//			int32 TextureWidth = 0;
//			int32 TextureHeight = 0;
//			for (int32 FCI = SpriteFileContent.Num() - 1; FCI >= 0; --FCI) // �� ������ �Ƹ��� ���� �ϴܿ� ���� ����.
//			{
//				FString CurrLine = SpriteFileContent[FCI];
//				if (CurrLine.StartsWith(TEXT("\t\"size")))
//				{
//					FParse::Value(*CurrLine, TEXT("\"w\":"), TextureWidth); // Should stop on comma. 
//					FParse::Value(*CurrLine, TEXT("\"h\":"), TextureHeight);
//					break;
//				}
//			}
//
//			if (TextureWidth > 0 && TextureHeight > 0)
//			{
//				// �� ��������Ʈ �������� �̸� ������ ���� �Ľ̵Ǿ�� ��.
//				FString CurrSpriteName;
//				FString CurrMatchingMICName;
//				FString CurrMatchingMICName2; // For additional naming rule
//				FString CurrMatchingSpriteName; // We might need sprite too for use of image draw batching in wrap-box.
//				for (int32 FCI = 0; FCI < SpriteFileContent.Num(); ++FCI)
//				{
//					FString CurrLine = SpriteFileContent[FCI];
//
//					if (CurrLine.StartsWith(TEXT("\"")) && CurrLine.EndsWith(TEXT("\":"))) // ��������Ʈ �̸� ������ ����
//					{
//						CurrSpriteName = CurrLine;
//						CurrSpriteName.RemoveAt(0, 1); // Remove first "
//						int32 LastQuoteStartIndex = CurrSpriteName.Find(TEXT("\""));
//						if (LastQuoteStartIndex != INDEX_NONE)
//						{
//							CurrSpriteName.RemoveAt(LastQuoteStartIndex, 2); // Remove last ":
//						}
//
//						FString ExtRemovedName = CurrSpriteName;
//						// MIC �̸������� ���� Ȯ���� �����ϰ� ������ ���ξ� �� ������ �߰� ��θ� ���δ�.
//						int32 ExtensionStartIndex = ExtRemovedName.Find(TEXT("."));
//						if (ExtensionStartIndex != INDEX_NONE)
//						{
//							ExtRemovedName.RemoveAt(ExtensionStartIndex, 4); // e.g. Removing ".png", ".tga"
//
//						}
//						// Then, add our naming convention.
//						CurrMatchingMICName = TEXT("MAT_") + ExtRemovedName;
//						CurrMatchingMICName2 = TEXT("MAT_S_") + ExtRemovedName; // �⺻���� �̹��� �Ѹ��� �̿��� �ణ Ư���� ȿ���� �� �͵��ε�, �̰͵��� ��ü�� material �ϳ��� MIC �ϳ��� ���� ���� �� ��. �ؽ��� ���ø��� �߰��Ǵ��� �⺻���� ��Ʋ�� �Ķ���ʹ� �ϴ� ���������� ��.
//						CurrMatchingSpriteName = TEXT("MAT_Spr_") + ExtRemovedName; // Material �� �������� ġ��..
//						CurrMatchingMICName = TEXT("/Game/") + MICPath + TEXT("/") + CurrMatchingMICName + TEXT(".") + CurrMatchingMICName;
//						CurrMatchingMICName2 = TEXT("/Game/") + MICPath + TEXT("/") + CurrMatchingMICName2 + TEXT(".") + CurrMatchingMICName2;
//						CurrMatchingSpriteName = TEXT("/Game/") + MICPath + TEXT("/") + CurrMatchingSpriteName + TEXT(".") + CurrMatchingSpriteName;
//					}
//					else if (CurrLine.StartsWith(TEXT("\t\"frame")))
//					{
//						int32 XCoord = 0;
//						FParse::Value(*CurrLine, TEXT("\"x\":"), XCoord); // Should stop on comma. 
//
//						int32 YCoord = 0;
//						FParse::Value(*CurrLine, TEXT("\"y\":"), YCoord);
//
//						int32 SizeX = 0;
//						FParse::Value(*CurrLine, TEXT("\"w\":"), SizeX);
//
//						int32 SizeY = 0;
//						FParse::Value(*CurrLine, TEXT("\"h\":"), SizeY);
//
//						UMaterialInstanceConstant* MatchingMIC = LoadObject<UMaterialInstanceConstant>(NULL, *CurrMatchingMICName);
//						UMaterialInstanceConstant* MatchingMIC2 = LoadObject<UMaterialInstanceConstant>(NULL, *CurrMatchingMICName2);
//
//						for (int32 MII = 0; MII < 2; ++MII)
//						{
//							UMaterialInstanceConstant* CurrMIC = (MII == 0) ? MatchingMIC : MatchingMIC2;
//
//							// NoAtlas ������ �� �� Ŀ�ǵ忡�� �����ؾ� �Ѵ�. ���̹��� ó������ �̷��� �� ������ �ȵ�����.. Atlas - NonAtlas ��ȯ ������ ���� ���� �����Ƿ� üũ.
//							bool bThisMICNoAtlas = false;
//							FGuid ExpressionGuid;
//							/*if (CurrMIC && CurrMIC->GetStaticSwitchParameterValue(MtrlStaticParamName_NoAtlas, bThisMICNoAtlas, ExpressionGuid))
//							{
//								if (bThisMICNoAtlas)
//								{
//									continue;
//								}
//							}*/
//
//							//if (CurrMIC && XCoord >= 0 && YCoord >= 0 && SizeX > 0 && SizeY > 0)
//							//{
//							//	// UI TexAtlas master material �� �־���� �Ķ���� �̸�
//							//	CurrMIC->SetScalarParameterValueEditorOnly(MtrlParamName_UIAtlasSampleX, XCoord);
//							//	CurrMIC->SetScalarParameterValueEditorOnly(MtrlParamName_UIAtlasSampleY, YCoord);
//							//	CurrMIC->SetScalarParameterValueEditorOnly(MtrlParamName_UIAtlasSampleWidth, SizeX);
//							//	CurrMIC->SetScalarParameterValueEditorOnly(MtrlParamName_UIAtlasSampleHeight, SizeY);
//							//	CurrMIC->SetScalarParameterValueEditorOnly(MtrlParamName_UIAtlasWholeWidth, TextureWidth);
//							//	CurrMIC->SetScalarParameterValueEditorOnly(MtrlParamName_UIAtlasWholeHeight, TextureHeight);
//
//							//	if (OptionalAtlasTexture)
//							//	{
//							//		CurrMIC->SetTextureParameterValueEditorOnly(MtrlParamName_UIAtlasTexture, OptionalAtlasTexture);
//							//	}
//
//							//	CurrMIC->MarkPackageDirty();
//							//}
//						}
//						// 4.14 ����.. WrapBox �ȿ����� ����ȭ�� ���� Paper2D Sprite �� ����.
//						UPaperSprite* MatchingPaperSprite = LoadObject<UPaperSprite>(NULL, *CurrMatchingSpriteName);
//						// �� �⺻ ���ֵ̹� üũ�� ����. ���غ� �� ������
//						if (!MatchingPaperSprite)
//						{
//							MatchingPaperSprite = LoadObject<UPaperSprite>(NULL, *CurrMatchingMICName);
//							if (!MatchingPaperSprite)
//							{
//								MatchingPaperSprite = LoadObject<UPaperSprite>(NULL, *CurrMatchingMICName2);
//							}
//						}
//						if (MatchingPaperSprite && XCoord >= 0 && YCoord >= 0 && SizeX > 0 && SizeY > 0)
//						{
//							MatchingPaperSprite->EditorSetEssentialParam(FVector2D(XCoord, YCoord), FVector2D(SizeX, SizeY), FVector2D(TextureWidth, TextureHeight), OptionalAtlasTexture);
//							MatchingPaperSprite->MarkPackageDirty();
//						}
//					}
//				}
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("AutoReplaceAtlasTexture")))
//	{ // �ӽ� ������μ� ������ Atlas �ؽ��ķ� ����ϴ� �� ������ �ɰ��� �ؽ��ķ� �ٲٵ��� �Ϸ��� �ϰ� ó��. 
//		// ������ ����� ���� �־ ������ �־��.
//		FString MICPath;
//		FParse::Value(Cmd, TEXT("MICPath="), MICPath); // /Game/.../MIC�����̸� ������ ���
//		FString MICNameFilter;
//		FParse::Value(Cmd, TEXT("MICNameFilter="), MICNameFilter);
//
//		FString TexturePath;
//		FParse::Value(Cmd, TEXT("TexturePath="), TexturePath); // /Game/.../MIC�����̸� ������ ���
//		FString TextureNameFilter;
//		FParse::Value(Cmd, TEXT("TextureNameFilter="), TextureNameFilter);
//
//		TArray<UMaterialInstanceConstant*> AllMICs;
//		EditorFindAndLoadAssets<UMaterialInstanceConstant>(MICPath, MICNameFilter, AllMICs, false); // ������ ���� ���� ���� ������ �־ recursive ���� �ʰ�.. �ʿ信 ���� �ٸ��� �ִ���
//
//		TArray<UTexture2D*> AllTextures;
//		EditorFindAndLoadAssets<UTexture2D>(TexturePath, TextureNameFilter, AllTextures, false);
//
//		// AllMICs �� AllTextures �� ���� ������ ����.. �ƹ����� �ٸ� �� ������..
//		UE_LOG(LogBladeII, Log, TEXT("Found %d MICs from %s, %d Textures from %s"), AllMICs.Num(), *MICPath, AllTextures.Num(), *TexturePath);
//
//		for (int32 MI = 0; MI < AllMICs.Num(); ++MI)
//		{
//			UMaterialInstanceConstant* ThisMIC = AllMICs[MI];
//			UTexture* MatchingTexture = NULL;
//			if (ThisMIC)
//			{
//				// ������ ���̹��� �ؽ��ĸ� ã�Ƽ�.. ������ �Ķ���� ����. ������ �Ѿ����.
//
//				FString ThisName = ThisMIC->GetName();
//				if (ThisName.StartsWith(TEXT("MAT_"))) // �̹� MICNameFilter �� �ɷ��� ���� ������.
//				{
//					ThisName.RemoveFromStart(TEXT("MAT_"));
//					ThisName = TEXT("TEX_") + ThisName;
//
//					// �ٽ� LoadObject �� ���� �ְ�����.. �� Ŀ�ǵ� ��� ���� �� ã�Ƴ� �ſ���.
//					for (UTexture* TextureFromList : AllTextures)
//					{
//						if (TextureFromList && TextureFromList->GetName() == ThisName)
//						{
//							MatchingTexture = TextureFromList;
//							break;
//						}
//					}
//				}
//
//				if (MatchingTexture)
//				{ // ���� ���� �۾�. StaticSwitchParameter �� �̷��Զ� ����Ǵ� �����ε� �� �������̽� ����?
//					FStaticParameterSet TheStaticParamSet;
//					ThisMIC->GetStaticParameterValues(TheStaticParamSet);
//					for (FStaticSwitchParameter& SSParam : TheStaticParamSet.StaticSwitchParameters)
//					{
//						if (SSParam.ParameterName == MtrlStaticParamName_NoAtlas)
//						{
//							SSParam.bOverride = true;
//							SSParam.Value = true;
//							break;
//						}
//					}
//					ThisMIC->UpdateStaticPermutation(TheStaticParamSet);
//					ThisMIC->SetTextureParameterValueEditorOnly(MtrlParamName_UIAtlasTexture, nullptr); // ���� �� ���۷����� �����߰ڴ�. �׷��� serialize �� �� ����.
//					ThisMIC->SetTextureParameterValueEditorOnly(MtrlParamName_NoAtlasSingleTexture, MatchingTexture);
//					ThisMIC->MarkPackageDirty();
//				}
//				else
//				{
//					// Ȯ���� �� �ʿ䰡 �ִ�.. �ٵ� �� ������. ���� �� ���ɰ� �ǰ�
//					UE_LOG(LogBladeII, Warning, TEXT("[AutoReplaceAtlasTexture] Cannot Find matching texture for MIC %s !"), *ThisMIC->GetPathName())
//				}
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ProcessImportedUITextures")))
//	{ // �� �뷮���� ó���Ϸ���
//		FString TexturePath;
//		FParse::Value(Cmd, TEXT("TexturePath="), TexturePath); // /Game/.../MIC�����̸� ������ ���
//		FString TextureNameFilter;
//		FParse::Value(Cmd, TEXT("TextureNameFilter="), TextureNameFilter);
//
//		TArray<UTexture2D*> AllTextures;
//		EditorFindAndLoadAssets<UTexture2D>(TexturePath, TextureNameFilter, AllTextures, true);
//
//		UE_LOG(LogBladeII, Log, TEXT("Found %d Textures from %s"), AllTextures.Num(), *TexturePath);
//
//		// �ٽ� LoadObject �� ���� �ְ�����.. �� Ŀ�ǵ� ��� ���� �� ã�Ƴ� �ſ���.
//		for (UTexture* TextureFromList : AllTextures)
//		{
//			if (TextureFromList)
//			{
//				TextureFromList->LODGroup = TextureGroup::TEXTUREGROUP_UI;
//				TextureFromList->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps; // UI �� ��� ���� MipLODBias �ָ� ���� �ѵ�.. ��Ʈ���� �Ǵ� �� �ƴ϶�.. �𸣰ڴ�.
//				TextureFromList->CompressionSettings = TextureCompressionSettings::TC_Default;
//				TextureFromList->MarkPackageDirty();
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GradeUpAnimation")))
//	{
//		auto GameModeEnum = GetB2GameModeType(this);
//
//		FString strBeforeGrade, strBeforeStar, strStar;
//		FParse::Token(Cmd, strBeforeGrade, true);
//		FParse::Token(Cmd, strBeforeStar, true);
//		FParse::Token(Cmd, strStar, true);
//
//		auto BeforeGrade = FCString::Atoi(*strBeforeGrade);
//		auto BeforeStarCount = FCString::Atoi(*strBeforeStar);
//		auto WantedStarCount = FCString::Atoi(*strStar);
//
//		auto* PvPResult = UB2UIManager::GetInstance()->GetUI<UB2UIPVPResult>("PvPResult");
//		if(PvPResult)
//			PvPResult->SetCommandRankUp(BeforeGrade, BeforeStarCount, WantedStarCount);
//	}
//	else if (FParse::Command(&Cmd, TEXT("ClearUnusedAtlasTextureParam")))
//	{ // NoAtlas �� ���߿� �ٲ� �͵��� ������ ���� ���۷����� �����ִ� ����.. �̰� �������� ������ �ҿ��� ����..
//		FString MICPath;
//		FParse::Value(Cmd, TEXT("MICPath="), MICPath); // /Game/.../MIC�����̸� ������ ���
//		FString MICNameFilter;
//		FParse::Value(Cmd, TEXT("MICNameFilter="), MICNameFilter);
//
//		TArray<UMaterialInstanceConstant*> AllMICs;
//		EditorFindAndLoadAssets<UMaterialInstanceConstant>(MICPath, MICNameFilter, AllMICs, true);
//
//		UE_LOG(LogBladeII, Log, TEXT("Found %d MICs from %s"), AllMICs.Num(), *MICPath);
//
//		int32 ParamChangedCount = 0;
//		for (int32 MI = 0; MI < AllMICs.Num(); ++MI)
//		{
//			UMaterialInstanceConstant* ThisMIC = AllMICs[MI];
//			if (ThisMIC)
//			{
//				bool bIsNoAtlasOn = false;
//				FGuid NoAtlasParamGuid;
//				ThisMIC->GetStaticSwitchParameterValue(MtrlStaticParamName_NoAtlas, bIsNoAtlasOn, NoAtlasParamGuid);
//				if (bIsNoAtlasOn)
//				{
//					UTexture* ExistingAtlasTexParamValue = NULL; // �̰� �� �α��
//					ThisMIC->GetTextureParameterOverrideValue(MtrlParamName_UIAtlasTexture, ExistingAtlasTexParamValue);
//
//					if (ExistingAtlasTexParamValue)
//					{
//						++ParamChangedCount;
//						ThisMIC->SetTextureParameterValueEditorOnly(MtrlParamName_UIAtlasTexture, nullptr); // NoAtlas switch �� ���� ������ ������ ��� ���ϴ� �ؽ��İ� �������� �޸𸮸� ������ ������.
//
//						UE_LOG(LogBladeII, Log, TEXT("%d. NULL-out MIC param %s. from %s (previously set as %s)"), 
//							ParamChangedCount, *MtrlParamName_UIAtlasTexture.ToString(), *ThisMIC->GetPathName(),
//							*ExistingAtlasTexParamValue->GetPathName());
//
//						ThisMIC->MarkPackageDirty();
//					}
//				}
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("AutoNoAffectNavMesh")))
//	{
//		if (GIsEditor)
//		{
//			// �Ϻ� NavMesh �� ������ ��ġ�� ���ƾ� �� Ŭ�����鿡 ���� CanEverAffectNavigation �ڵ� ���� Ŀ�ǵ�
//			for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//			{
//				// ������ �ʿ��� Ŭ������. �ʿ��ϸ� �߰�.
//				if (Cast<AB2MonsterSpawnPool>(*ActorIt) || Cast<AB2DestructibleLevelObjBase>(*ActorIt) || Cast<AB2LevelDoorBase>(*ActorIt))
//				{
//					TInlineComponentArray<UPrimitiveComponent*> AllPrimComps;
//					(*ActorIt)->GetComponents(AllPrimComps);
//					for (int32 CI = 0; CI < AllPrimComps.Num(); ++CI)
//					{
//						UPrimitiveComponent* CurrComp = AllPrimComps[CI];
//						if (CurrComp->CanEverAffectNavigation())
//						{
//							CurrComp->SetCanEverAffectNavigation(false);
//							CurrComp->MarkPackageDirty();
//						}
//					}
//				}
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CompileLOC")))
//	{
//		BladeIILocalize::CompileLOC();
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("AutoReplaceUButtons")))
//	{
//		// B2Button ��ü�� ���� Ŀ�ǵ�
//		FString WidgetPath;
//		FParse::Token(Cmd, WidgetPath, true);
//
//		UWidgetBlueprint* FoundWigetBP = LoadObject<UWidgetBlueprint>(NULL, *WidgetPath);
//		if (FoundWigetBP)
//		{
//#if !PLATFORM_MAC
//			ReplaceSubWidgetClass(FoundWigetBP, UButton::StaticClass(), UB2Button::StaticClass());
//#endif
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("AutoGenerateSafeZone")) || FParse::Command(&Cmd, TEXT("AutoGenSafeZone")))
//	{
//		FString WidgetPath;
//		FParse::Token(Cmd, WidgetPath, true);
//
//		UWidgetBlueprint* FoundWigetBP = LoadObject<UWidgetBlueprint>(NULL, *WidgetPath);
//		if (FoundWigetBP)
//		{
//			int32 MovedChildrenNum = 0;
//			if (FoundWigetBP->AutoGenerateSafeZoneWidget(MovedChildrenNum))
//			{
//#if !PLATFORM_MAC
//				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(
//					TEXT("%s �� SafeZone �� �ڵ����� �����Ͽ����ϴ�.\r\n\r\n")
//					TEXT("Root �Ʒ��� ��ġ�Ǿ� �ִ� %d ���� widget ���� ���� ������ SafeZone �Ʒ��� PanelWidget���� �Ű������ϴ�.\r\n\r\n")
//					TEXT("WidgetBP�� ���� ������ �� ���̾ƿ��� Ȯ���� ������."),
//					*FoundWigetBP->GetPathName(), MovedChildrenNum
//				)));
//#endif
//			}
//			else
//			{
//#if !PLATFORM_MAC
//				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(
//					TEXT("%s �� SafeZone �ڵ� ������ �����߽��ϴ�.\r\n\r\n")
//					TEXT("�̹� SafeZone �� �ְų�, �ֻ�� RootWidget �� Ÿ���� Ư���� ���� ������ �� �ֽ��ϴ�."),
//					*FoundWigetBP->GetPathName()
//				)));
//#endif
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("AutoReplaceUButtonsDir")))
//	{
//		// ReplaceSubWidgetClass �� ���� °��. ���� ��η� �����Ѵ�.
//		FString SearchFolder;
//		FParse::Token(Cmd, SearchFolder, true);
//		SearchFolder = SearchFolder.Replace(TEXT("\\"), TEXT("/")); // ������ �׳� �������� ����ϴ� �� ���� ����.
//		FPaths::RemoveDuplicateSlashes(SearchFolder);
//
//		TArray<FString> FoundWidgetFiles;
//		IFileManager::Get().FindFiles(FoundWidgetFiles, *SearchFolder, TEXT(".uasset"));
//		
//		// �����θ� ���� ��η� �ٲ۴�. �̰� ��ƿ�� ��� �ִ��� �𸣰ڴµ�..
//		FString LoadFolder = EditorGetGameRelativePathFromAbsPath(SearchFolder);
//
//		for (FString ThisFile : FoundWidgetFiles)
//		{
//			ThisFile.RemoveFromEnd(TEXT(".uasset"));
//			FString ThisWidgetPath = TEXT("/") + LoadFolder + TEXT("/") + ThisFile + TEXT(".") + ThisFile; // ���� �����ܿ� Ctrl+C �ϸ� ����Ǵ� ��� ���������� ���߱�.
//
//			UWidgetBlueprint* FoundWigetBP = LoadObject<UWidgetBlueprint>(NULL, *ThisWidgetPath);
//			if (FoundWigetBP)
//			{
//#if !PLATFORM_MAC
//				ReplaceSubWidgetClass(FoundWigetBP, UButton::StaticClass(), UB2Button::StaticClass(), true); // �̰� �޽��� �ڽ� �ȶ��� ����.
//#endif
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("AutoSetButtonSoundIndex")))
//	{
//		FString AutoSetNumStr;
//		FParse::Token(Cmd, AutoSetNumStr, true);
//		int32 AutoSetNum = FCString::Atoi(*AutoSetNumStr);
//
//		FString SearchFolder;
//		FParse::Token(Cmd, SearchFolder, true);
//		SearchFolder = SearchFolder.Replace(TEXT("\\"), TEXT("/")); // ������ �׳� �������� ����ϴ� �� ���� ����.
//		FPaths::RemoveDuplicateSlashes(SearchFolder);
//
//		TArray<FString> FoundWidgetFiles;
//		IFileManager::Get().FindFiles(FoundWidgetFiles, *SearchFolder, TEXT(".uasset"));
//
//		// �����θ� ���� ��η� �ٲ۴�. �̰� ��ƿ�� ��� �ִ��� �𸣰ڴµ�..
//		FString LoadFolder = EditorGetGameRelativePathFromAbsPath(SearchFolder);
//
//		for (FString ThisFile : FoundWidgetFiles)
//		{
//			ThisFile.RemoveFromEnd(TEXT(".uasset"));
//			FString ThisWidgetPath = TEXT("/") + LoadFolder + TEXT("/") + ThisFile + TEXT(".") + ThisFile; // ���� �����ܿ� Ctrl+C �ϸ� ����Ǵ� ��� ���������� ���߱�.
//
//			UWidgetBlueprint* FoundWigetBP = LoadObject<UWidgetBlueprint>(NULL, *ThisWidgetPath);
//			if (FoundWigetBP)
//			{
//#if !PLATFORM_MAC
//				ForceSetButtonClickSoundIndex(FoundWigetBP, AutoSetNum, true);
//#endif
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TryChangeUIResourcePathSpecial")))
//	{ // �ſ� Ư���� ���� ������ �ְ� �� ��. UI ������ �ϸ鼭 ������ �̸��� ��Ƽ������� �ٸ� ��ο� �������� ���ݱ��� ���� �͵� �ٲ�ġ�� �Ϸ��� ��Ȳ�ε� �̷��� �Ϸ��� redirector ���� ó���� �Ǵ� �͵� �ƴϰ�..
//		// �ᱹ ���۾��� �ʿ��ѵ� ������ �������� �ڵ����� ó�� �õ�.
//		FString WidgetSearchPath;
//		FParse::Value(Cmd, TEXT("WidgetPath="), WidgetSearchPath);
//		FString WidgetSearchNameFilter;
//		FParse::Value(Cmd, TEXT("WidgetFilter="), WidgetSearchNameFilter);
//
//		FString OldAssetPath; // ��ü�ϰ��� �ϴ� ���� ��� ����
//		FParse::Value(Cmd, TEXT("OldPath="), OldAssetPath);
//		FString NewAssetPath; // OldMaterialPath �� �̰ɷ� ��ü�� ���ҽ��� ã�Ƽ� ������ ����.
//		FParse::Value(Cmd, TEXT("NewPath="), NewAssetPath);
//
//		TArray<UWidgetBlueprint*> AllFoundWB;
//		EditorFindAndLoadAssets<UWidgetBlueprint>(WidgetSearchPath, WidgetSearchNameFilter, AllFoundWB);
//
//		TArray<FString> AllModifiedWidgets;
//		for (int32 WI = 0; WI < AllFoundWB.Num(); ++WI)
//		{
//			UWidgetBlueprint* ThisWB = AllFoundWB[WI];
//			if (!ThisWB) {
//				continue;
//			}
//#if PLATFORM_MAC
//			int32 ModifiedCount = 0;
//#else
//			// �˰� �ִ� Ÿ���� ���ҽ���. ���⼱ ��� �ؽ��Ķ� ��Ƽ���� �ƴϸ�.. ������ ��������Ʈ�� �ֱ�.
//			// �װͺ��� ReplaceSubWidgetResource �ȿ��� ó���ϴ� widget type �� �������� ����. ��� ���� ���۾��� ��¿ �� ����.
//			int32 ModifiedCount = ReplaceSubWidgetResources<UMaterialInterface>(ThisWB, OldAssetPath, NewAssetPath);
//			ModifiedCount += ReplaceSubWidgetResources<UTexture>(ThisWB, OldAssetPath, NewAssetPath);
//			ModifiedCount += ReplaceSubWidgetResources<UPaperSprite>(ThisWB, OldAssetPath, NewAssetPath);
//#endif
//			if (ModifiedCount > 0)
//			{
//#if !PLATFORM_MAC
//				AllModifiedWidgets.Add(FString::Printf(TEXT("%s, %d ��"), *ThisWB->GetPathName(), ModifiedCount));
//#endif
//				ThisWB->MarkPackageDirty();
//			}
//		}
//		if (AllModifiedWidgets.Num())
//		{
//#if !PLATFORM_MAC
//			FString MsgStr = TEXT("���� Widget Blueprint �鿡�� ���ҽ����� ����\r\n�ٲ� Widget Blueprint ���� ������ �� ������ �� �ּ���!\r\n\r\n");
//			for (int32 WI = 0; WI < AllModifiedWidgets.Num(); ++WI)
//			{
//				const FString& ThisPath = AllModifiedWidgets[WI];
//				MsgStr += FString::Printf(TEXT("%d. %s\r\n"), WI + 1, *ThisPath);
//			}
//			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(MsgStr));
//#endif
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestItemLevelupScene")))
//	{ // ������ ���� �׽�Ʈ ����.
//		FString IngredNumStr;
//		FParse::Token(Cmd, IngredNumStr, true);
//		int32 IngreNum = FCString::Atoi(*IngredNumStr);
//
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		if (LobbyGM && LobbyGM->GetLobbyInventory())
//		{
//			LobbyGM->GetLobbyInventory()->DevOnlyItemLevelupProgressSceneTest(IngreNum);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestItemEnhanceScene")))
//	{ // ��ȭ ���� �׽�Ʈ ����.
//		FString IngredNumStr;
//		FParse::Token(Cmd, IngredNumStr, true);
//		int32 IngreNum = FCString::Atoi(*IngredNumStr);
//
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		if (LobbyGM && LobbyGM->GetLobbyInventory())
//		{
//			LobbyGM->GetLobbyInventory()->DevOnlyItemEnhanceProgressSceneTest(IngreNum);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestItemComposeScene")))
//	{ // �ռ� ���� �׽�Ʈ ����.
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		if (LobbyGM && LobbyGM->GetLobbyInventory())
//		{
//			LobbyGM->GetLobbyInventory()->DevOnlyItemCUSProgressSceneTest(ELobbyInvenItemOpMode::EIVIOP_Composition);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestItemUpgradeScene")))
//	{ // �±� ���� �׽�Ʈ ����.
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		if (LobbyGM && LobbyGM->GetLobbyInventory())
//		{
//			LobbyGM->GetLobbyInventory()->DevOnlyItemCUSProgressSceneTest(ELobbyInvenItemOpMode::EIVIOP_Upgrade);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestItemSurpassScene")))
//	{ // �ʿ� ���� �׽�Ʈ ����.
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		if (LobbyGM && LobbyGM->GetLobbyInventory())
//		{
//			LobbyGM->GetLobbyInventory()->DevOnlyItemCUSProgressSceneTest(ELobbyInvenItemOpMode::EIVIOP_Surpass);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PVPPreview")))
//	{
//		extern bool GbEditorPVPGamePreview;
//		extern EPCClass GEditorPVPPreviewMyClassMain;
//		extern EPCClass GEditorPVPPreviewMyClassTag;
//		extern EPCClass GEditorPVPPreviewRemoteClassMain;
//		extern EPCClass GEditorPVPPreviewRemoteClassTag;
//
//		FString MyMainClassStr;
//		bool bHasMyMainClassStr = FParse::Token(Cmd, MyMainClassStr, true);
//		FString MyTagClassStr;
//		bool bHasMyTagClassStr = FParse::Token(Cmd, MyTagClassStr, true);
//		FString RemoteMainClassStr;
//		bool bHasRemoteMainClassStr = FParse::Token(Cmd, RemoteMainClassStr, true);
//		FString RemoteTagClassStr;
//		bool bHasRemoteTagClassStr = FParse::Token(Cmd, RemoteTagClassStr, true);
//
//		if (!bHasMyMainClassStr && !bHasMyTagClassStr && !bHasRemoteMainClassStr && !bHasRemoteTagClassStr)
//		{ // On/Off ����ġ�� ��Ÿ ���� �������� ��츸.
//			GbEditorPVPGamePreview = !GbEditorPVPGamePreview;
//		}
//		else
//		{
//			GbEditorPVPGamePreview = true; // ������ �Ǿ��ٸ� ������ �Ѵ� ��
//
//			GEditorPVPPreviewMyClassMain = IntToPCClass(FMath::Clamp(FCString::Atoi(*MyMainClassStr), 0, GetMaxPCClassNum() - 1));
//			GEditorPVPPreviewMyClassTag = IntToPCClass(FMath::Clamp(FCString::Atoi(*MyTagClassStr), 0, GetMaxPCClassNum() - 1));
//			GEditorPVPPreviewRemoteClassMain = IntToPCClass(FMath::Clamp(FCString::Atoi(*RemoteMainClassStr), 0, GetMaxPCClassNum() - 1));
//			GEditorPVPPreviewRemoteClassTag = IntToPCClass(FMath::Clamp(FCString::Atoi(*RemoteTagClassStr), 0, GetMaxPCClassNum() - 1));
//		}
//
//#if !PLATFORM_MAC
//		FString MsgStr = FString::Printf(TEXT("PVP ��� ������ ���� ������ %s."), GbEditorPVPGamePreview ? TEXT("����") : TEXT("����"));
//		if (GbEditorPVPGamePreview)
//		{
//			MsgStr += FString::Printf(TEXT("\n\n���� �÷��̾� ���� ĳ����: %d"), (int32)GEditorPVPPreviewMyClassMain);
//			MsgStr += FString::Printf(TEXT("\n���� �÷��̾� �±� ĳ����: %d"), (int32)GEditorPVPPreviewMyClassTag);
//			MsgStr += FString::Printf(TEXT("\n���ݻ�� �÷��̾� ���� ĳ����: %d"), (int32)GEditorPVPPreviewRemoteClassMain);
//			MsgStr += FString::Printf(TEXT("\n���ݻ�� �÷��̾� �±� ĳ����: %d"), (int32)GEditorPVPPreviewRemoteClassTag);
//			MsgStr += TEXT("\n\nPVP ��� ������ ������� �Ҿ����� �� ������ �÷��� ���� �۾����� ������ �ּ���.");
//		}
//		
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(MsgStr));
//#endif
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("RaidPreview")))
//	{
//		extern bool g_IsEditorRaidGamePreview;
//		extern EPCClass g_EditorRaidPreviewPCClassMain[4];
//		
//		FString PCClassStr_0, PCClassStr_1, PCClassStr_2, PCClassStr_3;
//		bool bPCClass_0 = FParse::Token(Cmd, PCClassStr_0, true);
//		bool bPCClass_1 = FParse::Token(Cmd, PCClassStr_1, true);
//		bool bPCClass_2 = FParse::Token(Cmd, PCClassStr_2, true);
//		bool bPCClass_3 = FParse::Token(Cmd, PCClassStr_3, true);
//
//		if (!bPCClass_0 && !bPCClass_1 && !bPCClass_2 && !bPCClass_3)
//		{ 
//			// ������ ����Ī
//			g_IsEditorRaidGamePreview = !g_IsEditorRaidGamePreview;
//		}
//		else
//		{
//			g_IsEditorRaidGamePreview = true; // ������ �Ǿ��ٸ� ������ �Ѵ� ��
//
//			g_EditorRaidPreviewPCClassMain[0] = IntToPCClass(FMath::Clamp(FCString::Atoi(*PCClassStr_0), 0, GetMaxPCClassNum() - 1));
//			g_EditorRaidPreviewPCClassMain[1] = IntToPCClass(FMath::Clamp(FCString::Atoi(*PCClassStr_1), 0, GetMaxPCClassNum() - 1));
//			g_EditorRaidPreviewPCClassMain[2] = IntToPCClass(FMath::Clamp(FCString::Atoi(*PCClassStr_2), 0, GetMaxPCClassNum() - 1));
//			g_EditorRaidPreviewPCClassMain[3] = IntToPCClass(FMath::Clamp(FCString::Atoi(*PCClassStr_3), 0, GetMaxPCClassNum() - 1));
//		}
//
//#if !PLATFORM_MAC
//		FString MsgStr = FString::Printf(TEXT("[������ ����] ���̵� ������ : %s."), g_IsEditorRaidGamePreview ? TEXT("����") : TEXT("����"));
//		if (g_IsEditorRaidGamePreview)
//		{
//			MsgStr += FString::Printf(TEXT("\n\n���� �÷��̾� - ���� ĳ����: %d"), (int32)g_EditorRaidPreviewPCClassMain[0]);
//			MsgStr += FString::Printf(TEXT("\n\n���� �÷��̾� - 1�� ��Ƽ�� ĳ����: %d"), (int32)g_EditorRaidPreviewPCClassMain[1]);
//			MsgStr += FString::Printf(TEXT("\n\n���� �÷��̾� - 2�� ��Ƽ�� ĳ����: %d"), (int32)g_EditorRaidPreviewPCClassMain[2]);
//			MsgStr += FString::Printf(TEXT("\n\n���� �÷��̾� - 3�� ��Ƽ�� ĳ����: %d"), (int32)g_EditorRaidPreviewPCClassMain[3]);
//			MsgStr += TEXT("\n\n���̵� ������ ��� ������ ������� �Ҿ����� �� ������ �÷��� ���� �۾����� ������ �ּ���.");
//		}
//
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(MsgStr));
//#endif
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ControlRoom")))
//	{
//		FString RoomNameStr;
//		FParse::Token(Cmd, RoomNameStr, true);
//
//		NetFakeConfigure::GetInstance().SetControlRoomName(RoomNameStr);
//		
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("RaidRoom")))
//	{
//		FString RoomNameStr;
//		FParse::Token(Cmd, RoomNameStr, true);
//
//		NetFakeConfigure::GetInstance().SetRaidRoomName(RoomNameStr);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("HateRaidPenalty")))
//	{
//		FString OnOrOff;
//		FParse::Token(Cmd, OnOrOff, true);
//
//		bool bPenaltyAvailable = (OnOrOff == TEXT("On")) ? true : false;
//		NetFakeConfigure::GetInstance().SetHateRaidPenalty(bPenaltyAvailable);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ForcedUnitySkillOpen")))
//	{
//		FString OnOrOff;
//		FParse::Token(Cmd, OnOrOff, true);
//
//		bool bIsOpen = (OnOrOff == TEXT("On")) ? true : false;
//		NetFakeConfigure::GetInstance().SetForcedUnitySkillOpen(bIsOpen);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("DJHackFixParticleFreezeProblem")))
//	{
//		// ��� ���ΰ� �߻��� �� �� ���� ���������� ����.. ���� ��� ���� �Ŷ� �̷��� �ӽ� ��ó��..
//		for (TObjectIterator<UParticleSystemComponent> PSCIT; PSCIT; ++PSCIT)
//		{
//			UParticleSystemComponent* ThisComp = Cast<UParticleSystemComponent>(*PSCIT);
//			if (ThisComp)
//			{
//				ThisComp->SecondsBeforeInactive = 10.0f;
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("UIEquipFx")))
//	{ // ���� ���������� �׽�Ʈ Ŀ�ǵ���
//		LobbyPlayUIEquipEffectClass<EItemEquipPlace>::GetInstance().Signal(EItemEquipPlace::EIEP_Helmet);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("UMGRefTex")))
//	{
//		FString WidgetPath;
//		FParse::Token(Cmd, WidgetPath, true);
//
//		UWidgetBlueprint* FoundWigetBP = LoadObject<UWidgetBlueprint>(NULL, *WidgetPath);
//		if (FoundWigetBP)
//		{
//			TArray<UTexture*> RefTextures;
//			UWidgetBlueprintGeneratedClass::TryGetAllReferencedTextures(FoundWigetBP->WidgetTree, RefTextures);
//
//			if (RefTextures.Num() > 0)
//			{
//				struct TextureResourceSizeSorter
//				{
//					bool operator()(UTexture& A, UTexture& B) const
//					{
//						return A.GetResourceSizeBytes(EResourceSizeMode::Inclusive) > B.GetResourceSizeBytes(EResourceSizeMode::Inclusive);
//					}
//				};
//				RefTextures.Sort(TextureResourceSizeSorter());
//
//#if !PLATFORM_MAC
//				FString MsgStr = FString::Printf(TEXT("WidgetBlueprint %s ���� (�ּ�) %d���� �ؽ��İ� ���ǰ� �ֽ��ϴ�.\n\n"), *FoundWigetBP->GetName(), RefTextures.Num());
//
//				for (int32 TI = 0; TI < RefTextures.Num(); ++TI)
//				{
//					UTexture* ThisTexture = RefTextures[TI];
//					UTexture2D* ThisTexture2D = Cast<UTexture2D>(ThisTexture);
//					if (ThisTexture)
//					{
//						int32 ThisTextureSizeX = ThisTexture2D ? ThisTexture2D->GetSizeX() : (int32)ThisTexture->GetSurfaceWidth();
//						int32 ThisTextureSizeY = ThisTexture2D ? ThisTexture2D->GetSizeY() : (int32)ThisTexture->GetSurfaceHeight();
//						int32 ThisTextureResourceSizeByte = ThisTexture->GetResourceSizeBytes(EResourceSizeMode::Inclusive);
//
//						MsgStr += FString::Printf(TEXT("%d. %s, Resolution %dx%d, Mem: %d KB\n"),
//							TI + 1, *ThisTexture->GetName(),
//							ThisTextureSizeX, ThisTextureSizeY,
//							(int32)(((float)ThisTextureResourceSizeByte / 1024.0f) + 0.5f));
//					}
//				}
//
//				FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(MsgStr));
//#endif
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("RefreshDialogInfo")))
//	{ // �����Ϳ��� DialogInfo ���̺� �۾� �Ŀ� �������� ������ �ȵ�.. ��� �̷��Զ�..,
//		// ��, �� UEditorEngine::PlayInEditor ���� �̰� ȣ���ϰ�..
//		BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
//		if (B2GameModule)
//		{
//			B2GameModule->DestroyDialogInfoTable();
//			B2GameModule->GetDialogInfoTable();
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("DummyClearReward")))
//	{ // �Ϲ� ������ �÷��̿��� Ŭ���� ���� ������ ���� ���� ����
//		FString RewardNumStr;
//		FParse::Token(Cmd, RewardNumStr, true);
//		int32 RewardNum = FCString::Atoi(*RewardNumStr);
//		
//		RewardNum = FMath::Clamp(RewardNum, 1, 5);
//
//		bool bIsDoubled = (FMath::RandRange(0, 2) == 0);// ���ʽ� ��Ȳ ����
//
//		UB2UIDocBattleStage* DocBS = UB2UIDocHelper::GetDocBattleStage();
//		if (DocBS)
//		{
//			TArray<FB2Item> AllRewardItem;
//			TArray<FB2Item> AllMaterialRewardItem;
//			AllRewardItem.Empty();
//			AllMaterialRewardItem.Empty();
//
//			for (int32 RI = 0; RI < RewardNum; ++RI)
//			{
//				FB2Item ThisItem;
//
//				// �������� grade ��.. ��, �̰� RewardNum �� 5�������� �ǵ� �����δ� 6������ Ȯ���ؾ� �ؼ�.. RewardNum �� ���� �� ������
//				int32 ThisItemGrade = (RI % FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE); // �� ���� �״�� ���� 0 �� ���� �� �־ �ȵǰ� �ּ��� 1�� ���ؾ� �Ѵ�.
//				if (RewardNum == 3 || RewardNum == 5)
//				{
//					ThisItemGrade += 2; // RewardNum 5 �� ���� 2, 3, 4, 5, 6 �� ��.
//				}
//				else if (RewardNum == 4)
//				{
//					ThisItemGrade += 3; // 3, 4, 5, 6
//				}
//				else
//				{
//					ThisItemGrade += 1;
//				}
//				ThisItemGrade = FMath::Clamp(ThisItemGrade, 1, FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE);
//
//				// �� ���ܷ� ��嵵 �����ֵ��� �� ����..
//				if (RewardNum == 5 && ThisItemGrade == 2)
//				{
//					GenerateGoldItem(ThisItem, 10000);
//					AllRewardItem.Add(ThisItem);
//				}
//				// ���ܷ� ��� �����۵� �����ֵ��� �غ���?
//				else if (RewardNum == 5 && ThisItemGrade == 3)
//				{
//					int32 RandNum = FMath::RandRange(0, 17);
//					ThisItem.ItemRefID = 8000000 + RandNum;
//					ThisItem.InventoryType = EItemInvenType::EIIVT_Consumables;
//					ThisItem.ItemClass = EItemClass::EIC_End;
//					ThisItem.InstanceUID = 8252424525250235 + RI; // �̰� ������
//					AllMaterialRewardItem.Add(ThisItem);
//				}
//				else
//				{
//					ThisItem.ItemRefID = 1000000 + ThisItemGrade * 100;
//					ThisItem.InstanceUID = 5252424525250235 + RI; // �̰� ������
//
//					DevOnlyLoadItemClientTestData(ThisItem);
//					AllRewardItem.Add(ThisItem);
//				}
//
//				if (bIsDoubled && ThisItem.ItemRefID != FItemRefIDHelper::GetGoodsID_Gold())
//				{
//					ThisItem.InstanceUID += 972442;
//					AllRewardItem.Add(ThisItem); // ���ʽ�. �Ȱ��� RefID �� �ϳ� ��
//				}
//			}
//
//			DocBS->SetStageClearRewardItemList(AllRewardItem);
//			DocBS->SetStageClearMaterialItemsList(AllMaterialRewardItem);
//		}
//		
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("FirstTimeNPCSensitiveInfoConstruction")))
//	{
//#if !PLATFORM_MAC
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("FirstTimeNPCSensitiveInfoConstruction ����� ù ��� �� ���� ��Ȱ��ȭ��. ����� �ʿ��� ��� ���α׷������� ����.")));
//#endif
//		/*UB2NPCClassInfoBox* MobClassInfoBox = StaticFindMobClassInfoBox();
//		if (MobClassInfoBox)
//		{
//			MobClassInfoBox->FirstTimeNPCSensitiveInfoConstruction();
//		}*/
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ViewAllUIName")))
//	{
//		UB2UIManager* UIMgr = UB2UIManager::GetInstance();
//		if (UIMgr)
//		{
//#if !PLATFORM_MAC
//			FString MsgStr = TEXT("���� �̸���\n\n");
//
//			TArray<FName> AllCreatedUIs;
//			UIMgr->GetAllCreatedUINames(AllCreatedUIs);
//
//			for (FName& ThisUIName : AllCreatedUIs)
//			{
//				MsgStr += ThisUIName.ToString() + TEXT("\n");
//			}
//
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(MsgStr));
//#endif
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ChangeUIScene")))
//	{
//		FString uiStr;
//		FParse::Token(Cmd, uiStr, true);
//		int32 eUISceneValue = FCString::Atoi(*uiStr);
//		if (eUISceneValue >= (int32)EUIScene::Max)
//			return false;
//
//		EUIScene uiEnum = static_cast<EUIScene>(eUISceneValue);
//		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(uiEnum);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("OpenUI")))
//	{
//		FString uiNameStr;
//		FParse::Token(Cmd, uiNameStr, true);
//		UB2UIManager::GetInstance()->OpenUI(FName(*uiNameStr));
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CloseUI")))
//	{
//		FString uiNameStr;
//		FParse::Token(Cmd, uiNameStr, true);
//		UB2UIManager::GetInstance()->CloseUI(FName(*uiNameStr));
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ChangePCCharMIParam")))
//	{
//		// ĳ���� PC �� MIC ���� �Ķ���͸� �ϰ� �����ϴ� ������ Ŀ�ǵ�.
//		// ��� ������ ���� ��� 
//		// "ChangePCCharMIParam 1 RoughnessSkinScale 0.4" �ϻ��� �� MIC ���� RoughnessSkinScale �Ķ���͸� 0.4 �� ����
//		// "ChangePCCharMIParam Gl MetalicScale 0.3" ������ �� MIC ���� MetalicScale �Ķ���͸� 0.3 ���� ����.
//		// PC ������ 0 ~ 3 Ȥ�� ��ӵ� �� ����� �� �ִ�.
//
//		FString PCClassStr;
//		FParse::Token(Cmd, PCClassStr, true);
//		EPCClass SpecifiedPCClass = EPCClass::EPC_End;
//		if (PCClassStr == TEXT("0") || PCClassStr == TEXT("Gl") || PCClassStr == TEXT("Gladiator"))
//		{
//			SpecifiedPCClass = EPCClass::EPC_Gladiator;
//		}
//		else if (PCClassStr == TEXT("1") || PCClassStr == TEXT("As") || PCClassStr == TEXT("Assassin"))
//		{
//			SpecifiedPCClass = EPCClass::EPC_Assassin;
//		}
//		else if (PCClassStr == TEXT("2") || PCClassStr == TEXT("Wi") || PCClassStr == TEXT("Wizard"))
//		{
//			SpecifiedPCClass = EPCClass::EPC_Wizard;
//		}
//		else if (PCClassStr == TEXT("3") || PCClassStr == TEXT("Fi") || PCClassStr == TEXT("Fighter"))
//		{
//			SpecifiedPCClass = EPCClass::EPC_Fighter;
//		}
//
//		if (SpecifiedPCClass == EPCClass::EPC_End && (PCClassToInt(EPCClass::EPC_Fighter) + 1 != GetMaxPCClassNum()))
//		{ // ������ �� �ڵ带 § ����� ���� �� ��Ȳ���� �ʵ���.
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("ChangePCCharMIParam Ŀ�ǵ�� Fighter ������ �غ�Ǿ����ϴ�.\n���ο� ĳ���Ϳ� ���� �ڵ�� �߰��ؾ� �մϴ�.")));
//#endif
//		}
//
//		// �츮 ĳ���� ���� ����ϴ� ���ֿ̹� ���� ������ ���͸� �ϴ� ��.
//		FString FileNameFilter;
//		if (SpecifiedPCClass == EPCClass::EPC_Gladiator) {
//			FileNameFilter = TEXT("PC_Gl");
//		}
//		else if (SpecifiedPCClass == EPCClass::EPC_Assassin) {
//			FileNameFilter = TEXT("PC_As");
//		}
//		else if (SpecifiedPCClass == EPCClass::EPC_Wizard) {
//			FileNameFilter = TEXT("PC_Wi");
//		}
//		else if (SpecifiedPCClass == EPCClass::EPC_Fighter) {
//			FileNameFilter = TEXT("PC_Fi");
//		}
//
//		FString WantedParamNameStr;
//		FParse::Token(Cmd, WantedParamNameStr, true);
//		const FName WantedParamName(*WantedParamNameStr);
//
//		FString WantedParamValueStr;
//		FParse::Token(Cmd, WantedParamValueStr, true);
//		float WantedParamValue = FCString::Atof(*WantedParamValueStr);
//
//		if (SpecifiedPCClass != EPCClass::EPC_End && WantedParamNameStr.Len() > 0 && FileNameFilter.Len() > 0)
//		{			
//			TArray<UMaterialInstanceConstant*> AllFoundMICs;
//			EditorFindAndLoadAssets<UMaterialInstanceConstant>(TEXT("Character/PC"), FileNameFilter, AllFoundMICs);
//
//			TArray<FString> FoundAndProcessedFiles; // ��� �޽��� ��
//
//			for (UMaterialInstanceConstant* ThisMIC : AllFoundMICs)
//			{
//				if (ThisMIC)
//				{
//					// �Ķ���Ͱ� �ִ��� ���� üũ�ϴ� �� ��� �޽����� �ݿ��ϱ� ����.
//					float bCurrentParamValue = 0.0f;
//					const bool bParamExist = ThisMIC->GetScalarParameterValue(WantedParamName, bCurrentParamValue);
//
//					if (bParamExist)
//					{
//						ThisMIC->SetScalarParameterValueEditorOnly(WantedParamName, WantedParamValue);
//						ThisMIC->MarkPackageDirty();
//						FoundAndProcessedFiles.Add(ThisMIC->GetPathName());
//					}
//				}
//			}
//
//#if !PLATFORM_MAC
//			FString MessageStr;
//			if (FoundAndProcessedFiles.Num() > 0)
//			{
//				MessageStr = FString::Printf(TEXT("���� %d �� MIC ���� �Ķ���� %s ��(��) %.3f (��)�� ����Ǿ����ϴ�. Ȯ�� �� ������ �ּ���.\n\n"),
//					FoundAndProcessedFiles.Num(), *WantedParamNameStr, WantedParamValue);
//				for (int32 FI = 0; FI < FoundAndProcessedFiles.Num(); ++FI)
//				{
//					MessageStr += FString::Printf(TEXT("%d. %s\n\n"), FI + 1, *FoundAndProcessedFiles[FI]);
//				}
//			}
//			else
//			{
//				MessageStr = FString::Printf(TEXT("%s ���̹��� Character MIC ���� �Ķ���� %s ��(��) %.3f (��)�� �����ϰ��� �Ͽ����� �ϳ��� �������� ���߽��ϴ�. ���� ��ũ�����̽� ���� ����/��ΰ� ����Ǿ��ų� �������� �ʴ� �Ķ���͸� �����Ͽ��� ���ɼ��� �ֽ��ϴ�."),
//					*FileNameFilter, *WantedParamNameStr, WantedParamValue);
//			}
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(MessageStr));
//#endif
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CheckMatineeRef")) || FParse::Command(&Cmd, TEXT("B2MatineeRefCheck")))
//	{ 
//		// UB2MatineeRefCheckCommandlet �� ����ٰ�.. �̷� ����� Ŀ�ǵ鷿 ȯ�濡�� �Ұ����� �͵� ����
//		// ������ �����Ϳ��� ����ϴ� �� ���� ���� �־.. �̷����� ������ Ŀ�ǵ�� ����.
//		UB2MatineeRefCheckCommandlet::HandleAsEditorCommand(Cmd, Ar);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("B2MaterialParamCheck")))
//	{ // ������ �̸��� Ŀ�ǵ鷿���� ����� �� ����.
//		UB2MaterialParamCheckCommandlet::EditorCommandMain(Cmd, Ar);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("B2AssetOptCheck")))
//	{ // ������ �̸��� Ŀ�ǵ鷿���� ����� �� ����.
//		UB2AssetOptCheckCommandlet::EditorCommandMain(Cmd, Ar);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SummarizeMaterialStaticSwitchUse")))
//	{ // �ٷ� �Ʒ� ReplaceParentMaterialStaticSwitch �� �����ؼ� ����Ϸ��� ����.
//		FString ParentMaterialName;
//		FParse::Token(Cmd, ParentMaterialName, true);
//		UMaterial* LoadedMaterial = LoadObject<UMaterial>(NULL, *ParentMaterialName);
//
//		FString ChildMICSearchPath;
//		FParse::Token(Cmd, ChildMICSearchPath, true);
//
//		SummarizeMaterialStaticSwitchUse(LoadedMaterial, ChildMICSearchPath);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ReplaceParentMaterialStaticSwitch")))
//	{ // �ٷ� �� SummarizeMaterialStaticSwitchUse �� �����ؼ� ����Ϸ��� ����.
//		// Ư�� Material �� Parent �� �ϸ鼭 Ư�� StaticSwitch ���� ������ MIC ���� Parent �� �ٲ�ġ�� ���. �ڼ��� ������ �Ʒ� �Լ�����..
//
//		FString CurrParentMaterialName;
//		FParse::Token(Cmd, CurrParentMaterialName, true);
//		UMaterial* CurrParentMaterial = LoadObject<UMaterial>(NULL, *CurrParentMaterialName);
//
//		FString NewMaterialName;
//		FParse::Token(Cmd, NewMaterialName, true);
//		UMaterial* NewMaterial = LoadObject<UMaterial>(NULL, *NewMaterialName);
//
//		FString ChildMICSearchPath;
//		FParse::Token(Cmd, ChildMICSearchPath, true);
//
//		FString SSParamSetString;
//		// �̰ɷ� StaticSwitch ������ ��� �����ϴ����� �Ʒ� �� �Լ����� �Ľ�.
//		FParse::Value(Cmd, TEXT("SSParam="), SSParamSetString, false); // ShouldStopOnComma �� false ��. SSParam ������ Comma �� ���� ����.
//
//		ReplaceParentMaterialOfStaticSwitchParam(CurrParentMaterial, NewMaterial, ChildMICSearchPath, SSParamSetString);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ReplaceParentMaterialOfMIC"))) // �ܼ��� �θ� Material �ٲ�ġ��. ������� �ݼ��� ��ź��
//	{ 
//		FString CurrParentMaterialName;
//		FParse::Token(Cmd, CurrParentMaterialName, true);
//		UMaterial* CurrParentMaterial = LoadObject<UMaterial>(NULL, *CurrParentMaterialName);
//
//		FString NewMaterialName;
//		FParse::Token(Cmd, NewMaterialName, true);
//		UMaterial* NewMaterial = LoadObject<UMaterial>(NULL, *NewMaterialName);
//
//		if (CurrParentMaterial && NewMaterial)
//		{
//			FString ChildMICSearchPath;
//			FParse::Token(Cmd, ChildMICSearchPath, true);
//
//			TArray<FString> AllFoundAssets;
//			// ã�� ���� �� MIC �� �ʿ������� �����δ� �ε��� �� ���� �ȴ�..
//			EditorFindAssetFileList(AllFoundAssets, ChildMICSearchPath, TEXT(""), false);
//
//			TArray<UMaterialInstanceConstant*> ChangedMICs;
//			for (int32 AAI = 0; AAI < AllFoundAssets.Num(); ++AAI)
//			{
//				UMaterialInstanceConstant* LoadedMIC = LoadObject<UMaterialInstanceConstant>(NULL, *(AllFoundAssets[AAI]));
//				if (!LoadedMIC || LoadedMIC->Parent != CurrParentMaterial) {
//					continue;
//				}
//				LoadedMIC->SetParentEditorOnly(NewMaterial);
//				LoadedMIC->MarkPackageDirty();
//				ChangedMICs.Add(LoadedMIC);
//			}
//			FString ResultLog = FString::Printf(TEXT("ReplaceParentMaterialOfMIC ReplacedMaterial %s, NewMaterial %s, %d MICs are changed.\r\n"), *(CurrParentMaterial->GetName()), *(NewMaterial->GetName()), ChangedMICs.Num());
//			for (int32 CMICI = 0; CMICI < ChangedMICs.Num(); ++CMICI)
//			{
//				if (ChangedMICs[CMICI]) {
//					ResultLog += FString::Printf(TEXT("%d. %s\r\n"), CMICI + 1, *(ChangedMICs[CMICI]->GetPathName()));
//				}
//			}
//			UE_LOG(LogBladeIIEditor, Log, TEXT("%s"), *ResultLog);
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TogglePowerToSave")))
//	{
//		EditorFakeConfigure::GetInstance().GivePowerToSave();
//		UE_LOG(LogBladeII, Log, TEXT("Power To Save GameMode in WorldSetting : %s"), EditorFakeConfigure::GetInstance().HavePowerToSave() ? TEXT("ON") : TEXT("OFF"));
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SPPreviewSize")))
//	{
//		FString NewSizeStr;
//		FParse::Token(Cmd, NewSizeStr, true);
//
//		AB2SpawnPoolEditingActor::MeshSizeLimit = FMath::Clamp(FCString::Atof(*NewSizeStr), 10.0f, 100000.0f);
//
//		for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//		{
//			AB2MonsterSpawnPool* CastedSP = Cast<AB2MonsterSpawnPool>(*ActorIt);
//			if (CastedSP)
//			{
//				CastedSP->RegenerateEditingActors();
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("Replace6StarPCDummy")))
//	{
//		// ����� SkeletalMeshActor �¾��ϴµ� �⺻ ���� �ƴ� 6�� ������ �� ���� �� �ʹ� ���� ���� ������ �ϰ�ó���� �ϰ�..
//
//		UB2PCClassInfoBox* PCInfoBox = StaticFindPCClassInfoBox();
//		UB2PCClassInfo* Info_Gladiator = PCInfoBox->GetSingleClassInfo(EPCClass::EPC_Gladiator);
//		UB2PCClassInfo* Info_Assassin = PCInfoBox->GetSingleClassInfo(EPCClass::EPC_Assassin);
//		UB2PCClassInfo* Info_Wizard = PCInfoBox->GetSingleClassInfo(EPCClass::EPC_Wizard);
//		UB2PCClassInfo* Info_Fighter = PCInfoBox->GetSingleClassInfo(EPCClass::EPC_Fighter);
//		check(Info_Gladiator && Info_Fighter->BaseMeshAsset && Info_Assassin && Info_Assassin->BaseMeshAsset
//			&& Info_Wizard && Info_Wizard->BaseMeshAsset && Info_Fighter && Info_Fighter->BaseMeshAsset);
//		DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
//		
//		FString ChangedMsgStr_Gladiator;
//		FString ChangedMsgStr_Assassin;
//		FString ChangedMsgStr_Wizard;
//		FString ChangedMsgStr_Fighter;
//		int32 ChangedNum_Gladiator = 0;
//		int32 ChangedNum_Assassin = 0;
//		int32 ChangedNum_Wizard = 0;
//		int32 ChangedNum_Fighter = 0;
//		for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//		{
//			ASkeletalMeshActor* CastedSKA = Cast<ASkeletalMeshActor>(*ActorIt);
//			if (CastedSKA && CastedSKA->GetSkeletalMeshComponent())
//			{
//				USkeletalMesh* TheMeshResource = CastedSKA->GetSkeletalMeshComponent()->SkeletalMesh;
//
//				if (TheMeshResource)
//				{ 
//					if (TheMeshResource->GetName() == TEXT("Pc_Gladiator_Set06"))
//					{
//						CastedSKA->GetSkeletalMeshComponent()->SetSkeletalMesh(Info_Gladiator->BaseMeshAsset);
//						++ChangedNum_Gladiator;
//						ChangedMsgStr_Gladiator += FString::Printf(TEXT("%d.Actor %s\r\n"), ChangedNum_Gladiator, *CastedSKA->GetName());
//					}
//					else if (TheMeshResource->GetName() == TEXT("CH_PC_Assassin_Set06"))
//					{
//						CastedSKA->GetSkeletalMeshComponent()->SetSkeletalMesh(Info_Assassin->BaseMeshAsset);
//						++ChangedNum_Assassin;
//						ChangedMsgStr_Assassin += FString::Printf(TEXT("%d.Actor %s\r\n"), ChangedNum_Assassin, *CastedSKA->GetName());
//					}
//					else if (TheMeshResource->GetName() == TEXT("PC_Wizard_Set06"))
//					{
//						CastedSKA->GetSkeletalMeshComponent()->SetSkeletalMesh(Info_Wizard->BaseMeshAsset);
//						++ChangedNum_Wizard;
//						ChangedMsgStr_Wizard += FString::Printf(TEXT("%d.Actor %s\r\n"), ChangedNum_Wizard, *CastedSKA->GetName());
//					}
//					else if (TheMeshResource->GetName() == TEXT("CH_PC_Fighter_Set06"))
//					{
//						CastedSKA->GetSkeletalMeshComponent()->SetSkeletalMesh(Info_Fighter->BaseMeshAsset);
//						++ChangedNum_Fighter;
//						ChangedMsgStr_Fighter += FString::Printf(TEXT("%d.Actor %s\r\n"), ChangedNum_Fighter, *CastedSKA->GetName());
//					}
//				}
//			}
//		}
//		if (ChangedNum_Gladiator > 0 || ChangedNum_Assassin > 0 || ChangedNum_Wizard > 0 || ChangedNum_Fighter > 0)
//		{
//			FString FinalMsgString = FString::Printf(TEXT("SkeletalMesh of total %d Actors are changed.\r\n\r\n")
//				TEXT("[Gladiator]\r\n%s\r\n\r\n")
//				TEXT("[Assassin]\r\n%s\r\n\r\n")
//				TEXT("[Wizard]\r\n%s\r\n\r\n")
//				TEXT("[Fighter]\r\n%s\r\n\r\n"),
//				ChangedNum_Gladiator + ChangedNum_Assassin + ChangedNum_Wizard + ChangedNum_Fighter,
//				*ChangedMsgStr_Gladiator, *ChangedMsgStr_Assassin, *ChangedMsgStr_Wizard, *ChangedMsgStr_Fighter
//				);
//
//			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(*FinalMsgString));
//			Inworld->MarkPackageDirty();
//		}
//		else
//		{
//#if !PLATFORM_MAC
//			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("�ٲ� ���� �����ϴ�.")));
//#endif
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("AddCostumeStoreSlot")))
//	{
//		FString AddCountStr;
//		FParse::Token(Cmd, AddCountStr, true);
//
//		int32 AddCount = FCString::Atoi(*AddCountStr);
//
//		for (TObjectIterator<UB2UICostumePageStore> PageIter; PageIter; ++PageIter)
//		{
//			UB2UICostumePageStore* PageStore = Cast<UB2UICostumePageStore>(*PageIter);
//			if (PageStore != nullptr)
//				PageStore->AddDevSlot(AddCount);
//		}
//		return true;
//	}

	return false;
}
#endif

#if !UE_BUILD_SHIPPING
bool gbHACKInGameUIHiddenByDevCommand = false; // ���߿� �����Ǵ� �ֵ鵵 ������ �Ǿ�� �ؼ�.. �Ф�
bool GCheat_CompleteIgnoreEventScene = false; // �̰��� ����ϴ� ���� �ƿ� ���� �������� ���� ������ ��� ���� ����� �÷��̰� �Ұ����� ���� ����. Ư���� ��Ȳ�� �׽�Ʈ��.
int32 GCheat_SkipEventSceneCount = -1; // �̰� ���� ������ ������ ���ؿ��� ���� ����� ��ŵ. 0 ���� ũ�� �� ���ڸ�ŭ�� ��������� ��ŵ�Ѵ�.
bool GCheat_HeroineMode = false; // Hero ���� ����ѵ� ���� ���� �� �ϳ��� ���̸� �������� �� ����.
float GCheat_StageDifficultyOverride = 0.0f; // 0.0 ������ �⺻��
bool GCheat_PCPartMergeRemoveExtraBoneInfluence = false;
int32 GCheat_RepeatBattleCountOverride = 0;
bool GTest_GenerateDummyChatMessage = false;
bool GTest_UseNonCompositedRawPCMesh = false; // PC ��� �ý��ۿ� ���� ���յ��� ���� �⺻ �޽� ���.
bool gIsDevUI = false;
bool gUseHeroTowerResetNoLoading = true; // ������ ž �ε� ���� ������ϴ� ����� ���� ���� ���׸� ����ų ������ �����Ƿ� ���� �߻��� �׽�Ʈ�� ���� ����.
bool gDrawPerfMark = false; // ScopedCallTracker ���� Ư�� �ð� �̻� �ҿ�� �� ȭ�鿡 ǥ��
bool gPreloadCombatEffectResources = true; // ���� �׽�Ʈ ��. ��� �������� spawn �ϴ� TAsset ���� �� ����Ʈ ���� ���ҽ����� �̸� �ε� �� ĳ���� ���� ���� ���� hitch �� �󸶳� �����Ǵ��� ���� ����.
bool gDisableInGameFloatingWidgets = false; // �������� �ݷ��ϰ� �����ϴ� DamageNum, AbnormalState, GoldAcq �� ����. ���� �׽�Ʈ��.
bool gDisableCombatEventUI = false; // ������ �̺�Ʈ ������ �����ϴ� Floating ���� �͵�
int32 gPCDataPreLoadMaxNum = -1; // �ε��ð� üũ��. ��ǻ� ĳ���� 4���������� 4�̻��̱⸸ �ϸ� �����۵�. 0, 1 ���� ���ڷ� �׽�Ʈ�ϱ� ����.
bool gNPCDataNoPreLoadMode = false;
bool gUseCharacterDefaultMaterial = false; // ���� ���� ��
int32 gTestAttachLightCompMode = 0; // �÷��̾� ĳ���� ����Ʈ attach mode
bool gbTestTurnOffPlayerLight = false;

 //ItemInfo ��� �񵿱� �ε� �Ϸ� �� �÷��̾� ĳ�� dummy spawn ���� �׽�Ʈ �ϱ� ����.
struct FItemInfoAsyncLoadTest
{
	FItemInfoAsyncLoadTest()
	{
		CachedRequestedClass = EPCClass::EPC_End;
		CachedRequestedGrade = 1;
	}

	EPCClass CachedRequestedClass;
	int32 CachedRequestedGrade;
	FString CachedRequestName;

	 //�׽�Ʈ Ŀ�ǵ忡�� ����Ϸ��� ����. ����� ����.

	void MarkAsyncLoadWaiting(EPCClass InRequestedClass, int32 InRequestedPartGrade, const FString& InRequestedName)
	{
		CachedRequestedClass = InRequestedClass;
		CachedRequestedGrade = InRequestedPartGrade;
		CachedRequestName = InRequestedName;

		UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
		if (ItemInfoTable)
		{
			ItemInfoTable->OnEquipPartsAyncLoadComplete.AddRaw(this, &FItemInfoAsyncLoadTest::OnAsyncLoadRequestComplete);
		}
	}
	void OnAsyncLoadRequestComplete(const FString& InRequestName)
	{
		if (InRequestName == CachedRequestName)
		{
			// ��û�� ���� ID �� ������ DummyPlayer spawn �� ���� �Ϸ��� �ص� ABladeIITestDummyPlayer �� spawn ��ɿ� ������ StandardEquipment ������ �ϴ� �͸� ����Ǿ� ����.

			if (GEngine)
			{ // �ε� �Ϸ�� ��� ������ spawn �׽�Ʈ. ���� �̷��� PCClassInfo �ε��� ���� �ɸ� �� �ִ�.
				GEngine->DeferredCommands.Add(FString::Printf(TEXT("SpawnDummyPlayer %d %d"), PCClassToInt(CachedRequestedClass), CachedRequestedGrade));
			}

			BII_SCREEN_LOG(FString::Printf(TEXT("[FItemInfoAsyncLoadTest::OnAsyncLoadRequestComplete] RequestName %s"), *CachedRequestName),
				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 12, 5.0f);

			UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
			if (ItemInfoTable)
			{
				ItemInfoTable->OnEquipPartsAyncLoadComplete.RemoveAll(this);
			}
		}
	}
};
static FItemInfoAsyncLoadTest ItemInfoAsyncLoadTestObj; // �׽�Ʈ�� ������� ��ü �ϳ�.

bool UB2StaticExecHelper::ExecDev(UWorld* Inworld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	 //����� �׽�Ʈ ���������� ����� Ŀ�ǵ带 ���⿡. �����ǿ����� ���������� ����������.
//
//	if (FParse::Command(&Cmd, TEXT("InGameUI")))
//	{
//		if (FParse::Command(&Cmd, TEXT("Off")))
//		{
//			gbHACKInGameUIHiddenByDevCommand = true;
//			for (TObjectIterator<UB2UIManager_InGameHUDChar> WMIT; WMIT; ++WMIT)
//			{
//				(*WMIT)->HideAll();
//			}
//		}
//		else if (FParse::Command(&Cmd, TEXT("On")))
//		{
//			gbHACKInGameUIHiddenByDevCommand = false;
//			for (TObjectIterator<UB2UIManager_InGameHUDChar> WMIT; WMIT; ++WMIT)
//			{
//				(*WMIT)->RestoreVisibilityAll();
//			}
//		}
//		UB2UIManager::GetInstance()->HideBattleRelatedWidgets(gbHACKInGameUIHiddenByDevCommand); // �� UI
//
//		return true;
//	}
//	if (FParse::Command(&Cmd, TEXT("ShowUI"))) // "InGameUI" �� ������ ���� �Ű�.. ������ �� �̷��� �ϸ� �� ������ ��..
//	{
//		gbHACKInGameUIHiddenByDevCommand = !gbHACKInGameUIHiddenByDevCommand;
//
//		for (TObjectIterator<UB2UnitedWidgetBase> WMIT; WMIT; ++WMIT)
//		{
//			if (gbHACKInGameUIHiddenByDevCommand)
//			{
//				(*WMIT)->ForceHide();
//			}
//			else
//			{
//				(*WMIT)->RestoreFromForceHidden();
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("Heroine")))
//	{
//		GCheat_HeroineMode = !GCheat_HeroineMode;
//		UE_LOG(LogBladeII, Log, TEXT("Heroine mode : killing one monster kills all monsters %s"), GCheat_HeroineMode ? TEXT("ON") : TEXT("OFF"));
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("Tag")))
//	{
//		bool bIsMainClass = false;
//		EPCClass TargetClass = EPCClass::EPC_End;
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		if (B2Player)
//		{
//			if (FParse::Command(&Cmd, TEXT("1")))
//			{
//				TargetClass = EPCClass::EPC_Gladiator;
//			}
//			else if (FParse::Command(&Cmd, TEXT("2")))
//			{
//				TargetClass = EPCClass::EPC_Assassin;
//			}
//			else if (FParse::Command(&Cmd, TEXT("3")))
//			{
//				TargetClass = EPCClass::EPC_Fighter;
//			}
//			else if (FParse::Command(&Cmd, TEXT("4")))
//			{
//				TargetClass = EPCClass::EPC_Wizard;
//			}
//
//			DEV_CHECK_FOR_POSSIBLE_NEW_PCCLASS();
//
//			UB2SkillRTManager* SkillRTManager = B2Player ? B2Player->GetSkillRTManager() : NULL;
//			if (SkillRTManager)
//			{
//				SkillRTManager->OverrideTagClass(TargetClass);
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("KillMyself")) || FParse::Command(&Cmd, TEXT("KillMe")))
//	{
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		if (B2Player){
//			B2Player->Suicide();
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GuildResetSkill")))
//	{
//		AB2GuildGameMode* B2GM = Cast<AB2GuildGameMode>(UGameplayStatics::GetGameMode(Inworld));
//
//		if (B2GM)
//			B2GM->ResetSkillCoolTime(ETeamIndex::Player);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SetGuildReaminTime")))
//	{
//		FString RemainTimeStr;
//		FParse::Token(Cmd, RemainTimeStr, true);
//
//		const float RemainTime = FCString::Atof(*RemainTimeStr);
//
//		AB2GuildGameMode* B2GM = Cast<AB2GuildGameMode>(UGameplayStatics::GetGameMode(Inworld));
//
//		if (B2GM)
//			B2GM->SetRemainBattleTime(RemainTime);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("KillAllAlly")))
//	{
//		AB2GuildGameMode* B2GM = Cast<AB2GuildGameMode>(UGameplayStatics::GetGameMode(Inworld));
//
//		if (B2GM)
//			B2GM->KillAllAlly();
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("NPCHPScale")))
//	{
//		FString ScaleString;
//		FParse::Token(Cmd, ScaleString, true);
//
//		const float HPScale = FCString::Atof(*ScaleString);
//
//		for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//		{
//			ABladeIICharacter* B2Char = Cast<ABladeIICharacter>(*ActorIt);
//			ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(*ActorIt);
//
//			if (B2Char != NULL && B2Player == NULL)
//			{
//				B2Char->Health *= HPScale;
//				B2Char->MaxHealth *= HPScale;
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PVPLocalHPSet")))
//	{
//		FString CurHPString, MaxHPString;
//		FParse::Token(Cmd, CurHPString, true);
//
//		const int32 CurHP= FCString::Atoi(*CurHPString);
//		const int32 MaxHP = FCString::Atoi(*MaxHPString);
//
//		for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//		{
//			ABladeIINetPlayer* B2Player = Cast<ABladeIINetPlayer>(*ActorIt);
//
//			if (B2Player != NULL)
//			{
//				B2Player->Health = CurHP;
//				B2Player->MaxHealth = MaxHP;
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PVPRivalHPSet")))
//	{
//		FString CurHPString, MaxHPString;
//		FParse::Token(Cmd, CurHPString, true);
//
//		const int32 CurHP = FCString::Atoi(*CurHPString);
//		const int32 MaxHP = FCString::Atoi(*MaxHPString);
//
//		for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//		{
//			auto* B2Player = Cast<ABladeIINetPlayer>(*ActorIt);
//
//			if (B2Player && B2Player->GetNetStatus() == ABladeIINetPlayer::NET_SLAVE)
//			{
//				B2Player->Health = CurHP;
//				B2Player->MaxHealth = MaxHP;
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SpawnDropItem")))
//	{
//		FString ItemIDString;
//		FParse::Token(Cmd, ItemIDString, true);
//
//		FString NumItemString;
//		FParse::Token(Cmd, NumItemString, true);
//
//		
//		int ItemRefID = FCString::Atoi(*ItemIDString);
//		if (ItemIDString == TEXT("Gold")) // Ư�� ���̽�.
//		{
//			ItemRefID = FItemRefIDHelper::GetGoodsID_Gold();			
//		}
//
//		bool bIsGold = ItemRefID == FItemRefIDHelper::GetGoodsID_Gold();
//
//		int NumItem = 1;
//		if (!NumItemString.IsEmpty())
//			NumItem = FCString::Atoi(*NumItemString);
//
//		auto* B2GM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		auto* StageManager = B2GM ? B2GM->GetStageManager() : NULL;
//
//		if (StageManager)
//		{
//			FDropItemInfo TempDropItemInfo;
//			TempDropItemInfo.RewardGoldWeight = 0;
//			TempDropItemInfo.bCanDropItem = 0;
//			TempDropItemInfo.TestNumSpawnGold = bIsGold ? NumItem : 0;
//			TempDropItemInfo.TestNumSpawnItemRefID = bIsGold ? 0 : ItemRefID;
//
//			if (bIsGold)
//				NumItem = 1;
//
//			for (int32 Idx = 0; Idx < NumItem; ++Idx)
//			{
//				ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//				if (B2Player)
//					StageManager->RequestSpawnDropItem(TempDropItemInfo, NULL, B2Player->GetActorLocation() + FVector(100.0f, 100.0f, 0.0f));
//			}
//		}
//		
//		return true;
//	}	
//	else if (FParse::Command(&Cmd, TEXT("SpawnDamageActor")))
//	{
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//
//		FString RadiusString;
//		FParse::Token(Cmd, RadiusString, true);
//		const float Radius = FCString::Atof(*RadiusString);
//
//		FString DelayString;
//		FParse::Token(Cmd, DelayString, true);
//		const float Delay = FCString::Atof(*DelayString);
//
//		FString IntervalString;
//		FParse::Token(Cmd, IntervalString, true);
//		const float Interval = FCString::Atof(*IntervalString);
//
//		FString LifeString;
//		FParse::Token(Cmd, LifeString, true);
//		const float Life = FCString::Atof(*LifeString);		
//
//		if (B2Player)
//		{
//			B2Player->CastAreaDamage_DamageZoneSkillIndex(100, FDamageInfo(), false, B2Player->GetActorLocation(), 0, Radius, Delay, Interval, Life);
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestTypeList")))
//	{
//		/*
////		MAKE_TYPELIST(Test, data_trader::CreateAccountMessage, data_trader::CreateAccountMessage, data_trader::CreateAccountMessage)
//		MAKE_TYPELIST(Test, data_trader::CreateAccountMessage, data_trader::AddItemMessage, data_trader::CreateAccountMessage) 
//
//		static_assert(TU::IsSameType<data_trader::AddItemMessage, TU::TL::TypeAt<TU::TL::Reverse<Test>::Result, 2>::Result>::value, "two types aren't same.");
//		static_assert(3 == TU::TL::Length<TU::TL::Reverse<Test>::Result>::value, "Length must be 3.");
//
//		static_assert(2 == TU::TL::Length<Test::Tail>::value, "Must be 2.");
////		static_assert(TU::IsSameType<data_trader::CreateAccountMessage, TU::TL::TypeAt<Test::Tail, 1>::Result>::value, "two types aren't same.");
//
//		using Test4 = TU::TL::Append<data_trader::AddItemMessage, TU::TypeList<>>::Result;
//		static_assert(1 == TU::TL::Length<Test4>::value, "Must have the same number of elements.");
//		static_assert(TU::IsSameType<data_trader::AddItemMessage, TU::TL::TypeAt<Test4, 0>::Result>::value, "two types aren't same.");
//		
////		using Test1 = TU::TL::Erase<data_trader::CreateAccountMessage, Test>::Result;
////		static_assert(2 == TU::TL::Length<Test1>::value, "have same elements.");
////		using Test2 = TU::TL::Erase<data_trader::CreateAccountMessage, Test1>::Result;
////		static_assert(1 == TU::TL::Length<Test2>::value, "have same elements.");
////		using Test3 = TU::TL::Erase<data_trader::CreateAccountMessage, Test2>::Result;
////		static_assert(0 == TU::TL::Length<Test3>::value, "have same elements.");
//		
//		static_assert(1 == TU::TL::Length<TU::TL::EraseAll<data_trader::CreateAccountMessage, Test>::Result>::value, "have only one type.");
//		*/
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("RequestDummyItems")) || FParse::Command(&Cmd, TEXT("RequestTestItems")))
//	{ // �κ񿡼� �κ��丮�� �ʱ�ȭ�� ���¿��� ����ؾ� ����.
//		FString RefIDString;
//		FParse::Token(Cmd, RefIDString, true);
//		int32 ParsedRefID = FCString::Atoi(*RefIDString);
//
//		FString CreateNumString;
//		FParse::Token(Cmd, CreateNumString, true);
//		int32 CreateNum = FCString::Atoi(*CreateNumString);
//		if (CreateNum <= 0)
//		{
//			CreateNum = 30; // �⺻��
//		}
//		if (ParsedRefID < 2000000)
//		{ // �Ϲ����� ���� �������̶�� �Ǽ��� �ʹ� ���� ��û���� �ʰ�..
//			CreateNum = FMath::Min(CreateNum, 100);
//		}
//
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
//		if (LobbyInven)
//		{ 
//			if (ParsedRefID > 0)
//			{
//				LobbyInven->DevOnlyRequestDummyItems(CreateNum, ParsedRefID, 0.1f);
//			}
//			else
//			{// �̰� ÷�� ������� ����ε� �ᱹ ���������� DevOnlyRequestDummyItems �� ����� ��. RefID �� �ȿ� �ϵ��ڵ�.
//				LobbyInven->DevOnlyGenerateDummyItemData(true);
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GenerateClientDummyItems")))
//	{ // �κ񿡼� �κ��丮�� �ʱ�ȭ�� ���¿��� ����ؾ� ����.
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
//		if (LobbyInven)
//		{ // �� ���, ������ ��û�ϴ� ��� Ŭ���̾�Ʈ���� ä���.
//			LobbyInven->DevOnlyGenerateDummyItemData(false);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GenerateClientDummyConsumable")))
//	{
//		FString ConsumableIDStr;
//		FParse::Token(Cmd, ConsumableIDStr, true);
//		int32 ConsumableID = FCString::Atoi(*ConsumableIDStr); // ID �� ������ �� �˾Ƽ� �־�� �� ��.
//
//		FString ConsumableAmountStr;
//		FParse::Token(Cmd, ConsumableAmountStr, true);
//		int32 ConsumableAmount = FCString::Atoi(*ConsumableAmountStr);
//
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
//		if (LobbyInven)
//		{
//			LobbyInven->DevOnlyGenerateDummyConsumable(ConsumableID, ConsumableAmount);
//		}
//	}
//	else if (FParse::Command(&Cmd, TEXT("GenerateClientDummyWings")))
//	{
//		// PCClass �����ϰ� �Ϸ��� �ߴµ� ���� �򰥸��⸸ �� ��. �� ��� ���� ��������.
//		//FString PCClassStr;
//		//FParse::Token(Cmd, PCClassStr, true);
//		//EPCClass ParsedPCClass = IntToPCClass(FCString::Atoi(*PCClassStr));
//
//		FString EvolveGradeStr;
//		FParse::Token(Cmd, EvolveGradeStr, true);
//		int32 ParsedEvolveGrade = FCString::Atoi(*EvolveGradeStr);
//
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		UB2Airport* LobbyAP = LobbyGM ? LobbyGM->GetLobbyAirport() : NULL;
//
//		if (LobbyAP)
//		{
//			LobbyAP->DevOnlyGenerateDummyWingData(ParsedEvolveGrade, false);
//		}
//
//		LobbyUpdateWingMainUIClass<>::GetInstance().Signal(); // Wing UI ������ ������Ʈ.
//
//		// �κ� �޽� ������Ʈ�� ����
//		for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//		{
//			LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(PCI));
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("WingEvolveFailTest")))
//	{
//		AB2LobbyGameMode* B2LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		UB2UIManager_Lobby* DJLegacyLobbyUIMgr = (B2LobbyGM && B2LobbyGM->IsValidObj()) ? B2LobbyGM->DJLegacy_GetLobbyUIManager() : NULL;
//		if (DJLegacyLobbyUIMgr)
//		{
//			DJLegacyLobbyUIMgr->BeginShowWingEvolutionFailMessage();
//		}
//		return true;
//	}
//	/*else if (FParse::Command(&Cmd, TEXT("CheckEquippedStat")))
//	{ // ���� �κ� ���� ĳ������ �������� ���� �����Ǵ� �ɷ�ġ Ȯ��
//		EPCClass LobbyPCClass = BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass();
//
//		TMap<EItemPrimaryPointType, float> AllPrimPoints;
//		UB2LobbyInventory::GetAllEquippedPrimPointsLocalPlayer(AllPrimPoints, LobbyPCClass);
//		for (auto& ThisElem : AllPrimPoints)
//		{
//			UE_LOG(LogBladeII, Log, TEXT("PrimPointType %d, amount %.2f"), (int32)ThisElem.Key, ThisElem.Value);
//		}
//
//		TMap<EItemOption, float> AllOptions;
//		UB2LobbyInventory::GetAllEquippedOptionPointsLocalPlayer(AllOptions, LobbyPCClass); // ���������� ����� ���� ���;� ��.
//		for (auto& ThisOption : AllOptions)
//		{ // Item Option �� int �δ� ������������ ���� ��.
//			UE_LOG(LogBladeII, Log, TEXT("OptionType %d, amount %.4f"), (int32)ThisOption.Key, ThisOption.Value);
//		}
//		return true;
//	}*/
//	else if (FParse::Command(&Cmd, TEXT("SPJump")))
//	{
//		// ���� �ʿ��� �ε��� SpawnPool ���� Ư�� Wave ���� ���� ���� �� ����.
//
//		FString JumpFinishWaveNumStr;
//		FParse::Token(Cmd, JumpFinishWaveNumStr, true);
//
//		int32 JumpFinishWaveNum = FCString::Atoi(*JumpFinishWaveNumStr);
//
//		if (JumpFinishWaveNum >= 0)
//		{
//			ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(Inworld));
//			AB2MonsterSpawnPool* CurrentSpawnPool = B2GM ? B2GM->GetActiveSpawnPool() : NULL;
//			if (CurrentSpawnPool)
//			{
//				CurrentSpawnPool->Cheat_FinishUpToCertainWave(JumpFinishWaveNum);
//			}
//
//			for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//			{
//				AB2StageEventDirector* CurrSED = Cast<AB2StageEventDirector>(*ActorIt);
//				if (CurrSED && CurrSED->GetDirectingEvent() != EStageEvent::ESEV_Clear)
//				{
//					CurrSED->CustomFinishShow(); // �� ������ ��ġ�鼭 ������ ������� ������ �� ���� ��.
//				}
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SpawnGuildNPC")))
//	{
//		// �Ķ���� ����
//		FString IDString;
//		FParse::Token(Cmd, IDString, true);
//		const int32 MercenaryID = FCString::Atoi(*IDString);
//
//		FString LVString;
//		FParse::Token(Cmd, LVString, true);
//		const int32 MercenaryLevel = FCString::Atoi(*LVString);
//
//		FTransform SpawnTransform;
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		if (B2Player) // �÷��̾�� �ణ �񲸳� ��ġ��
//		{
//			SpawnTransform = B2Player->GetTransform();
//			SpawnTransform.AddToTranslation(FVector(100.0f, 0.0f, 100.0f));
//		}
//
//		FActorSpawnParameters ControllerSpawnInfo;
//		ControllerSpawnInfo.Instigator = NULL;
//		ControllerSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//		ControllerSpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save player controllers into a map
//		ControllerSpawnInfo.bDeferConstruction = true;
//
//		FActorSpawnParameters PawnSpawnInfo;
//		PawnSpawnInfo.bDeferPostSpawnInitialize = true;
//
//		UB2GuildNPCInfo* GuildNPCInfo = StaticFindGuildNPCInfo();
//		UB2NPCClassInfoBox* NPCInfoBox = StaticFindMobClassInfoBox();
//		if (NPCInfoBox && GuildNPCInfo)
//		{
//			const FGuildNPCInfo* NPCInfo = GuildNPCInfo->GetGuildNPCInfoOfID(MercenaryID);
//			if (NPCInfo)
//			{
//				//skillState
//				TSharedPtr<FNPCUsingSkillState> SkillState = MakeShared<FNPCUsingSkillState>();
//				SkillState->bUsingSkill = false;
//
//				//CombatParam
//				FCombatStatParam StatParam;
//				StatParam.CharacterDataStore = nullptr;
//				StatParam.ReduceDamageBonusByFormation = 1.0f;
//				StatParam.AttackBonusByFormation = 1.0f;
//				StatParam.ModeDamageRate = 1.0f;
//				StatParam.ModeHealthRate = 2.0f;
//				StatParam.SpawnPCClass = EPCClass::EPC_End;
//
//				//MercenaryInfo
//				FGuildMercenaryInfo MercenaryInfo(MercenaryID, MercenaryLevel, 1, 1, 1);
//
//				AB2TestGuildNPCAIController* NewAC = Inworld->SpawnActor<AB2TestGuildNPCAIController>(AB2TestGuildNPCAIController::StaticClass(), SpawnTransform, ControllerSpawnInfo);
//				UGameplayStatics::FinishSpawningActor(NewAC, SpawnTransform);
//
//				ABladeIIMercenaryPlayer* NPCCharacter = Cast<ABladeIIMercenaryPlayer>(NPCInfoBox->SpawnNPCClassInfoGuildCharacter(Inworld, NPCInfo->NPCClass, ENPCClassVariation::ENCV_GuildBattle, SpawnTransform, ControllerSpawnInfo));
//
//				NPCCharacter->SetMercenaryInfo(MercenaryInfo);
//				NPCCharacter->SetMercenaryUsingState(SkillState);
//
//				NPCCharacter->InitCombateStatsForParam(StatParam);
//				ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(Inworld));
//				NPCCharacter->InjectGameMode(B2GM);
//				NPCCharacter->AutoPossessAI = EAutoPossessAI::Disabled;
//				NPCCharacter->FinishSpawning(SpawnTransform);
//
//				Inworld->ManualPostSpawnInitialize(NPCCharacter, SpawnTransform, PawnSpawnInfo);
//
//				NewAC->SetTeamNum((int8)ETeamType::ETT_Mob);
//				NewAC->Possess(NPCCharacter);
//				NewAC->BeginAutoPlay();
//			}
//		}
//
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SpawnNPC")))
//	{
//		// NPCClassInfo �� ��ϵ� NPC spawn. MonsterSpawnPool ��ġ ���� �Ｎ���� Ŀ�ǵ�� spawn ��Ű�� ���� ��
//		// "SpawnNPC ClassName VariationName" ClassName �� VariationName �� ���� ENPCClass �� ENPCClassVariation �׸񿡼� prefix �� ��
//
//		// �Ķ���� ����
//		FString ClassStr;
//		FParse::Token(Cmd, ClassStr, true);
//		ENPCClass WantedClass = UB2NPCSingleClassInfo::FindNPCClassEnumFromTrimmedName(ClassStr);
//
//		FString ClassVariStr;
//		FParse::Token(Cmd, ClassVariStr, true);
//		ENPCClassVariation WantedClassVariation = UB2NPCSingleClassInfo::FindNPCClassVariEnumFromTrimmedName(ClassVariStr);
//		
//		bool bSpawnAsBoss = FParse::Param(Cmd, TEXT("Boss")); // SpawnPool �� boss ������.. �̰� �����Ϸ��� Variation �� �����ؾ�..
//
//		if (WantedClass != ENPCClass::ENC_End)
//		{
//			FTransform SpawnTransform;
//			ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//			if (B2Player) // �÷��̾�� �ణ �񲸳� ��ġ��
//			{
//				SpawnTransform = B2Player->GetTransform();
//				SpawnTransform.AddToTranslation(FVector(100.0f, 0.0f, 100.0f));
//			}
//
//			FActorSpawnParameters SpawnInfo;
//			SpawnInfo.Owner = B2Player;
//			SpawnInfo.bDeferConstruction = true; // Need deferred construction and manually call FinishSpawning. AutoPossessAI ���� �� NPCClassInfo ������ ���� DeferConstruction �� �ʿ�.
//			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
//			SpawnInfo.ObjectFlags |= RF_Transient;	// We do not want to save them into the map.
//
//
//			UB2NPCClassInfoBox* ClassInfoBox = StaticFindMobClassInfoBox();
//			if (ClassInfoBox)
//			{
//				ABladeIICharacter* SpawnedNPC = ClassInfoBox->SpawnNPCClassInfoCharacter(Inworld, WantedClass, WantedClassVariation, SpawnTransform, SpawnInfo);
//
//				if (SpawnedNPC)
//				{					
//					SpawnedNPC->AutoPossessAI = EAutoPossessAI::Spawned;
//
//					FMobSpawnedBirthplaceInfo DummyBirthplaceInfo; // ������ Birthplace �� ������..
//					DummyBirthplaceInfo.bSpawnedAsBossMob = bSpawnAsBoss; // ��� �����Ϸ���.
//					SpawnedNPC->SetBirthPlace(DummyBirthplaceInfo);
//
//					SpawnedNPC->FinishSpawning(SpawnTransform);
//					SnapBladeIICharacterToGround(SpawnedNPC);
//
//					// Boss ���̸� UI �׽�Ʈ�� �ϳ��� �����̴� ������ �� �ִ� �͵� �ִ���..
//					UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
//					if (bSpawnAsBoss && DocBattle)
//					{
//						DocBattle->SetbCurrentlyBossAppeared(true); // ���� �� �ٽ� �����ؾ� ��.
//						DocBattle->SetBossMobClass(WantedClass);
//						DocBattle->SetBossMobClassVariation(WantedClassVariation);
//						DocBattle->SetBossHPMax(SpawnedNPC->GetMaxHealth());
//						DocBattle->SetBossHPCurrent(SpawnedNPC->GetHealth());
//						DocBattle->SetbBossQTEBreakable(SpawnedNPC->IsQTEBreakable());
//						DocBattle->SetBossQTEBreakPointMax(SpawnedNPC->GetMaxQTEBreakPoint());
//						DocBattle->SetBossQTEBreakPointCurrent(SpawnedNPC->GetCurrentQTEBreakPoint());
//					}
//				}
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SpawnDummyNPC")))
//	{
//		// NPCClassInfo �� ��ϵ� NPC �� dummy �� spawn. 
//		// SpawnNPC �� ������ ������ ���̴� NPC �� spawn ��Ű���� �̰� SpawnDummyPlayer ó�� �ִϸ��̼Ǹ� �Ѵ�.
//		// "SpawnDummyNPC ClassName VariationName" ClassName �� VariationName �� ���� ENPCClass �� ENPCClassVariation �׸񿡼� prefix �� ��
//
//		// �Ķ���� ����
//		FString ClassStr;
//		FParse::Token(Cmd, ClassStr, true);
//		ENPCClass WantedClass = UB2NPCSingleClassInfo::FindNPCClassEnumFromTrimmedName(ClassStr);
//
//		FString ClassVariStr;
//		FParse::Token(Cmd, ClassVariStr, true);
//		ENPCClassVariation WantedClassVariation = UB2NPCSingleClassInfo::FindNPCClassVariEnumFromTrimmedName(ClassVariStr);
//
//		FString ParsedAnimStateString;
//		FParse::Value(Cmd, TEXT("Anim="), ParsedAnimStateString);
//
//		float ParsedAnimCooltime = 0.0f;
//		FString ParsedAnimCooltimeString;
//		if (FParse::Value(Cmd, TEXT("AnimCool="), ParsedAnimCooltimeString))
//		{
//			ParsedAnimCooltime = FCString::Atof(*ParsedAnimCooltimeString);
//		}
//
//		float ParsedSpawnFloatingInterval = 0.0f;
//		FString ParsedSpawnFloatingIntervalStr;
//		if (FParse::Value(Cmd, TEXT("FUI="), ParsedSpawnFloatingIntervalStr))
//		{
//			ParsedSpawnFloatingInterval = FCString::Atof(*ParsedSpawnFloatingIntervalStr);
//		}
//
//		float ParsedSpawnDamageFxInterval = 0.0f;
//		FString ParsedSpawnDamageFxIntervalStr;
//		if (FParse::Value(Cmd, TEXT("DFX="), ParsedSpawnDamageFxIntervalStr))
//		{
//			ParsedSpawnDamageFxInterval = FCString::Atof(*ParsedSpawnDamageFxIntervalStr);
//		}
//
//		if (WantedClass != ENPCClass::ENC_End)
//		{
//			FTransform SpawnTransform;
//			ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//			if (B2Player) // �÷��̾�� �ణ �񲸳� ��ġ��
//			{
//				SpawnTransform = B2Player->GetTransform();
//				SpawnTransform.AddToTranslation(FVector(200.0f, 0.0f, 200.0f));
//			}
//
//			// iOS ������ ����
//			/*
//			ABladeIITestDummyNPC* NewSpawned = ABladeIITestDummyNPC::SpawnUtil(Inworld, WantedClass, WantedClassVariation, SpawnTransform,
//				ParsedSpawnFloatingInterval, ParsedSpawnDamageFxInterval);
//
//			if (NewSpawned)
//			{
//				if (ParsedAnimStateString == TEXT("Rand") || ParsedAnimStateString == TEXT("Random"))
//				{ // �����ϰ� �÷���
//					NewSpawned->SetUseRandomAttackState(true);
//				}
//				else
//				{ // ���س��� �ϳ� �÷���
//					NewSpawned->SetUseRandomAttackState(false);
//					NewSpawned->SetDirectAttackStateByKeyword(ParsedAnimStateString);
//				}
//
//				NewSpawned->SetCooltimeBetweenAnim(ParsedAnimCooltime);
//
//				// �⺻�����δ� �� �����̰� Ư���� ���ڸ� �ָ� �̵��̳� ȸ�� �����ϰ�
//				if (FParse::Param(Cmd, TEXT("Move"))) {
//					// ȸ������ �̵��� �ϴ� �� �콺�ν������ �� �� ��
//					NewSpawned->SetMaintainLocation(false);
//					NewSpawned->SetMaintainRotation(false);
//				}
//				if (FParse::Param(Cmd, TEXT("Rot"))) {
//					NewSpawned->SetMaintainRotation(false);
//				}
//			}
//			*/
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestCounterAttack")))
//	{
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//
//		if(B2Player)
//		{
//			B2Player->ForceCounterAttack();
//		}
//	}
//	else if (FParse::Command(&Cmd, TEXT("B2SED"))) // ���� ��� ���� ���ֱ�. ���� �� ��ŵ�� �ƴ϶� �������� ���� �ʴ� ������ ũ���� ���� ���� ���� �׽�Ʈ��.
//	{
//		FString OnOffStr;
//		FParse::Token(Cmd, OnOffStr, true);
//				
//		// GCheat_SkipEventSceneCount �ʹ� �ݴ�.. ��.��
//		if (OnOffStr == TEXT("On") || OnOffStr == TEXT("True") || OnOffStr == TEXT("1"))
//		{
//			GCheat_CompleteIgnoreEventScene = false;
//		}
//		else if (OnOffStr == TEXT("Off") || OnOffStr == TEXT("False") || OnOffStr == TEXT("0"))
//		{
//			GCheat_CompleteIgnoreEventScene = true;
//		}
//
//		if (GCheat_CompleteIgnoreEventScene){
//			UE_LOG(LogBladeII, Log, TEXT("Stage event scene will be completely off. It might be impossible to play through some stage."));
//		}
//		else{ // GCheat_SkipEventSceneCount �� ���� �� ������
//			UE_LOG(LogBladeII, Log, TEXT("Stage event scene is possible by CompleteIgnore cheat, but could be still off by other cheat."));
//		}	
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SkipSED")))
//	{
//		FString OnOffOrCountStr;
//		FParse::Token(Cmd, OnOffOrCountStr, true);
//
//		int32 SecifiedCount = FCString::Atoi(*OnOffOrCountStr);
//
//		if (SecifiedCount > 0)
//		{
//			GCheat_SkipEventSceneCount = SecifiedCount; // ������ ������ŭ�� ������鸸 ����� ��.
//		}
//		else if (OnOffOrCountStr == TEXT("On") || OnOffOrCountStr == TEXT("True"))
//		{
//			GCheat_SkipEventSceneCount = 100000000; // �ܼ��� turn-on �̸� �� ���� ū ���ڸ� �༭ ��ǻ� ��� ��ŵ �ǵ���. ������ ���߿� ġƮ�̴�.
//		}
//		else if (OnOffOrCountStr == TEXT("Off") || OnOffOrCountStr == TEXT("False") || OnOffOrCountStr == TEXT("0"))
//		{
//			GCheat_SkipEventSceneCount = -1;
//		}
//
//		if (GCheat_SkipEventSceneCount > 0){
//			
//			// ���� �÷��� �Ǵ� �� �ִٸ� ������.
//			for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//			{
//				AB2StageEventDirector* CurrSED = Cast<AB2StageEventDirector>(*ActorIt);
//				if (CurrSED && CurrSED->GetDirectingEvent() != EStageEvent::ESEV_Clear && CurrSED->IsPlayingShow())
//				{
//					//--GCheat_SkipEventSceneCount; �̰� ���Խ�Ű�� ����..
//					CurrSED->CustomFinishShow();
//				}
//			}
//
//			UE_LOG(LogBladeII, Log, TEXT("Stage event scene will be skipped for %d times, but some essential scene will still be played."), GCheat_SkipEventSceneCount);
//		}
//		else{ // GCheat_CompleteIgnoreEventScene �� ���� �� ������
//			UE_LOG(LogBladeII, Log, TEXT("Stage event scene is possible by SkipEventScene cheat, but could be still off by other cheat."));
//		}
//
//		return true;
//	}	
//	else if (FParse::Command(&Cmd, TEXT("GoldSpawnRate")))
//	{
//		extern float GCheat_DropGoldSpawnRate;
//		FString NewSpawnRateStr;
//		FParse::Token(Cmd, NewSpawnRateStr, true);
//
//		float NewSpawnRate = FCString::Atof(*NewSpawnRateStr);
//		GCheat_DropGoldSpawnRate = FMath::Clamp(NewSpawnRate, 0.0f, 1.0f);
//
//		UE_LOG(LogBladeII, Log, TEXT("GoldSpawnRate %.2f"), GCheat_DropGoldSpawnRate);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("StageDifficulty")))
//	{
//		FString DifficultyStr;
//		FParse::Token(Cmd, DifficultyStr, true);
//
//		// 0 ���� ũ�� StageManager �ʿ��� �������̵� �ؼ� ����� ����.
//		GCheat_StageDifficultyOverride = FCString::Atof(*DifficultyStr);
//
//		AB2StageGameMode* B2SGM = Cast<AB2StageGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		if (B2SGM)
//		{
//			for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//			{
//				ABladeIICharacter* CastedCharacter = Cast<ABladeIICharacter>(*ActorIt);
//				ABladeIIPlayer* CastedPlayer = Cast<ABladeIIPlayer>(*ActorIt);
//
//				if (CastedCharacter && !CastedPlayer)
//				{
//					float HealthRatio = CastedCharacter->Health / CastedCharacter->MaxHealth;
//					CastedCharacter->CharacterLevel = B2SGM->GetCurrentStageDifficultyScale();
//					CastedCharacter->InitializeCombatStats(); // �̹� spawn �� �ֵ��� ������ ���� ������ ����.
//					CastedCharacter->Health = HealthRatio * CastedCharacter->MaxHealth; // ���� �Ŀ� HP ���� ���Ƿ� ������ ������ ��
//				}
//			}
//		}
//		
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PCMerge")))
//	{
//		// Mesh merge �׽�Ʈ. UEngine::HandleMergeMeshCommand ����. ���ڸ� ���� ���ҽ��� �Ѱ����� ������ �ܼ��� ���� ĳ���� �±� ĳ�� �޽��� merge ���� 
//		// �̰� ��� ���� �־��ֱ� ����� ����� ȯ�� �۵� �׽�Ʈ�� ���ο� �� ��
//
//		bool bGenerateNewSkeleton = FParse::Param(Cmd, TEXT("NewSkeleton")); // Skeleton �� ���� ����ų� ù��° �ҽ� �޽��� skeleton �� ���յ� �޽��� skeleton ���� ���.
//
//		// array of source meshes that will be merged
//		TArray<USkeletalMesh*> SourceMeshList;
//		TArray<FSkelMeshMergeSectionMapping> ForceSectionMapping;
//		TArray<FSkelMeshMergeMtrlOverrideMapping> SrcMtrlOverrides;
//
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		if (!B2Player){ // �̰� ���ϰ� �ʿ�. ������� ���⿡ ���� �Ŷ�
//			return true;
//		}
//				 
//		FString CmdCopy = Cmd;
//		TArray<FString> Tokens;
//		while (CmdCopy.Len() > 0)
//		{
//			const TCHAR* LocalCmd = *CmdCopy;
//			FString Token = FParse::Token(LocalCmd, true);
//			Tokens.Add(Token);
//			CmdCopy = CmdCopy.Right(CmdCopy.Len() - Token.Len() - 1);
//		}
//		if (Tokens.Num() >= 2)
//		{ // Ŀ�ǵ� ���ڷ� skeletal mesh �� �Ѱ��� ���.
//			for (int32 I = 0; I < Tokens.Num(); ++I)
//			{
//				USkeletalMesh * SrcMesh = LoadObject<USkeletalMesh>(NULL, *Tokens[I], NULL, LOAD_None, NULL);
//				if (SrcMesh)
//				{
//					SourceMeshList.Add(SrcMesh);
//				}
//			}
//		}
//
//		if (SourceMeshList.Num() > 0)
//		{
//			EPCClass MainPCClass = B2Player->GetCurrentPlayerClass();
//			UB2PCClassInfoBox* PCInfoBox = StaticFindPCClassInfoBox();
//			UB2PCClassInfo* MainInfo = PCInfoBox ? PCInfoBox->GetSingleClassInfo(MainPCClass) : NULL;
//			/*if (MainInfo) {
//				SourceMeshList.Add(MainInfo->BaseMeshAsset);
//			}*/
//			TArray<FPCPartsAssetInfo> DefaultPartsCopy = MainInfo->DefaultPartsInfo;
//
//			int32 ForceSectionID = 0;
//			for (int32 SourceMeshCount = 0; SourceMeshCount < SourceMeshList.Num(); ++SourceMeshCount)
//			{
//				USkeletalMesh* CurrentSkeletalMesh = SourceMeshList[SourceMeshCount];
//				FSkeletalMeshResource* ThisMeshResource = CurrentSkeletalMesh->GetImportedResource();
//				if (ThisMeshResource && ThisMeshResource->LODModels.Num() > 0)
//				{
//					FSkelMeshMergeSectionMapping NewSectionMap;
//					FSkelMeshMergeMtrlOverrideMapping NewMtrlMap; // ���� ���߿� section ID ������ �ʿ� ���� �� �������� �ٷ� material override ��..
//					for (int32 SI = 0; SI < ThisMeshResource->LODModels[0].Sections.Num(); ++SI) // ForceSectionMapping �� LODModel 0 �� �������� �ϸ� ��.
//					{
//						NewSectionMap.SectionIDs.Add(ForceSectionID);
//
//						// ������ ��ġ�� �Ǹ� ��Ʈ�� ������ ���ļ� ����ϱ� ���� �غ�� ������ material �� ����Ѵ�. ���� �����̸� ������ material �� ����ؾ� ��.
//						// MergeSections ������ ������ null �� �� �ִ�. �׷� ���� ���� material �� ���� �ǵ� ������ ��������� ���� ������ �����ϴ� ��Ʈ�� ������ �׷��� �� ��
//
//						NewMtrlMap.SectionMatOverrides.Add(CurrentSkeletalMesh->Materials[ThisMeshResource->LODModels[0].Sections[SI].MaterialIndex].MaterialInterface);
//						++ForceSectionID;
//					}
//
//					ForceSectionMapping.Add(NewSectionMap);
//					SrcMtrlOverrides.Add(NewMtrlMap);
//				}
//			}
//		
//		
//			// create the composite mesh
//			auto CompositeMesh = NewObject<USkeletalMesh>(GetTransientPackage(), NAME_None, RF_Transient);
//
//			// create an instance of the FSkeletalMeshMerge utility
//			FSkeletalMeshMerge MeshMergeUtil(CompositeMesh, SourceMeshList, ForceSectionMapping, 0, EMeshBufferAccess::Default, nullptr, MainInfo->BaseMeshAsset, &SrcMtrlOverrides);
//
//			// merge the source meshes into the composite mesh
//			if (!MeshMergeUtil.DoMerge())
//			{
//				UE_LOG(LogEngine, Log, TEXT("DoMerge Error: Merge Mesh Test Failed"));
//				return true;
//			}
//			USkeletalMeshComponent* SKCompToSetup = B2Player->GetMesh();
//
//			for (int32 MI = 0; MI < SKCompToSetup->GetNumMaterials(); ++MI)
//				SKCompToSetup->SetMaterial(MI, nullptr);
//			SKCompToSetup->B2ScalarMatParameter.Empty(); 
//			SKCompToSetup->SetSkeletalMesh(CompositeMesh);
//			SKCompToSetup->SetAnimInstanceClass(MainInfo->GetInGameOnlyInfo(B2Player)->AnimBPOverride);
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PCMergeNoExtraBone")))
//	{
//		GCheat_PCPartMergeRemoveExtraBoneInfluence = !GCheat_PCPartMergeRemoveExtraBoneInfluence;
//
//		UE_LOG(LogBladeII, Log, TEXT("ForceRemoveExtraBoneInfluence for PC part merging %d"), GCheat_PCPartMergeRemoveExtraBoneInfluence);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SetSkill")))
//	{
//		FString SkillIdString;
//		FParse::Token(Cmd, SkillIdString, true);
//
//		if (SkillIdString.IsNumeric())
//		{
//			int32 SkillId = FCString::Atoi(*SkillIdString);
//			auto* BattleDoc = UB2UIDocHelper::GetDocBattle();
//			auto* HeroDoc = BattleDoc ? UB2UIDocHelper::GetDocHero(BattleDoc->GetCurPCClass()) : nullptr;
//
//			if (HeroDoc)
//			{
//				//Battle UI������Ʈ
//				FB2UIDocKey DocKeySkill(EUIDocType::Skill, SkillId);
//				UB2UIDocManager* UIDocManager = UB2UIDocManager::GetInstance();
//				auto* DocSkill = UIDocManager ? UIDocManager->GetDoc<UB2UIDocSkill>(DocKeySkill) : NULL;
//
//				if (!DocSkill){
//					DocSkill = UIDocManager ? UIDocManager->CreateDoc<UB2UIDocSkill>(DocKeySkill) : NULL;
//				}
//
//				HeroDoc->AddSkillDoc(DocSkill, SkillId, 0);
//
//				auto* BattleMainScene = UB2UIManager::GetInstance()->GetUI<UB2UIBattleMain>(UIFName::BattleMain);
//				if (BattleMainScene)
//					BattleMainScene->BindDocMainChar(HeroDoc);
//
//				//SkillRTManager data������Ʈ
//				ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//				if (B2Player)
//				{
//					UB2SkillRTManager* SkillRTManager = B2Player ? B2Player->GetSkillRTManager() : NULL;
//					if (SkillRTManager)
//					{
//						SkillRTManager->ChangeSkillInfo(0, SkillId);
//					}
//				}
//			}
//		}
//	}
//	else if (FParse::Command(&Cmd, TEXT("RepeatBattleCount")))
//	{
//		FString CountStr;
//		FParse::Token(Cmd, CountStr, true);
//		int32 NewCount = FCString::Atoi(*CountStr);
//
//		GCheat_RepeatBattleCountOverride = NewCount;
//
//		UE_LOG(LogBladeII, Log, TEXT("Repeat Battle Count Override %d"), GCheat_RepeatBattleCountOverride);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestWholeGameTimeScale")))
//	{
//		FString ScaleStr;
//		FParse::Token(Cmd, ScaleStr, true);
//		float NewScale = FCString::Atoi(*ScaleStr);
//
//		ABladeIIGameMode* B2GM = Cast<ABladeIIGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		if (B2GM){
//			B2GM->SetWholeGameTimeScale(NewScale);
//
//			UE_LOG(LogBladeII, Log, TEXT("WholeGameTimeScale %f"), B2GM->GetWholeGameTimeScale());
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("DummyChat")))
//	{
//		GTest_GenerateDummyChatMessage = !GTest_GenerateDummyChatMessage;
//		UE_LOG(LogBladeII, Log, TEXT("GenerateDummyChatMessage %d"), GTest_GenerateDummyChatMessage);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("UseBulkPCModel")))
//	{
//		GTest_UseNonCompositedRawPCMesh = !GTest_UseNonCompositedRawPCMesh;
//		UE_LOG(LogBladeII, Log, TEXT("UseNonCompositedRawPCMesh %d"), (int32)GTest_UseNonCompositedRawPCMesh);
//
//		for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//		{ // �κ��� ������Ʈ��..
//			LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(PCI));
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SpawnBuffText")))
//	{
//		FString BuffCatStr;
//		FParse::Token(Cmd, BuffCatStr, true);
//		const int32 BuffCat = FCString::Atoi(*BuffCatStr); // ���ڷ� �����ؼ� 0 : �Ϲ�Buff, 1 : TeamBuff, 2 : StateAttack, 3 : ResurrectBuff, 4 : Influence
//		FString BuffTypeStr;
//		FParse::Token(Cmd, BuffTypeStr, true);
//		const int32 BuffTypeInt = FCString::Atoi(*BuffTypeStr); // BuffCat �� ���� �ٸ�, 2������ TeamBuff �� �׳� �ٸ� �Ŷ� ���������� �������..
//
//		ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		if (LocalB2Player)
//		{
//			UB2BuffModeEffectInfo* BuffEffectInfo = LocalB2Player->GetBuffModeEffectInfo();
//			if (BuffEffectInfo)
//			{
//				switch (BuffCat)
//				{
//				case 5: {
//					BuffEffectInfo->SpawnBreakTextEffect(LocalB2Player);
//				}break;
//				case 6: {
//					// �̰� ���� FloatingDamageNum �ʿ�..
//					CharacterTakeDamageClass<ABladeIICharacter*, float, bool>::GetInstance().Signal(LocalB2Player, FMath::RandRange(1.0f, 2000.0f), true);
//				}break;
//				case 7: {
//					// �̰� ��� Text �� �ƴ� Effect. EndEffect ���� ���δ� ����.
//					BuffEffectInfo->CharacterBuffEffect((EBuffType)BuffTypeInt, LocalB2Player, FMath::RandBool());
//				}break;
//				default:{
//					BuffEffectInfo->SpawnCharacterBuffTextEffect(EBuffType(BuffTypeInt), LocalB2Player);
//				}break;
//				}
//
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("AddBuffSelf")))
//	{
//		FString strBuffType, strDuration, strAmount, strParam1, strParam2, strParam3, strParam4;
//		FParse::Token(Cmd, strBuffType, true);
//		FParse::Token(Cmd, strDuration, true);
//		FParse::Token(Cmd, strAmount, true);
//
//		FParse::Token(Cmd, strParam1, true);
//		FParse::Token(Cmd, strParam2, true);
//		FParse::Token(Cmd, strParam3, true);
//		FParse::Token(Cmd, strParam4, true);
//
//		EBuffType BuffType = EBuffType::Buff_None;
//
//		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EBuffType"), true);
//		if (EnumPtr && strBuffType.Len() != 0)
//		{
//			BuffType = EBuffType(EnumPtr->GetValueByName(FName(*strBuffType)));
//		}
//		
// 		float fDuration = FCString::Atof(*strDuration);
//
//		TArray<float> arAdditionalParams;
//
//		if (strParam1.Len() != 0)
//			arAdditionalParams.Add(FCString::Atof(*strParam1));
//
//		if (strParam2.Len() != 0)
//			arAdditionalParams.Add(FCString::Atof(*strParam2));
//
//		if (strParam3.Len() != 0)
//			arAdditionalParams.Add(FCString::Atof(*strParam3));
//
//		if (strParam4.Len() != 0)
//			arAdditionalParams.Add(FCString::Atof(*strParam4));
//
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		if (B2Player)
//		{
//			B2Player->AddBuff(BuffType, fDuration, FCString::Atof(*strAmount), B2Player, true, true, arAdditionalParams.Num() == 0 ? nullptr : &arAdditionalParams);
//		}
//	}
//	else if (FParse::Command(&Cmd, TEXT("AddBuffEnemy")))
//	{
//		FString strBuffType, strDiatance, strDuration, strAmount;
//		FParse::Token(Cmd, strBuffType, true);
//		FParse::Token(Cmd, strDiatance, true);
//		FParse::Token(Cmd, strDuration, true);
//		FParse::Token(Cmd, strAmount, true);
//
//		EBuffType BuffType = EBuffType::Buff_None;
//
//		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EBuffType"), true);
//		if (EnumPtr && strBuffType.Len() != 0)
//		{
//			BuffType = EBuffType(EnumPtr->GetValueByName(FName(*strBuffType)));
//		}
//
//		float fDistance = FCString::Atof(*strDiatance);
//		float fDuration = FCString::Atof(*strDuration);
//
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		if (B2Player)
//		{
//			B2Player->AddBuffEnemy(BuffType, fDistance, fDuration, FCString::Atof(*strAmount));
//		}
//
//	}
//	else if (FParse::Command(&Cmd, TEXT("ClearBuffSelf")))
//	{
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		if (B2Player)
//		{
//			B2Player->ClearAllBuffs();
//		}
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestRewardPopup")))
//	{
//		FString StrGold, StrGem, StrBladePoint;
//
//		FParse::Token(Cmd, StrGold, true);
//		const int32 Gold = FCString::Atoi(*StrGold);
//
//		FParse::Token(Cmd, StrGem, true);
//		const int32 Gem = FCString::Atoi(*StrGem);
//
//		FParse::Token(Cmd, StrBladePoint, true);
//		const int32 BladePoint = FCString::Atoi(*StrBladePoint);
//
//		FString StrItemID, StrGeneralLottery, StrSelectiveLottery;
//		FParse::Token(Cmd, StrItemID, true);
//		const int32 ItemID = FCString::Atoi(*StrItemID);
//
//		FParse::Token(Cmd, StrGeneralLottery, true);
//		const int32 GeneralLottery = FCString::Atoi(*StrGeneralLottery);
//
//		FParse::Token(Cmd, StrSelectiveLottery, true);
//		const int32 SelectiveLottery = FCString::Atoi(*StrSelectiveLottery);
//
//
//		UB2UIManager* UIMgr = UB2UIManager::GetInstance();
//		if (UIMgr)
//		{
//			UB2UIMsgPopupReward* Popup = UIMgr->OpenMsgPopup<UB2UIMsgPopupReward>(EUIMsgPopup::ItemReward, FText::GetEmpty(), FText::GetEmpty());
//			if (Popup)
//			{
//				Popup->AddSingleRewardItem(ERewardType::Gold, Gold);
//				Popup->AddSingleRewardItem(ERewardType::Gem, Gem);
//				Popup->AddSingleRewardItem(ERewardType::BladePoint, BladePoint);
//
//				Popup->AddSingleRewardItem(ERewardType::Item, ItemID);
//				Popup->AddSingleRewardItem(ERewardType::General_Lottery, GeneralLottery);
//				Popup->AddSingleRewardItem(ERewardType::Selective_Lottery, SelectiveLottery);
//
//				Popup->ShowRewardPopup();
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestWingEvolScene")))
//	{
//		BeginWingEvolutionSceneClass<EPCClass>::GetInstance().Signal(EPCClass::EPC_End); // �� �̺�Ʈ�� �޴� �������� ��忩��.
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("STLog")))
//	{ // B2_SCOPED_TRACK_LOG �� ���� �αװ� �ʹ� ���Ƽ� �ٸ� ����뿡 ���ذ� �� ���� ����.
//		// �׳� ���� �α� ī�װ� ���� ��������� �װ� ������ Log LogB2ScopedCallTrack Off ����� �־ ��.
//		FB2ScopedCallTracker::bIsOn = !FB2ScopedCallTracker::bIsOn;
//		UE_LOG(LogBladeII, Log, TEXT("B2ScopedCallTrack Log being turned %s"), FB2ScopedCallTracker::bIsOn ? TEXT("ON") : TEXT("OFF"));
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("STimeLog")))
//	{ // ���⵵ �������� �α� ī�װ��� ������ �װ� �ᵵ.. LogB2ScopedTimeLog
//		FB2ScopedTimeLogger::bIsOn = !FB2ScopedTimeLogger::bIsOn;
//		UE_LOG(LogBladeII, Log, TEXT("FB2ScopedTimeLogger being turned %s"), FB2ScopedTimeLogger::bIsOn ? TEXT("ON") : TEXT("OFF"));
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("MissySexy")))
//	{
//		// ��������ĳ�迡 �δ���� ������ ���� ��� ����. �̰͵鸸 ������ �尩 ���� �� �����. 
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1100600 4")); // ServerCmd ����
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1102600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1200600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1202600 4"));
//		
//		// �Ʒ� �Ǽ��縮�� �� ü�� �ø��� ��.
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1105600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1106600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1107600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1205600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1206600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1207600 4"));
//
//		// �������� �콺�ν����� ���� ���� ��丸 6������.
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1001600 4"));
//
//		// �׸��� �߰��� �ָӴϵ� �˳��ϰ� ä������. �̰͵��� GetAccount �� �־�� �ݿ���.
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncGold 10000000"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncGem 1000000"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncDarkCrystal 1000000"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncRankPromotionPoint 100000"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncSocialPoint 1000"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncHeroPiece 1000"));
//
//		// ���� ��ȭ ���� ���.
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 8000005 1 999"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 8000006 1 999"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 8000007 1 999"));
//
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 8000010 1 999"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 8000011 1 999"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 8000012 1 999"));
//
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 8000013 1 999"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 8000014 1 999"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 8000015 1 999"));
//
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 8000008 1 999"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 8000009 1 999"));
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ItemOptionTest")))
//	{
//		// E.g. Assassin, Weapon (if equipped), IncCriticalRate 55, IncHealth 20, AdditionalExt 80
//		// ItemOptionTest PC=1 EQ=0 Options=14-55,32-20,51-80
//
//		int32 PCClassInt = 0;
//		FParse::Value(Cmd, TEXT("PC="), PCClassInt);
//		EPCClass CastedPCClass = IntToPCClass( PCClassInt );
//		if (CastedPCClass == EPCClass::EPC_End)
//		{
//			CastedPCClass = IntToPCClass(GetMaxPCClassNum() - 1);
//		}
//
//		int32 EquipPlaceInt = 0;
//		FParse::Value(Cmd, TEXT("EQ="), EquipPlaceInt);
//		EItemEquipPlace CastedEquipPlace = static_cast<EItemEquipPlace>(EquipPlaceInt);
//		if (CastedEquipPlace == EItemEquipPlace::EIEP_End)
//		{
//			CastedEquipPlace = static_cast<EItemEquipPlace>((int32)EItemEquipPlace::EIEP_End - 1);
//		}
//
//		// Options= ���ķδ� ID-Value,ID-Value �̷� ������ �߰��ϱ� ���ϴ� ���� �ɼǵ��� ����. ID �� ���� B2ItemOption ���� ����. �� ���ڰ� ��ȹ ���� ���Ͽ��� ���� �Ͱ� ��ġ�ϹǷ�
//		FString RawOptionString;
//		FParse::Value(Cmd, TEXT("Options="), RawOptionString, false); // No stop on comma.
//		TArray<FString> ParsedOptionStringArray;
//		RawOptionString.ParseIntoArray(ParsedOptionStringArray, TEXT(","));
//		
//		TArray<FItemOption> FinalGatheredOptions;
//		for (FString ThisParsedStr : ParsedOptionStringArray)
//		{
//			FString OptionIDStr;
//			FString OptionValueStr;
//			// ID-Value ������ ������ �������� ���.
//			if (ThisParsedStr.Split(TEXT("-"), &OptionIDStr, &OptionValueStr))
//			{
//				int32 OptionIDInt = FCString::Atoi(*OptionIDStr);
//				EItemOption CliItemOption = SvrToCliOptionType(OptionIDInt); // ID ���ڴ� ���� Ÿ�Կ��� ����ϴ� ������ Client enum type ���� ����.
//				
//				float OptionValueFloat = FCString::Atof(*OptionValueStr);
//
//				if (CliItemOption != EItemOption::EIO_End)
//				{
//					FItemOption ThisItemOptionStruct;
//					ThisItemOptionStruct.OptionType = CliItemOption;
//					ThisItemOptionStruct.RawOptionAmount = OptionValueFloat;
//					FinalGatheredOptions.Add(ThisItemOptionStruct);
//				}
//			}
//		}
//
//		// �ϴ��� ���� �� ������ �ɼ�����.. ���� �Ű������� �� ���� �ְ�..
//		UB2LobbyInventory::DevOnlyReplaceEquippedOptions(CastedPCClass, CastedEquipPlace, FinalGatheredOptions);
//
//		LobbyUpdateAllItemIconsClass<>::GetInstance().Signal();
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PCOptionDisplay")) || FParse::Command(&Cmd, TEXT("PlayerOptionDisplay")))
//	{
//		FString DisplaySetNumStr;
//		FParse::Token(Cmd, DisplaySetNumStr, true);
//		int32 DisplaySetNum = FCString::Atoi(*DisplaySetNumStr);
//		UB2LobbyUI_CharStatDetail::SetDebugDrawPlayerStatMode(DisplaySetNum); // DisplaySetNum �� 0 ���� ũ�� ���̱� ������ ��.
//
//		// �������� ����� ������ ���� �÷��̾� ĳ���Ͱ� ���� ��. �̰� ��Ʈ��ũ ��� ������ �ǵ��� ��.
//		int32 PCClassInt = 0;
//		FParse::Value(Cmd, TEXT("NetPC="), PCClassInt);
//		EPCClass OverridePCClass = IntToPCClass(PCClassInt);
//		UB2LobbyUI_CharStatDetail::SetDebugDrawPlayerStatClass(OverridePCClass);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("NumToPlayRaid"))) // �ݰ��Ӹ�� �÷��� ������ ���̱�, ���̵�� ȥ���ϱⰡ �ǹǷ� �Ⱦ�
//	{
//		FString NumStr;
//		FParse::Token(Cmd, NumStr, true);
//		int32 RaidPlayNum = FCString::Atoi(*NumStr);
//
//		NetFakeConfigure::GetInstance().SetNumToPlayRaid(RaidPlayNum);
//
//		UE_LOG(LogBladeII, Log, TEXT("NumToPlayRaid %d"), RaidPlayNum);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ClearLocalSavedState")))
//	{// �׽�Ʈ�� ���� ġƮŰ�� ����. Ư�� ��Ȳ���� �����ؼ� ����ؾ� �� ���.
//		ClearLocalIniSavedState(); 
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ToggleShowWing")))
//	{
//		if (AB2LobbyGameMode* B2LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld)))
//		{
//			B2LobbyGM->ToggleShowLobbyWing();
//		}
//		
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ShowDevUI")))
//	{ 
//		gIsDevUI = !gIsDevUI;
//		UE_LOG(LogBladeII, Log, TEXT("Dev UI Mode : %s"), gIsDevUI ? TEXT("ON") : TEXT("OFF"));
//
//#if !UE_BUILD_SHIPPING
//		// �ʼ����� �ƴϰ� �ٸ� ������ �׽�Ʈ �� �� ���߿� ShowDevUI ġ�� �ٷ� ������Ʈ �ȵǸ� ����� ���� �������� �� ������Ʈ �� ��.
//		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
//		UB2UIBattleMain* BattleMainUI = UIMgrInst ? UIMgrInst->GetUI<UB2UIBattleMain>(UIFName::BattleMain) : NULL;
//		if (BattleMainUI)
//		{
//			BattleMainUI->UpdateDevFxLODWidgets();
//		}
//#endif
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("EnterShop")))
//	{
//		FString HasSomething;
//		FParse::Token(Cmd, HasSomething, true);
//
//		bool bGeneralShop = true;
//		if (HasSomething == TEXT(""))
//		{
//			bGeneralShop = false;
//		}
//
//		EStorePageWhere TargetPage = bGeneralShop ? EStorePageWhere::Store : EStorePageWhere::EquipStore;
//		EnterShopClass<int32>::GetInstance().Signal(static_cast<int32>(TargetPage));
//		
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("LogAddFilter")))
//	{
//		FString AddFilterName;
//		FParse::Token(Cmd, AddFilterName, true);
//
//#if DEBUG_LOG
//		B2Logger::B2LogManager::AddFilter(std::string(TCHAR_TO_UTF8(*AddFilterName)));
//#endif //DEBUG_LOG
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("LogRemoveFilter")))
//	{
//		FString RemoveFilterName;
//		FParse::Token(Cmd, RemoveFilterName, true);
//
//#if DEBUG_LOG
//		B2Logger::B2LogManager::RemoveFilter(std::string(TCHAR_TO_UTF8(*RemoveFilterName)));
//#endif //DEBUG_LOG
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("LogRemoveAllFilter")))
//	{
//#if DEBUG_LOG
//		B2Logger::B2LogManager::RemoveAllFilter();
//#endif //DEBUG_LOG
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("LogShowAllFilter")))
//	{
//#if DEBUG_LOG
//		B2Logger::B2LogManager::ShowFilterList();
//#endif //DEBUG_LOG
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("LogChangeLogLevel")))
//	{
//		FString NumStr;
//		FParse::Token(Cmd, NumStr, true);
//		auto level = FCString::Atoi(*NumStr);
//#if DEBUG_LOG
//		GB2LOGMANAGER_LOG_LEVEL = level < B2ClientLogLevel::B2_LOG_LEVEL_NOSHOW ? level : B2ClientLogLevel::B2_LOG_LEVEL_NOSHOW;
//#endif //DEBUG_LOG
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("LogShowLogLevel")))
//	{
//#if DEBUG_LOG
//		B2Logger::B2LogManager::ShowLogLevel();
//#endif //DEBUG_LOG
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("LogShowFileInfo")))
//	{
//		FString NumStr;
//		FParse::Token(Cmd, NumStr, true);
//		auto show = FCString::Atoi(*NumStr);
//#if DEBUG_LOG
//		GB2LOGMANAGER_LOG_SHOW_FILEINFO = show > 0 ? true : false;
//#endif //DEBUG_LOG
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SetWindowTitle")))
//	{
//		FString TitleStr;
//		FParse::Token(Cmd, TitleStr, true);
//		UKismetSystemLibrary::SetWindowTitle(FText::FromString(TitleStr));
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestResetStage")))
//	{ // �׽�Ʈ Ŀ�ǵ� �������� AB2StageGameModeBase �� ������ ������ ���� ����� �̰� ���� Ư�� ���Ӹ�忡���� �ؾ� �Ѵ�.
//		AB2StageGameModeBase* SGM = Cast<AB2StageGameModeBase>(UGameplayStatics::GetGameMode(Inworld));
//		if (SGM)
//		{
//			SGM->ResetAssumeNoWorldLoading();
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("WeaponSkillPoint")))
//	{
//		FString PointStr;
//		FParse::Token(Cmd, PointStr, true);
//		int32 PointToAdd = FCString::Atoi(*PointStr);
//
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		if (B2Player) {
//			B2Player->SetWeaponSkillPoint(B2Player->GetWeaponSkillPoint() + PointToAdd);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestWeaponSkill")))
//	{
//		FString FirstArgStr;
//		FParse::Token(Cmd, FirstArgStr, true);
//		int32 FirstArgInt = FCString::Atoi(*FirstArgStr);
//
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		if (B2Player)
//		{
//			ESkillAnimType WantedSkillAnim = (ESkillAnimType)((int32)ESkillAnimType::ESA_Weapon_01 + FirstArgInt);
//			if (WantedSkillAnim == ESkillAnimType::ESA_End)
//			{
//				WantedSkillAnim = ESkillAnimType::ESA_Weapon_01;
//			}
//
//			B2Player->DevTestStartWeaponSkill(WantedSkillAnim);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("WeaponSkillLight")))
//	{
//		extern float GDevTestWeaponSkillLOD0LightIntensity;
//		extern float GDevTestWeaponSkillLOD0LightRadius;
//		FParse::Value(Cmd, TEXT("Intensity="), GDevTestWeaponSkillLOD0LightIntensity);
//		FParse::Value(Cmd, TEXT("Radius="), GDevTestWeaponSkillLOD0LightRadius);
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("WeaponSkillLight Intensity %.2f, Radius %.2f"), GDevTestWeaponSkillLOD0LightIntensity, GDevTestWeaponSkillLOD0LightRadius),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("MakeDir")))
//	{
//		FString DirName;
//		FParse::Token(Cmd, DirName, true);
//
//		IFileManager::Get().MakeDirectory(*DirName);
//	}
//	else if (FParse::Command(&Cmd, TEXT("DirExists")))
//	{
//		FString DirName;
//		FParse::Token(Cmd, DirName, true);
//
//		if (IFileManager::Get().DirectoryExists(*DirName))
//		{
//			UE_LOG(LogBladeII, Display, TEXT("%s Directory Exists"), *DirName);
//		}
//		else
//		{
//			UE_LOG(LogBladeII, Display, TEXT("%s Directory No Exists"), *DirName);
//		}
//	}
//	else if (FParse::Command(&Cmd, TEXT("Testpoikl2212")))
//	{
//		// ������ - �׽�Ʈ������ ���� Ŀ�ǵ��Դϴ�.
//	}
//	else if (FParse::Command(&Cmd, TEXT("HeroTowerLoading")))
//	{
//		gUseHeroTowerResetNoLoading = !gUseHeroTowerResetNoLoading;
//		UE_LOG(LogBladeII, Display, TEXT("HeroTowerGameMode restart without world loading %d"), (int32)gUseHeroTowerResetNoLoading);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CharFxLOD")) || FParse::Command(&Cmd, TEXT("CFxLOD"))) // "CFxLOD PC/Mob/Boss #" ĳ���� �������� ParticleSystemComponent LOD ����
//	{
//		FString TypeStr;
//		FParse::Token(Cmd, TypeStr, true);
//
//		FString ParsedLODLevelStr;
//		FParse::Token(Cmd, ParsedLODLevelStr, true); // �ϰ͵� ���� �ȵǾ��ٸ� 0�̰���. �׷� 0���� �����ϸ� ��.
//		const int32 ParsedLODLevel = FCString::Atoi(*ParsedLODLevelStr);
//
//		if (TypeStr == TEXT("None")){
//			// ��� ����
//			GB2CharFxCheatLODLevel_PC = -1;
//			GB2CharFxCheatLODLevel_Mob = -1;
//			GB2CharFxCheatLODLevel_Boss = -1;
//		}
//		else if (TypeStr == TEXT("PC")){
//			GB2CharFxCheatLODLevel_PC = ParsedLODLevel;
//		}
//		else if (TypeStr == TEXT("Mob")){
//			GB2CharFxCheatLODLevel_Mob = ParsedLODLevel;
//		}
//		else if (TypeStr == TEXT("Boss")){
//			GB2CharFxCheatLODLevel_Boss = ParsedLODLevel;
//		}
//		else if (TypeStr == TEXT("All")) {
//			GB2CharFxCheatLODLevel_PC = ParsedLODLevel;
//			GB2CharFxCheatLODLevel_Mob = ParsedLODLevel;
//			GB2CharFxCheatLODLevel_Boss = ParsedLODLevel;
//		}
//		else{
//			GB2CharFxCheatLODLevel_PC = -1; // ���� �� �ƴ� ��쵵 ����
//			GB2CharFxCheatLODLevel_Mob = -1;
//			GB2CharFxCheatLODLevel_Boss = -1;
//		}
//		GB2CharFxCheatLODLevel_PC = FMath::Min(GB2CharFxCheatLODLevel_PC, GB2CharFxCheatLODLevel_PC_Max);
//		GB2CharFxCheatLODLevel_Mob = FMath::Min(GB2CharFxCheatLODLevel_Mob, GB2CharFxCheatLODLevel_Mob_Max);
//		GB2CharFxCheatLODLevel_Boss = FMath::Min(GB2CharFxCheatLODLevel_Boss, GB2CharFxCheatLODLevel_Boss_Max);
//
//		for (TObjectIterator<UParticleSystemComponent> PSCIT; PSCIT; ++PSCIT)
//		{
//			UParticleSystemComponent* ThisComp = Cast<UParticleSystemComponent>(*PSCIT);
//			if (ThisComp && ThisComp->Template)
//			{
//				if (IsB2CharFxCheatLODInUse()){
//					ThisComp->SetLODLevel(0); // ���� �־��ִ� ���� �ǹ̾��� �� ������Ʈ ����..
//				}
//				else {
//					if (ThisComp->GetWorld()) {
//						ThisComp->RestoreLODLevelToDesiredValue(); // �̷��� ���� ����Ǵ� ������ ���ư�����
//					}
//				}
//			}
//		}
//
//#if !UE_BUILD_SHIPPING
//		// Ȥ Ŀ�ǵ带 ������ ��ư ����ǵ���..
//		UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
//		UB2UIBattleMain* BattleMainUI = UIMgrInst ? UIMgrInst->GetUI<UB2UIBattleMain>(UIFName::BattleMain) : NULL;
//		if (BattleMainUI)
//		{
//			BattleMainUI->UpdateDevFxLODWidgets();
//		}
//#endif
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("RTMP"))) // �׽�Ʈ�� ������ ª�� ����..
//	{
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("ResetTeamMatchPoint"));
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SetCustomTimeDilation")))
//	{
//		FString DilationStr;
//		FParse::Token(Cmd, DilationStr, true);
//		float fDilation = FCString::Atof(*DilationStr);
//
//		ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//
//		if (LocalB2Player)
//			LocalB2Player->SetCustomTimeDilation(fDilation);
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestNormalDamage")))
//	{
//		ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//
//		LocalB2Player->UpdateDamageNum(FDamageInfo::GetDefaultDamage());
//		LocalB2Player->ApplyDefaultDamageAnimation();
//		LocalB2Player->LastDamageType = EAttackType::EAT_Default;
//	}	
//	else if (FParse::Command(&Cmd, TEXT("NMLOD")))
//	{
//		FString TypeStr;
//		FParse::Token(Cmd, TypeStr, true);
//
//		auto lod = FCString::Atoi(*TypeStr);
//		auto ClampedLod = FMath::Clamp(lod, 0, 2);
//
//		NetFakeConfigure::GetInstance().SetNetMessageLOD(ClampedLod);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("NMF")))
//	{
//		FString TypeStr;
//		FParse::Token(Cmd, TypeStr, true);
//
//		auto Frequency = FCString::Atoi(*TypeStr);
//		auto ClampedFrequency = FMath::Clamp(Frequency, 50, 1000);
//
//		NetFakeConfigure::GetInstance().SetNetMessageFrequency(ClampedFrequency);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ULRadius")))
//	{
//		FString TypeStr;
//		FParse::Token(Cmd, TypeStr, true);
//
//		auto Radius = FCString::Atoi(*TypeStr);
//		auto ClampedRadius = FMath::Clamp(Radius, 10, 2000);
//
//		NetFakeConfigure::GetInstance().SetUpdateLocationRadius(ClampedRadius);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ShowPerfMark")))
//	{
//		FString Lv1TimeStr;
//		FParse::Token(Cmd, Lv1TimeStr, true);
//
//		FString Lv2TimeStr;
//		FParse::Token(Cmd, Lv2TimeStr, true);
//
//		if (Lv1TimeStr.Len() == 0)
//		{ // �� ��� �ܼ��� ���� On/Off 
//			gDrawPerfMark = !gDrawPerfMark;
//		}
//		else
//		{
//			gDrawPerfMark = true; // PerfMark �ð� ���� ���� �� ������ �Ҵ�.
//			FB2ScopedCallTracker::PerfMarkLv1Time = FMath::Max(0.001f, FCString::Atof(*Lv1TimeStr));
//			FB2ScopedCallTracker::PerfMarkLv2Time = FMath::Max(FB2ScopedCallTracker::PerfMarkLv1Time, FCString::Atof(*Lv2TimeStr));
//		}
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("ShowPerfMark %s, Lv1Time %.1f ms, LV2Time %.1f ms"), gDrawPerfMark ? TEXT("ON") : TEXT("OFF"),
//			FB2ScopedCallTracker::PerfMarkLv1Time * 1000.f, FB2ScopedCallTracker::PerfMarkLv2Time * 1000.0f), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SkMeshCPUData")))
//	{
//		GAllowRemoveCPURenderData = !GAllowRemoveCPURenderData;
//		BII_SCREEN_LOG(FString::Printf(TEXT("GAllowRemoveCPURenderData %s"), GAllowRemoveCPURenderData ? TEXT("ON") : TEXT("OFF")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PreloadCombatFx")))
//	{
//		gPreloadCombatEffectResources = !gPreloadCombatEffectResources;
//		BII_SCREEN_LOG(FString::Printf(TEXT("PreloadCombatEffectResources %s"), gPreloadCombatEffectResources ? TEXT("ON") : TEXT("OFF")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("B2ScaleOverride")))
//	{
//		B2Scalability::bUseBladeIIScalabilityOverride = !B2Scalability::bUseBladeIIScalabilityOverride;
//		BII_SCREEN_LOG(FString::Printf(TEXT("BladeIIScalabilityOverride %s"), B2Scalability::bUseBladeIIScalabilityOverride ? TEXT("ON") : TEXT("OFF")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		B2Scalability::AdjustScalabilityBySavedUserSelection(Inworld);
//
//		// GameplayOnlyLODQuality ������ ��� bUseBladeIIScalabilityOverride �� ���� �ΰ��� ���� LOD ������ �޶��� �� ������ BladeIIPlayer iteration ��..
//		for (FActorIterator ItPlayer(Inworld); ItPlayer; ++ItPlayer)
//		{
//			ABladeIIPlayer* CastedPlayer = Cast<ABladeIIPlayer>(*ItPlayer);
//			if (CastedPlayer)
//			{ // Ȥ���� ���� �÷��� �����̶� �ϴ���.. ���� ���߿��� ��κ� BladeIIPlayer �� �ƴ� SkeletalMeshActor �� ���ǹǷ� �̰��� ȣ���ϴ� �� ������ ��ġ�� �ʴ´�. �ƴ� ���� �ϴ��� ���� ���߿� pause �޴��� �������� �ʰ�..
//				CastedPlayer->SetupLODInfoForInGame();
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GF"))) // �츮 ���� ȯ�濡�� �������� ��ε� feature �� On/Off �ϸ鼭 ���� ������ ����. ����� ����̽����� �����ϰ� Ŀ�ǵ� ���� �� �ֵ��� ª�� ��
//	{
//		FString FirstArgStr;
//		FParse::Token(Cmd, FirstArgStr, true);
//
//		if (FirstArgStr == TEXT("FxParticle"))
//		{
//			bool bWasDisabled = (UAnimNotifyState_TimedParticleEffect::bDisableTimedParticleEffect || UAnimNotify_PlayParticleEffect::bDisablePlayParticleEffect || UB2DamageEffectInfo::bDisableDamageEffectParticle);
//			// ���� �Ȱ��� ��Ʈ�� �ǵ���
//			UAnimNotifyState_TimedParticleEffect::bDisableTimedParticleEffect = !bWasDisabled;
//			UAnimNotify_PlayParticleEffect::bDisablePlayParticleEffect = !bWasDisabled;
//			UB2DamageEffectInfo::bDisableDamageEffectParticle = !bWasDisabled; // �̰͵� �����ε�.. ���� ���°� ����..
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("Particle from AnimNotify/EffectInfo %s"), bWasDisabled ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		if (FirstArgStr == TEXT("FxNotify")) // FxParticle �� �Ϻ�
//		{
//			bool bWasDisabled = (UAnimNotifyState_TimedParticleEffect::bDisableTimedParticleEffect || UAnimNotify_PlayParticleEffect::bDisablePlayParticleEffect);
//			// ���� �Ȱ��� ��Ʈ�� �ǵ���
//			UAnimNotifyState_TimedParticleEffect::bDisableTimedParticleEffect = !bWasDisabled;
//			UAnimNotify_PlayParticleEffect::bDisablePlayParticleEffect = !bWasDisabled;
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("Particle from AnimNotify/EffectInfo %s"), bWasDisabled ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		if (FirstArgStr == TEXT("DFX")) // FxParticle �� �Ϻ�
//		{
//			UB2DamageEffectInfo::bDisableDamageEffectParticle = !UB2DamageEffectInfo::bDisableDamageEffectParticle;
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("Particle from DamageEffectInfo %s"), !UB2DamageEffectInfo::bDisableDamageEffectParticle ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstArgStr == TEXT("FxSound"))
//		{
//			bool bWasDisabled = (UAnimNotify_PlaySound::bDisablePlaySound || UB2DamageEffectInfo::bDisableDamageEffectSound);
//			UAnimNotify_PlaySound::bDisablePlaySound = !bWasDisabled;
//			UB2DamageEffectInfo::bDisableDamageEffectSound = !bWasDisabled;
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("Sound from AnimNotify/EffectInfo %s"), bWasDisabled ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstArgStr == TEXT("FloatUI"))
//		{
//			gDisableInGameFloatingWidgets = !gDisableInGameFloatingWidgets;
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("Floating Wigdets from in-gmae play %s"), !gDisableInGameFloatingWidgets ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstArgStr == TEXT("CombatEventUI"))
//		{
//			gDisableCombatEventUI = !gDisableCombatEventUI;
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("Combat event UIs %s"), !gDisableCombatEventUI ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstArgStr == TEXT("PC"))
//		{
//			ABladeIIPlayer::bHideAllPCs = !ABladeIIPlayer::bHideAllPCs;
//			// SetHidden �� �ؾ� �����. �ٽ� �� �� ������ ������ �־�� �ϴ� ���¶�� ������ �ǰ�����.. �׽�Ʈ�ϴµ� ���������̾�
//			for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//			{
//				ABladeIIPlayer* CastedPC = Cast<ABladeIIPlayer>(*ActorIt);
//				if (CastedPC)
//				{ // bHideAllPCs true �� ���� �ѹ� ����� �ٽ� SetActorHiddenInGame(false) �ص� �� ����.
//					CastedPC->SetActorHiddenInGame(ABladeIIPlayer::bHideAllPCs);
//				}
//			}
//			BII_SCREEN_LOG(FString::Printf(TEXT("Player Characters %s"), !ABladeIIPlayer::bHideAllPCs ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstArgStr == TEXT("NPC"))
//		{
//			ABladeIICharacter::bHideAllNPCs = !ABladeIICharacter::bHideAllNPCs;
//			// SetHidden �� �ؾ� �����. �ٽ� �� �� ������ ������ �־�� �ϴ� ���¶�� ������ �ǰ�����.. �׽�Ʈ�ϴµ� ���������̾�
//			for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//			{
//				ABladeIIPlayer* CastedPC = Cast<ABladeIIPlayer>(*ActorIt);
//				ABladeIICharacter* CastedNPC = Cast<ABladeIICharacter>(*ActorIt);
//				if (CastedNPC && !CastedPC)
//				{ // bHideAllPCs true �� ���� �ѹ� ����� �ٽ� SetActorHiddenInGame(false) �ص� �� ����.
//					CastedNPC->SetActorHiddenInGame(ABladeIICharacter::bHideAllNPCs);
//				}
//			}
//			BII_SCREEN_LOG(FString::Printf(TEXT("Non-Playable Characters %s"), !ABladeIICharacter::bHideAllNPCs ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstArgStr == TEXT("SELight"))
//		{
//			AB2StageEventDirector::bDisableStageEventLights = !AB2StageEventDirector::bDisableStageEventLights;
//			BII_SCREEN_LOG(FString::Printf(TEXT("StageEventCinematics dynamic light %s"), !AB2StageEventDirector::bDisableStageEventLights ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CalmDown")))
//	{ // ��ŷ �������� ���� �� ���� ���¿��� �÷����� �� �ӵ��� �̻��ϰ� �������� �̷��Զ� �ÿ���å.
//		ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		if (LocalB2Player)
//		{
//			LocalB2Player->DevCalmDown();
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PCDataLoad")))
//	{
//		FString NumStr;
//		FParse::Token(Cmd, NumStr, true);
//		gPCDataPreLoadMaxNum = FCString::Atoi(*NumStr);
//		BII_SCREEN_LOG(FString::Printf(TEXT("Player Character Data Pre-Load Num %d"), gPCDataPreLoadMaxNum),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("NPCDataLoad")))
//	{
//		gNPCDataNoPreLoadMode = !gNPCDataNoPreLoadMode;
//		BII_SCREEN_LOG(FString::Printf(TEXT("NPC Data Pre-Load %s"), !gNPCDataNoPreLoadMode ? TEXT("ON") : TEXT("OFF")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ManagedInfoUnload")))
//	{
//		// ���⼭ �����Ϸ��� ������ ini �׸� �о���̴� ���� �� InfoAsset �����ڿ��� �ϹǷ� ���� �о������ �ʾҴٸ� ���⼭ �� ��.
//		StaticFindPCClassInfoBox();
//		StaticFindSkillAnimInfo();
//
//		// ���� ���ÿ� �Ȱ��� ������ �� �ֵ���
//		const bool bPrevManagedUnload = (!UB2PCClassInfoBox::ShouldUnloadAllPCClassInfoOnLevelTransition() && !UB2SkillAnimInfo::ShouldUnloadAllSkillAnimOnLevelTransition());
//
//		UB2PCClassInfoBox::DevSetManagedPCClassUnload(!bPrevManagedUnload);
//		UB2SkillAnimInfo::DevSetManagedSkillAnimUnload(!bPrevManagedUnload);
//		BII_SCREEN_LOG(FString::Printf(TEXT("Managed Unloading of PCClass %s, SkillAnimInfo %s"), 
//			UB2PCClassInfoBox::ShouldUnloadAllPCClassInfoOnLevelTransition() ? TEXT("OFF") : TEXT("ON"),
//			UB2SkillAnimInfo::ShouldUnloadAllSkillAnimOnLevelTransition() ? TEXT("OFF") : TEXT("ON")		
//			),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 8.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestLoadAllPCClass"))) // PCClassInfo �ε��Ǵ� ���� ���̴� �� �󸶳� ������ �Ǵ��� ���� ����
//	{
//		double fStartTime = FPlatformTime::Seconds();
//		
//		UB2PCClassInfoBox* PCClassInfoBox = StaticFindPCClassInfoBox();
//		int32 FullyLoadedNum = 0;
//		if (PCClassInfoBox)
//		{
//			// �ε��� �� �Ǿ� �ִٴ� �������� �ϴ� ����.
//			for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//			{
//				EPCClass ThisClassToLoad = IntToPCClass(PCI);
//
//				UB2PCClassInfo* LoadedClassInfo = PCClassInfoBox->GetSingleClassInfo(ThisClassToLoad);
//				if (LoadedClassInfo)
//				{
//					if (LoadedClassInfo->GetInGameOnlyInfo(Inworld)) // �����δ� InGameOnlyInfo ���� �ε��Ͽ��� �ǹ� �ִ� ���� �����͵��� �ε��ȴ�.
//					{
//						++FullyLoadedNum;
//					}
//				}
//			}
//		}
//		double fEndTime = FPlatformTime::Seconds();
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("TestLoadAllPCClass %d classes took %f s."), FullyLoadedNum, fEndTime - fStartTime),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 8.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestLoadSkillAnims"))) // PCClassInfo �� �Ϻημ� �ε��Ǵ� SkillInfo �ε� �׽�Ʈ
//	{
//		bool bLoadAll = FParse::Param(Cmd, TEXT("All")); // �ƴϸ� �⺻ ���� ��忡 �ʿ��� �͸�
//
//		double fStartTime = FPlatformTime::Seconds();
//
//		TArray<FCombinedPCSkillAnimID> AllSkillAnimIDToLoad;
//		UB2SkillAnimInfo* SkillAnimInfo = StaticFindSkillAnimInfo();
//		if (SkillAnimInfo)
//		{			
//			// �ε��� �� �Ǿ� �ִٴ� �������� �ϴ� ����.
//			for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//			{
//				EPCClass ThisClassToLoad = IntToPCClass(PCI);
//
//				TArray<ESkillAnimType> SkillAnimsOfThisClass;
//
//				if (bLoadAll)
//				{
//					AllSkillAnimIDToLoad.Append(ABladeIIGameMode::GetAllPCSkillAnimsOfClass(ThisClassToLoad));
//				}
//				else
//				{
//					// �⺻ ���� ��忡 �ʿ��� �� �̿ܿ� �ɷ�����.
//					if (ThisClassToLoad != BladeIIGameImpl::GetLocalCharacterData().GetMainPlayerClass() &&
//						ThisClassToLoad != BladeIIGameImpl::GetLocalCharacterData().GetSubPlayerClass())
//					{
//						continue;
//					}					
//					UB2SkillRTManager::GetCharacterRegisteredSkillAnimTypes(Inworld, ThisClassToLoad, EB2GameMode::Scenario, SkillAnimsOfThisClass, &BladeIIGameImpl::GetLocalCharacterData());
//					for (ESkillAnimType ThisSkillAnimType : SkillAnimsOfThisClass)
//					{
//						AllSkillAnimIDToLoad.Add(FCombinedPCSkillAnimID(ThisClassToLoad, ThisSkillAnimType));
//					}
//				}
//			}
//
//			SkillAnimInfo->SyncPreloadAssets(AllSkillAnimIDToLoad);
//		}
//		double fEndTime = FPlatformTime::Seconds();
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("TestLoadSkillAnims %d AnimSequences took %f s."), AllSkillAnimIDToLoad.Num(), fEndTime - fStartTime),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 8.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CharDefMat")))
//	{
//		gUseCharacterDefaultMaterial = !gUseCharacterDefaultMaterial;
//		BII_SCREEN_LOG(FString::Printf(TEXT("Character Default Material %s"), gUseCharacterDefaultMaterial ? TEXT("ON") : TEXT("OFF")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//
//		// ���� merge �� BeginPlay �ÿ� DefaultMaterial �� �����ϴµ� �Ｎ���� ���浵 �� �ְ�.. 
//		for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//		{
//			ABladeIICharacter* CastedChar = Cast<ABladeIICharacter>(*ActorIt);
//			if (CastedChar && CastedChar->GetMesh())
//			{ // bHideAllPCs true �� ���� �ѹ� ����� �ٽ� SetActorHiddenInGame(false) �ص� �� ����.
//				USkeletalMeshComponent* MainMeshComp = CastedChar->GetMesh();
//				for (int32 MI = 0; MI < MainMeshComp->GetNumMaterials(); ++MI)
//				{
//					// �̰� ���� ������ Material override �� Ŭ���� �� �ִµ� �Ƹ� �̷��� �ϸ� ���� ���Ǵ� �ɷ� ���ư� �� ������ �ƴ� ���� �ִ�.
//					// ���ϰ� �׽�Ʈ ����� ���翡 �׷��Ա���..
//					MainMeshComp->SetMaterial(MI, gUseCharacterDefaultMaterial ? UMaterial::GetDefaultMaterial(MD_Surface) : nullptr);
//				}
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("AddEther")))
//	{
//		FString IDString;
//		FParse::Token(Cmd, IDString, true);
//
//		const int32 EtherID = FCString::Atoi(*IDString);
//
//		FString TriggerString;
//		FParse::Token(Cmd, TriggerString, true);
//
//		float TriggerRate = FCString::Atof(*TriggerString);
//		TriggerRate = FMath::Clamp(TriggerRate, 0.f, 1.f);
//
//		FString CoolTimeString;
//		FParse::Token(Cmd, CoolTimeString, true);
//
//		float CoolTime = FCString::Atof(*CoolTimeString);
//		FParse::Token(Cmd, CoolTimeString, true);
//
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		if (B2Player)
//			B2Player->AddEtherSetEffect(EtherID, TriggerRate, CoolTime);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PCLightAttach")))
//	{
//		++gTestAttachLightCompMode;
//		if (gTestAttachLightCompMode > 2)
//		{
//			gTestAttachLightCompMode = 0; // 0, 1, 2
//		}
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("AttachLightComp to %s"), (gTestAttachLightCompMode == 0) ? TEXT("Capsule following Bip001") : ((gTestAttachLightCompMode == 1) ? TEXT("Capsule") : TEXT("Rootbone"))),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 8.0f);
//
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		if (B2Player && B2Player->LightComponent)
//		{
//			B2Player->LightComponent->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
//
//			if (gTestAttachLightCompMode == 0 || gTestAttachLightCompMode == 1)
//			{ // ĸ���� ���̴µ� 0 ���� ���� ƽ���� �߰� ����
//				B2Player->LightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, B2Player->LightComponentAttachDistZ));
//				B2Player->LightComponent->AttachToComponent(B2Player->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//			}
//			else if(gTestAttachLightCompMode == 2)
//			{
//				// ��Ʈ���� �׳�
//				B2Player->LightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, B2Player->LightComponentAttachDistZ + ZOffsetFromMatchingSKActorToBIICharacter(B2Player)));
//				B2Player->LightComponent->AttachToComponent(B2Player->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), B2PLAYER_ROOTBONE_NAME);
//			}
//			else
//			{
//				checkSlow(0);
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PCLight")))
//	{
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//
//		if (FirstArg == TEXT("Intensity"))
//		{
//			FString SecondArg;
//			FParse::Token(Cmd, SecondArg, true);
//			float WantedIntensity = FCString::Atof(*SecondArg);
//
//			if (B2Player && B2Player->LightComponent)
//			{
//				B2Player->LightComponent->SetIntensity(WantedIntensity);
//			}
//		}
//		else
//		{
//			// ù ���� ���� �׳� On/Off
//			gbTestTurnOffPlayerLight = !gbTestTurnOffPlayerLight;
//			
//			if (B2Player)
//			{
//				// �׳� ���� ��Ƽ� �� �ϴ� ��.
//				B2Player->FormalSetPointLightOn(!gbTestTurnOffPlayerLight);
//			}
//			UE_LOG(LogBladeII, Display, TEXT("LocalPlayerCharacter Light %s"), gbTestTurnOffPlayerLight ? TEXT("Off") : TEXT("On"));
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PlayActOpen")))
//	{
//		UB2UIChapterFrame* ChapterUI = UB2UIManager::GetInstance()->GetUI<UB2UIChapterFrame>(UIFName::Chapter);
//		if (ChapterUI)
//		{
//			ChapterUI->PlayActOpen();
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PlayActClear")))
//	{
//		UB2UIChapterFrame* ChapterUI = UB2UIManager::GetInstance()->GetUI<UB2UIChapterFrame>(UIFName::Chapter);
//		if (ChapterUI)
//		{
//			ChapterUI->PlayActClear();
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("AllCamAdaptiveFOV")))
//	{
//		static bool bWasSetAdaptiveFOV = false;
//
//		bWasSetAdaptiveFOV = !bWasSetAdaptiveFOV;
//		// AspectRatioAdaptiveFOV �� ���� ������ �׽�Ʈ.
//		for (TObjectIterator<UCameraComponent> CamIT; CamIT; ++CamIT)
//		{
//			UCameraComponent* ThisComp = Cast<UCameraComponent>(*CamIT);
//			if (ThisComp)
//			{
//				if (bWasSetAdaptiveFOV)
//				{ // AspectRatioAdaptiveFOV �� ����Ϸ��� �ϴ� Constraint �� ���� �ʾƾ�.
//					ThisComp->SetConstraintAspectRatio(false);
//				}
//				ThisComp->SetAspectRatioAdaptiveFOV(bWasSetAdaptiveFOV);
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SlateUIAspectRatio")))
//	{
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		FString SecondArg;
//		FParse::Token(Cmd, SecondArg, true);
//		float SecondArgFloat = FCString::Atof(*SecondArg);
//
//		if (FirstArg == TEXT("Min"))
//		{
//			GEngine->SlateUIMinAspectRatio = SecondArgFloat;
//		}
//		else if (FirstArg == TEXT("Max"))
//		{
//			GEngine->SlateUIMaxAspectRatio = SecondArgFloat;
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("DefCamAspectRatio")))
//	{
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		FString SecondArg;
//		FParse::Token(Cmd, SecondArg, true);
//		float SecondArgFloat = FCString::Atof(*SecondArg);
//
//		if (FirstArg == TEXT("Min"))
//		{
//			GEngine->DefaultCameraMinAspectRatio = SecondArgFloat;
//		}
//		else if (FirstArg == TEXT("Max"))
//		{
//			GEngine->DefaultCameraMaxAspectRatio = SecondArgFloat;
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CheckMobileShaderCompile")))
//	{
//		GDevCheckMobileShaderCompile = !GDevCheckMobileShaderCompile;
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("CheckMobileShaderCompile %s."), GDevCheckMobileShaderCompile ? TEXT("On") : TEXT("Off")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CheckSoundWaveLoad")))
//	{
//		GDevCheckSoundWaveLoading = !GDevCheckSoundWaveLoading;
//		BII_SCREEN_LOG(FString::Printf(TEXT("CheckSoundWaveLoading %s."), GDevCheckSoundWaveLoading ? TEXT("On") : TEXT("Off")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TexGroupLODBias")))
//	{
//		FString FirstArg; // LODGroup ����
//		FParse::Token(Cmd, FirstArg, true);
//		FString SecondArg; // LODBias �� �󸶳� �������� ����
//		FParse::Token(Cmd, SecondArg, true);
//
//		TEnumAsByte<enum TextureGroup> WantedLODGroup = TextureGroup::TEXTUREGROUP_MAX;
//		// UEnum �δٰ� enum ���ڿ��� �״�� ������ ���� �ְ����� �׳� �׽�Ʈ Ŀ�ǵ��̴� �ʿ��� �͸� �ܼ���...
//		if (FirstArg == TEXT("World")) {
//			WantedLODGroup = TextureGroup::TEXTUREGROUP_World;
//		}
//		else if (FirstArg == TEXT("WorldN")) {
//			WantedLODGroup = TextureGroup::TEXTUREGROUP_WorldNormalMap;
//		}
//		else if (FirstArg == TEXT("WorldS")) {
//			WantedLODGroup = TextureGroup::TEXTUREGROUP_WorldSpecular;
//		}
//		else if (FirstArg == TEXT("Char")) {
//			WantedLODGroup = TextureGroup::TEXTUREGROUP_Character;
//		}
//		else if (FirstArg == TEXT("CharN")) {
//			WantedLODGroup = TextureGroup::TEXTUREGROUP_CharacterNormalMap;
//		}
//		else if (FirstArg == TEXT("CharS")) {
//			WantedLODGroup = TextureGroup::TEXTUREGROUP_CharacterSpecular;
//		}
//		// TEXTUREGROUP_MAX �� ��ü �� �ϴ� �ɷ�?
//
//		int32 WantedLODBias = FCString::Atoi(*SecondArg);
//
//		UTextureLODSettings* LODSettingInfo = UDeviceProfileManager::Get().GetActiveProfile()->GetTextureLODSettings();
//		if (LODSettingInfo)
//		{
//			if (WantedLODGroup == TEXTUREGROUP_MAX)
//			{
//				for (FTextureLODGroup& ThisGroupInfo : LODSettingInfo->TextureLODGroups)
//				{
//					ThisGroupInfo.LODBias = WantedLODBias;
//				}
//				BII_SCREEN_LOG(FString::Printf(TEXT("Adjusted LODBias of all LODGroups to %d"), WantedLODBias),
//					FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//			}
//			else
//			{
//				FTextureLODGroup& WantedLODGroupInfo = LODSettingInfo->TextureLODGroups[(int32)WantedLODGroup];
//				WantedLODGroupInfo.LODBias = WantedLODBias;
//				BII_SCREEN_LOG(FString::Printf(TEXT("Adjusted LODBias of LODGroup %d by %d"), (int32)WantedLODGroup, WantedLODBias),
//					FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//			}
//		}
//
//		for (TObjectIterator<UTexture> TIT; TIT; ++TIT)
//		{
//			UTexture* ThisTexture = Cast<UTexture>(*TIT);
//			if (ThisTexture)
//			{
//				ThisTexture->UpdateCachedLODBias();
//				ThisTexture->UpdateResource();
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("RemielShow")))
//	{
//		FString FirstArg; // � �ִϸ��̼��� ����.. ELobbyRelaxingDoumiState �� int ĳ����
//		FParse::Token(Cmd, FirstArg, true);
//		int32 FirstArgInt = FCString::Atoi(*FirstArg);
//		ELobbyRelaxingDoumiState WantedAnimState = static_cast<ELobbyRelaxingDoumiState>(FirstArgInt);
//		if (WantedAnimState == ELobbyRelaxingDoumiState::ELRD_End)
//		{
//			WantedAnimState = ELobbyRelaxingDoumiState::ELRD_IdleLoop;
//		}
//
//		AB2LobbyRelaxingDoumi* FoundRemielShowGirl = nullptr;
//		for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//		{ 
//			AB2LobbyRelaxingDoumi* CastedRelaxingDoumi = Cast<AB2LobbyRelaxingDoumi>(*ActorIt);
//			if (CastedRelaxingDoumi)
//			{
//				FoundRemielShowGirl = CastedRelaxingDoumi;
//				break;;
//			}
//		}
//		if (FoundRemielShowGirl)
//		{
//			if (WantedAnimState == ELobbyRelaxingDoumiState::ELRD_Welcoming ||
//				WantedAnimState == ELobbyRelaxingDoumiState::ELRD_RewardNormal ||
//				WantedAnimState == ELobbyRelaxingDoumiState::ELRD_RewardDouble ||
//				WantedAnimState == ELobbyRelaxingDoumiState::ELRD_TouchReaction)
//			{
//				FoundRemielShowGirl->PlayTemporaryAnimOfState(WantedAnimState);
//			}
//			else if (WantedAnimState == ELobbyRelaxingDoumiState::ELRD_IdleLoop)
//			{
//				FoundRemielShowGirl->BeginDefaultPose();
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ShopGirlShow")))
//	{
//		FString FirstArg; // � �ִϸ��̼��� ����.. ELobbyShopGirlState �� int ĳ����
//		FParse::Token(Cmd, FirstArg, true);
//		int32 FirstArgInt = FCString::Atoi(*FirstArg);
//		ELobbyShopGirlState WantedAnimState = static_cast<ELobbyShopGirlState>(FirstArgInt);
//		if (WantedAnimState == ELobbyShopGirlState::ELSG_End)
//		{
//			WantedAnimState = ELobbyShopGirlState::ELSG_IdleLoop;
//		}
//
//		AB2LobbyShopGirl* FoundShopGirl = nullptr;
//		for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//		{
//			AB2LobbyShopGirl* CastedShopGirl = Cast<AB2LobbyShopGirl>(*ActorIt);
//			if (CastedShopGirl)
//			{
//				FoundShopGirl = CastedShopGirl;
//				break;;
//			}
//		}
//		if (FoundShopGirl)
//		{
//			if (WantedAnimState == ELobbyShopGirlState::ELSG_Welcoming ||
//				WantedAnimState == ELobbyShopGirlState::ELSG_CheerNormal ||
//				WantedAnimState == ELobbyShopGirlState::ELSG_CheerMuch ||
//				WantedAnimState == ELobbyShopGirlState::ELSG_TouchReaction)
//			{
//				FoundShopGirl->PlayTemporaryAnimOfState(WantedAnimState);
//			}
//			else if (WantedAnimState == ELobbyShopGirlState::ELSG_IdleLoop)
//			{
//				FoundShopGirl->BeginDefaultPose();
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("LobbyVerticalScreen")))
//	{
//		FString OnOffStr;
//		FParse::Token(Cmd, OnOffStr, true);
//
//		bool bVertical = false;
//		if (OnOffStr == TEXT("On") || OnOffStr == TEXT("True") || OnOffStr == TEXT("1"))
//			bVertical = true;
//		else
//			bVertical = false;
//
//		LobbyCharObserveChangeScreenClass<bool>::GetInstance().Signal(bVertical);
//	}
//	else if (FParse::Command(&Cmd, TEXT("ItemInfoAsyncLoadTest")))
//	{
//		FString PCClassStr;
//		FParse::Token(Cmd, PCClassStr, true);
//		EPCClass WantedClass = IntToPCClass(FMath::Clamp(FCString::Atoi(*PCClassStr), 0, GetMaxPCClassNum() - 1));
//
//		FString GradeStr;
//		FParse::Token(Cmd, GradeStr, true);
//		int32 WantedGrade = FMath::Clamp(FCString::Atoi(*GradeStr), 1, FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE);
//
//		TArray<FB2Item> RequiredPartItems;
//		GetStandardDummyPCPartItemData(WantedClass, WantedGrade, RequiredPartItems);
//
//		UB2ItemInfo* ItemInfoTable = StaticFindItemInfo();
//		if (ItemInfoTable && RequiredPartItems.Num() > 0)
//		{
//			TArray<int32> WantedRefIds;
//			for (FB2Item& ThisItemData : RequiredPartItems)
//			{
//				WantedRefIds.Add(ThisItemData.ItemRefID);
//			}
//			FString RequestedName;
//			if (ItemInfoTable->TryAsyncLoadOfEquipParts(WantedRefIds, RequestedName, true))
//			{
//				// �ε� �Ϸ� �� �ڵ鸵 ����.
//				ItemInfoAsyncLoadTestObj.MarkAsyncLoadWaiting(WantedClass, WantedGrade, RequestedName);
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PLOnOff")))
//	{
//		// Ư�� �÷����� ������ ���� ���� ��.
//
//		FString OnOffStr;
//		FParse::Token(Cmd, OnOffStr, true);
//
//		bool bSetOn = false;
//		if (OnOffStr == TEXT("On") || OnOffStr == TEXT("True") || OnOffStr == TEXT("1"))
//			bSetOn = true;
//		else
//			bSetOn = false;
//
//		int32 ChangedCount = 0;
//		for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//		{
//			APointLight* ThisPL = Cast<APointLight>(*ActorIt);
//			UPointLightComponent* ThisPLComp = ThisPL ? Cast<UPointLightComponent>(ThisPL->GetLightComponent()) : nullptr;
//			if (ThisPL && ThisPLComp && ThisPLComp->Mobility == EComponentMobility::Movable)
//			{
//				ThisPLComp->bAffectsWorld = bSetOn;
//				ThisPLComp->MarkRenderStateDirty();
//				ChangedCount++;
//			}
//		}
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("%d PointLight AffectsWorld changed to %d"), ChangedCount, (int32)bSetOn),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("DirOnOff")))
//	{
//		// Ư�� �÷����� ������ ���� ���� ��.
//
//		FString OnOffStr;
//		FParse::Token(Cmd, OnOffStr, true);
//
//		bool bSetOn = false;
//		if (OnOffStr == TEXT("On") || OnOffStr == TEXT("True") || OnOffStr == TEXT("1"))
//			bSetOn = true;
//		else
//			bSetOn = false;
//
//		int32 ChangedCount = 0;
//		for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//		{
//			ADirectionalLight* ThisDL = Cast<ADirectionalLight>(*ActorIt);
//			UDirectionalLightComponent* ThisDLComp = ThisDL ? Cast<UDirectionalLightComponent>(ThisDL->GetLightComponent()) : nullptr;
//			if (ThisDL && ThisDLComp && (ThisDLComp->Mobility == EComponentMobility::Movable || ThisDLComp->Mobility == EComponentMobility::Stationary))
//			{
//				ThisDLComp->bAffectsWorld = bSetOn;
//				ThisDLComp->MarkRenderStateDirty();
//				ChangedCount++;
//			}
//		}
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("%d DirectionalLight AffectsWorld changed to %d"), ChangedCount, (int32)bSetOn),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("DOFTest")))
//	{
//		static float DOFTest_Scale = -1.0f;
//		static float DOFTest_FocalDist = -1.0f;
//		static float DOFTest_FocalRegion = -1.0f;
//		static float DOFTest_NearTransitionRegion = -1.0f;
//		static float DOFTest_FarTransitionRegion = -1.0f;
//		static float DOFTest_Occlusion = -1.0f;
//		static float DOFTest_Vignette = -1.0f;
//
//		FParse::Value(Cmd, TEXT("Scale="), DOFTest_Scale);
//		FParse::Value(Cmd, TEXT("FD="), DOFTest_FocalDist);
//		FParse::Value(Cmd, TEXT("FR="), DOFTest_FocalRegion);
//		FParse::Value(Cmd, TEXT("NTR="), DOFTest_NearTransitionRegion);
//		FParse::Value(Cmd, TEXT("FTR="), DOFTest_FarTransitionRegion);
//		FParse::Value(Cmd, TEXT("OCC="), DOFTest_Occlusion);
//		FParse::Value(Cmd, TEXT("Vig="), DOFTest_Vignette);
//
//		UCameraComponent* CurrCamComp = GetCurrentActiveCamComp(Inworld);
//		if (CurrCamComp)
//		{
//			if (DOFTest_Scale >= 0.0f)
//			{
//				CurrCamComp->PostProcessSettings.bOverride_DepthOfFieldScale = true;
//				CurrCamComp->PostProcessSettings.DepthOfFieldScale = DOFTest_Scale;
//			}
//			if (DOFTest_FocalDist >= 0.0f)
//			{
//				CurrCamComp->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
//				CurrCamComp->PostProcessSettings.DepthOfFieldFocalDistance = DOFTest_FocalDist;
//			}
//			if (DOFTest_FocalRegion >= 0.0f)
//			{
//				CurrCamComp->PostProcessSettings.bOverride_DepthOfFieldFocalRegion = true;
//				CurrCamComp->PostProcessSettings.DepthOfFieldFocalRegion = DOFTest_FocalRegion;
//			}
//			if (DOFTest_NearTransitionRegion >= 0.0f) 
//			{
//				CurrCamComp->PostProcessSettings.bOverride_DepthOfFieldNearTransitionRegion = true;
//				CurrCamComp->PostProcessSettings.DepthOfFieldNearTransitionRegion = DOFTest_NearTransitionRegion;
//			}
//			if (DOFTest_FarTransitionRegion >= 0.0f)
//			{
//				CurrCamComp->PostProcessSettings.bOverride_DepthOfFieldFarTransitionRegion = true;
//				CurrCamComp->PostProcessSettings.DepthOfFieldFarTransitionRegion = DOFTest_FarTransitionRegion;
//			}
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("Applied DOF Test Param. Scale %.2f, FocalDist %.2f, FocalReg %.2f, NearTransReg %.2f, FarTransReg %.2f"),
//				DOFTest_Scale, DOFTest_FocalDist, DOFTest_FocalRegion, DOFTest_NearTransitionRegion, DOFTest_FarTransitionRegion),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//
//
//			// ���� ���Ǽ��� ������ ������ �׷��� ��͵� ���� ����.
//			if (DOFTest_Occlusion >= 0.0f)
//			{
//				CurrCamComp->PostProcessSettings.bOverride_DepthOfFieldOcclusion = true;
//				CurrCamComp->PostProcessSettings.DepthOfFieldOcclusion = DOFTest_Occlusion;
//			}
//			if (DOFTest_Vignette >= 0.0f)
//			{
//				CurrCamComp->PostProcessSettings.bOverride_DepthOfFieldVignetteSize = true;
//				CurrCamComp->PostProcessSettings.DepthOfFieldVignetteSize = DOFTest_Vignette;
//			}
//
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestCombatMode")))
//	{
//		FSoftObjectPath TestWidgetAsset;
//		TestWidgetAsset.SetPath("/Game/UI/DevOnlyWidget/CombatTestWidget.CombatTestWidget");
//
//		if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
//		{
//			UBlueprint* CombatWidgetBP = Cast<UBlueprint>(TestWidgetAsset.TryLoad());
//			if (CombatWidgetBP != nullptr)
//			{
//				UB2UIWidget* BattleMain = UIManager->GetUI<UB2UIWidget>(UIFName::BattleMain);
//				BattleMain->DestroySelf(UIManager);
//
//				UB2UIWidget* Widget = UIManager->CreateWidget<UB2UIWidget>(Inworld, UIManager, CombatWidgetBP->GeneratedClass);
//				if (Widget)
//				{
//					Widget->Init();
//					Widget->AddToViewport();
//				}
//			}
//
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PlayRecord")))
//	{
//		FString RecordId;
//		FParse::Token(Cmd, RecordId, true);
//
//		if (auto* DocSome = UB2UIDocHelper::GetDocSome())
//			DocSome->ReplayFileName = RecordId;
//		
//		auto* GameMode = Cast<AGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		FString ReplayMapName(TEXT("/Game/Maps/ReplayTest"));
//		UGameplayStatics::OpenLevel(GameMode, FName(*ReplayMapName), true);
//		
//		return true;
//	}
//    else if (FParse::Command(&Cmd, TEXT("PlayRecordURL")))
//    {
//        FString URL;
//        FParse::Token(Cmd, URL, true);
//
//        if (auto* DocSome = UB2UIDocHelper::GetDocSome())
//        {
//            auto* GameMode = Cast<AGameMode>(UGameplayStatics::GetGameMode(Inworld));
//            FModReplayInfo ReplayInfo{ URL, 0, 0 };
//            StartBladeIIPVPReplay(GameMode, ReplayInfo);
//        }
//    }
//	else if (FParse::Command(&Cmd, TEXT("TotemRaffleTest")))
//	{
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		int32 FirstArgInt = FCString::Atoi(*FirstArg);
//
//		UB2UITotemRaffle *UIItemRaffle = UB2UIManager::GetInstance()->OpenUI<UB2UITotemRaffle>(UIFName::TotemRaffle, true);
//
//
//		if (UIItemRaffle)
//		{
//			TArray<FB2Totem> TotemInfo;
//
//			UB2LobbyInventory::GetAllTotemList(TotemInfo);
//			UIItemRaffle->StartFromRaffle(TotemInfo[FirstArgInt]);
//		}
//	}
//	else if (FParse::Command(&Cmd, TEXT("RequestCombatPower")))
//	{
//		FString PCClassStr;
//		FParse::Token(Cmd, PCClassStr, true);
//		EPCClass SpecifiedPCClass = EPCClass::EPC_End;
//		if (PCClassStr == TEXT("0") || PCClassStr == TEXT("Gl") || PCClassStr == TEXT("Gladiator"))
//		{
//			SpecifiedPCClass = EPCClass::EPC_Gladiator;
//		}
//		else if (PCClassStr == TEXT("1") || PCClassStr == TEXT("As") || PCClassStr == TEXT("Assassin"))
//		{
//			SpecifiedPCClass = EPCClass::EPC_Assassin;
//		}
//		else if (PCClassStr == TEXT("2") || PCClassStr == TEXT("Wi") || PCClassStr == TEXT("Wizard"))
//		{
//			SpecifiedPCClass = EPCClass::EPC_Wizard;
//		}
//		else if (PCClassStr == TEXT("3") || PCClassStr == TEXT("Fi") || PCClassStr == TEXT("Fighter"))
//		{
//			SpecifiedPCClass = EPCClass::EPC_Fighter;
//		}
//
//		RequestCombatPowerClass<EPCClass>::GetInstance().Signal(SpecifiedPCClass);
//	}

	return false;
}
#endif
