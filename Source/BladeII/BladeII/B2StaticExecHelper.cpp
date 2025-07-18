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
	bIsAllowedForShipping = true; // 우리 커맨드만 쉽핑빌드서 허용.. 물론 완전 최종 오픈버전에서 BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1 도 0 으로 되면 짤없이 다 빠짐.
#endif
}

bool UB2StaticExecHelper::Exec(UWorld* Inworld, const TCHAR* Cmd, FOutputDevice& Ar)
{
#if B2_BUILD_LEVEL == B2_BUILD_LEVEL_LIVE // B2_BUILD_LEVEL_LIVE
	// 쉽핑 버전에서 콘솔창이 나가는 경우라도 (시연 등) 이 이하로 붙은 커맨드는 실행되지 않도록 한다. 엔진 커맨드 포함.
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
//	// 여긴 가끔식 shipping 성격의 빌드에도 포함될 필요가 있는 보다 정식 기능을 구현할 때 사용.
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	FString CultureName;
//#endif
//
//	if (FParse::Command(&Cmd, TEXT("ServerCmd")))
//	{
//		return data_trader::ExecHandler::GetInstance().ExecuteShellCommand(Cmd);
//	}
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // ServerCmd 외에는 LV2 로 간주함
//	else if (FParse::Command(&Cmd, TEXT("CurrentMemStat")) || FParse::Command(&Cmd, TEXT("CMS"))) // Shipping 빌드에서 체크할 일이 있어서 여기로 옮겨온다.
//	{
//		FGenericPlatformMemory::DumpStats(Ar);
//
//		// FGenericPlatformMemory::DumpStats 코드 가져와서 동일한 정보를 화면에도 뿌려줌
//		const float InvMB = 1.0f / 1024.0f / 1024.0f;
//		FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
//		const float DrawTextTime = 20.0f;
//		const int32 FontSize = 14;
//		// 보기 원하는 역순으로 넣는다. 줄 구분을 위해 한줄 걸러서 색상 좀 다르게
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
//		else { // 아무것도 지정 안하면 그냥 On/Off 
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
//	else if (FParse::Command(&Cmd, TEXT("SuperHero"))) // 빨리빨리 쳐부수며 테스트하고 싶을 때 간편하게..
//	{
//		//ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		//if (B2Player)
//		//{
//		//	bool bCurrentlyInSuperHero = (B2Player->bCheatHeroMode && B2Player->bCheatImmortalMode); // Zombie 와 Hero 가 동시에 설정되어 있으면 On 상태인 걸로 간주하고 끈다.
//
//		//	bCurrentlyInSuperHero = !bCurrentlyInSuperHero;
//
//		//	// 여기서부터 본격 세팅. Hero, Zombie, Cooltime 죄다.
//		//	B2Player->bCheatHeroMode = bCurrentlyInSuperHero;
//		//	B2Player->bCheatImmortalMode = bCurrentlyInSuperHero;
//
//		//	UB2SkillRTManager* SkillRTManager = B2Player->GetSkillRTManager();
//		//	if (SkillRTManager)
//		//	{ // 치트면 1초, 아니면 기본값
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
//		{ // 로비 메쉬들 리프레쉬. 원래 LobbyUpdateCharacterPartClass 시그널은 강제 업데이트는 아니므로 이 치트가 바로 안 먹힐 수 있는데 이 치트 사용 시 안쪽에서 처리하도록 하였음.
//			LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(PCI));
//		}
//		// 인게임이면 태그를 다시 하던지.. 아 메쉬가 캐싱이 되었으니 그냥 레벨 다시 로딩해야겠다.
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CombatStatScale")))
//	{ // 로컬 플레이어 캐릭터에만 먹히는 거
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
//	{ // 단독 플레이 모드를 위한 치트키 모음
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
//			UB2PreRenderer::ClearCompleteState(); // 다시 Pre-render 유발
//			BII_SCREEN_LOG(FString::Printf(TEXT("PreRenderState Clear")), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstTokenStr == TEXT("FarAway")) // 처음 걸 FarAway 로 주면 걍 이거만 세팅
//		{
//			UB2PreRenderer::bPreRenderAtFarAway = !UB2PreRenderer::bPreRenderAtFarAway;
//			UE_LOG(LogBladeII, Display, TEXT("PreRenderAtFarAway %d"), (int32)UB2PreRenderer::bPreRenderAtFarAway);
//			BII_SCREEN_LOG(FString::Printf(TEXT("PreRenderAtFarAway %d"), (int32)UB2PreRenderer::bPreRenderAtFarAway), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstTokenStr == TEXT("All")) // 모든 프리미티브 컴포넌트들을 Pre-render 하는 옵션
//		{
//			UB2PreRenderer::bPreRenderAllPrimitives = !UB2PreRenderer::bPreRenderAllPrimitives;
//			UE_LOG(LogBladeII, Display, TEXT("bPreRenderAllPrimitives %d"), (int32)UB2PreRenderer::bPreRenderAllPrimitives);
//			BII_SCREEN_LOG(FString::Printf(TEXT("bPreRenderAllPrimitives %d"), (int32)UB2PreRenderer::bPreRenderAllPrimitives), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstTokenStr == TEXT("SpawnDist")) // 처음 걸 SpawnDist 로 주면 더미들 spawn 할 거리 설정
//		{
//			FString DistStr;
//			FParse::Token(Cmd, DistStr, true);
//			UB2PreRenderer::PreRenderSpawnDistFromView = FCString::Atof(*DistStr);
//			UE_LOG(LogBladeII, Display, TEXT("PreRenderSpawnDistFromView %f"), UB2PreRenderer::PreRenderSpawnDistFromView);
//			BII_SCREEN_LOG(FString::Printf(TEXT("PreRenderSpawnDistFromView %d"), UB2PreRenderer::PreRenderSpawnDistFromView), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else
//		{
//			// 아닌 경우는 지정한 숫자대로 기준 횟수들 세팅
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
//	else if (FParse::Command(&Cmd, TEXT("PCSectionMerge"))) // 테스트 도중에만 제공.. 내지는 정식으로 구현된 이후에도 성능 테스트를 위한 On/Off 플래그가 될 수도.
//	{
//		++UB2PCMeshSectionMergeInfo::SectionMergeTestFlag; // 1, 2 가 강제 사용 혹은 Off
//		if (UB2PCMeshSectionMergeInfo::SectionMergeTestFlag > 2) {
//			UB2PCMeshSectionMergeInfo::SectionMergeTestFlag = 0;
//		}
//
//		UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
//		// 이 치트가 실제 적용되는 게 좀 안쪽이라 캐쉬 상태를 비워야 한다.
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
//				// 위에서 CompoiteMeshCache 를 비우니 LobbyUpdateCharacterPartClass 로도 어느 정도까지는 강제 업데이트가 되는데 안될때도 있다.
//				// 이런 상황에서 쓸 이벤트를 따로 만들기는 뭐하니 그냥 LobbyGameMode 접근해서 실행하자..
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
//	else if (FParse::Command(&Cmd, TEXT("KillAllEnemy"))) // 적다죽임
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
//		//	// 파싱한 인자들과 함께 dummy player spawn.
//		//	ABladeIITestDummyPlayer* SpawnedDummy = ABladeIITestDummyPlayer::SpawnWithStandardEquipment(
//		//		Inworld, SpawnClass, NewTransform, EquipGrade, ParsedSpawnFloatingInterval, ParsedSpawnDamageFxInterval,
//		//		bHasWing, MAX_WING_EVOLUTION_GRADE, true // 날개 등급은 걍 여기서..
//		//	);
//
//		//	if (SpawnedDummy)
//		//	{
//		//		if (ParsedAnimStateString == TEXT("Rand") || ParsedAnimStateString == TEXT("Random"))
//		//		{ // 랜덤하게 플레이
//		//			SpawnedDummy->SetUseRandomAttackState(true);
//		//		}
//		//		else
//		//		{ // 정해놓고 하나 플레이
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
//		// Off라고 명시하지않으면 그냥 진행
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
//		// 치트를 써가면서 플레이 하는 것의 영향인지 몰라도 어떻게 해도 튜토리얼 창이 안 닫힐 때가 있다. 응급 조치
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
//	else if (FParse::Command(&Cmd, TEXT("ALLog"))) // AssetLoaderLog 혹은 AsyncLoadLog 뭐라고 해석하든 ㅋ
//	{
//		UB2AssetLoader::bEnableDevLog = !UB2AssetLoader::bEnableDevLog;
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("B2AssetLoader Dev Logging %s"), UB2AssetLoader::bEnableDevLog ? TEXT("ON") : TEXT("OFF")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("NPCAsyncLimit"))) // 이미 r.PostGameLoadNPCAsyncLoadLimit 콘솔명령이지만 Shipping 에서 테스트하려고.
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
//	else if (FParse::Command(&Cmd, TEXT("ForcedCSMDist"))) // 이미 r.Shadow.EnforcedStationaryCSMDistance 콘솔명령이지만 Shipping 에서 테스트하려고. 바로 갱신 필요도 있고.
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
//			// 이게 바로 적용되려면 DirectionalLightComponent 들을 한차례 업데이트 해 주어야..
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
//	else if (FParse::Command(&Cmd, TEXT("TestModShadowColor"))) // 이건 명령 넣고 바로 업데이트 되는 거 보려고..
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
//			// 이게 바로 적용되려면 DirectionalLightComponent 들을 한차례 업데이트 해 주어야..
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
//		// 기기에 따라 감지된 값으로 성능 옵션 세팅
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
//		// GetActiveProfileName 중간 결과들도 출력하면 보다 도움이 될 듯.
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
//			//FString VulkanVersion = FAndroidMisc::GetVulkanVersion(); 됐고 ㅋ
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
//		// FIOSPlatformMisc 에 필요한 정보가 있을 거 같은데.. 실제 실행 및 기능 확인 하기 전까지는..
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
//		// 정해진 InfoAsset 을 로딩하는 성능 테스트
//		// 로딩이 안 된 상태에서 해야 함.
//
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		if (FirstArg == TEXT("UI"))
//		{
//			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
//
//			//UIMgrInst->UnloadAllTAssets(); 이거 하고 바로 GC 돌릴 수 있으면 하나 가지고 여러번 로딩하는 식이 될 수도..
//
//			double LoadBeginTime = FPlatformTime::Seconds();
//
//			// 로딩 후 루트셋에 넣었다거나 캐싱이 된 건 이걸로 정확한 테스트가 불가능. 대상은 적절히 선정..
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
//		/** CBTVersionReady 가 true 인 상태로 쿠킹 테스트할 때 컨텐츠가 막혀서 당황스러운 상황 대비. */
//		extern bool gbAllowNonCBTVersionContentOverride;
//		gbAllowNonCBTVersionContentOverride = !gbAllowNonCBTVersionContentOverride;
//
//		UB2StageInfo* StageInfoTable = StaticFindStageInfo();
//		if (StageInfoTable)
//		{ // 갱신해 주어야 하는 게 있어서..
//			StageInfoTable->SetDataByCBTVersionContent();
//		}
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("AllowNonCBTVersionContentOverride %s."), gbAllowNonCBTVersionContentOverride ? TEXT("On") : TEXT("Off")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TAATest"))) // TemporalAA On/Off 테스트 용
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
//	else if (FParse::Command(&Cmd, TEXT("FXAATest"))) // FXAA On/Off 테스트 용.. 근데 이거 모바일서 안 먹힘?
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
//		// 현재 구현이 딴 거 필요없이 r.MobileMSAA 이거 하나로 다 먹히게 되어 있음.
//		static IConsoleVariable* _CVarMobileMSAA = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileMSAA"));
//		if (_CVarMobileMSAA)
//		{
//			const int32 CurMSAAValue = _CVarMobileMSAA->GetInt();
//			int32 NewMSAAValue = FCString::Atoi(*FirstArg);
//			if (!bHasFirstArg)
//			{
//				// 지정하지 않았다면 On/Off 수준으로.. 기본 샘플값은 4로 함.
//				NewMSAAValue = ((CurMSAAValue <= 1) ? 4 : 1);
//			}
//			else if (NewMSAAValue != 1 && NewMSAAValue != 2 && NewMSAAValue != 4 && NewMSAAValue != 8)
//			{ // 예외처리.
//				NewMSAAValue = 1; // 1 이 끄는 거.
//			}
//
//			// %%%%%%%%%%%%%%%%%%%%%
//			// 여기서 세팅한 건 B2Scalability 쪽 컨트롤로 인해 다음 로딩 세션 넘어서까지 지속되지 않을 것이다.
//			// 이 커맨드로 상세한 테스트를 하려면 B2Scalability 쪽에서의 제어를 꺼 놓도록.
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
//		// EB2ResolutionLevel 은 UI 단을 위한 인터페이스이고 EB2GraphicsRQType 을 현재 허용 여부 관계없이 강제로 세팅하는 기능이 있으면 좋을 듯.
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
//			// AdjustScalabilityBySavedUserSelection 에서 저장한 값을 불러올 테니 저장이 필요..
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
//		// PC 버전 CVar 는 따로 있음. 이건 모바일 쉽핑 테스트 배포에서 쓰기 위함.
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
//			{ // 모바일에선 역할을 못할 듯..
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
//		// 숫자 두 개 명시하면 Lower / Upper 로, 한 개만 명시하면 Upper 로
//		float SpecifiedUpperBound = (fSecondValue > 0.0f) ? fSecondValue : fFirstValue;
//		float SpecifiedLowerBound = (fSecondValue > 0.0f) ? fFirstValue : 0.0f;
//
//		if (GEngine && SpecifiedUpperBound > 0.0f)
//		{
//			// 테스트 용이니 그냥 다 똑같이 바꿔침. 이렇게 하고 나면 r.CurrentMobileFrameLimitLevel 가 의미가 없겠지 ㅋ
//
//			// 실제 준비되어 있는 SmoothedFrameRateRange_Mobile 세팅보다 큰 인덱스에서는 제일 큰 걸 사용하므로 확장 가능성을 생각해서 적당히 큰 숫자까지 iteration 함.
//			for (int32 SFI = 0; SFI < 10; ++SFI) 
//			{
//				// 내부적으로 0 이나 음수에서는 현재 값을 유지하게 될 것.
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
//	{ // GEngine->SmoothedFrameRateRange_Mobile** 들의 값을 직접 건드리지 않고 
//		// 실제 의도한 기능대로 SmoothedFrameRateRange_Mobile** 중 하나를 선택하는 레벨 변수를 변경.
//		IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.CurrentMobileFrameLimitLevel"));
//		if (CVar)
//		{
//			FString FirstArg;
//			FParse::Token(Cmd, FirstArg, true);
//
//			if (FirstArg == TEXT("Low") || FirstArg == TEXT("Mid") || FirstArg == TEXT("High"))
//			{ // 게임 메뉴에서 사용할 의도로 제공되는 상대적인 값으로의 세팅 인터페이스
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
//			{ // 직접 값 세팅
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
//		// 간혹 눈에 띄는 문제에 대한 테스트 차원인데.. 이게 해결 방안이 되는 게 맞으면 정식 채택.
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
//	{ // 네트워크 연결 안된 상태에서 로비 특정 장면으로 이동하기 위해 제공.
//		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::CharacterSelectPage);
//		return true;
//	}	
//	else if (FParse::Command(&Cmd, TEXT("GoToLobbyMain")))
//	{ // 네트워크 연결 안된 상태에서 로비 특정 장면으로 이동하기 위해 제공.
//		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::LobbyMain);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GoToLobbyShop")))
//	{ // 네트워크 연결 안된 상태에서 로비 특정 장면으로 이동하기 위해 제공.
//		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_SummonItem);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GoToLobbyInventory")))
//	{ // 네트워크 연결 안된 상태에서 로비 특정 장면으로 이동하기 위해 제공.
//		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PrepareLobbyHallOfFame")))
//	{ // 그냥은 못들어가지는데 정보를 좀 채워넣어야.. 이건 클라 단독은 아니고 서버 붙은 상황에서.
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddRankingStatus 2 3 10 100"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddRankingStatus 3 3 10 100"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddRankingStatus 5 3 10 100"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("forceSettlement 2"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("forceSettlement 3"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("forceSettlement 5"));
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestStage")))
//	{ // Standalone 스테이지 플레이를 좀 더 간편히 하기 위해. 외부 전달 테스트 빌드에서도 필요하기도 하고..
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		const int32 WantedStageNum = FCString::Atoi(*FirstArg);
//
//		FString SecondArg;
//		FParse::Token(Cmd, SecondArg, true);
//		const bool bScenarioModeOn = (SecondArg == TEXT("-SM") || SecondArg == TEXT("-Scenario"));
//
//		// ScenarioMode 를 지정하면 난이도는 무조건 Normal 이 됨. 난이도를 지정하고 싶은 경우도 두번째 인자로.
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
//			{ // 사실상 Open 명령으로 할 수도 있다. 다루기 귀찮고 까다로워서 보다 간편한 명령을 제공하려는 거.
//				OpenStageCommand = FString::Printf(TEXT("Open %s?StageNum=%d?SM=%d?DF=%d?Standalone") ,
//					*WantedStageInfoData->MainMap.ToString(),
//					WantedStageNum, (int32)bScenarioModeOn, (int32)WantedDifficulty
//				);
//			}
//		}
//
//		GlobalSetPureMapLoadingShareValue(0.5f); // 작은 써비스.
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
//	{ // Standalone 플레이 같은 상황에서 캐릭터 선택하고자 할 때
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		const EPCClass MainClass = IntToPCClass(FCString::Atoi(*FirstArg));
//		
//		FString SecondArg;
//		FParse::Token(Cmd, SecondArg, true);
//		const EPCClass SubClass = IntToPCClass(FCString::Atoi(*SecondArg));
//
//		if (SubClass != EPCClass::EPC_End && SubClass != MainClass) 
//		{ // Sub 를 먼저 세팅하는 이유는 화면에 로그 뿌릴 때 이게 아래로 밀리게 하려고
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
//		// "SkillCoolTime" 과 코드가 겹치지만 개발 치트이고 여기에 따로 함수 만드는 것도 구질구질하니 걍 넘어가자
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
//		// 거의 성능 테스트 용도로서 스킬 종류나 쿨타임 정해놓고 가는 거.		
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
	// 개발용 커맨드이지만 모바일 테스트에서 사용할 거라면 아래 ExecDev 에 구현합니다.

	// .paper2dsprites 포맷의 스프라이트 소스 파일에서 각 스프라이트 영역 정보 가져와서 정해진 이름의 MIC 에 있는 atlas 파라미터에 넣어 주는 기능.
	// 사용 예시 : B2UIAtlasParam Sprite=D:\DepotWS\BII-Dev\B2_TexAtlas_InGameHUD_PC.paper2dsprites MICPath=HUD/Materials AtlasTexture=/Game/HUD/Texture/PCHUDAtlas/Textures/B2_TexAtlas_InGameHUD_PC.B2_TexAtlas_InGameHUD_PC 
	// AtlasTexture_PCPreview=/Game/DevOnly/B2_TexAtlas_InGameHUD_PC_sRGB_PCTemp.B2_TexAtlas_InGameHUD_PC_sRGB_PCTemp
