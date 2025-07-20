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
#include "BladeIIUtil.h"
//#include "B2UITotemRaffle.h"



UB2StaticExecHelper::UB2StaticExecHelper(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if UE_BUILD_SHIPPING
	bIsAllowedForShipping = true; // 快府 目盖靛父 奖俏呼靛辑 倾侩.. 拱沸 肯傈 弥辆 坷锹滚傈俊辑 BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1 档 0 栏肺 登搁 漏绝捞 促 狐咙.
#endif
}

bool UB2StaticExecHelper::Exec(UWorld* Inworld, const TCHAR* Cmd, FOutputDevice& Ar)
{
#if B2_BUILD_LEVEL == B2_BUILD_LEVEL_LIVE // B2_BUILD_LEVEL_LIVE
	// 奖俏 滚傈俊辑 能贾芒捞 唱啊绰 版快扼档 (矫楷 殿) 捞 捞窍肺 嘿篮 目盖靛绰 角青登瘤 臼档废 茄促. 浚柳 目盖靛 器窃.
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
//	// 咯变 啊阐侥 shipping 己拜狼 呼靛俊档 器窃瞪 鞘夸啊 乐绰 焊促 沥侥 扁瓷阑 备泅且 锭 荤侩.
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
//	FString CultureName;
//#endif
//
//	if (FParse::Command(&Cmd, TEXT("ServerCmd")))
//	{
//		return data_trader::ExecHandler::GetInstance().ExecuteShellCommand(Cmd);
//	}
//
//#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2 // ServerCmd 寇俊绰 LV2 肺 埃林窃
//	else if (FParse::Command(&Cmd, TEXT("CurrentMemStat")) || FParse::Command(&Cmd, TEXT("CMS"))) // Shipping 呼靛俊辑 眉农且 老捞 乐绢辑 咯扁肺 颗败柯促.
//	{
//		FGenericPlatformMemory::DumpStats(Ar);
//
//		// FGenericPlatformMemory::DumpStats 内靛 啊廉客辑 悼老茄 沥焊甫 拳搁俊档 谎妨淋
//		const float InvMB = 1.0f / 1024.0f / 1024.0f;
//		FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
//		const float DrawTextTime = 20.0f;
//		const int32 FontSize = 14;
//		// 焊扁 盔窍绰 开鉴栏肺 持绰促. 临 备盒阑 困秦 茄临 吧矾辑 祸惑 粱 促福霸
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
//		else { // 酒公巴档 瘤沥 救窍搁 弊成 On/Off 
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
//	else if (FParse::Command(&Cmd, TEXT("SuperHero"))) // 弧府弧府 媚何荐哥 抛胶飘窍绊 酵阑 锭 埃祈窍霸..
//	{
//		//ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		//if (B2Player)
//		//{
//		//	bool bCurrentlyInSuperHero = (B2Player->bCheatHeroMode && B2Player->bCheatImmortalMode); // Zombie 客 Hero 啊 悼矫俊 汲沥登绢 乐栏搁 On 惑怕牢 吧肺 埃林窍绊 馋促.
//
//		//	bCurrentlyInSuperHero = !bCurrentlyInSuperHero;
//
//		//	// 咯扁辑何磐 夯拜 技泼. Hero, Zombie, Cooltime 了促.
//		//	B2Player->bCheatHeroMode = bCurrentlyInSuperHero;
//		//	B2Player->bCheatImmortalMode = bCurrentlyInSuperHero;
//
//		//	UB2SkillRTManager* SkillRTManager = B2Player->GetSkillRTManager();
//		//	if (SkillRTManager)
//		//	{ // 摹飘搁 1檬, 酒聪搁 扁夯蔼
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
//		{ // 肺厚 皋浆甸 府橇饭浆. 盔贰 LobbyUpdateCharacterPartClass 矫弊澄篮 碍力 诀单捞飘绰 酒聪骨肺 捞 摹飘啊 官肺 救 冈鳃 荐 乐绰单 捞 摹飘 荤侩 矫 救率俊辑 贸府窍档废 窍看澜.
//			LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(PCI));
//		}
//		// 牢霸烙捞搁 怕弊甫 促矫 窍带瘤.. 酒 皋浆啊 某教捞 登菌栏聪 弊成 饭骇 促矫 肺爹秦具摆促.
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CombatStatScale")))
//	{ // 肺拿 敲饭捞绢 某腐磐俊父 冈洒绰 芭
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
//	{ // 窜刀 敲饭捞 葛靛甫 困茄 摹飘虐 葛澜
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
//			UB2PreRenderer::ClearCompleteState(); // 促矫 Pre-render 蜡惯
//			BII_SCREEN_LOG(FString::Printf(TEXT("PreRenderState Clear")), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstTokenStr == TEXT("FarAway")) // 贸澜 吧 FarAway 肺 林搁 傲 捞芭父 技泼
//		{
//			UB2PreRenderer::bPreRenderAtFarAway = !UB2PreRenderer::bPreRenderAtFarAway;
//			UE_LOG(LogBladeII, Display, TEXT("PreRenderAtFarAway %d"), (int32)UB2PreRenderer::bPreRenderAtFarAway);
//			BII_SCREEN_LOG(FString::Printf(TEXT("PreRenderAtFarAway %d"), (int32)UB2PreRenderer::bPreRenderAtFarAway), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstTokenStr == TEXT("All")) // 葛电 橇府固萍宏 哪器惩飘甸阑 Pre-render 窍绰 可记
//		{
//			UB2PreRenderer::bPreRenderAllPrimitives = !UB2PreRenderer::bPreRenderAllPrimitives;
//			UE_LOG(LogBladeII, Display, TEXT("bPreRenderAllPrimitives %d"), (int32)UB2PreRenderer::bPreRenderAllPrimitives);
//			BII_SCREEN_LOG(FString::Printf(TEXT("bPreRenderAllPrimitives %d"), (int32)UB2PreRenderer::bPreRenderAllPrimitives), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstTokenStr == TEXT("SpawnDist")) // 贸澜 吧 SpawnDist 肺 林搁 歹固甸 spawn 且 芭府 汲沥
//		{
//			FString DistStr;
//			FParse::Token(Cmd, DistStr, true);
//			UB2PreRenderer::PreRenderSpawnDistFromView = FCString::Atof(*DistStr);
//			UE_LOG(LogBladeII, Display, TEXT("PreRenderSpawnDistFromView %f"), UB2PreRenderer::PreRenderSpawnDistFromView);
//			BII_SCREEN_LOG(FString::Printf(TEXT("PreRenderSpawnDistFromView %d"), UB2PreRenderer::PreRenderSpawnDistFromView), FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else
//		{
//			// 酒囱 版快绰 瘤沥茄 箭磊措肺 扁霖 冉荐甸 技泼
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
//	else if (FParse::Command(&Cmd, TEXT("PCSectionMerge"))) // 抛胶飘 档吝俊父 力傍.. 郴瘤绰 沥侥栏肺 备泅等 捞饶俊档 己瓷 抛胶飘甫 困茄 On/Off 敲贰弊啊 瞪 荐档.
//	{
//		++UB2PCMeshSectionMergeInfo::SectionMergeTestFlag; // 1, 2 啊 碍力 荤侩 趣篮 Off
//		if (UB2PCMeshSectionMergeInfo::SectionMergeTestFlag > 2) {
//			UB2PCMeshSectionMergeInfo::SectionMergeTestFlag = 0;
//		}
//
//		UB2CompositeMeshCache* MeshCacheManager = StaticFindCompositeMeshCacher();
//		// 捞 摹飘啊 角力 利侩登绰 霸 粱 救率捞扼 某浆 惑怕甫 厚况具 茄促.
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
//				// 困俊辑 CompoiteMeshCache 甫 厚快聪 LobbyUpdateCharacterPartClass 肺档 绢蠢 沥档鳖瘤绰 碍力 诀单捞飘啊 登绰单 救瞪锭档 乐促.
//				// 捞繁 惑炔俊辑 镜 捞亥飘甫 蝶肺 父甸扁绰 构窍聪 弊成 LobbyGameMode 立辟秦辑 角青窍磊..
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
//	else if (FParse::Command(&Cmd, TEXT("KillAllEnemy"))) // 利促磷烙
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
//		//	// 颇教茄 牢磊甸苞 窃膊 dummy player spawn.
//		//	ABladeIITestDummyPlayer* SpawnedDummy = ABladeIITestDummyPlayer::SpawnWithStandardEquipment(
//		//		Inworld, SpawnClass, NewTransform, EquipGrade, ParsedSpawnFloatingInterval, ParsedSpawnDamageFxInterval,
//		//		bHasWing, MAX_WING_EVOLUTION_GRADE, true // 朝俺 殿鞭篮 傲 咯扁辑..
//		//	);
//
//		//	if (SpawnedDummy)
//		//	{
//		//		if (ParsedAnimStateString == TEXT("Rand") || ParsedAnimStateString == TEXT("Random"))
//		//		{ // 罚待窍霸 敲饭捞
//		//			SpawnedDummy->SetUseRandomAttackState(true);
//		//		}
//		//		else
//		//		{ // 沥秦初绊 窍唱 敲饭捞
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
//		// Off扼绊 疙矫窍瘤臼栏搁 弊成 柳青
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
//		// 摹飘甫 结啊搁辑 敲饭捞 窍绰 巴狼 康氢牢瘤 隔扼档 绢痘霸 秦档 譬配府倔 芒捞 救 摧鳃 锭啊 乐促. 览鞭 炼摹
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
//	else if (FParse::Command(&Cmd, TEXT("ALLog"))) // AssetLoaderLog 趣篮 AsyncLoadLog 构扼绊 秦籍窍电 せ
//	{
//		UB2AssetLoader::bEnableDevLog = !UB2AssetLoader::bEnableDevLog;
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("B2AssetLoader Dev Logging %s"), UB2AssetLoader::bEnableDevLog ? TEXT("ON") : TEXT("OFF")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("NPCAsyncLimit"))) // 捞固 r.PostGameLoadNPCAsyncLoadLimit 能贾疙飞捞瘤父 Shipping 俊辑 抛胶飘窍妨绊.
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
//	else if (FParse::Command(&Cmd, TEXT("ForcedCSMDist"))) // 捞固 r.Shadow.EnforcedStationaryCSMDistance 能贾疙飞捞瘤父 Shipping 俊辑 抛胶飘窍妨绊. 官肺 盎脚 鞘夸档 乐绊.
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
//			// 捞霸 官肺 利侩登妨搁 DirectionalLightComponent 甸阑 茄瞒肥 诀单捞飘 秦 林绢具..
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
//	else if (FParse::Command(&Cmd, TEXT("TestModShadowColor"))) // 捞扒 疙飞 持绊 官肺 诀单捞飘 登绰 芭 焊妨绊..
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
//			// 捞霸 官肺 利侩登妨搁 DirectionalLightComponent 甸阑 茄瞒肥 诀单捞飘 秦 林绢具..
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
//		// 扁扁俊 蝶扼 皑瘤等 蔼栏肺 己瓷 可记 技泼
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
//		// GetActiveProfileName 吝埃 搬苞甸档 免仿窍搁 焊促 档框捞 瞪 淀.
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
//			//FString VulkanVersion = FAndroidMisc::GetVulkanVersion(); 灯绊 せ
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
//		// FIOSPlatformMisc 俊 鞘夸茄 沥焊啊 乐阑 芭 鞍篮单.. 角力 角青 棺 扁瓷 犬牢 窍扁 傈鳖瘤绰..
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
//		// 沥秦柳 InfoAsset 阑 肺爹窍绰 己瓷 抛胶飘
//		// 肺爹捞 救 等 惑怕俊辑 秦具 窃.
//
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		if (FirstArg == TEXT("UI"))
//		{
//			UB2UIManager* UIMgrInst = UB2UIManager::GetInstance();
//
//			//UIMgrInst->UnloadAllTAssets(); 捞芭 窍绊 官肺 GC 倒副 荐 乐栏搁 窍唱 啊瘤绊 咯矾锅 肺爹窍绰 侥捞 瞪 荐档..
//
//			double LoadBeginTime = FPlatformTime::Seconds();
//
//			// 肺爹 饶 风飘悸俊 持菌促芭唱 某教捞 等 扒 捞吧肺 沥犬茄 抛胶飘啊 阂啊瓷. 措惑篮 利例洒 急沥..
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
//		/** CBTVersionReady 啊 true 牢 惑怕肺 捻欧 抛胶飘且 锭 牧刨明啊 阜囚辑 寸炔胶矾款 惑炔 措厚. */
//		extern bool gbAllowNonCBTVersionContentOverride;
//		gbAllowNonCBTVersionContentOverride = !gbAllowNonCBTVersionContentOverride;
//
//		UB2StageInfo* StageInfoTable = StaticFindStageInfo();
//		if (StageInfoTable)
//		{ // 盎脚秦 林绢具 窍绰 霸 乐绢辑..
//			StageInfoTable->SetDataByCBTVersionContent();
//		}
//
//		BII_SCREEN_LOG(FString::Printf(TEXT("AllowNonCBTVersionContentOverride %s."), gbAllowNonCBTVersionContentOverride ? TEXT("On") : TEXT("Off")),
//			FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TAATest"))) // TemporalAA On/Off 抛胶飘 侩
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
//	else if (FParse::Command(&Cmd, TEXT("FXAATest"))) // FXAA On/Off 抛胶飘 侩.. 辟单 捞芭 葛官老辑 救 冈塞?
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
//		// 泅犁 备泅捞 谍 芭 鞘夸绝捞 r.MobileMSAA 捞芭 窍唱肺 促 冈洒霸 登绢 乐澜.
//		static IConsoleVariable* _CVarMobileMSAA = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileMSAA"));
//		if (_CVarMobileMSAA)
//		{
//			const int32 CurMSAAValue = _CVarMobileMSAA->GetInt();
//			int32 NewMSAAValue = FCString::Atoi(*FirstArg);
//			if (!bHasFirstArg)
//			{
//				// 瘤沥窍瘤 臼疽促搁 On/Off 荐霖栏肺.. 扁夯 基敲蔼篮 4肺 窃.
//				NewMSAAValue = ((CurMSAAValue <= 1) ? 4 : 1);
//			}
//			else if (NewMSAAValue != 1 && NewMSAAValue != 2 && NewMSAAValue != 4 && NewMSAAValue != 8)
//			{ // 抗寇贸府.
//				NewMSAAValue = 1; // 1 捞 掺绰 芭.
//			}
//
//			// %%%%%%%%%%%%%%%%%%%%%
//			// 咯扁辑 技泼茄 扒 B2Scalability 率 牧飘费肺 牢秦 促澜 肺爹 技记 逞绢辑鳖瘤 瘤加登瘤 臼阑 巴捞促.
//			// 捞 目盖靛肺 惑技茄 抛胶飘甫 窍妨搁 B2Scalability 率俊辑狼 力绢甫 波 初档废.
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
//		// EB2ResolutionLevel 篮 UI 窜阑 困茄 牢磐其捞胶捞绊 EB2GraphicsRQType 阑 泅犁 倾侩 咯何 包拌绝捞 碍力肺 技泼窍绰 扁瓷捞 乐栏搁 亮阑 淀.
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
//			// AdjustScalabilityBySavedUserSelection 俊辑 历厘茄 蔼阑 阂矾棵 抛聪 历厘捞 鞘夸..
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
//		// PC 滚傈 CVar 绰 蝶肺 乐澜. 捞扒 葛官老 奖俏 抛胶飘 硅器俊辑 静扁 困窃.
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
//			{ // 葛官老俊急 开且阑 给且 淀..
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
//		// 箭磊 滴 俺 疙矫窍搁 Lower / Upper 肺, 茄 俺父 疙矫窍搁 Upper 肺
//		float SpecifiedUpperBound = (fSecondValue > 0.0f) ? fSecondValue : fFirstValue;
//		float SpecifiedLowerBound = (fSecondValue > 0.0f) ? fFirstValue : 0.0f;
//
//		if (GEngine && SpecifiedUpperBound > 0.0f)
//		{
//			// 抛胶飘 侩捞聪 弊成 促 度鞍捞 官层魔. 捞犯霸 窍绊 唱搁 r.CurrentMobileFrameLimitLevel 啊 狼固啊 绝摆瘤 せ
//
//			// 角力 霖厚登绢 乐绰 SmoothedFrameRateRange_Mobile 技泼焊促 奴 牢郸胶俊辑绰 力老 奴 吧 荤侩窍骨肺 犬厘 啊瓷己阑 积阿秦辑 利寸洒 奴 箭磊鳖瘤 iteration 窃.
//			for (int32 SFI = 0; SFI < 10; ++SFI) 
//			{
//				// 郴何利栏肺 0 捞唱 澜荐俊辑绰 泅犁 蔼阑 蜡瘤窍霸 瞪 巴.
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
//	{ // GEngine->SmoothedFrameRateRange_Mobile** 甸狼 蔼阑 流立 扒靛府瘤 臼绊 
//		// 角力 狼档茄 扁瓷措肺 SmoothedFrameRateRange_Mobile** 吝 窍唱甫 急琶窍绰 饭骇 函荐甫 函版.
//		IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.CurrentMobileFrameLimitLevel"));
//		if (CVar)
//		{
//			FString FirstArg;
//			FParse::Token(Cmd, FirstArg, true);
//
//			if (FirstArg == TEXT("Low") || FirstArg == TEXT("Mid") || FirstArg == TEXT("High"))
//			{ // 霸烙 皋春俊辑 荤侩且 狼档肺 力傍登绰 惑措利牢 蔼栏肺狼 技泼 牢磐其捞胶
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
//			{ // 流立 蔼 技泼
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
//		// 埃趣 传俊 剁绰 巩力俊 措茄 抛胶飘 瞒盔牢单.. 捞霸 秦搬 规救捞 登绰 霸 嘎栏搁 沥侥 盲琶.
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
//	{ // 匙飘况农 楷搬 救等 惑怕俊辑 肺厚 漂沥 厘搁栏肺 捞悼窍扁 困秦 力傍.
//		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::CharacterSelectPage);
//		return true;
//	}	
//	else if (FParse::Command(&Cmd, TEXT("GoToLobbyMain")))
//	{ // 匙飘况农 楷搬 救等 惑怕俊辑 肺厚 漂沥 厘搁栏肺 捞悼窍扁 困秦 力傍.
//		LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::LobbyMain);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GoToLobbyShop")))
//	{ // 匙飘况农 楷搬 救等 惑怕俊辑 肺厚 漂沥 厘搁栏肺 捞悼窍扁 困秦 力傍.
//		LobbyChangeSceneClass<ELobbyScene>::GetInstance().Signal(ELobbyScene::ELobbyScene_SummonItem);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GoToLobbyInventory")))
//	{ // 匙飘况农 楷搬 救等 惑怕俊辑 肺厚 漂沥 厘搁栏肺 捞悼窍扁 困秦 力傍.
//		LobbyEnterHeroMgmtModeClass<EHeroMgmtSubMode>::GetInstance().Signal(EHeroMgmtSubMode::EHMSM_Inventory);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PrepareLobbyHallOfFame")))
//	{ // 弊成篮 给甸绢啊瘤绰单 沥焊甫 粱 盲况持绢具.. 捞扒 努扼 窜刀篮 酒聪绊 辑滚 嘿篮 惑炔俊辑.
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddRankingStatus 2 3 10 100"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddRankingStatus 3 3 10 100"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddRankingStatus 5 3 10 100"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("forceSettlement 2"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("forceSettlement 3"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("forceSettlement 5"));
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestStage")))
//	{ // Standalone 胶抛捞瘤 敲饭捞甫 粱 歹 埃祈洒 窍扁 困秦. 寇何 傈崔 抛胶飘 呼靛俊辑档 鞘夸窍扁档 窍绊..
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		const int32 WantedStageNum = FCString::Atoi(*FirstArg);
//
//		FString SecondArg;
//		FParse::Token(Cmd, SecondArg, true);
//		const bool bScenarioModeOn = (SecondArg == TEXT("-SM") || SecondArg == TEXT("-Scenario"));
//
//		// ScenarioMode 甫 瘤沥窍搁 抄捞档绰 公炼扒 Normal 捞 凳. 抄捞档甫 瘤沥窍绊 酵篮 版快档 滴锅掳 牢磊肺.
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
//			{ // 荤角惑 Open 疙飞栏肺 且 荐档 乐促. 促风扁 蓖满绊 鳖促肺况辑 焊促 埃祈茄 疙飞阑 力傍窍妨绰 芭.
//				OpenStageCommand = FString::Printf(TEXT("Open %s?StageNum=%d?SM=%d?DF=%d?Standalone") ,
//					*WantedStageInfoData->MainMap.ToString(),
//					WantedStageNum, (int32)bScenarioModeOn, (int32)WantedDifficulty
//				);
//			}
//		}
//
//		GlobalSetPureMapLoadingShareValue(0.5f); // 累篮 结厚胶.
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
//	{ // Standalone 敲饭捞 鞍篮 惑炔俊辑 某腐磐 急琶窍绊磊 且 锭
//		FString FirstArg;
//		FParse::Token(Cmd, FirstArg, true);
//		const EPCClass MainClass = IntToPCClass(FCString::Atoi(*FirstArg));
//		
//		FString SecondArg;
//		FParse::Token(Cmd, SecondArg, true);
//		const EPCClass SubClass = IntToPCClass(FCString::Atoi(*SecondArg));
//
//		if (SubClass != EPCClass::EPC_End && SubClass != MainClass) 
//		{ // Sub 甫 刚历 技泼窍绰 捞蜡绰 拳搁俊 肺弊 谎副 锭 捞霸 酒贰肺 剐府霸 窍妨绊
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
//		// "SkillCoolTime" 苞 内靛啊 般摹瘤父 俺惯 摹飘捞绊 咯扁俊 蝶肺 窃荐 父靛绰 巴档 备龙备龙窍聪 傲 逞绢啊磊
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
//		// 芭狼 己瓷 抛胶飘 侩档肺辑 胶懦 辆幅唱 酿鸥烙 沥秦初绊 啊绰 芭.		
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
	// 俺惯侩 目盖靛捞瘤父 葛官老 抛胶飘俊辑 荤侩且 芭扼搁 酒贰 ExecDev 俊 备泅钦聪促.

	// .paper2dsprites 器杆狼 胶橇扼捞飘 家胶 颇老俊辑 阿 胶橇扼捞飘 康开 沥焊 啊廉客辑 沥秦柳 捞抚狼 MIC 俊 乐绰 atlas 颇扼固磐俊 持绢 林绰 扁瓷.
	// 荤侩 抗矫 : B2UIAtlasParam Sprite=D:\DepotWS\BII-Dev\B2_TexAtlas_InGameHUD_PC.paper2dsprites MICPath=HUD/Materials AtlasTexture=/Game/HUD/Texture/PCHUDAtlas/Textures/B2_TexAtlas_InGameHUD_PC.B2_TexAtlas_InGameHUD_PC 
	// AtlasTexture_PCPreview=/Game/DevOnly/B2_TexAtlas_InGameHUD_PC_sRGB_PCTemp.B2_TexAtlas_InGameHUD_PC_sRGB_PCTemp