//	if (FParse::Command(&Cmd, TEXT("B2UIAtlasParam")))
//	{
//		FString SpriteName;
//		if (FParse::Value(Cmd, TEXT("Sprite="), SpriteName)) // 스프라이트 원본 소스 파일 위치. **.paper2dsprites
//		{
//			FString MICPath;
//			FParse::Value(Cmd, TEXT("MICPath="), MICPath); // /Game/.../MIC파일이름 사이의 경로
//
//			FString OptionalAtlasTexturePath; // 텍스쳐는 기본 값으로 들어간 걸 쓰던지, 아니면 따로 넣을 수도 있겠다.
//			FParse::Value(Cmd, TEXT("AtlasTexture="), OptionalAtlasTexturePath);
//			UTexture2D* OptionalAtlasTexture = LoadObject<UTexture2D>(NULL, *OptionalAtlasTexturePath);
//
//			TArray<FString> SpriteFileContent;
//			FFileHelper::LoadANSITextFileToStrings(*SpriteName, &IFileManager::Get(), SpriteFileContent); // 좋은 유틸리티로군 낄낄
//
//			// 전체 사이즈부터
//			int32 TextureWidth = 0;
//			int32 TextureHeight = 0;
//			for (int32 FCI = SpriteFileContent.Num() - 1; FCI >= 0; --FCI) // 이 정보는 아마도 파일 하단에 있을 것임.
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
//				// 각 스프라이트 영역별로 이름 정보가 먼저 파싱되어야 함.
//				FString CurrSpriteName;
//				FString CurrMatchingMICName;
//				FString CurrMatchingMICName2; // For additional naming rule
//				FString CurrMatchingSpriteName; // We might need sprite too for use of image draw batching in wrap-box.
//				for (int32 FCI = 0; FCI < SpriteFileContent.Num(); ++FCI)
//				{
//					FString CurrLine = SpriteFileContent[FCI];
//
//					if (CurrLine.StartsWith(TEXT("\"")) && CurrLine.EndsWith(TEXT("\":"))) // 스프라이트 이름 라인의 포맷
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
//						// MIC 이름에서는 뒤쪽 확장자 제거하고 정해진 접두어 및 지정된 추가 경로를 붙인다.
//						int32 ExtensionStartIndex = ExtRemovedName.Find(TEXT("."));
//						if (ExtensionStartIndex != INDEX_NONE)
//						{
//							ExtRemovedName.RemoveAt(ExtensionStartIndex, 4); // e.g. Removing ".png", ".tga"
//
//						}
//						// Then, add our naming convention.
//						CurrMatchingMICName = TEXT("MAT_") + ExtRemovedName;
//						CurrMatchingMICName2 = TEXT("MAT_S_") + ExtRemovedName; // 기본적인 이미지 뿌리는 이외의 약간 특수한 효과가 들어간 것들인데, 이것들은 대체로 material 하나당 MIC 하나만 만들어서 쓰게 될 것. 텍스쳐 샘플링이 추가되더라도 기본적인 아틀라스 파라미터는 일단 마찬가지일 것.
//						CurrMatchingSpriteName = TEXT("MAT_Spr_") + ExtRemovedName; // Material 의 일종으로 치자..
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
//							// NoAtlas 세팅한 건 이 커맨드에서 제외해야 한다. 네이밍이 처음부터 이렇게 들어가 있으면 안되지만.. Atlas - NonAtlas 전환 과정에 있을 수도 있으므로 체크.
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
//							//	// UI TexAtlas master material 에 넣어놓은 파라미터 이름
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
//						// 4.14 이후.. WrapBox 안에서의 최적화를 위해 Paper2D Sprite 도 지원.
//						UPaperSprite* MatchingPaperSprite = LoadObject<UPaperSprite>(NULL, *CurrMatchingSpriteName);
//						// 걍 기본 네이밍도 체크해 보자. 손해볼 건 없으니
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
//	{ // 임시 기능으로서 기존에 Atlas 텍스쳐로 사용하던 거 개별로 쪼개진 텍스쳐로 바꾸도록 하려고 일괄 처리. 
//		// 여러번 사용할 수도 있어서 참고삼아 넣어둠.
//		FString MICPath;
//		FParse::Value(Cmd, TEXT("MICPath="), MICPath); // /Game/.../MIC파일이름 사이의 경로
//		FString MICNameFilter;
//		FParse::Value(Cmd, TEXT("MICNameFilter="), MICNameFilter);
//
//		FString TexturePath;
//		FParse::Value(Cmd, TEXT("TexturePath="), TexturePath); // /Game/.../MIC파일이름 사이의 경로
//		FString TextureNameFilter;
//		FParse::Value(Cmd, TEXT("TextureNameFilter="), TextureNameFilter);
//
//		TArray<UMaterialInstanceConstant*> AllMICs;
//		EditorFindAndLoadAssets<UMaterialInstanceConstant>(MICPath, MICNameFilter, AllMICs, false); // 기존에 넣은 곳에 하위 폴더가 있어서 recursive 하지 않게.. 필요에 따라 다르게 넣던지
//
//		TArray<UTexture2D*> AllTextures;
//		EditorFindAndLoadAssets<UTexture2D>(TexturePath, TextureNameFilter, AllTextures, false);
//
//		// AllMICs 랑 AllTextures 랑 수가 같은지 정도.. 아무래도 다를 거 같은데..
//		UE_LOG(LogBladeII, Log, TEXT("Found %d MICs from %s, %d Textures from %s"), AllMICs.Num(), *MICPath, AllTextures.Num(), *TexturePath);
//
//		for (int32 MI = 0; MI < AllMICs.Num(); ++MI)
//		{
//			UMaterialInstanceConstant* ThisMIC = AllMICs[MI];
//			UTexture* MatchingTexture = NULL;
//			if (ThisMIC)
//			{
//				// 동일한 네이밍의 텍스쳐를 찾아서.. 있으면 파라미터 세팅. 없으면 넘어가도록.
//
//				FString ThisName = ThisMIC->GetName();
//				if (ThisName.StartsWith(TEXT("MAT_"))) // 이미 MICNameFilter 로 걸러낼 수도 있지만.
//				{
//					ThisName.RemoveFromStart(TEXT("MAT_"));
//					ThisName = TEXT("TEX_") + ThisName;
//
//					// 다시 LoadObject 할 수도 있겠지만.. 이 커맨드 사용 목적 상 찾아낸 거에서.
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
//				{ // 본격 세팅 작업. StaticSwitchParameter 가 이렇게라도 변경되니 다행인데 딴 인터페이스 없나?
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
//					ThisMIC->SetTextureParameterValueEditorOnly(MtrlParamName_UIAtlasTexture, nullptr); // 기존 꺼 레퍼런스를 날려야겠다. 그래야 serialize 를 안 하지.
//					ThisMIC->SetTextureParameterValueEditorOnly(MtrlParamName_NoAtlasSingleTexture, MatchingTexture);
//					ThisMIC->MarkPackageDirty();
//				}
//				else
//				{
//					// 확인해 볼 필요가 있다.. 근데 좀 많은데. 아직 다 못쪼갠 건가
//					UE_LOG(LogBladeII, Warning, TEXT("[AutoReplaceAtlasTexture] Cannot Find matching texture for MIC %s !"), *ThisMIC->GetPathName())
//				}
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ProcessImportedUITextures")))
//	{ // 뭐 대량으로 처리하려고
//		FString TexturePath;
//		FParse::Value(Cmd, TEXT("TexturePath="), TexturePath); // /Game/.../MIC파일이름 사이의 경로
//		FString TextureNameFilter;
//		FParse::Value(Cmd, TEXT("TextureNameFilter="), TextureNameFilter);
//
//		TArray<UTexture2D*> AllTextures;
//		EditorFindAndLoadAssets<UTexture2D>(TexturePath, TextureNameFilter, AllTextures, true);
//
//		UE_LOG(LogBladeII, Log, TEXT("Found %d Textures from %s"), AllTextures.Num(), *TexturePath);
//
//		// 다시 LoadObject 할 수도 있겠지만.. 이 커맨드 사용 목적 상 찾아낸 거에서.
//		for (UTexture* TextureFromList : AllTextures)
//		{
//			if (TextureFromList)
//			{
//				TextureFromList->LODGroup = TextureGroup::TEXTUREGROUP_UI;
//				TextureFromList->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps; // UI 도 사양 따라서 MipLODBias 주면 좋긴 한데.. 스트리밍 되는 건 아니라서.. 모르겠다.
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
//	{ // NoAtlas 로 도중에 바꾼 것들이 어차피 기존 레퍼런스가 남아있다 보니.. 이걸 제거하지 않으면 소용이 없다..
//		FString MICPath;
//		FParse::Value(Cmd, TEXT("MICPath="), MICPath); // /Game/.../MIC파일이름 사이의 경로
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
//					UTexture* ExistingAtlasTexParamValue = NULL; // 이건 걍 로깅용
//					ThisMIC->GetTextureParameterOverrideValue(MtrlParamName_UIAtlasTexture, ExistingAtlasTexParamValue);
//
//					if (ExistingAtlasTexParamValue)
//					{
//						++ParamChangedCount;
//						ThisMIC->SetTextureParameterValueEditorOnly(MtrlParamName_UIAtlasTexture, nullptr); // NoAtlas switch 가 켜져 있으면 어차피 사용 안하는 텍스쳐고 쓸데없이 메모리만 먹으니 날린다.
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
//			// 일부 NavMesh 에 영향을 미치지 말아야 할 클래스들에 대한 CanEverAffectNavigation 자동 세팅 커맨드
//			for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//			{
//				// 설정이 필요한 클래스들. 필요하면 추가.
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
//		// B2Button 교체를 위한 커맨드
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
//					TEXT("%s 의 SafeZone 을 자동으로 생성하였습니다.\r\n\r\n")
//					TEXT("Root 아래에 배치되어 있던 %d 개의 widget 들이 새로 생성된 SafeZone 아래의 PanelWidget으로 옮겨졌습니다.\r\n\r\n")
//					TEXT("WidgetBP를 열고 컴파일 후 레이아웃을 확인해 보세요."),
//					*FoundWigetBP->GetPathName(), MovedChildrenNum
//				)));
//#endif
//			}
//			else
//			{
//#if !PLATFORM_MAC
//				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(
//					TEXT("%s 의 SafeZone 자동 생성이 실패했습니다.\r\n\r\n")
//					TEXT("이미 SafeZone 이 있거나, 최상단 RootWidget 의 타입이 특수한 것이 원인일 수 있습니다."),
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
//		// ReplaceSubWidgetClass 를 폴더 째로. 절대 경로로 지정한다.
//		FString SearchFolder;
//		FParse::Token(Cmd, SearchFolder, true);
//		SearchFolder = SearchFolder.Replace(TEXT("\\"), TEXT("/")); // 가능한 그냥 슬래쉬를 사용하는 게 좋을 것임.
//		FPaths::RemoveDuplicateSlashes(SearchFolder);
//
//		TArray<FString> FoundWidgetFiles;
//		IFileManager::Get().FindFiles(FoundWidgetFiles, *SearchFolder, TEXT(".uasset"));
//		
//		// 절대경로를 게임 경로로 바꾼다. 이거 유틸이 어디 있는지 모르겠는데..
//		FString LoadFolder = EditorGetGameRelativePathFromAbsPath(SearchFolder);
//
//		for (FString ThisFile : FoundWidgetFiles)
//		{
//			ThisFile.RemoveFromEnd(TEXT(".uasset"));
//			FString ThisWidgetPath = TEXT("/") + LoadFolder + TEXT("/") + ThisFile + TEXT(".") + ThisFile; // 에셋 아이콘에 Ctrl+C 하면 복사되는 경로 컨벤션으로 맞추기.
//
//			UWidgetBlueprint* FoundWigetBP = LoadObject<UWidgetBlueprint>(NULL, *ThisWidgetPath);
//			if (FoundWigetBP)
//			{
//#if !PLATFORM_MAC
//				ReplaceSubWidgetClass(FoundWigetBP, UButton::StaticClass(), UB2Button::StaticClass(), true); // 이건 메시지 박스 안띄우고 실행.
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
//		SearchFolder = SearchFolder.Replace(TEXT("\\"), TEXT("/")); // 가능한 그냥 슬래쉬를 사용하는 게 좋을 것임.
//		FPaths::RemoveDuplicateSlashes(SearchFolder);
//
//		TArray<FString> FoundWidgetFiles;
//		IFileManager::Get().FindFiles(FoundWidgetFiles, *SearchFolder, TEXT(".uasset"));
//
//		// 절대경로를 게임 경로로 바꾼다. 이거 유틸이 어디 있는지 모르겠는데..
//		FString LoadFolder = EditorGetGameRelativePathFromAbsPath(SearchFolder);
//
//		for (FString ThisFile : FoundWidgetFiles)
//		{
//			ThisFile.RemoveFromEnd(TEXT(".uasset"));
//			FString ThisWidgetPath = TEXT("/") + LoadFolder + TEXT("/") + ThisFile + TEXT(".") + ThisFile; // 에셋 아이콘에 Ctrl+C 하면 복사되는 경로 컨벤션으로 맞추기.
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
//	{ // 매우 특수한 사정 때문에 넣게 된 것. UI 리뉴얼 하면서 동일한 이름의 머티리얼들을 다른 경로에 만들어놓고 지금까지 만든 것들 바꿔치기 하려는 상황인데 이렇게 하려니 redirector 같은 처리가 되는 것도 아니고..
//		// 결국 수작업이 필요한데 가능한 선까지만 자동으로 처리 시도.
//		FString WidgetSearchPath;
//		FParse::Value(Cmd, TEXT("WidgetPath="), WidgetSearchPath);
//		FString WidgetSearchNameFilter;
//		FParse::Value(Cmd, TEXT("WidgetFilter="), WidgetSearchNameFilter);
//
//		FString OldAssetPath; // 대체하고자 하는 이전 경로 문자
//		FParse::Value(Cmd, TEXT("OldPath="), OldAssetPath);
//		FString NewAssetPath; // OldMaterialPath 가 이걸로 대체된 리소스를 찾아서 세팅할 것임.
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
//			// 알고 있는 타입의 리소스만. 여기선 사실 텍스쳐랑 머티리얼 아니면.. 페이퍼 스프라이트도 있군.
//			// 그것보다 ReplaceSubWidgetResource 안에서 처리하는 widget type 이 제한적일 것임. 어느 정도 수작업은 어쩔 수 없다.
//			int32 ModifiedCount = ReplaceSubWidgetResources<UMaterialInterface>(ThisWB, OldAssetPath, NewAssetPath);
//			ModifiedCount += ReplaceSubWidgetResources<UTexture>(ThisWB, OldAssetPath, NewAssetPath);
//			ModifiedCount += ReplaceSubWidgetResources<UPaperSprite>(ThisWB, OldAssetPath, NewAssetPath);
//#endif
//			if (ModifiedCount > 0)
//			{
//#if !PLATFORM_MAC
//				AllModifiedWidgets.Add(FString::Printf(TEXT("%s, %d 개"), *ThisWB->GetPathName(), ModifiedCount));
//#endif
//				ThisWB->MarkPackageDirty();
//			}
//		}
//		if (AllModifiedWidgets.Num())
//		{
//#if !PLATFORM_MAC
//			FString MsgStr = TEXT("다음 Widget Blueprint 들에서 리소스들을 변경\r\n바뀐 Widget Blueprint 들은 컴파일 및 저장을 해 주세요!\r\n\r\n");
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
//	{ // 레벨업 연출 테스트 세팅.
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
//	{ // 강화 연출 테스트 세팅.
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
//	{ // 합성 연출 테스트 세팅.
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		if (LobbyGM && LobbyGM->GetLobbyInventory())
//		{
//			LobbyGM->GetLobbyInventory()->DevOnlyItemCUSProgressSceneTest(ELobbyInvenItemOpMode::EIVIOP_Composition);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestItemUpgradeScene")))
//	{ // 승급 연출 테스트 세팅.
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		if (LobbyGM && LobbyGM->GetLobbyInventory())
//		{
//			LobbyGM->GetLobbyInventory()->DevOnlyItemCUSProgressSceneTest(ELobbyInvenItemOpMode::EIVIOP_Upgrade);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestItemSurpassScene")))
//	{ // 초월 연출 테스트 세팅.
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
//		{ // On/Off 스위치는 여타 인자 지정안한 경우만.
//			GbEditorPVPGamePreview = !GbEditorPVPGamePreview;
//		}
//		else
//		{
//			GbEditorPVPGamePreview = true; // 지정이 되었다면 무조건 켜는 거
//
//			GEditorPVPPreviewMyClassMain = IntToPCClass(FMath::Clamp(FCString::Atoi(*MyMainClassStr), 0, GetMaxPCClassNum() - 1));
//			GEditorPVPPreviewMyClassTag = IntToPCClass(FMath::Clamp(FCString::Atoi(*MyTagClassStr), 0, GetMaxPCClassNum() - 1));
//			GEditorPVPPreviewRemoteClassMain = IntToPCClass(FMath::Clamp(FCString::Atoi(*RemoteMainClassStr), 0, GetMaxPCClassNum() - 1));
//			GEditorPVPPreviewRemoteClassTag = IntToPCClass(FMath::Clamp(FCString::Atoi(*RemoteTagClassStr), 0, GetMaxPCClassNum() - 1));
//		}
//
//#if !PLATFORM_MAC
//		FString MsgStr = FString::Printf(TEXT("PVP 모드 에디터 전용 프리뷰 %s."), GbEditorPVPGamePreview ? TEXT("켜짐") : TEXT("꺼짐"));
//		if (GbEditorPVPGamePreview)
//		{
//			MsgStr += FString::Printf(TEXT("\n\n로컬 플레이어 메인 캐릭터: %d"), (int32)GEditorPVPPreviewMyClassMain);
//			MsgStr += FString::Printf(TEXT("\n로컬 플레이어 태그 캐릭터: %d"), (int32)GEditorPVPPreviewMyClassTag);
//			MsgStr += FString::Printf(TEXT("\n원격상대 플레이어 메인 캐릭터: %d"), (int32)GEditorPVPPreviewRemoteClassMain);
//			MsgStr += FString::Printf(TEXT("\n원격상대 플레이어 태그 캐릭터: %d"), (int32)GEditorPVPPreviewRemoteClassTag);
//			MsgStr += TEXT("\n\nPVP 모드 에디터 프리뷰는 불안정할 수 있으니 플레이 전에 작업물을 저장해 주세요.");
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
//			// 프리뷰 스위칭
//			g_IsEditorRaidGamePreview = !g_IsEditorRaidGamePreview;
//		}
//		else
//		{
//			g_IsEditorRaidGamePreview = true; // 지정이 되었다면 무조건 켜는 거
//
//			g_EditorRaidPreviewPCClassMain[0] = IntToPCClass(FMath::Clamp(FCString::Atoi(*PCClassStr_0), 0, GetMaxPCClassNum() - 1));
//			g_EditorRaidPreviewPCClassMain[1] = IntToPCClass(FMath::Clamp(FCString::Atoi(*PCClassStr_1), 0, GetMaxPCClassNum() - 1));
//			g_EditorRaidPreviewPCClassMain[2] = IntToPCClass(FMath::Clamp(FCString::Atoi(*PCClassStr_2), 0, GetMaxPCClassNum() - 1));
//			g_EditorRaidPreviewPCClassMain[3] = IntToPCClass(FMath::Clamp(FCString::Atoi(*PCClassStr_3), 0, GetMaxPCClassNum() - 1));
//		}
//
//#if !PLATFORM_MAC
//		FString MsgStr = FString::Printf(TEXT("[에디터 전용] 레이드 프리뷰 : %s."), g_IsEditorRaidGamePreview ? TEXT("켜짐") : TEXT("꺼짐"));
//		if (g_IsEditorRaidGamePreview)
//		{
//			MsgStr += FString::Printf(TEXT("\n\n로컬 플레이어 - 메인 캐릭터: %d"), (int32)g_EditorRaidPreviewPCClassMain[0]);
//			MsgStr += FString::Printf(TEXT("\n\n로컬 플레이어 - 1번 파티원 캐릭터: %d"), (int32)g_EditorRaidPreviewPCClassMain[1]);
//			MsgStr += FString::Printf(TEXT("\n\n로컬 플레이어 - 2번 파티원 캐릭터: %d"), (int32)g_EditorRaidPreviewPCClassMain[2]);
//			MsgStr += FString::Printf(TEXT("\n\n로컬 플레이어 - 3번 파티원 캐릭터: %d"), (int32)g_EditorRaidPreviewPCClassMain[3]);
//			MsgStr += TEXT("\n\n레이드 프리뷰 모드 에디터 프리뷰는 불안정할 수 있으니 플레이 전에 작업물을 저장해 주세요.");
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
//		// 어느 날인가 발생한 알 수 없는 문제점으로 인해.. 영상 찍기 위한 거라 이렇게 임시 대처로..
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
//	{ // 개발 과정에서의 테스트 커맨드임
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
//				FString MsgStr = FString::Printf(TEXT("WidgetBlueprint %s 에서 (최소) %d개의 텍스쳐가 사용되고 있습니다.\n\n"), *FoundWigetBP->GetName(), RefTextures.Num());
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
//	{ // 에디터에서 DialogInfo 테이블 작업 후에 실질적인 갱신이 안됨.. 대신 이렇게라도..,
//		// 아, 걍 UEditorEngine::PlayInEditor 에서 이거 호출하게..
//		BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
//		if (B2GameModule)
//		{
//			B2GameModule->DestroyDialogInfoTable();
//			B2GameModule->GetDialogInfoTable();
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("DummyClearReward")))
//	{ // 일반 에디터 플레이에서 클리어 보상 아이템 연출 보기 위해
//		FString RewardNumStr;
//		FParse::Token(Cmd, RewardNumStr, true);
//		int32 RewardNum = FCString::Atoi(*RewardNumStr);
//		
//		RewardNum = FMath::Clamp(RewardNum, 1, 5);
//
//		bool bIsDoubled = (FMath::RandRange(0, 2) == 0);// 보너스 상황 모의
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
//				// 개수따라 grade 로.. 단, 이게 RewardNum 은 5개까지일 건데 실제로는 6성까지 확인해야 해서.. RewardNum 에 따라 좀 조절함
//				int32 ThisItemGrade = (RI % FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE); // 이 값을 그대로 쓰면 0 이 나올 수 있어서 안되고 최소한 1을 더해야 한다.
//				if (RewardNum == 3 || RewardNum == 5)
//				{
//					ThisItemGrade += 2; // RewardNum 5 인 경우는 2, 3, 4, 5, 6 이 됨.
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
//				// 좀 예외로 골드도 보여주도록 해 보자..
//				if (RewardNum == 5 && ThisItemGrade == 2)
//				{
//					GenerateGoldItem(ThisItem, 10000);
//					AllRewardItem.Add(ThisItem);
//				}
//				// 예외로 재료 아이템도 보여주도록 해볼까?
//				else if (RewardNum == 5 && ThisItemGrade == 3)
//				{
//					int32 RandNum = FMath::RandRange(0, 17);
//					ThisItem.ItemRefID = 8000000 + RandNum;
//					ThisItem.InventoryType = EItemInvenType::EIIVT_Consumables;
//					ThisItem.ItemClass = EItemClass::EIC_End;
//					ThisItem.InstanceUID = 8252424525250235 + RI; // 이건 적당히
//					AllMaterialRewardItem.Add(ThisItem);
//				}
//				else
//				{
//					ThisItem.ItemRefID = 1000000 + ThisItemGrade * 100;
//					ThisItem.InstanceUID = 5252424525250235 + RI; // 이건 적당히
//
//					DevOnlyLoadItemClientTestData(ThisItem);
//					AllRewardItem.Add(ThisItem);
//				}
//
//				if (bIsDoubled && ThisItem.ItemRefID != FItemRefIDHelper::GetGoodsID_Gold())
//				{
//					ThisItem.InstanceUID += 972442;
//					AllRewardItem.Add(ThisItem); // 보너스. 똑같은 RefID 로 하나 더
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
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("FirstTimeNPCSensitiveInfoConstruction 기능은 첫 사용 후 현재 비활성화됨. 사용이 필요한 경우 프로그래밍팀에 문의.")));
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
//			FString MsgStr = TEXT("위젯 이름들\n\n");
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
//		// 캐릭터 PC 용 MIC 들의 파라미터를 일괄 세팅하는 에디터 커맨드.
//		// 사용 포맷은 예를 들어 
//		// "ChangePCCharMIParam 1 RoughnessSkinScale 0.4" 암살자 용 MIC 들의 RoughnessSkinScale 파라미터를 0.4 로 세팅
//		// "ChangePCCharMIParam Gl MetalicScale 0.3" 검투사 용 MIC 들의 MetalicScale 파라미터를 0.3 으로 세팅.
//		// PC 지정은 0 ~ 3 혹은 약속된 약어를 사용할 수 있다.
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
//		{ // 언젠가 이 코드를 짠 사람이 없을 때 당황하지 않도록.
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("ChangePCCharMIParam 커맨드는 Fighter 까지만 준비되었습니다.\n새로운 캐릭터에 대한 코드는 추가해야 합니다.")));
//#endif
//		}
//
//		// 우리 캐릭터 팀서 사용하는 네이밍에 따라 파일을 필터링 하는 거.
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
//			TArray<FString> FoundAndProcessedFiles; // 결과 메시지 용
//
//			for (UMaterialInstanceConstant* ThisMIC : AllFoundMICs)
//			{
//				if (ThisMIC)
//				{
//					// 파라미터가 있는지 여부 체크하는 건 결과 메시지에 반영하기 위함.
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
//				MessageStr = FString::Printf(TEXT("다음 %d 개 MIC 들의 파라미터 %s 이(가) %.3f (으)로 변경되었습니다. 확인 및 저장해 주세요.\n\n"),
//					FoundAndProcessedFiles.Num(), *WantedParamNameStr, WantedParamValue);
//				for (int32 FI = 0; FI < FoundAndProcessedFiles.Num(); ++FI)
//				{
//					MessageStr += FString::Printf(TEXT("%d. %s\n\n"), FI + 1, *FoundAndProcessedFiles[FI]);
//				}
//			}
//			else
//			{
//				MessageStr = FString::Printf(TEXT("%s 네이밍의 Character MIC 들의 파라미터 %s 을(를) %.3f (으)로 변경하고자 하였으나 하나도 성공하지 못했습니다. 로컬 워크스페이스 상의 파일/경로가 변경되었거나 존재하지 않는 파라미터를 지정하였을 가능성이 있습니다."),
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
//		// UB2MatineeRefCheckCommandlet 을 만들다가.. 이런 기능이 커맨들렛 환경에서 불가능한 것도 같고
//		// 어차피 에디터에서 사용하는 게 편할 수도 있어서.. 이래저래 에디터 커맨드로 제공.
//		UB2MatineeRefCheckCommandlet::HandleAsEditorCommand(Cmd, Ar);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("B2MaterialParamCheck")))
//	{ // 동일한 이름의 커맨들렛으로 사용할 수 있음.
//		UB2MaterialParamCheckCommandlet::EditorCommandMain(Cmd, Ar);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("B2AssetOptCheck")))
//	{ // 동일한 이름의 커맨들렛으로 사용할 수 있음.
//		UB2AssetOptCheckCommandlet::EditorCommandMain(Cmd, Ar);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SummarizeMaterialStaticSwitchUse")))
//	{ // 바로 아래 ReplaceParentMaterialStaticSwitch 와 연계해서 사용하려는 것임.
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
//	{ // 바로 위 SummarizeMaterialStaticSwitchUse 와 연계해서 사용하려는 것임.
//		// 특정 Material 을 Parent 로 하면서 특정 StaticSwitch 셋을 설정한 MIC 들의 Parent 를 바꿔치는 기능. 자세한 동작은 아래 함수에서..
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
//		// 이걸로 StaticSwitch 구성을 어떻게 지정하는지는 아래 본 함수에서 파싱.
//		FParse::Value(Cmd, TEXT("SSParam="), SSParamSetString, false); // ShouldStopOnComma 를 false 로. SSParam 구문에 Comma 가 들어가기 때문.
//
//		ReplaceParentMaterialOfStaticSwitchParam(CurrParentMaterial, NewMaterial, ChildMICSearchPath, SSParamSetString);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ReplaceParentMaterialOfMIC"))) // 단순한 부모 Material 바꿔치기. 흙수저의 금수저 재탄생
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
//			// 찾은 파일 중 MIC 만 필요하지만 실제로는 로딩을 해 봐야 안다..
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
//		// 연출용 SkeletalMeshActor 셋업하는데 기본 더미 아닌 6성 용으로 해 놓은 게 너무 많아 레벨 단위로 일괄처리라도 하게..
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
//			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("바뀐 것이 없습니다.")));
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
bool gbHACKInGameUIHiddenByDevCommand = false; // 도중에 생성되는 애들도 적용이 되어야 해서.. ㅠㅠ
bool GCheat_CompleteIgnoreEventScene = false; // 이것을 사용하는 경우는 아예 연출 시작조차 되지 않으며 몇몇 맵은 제대로 플레이가 불가능할 수도 있음. 특수한 상황의 테스트용.
int32 GCheat_SkipEventSceneCount = -1; // 이건 게임 진행이 가능한 수준에서 연출 장면을 스킵. 0 보다 크면 그 숫자만큼의 연출장면을 스킵한다.
bool GCheat_HeroineMode = false; // Hero 모드와 비슷한데 현재 몹들 중 하나만 죽이면 나머지가 다 죽음.
float GCheat_StageDifficultyOverride = 0.0f; // 0.0 에서는 기본값
bool GCheat_PCPartMergeRemoveExtraBoneInfluence = false;
int32 GCheat_RepeatBattleCountOverride = 0;
bool GTest_GenerateDummyChatMessage = false;
bool GTest_UseNonCompositedRawPCMesh = false; // PC 장비 시스템에 의해 조합되지 않은 기본 메쉬 사용.
bool gIsDevUI = false;
bool gUseHeroTowerResetNoLoading = true; // 영웅의 탑 로딩 없이 재시작하는 기능이 개발 도중 버그를 일으킬 소지가 많으므로 문제 발생시 테스트를 위해 제공.
bool gDrawPerfMark = false; // ScopedCallTracker 에서 특정 시간 이상 소요된 거 화면에 표시
bool gPreloadCombatEffectResources = true; // 성능 테스트 용. 몇몇 전투에서 spawn 하는 TAsset 으로 된 이펙트 관련 리소스들을 미리 로딩 및 캐싱해 놓은 것을 통해 hitch 가 얼마나 개선되는지 보기 위해.
bool gDisableInGameFloatingWidgets = false; // 전투에서 격렬하게 등장하는 DamageNum, AbnormalState, GoldAcq 등 끄기. 성능 테스트용.
bool gDisableCombatEventUI = false; // 전투등 이벤트 격으로 등장하는 Floating 외의 것들
int32 gPCDataPreLoadMaxNum = -1; // 로딩시간 체크용. 사실상 캐릭터 4종류에서는 4이상이기만 하면 정상작동. 0, 1 같은 숫자로 테스트하기 위함.
bool gNPCDataNoPreLoadMode = false;
bool gUseCharacterDefaultMaterial = false; // 성능 측정 용
int32 gTestAttachLightCompMode = 0; // 플레이어 캐릭터 라이트 attach mode
bool gbTestTurnOffPlayerLight = false;

 //ItemInfo 장비 비동기 로딩 완료 후 플레이어 캐릭 dummy spawn 까지 테스트 하기 위함.
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

	 //테스트 커맨드에서 사용하려는 것임. 기능은 참고만.

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
			// 요청한 장비들 ID 를 가지고 DummyPlayer spawn 을 직접 하려고 해도 ABladeIITestDummyPlayer 쪽 spawn 기능에 어차피 StandardEquipment 가지고 하는 것만 구비되어 있음.

			if (GEngine)
			{ // 로딩 완료된 장비 가지고 spawn 테스트. 물론 이래도 PCClassInfo 로딩에 오래 걸릴 수 있다.
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
static FItemInfoAsyncLoadTest ItemInfoAsyncLoadTestObj; // 테스트에 우려먹을 객체 하나.

bool UB2StaticExecHelper::ExecDev(UWorld* Inworld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	 //모바일 테스트 버전에서는 사용할 커맨드를 여기에. 최종판에서는 마찬가지로 빠지겠지만.
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
//		UB2UIManager::GetInstance()->HideBattleRelatedWidgets(gbHACKInGameUIHiddenByDevCommand); // 신 UI
//
//		return true;
//	}
//	if (FParse::Command(&Cmd, TEXT("ShowUI"))) // "InGameUI" 는 옛날에 만든 거고.. 이제는 걍 이렇게 하면 잘 먹히는 듯..
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
//		if (ItemIDString == TEXT("Gold")) // 특수 케이스.
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
//	{ // 로비에서 인벤토리가 초기화된 상태에서 사용해야 먹힘.
//		FString RefIDString;
//		FParse::Token(Cmd, RefIDString, true);
//		int32 ParsedRefID = FCString::Atoi(*RefIDString);
//
//		FString CreateNumString;
//		FParse::Token(Cmd, CreateNumString, true);
//		int32 CreateNum = FCString::Atoi(*CreateNumString);
//		if (CreateNum <= 0)
//		{
//			CreateNum = 30; // 기본값
//		}
//		if (ParsedRefID < 2000000)
//		{ // 일반적인 장착 아이템이라면 실수로 너무 많이 요청하지 않게..
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
//			{// 이건 첨에 만들었던 기능인데 결국 마찬가지로 DevOnlyRequestDummyItems 를 사용할 것. RefID 는 안에 하드코딩.
//				LobbyInven->DevOnlyGenerateDummyItemData(true);
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GenerateClientDummyItems")))
//	{ // 로비에서 인벤토리가 초기화된 상태에서 사용해야 먹힘.
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
//		if (LobbyInven)
//		{ // 이 경우, 서버로 요청하는 대신 클라이언트에만 채운다.
//			LobbyInven->DevOnlyGenerateDummyItemData(false);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GenerateClientDummyConsumable")))
//	{
//		FString ConsumableIDStr;
//		FParse::Token(Cmd, ConsumableIDStr, true);
//		int32 ConsumableID = FCString::Atoi(*ConsumableIDStr); // ID 는 정해진 거 알아서 넣어야 할 듯.
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
//		// PCClass 지정하게 하려고 했는데 괜히 헷갈리기만 할 듯. 걍 모두 동시 생성하자.
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
//		LobbyUpdateWingMainUIClass<>::GetInstance().Signal(); // Wing UI 페이지 업데이트.
//
//		// 로비 메쉬 업데이트도 같이
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
//	{ // 현재 로비 선택 캐릭터의 장착으로 인해 증가되는 능력치 확인
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
//		UB2LobbyInventory::GetAllEquippedOptionPointsLocalPlayer(AllOptions, LobbyPCClass); // 최종적으로 적용될 값이 나와야 함.
//		for (auto& ThisOption : AllOptions)
//		{ // Item Option 은 int 로는 순차적이지는 않을 것.
//			UE_LOG(LogBladeII, Log, TEXT("OptionType %d, amount %.4f"), (int32)ThisOption.Key, ThisOption.Value);
//		}
//		return true;
//	}*/
//	else if (FParse::Command(&Cmd, TEXT("SPJump")))
//	{
//		// 현재 맵에서 로딩된 SpawnPool 들의 특정 Wave 까지 강제 시작 및 종료.
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
//					CurrSED->CustomFinishShow(); // 위 과정을 거치면서 나오는 연출들은 끝내는 게 좋을 듯.
//				}
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SpawnGuildNPC")))
//	{
//		// 파라미터 추출
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
//		if (B2Player) // 플레이어에서 약간 비껴난 위치에
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
//		// NPCClassInfo 에 등록된 NPC spawn. MonsterSpawnPool 설치 없이 즉석에서 커맨드로 spawn 시키고 싶을 때
//		// "SpawnNPC ClassName VariationName" ClassName 과 VariationName 은 각각 ENPCClass 랑 ENPCClassVariation 항목에서 prefix 뺀 거
//
//		// 파라미터 추출
//		FString ClassStr;
//		FParse::Token(Cmd, ClassStr, true);
//		ENPCClass WantedClass = UB2NPCSingleClassInfo::FindNPCClassEnumFromTrimmedName(ClassStr);
//
//		FString ClassVariStr;
//		FParse::Token(Cmd, ClassVariStr, true);
//		ENPCClassVariation WantedClassVariation = UB2NPCSingleClassInfo::FindNPCClassVariEnumFromTrimmedName(ClassVariStr);
//		
//		bool bSpawnAsBoss = FParse::Param(Cmd, TEXT("Boss")); // SpawnPool 의 boss 격으로.. 이걸 지정하려면 Variation 도 지정해야..
//
//		if (WantedClass != ENPCClass::ENC_End)
//		{
//			FTransform SpawnTransform;
//			ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//			if (B2Player) // 플레이어에서 약간 비껴난 위치에
//			{
//				SpawnTransform = B2Player->GetTransform();
//				SpawnTransform.AddToTranslation(FVector(100.0f, 0.0f, 100.0f));
//			}
//
//			FActorSpawnParameters SpawnInfo;
//			SpawnInfo.Owner = B2Player;
//			SpawnInfo.bDeferConstruction = true; // Need deferred construction and manually call FinishSpawning. AutoPossessAI 설정 및 NPCClassInfo 적용을 위해 DeferConstruction 이 필요.
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
//					FMobSpawnedBirthplaceInfo DummyBirthplaceInfo; // 실제로 Birthplace 는 없지만..
//					DummyBirthplaceInfo.bSpawnedAsBossMob = bSpawnAsBoss; // 요거 전달하려고.
//					SpawnedNPC->SetBirthPlace(DummyBirthplaceInfo);
//
//					SpawnedNPC->FinishSpawning(SpawnTransform);
//					SnapBladeIICharacterToGround(SpawnedNPC);
//
//					// Boss 몹이면 UI 테스트가 하나의 목적이니 세팅할 수 있는 것들 최대한..
//					UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
//					if (bSpawnAsBoss && DocBattle)
//					{
//						DocBattle->SetbCurrentlyBossAppeared(true); // 죽을 때 다시 리셋해야 함.
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
//		// NPCClassInfo 에 등록된 NPC 를 dummy 로 spawn. 
//		// SpawnNPC 는 실제로 전투를 벌이는 NPC 를 spawn 시키지만 이건 SpawnDummyPlayer 처럼 애니메이션만 한다.
//		// "SpawnDummyNPC ClassName VariationName" ClassName 과 VariationName 은 각각 ENPCClass 랑 ENPCClassVariation 항목에서 prefix 뺀 거
//
//		// 파라미터 추출
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
//			if (B2Player) // 플레이어에서 약간 비껴난 위치에
//			{
//				SpawnTransform = B2Player->GetTransform();
//				SpawnTransform.AddToTranslation(FVector(200.0f, 0.0f, 200.0f));
//			}
//
//			// iOS 컴파일 에러
//			/*
//			ABladeIITestDummyNPC* NewSpawned = ABladeIITestDummyNPC::SpawnUtil(Inworld, WantedClass, WantedClassVariation, SpawnTransform,
//				ParsedSpawnFloatingInterval, ParsedSpawnDamageFxInterval);
//
//			if (NewSpawned)
//			{
//				if (ParsedAnimStateString == TEXT("Rand") || ParsedAnimStateString == TEXT("Random"))
//				{ // 랜덤하게 플레이
//					NewSpawned->SetUseRandomAttackState(true);
//				}
//				else
//				{ // 정해놓고 하나 플레이
//					NewSpawned->SetUseRandomAttackState(false);
//					NewSpawned->SetDirectAttackStateByKeyword(ParsedAnimStateString);
//				}
//
//				NewSpawned->SetCooltimeBetweenAnim(ParsedAnimCooltime);
//
//				// 기본적으로는 안 움직이고 특별히 인자를 주면 이동이나 회전 가능하게
//				if (FParse::Param(Cmd, TEXT("Move"))) {
//					// 회전없이 이동만 하는 건 우스꽝스러우니 둘 다 ㅋ
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
//	else if (FParse::Command(&Cmd, TEXT("B2SED"))) // 연출 장면 강제 없애기. 시작 후 스킵이 아니라 시작조차 하지 않는 것으로 크래쉬 추적 등을 위한 테스트용.
//	{
//		FString OnOffStr;
//		FParse::Token(Cmd, OnOffStr, true);
//				
//		// GCheat_SkipEventSceneCount 와는 반대.. ㅡ.ㅡ
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
//		else{ // GCheat_SkipEventSceneCount 가 있을 수 있으니
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
//			GCheat_SkipEventSceneCount = SecifiedCount; // 정해진 개수만큼의 연출장면만 통과할 것.
//		}
//		else if (OnOffOrCountStr == TEXT("On") || OnOffOrCountStr == TEXT("True"))
//		{
//			GCheat_SkipEventSceneCount = 100000000; // 단순한 turn-on 이면 걍 존나 큰 숫자를 줘서 사실상 계속 스킵 되도록. 어차피 개발용 치트이니.
//		}
//		else if (OnOffOrCountStr == TEXT("Off") || OnOffOrCountStr == TEXT("False") || OnOffOrCountStr == TEXT("0"))
//		{
//			GCheat_SkipEventSceneCount = -1;
//		}
//
//		if (GCheat_SkipEventSceneCount > 0){
//			
//			// 지금 플레이 되는 게 있다면 끝내자.
//			for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//			{
//				AB2StageEventDirector* CurrSED = Cast<AB2StageEventDirector>(*ActorIt);
//				if (CurrSED && CurrSED->GetDirectingEvent() != EStageEvent::ESEV_Clear && CurrSED->IsPlayingShow())
//				{
//					//--GCheat_SkipEventSceneCount; 이건 포함시키지 말자..
//					CurrSED->CustomFinishShow();
//				}
//			}
//
//			UE_LOG(LogBladeII, Log, TEXT("Stage event scene will be skipped for %d times, but some essential scene will still be played."), GCheat_SkipEventSceneCount);
//		}
//		else{ // GCheat_CompleteIgnoreEventScene 이 있을 수 있으니
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
//		// 0 보다 크면 StageManager 쪽에서 오버라이드 해서 사용할 것임.
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
//					CastedCharacter->InitializeCombatStats(); // 이미 spawn 된 애들은 레벨에 따라 전투력 재계산.
//					CastedCharacter->Health = HealthRatio * CastedCharacter->MaxHealth; // 재계산 후에 HP 만땅 차므로 비율만 맞춰줌 ㅋ
//				}
//			}
//		}
//		
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PCMerge")))
//	{
//		// Mesh merge 테스트. UEngine::HandleMergeMeshCommand 참고. 인자를 통해 리소스를 넘겨주지 않으면 단순히 메인 캐릭과 태그 캐릭 메쉬를 merge 낄낄 
//		// 이건 길게 인자 넣어주기 어려운 모바일 환경 작동 테스트를 염두에 둔 거
//
//		bool bGenerateNewSkeleton = FParse::Param(Cmd, TEXT("NewSkeleton")); // Skeleton 을 새로 만들거나 첫번째 소스 메쉬의 skeleton 을 조합된 메쉬의 skeleton 으로 사용.
//
//		// array of source meshes that will be merged
//		TArray<USkeletalMesh*> SourceMeshList;
//		TArray<FSkelMeshMergeSectionMapping> ForceSectionMapping;
//		TArray<FSkelMeshMergeMtrlOverrideMapping> SrcMtrlOverrides;
//
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		if (!B2Player){ // 이건 여하간 필요. 결과물을 여기에 넣을 거라
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
//		{ // 커맨드 인자로 skeletal mesh 를 넘겨준 경우.
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
//					FSkelMeshMergeMtrlOverrideMapping NewMtrlMap; // 물론 나중에 section ID 접근할 필요 없이 현 시점에서 바로 material override 를..
//					for (int32 SI = 0; SI < ThisMeshResource->LODModels[0].Sections.Num(); ++SI) // ForceSectionMapping 은 LODModel 0 번 기준으로 하면 됨.
//					{
//						NewSectionMap.SectionIDs.Add(ForceSectionID);
//
//						// 섹션을 합치게 되면 파트별 섹션을 합쳐서 사용하기 위해 준비된 별도의 material 을 사용한다. 같은 섹션이면 동일한 material 을 사용해야 함.
//						// MergeSections 에서도 여전히 null 일 수 있다. 그런 경우는 원래 material 을 쓰는 건데 여전히 명시적으로 단일 섹션을 구성하는 파트가 있으면 그렇게 될 듯
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
//				//Battle UI업데이트
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
//				//SkillRTManager data업데이트
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
//		{ // 로비라면 업데이트를..
//			LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(PCI));
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SpawnBuffText")))
//	{
//		FString BuffCatStr;
//		FParse::Token(Cmd, BuffCatStr, true);
//		const int32 BuffCat = FCString::Atoi(*BuffCatStr); // 숫자로 지정해서 0 : 일반Buff, 1 : TeamBuff, 2 : StateAttack, 3 : ResurrectBuff, 4 : Influence
//		FString BuffTypeStr;
//		FParse::Token(Cmd, BuffTypeStr, true);
//		const int32 BuffTypeInt = FCString::Atoi(*BuffTypeStr); // BuffCat 에 따라 다름, 2진수인 TeamBuff 도 그냥 다른 거랑 마찬가지로 취급하자..
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
//					// 이건 현재 FloatingDamageNum 쪽에..
//					CharacterTakeDamageClass<ABladeIICharacter*, float, bool>::GetInstance().Signal(LocalB2Player, FMath::RandRange(1.0f, 2000.0f), true);
//				}break;
//				case 7: {
//					// 이건 사실 Text 가 아닌 Effect. EndEffect 일지 여부는 랜덤.
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
//		BeginWingEvolutionSceneClass<EPCClass>::GetInstance().Signal(EPCClass::EPC_End); // 요 이벤트를 받는 영웅관리 모드여야.
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("STLog")))
//	{ // B2_SCOPED_TRACK_LOG 에 의한 로그가 너무 많아서 다른 디버깅에 방해가 될 때에 끈다.
//		// 그냥 이쪽 로그 카테고리 따로 만들었으니 그걸 가지고 Log LogB2ScopedCallTrack Off 명령을 주어도 됨.
//		FB2ScopedCallTracker::bIsOn = !FB2ScopedCallTracker::bIsOn;
//		UE_LOG(LogBladeII, Log, TEXT("B2ScopedCallTrack Log being turned %s"), FB2ScopedCallTracker::bIsOn ? TEXT("ON") : TEXT("OFF"));
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("STimeLog")))
//	{ // 여기도 독자적인 로그 카테고리가 있으니 그걸 써도.. LogB2ScopedTimeLog
//		FB2ScopedTimeLogger::bIsOn = !FB2ScopedTimeLogger::bIsOn;
//		UE_LOG(LogBladeII, Log, TEXT("FB2ScopedTimeLogger being turned %s"), FB2ScopedTimeLogger::bIsOn ? TEXT("ON") : TEXT("OFF"));
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("MissySexy")))
//	{
//		// 섹시퇴폐여캐룩에 부담없는 전투를 위한 장비 구성. 이것들만 입히고 장갑 부츠 다 벗길것. 
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1100600 4")); // ServerCmd 들임
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1102600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1200600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1202600 4"));
//		
//		// 아래 악세사리는 걍 체력 늘리기 용.
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1105600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1106600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1107600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1205600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1206600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1207600 4"));
//
//		// 검투사의 우스꽝스러운 룩을 위해 헬멧만 6성맞춤.
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1001600 4"));
//
//		// 그리고 추가로 주머니도 넉넉하게 채워주자. 이것들은 GetAccount 해 주어야 반영됨.
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncGold 10000000"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncGem 1000000"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncDarkCrystal 1000000"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncRankPromotionPoint 100000"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncSocialPoint 1000"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncHeroPiece 1000"));
//
//		// 날개 강화 등의 재료.
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
//		// Options= 이후로는 ID-Value,ID-Value 이런 식으로 추가하기 원하는 세부 옵션들을 지정. ID 는 서버 B2ItemOption 숫자 기준. 그 숫자가 기획 엑셀 파일에서 보는 것과 일치하므로
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
//			// ID-Value 각각을 하이픈 기준으로 떼어냄.
//			if (ThisParsedStr.Split(TEXT("-"), &OptionIDStr, &OptionValueStr))
//			{
//				int32 OptionIDInt = FCString::Atoi(*OptionIDStr);
//				EItemOption CliItemOption = SvrToCliOptionType(OptionIDInt); // ID 숫자는 서버 타입에서 사용하는 것으로 Client enum type 으로 변경.
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
//		// 일단은 기존 걸 날리는 옵션으로.. 여기 매개변수로 줄 수도 있고..
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
//		UB2LobbyUI_CharStatDetail::SetDebugDrawPlayerStatMode(DisplaySetNum); // DisplaySetNum 이 0 보다 크면 보이기 시작할 것.
//
//		// 스테이지 모드라면 어차피 현재 플레이어 캐릭터가 사용될 것. 이건 네트워크 모드 용으로 의도한 것.
//		int32 PCClassInt = 0;
//		FParse::Value(Cmd, TEXT("NetPC="), PCClassInt);
//		EPCClass OverridePCClass = IntToPCClass(PCClassInt);
//		UB2LobbyUI_CharStatDetail::SetDebugDrawPlayerStatClass(OverridePCClass);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("NumToPlayRaid"))) // 넷게임모드 플레이 참가자 줄이기, 레이드는 혼자하기가 되므로 안씀
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
//	{// 테스트를 위해 치트키로 제공. 특정 상황에서 조심해서 사용해야 할 기능.
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
//		// 필수까진 아니고 다른 팀에서 테스트 할 때 도중에 ShowDevUI 치고 바로 업데이트 안되면 헤멜까봐 편의 차원에서 좀 업데이트 해 줌.
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
//	{ // 테스트 커맨드 차원에서 AB2StageGameModeBase 를 가지고 하지만 실제 사용은 이게 허용된 특정 게임모드에서만 해야 한다.
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
//		// 권혁중 - 테스트용으로 쓰는 커맨드입니다.
//	}
//	else if (FParse::Command(&Cmd, TEXT("HeroTowerLoading")))
//	{
//		gUseHeroTowerResetNoLoading = !gUseHeroTowerResetNoLoading;
//		UE_LOG(LogBladeII, Display, TEXT("HeroTowerGameMode restart without world loading %d"), (int32)gUseHeroTowerResetNoLoading);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CharFxLOD")) || FParse::Command(&Cmd, TEXT("CFxLOD"))) // "CFxLOD PC/Mob/Boss #" 캐릭터 종류별로 ParticleSystemComponent LOD 적용
//	{
//		FString TypeStr;
//		FParse::Token(Cmd, TypeStr, true);
//
//		FString ParsedLODLevelStr;
//		FParse::Token(Cmd, ParsedLODLevelStr, true); // 암것도 지정 안되었다면 0이겠지. 그럼 0으로 세팅하면 됨.
//		const int32 ParsedLODLevel = FCString::Atoi(*ParsedLODLevelStr);
//
//		if (TypeStr == TEXT("None")){
//			// 모두 리셋
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
//			GB2CharFxCheatLODLevel_PC = -1; // 뭔가 다 아닌 경우도 리셋
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
//					ThisComp->SetLODLevel(0); // 여기 넣어주는 값은 의미없고 걍 업데이트 차원..
//				}
//				else {
//					if (ThisComp->GetWorld()) {
//						ThisComp->RestoreLODLevelToDesiredValue(); // 이러면 원래 예상되는 값으로 돌아가려나
//					}
//				}
//			}
//		}
//
//#if !UE_BUILD_SHIPPING
//		// 혹 커맨드를 쓰더라도 버튼 변경되도록..
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
//	else if (FParse::Command(&Cmd, TEXT("RTMP"))) // 테스트에 쓰려고 짧은 버전..
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
//		{ // 이 경우 단순히 상태 On/Off 
//			gDrawPerfMark = !gDrawPerfMark;
//		}
//		else
//		{
//			gDrawPerfMark = true; // PerfMark 시간 기준 지정 시 무조건 켠다.
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
//		// GameplayOnlyLODQuality 변경이 없어도 bUseBladeIIScalabilityOverride 에 따라 인게임 연출 LOD 적용이 달라질 수 있으니 BladeIIPlayer iteration 도..
//		for (FActorIterator ItPlayer(Inworld); ItPlayer; ++ItPlayer)
//		{
//			ABladeIIPlayer* CastedPlayer = Cast<ABladeIIPlayer>(*ItPlayer);
//			if (CastedPlayer)
//			{ // 혹여나 영상 플레이 도중이라 하더라도.. 연출 도중에는 대부분 BladeIIPlayer 가 아닌 SkeletalMeshActor 가 사용되므로 이것을 호출하는 게 영향을 미치지 않는다. 아닌 경우라 하더라도 연출 도중에 pause 메뉴가 나오지도 않고..
//				CastedPlayer->SetupLODInfoForInGame();
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GF"))) // 우리 게임 환경에서 컨텐츠와 결부된 feature 들 On/Off 하면서 성능 측정을 위해. 모바일 디바이스에서 간략하게 커맨드 넣을 수 있도록 짧게 함
//	{
//		FString FirstArgStr;
//		FParse::Token(Cmd, FirstArgStr, true);
//
//		if (FirstArgStr == TEXT("FxParticle"))
//		{
//			bool bWasDisabled = (UAnimNotifyState_TimedParticleEffect::bDisableTimedParticleEffect || UAnimNotify_PlayParticleEffect::bDisablePlayParticleEffect || UB2DamageEffectInfo::bDisableDamageEffectParticle);
//			// 둘이 똑같이 컨트롤 되도록
//			UAnimNotifyState_TimedParticleEffect::bDisableTimedParticleEffect = !bWasDisabled;
//			UAnimNotify_PlayParticleEffect::bDisablePlayParticleEffect = !bWasDisabled;
//			UB2DamageEffectInfo::bDisableDamageEffectParticle = !bWasDisabled; // 이것도 포함인데.. 따로 떼는게 좋나..
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("Particle from AnimNotify/EffectInfo %s"), bWasDisabled ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		if (FirstArgStr == TEXT("FxNotify")) // FxParticle 의 일부
//		{
//			bool bWasDisabled = (UAnimNotifyState_TimedParticleEffect::bDisableTimedParticleEffect || UAnimNotify_PlayParticleEffect::bDisablePlayParticleEffect);
//			// 둘이 똑같이 컨트롤 되도록
//			UAnimNotifyState_TimedParticleEffect::bDisableTimedParticleEffect = !bWasDisabled;
//			UAnimNotify_PlayParticleEffect::bDisablePlayParticleEffect = !bWasDisabled;
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("Particle from AnimNotify/EffectInfo %s"), bWasDisabled ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		if (FirstArgStr == TEXT("DFX")) // FxParticle 의 일부
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
//			// SetHidden 을 해야 적용됨. 다시 켤 때 실제로 숨겨져 있어야 하는 상태라면 문제가 되겠지만.. 테스트하는데 이정도쯤이야
//			for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//			{
//				ABladeIIPlayer* CastedPC = Cast<ABladeIIPlayer>(*ActorIt);
//				if (CastedPC)
//				{ // bHideAllPCs true 해 놓고 한번 숨기면 다시 SetActorHiddenInGame(false) 해도 안 켜짐.
//					CastedPC->SetActorHiddenInGame(ABladeIIPlayer::bHideAllPCs);
//				}
//			}
//			BII_SCREEN_LOG(FString::Printf(TEXT("Player Characters %s"), !ABladeIIPlayer::bHideAllPCs ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstArgStr == TEXT("NPC"))
//		{
//			ABladeIICharacter::bHideAllNPCs = !ABladeIICharacter::bHideAllNPCs;
//			// SetHidden 을 해야 적용됨. 다시 켤 때 실제로 숨겨져 있어야 하는 상태라면 문제가 되겠지만.. 테스트하는데 이정도쯤이야
//			for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//			{
//				ABladeIIPlayer* CastedPC = Cast<ABladeIIPlayer>(*ActorIt);
//				ABladeIICharacter* CastedNPC = Cast<ABladeIICharacter>(*ActorIt);
//				if (CastedNPC && !CastedPC)
//				{ // bHideAllPCs true 해 놓고 한번 숨기면 다시 SetActorHiddenInGame(false) 해도 안 켜짐.
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
//	{ // 쿠킹 버전으로 서버 안 붙은 상태에서 플레이할 때 속도가 이상하게 빨라져서 이렇게라도 궁여지책.
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
//		// 여기서 변경하려는 변수의 ini 항목 읽어들이는 것을 두 InfoAsset 생성자에서 하므로 아직 읽어들이지 않았다면 여기서 해 둠.
//		StaticFindPCClassInfoBox();
//		StaticFindSkillAnimInfo();
//
//		// 둘을 동시에 똑같이 세팅할 수 있도록
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
//	else if (FParse::Command(&Cmd, TEXT("TestLoadAllPCClass"))) // PCClassInfo 로딩되는 양을 줄이는 게 얼마나 도움이 되는지 보기 위함
//	{
//		double fStartTime = FPlatformTime::Seconds();
//		
//		UB2PCClassInfoBox* PCClassInfoBox = StaticFindPCClassInfoBox();
//		int32 FullyLoadedNum = 0;
//		if (PCClassInfoBox)
//		{
//			// 로딩이 안 되어 있다는 가정에서 하는 것임.
//			for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//			{
//				EPCClass ThisClassToLoad = IntToPCClass(PCI);
//
//				UB2PCClassInfo* LoadedClassInfo = PCClassInfoBox->GetSingleClassInfo(ThisClassToLoad);
//				if (LoadedClassInfo)
//				{
//					if (LoadedClassInfo->GetInGameOnlyInfo(Inworld)) // 실제로는 InGameOnlyInfo 까지 로딩하여야 의미 있는 양의 데이터들이 로딩된다.
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
//	else if (FParse::Command(&Cmd, TEXT("TestLoadSkillAnims"))) // PCClassInfo 의 일부로서 로딩되던 SkillInfo 로딩 테스트
//	{
//		bool bLoadAll = FParse::Param(Cmd, TEXT("All")); // 아니면 기본 모험 모드에 필요한 것만
//
//		double fStartTime = FPlatformTime::Seconds();
//
//		TArray<FCombinedPCSkillAnimID> AllSkillAnimIDToLoad;
//		UB2SkillAnimInfo* SkillAnimInfo = StaticFindSkillAnimInfo();
//		if (SkillAnimInfo)
//		{			
//			// 로딩이 안 되어 있다는 가정에서 하는 것임.
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
//					// 기본 모험 모드에 필요한 것 이외에 걸러낸다.
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
//		// 이후 merge 나 BeginPlay 시에 DefaultMaterial 을 세팅하는데 즉석에서 변경도 해 주고.. 
//		for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//		{
//			ABladeIICharacter* CastedChar = Cast<ABladeIICharacter>(*ActorIt);
//			if (CastedChar && CastedChar->GetMesh())
//			{ // bHideAllPCs true 해 놓고 한번 숨기면 다시 SetActorHiddenInGame(false) 해도 안 켜짐.
//				USkeletalMeshComponent* MainMeshComp = CastedChar->GetMesh();
//				for (int32 MI = 0; MI < MainMeshComp->GetNumMaterials(); ++MI)
//				{
//					// 이걸 껐을 때에는 Material override 를 클리어 해 주는데 아마 이렇게 하면 원래 기대되는 걸로 돌아갈 거 같지만 아닐 수도 있다.
//					// 여하간 테스트 기능인 마당에 그렇게까지..
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
//			{ // 캡슐에 붙이는데 0 번은 이후 틱에서 추가 조절
//				B2Player->LightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, B2Player->LightComponentAttachDistZ));
//				B2Player->LightComponent->AttachToComponent(B2Player->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//			}
//			else if(gTestAttachLightCompMode == 2)
//			{
//				// 루트본에 그냥
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
//			// 첫 인자 없음 그냥 On/Off
//			gbTestTurnOffPlayerLight = !gbTestTurnOffPlayerLight;
//			
//			if (B2Player)
//			{
//				// 그냥 갱신 삼아서 콜 하는 거.
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
//		// AspectRatioAdaptiveFOV 에 따른 간단한 테스트.
//		for (TObjectIterator<UCameraComponent> CamIT; CamIT; ++CamIT)
//		{
//			UCameraComponent* ThisComp = Cast<UCameraComponent>(*CamIT);
//			if (ThisComp)
//			{
//				if (bWasSetAdaptiveFOV)
//				{ // AspectRatioAdaptiveFOV 를 사용하려면 일단 Constraint 를 하지 않아야.
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
//		FString FirstArg; // LODGroup 지정
//		FParse::Token(Cmd, FirstArg, true);
//		FString SecondArg; // LODBias 를 얼마나 조절할지 지정
//		FParse::Token(Cmd, SecondArg, true);
//
//		TEnumAsByte<enum TextureGroup> WantedLODGroup = TextureGroup::TEXTUREGROUP_MAX;
//		// UEnum 로다가 enum 문자열을 그대로 맵핑할 수도 있겠지만 그냥 테스트 커맨드이니 필요한 것만 단순히...
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
//		// TEXTUREGROUP_MAX 면 전체 다 하는 걸로?
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
//		FString FirstArg; // 어떤 애니메이션을 할지.. ELobbyRelaxingDoumiState 의 int 캐스팅
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
//		FString FirstArg; // 어떤 애니메이션을 할지.. ELobbyShopGirlState 의 int 캐스팅
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
//				// 로딩 완료 후 핸들링 예약.
//				ItemInfoAsyncLoadTestObj.MarkAsyncLoadWaiting(WantedClass, WantedGrade, RequestedName);
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PLOnOff")))
//	{
//		// 특정 플랫폼의 렌더링 문제 조사 용.
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
//		// 특정 플랫폼의 렌더링 문제 조사 용.
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
//			// 별로 용의선상에 있지는 않지만 그래도 요것도 변경 여지.
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