//	if (FParse::Command(&Cmd, TEXT("B2UIAtlasParam")))
//	{
//		FString SpriteName;
//		if (FParse::Value(Cmd, TEXT("Sprite="), SpriteName)) // 胶橇扼捞飘 盔夯 家胶 颇老 困摹. **.paper2dsprites
//		{
//			FString MICPath;
//			FParse::Value(Cmd, TEXT("MICPath="), MICPath); // /Game/.../MIC颇老捞抚 荤捞狼 版肺
//
//			FString OptionalAtlasTexturePath; // 咆胶媚绰 扁夯 蔼栏肺 甸绢埃 吧 静带瘤, 酒聪搁 蝶肺 持阑 荐档 乐摆促.
//			FParse::Value(Cmd, TEXT("AtlasTexture="), OptionalAtlasTexturePath);
//			UTexture2D* OptionalAtlasTexture = LoadObject<UTexture2D>(NULL, *OptionalAtlasTexturePath);
//
//			TArray<FString> SpriteFileContent;
//			FFileHelper::LoadANSITextFileToStrings(*SpriteName, &IFileManager::Get(), SpriteFileContent); // 亮篮 蜡瓶府萍肺焙 偿偿
//
//			// 傈眉 荤捞令何磐
//			int32 TextureWidth = 0;
//			int32 TextureHeight = 0;
//			for (int32 FCI = SpriteFileContent.Num() - 1; FCI >= 0; --FCI) // 捞 沥焊绰 酒付档 颇老 窍窜俊 乐阑 巴烙.
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
//				// 阿 胶橇扼捞飘 康开喊肺 捞抚 沥焊啊 刚历 颇教登绢具 窃.
//				FString CurrSpriteName;
//				FString CurrMatchingMICName;
//				FString CurrMatchingMICName2; // For additional naming rule
//				FString CurrMatchingSpriteName; // We might need sprite too for use of image draw batching in wrap-box.
//				for (int32 FCI = 0; FCI < SpriteFileContent.Num(); ++FCI)
//				{
//					FString CurrLine = SpriteFileContent[FCI];
//
//					if (CurrLine.StartsWith(TEXT("\"")) && CurrLine.EndsWith(TEXT("\":"))) // 胶橇扼捞飘 捞抚 扼牢狼 器杆
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
//						// MIC 捞抚俊辑绰 第率 犬厘磊 力芭窍绊 沥秦柳 立滴绢 棺 瘤沥等 眠啊 版肺甫 嘿牢促.
//						int32 ExtensionStartIndex = ExtRemovedName.Find(TEXT("."));
//						if (ExtensionStartIndex != INDEX_NONE)
//						{
//							ExtRemovedName.RemoveAt(ExtensionStartIndex, 4); // e.g. Removing ".png", ".tga"
//
//						}
//						// Then, add our naming convention.
//						CurrMatchingMICName = TEXT("MAT_") + ExtRemovedName;
//						CurrMatchingMICName2 = TEXT("MAT_S_") + ExtRemovedName; // 扁夯利牢 捞固瘤 谎府绰 捞寇狼 距埃 漂荐茄 瓤苞啊 甸绢埃 巴甸牢单, 捞巴甸篮 措眉肺 material 窍唱寸 MIC 窍唱父 父甸绢辑 静霸 瞪 巴. 咆胶媚 基敲傅捞 眠啊登歹扼档 扁夯利牢 酒撇扼胶 颇扼固磐绰 老窜 付蛮啊瘤老 巴.
//						CurrMatchingSpriteName = TEXT("MAT_Spr_") + ExtRemovedName; // Material 狼 老辆栏肺 摹磊..
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
//							// NoAtlas 技泼茄 扒 捞 目盖靛俊辑 力寇秦具 茄促. 匙捞怪捞 贸澜何磐 捞犯霸 甸绢啊 乐栏搁 救登瘤父.. Atlas - NonAtlas 傈券 苞沥俊 乐阑 荐档 乐栏骨肺 眉农.
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
//							//	// UI TexAtlas master material 俊 持绢初篮 颇扼固磐 捞抚
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
//						// 4.14 捞饶.. WrapBox 救俊辑狼 弥利拳甫 困秦 Paper2D Sprite 档 瘤盔.
//						UPaperSprite* MatchingPaperSprite = LoadObject<UPaperSprite>(NULL, *CurrMatchingSpriteName);
//						// 傲 扁夯 匙捞怪档 眉农秦 焊磊. 颊秦杭 扒 绝栏聪
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
//	{ // 烙矫 扁瓷栏肺辑 扁粮俊 Atlas 咆胶媚肺 荤侩窍带 芭 俺喊肺 律俺柳 咆胶媚肺 官操档废 窍妨绊 老褒 贸府. 
//		// 咯矾锅 荤侩且 荐档 乐绢辑 曼绊伙酒 持绢狄.
//		FString MICPath;
//		FParse::Value(Cmd, TEXT("MICPath="), MICPath); // /Game/.../MIC颇老捞抚 荤捞狼 版肺
//		FString MICNameFilter;
//		FParse::Value(Cmd, TEXT("MICNameFilter="), MICNameFilter);
//
//		FString TexturePath;
//		FParse::Value(Cmd, TEXT("TexturePath="), TexturePath); // /Game/.../MIC颇老捞抚 荤捞狼 版肺
//		FString TextureNameFilter;
//		FParse::Value(Cmd, TEXT("TextureNameFilter="), TextureNameFilter);
//
//		TArray<UMaterialInstanceConstant*> AllMICs;
//		EditorFindAndLoadAssets<UMaterialInstanceConstant>(MICPath, MICNameFilter, AllMICs, false); // 扁粮俊 持篮 镑俊 窍困 弃歹啊 乐绢辑 recursive 窍瘤 臼霸.. 鞘夸俊 蝶扼 促福霸 持带瘤
//
//		TArray<UTexture2D*> AllTextures;
//		EditorFindAndLoadAssets<UTexture2D>(TexturePath, TextureNameFilter, AllTextures, false);
//
//		// AllMICs 尔 AllTextures 尔 荐啊 鞍篮瘤 沥档.. 酒公贰档 促甫 芭 鞍篮单..
//		UE_LOG(LogBladeII, Log, TEXT("Found %d MICs from %s, %d Textures from %s"), AllMICs.Num(), *MICPath, AllTextures.Num(), *TexturePath);
//
//		for (int32 MI = 0; MI < AllMICs.Num(); ++MI)
//		{
//			UMaterialInstanceConstant* ThisMIC = AllMICs[MI];
//			UTexture* MatchingTexture = NULL;
//			if (ThisMIC)
//			{
//				// 悼老茄 匙捞怪狼 咆胶媚甫 茫酒辑.. 乐栏搁 颇扼固磐 技泼. 绝栏搁 逞绢啊档废.
//
//				FString ThisName = ThisMIC->GetName();
//				if (ThisName.StartsWith(TEXT("MAT_"))) // 捞固 MICNameFilter 肺 吧矾尘 荐档 乐瘤父.
//				{
//					ThisName.RemoveFromStart(TEXT("MAT_"));
//					ThisName = TEXT("TEX_") + ThisName;
//
//					// 促矫 LoadObject 且 荐档 乐摆瘤父.. 捞 目盖靛 荤侩 格利 惑 茫酒辰 芭俊辑.
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
//				{ // 夯拜 技泼 累诀. StaticSwitchParameter 啊 捞犯霸扼档 函版登聪 促青牢单 谍 牢磐其捞胶 绝唱?
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
//					ThisMIC->SetTextureParameterValueEditorOnly(MtrlParamName_UIAtlasTexture, nullptr); // 扁粮 波 饭欺繁胶甫 朝妨具摆促. 弊贰具 serialize 甫 救 窍瘤.
//					ThisMIC->SetTextureParameterValueEditorOnly(MtrlParamName_NoAtlasSingleTexture, MatchingTexture);
//					ThisMIC->MarkPackageDirty();
//				}
//				else
//				{
//					// 犬牢秦 杭 鞘夸啊 乐促.. 辟单 粱 腹篮单. 酒流 促 给律暗 扒啊
//					UE_LOG(LogBladeII, Warning, TEXT("[AutoReplaceAtlasTexture] Cannot Find matching texture for MIC %s !"), *ThisMIC->GetPathName())
//				}
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ProcessImportedUITextures")))
//	{ // 构 措樊栏肺 贸府窍妨绊
//		FString TexturePath;
//		FParse::Value(Cmd, TEXT("TexturePath="), TexturePath); // /Game/.../MIC颇老捞抚 荤捞狼 版肺
//		FString TextureNameFilter;
//		FParse::Value(Cmd, TEXT("TextureNameFilter="), TextureNameFilter);
//
//		TArray<UTexture2D*> AllTextures;
//		EditorFindAndLoadAssets<UTexture2D>(TexturePath, TextureNameFilter, AllTextures, true);
//
//		UE_LOG(LogBladeII, Log, TEXT("Found %d Textures from %s"), AllTextures.Num(), *TexturePath);
//
//		// 促矫 LoadObject 且 荐档 乐摆瘤父.. 捞 目盖靛 荤侩 格利 惑 茫酒辰 芭俊辑.
//		for (UTexture* TextureFromList : AllTextures)
//		{
//			if (TextureFromList)
//			{
//				TextureFromList->LODGroup = TextureGroup::TEXTUREGROUP_UI;
//				TextureFromList->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps; // UI 档 荤剧 蝶扼辑 MipLODBias 林搁 亮变 茄单.. 胶飘府怪 登绰 扒 酒聪扼辑.. 葛福摆促.
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
//	{ // NoAtlas 肺 档吝俊 官槽 巴甸捞 绢瞒乔 扁粮 饭欺繁胶啊 巢酒乐促 焊聪.. 捞吧 力芭窍瘤 臼栏搁 家侩捞 绝促..
//		FString MICPath;
//		FParse::Value(Cmd, TEXT("MICPath="), MICPath); // /Game/.../MIC颇老捞抚 荤捞狼 版肺
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
//					UTexture* ExistingAtlasTexParamValue = NULL; // 捞扒 傲 肺彪侩
//					ThisMIC->GetTextureParameterOverrideValue(MtrlParamName_UIAtlasTexture, ExistingAtlasTexParamValue);
//
//					if (ExistingAtlasTexParamValue)
//					{
//						++ParamChangedCount;
//						ThisMIC->SetTextureParameterValueEditorOnly(MtrlParamName_UIAtlasTexture, nullptr); // NoAtlas switch 啊 难廉 乐栏搁 绢瞒乔 荤侩 救窍绰 咆胶媚绊 镜单绝捞 皋葛府父 冈栏聪 朝赴促.
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
//			// 老何 NavMesh 俊 康氢阑 固摹瘤 富酒具 且 努贰胶甸俊 措茄 CanEverAffectNavigation 磊悼 技泼 目盖靛
//			for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//			{
//				// 汲沥捞 鞘夸茄 努贰胶甸. 鞘夸窍搁 眠啊.
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
//		// B2Button 背眉甫 困茄 目盖靛
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
//					TEXT("%s 狼 SafeZone 阑 磊悼栏肺 积己窍看嚼聪促.\r\n\r\n")
//					TEXT("Root 酒贰俊 硅摹登绢 乐带 %d 俺狼 widget 甸捞 货肺 积己等 SafeZone 酒贰狼 PanelWidget栏肺 颗败脸嚼聪促.\r\n\r\n")
//					TEXT("WidgetBP甫 凯绊 哪颇老 饶 饭捞酒眶阑 犬牢秦 焊技夸."),
//					*FoundWigetBP->GetPathName(), MovedChildrenNum
//				)));
//#endif
//			}
//			else
//			{
//#if !PLATFORM_MAC
//				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(
//					TEXT("%s 狼 SafeZone 磊悼 积己捞 角菩沁嚼聪促.\r\n\r\n")
//					TEXT("捞固 SafeZone 捞 乐芭唱, 弥惑窜 RootWidget 狼 鸥涝捞 漂荐茄 巴捞 盔牢老 荐 乐嚼聪促."),
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
//		// ReplaceSubWidgetClass 甫 弃歹 掳肺. 例措 版肺肺 瘤沥茄促.
//		FString SearchFolder;
//		FParse::Token(Cmd, SearchFolder, true);
//		SearchFolder = SearchFolder.Replace(TEXT("\\"), TEXT("/")); // 啊瓷茄 弊成 浇贰浆甫 荤侩窍绰 霸 亮阑 巴烙.
//		FPaths::RemoveDuplicateSlashes(SearchFolder);
//
//		TArray<FString> FoundWidgetFiles;
//		IFileManager::Get().FindFiles(FoundWidgetFiles, *SearchFolder, TEXT(".uasset"));
//		
//		// 例措版肺甫 霸烙 版肺肺 官槽促. 捞芭 蜡瓶捞 绢叼 乐绰瘤 葛福摆绰单..
//		FString LoadFolder = EditorGetGameRelativePathFromAbsPath(SearchFolder);
//
//		for (FString ThisFile : FoundWidgetFiles)
//		{
//			ThisFile.RemoveFromEnd(TEXT(".uasset"));
//			FString ThisWidgetPath = TEXT("/") + LoadFolder + TEXT("/") + ThisFile + TEXT(".") + ThisFile; // 俊悸 酒捞能俊 Ctrl+C 窍搁 汗荤登绰 版肺 牧亥记栏肺 嘎眠扁.
//
//			UWidgetBlueprint* FoundWigetBP = LoadObject<UWidgetBlueprint>(NULL, *ThisWidgetPath);
//			if (FoundWigetBP)
//			{
//#if !PLATFORM_MAC
//				ReplaceSubWidgetClass(FoundWigetBP, UButton::StaticClass(), UB2Button::StaticClass(), true); // 捞扒 皋矫瘤 冠胶 救剁快绊 角青.
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
//		SearchFolder = SearchFolder.Replace(TEXT("\\"), TEXT("/")); // 啊瓷茄 弊成 浇贰浆甫 荤侩窍绰 霸 亮阑 巴烙.
//		FPaths::RemoveDuplicateSlashes(SearchFolder);
//
//		TArray<FString> FoundWidgetFiles;
//		IFileManager::Get().FindFiles(FoundWidgetFiles, *SearchFolder, TEXT(".uasset"));
//
//		// 例措版肺甫 霸烙 版肺肺 官槽促. 捞芭 蜡瓶捞 绢叼 乐绰瘤 葛福摆绰单..
//		FString LoadFolder = EditorGetGameRelativePathFromAbsPath(SearchFolder);
//
//		for (FString ThisFile : FoundWidgetFiles)
//		{
//			ThisFile.RemoveFromEnd(TEXT(".uasset"));
//			FString ThisWidgetPath = TEXT("/") + LoadFolder + TEXT("/") + ThisFile + TEXT(".") + ThisFile; // 俊悸 酒捞能俊 Ctrl+C 窍搁 汗荤登绰 版肺 牧亥记栏肺 嘎眠扁.
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
//	{ // 概快 漂荐茄 荤沥 锭巩俊 持霸 等 巴. UI 府春倔 窍搁辑 悼老茄 捞抚狼 赣萍府倔甸阑 促弗 版肺俊 父甸绢初绊 瘤陛鳖瘤 父电 巴甸 官层摹扁 窍妨绰 惑炔牢单 捞犯霸 窍妨聪 redirector 鞍篮 贸府啊 登绰 巴档 酒聪绊..
//		// 搬惫 荐累诀捞 鞘夸茄单 啊瓷茄 急鳖瘤父 磊悼栏肺 贸府 矫档.
//		FString WidgetSearchPath;
//		FParse::Value(Cmd, TEXT("WidgetPath="), WidgetSearchPath);
//		FString WidgetSearchNameFilter;
//		FParse::Value(Cmd, TEXT("WidgetFilter="), WidgetSearchNameFilter);
//
//		FString OldAssetPath; // 措眉窍绊磊 窍绰 捞傈 版肺 巩磊
//		FParse::Value(Cmd, TEXT("OldPath="), OldAssetPath);
//		FString NewAssetPath; // OldMaterialPath 啊 捞吧肺 措眉等 府家胶甫 茫酒辑 技泼且 巴烙.
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
//			// 舅绊 乐绰 鸥涝狼 府家胶父. 咯扁急 荤角 咆胶媚尔 赣萍府倔 酒聪搁.. 其捞欺 胶橇扼捞飘档 乐焙.
//			// 弊巴焊促 ReplaceSubWidgetResource 救俊辑 贸府窍绰 widget type 捞 力茄利老 巴烙. 绢蠢 沥档 荐累诀篮 绢驴 荐 绝促.
//			int32 ModifiedCount = ReplaceSubWidgetResources<UMaterialInterface>(ThisWB, OldAssetPath, NewAssetPath);
//			ModifiedCount += ReplaceSubWidgetResources<UTexture>(ThisWB, OldAssetPath, NewAssetPath);
//			ModifiedCount += ReplaceSubWidgetResources<UPaperSprite>(ThisWB, OldAssetPath, NewAssetPath);
//#endif
//			if (ModifiedCount > 0)
//			{
//#if !PLATFORM_MAC
//				AllModifiedWidgets.Add(FString::Printf(TEXT("%s, %d 俺"), *ThisWB->GetPathName(), ModifiedCount));
//#endif
//				ThisWB->MarkPackageDirty();
//			}
//		}
//		if (AllModifiedWidgets.Num())
//		{
//#if !PLATFORM_MAC
//			FString MsgStr = TEXT("促澜 Widget Blueprint 甸俊辑 府家胶甸阑 函版\r\n官诧 Widget Blueprint 甸篮 哪颇老 棺 历厘阑 秦 林技夸!\r\n\r\n");
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
//	{ // 饭骇诀 楷免 抛胶飘 技泼.
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
//	{ // 碍拳 楷免 抛胶飘 技泼.
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
//	{ // 钦己 楷免 抛胶飘 技泼.
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		if (LobbyGM && LobbyGM->GetLobbyInventory())
//		{
//			LobbyGM->GetLobbyInventory()->DevOnlyItemCUSProgressSceneTest(ELobbyInvenItemOpMode::EIVIOP_Composition);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestItemUpgradeScene")))
//	{ // 铰鞭 楷免 抛胶飘 技泼.
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		if (LobbyGM && LobbyGM->GetLobbyInventory())
//		{
//			LobbyGM->GetLobbyInventory()->DevOnlyItemCUSProgressSceneTest(ELobbyInvenItemOpMode::EIVIOP_Upgrade);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("TestItemSurpassScene")))
//	{ // 檬岿 楷免 抛胶飘 技泼.
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
//		{ // On/Off 胶困摹绰 咯鸥 牢磊 瘤沥救茄 版快父.
//			GbEditorPVPGamePreview = !GbEditorPVPGamePreview;
//		}
//		else
//		{
//			GbEditorPVPGamePreview = true; // 瘤沥捞 登菌促搁 公炼扒 难绰 芭
//
//			GEditorPVPPreviewMyClassMain = IntToPCClass(FMath::Clamp(FCString::Atoi(*MyMainClassStr), 0, GetMaxPCClassNum() - 1));
//			GEditorPVPPreviewMyClassTag = IntToPCClass(FMath::Clamp(FCString::Atoi(*MyTagClassStr), 0, GetMaxPCClassNum() - 1));
//			GEditorPVPPreviewRemoteClassMain = IntToPCClass(FMath::Clamp(FCString::Atoi(*RemoteMainClassStr), 0, GetMaxPCClassNum() - 1));
//			GEditorPVPPreviewRemoteClassTag = IntToPCClass(FMath::Clamp(FCString::Atoi(*RemoteTagClassStr), 0, GetMaxPCClassNum() - 1));
//		}
//
//#if !PLATFORM_MAC
//		FString MsgStr = FString::Printf(TEXT("PVP 葛靛 俊叼磐 傈侩 橇府轰 %s."), GbEditorPVPGamePreview ? TEXT("难咙") : TEXT("波咙"));
//		if (GbEditorPVPGamePreview)
//		{
//			MsgStr += FString::Printf(TEXT("\n\n肺拿 敲饭捞绢 皋牢 某腐磐: %d"), (int32)GEditorPVPPreviewMyClassMain);
//			MsgStr += FString::Printf(TEXT("\n肺拿 敲饭捞绢 怕弊 某腐磐: %d"), (int32)GEditorPVPPreviewMyClassTag);
//			MsgStr += FString::Printf(TEXT("\n盔拜惑措 敲饭捞绢 皋牢 某腐磐: %d"), (int32)GEditorPVPPreviewRemoteClassMain);
//			MsgStr += FString::Printf(TEXT("\n盔拜惑措 敲饭捞绢 怕弊 某腐磐: %d"), (int32)GEditorPVPPreviewRemoteClassTag);
//			MsgStr += TEXT("\n\nPVP 葛靛 俊叼磐 橇府轰绰 阂救沥且 荐 乐栏聪 敲饭捞 傈俊 累诀拱阑 历厘秦 林技夸.");
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
//			// 橇府轰 胶困莫
//			g_IsEditorRaidGamePreview = !g_IsEditorRaidGamePreview;
//		}
//		else
//		{
//			g_IsEditorRaidGamePreview = true; // 瘤沥捞 登菌促搁 公炼扒 难绰 芭
//
//			g_EditorRaidPreviewPCClassMain[0] = IntToPCClass(FMath::Clamp(FCString::Atoi(*PCClassStr_0), 0, GetMaxPCClassNum() - 1));
//			g_EditorRaidPreviewPCClassMain[1] = IntToPCClass(FMath::Clamp(FCString::Atoi(*PCClassStr_1), 0, GetMaxPCClassNum() - 1));
//			g_EditorRaidPreviewPCClassMain[2] = IntToPCClass(FMath::Clamp(FCString::Atoi(*PCClassStr_2), 0, GetMaxPCClassNum() - 1));
//			g_EditorRaidPreviewPCClassMain[3] = IntToPCClass(FMath::Clamp(FCString::Atoi(*PCClassStr_3), 0, GetMaxPCClassNum() - 1));
//		}
//
//#if !PLATFORM_MAC
//		FString MsgStr = FString::Printf(TEXT("[俊叼磐 傈侩] 饭捞靛 橇府轰 : %s."), g_IsEditorRaidGamePreview ? TEXT("难咙") : TEXT("波咙"));
//		if (g_IsEditorRaidGamePreview)
//		{
//			MsgStr += FString::Printf(TEXT("\n\n肺拿 敲饭捞绢 - 皋牢 某腐磐: %d"), (int32)g_EditorRaidPreviewPCClassMain[0]);
//			MsgStr += FString::Printf(TEXT("\n\n肺拿 敲饭捞绢 - 1锅 颇萍盔 某腐磐: %d"), (int32)g_EditorRaidPreviewPCClassMain[1]);
//			MsgStr += FString::Printf(TEXT("\n\n肺拿 敲饭捞绢 - 2锅 颇萍盔 某腐磐: %d"), (int32)g_EditorRaidPreviewPCClassMain[2]);
//			MsgStr += FString::Printf(TEXT("\n\n肺拿 敲饭捞绢 - 3锅 颇萍盔 某腐磐: %d"), (int32)g_EditorRaidPreviewPCClassMain[3]);
//			MsgStr += TEXT("\n\n饭捞靛 橇府轰 葛靛 俊叼磐 橇府轰绰 阂救沥且 荐 乐栏聪 敲饭捞 傈俊 累诀拱阑 历厘秦 林技夸.");
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
//		// 绢蠢 朝牢啊 惯积茄 舅 荐 绝绰 巩力痢栏肺 牢秦.. 康惑 嘛扁 困茄 芭扼 捞犯霸 烙矫 措贸肺..
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
//	{ // 俺惯 苞沥俊辑狼 抛胶飘 目盖靛烙
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
//				FString MsgStr = FString::Printf(TEXT("WidgetBlueprint %s 俊辑 (弥家) %d俺狼 咆胶媚啊 荤侩登绊 乐嚼聪促.\n\n"), *FoundWigetBP->GetName(), RefTextures.Num());
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
//	{ // 俊叼磐俊辑 DialogInfo 抛捞喉 累诀 饶俊 角龙利牢 盎脚捞 救凳.. 措脚 捞犯霸扼档..,
//		// 酒, 傲 UEditorEngine::PlayInEditor 俊辑 捞芭 龋免窍霸..
//		BladeIIGameImpl* B2GameModule = FModuleManager::GetModulePtr<BladeIIGameImpl>(BladeIIGameImpl::GetModuleName());
//		if (B2GameModule)
//		{
//			B2GameModule->DestroyDialogInfoTable();
//			B2GameModule->GetDialogInfoTable();
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("DummyClearReward")))
//	{ // 老馆 俊叼磐 敲饭捞俊辑 努府绢 焊惑 酒捞袍 楷免 焊扁 困秦
//		FString RewardNumStr;
//		FParse::Token(Cmd, RewardNumStr, true);
//		int32 RewardNum = FCString::Atoi(*RewardNumStr);
//		
//		RewardNum = FMath::Clamp(RewardNum, 1, 5);
//
//		bool bIsDoubled = (FMath::RandRange(0, 2) == 0);// 焊呈胶 惑炔 葛狼
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
//				// 俺荐蝶扼 grade 肺.. 窜, 捞霸 RewardNum 篮 5俺鳖瘤老 扒单 角力肺绰 6己鳖瘤 犬牢秦具 秦辑.. RewardNum 俊 蝶扼 粱 炼例窃
//				int32 ThisItemGrade = (RI % FItemGradeInfo::MAX_NORMAL_ITEM_STAR_GRADE); // 捞 蔼阑 弊措肺 静搁 0 捞 唱棵 荐 乐绢辑 救登绊 弥家茄 1阑 歹秦具 茄促.
//				if (RewardNum == 3 || RewardNum == 5)
//				{
//					ThisItemGrade += 2; // RewardNum 5 牢 版快绰 2, 3, 4, 5, 6 捞 凳.
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
//				// 粱 抗寇肺 榜靛档 焊咯林档废 秦 焊磊..
//				if (RewardNum == 5 && ThisItemGrade == 2)
//				{
//					GenerateGoldItem(ThisItem, 10000);
//					AllRewardItem.Add(ThisItem);
//				}
//				// 抗寇肺 犁丰 酒捞袍档 焊咯林档废 秦杭鳖?
//				else if (RewardNum == 5 && ThisItemGrade == 3)
//				{
//					int32 RandNum = FMath::RandRange(0, 17);
//					ThisItem.ItemRefID = 8000000 + RandNum;
//					ThisItem.InventoryType = EItemInvenType::EIIVT_Consumables;
//					ThisItem.ItemClass = EItemClass::EIC_End;
//					ThisItem.InstanceUID = 8252424525250235 + RI; // 捞扒 利寸洒
//					AllMaterialRewardItem.Add(ThisItem);
//				}
//				else
//				{
//					ThisItem.ItemRefID = 1000000 + ThisItemGrade * 100;
//					ThisItem.InstanceUID = 5252424525250235 + RI; // 捞扒 利寸洒
//
//					DevOnlyLoadItemClientTestData(ThisItem);
//					AllRewardItem.Add(ThisItem);
//				}
//
//				if (bIsDoubled && ThisItem.ItemRefID != FItemRefIDHelper::GetGoodsID_Gold())
//				{
//					ThisItem.InstanceUID += 972442;
//					AllRewardItem.Add(ThisItem); // 焊呈胶. 度鞍篮 RefID 肺 窍唱 歹
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
//		FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("FirstTimeNPCSensitiveInfoConstruction 扁瓷篮 霉 荤侩 饶 泅犁 厚劝己拳凳. 荤侩捞 鞘夸茄 版快 橇肺弊贰怪评俊 巩狼.")));
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
//			FString MsgStr = TEXT("困连 捞抚甸\n\n");
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
//		// 某腐磐 PC 侩 MIC 甸狼 颇扼固磐甫 老褒 技泼窍绰 俊叼磐 目盖靛.
//		// 荤侩 器杆篮 抗甫 甸绢 
//		// "ChangePCCharMIParam 1 RoughnessSkinScale 0.4" 鞠混磊 侩 MIC 甸狼 RoughnessSkinScale 颇扼固磐甫 0.4 肺 技泼
//		// "ChangePCCharMIParam Gl MetalicScale 0.3" 八捧荤 侩 MIC 甸狼 MetalicScale 颇扼固磐甫 0.3 栏肺 技泼.
//		// PC 瘤沥篮 0 ~ 3 趣篮 距加等 距绢甫 荤侩且 荐 乐促.
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
//		{ // 攫哩啊 捞 内靛甫 搂 荤恩捞 绝阑 锭 寸炔窍瘤 臼档废.
//#if !PLATFORM_MAC
//			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(TEXT("ChangePCCharMIParam 目盖靛绰 Fighter 鳖瘤父 霖厚登菌嚼聪促.\n货肺款 某腐磐俊 措茄 内靛绰 眠啊秦具 钦聪促.")));
//#endif
//		}
//
//		// 快府 某腐磐 评辑 荤侩窍绰 匙捞怪俊 蝶扼 颇老阑 鞘磐傅 窍绰 芭.
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
//			TArray<FString> FoundAndProcessedFiles; // 搬苞 皋矫瘤 侩
//
//			for (UMaterialInstanceConstant* ThisMIC : AllFoundMICs)
//			{
//				if (ThisMIC)
//				{
//					// 颇扼固磐啊 乐绰瘤 咯何 眉农窍绰 扒 搬苞 皋矫瘤俊 馆康窍扁 困窃.
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
//				MessageStr = FString::Printf(TEXT("促澜 %d 俺 MIC 甸狼 颇扼固磐 %s 捞(啊) %.3f (栏)肺 函版登菌嚼聪促. 犬牢 棺 历厘秦 林技夸.\n\n"),
//					FoundAndProcessedFiles.Num(), *WantedParamNameStr, WantedParamValue);
//				for (int32 FI = 0; FI < FoundAndProcessedFiles.Num(); ++FI)
//				{
//					MessageStr += FString::Printf(TEXT("%d. %s\n\n"), FI + 1, *FoundAndProcessedFiles[FI]);
//				}
//			}
//			else
//			{
//				MessageStr = FString::Printf(TEXT("%s 匙捞怪狼 Character MIC 甸狼 颇扼固磐 %s 阑(甫) %.3f (栏)肺 函版窍绊磊 窍看栏唱 窍唱档 己傍窍瘤 给沁嚼聪促. 肺拿 况农胶其捞胶 惑狼 颇老/版肺啊 函版登菌芭唱 粮犁窍瘤 臼绰 颇扼固磐甫 瘤沥窍看阑 啊瓷己捞 乐嚼聪促."),
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
//		// UB2MatineeRefCheckCommandlet 阑 父甸促啊.. 捞繁 扁瓷捞 目盖甸房 券版俊辑 阂啊瓷茄 巴档 鞍绊
//		// 绢瞒乔 俊叼磐俊辑 荤侩窍绰 霸 祈且 荐档 乐绢辑.. 捞贰历贰 俊叼磐 目盖靛肺 力傍.
//		UB2MatineeRefCheckCommandlet::HandleAsEditorCommand(Cmd, Ar);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("B2MaterialParamCheck")))
//	{ // 悼老茄 捞抚狼 目盖甸房栏肺 荤侩且 荐 乐澜.
//		UB2MaterialParamCheckCommandlet::EditorCommandMain(Cmd, Ar);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("B2AssetOptCheck")))
//	{ // 悼老茄 捞抚狼 目盖甸房栏肺 荤侩且 荐 乐澜.
//		UB2AssetOptCheckCommandlet::EditorCommandMain(Cmd, Ar);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SummarizeMaterialStaticSwitchUse")))
//	{ // 官肺 酒贰 ReplaceParentMaterialStaticSwitch 客 楷拌秦辑 荤侩窍妨绰 巴烙.
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
//	{ // 官肺 困 SummarizeMaterialStaticSwitchUse 客 楷拌秦辑 荤侩窍妨绰 巴烙.
//		// 漂沥 Material 阑 Parent 肺 窍搁辑 漂沥 StaticSwitch 悸阑 汲沥茄 MIC 甸狼 Parent 甫 官层摹绰 扁瓷. 磊技茄 悼累篮 酒贰 窃荐俊辑..
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
//		// 捞吧肺 StaticSwitch 备己阑 绢痘霸 瘤沥窍绰瘤绰 酒贰 夯 窃荐俊辑 颇教.
//		FParse::Value(Cmd, TEXT("SSParam="), SSParamSetString, false); // ShouldStopOnComma 甫 false 肺. SSParam 备巩俊 Comma 啊 甸绢啊扁 锭巩.
//
//		ReplaceParentMaterialOfStaticSwitchParam(CurrParentMaterial, NewMaterial, ChildMICSearchPath, SSParamSetString);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("ReplaceParentMaterialOfMIC"))) // 窜鉴茄 何葛 Material 官层摹扁. 入荐历狼 陛荐历 犁藕积
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
//			// 茫篮 颇老 吝 MIC 父 鞘夸窍瘤父 角力肺绰 肺爹阑 秦 毫具 救促..
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
//		// 楷免侩 SkeletalMeshActor 悸诀窍绰单 扁夯 歹固 酒囱 6己 侩栏肺 秦 初篮 霸 呈公 腹酒 饭骇 窜困肺 老褒贸府扼档 窍霸..
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
//			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("官诧 巴捞 绝嚼聪促.")));
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
bool gbHACKInGameUIHiddenByDevCommand = false; // 档吝俊 积己登绰 局甸档 利侩捞 登绢具 秦辑.. ばば
bool GCheat_CompleteIgnoreEventScene = false; // 捞巴阑 荤侩窍绰 版快绰 酒抗 楷免 矫累炼瞒 登瘤 臼栏哥 割割 甘篮 力措肺 敲饭捞啊 阂啊瓷且 荐档 乐澜. 漂荐茄 惑炔狼 抛胶飘侩.
int32 GCheat_SkipEventSceneCount = -1; // 捞扒 霸烙 柳青捞 啊瓷茄 荐霖俊辑 楷免 厘搁阑 胶诺. 0 焊促 农搁 弊 箭磊父怒狼 楷免厘搁阑 胶诺茄促.
bool GCheat_HeroineMode = false; // Hero 葛靛客 厚搅茄单 泅犁 各甸 吝 窍唱父 磷捞搁 唱赣瘤啊 促 磷澜.
float GCheat_StageDifficultyOverride = 0.0f; // 0.0 俊辑绰 扁夯蔼
bool GCheat_PCPartMergeRemoveExtraBoneInfluence = false;
int32 GCheat_RepeatBattleCountOverride = 0;
bool GTest_GenerateDummyChatMessage = false;
bool GTest_UseNonCompositedRawPCMesh = false; // PC 厘厚 矫胶袍俊 狼秦 炼钦登瘤 臼篮 扁夯 皋浆 荤侩.
bool gIsDevUI = false;
bool gUseHeroTowerResetNoLoading = true; // 康旷狼 啪 肺爹 绝捞 犁矫累窍绰 扁瓷捞 俺惯 档吝 滚弊甫 老栏懦 家瘤啊 腹栏骨肺 巩力 惯积矫 抛胶飘甫 困秦 力傍.
bool gDrawPerfMark = false; // ScopedCallTracker 俊辑 漂沥 矫埃 捞惑 家夸等 芭 拳搁俊 钎矫
bool gPreloadCombatEffectResources = true; // 己瓷 抛胶飘 侩. 割割 傈捧俊辑 spawn 窍绰 TAsset 栏肺 等 捞棋飘 包访 府家胶甸阑 固府 肺爹 棺 某教秦 初篮 巴阑 烹秦 hitch 啊 倔付唱 俺急登绰瘤 焊扁 困秦.
bool gDisableInGameFloatingWidgets = false; // 傈捧俊辑 拜纺窍霸 殿厘窍绰 DamageNum, AbnormalState, GoldAcq 殿 掺扁. 己瓷 抛胶飘侩.
bool gDisableCombatEventUI = false; // 傈捧殿 捞亥飘 拜栏肺 殿厘窍绰 Floating 寇狼 巴甸
int32 gPCDataPreLoadMaxNum = -1; // 肺爹矫埃 眉农侩. 荤角惑 某腐磐 4辆幅俊辑绰 4捞惑捞扁父 窍搁 沥惑累悼. 0, 1 鞍篮 箭磊肺 抛胶飘窍扁 困窃.
bool gNPCDataNoPreLoadMode = false;
bool gUseCharacterDefaultMaterial = false; // 己瓷 螟沥 侩
int32 gTestAttachLightCompMode = 0; // 敲饭捞绢 某腐磐 扼捞飘 attach mode
bool gbTestTurnOffPlayerLight = false;

 //ItemInfo 厘厚 厚悼扁 肺爹 肯丰 饶 敲饭捞绢 某腐 dummy spawn 鳖瘤 抛胶飘 窍扁 困窃.
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

	 //抛胶飘 目盖靛俊辑 荤侩窍妨绰 巴烙. 扁瓷篮 曼绊父.

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
			// 夸没茄 厘厚甸 ID 甫 啊瘤绊 DummyPlayer spawn 阑 流立 窍妨绊 秦档 ABladeIITestDummyPlayer 率 spawn 扁瓷俊 绢瞒乔 StandardEquipment 啊瘤绊 窍绰 巴父 备厚登绢 乐澜.

			if (GEngine)
			{ // 肺爹 肯丰等 厘厚 啊瘤绊 spawn 抛胶飘. 拱沸 捞贰档 PCClassInfo 肺爹俊 坷贰 吧副 荐 乐促.
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
static FItemInfoAsyncLoadTest ItemInfoAsyncLoadTestObj; // 抛胶飘俊 快妨冈阑 按眉 窍唱.

bool UB2StaticExecHelper::ExecDev(UWorld* Inworld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	 //葛官老 抛胶飘 滚傈俊辑绰 荤侩且 目盖靛甫 咯扁俊. 弥辆魄俊辑绰 付蛮啊瘤肺 狐瘤摆瘤父.
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
//		UB2UIManager::GetInstance()->HideBattleRelatedWidgets(gbHACKInGameUIHiddenByDevCommand); // 脚 UI
//
//		return true;
//	}
//	if (FParse::Command(&Cmd, TEXT("ShowUI"))) // "InGameUI" 绰 烤朝俊 父电 芭绊.. 捞力绰 傲 捞犯霸 窍搁 肋 冈洒绰 淀..
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
//		if (ItemIDString == TEXT("Gold")) // 漂荐 纳捞胶.
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
//	{ // 肺厚俊辑 牢亥配府啊 檬扁拳等 惑怕俊辑 荤侩秦具 冈塞.
//		FString RefIDString;
//		FParse::Token(Cmd, RefIDString, true);
//		int32 ParsedRefID = FCString::Atoi(*RefIDString);
//
//		FString CreateNumString;
//		FParse::Token(Cmd, CreateNumString, true);
//		int32 CreateNum = FCString::Atoi(*CreateNumString);
//		if (CreateNum <= 0)
//		{
//			CreateNum = 30; // 扁夯蔼
//		}
//		if (ParsedRefID < 2000000)
//		{ // 老馆利牢 厘馒 酒捞袍捞扼搁 角荐肺 呈公 腹捞 夸没窍瘤 臼霸..
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
//			{// 捞扒 梅俊 父甸菌带 扁瓷牢单 搬惫 付蛮啊瘤肺 DevOnlyRequestDummyItems 甫 荤侩且 巴. RefID 绰 救俊 窍靛内爹.
//				LobbyInven->DevOnlyGenerateDummyItemData(true);
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GenerateClientDummyItems")))
//	{ // 肺厚俊辑 牢亥配府啊 檬扁拳等 惑怕俊辑 荤侩秦具 冈塞.
//		AB2LobbyGameMode* LobbyGM = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(Inworld));
//		UB2LobbyInventory* LobbyInven = LobbyGM ? LobbyGM->GetLobbyInventory() : NULL;
//		if (LobbyInven)
//		{ // 捞 版快, 辑滚肺 夸没窍绰 措脚 努扼捞攫飘俊父 盲款促.
//			LobbyInven->DevOnlyGenerateDummyItemData(false);
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GenerateClientDummyConsumable")))
//	{
//		FString ConsumableIDStr;
//		FParse::Token(Cmd, ConsumableIDStr, true);
//		int32 ConsumableID = FCString::Atoi(*ConsumableIDStr); // ID 绰 沥秦柳 芭 舅酒辑 持绢具 且 淀.
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
//		// PCClass 瘤沥窍霸 窍妨绊 沁绰单 宝洒 球哎府扁父 且 淀. 傲 葛滴 悼矫 积己窍磊.
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
//		LobbyUpdateWingMainUIClass<>::GetInstance().Signal(); // Wing UI 其捞瘤 诀单捞飘.
//
//		// 肺厚 皋浆 诀单捞飘档 鞍捞
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
//	{ // 泅犁 肺厚 急琶 某腐磐狼 厘馒栏肺 牢秦 刘啊登绰 瓷仿摹 犬牢
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
//		UB2LobbyInventory::GetAllEquippedOptionPointsLocalPlayer(AllOptions, LobbyPCClass); // 弥辆利栏肺 利侩瞪 蔼捞 唱客具 窃.
//		for (auto& ThisOption : AllOptions)
//		{ // Item Option 篮 int 肺绰 鉴瞒利捞瘤绰 臼阑 巴.
//			UE_LOG(LogBladeII, Log, TEXT("OptionType %d, amount %.4f"), (int32)ThisOption.Key, ThisOption.Value);
//		}
//		return true;
//	}*/
//	else if (FParse::Command(&Cmd, TEXT("SPJump")))
//	{
//		// 泅犁 甘俊辑 肺爹等 SpawnPool 甸狼 漂沥 Wave 鳖瘤 碍力 矫累 棺 辆丰.
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
//					CurrSED->CustomFinishShow(); // 困 苞沥阑 芭摹搁辑 唱坷绰 楷免甸篮 场郴绰 霸 亮阑 淀.
//				}
//			}
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SpawnGuildNPC")))
//	{
//		// 颇扼固磐 眠免
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
//		if (B2Player) // 敲饭捞绢俊辑 距埃 厚哺抄 困摹俊
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
//		// NPCClassInfo 俊 殿废等 NPC spawn. MonsterSpawnPool 汲摹 绝捞 溜籍俊辑 目盖靛肺 spawn 矫虐绊 酵阑 锭
//		// "SpawnNPC ClassName VariationName" ClassName 苞 VariationName 篮 阿阿 ENPCClass 尔 ENPCClassVariation 亲格俊辑 prefix 猾 芭
//
//		// 颇扼固磐 眠免
//		FString ClassStr;
//		FParse::Token(Cmd, ClassStr, true);
//		ENPCClass WantedClass = UB2NPCSingleClassInfo::FindNPCClassEnumFromTrimmedName(ClassStr);
//
//		FString ClassVariStr;
//		FParse::Token(Cmd, ClassVariStr, true);
//		ENPCClassVariation WantedClassVariation = UB2NPCSingleClassInfo::FindNPCClassVariEnumFromTrimmedName(ClassVariStr);
//		
//		bool bSpawnAsBoss = FParse::Param(Cmd, TEXT("Boss")); // SpawnPool 狼 boss 拜栏肺.. 捞吧 瘤沥窍妨搁 Variation 档 瘤沥秦具..
//
//		if (WantedClass != ENPCClass::ENC_End)
//		{
//			FTransform SpawnTransform;
//			ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//			if (B2Player) // 敲饭捞绢俊辑 距埃 厚哺抄 困摹俊
//			{
//				SpawnTransform = B2Player->GetTransform();
//				SpawnTransform.AddToTranslation(FVector(100.0f, 0.0f, 100.0f));
//			}
//
//			FActorSpawnParameters SpawnInfo;
//			SpawnInfo.Owner = B2Player;
//			SpawnInfo.bDeferConstruction = true; // Need deferred construction and manually call FinishSpawning. AutoPossessAI 汲沥 棺 NPCClassInfo 利侩阑 困秦 DeferConstruction 捞 鞘夸.
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
//					FMobSpawnedBirthplaceInfo DummyBirthplaceInfo; // 角力肺 Birthplace 绰 绝瘤父..
//					DummyBirthplaceInfo.bSpawnedAsBossMob = bSpawnAsBoss; // 夸芭 傈崔窍妨绊.
//					SpawnedNPC->SetBirthPlace(DummyBirthplaceInfo);
//
//					SpawnedNPC->FinishSpawning(SpawnTransform);
//					SnapBladeIICharacterToGround(SpawnedNPC);
//
//					// Boss 各捞搁 UI 抛胶飘啊 窍唱狼 格利捞聪 技泼且 荐 乐绰 巴甸 弥措茄..
//					UB2UIDocBattle* DocBattle = UB2UIDocHelper::GetDocBattle();
//					if (bSpawnAsBoss && DocBattle)
//					{
//						DocBattle->SetbCurrentlyBossAppeared(true); // 磷阑 锭 促矫 府悸秦具 窃.
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
//		// NPCClassInfo 俊 殿废等 NPC 甫 dummy 肺 spawn. 
//		// SpawnNPC 绰 角力肺 傈捧甫 国捞绰 NPC 甫 spawn 矫虐瘤父 捞扒 SpawnDummyPlayer 贸烦 局聪皋捞记父 茄促.
//		// "SpawnDummyNPC ClassName VariationName" ClassName 苞 VariationName 篮 阿阿 ENPCClass 尔 ENPCClassVariation 亲格俊辑 prefix 猾 芭
//
//		// 颇扼固磐 眠免
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
//			if (B2Player) // 敲饭捞绢俊辑 距埃 厚哺抄 困摹俊
//			{
//				SpawnTransform = B2Player->GetTransform();
//				SpawnTransform.AddToTranslation(FVector(200.0f, 0.0f, 200.0f));
//			}
//
//			// iOS 哪颇老 俊矾
//			/*
//			ABladeIITestDummyNPC* NewSpawned = ABladeIITestDummyNPC::SpawnUtil(Inworld, WantedClass, WantedClassVariation, SpawnTransform,
//				ParsedSpawnFloatingInterval, ParsedSpawnDamageFxInterval);
//
//			if (NewSpawned)
//			{
//				if (ParsedAnimStateString == TEXT("Rand") || ParsedAnimStateString == TEXT("Random"))
//				{ // 罚待窍霸 敲饭捞
//					NewSpawned->SetUseRandomAttackState(true);
//				}
//				else
//				{ // 沥秦初绊 窍唱 敲饭捞
//					NewSpawned->SetUseRandomAttackState(false);
//					NewSpawned->SetDirectAttackStateByKeyword(ParsedAnimStateString);
//				}
//
//				NewSpawned->SetCooltimeBetweenAnim(ParsedAnimCooltime);
//
//				// 扁夯利栏肺绰 救 框流捞绊 漂喊洒 牢磊甫 林搁 捞悼捞唱 雀傈 啊瓷窍霸
//				if (FParse::Param(Cmd, TEXT("Move"))) {
//					// 雀傈绝捞 捞悼父 窍绰 扒 快胶参胶矾快聪 笛 促 せ
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
//	else if (FParse::Command(&Cmd, TEXT("B2SED"))) // 楷免 厘搁 碍力 绝局扁. 矫累 饶 胶诺捞 酒聪扼 矫累炼瞒 窍瘤 臼绰 巴栏肺 农贰浆 眠利 殿阑 困茄 抛胶飘侩.
//	{
//		FString OnOffStr;
//		FParse::Token(Cmd, OnOffStr, true);
//				
//		// GCheat_SkipEventSceneCount 客绰 馆措.. ぱ.ぱ
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
//		else{ // GCheat_SkipEventSceneCount 啊 乐阑 荐 乐栏聪
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
//			GCheat_SkipEventSceneCount = SecifiedCount; // 沥秦柳 俺荐父怒狼 楷免厘搁父 烹苞且 巴.
//		}
//		else if (OnOffOrCountStr == TEXT("On") || OnOffOrCountStr == TEXT("True"))
//		{
//			GCheat_SkipEventSceneCount = 100000000; // 窜鉴茄 turn-on 捞搁 傲 粮唱 奴 箭磊甫 拎辑 荤角惑 拌加 胶诺 登档废. 绢瞒乔 俺惯侩 摹飘捞聪.
//		}
//		else if (OnOffOrCountStr == TEXT("Off") || OnOffOrCountStr == TEXT("False") || OnOffOrCountStr == TEXT("0"))
//		{
//			GCheat_SkipEventSceneCount = -1;
//		}
//
//		if (GCheat_SkipEventSceneCount > 0){
//			
//			// 瘤陛 敲饭捞 登绰 霸 乐促搁 场郴磊.
//			for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//			{
//				AB2StageEventDirector* CurrSED = Cast<AB2StageEventDirector>(*ActorIt);
//				if (CurrSED && CurrSED->GetDirectingEvent() != EStageEvent::ESEV_Clear && CurrSED->IsPlayingShow())
//				{
//					//--GCheat_SkipEventSceneCount; 捞扒 器窃矫虐瘤 富磊..
//					CurrSED->CustomFinishShow();
//				}
//			}
//
//			UE_LOG(LogBladeII, Log, TEXT("Stage event scene will be skipped for %d times, but some essential scene will still be played."), GCheat_SkipEventSceneCount);
//		}
//		else{ // GCheat_CompleteIgnoreEventScene 捞 乐阑 荐 乐栏聪
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
//		// 0 焊促 农搁 StageManager 率俊辑 坷滚扼捞靛 秦辑 荤侩且 巴烙.
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
//					CastedCharacter->InitializeCombatStats(); // 捞固 spawn 等 局甸篮 饭骇俊 蝶扼 傈捧仿 犁拌魂.
//					CastedCharacter->Health = HealthRatio * CastedCharacter->MaxHealth; // 犁拌魂 饶俊 HP 父顶 瞒骨肺 厚啦父 嘎苗淋 せ
//				}
//			}
//		}
//		
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PCMerge")))
//	{
//		// Mesh merge 抛胶飘. UEngine::HandleMergeMeshCommand 曼绊. 牢磊甫 烹秦 府家胶甫 逞败林瘤 臼栏搁 窜鉴洒 皋牢 某腐苞 怕弊 某腐 皋浆甫 merge 偿偿 
//		// 捞扒 辨霸 牢磊 持绢林扁 绢妨款 葛官老 券版 累悼 抛胶飘甫 堪滴俊 敌 芭
//
//		bool bGenerateNewSkeleton = FParse::Param(Cmd, TEXT("NewSkeleton")); // Skeleton 阑 货肺 父甸芭唱 霉锅掳 家胶 皋浆狼 skeleton 阑 炼钦等 皋浆狼 skeleton 栏肺 荤侩.
//
//		// array of source meshes that will be merged
//		TArray<USkeletalMesh*> SourceMeshList;
//		TArray<FSkelMeshMergeSectionMapping> ForceSectionMapping;
//		TArray<FSkelMeshMergeMtrlOverrideMapping> SrcMtrlOverrides;
//
//		ABladeIIPlayer* B2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(Inworld));
//		if (!B2Player){ // 捞扒 咯窍埃 鞘夸. 搬苞拱阑 咯扁俊 持阑 芭扼
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
//		{ // 目盖靛 牢磊肺 skeletal mesh 甫 逞败霖 版快.
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
//					FSkelMeshMergeMtrlOverrideMapping NewMtrlMap; // 拱沸 唱吝俊 section ID 立辟且 鞘夸 绝捞 泅 矫痢俊辑 官肺 material override 甫..
//					for (int32 SI = 0; SI < ThisMeshResource->LODModels[0].Sections.Num(); ++SI) // ForceSectionMapping 篮 LODModel 0 锅 扁霖栏肺 窍搁 凳.
//					{
//						NewSectionMap.SectionIDs.Add(ForceSectionID);
//
//						// 冀记阑 钦摹霸 登搁 颇飘喊 冀记阑 钦媚辑 荤侩窍扁 困秦 霖厚等 喊档狼 material 阑 荤侩茄促. 鞍篮 冀记捞搁 悼老茄 material 阑 荤侩秦具 窃.
//						// MergeSections 俊辑档 咯傈洒 null 老 荐 乐促. 弊繁 版快绰 盔贰 material 阑 静绰 扒单 咯傈洒 疙矫利栏肺 窜老 冀记阑 备己窍绰 颇飘啊 乐栏搁 弊犯霸 瞪 淀
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
//				//Battle UI诀单捞飘
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
//				//SkillRTManager data诀单捞飘
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
//		{ // 肺厚扼搁 诀单捞飘甫..
//			LobbyUpdateCharacterPartClass<EPCClass>::GetInstance().Signal(IntToPCClass(PCI));
//		}
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("SpawnBuffText")))
//	{
//		FString BuffCatStr;
//		FParse::Token(Cmd, BuffCatStr, true);
//		const int32 BuffCat = FCString::Atoi(*BuffCatStr); // 箭磊肺 瘤沥秦辑 0 : 老馆Buff, 1 : TeamBuff, 2 : StateAttack, 3 : ResurrectBuff, 4 : Influence
//		FString BuffTypeStr;
//		FParse::Token(Cmd, BuffTypeStr, true);
//		const int32 BuffTypeInt = FCString::Atoi(*BuffTypeStr); // BuffCat 俊 蝶扼 促抚, 2柳荐牢 TeamBuff 档 弊成 促弗 芭尔 付蛮啊瘤肺 秒鞭窍磊..
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
//					// 捞扒 泅犁 FloatingDamageNum 率俊..
//					CharacterTakeDamageClass<ABladeIICharacter*, float, bool>::GetInstance().Signal(LocalB2Player, FMath::RandRange(1.0f, 2000.0f), true);
//				}break;
//				case 7: {
//					// 捞扒 荤角 Text 啊 酒囱 Effect. EndEffect 老瘤 咯何绰 罚待.
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
//		BeginWingEvolutionSceneClass<EPCClass>::GetInstance().Signal(EPCClass::EPC_End); // 夸 捞亥飘甫 罐绰 康旷包府 葛靛咯具.
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("STLog")))
//	{ // B2_SCOPED_TRACK_LOG 俊 狼茄 肺弊啊 呈公 腹酒辑 促弗 叼滚彪俊 规秦啊 瞪 锭俊 馋促.
//		// 弊成 捞率 肺弊 墨抛绊府 蝶肺 父甸菌栏聪 弊吧 啊瘤绊 Log LogB2ScopedCallTrack Off 疙飞阑 林绢档 凳.
//		FB2ScopedCallTracker::bIsOn = !FB2ScopedCallTracker::bIsOn;
//		UE_LOG(LogBladeII, Log, TEXT("B2ScopedCallTrack Log being turned %s"), FB2ScopedCallTracker::bIsOn ? TEXT("ON") : TEXT("OFF"));
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("STimeLog")))
//	{ // 咯扁档 刀磊利牢 肺弊 墨抛绊府啊 乐栏聪 弊吧 结档.. LogB2ScopedTimeLog
//		FB2ScopedTimeLogger::bIsOn = !FB2ScopedTimeLogger::bIsOn;
//		UE_LOG(LogBladeII, Log, TEXT("FB2ScopedTimeLogger being turned %s"), FB2ScopedTimeLogger::bIsOn ? TEXT("ON") : TEXT("OFF"));
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("MissySexy")))
//	{
//		// 冀矫硼企咯某疯俊 何淬绝绰 傈捧甫 困茄 厘厚 备己. 捞巴甸父 涝洒绊 厘癌 何明 促 哈辨巴. 
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1100600 4")); // ServerCmd 甸烙
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1102600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1200600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1202600 4"));
//		
//		// 酒贰 厩技荤府绰 傲 眉仿 疵府扁 侩.
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1105600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1106600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1107600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1205600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1206600 4"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1207600 4"));
//
//		// 八捧荤狼 快胶参胶矾款 疯阑 困秦 秋镐父 6己嘎勉.
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("AddItem 1001600 4"));
//
//		// 弊府绊 眠啊肺 林赣聪档 乘乘窍霸 盲况林磊. 捞巴甸篮 GetAccount 秦 林绢具 馆康凳.
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncGold 10000000"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncGem 1000000"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncDarkCrystal 1000000"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncRankPromotionPoint 100000"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncSocialPoint 1000"));
//		data_trader::ExecHandler::GetInstance().ExecuteShellCommand(TEXT("IncHeroPiece 1000"));
//
//		// 朝俺 碍拳 殿狼 犁丰.
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
//		// Options= 捞饶肺绰 ID-Value,ID-Value 捞繁 侥栏肺 眠啊窍扁 盔窍绰 技何 可记甸阑 瘤沥. ID 绰 辑滚 B2ItemOption 箭磊 扁霖. 弊 箭磊啊 扁裙 竣伎 颇老俊辑 焊绰 巴苞 老摹窍骨肺
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
//			// ID-Value 阿阿阑 窍捞锹 扁霖栏肺 都绢晨.
//			if (ThisParsedStr.Split(TEXT("-"), &OptionIDStr, &OptionValueStr))
//			{
//				int32 OptionIDInt = FCString::Atoi(*OptionIDStr);
//				EItemOption CliItemOption = SvrToCliOptionType(OptionIDInt); // ID 箭磊绰 辑滚 鸥涝俊辑 荤侩窍绰 巴栏肺 Client enum type 栏肺 函版.
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
//		// 老窜篮 扁粮 吧 朝府绰 可记栏肺.. 咯扁 概俺函荐肺 临 荐档 乐绊..
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
//		UB2LobbyUI_CharStatDetail::SetDebugDrawPlayerStatMode(DisplaySetNum); // DisplaySetNum 捞 0 焊促 农搁 焊捞扁 矫累且 巴.
//
//		// 胶抛捞瘤 葛靛扼搁 绢瞒乔 泅犁 敲饭捞绢 某腐磐啊 荤侩瞪 巴. 捞扒 匙飘况农 葛靛 侩栏肺 狼档茄 巴.
//		int32 PCClassInt = 0;
//		FParse::Value(Cmd, TEXT("NetPC="), PCClassInt);
//		EPCClass OverridePCClass = IntToPCClass(PCClassInt);
//		UB2LobbyUI_CharStatDetail::SetDebugDrawPlayerStatClass(OverridePCClass);
//
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("NumToPlayRaid"))) // 齿霸烙葛靛 敲饭捞 曼啊磊 临捞扁, 饭捞靛绰 去磊窍扁啊 登骨肺 救靖
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
//	{// 抛胶飘甫 困秦 摹飘虐肺 力傍. 漂沥 惑炔俊辑 炼缴秦辑 荤侩秦具 且 扁瓷.
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
//		// 鞘荐鳖柳 酒聪绊 促弗 评俊辑 抛胶飘 且 锭 档吝俊 ShowDevUI 摹绊 官肺 诀单捞飘 救登搁 庆羔鳖毫 祈狼 瞒盔俊辑 粱 诀单捞飘 秦 淋.
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
//	{ // 抛胶飘 目盖靛 瞒盔俊辑 AB2StageGameModeBase 甫 啊瘤绊 窍瘤父 角力 荤侩篮 捞霸 倾侩等 漂沥 霸烙葛靛俊辑父 秦具 茄促.
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
//		// 鼻酋吝 - 抛胶飘侩栏肺 静绰 目盖靛涝聪促.
//	}
//	else if (FParse::Command(&Cmd, TEXT("HeroTowerLoading")))
//	{
//		gUseHeroTowerResetNoLoading = !gUseHeroTowerResetNoLoading;
//		UE_LOG(LogBladeII, Display, TEXT("HeroTowerGameMode restart without world loading %d"), (int32)gUseHeroTowerResetNoLoading);
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("CharFxLOD")) || FParse::Command(&Cmd, TEXT("CFxLOD"))) // "CFxLOD PC/Mob/Boss #" 某腐磐 辆幅喊肺 ParticleSystemComponent LOD 利侩
//	{
//		FString TypeStr;
//		FParse::Token(Cmd, TypeStr, true);
//
//		FString ParsedLODLevelStr;
//		FParse::Token(Cmd, ParsedLODLevelStr, true); // 鞠巴档 瘤沥 救登菌促搁 0捞摆瘤. 弊烦 0栏肺 技泼窍搁 凳.
//		const int32 ParsedLODLevel = FCString::Atoi(*ParsedLODLevelStr);
//
//		if (TypeStr == TEXT("None")){
//			// 葛滴 府悸
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
//			GB2CharFxCheatLODLevel_PC = -1; // 购啊 促 酒囱 版快档 府悸
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
//					ThisComp->SetLODLevel(0); // 咯扁 持绢林绰 蔼篮 狼固绝绊 傲 诀单捞飘 瞒盔..
//				}
//				else {
//					if (ThisComp->GetWorld()) {
//						ThisComp->RestoreLODLevelToDesiredValue(); // 捞矾搁 盔贰 抗惑登绰 蔼栏肺 倒酒啊妨唱
//					}
//				}
//			}
//		}
//
//#if !UE_BUILD_SHIPPING
//		// 趣 目盖靛甫 静歹扼档 滚瓢 函版登档废..
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
//	else if (FParse::Command(&Cmd, TEXT("RTMP"))) // 抛胶飘俊 静妨绊 陋篮 滚傈..
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
//		{ // 捞 版快 窜鉴洒 惑怕 On/Off 
//			gDrawPerfMark = !gDrawPerfMark;
//		}
//		else
//		{
//			gDrawPerfMark = true; // PerfMark 矫埃 扁霖 瘤沥 矫 公炼扒 囊促.
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
//		// GameplayOnlyLODQuality 函版捞 绝绢档 bUseBladeIIScalabilityOverride 俊 蝶扼 牢霸烙 楷免 LOD 利侩捞 崔扼龙 荐 乐栏聪 BladeIIPlayer iteration 档..
//		for (FActorIterator ItPlayer(Inworld); ItPlayer; ++ItPlayer)
//		{
//			ABladeIIPlayer* CastedPlayer = Cast<ABladeIIPlayer>(*ItPlayer);
//			if (CastedPlayer)
//			{ // 趣咯唱 康惑 敲饭捞 档吝捞扼 窍歹扼档.. 楷免 档吝俊绰 措何盒 BladeIIPlayer 啊 酒囱 SkeletalMeshActor 啊 荤侩登骨肺 捞巴阑 龋免窍绰 霸 康氢阑 固摹瘤 臼绰促. 酒囱 版快扼 窍歹扼档 楷免 档吝俊 pause 皋春啊 唱坷瘤档 臼绊..
//				CastedPlayer->SetupLODInfoForInGame();
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("GF"))) // 快府 霸烙 券版俊辑 牧刨明客 搬何等 feature 甸 On/Off 窍搁辑 己瓷 螟沥阑 困秦. 葛官老 叼官捞胶俊辑 埃帆窍霸 目盖靛 持阑 荐 乐档废 陋霸 窃
//	{
//		FString FirstArgStr;
//		FParse::Token(Cmd, FirstArgStr, true);
//
//		if (FirstArgStr == TEXT("FxParticle"))
//		{
//			bool bWasDisabled = (UAnimNotifyState_TimedParticleEffect::bDisableTimedParticleEffect || UAnimNotify_PlayParticleEffect::bDisablePlayParticleEffect || UB2DamageEffectInfo::bDisableDamageEffectParticle);
//			// 笛捞 度鞍捞 牧飘费 登档废
//			UAnimNotifyState_TimedParticleEffect::bDisableTimedParticleEffect = !bWasDisabled;
//			UAnimNotify_PlayParticleEffect::bDisablePlayParticleEffect = !bWasDisabled;
//			UB2DamageEffectInfo::bDisableDamageEffectParticle = !bWasDisabled; // 捞巴档 器窃牢单.. 蝶肺 都绰霸 亮唱..
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("Particle from AnimNotify/EffectInfo %s"), bWasDisabled ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		if (FirstArgStr == TEXT("FxNotify")) // FxParticle 狼 老何
//		{
//			bool bWasDisabled = (UAnimNotifyState_TimedParticleEffect::bDisableTimedParticleEffect || UAnimNotify_PlayParticleEffect::bDisablePlayParticleEffect);
//			// 笛捞 度鞍捞 牧飘费 登档废
//			UAnimNotifyState_TimedParticleEffect::bDisableTimedParticleEffect = !bWasDisabled;
//			UAnimNotify_PlayParticleEffect::bDisablePlayParticleEffect = !bWasDisabled;
//
//			BII_SCREEN_LOG(FString::Printf(TEXT("Particle from AnimNotify/EffectInfo %s"), bWasDisabled ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		if (FirstArgStr == TEXT("DFX")) // FxParticle 狼 老何
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
//			// SetHidden 阑 秦具 利侩凳. 促矫 挠 锭 角力肺 见败廉 乐绢具 窍绰 惑怕扼搁 巩力啊 登摆瘤父.. 抛胶飘窍绰单 捞沥档码捞具
//			for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//			{
//				ABladeIIPlayer* CastedPC = Cast<ABladeIIPlayer>(*ActorIt);
//				if (CastedPC)
//				{ // bHideAllPCs true 秦 初绊 茄锅 见扁搁 促矫 SetActorHiddenInGame(false) 秦档 救 难咙.
//					CastedPC->SetActorHiddenInGame(ABladeIIPlayer::bHideAllPCs);
//				}
//			}
//			BII_SCREEN_LOG(FString::Printf(TEXT("Player Characters %s"), !ABladeIIPlayer::bHideAllPCs ? TEXT("ON") : TEXT("OFF")),
//				FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 16, 5.0f);
//		}
//		else if (FirstArgStr == TEXT("NPC"))
//		{
//			ABladeIICharacter::bHideAllNPCs = !ABladeIICharacter::bHideAllNPCs;
//			// SetHidden 阑 秦具 利侩凳. 促矫 挠 锭 角力肺 见败廉 乐绢具 窍绰 惑怕扼搁 巩力啊 登摆瘤父.. 抛胶飘窍绰单 捞沥档码捞具
//			for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//			{
//				ABladeIIPlayer* CastedPC = Cast<ABladeIIPlayer>(*ActorIt);
//				ABladeIICharacter* CastedNPC = Cast<ABladeIICharacter>(*ActorIt);
//				if (CastedNPC && !CastedPC)
//				{ // bHideAllPCs true 秦 初绊 茄锅 见扁搁 促矫 SetActorHiddenInGame(false) 秦档 救 难咙.
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
//	{ // 捻欧 滚傈栏肺 辑滚 救 嘿篮 惑怕俊辑 敲饭捞且 锭 加档啊 捞惑窍霸 弧扼廉辑 捞犯霸扼档 泵咯瘤氓.
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
//		// 咯扁辑 函版窍妨绰 函荐狼 ini 亲格 佬绢甸捞绰 巴阑 滴 InfoAsset 积己磊俊辑 窍骨肺 酒流 佬绢甸捞瘤 臼疽促搁 咯扁辑 秦 狄.
//		StaticFindPCClassInfoBox();
//		StaticFindSkillAnimInfo();
//
//		// 笛阑 悼矫俊 度鞍捞 技泼且 荐 乐档废
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
//	else if (FParse::Command(&Cmd, TEXT("TestLoadAllPCClass"))) // PCClassInfo 肺爹登绰 剧阑 临捞绰 霸 倔付唱 档框捞 登绰瘤 焊扁 困窃
//	{
//		double fStartTime = FPlatformTime::Seconds();
//		
//		UB2PCClassInfoBox* PCClassInfoBox = StaticFindPCClassInfoBox();
//		int32 FullyLoadedNum = 0;
//		if (PCClassInfoBox)
//		{
//			// 肺爹捞 救 登绢 乐促绰 啊沥俊辑 窍绰 巴烙.
//			for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
//			{
//				EPCClass ThisClassToLoad = IntToPCClass(PCI);
//
//				UB2PCClassInfo* LoadedClassInfo = PCClassInfoBox->GetSingleClassInfo(ThisClassToLoad);
//				if (LoadedClassInfo)
//				{
//					if (LoadedClassInfo->GetInGameOnlyInfo(Inworld)) // 角力肺绰 InGameOnlyInfo 鳖瘤 肺爹窍咯具 狼固 乐绰 剧狼 单捞磐甸捞 肺爹等促.
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
//	else if (FParse::Command(&Cmd, TEXT("TestLoadSkillAnims"))) // PCClassInfo 狼 老何肺辑 肺爹登带 SkillInfo 肺爹 抛胶飘
//	{
//		bool bLoadAll = FParse::Param(Cmd, TEXT("All")); // 酒聪搁 扁夯 葛氰 葛靛俊 鞘夸茄 巴父
//
//		double fStartTime = FPlatformTime::Seconds();
//
//		TArray<FCombinedPCSkillAnimID> AllSkillAnimIDToLoad;
//		UB2SkillAnimInfo* SkillAnimInfo = StaticFindSkillAnimInfo();
//		if (SkillAnimInfo)
//		{			
//			// 肺爹捞 救 登绢 乐促绰 啊沥俊辑 窍绰 巴烙.
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
//					// 扁夯 葛氰 葛靛俊 鞘夸茄 巴 捞寇俊 吧矾辰促.
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
//		// 捞饶 merge 唱 BeginPlay 矫俊 DefaultMaterial 阑 技泼窍绰单 溜籍俊辑 函版档 秦 林绊.. 
//		for (FActorIterator ActorIt(Inworld); ActorIt; ++ActorIt)
//		{
//			ABladeIICharacter* CastedChar = Cast<ABladeIICharacter>(*ActorIt);
//			if (CastedChar && CastedChar->GetMesh())
//			{ // bHideAllPCs true 秦 初绊 茄锅 见扁搁 促矫 SetActorHiddenInGame(false) 秦档 救 难咙.
//				USkeletalMeshComponent* MainMeshComp = CastedChar->GetMesh();
//				for (int32 MI = 0; MI < MainMeshComp->GetNumMaterials(); ++MI)
//				{
//					// 捞吧 舶阑 锭俊绰 Material override 甫 努府绢 秦 林绰单 酒付 捞犯霸 窍搁 盔贰 扁措登绰 吧肺 倒酒哎 芭 鞍瘤父 酒匆 荐档 乐促.
//					// 咯窍埃 抛胶飘 扁瓷牢 付寸俊 弊犯霸鳖瘤..
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
//			{ // 母蕉俊 嘿捞绰单 0 锅篮 捞饶 平俊辑 眠啊 炼例
//				B2Player->LightComponent->SetRelativeLocation(FVector(0.0f, 0.0f, B2Player->LightComponentAttachDistZ));
//				B2Player->LightComponent->AttachToComponent(B2Player->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//			}
//			else if(gTestAttachLightCompMode == 2)
//			{
//				// 风飘夯俊 弊成
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
//			// 霉 牢磊 绝澜 弊成 On/Off
//			gbTestTurnOffPlayerLight = !gbTestTurnOffPlayerLight;
//			
//			if (B2Player)
//			{
//				// 弊成 盎脚 伙酒辑 妮 窍绰 芭.
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
//		// AspectRatioAdaptiveFOV 俊 蝶弗 埃窜茄 抛胶飘.
//		for (TObjectIterator<UCameraComponent> CamIT; CamIT; ++CamIT)
//		{
//			UCameraComponent* ThisComp = Cast<UCameraComponent>(*CamIT);
//			if (ThisComp)
//			{
//				if (bWasSetAdaptiveFOV)
//				{ // AspectRatioAdaptiveFOV 甫 荤侩窍妨搁 老窜 Constraint 甫 窍瘤 臼酒具.
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
//		FString FirstArg; // LODGroup 瘤沥
//		FParse::Token(Cmd, FirstArg, true);
//		FString SecondArg; // LODBias 甫 倔付唱 炼例且瘤 瘤沥
//		FParse::Token(Cmd, SecondArg, true);
//
//		TEnumAsByte<enum TextureGroup> WantedLODGroup = TextureGroup::TEXTUREGROUP_MAX;
//		// UEnum 肺促啊 enum 巩磊凯阑 弊措肺 甘俏且 荐档 乐摆瘤父 弊成 抛胶飘 目盖靛捞聪 鞘夸茄 巴父 窜鉴洒...
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
//		// TEXTUREGROUP_MAX 搁 傈眉 促 窍绰 吧肺?
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
//		FString FirstArg; // 绢恫 局聪皋捞记阑 且瘤.. ELobbyRelaxingDoumiState 狼 int 某胶泼
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
//		FString FirstArg; // 绢恫 局聪皋捞记阑 且瘤.. ELobbyShopGirlState 狼 int 某胶泼
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
//				// 肺爹 肯丰 饶 勤甸傅 抗距.
//				ItemInfoAsyncLoadTestObj.MarkAsyncLoadWaiting(WantedClass, WantedGrade, RequestedName);
//			}
//		}
//		return true;
//	}
//	else if (FParse::Command(&Cmd, TEXT("PLOnOff")))
//	{
//		// 漂沥 敲阀汽狼 坊歹傅 巩力 炼荤 侩.
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
//		// 漂沥 敲阀汽狼 坊歹傅 巩力 炼荤 侩.
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
//			// 喊肺 侩狼急惑俊 乐瘤绰 臼瘤父 弊贰档 夸巴档 函版 咯瘤.
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
