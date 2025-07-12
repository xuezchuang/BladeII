
#include "B2PreRenderer.h"
#include "BladeIIGameImpl.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/SkeletalMeshActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "BladeIIGameMode.h"
#include "B2LobbyGameMode.h"
#include "B2NPCClassInfoBox.h"
#include "B2PCClassInfoBox.h"
#include "B2ItemInfo.h"
#include "B2SkillInfo.h"
#include "BladeIICharacter.h"
#include "Global.h"
#include "B2DamageEffectInfo.h"
#include "B2StageEventDirector.h"
#include "B2StageGameMode.h"
#include "B2SomeInfo.h"
#include "B2PreRenderGameMode.h"
#include "B2MonsterSpawnPool.h"
//#include "B2PreLoadingScreen.h"

#if PLATFORM_ANDROID
#include "Android/AndroidMisc.h"
#endif

bool FB2GMPreRenderDummyInfo::UpdateDummy(bool bForceContinue /*= false*/)
{
	//if (!PreRenderDummy) {
	//	return false;
	//}

	bool bNotFinishedCondition = bForceContinue; // 일단 ForceContinue 가 있으면 못 끝냄.

	//// 루트 컴포넌트에 대한 기본 PreRender 체크..
	//USkeletalMeshComponent* ThisDummySKComp = PreRenderDummy->GetSkeletalMeshComponent();
	//if (ThisDummySKComp) {
	//	ThisDummySKComp->SetExcessiveRenderProcessing(true);// 예전에 컴포넌트 쪽에 이 값을 넣어놓지 않았을 때 씬프록시가 업데이트 되길래 매번 세팅해 줬었는데.. 이제 필요없을 수도.
	//}
	//if (!ThisDummySKComp || ThisDummySKComp->LastRenderTime <= 0.0f) {
	//	bNotFinishedCondition = true;
	//}
	//if (PreRenderedCount < UB2PreRenderer::MaxPreRenderCount) {
	//	++PreRenderedCount;
	//	bNotFinishedCondition = true;
	//}

	//// 루트 SkeletalMeshComponent 에 붙은 추가 ParticleSystemComponent 들의 LastRenderTime 도 체크. 이들은 특히 EmitterDelay 등으로 인해 실제로는 더 돌려야 되는 경우도 있다.
	//int32 NotRenderedPSCNum = 0;
	//TArray<UActorComponent*> AllPSC = PreRenderDummy->GetComponentsByClass(UParticleSystemComponent::StaticClass());
	//for (UActorComponent* ThisAC : AllPSC)
	//{
	//	UParticleSystemComponent* ThisPSC = Cast<UParticleSystemComponent>(ThisAC);
	//	if (ThisPSC)
	//	{
	//		if (ThisPSC->LastRenderTime <= 0.0f){
	//			++NotRenderedPSCNum;
	//		}
	//		else {
	//			//ThisPSC->SetVisibility(false); // 한번 된 건 바로 visibility 꺼주려고 했는데.. EmitterDelay 같은 게 있어서.. 일단 취소.
	//		}
	//		ThisPSC->SetExcessiveRenderProcessing(true); // 예전에 컴포넌트 쪽에 이 값을 넣어놓지 않았을 때 씬프록시가 업데이트 되길래 매번 세팅해 줬었는데.. 이제 필요없을 수도.
	//	}
	//}

	//if (NotRenderedPSCNum > 0)
	//{
	//	bNotFinishedCondition = true;
	//}

	return !bNotFinishedCondition;
}

void FPreRenderCompleteState::MarkPCClassPreRendered(EPCClass InClass)
{
	PreRenderedPCs.AddUnique(InClass);
}
bool FPreRenderCompleteState::IsPCClassPreRendered(EPCClass InClass) const
{
	return (PreRenderedPCs.Contains(InClass));
}

void FPreRenderCompleteState::MarkSkillAnimPreRendered(FCombinedPCSkillAnimID InCombinedPCSkillAnimId)
{
	bool& ValueRef = PreRenderedSkillAnims.FindOrAdd(InCombinedPCSkillAnimId);
	ValueRef = true; // Key 만 있으면 되므로 딱히 필요는 없지만 의미상 넣어줌
}
bool FPreRenderCompleteState::IsSkillAnimPreRendered(FCombinedPCSkillAnimID InCombinedPCSkillAnimId) const
{
	return (PreRenderedSkillAnims.Find(InCombinedPCSkillAnimId) != nullptr);
}

void FPreRenderCompleteState::MarkNPCClassPreRendered(ENPCClass InClass, ENPCClassVariation InClassVari)
{
	MarkNPCClassPreRendered(FCombinedNPCClassID(InClass, InClassVari));
}
void FPreRenderCompleteState::MarkNPCClassPreRendered(FCombinedNPCClassID InCombinedNPCId)
{
	bool& ValueRef = PreRenderedNPCs.FindOrAdd(InCombinedNPCId);
	ValueRef = true; // Key 만 있으면 되므로 딱히 필요는 없지만 의미상 넣어줌
}
bool FPreRenderCompleteState::IsNPCClassPreRendered(ENPCClass InClass, ENPCClassVariation InClassVari) const
{
	return IsNPCClassPreRendered(FCombinedNPCClassID(InClass, InClassVari));
}
bool FPreRenderCompleteState::IsNPCClassPreRendered(FCombinedNPCClassID InCombinedNPCId) const
{
	return (PreRenderedNPCs.Find(InCombinedNPCId) != nullptr);
}

void FPreRenderCompleteState::MarkWorldLevelPreRendered(class UWorld* InWorld)
{
	UPackage* WorldOuterMost = InWorld ? InWorld->GetOutermost() : nullptr;
	if (WorldOuterMost)
	{
		// 현재 월드의 P 레벨에 해당하는 레벨 이름으로 Pre-render 여부를 마크.
		// 정식 레벨이면 이름 겹치는 거 없어야 함.
		bool& ValueRef = PreRenderedLevels.FindOrAdd(WorldOuterMost->GetFName());
		ValueRef = true; // Key 만 있으면 되므로 딱히 필요는 없지만 의미상 넣어줌
	}
}
bool FPreRenderCompleteState::IsWorldLevelPreRendered(class UWorld* InWorld) const
{
	UPackage* WorldOuterMost = InWorld ? InWorld->GetOutermost() : nullptr;
	if (WorldOuterMost)
	{
		return (PreRenderedLevels.Find(WorldOuterMost->GetFName()) != nullptr);
	}
	return false;
}

/**
 * ==================================================================
 * Pre-render 의 결과물인 셰이더 캐쉬가 r.UseProgramBinaryCache 에 의해 폰에 저장된다면
 * Pre-render 상태를 로컬에 저장 및 로드함으로서 다음번 실행시에도 한번 pre-render 를 거친 것을 반복하지 않도록 한다.
 * ==================================================================
 */

bool FPreRenderCompleteState::CanSavePreRenderState() const
{
#if PLATFORM_ANDROID
	// 바이너리 셰이더 캐쉬 저장 여부가 중요함.
	IConsoleVariable* UseProgramBinaryCacheVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.UseProgramBinaryCache"));
	return (FAndroidMisc::ShouldUseVulkan() || // Vulkan 은 OpenGL 과 별도로 저장되도록 처리됨.. FVulkanDynamicRHI::SavePipelineCache
		(UseProgramBinaryCacheVar && (UseProgramBinaryCacheVar->GetInt() != 0))
		//FOpenGLES2::SupportsProgramBinary() 이걸 위한 include 가 잘 안돼서.. OpenGL ES 의 SupportsProgramBinary 는 웬만큼 낡은 디바이스 아니면 될 꺼라고는 하는데.. 이 가정이 틀린 경우 플레이를 여러번 하다 보면 셰이더 캐싱의 악몽이 펼쳐질 것. 
		// 정 문제가 되면 FOpenGLProgramBinaryCache::Initialize 에서 캐시 했는지 여부를 전역변수로 빼 보던지..
		);
#elif PLATFORM_IOS
	// PreRender 일부를 하고 FShaderCache 로 저장도 함 (FShaderCache::InitShaderCache)
	return UB2PreRenderer::ShouldDoPreRenderForIOS();
#elif PLATFORM_WINDOWS
	return true; // 동작 테스트를 위해..
#else
	return false;
#endif
}

void FPreRenderCompleteState::SaveState()
{
	if (CanSavePreRenderState() && GConfig)
	{
		SavePCClassPreRenderState();
		SaveSkillAnimPreRenderState();
		SaveNPCClassPreRenderState();
		SaveWorldLevelPreRenderState();

		GConfig->SetBool(TEXT("Misc"), TEXT("PreRenderLevel"), bPreRenderLevelGoneThrough, GB2PreRenderSavedStateIni);
		GConfig->SetBool(TEXT("Misc"), TEXT("DamageEffectInfo"), bDamageEffectInfoPreRendered, GB2PreRenderSavedStateIni);
	}
}
void FPreRenderCompleteState::LoadState()
{
	if (CanSavePreRenderState() && GConfig)
	{
		LoadPCClassPreRenderState();
		LoadSkillAnimPreRenderState();
		LoadNPCClassPreRenderState();
		LoadWorldLevelPreRenderState();

		GConfig->GetBool(TEXT("Misc"), TEXT("PreRenderLevel"), bPreRenderLevelGoneThrough, GB2PreRenderSavedStateIni);
		GConfig->GetBool(TEXT("Misc"), TEXT("DamageEffectInfo"), bDamageEffectInfoPreRendered, GB2PreRenderSavedStateIni);
	}
}
void FPreRenderCompleteState::ClearState()
{
	// 흔히 쓸 기능은 아니고 원래 테스트 용 기능인데 혹 셰이더 캐쉬 리셋 같은 상황에서 필요한 기능이 될지도? 

	PreRenderedPCs.Empty();
	PreRenderedSkillAnims.Empty();
	PreRenderedNPCs.Empty();
	PreRenderedLevels.Empty();
	bPreRenderLevelGoneThrough = false;
	bDamageEffectInfoPreRendered = false;

	if (GConfig)
	{
		//GConfig->EmptyAll(GB2PreRenderSavedStateIni);
		GConfig->Flush(false, GB2PreRenderSavedStateIni);
	}
}

void FPreRenderCompleteState::SavePCClassPreRenderState()
{
	TArray<FString> SaveClassString;
	for (EPCClass ThisPCClass : PreRenderedPCs)
	{
		SaveClassString.Add(FString::Printf(TEXT("%d"), PCClassToInt(ThisPCClass)));
	}
	GConfig->SetArray(TEXT("PCClass"), TEXT("PCClass"), SaveClassString, GB2PreRenderSavedStateIni);
}
void FPreRenderCompleteState::LoadPCClassPreRenderState()
{
	PreRenderedPCs.Empty();

	TArray<FString> LoadedClassString;
	GConfig->GetArray(TEXT("PCClass"), TEXT("PCClass"), LoadedClassString, GB2PreRenderSavedStateIni);
	for (const FString& ThisClassStr : LoadedClassString)
	{
		PreRenderedPCs.Add(IntToPCClass(FCString::Atoi(*ThisClassStr)));
	}
}

void FPreRenderCompleteState::SaveSkillAnimPreRenderState()
{
	TArray<FString> SaveSkillAnimString;
	for (TMap<FCombinedPCSkillAnimID, bool>::TIterator SAIt(PreRenderedSkillAnims); SAIt; ++SAIt)
	{
		FCombinedPCSkillAnimID& ThisSkillAnimID = SAIt.Key();
		SaveSkillAnimString.Add(
			FString::Printf(TEXT("%d_%d"), PCClassToInt(ThisSkillAnimID.GetPCClassEnum()), (int32)ThisSkillAnimID.GetSkillAnimType())
		);
	}
	GConfig->SetArray(TEXT("SkillAnim"), TEXT("SkillAnim"), SaveSkillAnimString, GB2PreRenderSavedStateIni);
}
void FPreRenderCompleteState::LoadSkillAnimPreRenderState()
{
	PreRenderedSkillAnims.Empty();

	TArray<FString> LoadedSkillAnimString;
	GConfig->GetArray(TEXT("SkillAnim"), TEXT("SkillAnim"), LoadedSkillAnimString, GB2PreRenderSavedStateIni);
	for (const FString& ThisSkillAnimStr : LoadedSkillAnimString)
	{
		FString PCClassStr;
		FString SkillAnimTypeStr;
		if (ThisSkillAnimStr.Split(TEXT("_"), &PCClassStr, &SkillAnimTypeStr))
		{
			EPCClass ParsedPCClass = IntToPCClass(FCString::Atoi(*PCClassStr));
			ESkillAnimType SkillAnimType = (ESkillAnimType)(FCString::Atoi(*SkillAnimTypeStr));

			PreRenderedSkillAnims.Add(FCombinedPCSkillAnimID(ParsedPCClass, SkillAnimType), true);
		}
	}
}
void FPreRenderCompleteState::SaveNPCClassPreRenderState()
{
	TArray<FString> SavedNPCClassIDString;
	for (TMap<FCombinedNPCClassID, bool>::TIterator NPCIt(PreRenderedNPCs); NPCIt; ++NPCIt)
	{
		FCombinedNPCClassID& ThisNPCID = NPCIt.Key();
		SavedNPCClassIDString.Add(
			FString::Printf(TEXT("%d_%d"), (int32)ThisNPCID.GetNPCClassEnum(), (int32)ThisNPCID.GetNPCVariationEnum())
		);
	}
	GConfig->SetArray(TEXT("NPCClass"), TEXT("NPCClass"), SavedNPCClassIDString, GB2PreRenderSavedStateIni);
}
void FPreRenderCompleteState::LoadNPCClassPreRenderState()
{
	PreRenderedNPCs.Empty();

	TArray<FString> LoadedNPCIDString;
	GConfig->GetArray(TEXT("NPCClass"), TEXT("NPCClass"), LoadedNPCIDString, GB2PreRenderSavedStateIni);
	for (const FString& ThisNPCIDStr : LoadedNPCIDString)
	{
		FString NPCClassStr;
		FString NPCVariStr;
		if (ThisNPCIDStr.Split(TEXT("_"), &NPCClassStr, &NPCVariStr))
		{
			ENPCClass ParsedNPCClass = (ENPCClass)FCString::Atoi(*NPCClassStr);
			ENPCClassVariation NPCVariation = (ENPCClassVariation)(FCString::Atoi(*NPCVariStr));

			PreRenderedNPCs.Add(FCombinedNPCClassID(ParsedNPCClass, NPCVariation), true);
		}
	}
}
void FPreRenderCompleteState::SaveWorldLevelPreRenderState()
{
	TArray<FString> SavedLevelString;
	for (TMap<FName, bool>::TIterator PreRenderLevelIt(PreRenderedLevels); PreRenderLevelIt; ++PreRenderLevelIt)
	{
		SavedLevelString.Add(PreRenderLevelIt.Key().ToString());
	}
	GConfig->SetArray(TEXT("WorldLevel"), TEXT("WorldLevel"), SavedLevelString, GB2PreRenderSavedStateIni);
}
void FPreRenderCompleteState::LoadWorldLevelPreRenderState()
{
	PreRenderedLevels.Empty();

	TArray<FString> LoadedLevelString;
	GConfig->GetArray(TEXT("WorldLevel"), TEXT("WorldLevel"), LoadedLevelString, GB2PreRenderSavedStateIni);
	for (const FString& ThisLevelStr : LoadedLevelString)
	{
		PreRenderedLevels.Add(FName(*ThisLevelStr));
	}
}

void FPreRenderPerMaterialState::AddUsedMobility(EComponentMobility::Type InMobility)
{
	AllUsedMobilities.AddUnique(InMobility);
}
bool FPreRenderPerMaterialState::CheckMobilityUsed(EComponentMobility::Type InMobility) const
{
	return AllUsedMobilities.Contains(InMobility);
}
const FName FPreRenderMaterialCompleteState::GetMaterialKeyName(class UMaterialInterface* InCheckMtrl) const
{
	// 셰이더 생성 차원에서 material 을 구분하자면 
	// 우리로서는 StaticSwitch 를 배제하고 모든 조합마다 별도의 MasterMaterial 을 사용하는 방향이라 부모 material 만 보면 될 수도 있는데
	// 확실히 아닐 수도 있어서 일단 그냥 MaterialInterface 단위로 체크
	// 단, MID 는 인게임 동적 생성이라 예외로..
	UMaterialInterface* RetMtrl = InCheckMtrl;
	UMaterialInstanceDynamic* CastedMID = Cast<UMaterialInstanceDynamic>(InCheckMtrl);
	if (CastedMID)
	{
		RetMtrl = CastedMID->Parent; // MIC 거나 Material 일 껀데 굳이 체크할 필요까지는 없을 거 같고.
	}
	if (RetMtrl)
	{
		return FName(*RetMtrl->GetPathName());
	}
	return NAME_None;
}
void FPreRenderMaterialCompleteState::MarkPrimCompMaterials(class UPrimitiveComponent* InPrimComp)
{
	if (InPrimComp)
	{
		TArray<UMaterialInterface*> AllUsedMtrls;
		InPrimComp->GetUsedMaterials(AllUsedMtrls);

		for (UMaterialInterface* ThisOne : AllUsedMtrls)
		{
			const FName ThisMtrlName = GetMaterialKeyName(ThisOne);
			if (ThisMtrlName != NAME_None)
			{
				FPreRenderPerMaterialState& FoundOrAddPerMtrlState = AllStates.FindOrAdd(ThisMtrlName);
				FoundOrAddPerMtrlState.AddUsedMobility(InPrimComp->Mobility);
			}
		}
	}
}
bool FPreRenderMaterialCompleteState::ArePrimCompAllMaterialsPreRendered(class UPrimitiveComponent* InPrimComp) const
{ // InPrimComp 가 사용하는 모든 Material 이 처리되었는지 체크
	if (InPrimComp)
	{
		TArray<UMaterialInterface*> AllUsedMtrls;
		InPrimComp->GetUsedMaterials(AllUsedMtrls);

		bool bFoundNonRendered = false;
		for (UMaterialInterface* ThisOne : AllUsedMtrls)
		{
			const FName ThisMtrlName = GetMaterialKeyName(ThisOne);
			if (ThisMtrlName != NAME_None)
			{
				const FPreRenderPerMaterialState* FoundState = AllStates.Find(ThisMtrlName);
				// 이 Material 을 아예 처리한 적 없거나, 있더라도 mobility 가 달랐다면.. (그 경우 실제 셰이더는 다를 확률이 높다)
				if (!FoundState || !FoundState->CheckMobilityUsed(InPrimComp->Mobility))
				{
					bFoundNonRendered = true;
					break;
				}
			}
		}
		return !bFoundNonRendered; // 아, 물론 material 이 전혀 없어도 true 리턴이 되겠는데 이거 사용 목적상 그게 맞긴 하겠다.
	}
	return false;
}
void FPreRenderMaterialCompleteState::ClearState()
{
	AllStates.Empty();
}

FPreRenderCompleteState UB2PreRenderer::TotalCompleteState;
FPreRenderMaterialCompleteState UB2PreRenderer::MaterialBasedCompleteState;

UB2PreRenderer::UB2PreRenderer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OwnerGameMode = nullptr;

	OverallTickCount = 0;
	bCanDoOverallCount = false;
	TickCountFromSetup = 0;
	PreRenderAllPrimTickCount = 0;

	bLightEnvironmentSetupForDynPointLight = false;
	bLocalPlayerHeadLightModified = false;
	LastCachedLocalPCLightRadius = 0.0f;
	LastCachedLocalPCLightIntensity = 0.0f;
	PreRenderDummyPointLight = nullptr;

	bLightEnvironmentSetupForDynDirLight = false;
	PreRenderDirLight = nullptr;

	bHadDoneAnyPreRender = false;
	bPendingForPreRenderGM = false;

	//CachedPreLoadingScreen = nullptr;
	EstimatedTotalProgressStep = 0;
	CurrPrerenderProgressStep = 0;
	ProgressStepCountForSelectedPhase = 0;
}

bool UB2PreRenderer::SetupPreRenderObjects(class ABladeIIGameMode* InGM) // 셰이더 컴파일을 미리 해 놓지 않는 모바일 플랫폼에서의 셰이더 컴파일을 위한 기능. 여기서 놓치는 게 있다고 뭐가 작동 안하는 건 아니고 걍 중간에 필요해 질 때 버벅거림.
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2PreRenderer::SetupPreRenderObjects"));
	B2_SCOPED_TIME_LOG(TEXT("UB2PreRenderer::SetupPreRenderObjects"));

	OwnerGameMode = InGM;
	check(OwnerGameMode);
	if (!OwnerGameMode)
	{
		return false;
	}

	// 원칙적으로 이 Material 단위 Pre-render 상태 트랙킹은 월드를 지나서도 유지가 될 수 있는 것이지만
	// 아직은 동일한 material 이더라도 다른 조명 조건 하에서의 셰이더 컴파일 처리가 확실하지 않아 매 레벨마다 리셋하는 걸로 함.
	MaterialBasedCompleteState.ClearState();

	AllPreRenderDummy.Empty();
	UWorld* TheWorld = OwnerGameMode->GetWorld();

	if (GetB2GameModeType(OwnerGameMode) == EB2GameMode::Lobby)
	{
		// 로비는 특수성으로 인해 거의 별도 처리함. 사실상 게임 실행하고 맨 처음 한 번 처리하는 게 될 것.
		SetupPreRenderObjects_FirstLobby();
	}
	else
	{
#if PLATFORM_IOS
		if (ShouldDoPreRenderForIOS_Basic())
#endif
		{
			{
				// PC 기본 데이터는 사실상 대부분은 PreRenderGameMode 에서 게임 설치 후 처음 한번 처리가 될 것이다.
				TArray<EPCClass> PCClassesToLoad = OwnerGameMode->GetPCClassesToPreLoad();
				for (EPCClass ThisCharClass : PCClassesToLoad)
				{
					ConditionalSetupPreRenderObjectForPCClass(ThisCharClass);
				}

				// PreloadAnyNecessaryInfo 할 때 긁어온 NPCClass 들로 prerender 더미 생성
				TArray<FCombinedNPCClassID> AllExpectedNPCs;
				AB2MonsterSpawnPool* ActiveSP = OwnerGameMode->GetActiveSpawnPool();
				if (ActiveSP)
				{
					ActiveSP->GetAllExpectedNPCIDs(AllExpectedNPCs);
				}
				for (FCombinedNPCClassID ThisMobId : AllExpectedNPCs)
				{
					ConditionalSetupPreRenderObjectForNPCClass(ThisMobId);
				}

				// PCClass 데이터 중 덩치가 크고 현재 PC 성장 상태에 따라 별도 관리가 가능한 Skill Anim 데이터..
				TArray<FCombinedPCSkillAnimID> SkillAnimsToPreLoad = OwnerGameMode->GetPCSkillAnimsToPreLoad();
				ConditionalSetupPreRenderObjectForPCSkillAnims(SkillAnimsToPreLoad);

				// DamageEffectInfo.. 인게임 성능에 영향을 꽤나 미치다 보니 얘들도 pre-render 에 포함. 얘들도 처음 한번만 필요할 듯 한데..
				ConditionalSetupPreRenderObjectForDamageEffectInfo();
			}

			// 연출 셋업에 대한 PreRender 셋업. 현재 월드에 대해서 이전에 pre-render 한적 없을 때에만. 
			// 사실 아래 StageEventDirector 는 같은 레벨이라도 StageNumber 에 따라 사용 여부가 다를 수 있어서 그것까지 체크하는 게 확실하긴 하다.
			// 실제로는 이것까지 빠득빠득 pre-render 를 철저하게 거는 게 크게 효과가 있는 것 같지는 않아서 StageNumber 상관없이 한번만 걸어둠.
			if (!IsCurrentWorldLevelPreRendered())
			{
				ASkeletalMeshActor* ExtraDummySKActor = GetExtraPrerenderDummySKActor(); // ExtraDummy 는 필요한 상황에서만 꺼내온다.
				if (ExtraDummySKActor)
				{
					for (AB2StageEventDirector* SED : OwnerGameMode->GetEventDirectorArray())
					{
						if (SED && SED->IsStageSupportedForGameMode(OwnerGameMode))
						{
							TArray<ASkeletalMeshActor*> ShowSettingUnboundSKActors;
							SED->SetupPreRenderSKMeshActor(ExtraDummySKActor, ShowSettingUnboundSKActors);

							for (ASkeletalMeshActor* NewSKActor : ShowSettingUnboundSKActors)
							{ // ShowSettings 에 연결되지 않은 연출에만 사용되는 SkeletalMeshActor 들 생성된 거. 얘들도 PreRenderDummy 로..
								FB2GMPreRenderDummyInfo NewDummyInfo;
								NewDummyInfo.PreRenderDummy = NewSKActor;
								AllPreRenderDummy.Add(NewDummyInfo);
							}
						}
					}
				}
			}
		}
	}

	// 공통적인 후처리
	PostPreRenderSeupProcess();

	// 생성된 더미 정보가 없더라도 AllPrimitives 옵션이 있거나 전용 레벨 진입 예정이면 이후 진행을 하게 될 수 있으니 true 리턴
	if (AllPreRenderDummy.Num() > 0 || (bPreRenderAllPrimitives && !IsCurrentWorldLevelPreRendered()) || bPendingForPreRenderGM)
	{
		bHadDoneAnyPreRender = true; // 아직 실제로 한 건 아니지만 이 플래그 목적 상 이쯤에서 true 마크해도 좋겠다.

		if (!bPendingForPreRenderGM)
		{ // Pre-render 되는 동안 화면 가림막 세팅
			SetupDefaultPreRenderScreen();
		}

		return true;
	}

	return false; // 만일 현재 구성이 죄다 이전에 Pre-render 를 거쳤다면 이렇게 가게 될 듯. 로딩 시간 페널티 없으니 좋은 거.
}

void UB2PreRenderer::ConditionalSetupPreRenderObjectForPCClass(EPCClass InClass, bool bForceSetup)
{
	if ((!TotalCompleteState.IsPCClassPreRendered(InClass) || bForceSetup)
		&& OwnerGameMode)
	{
		UWorld* TheWorld = OwnerGameMode->GetWorld();

		FB2GMPreRenderDummyInfo ThisDummyInfo;

		// 로컬 캐릭터의 장착 장비와 날개 정보를 가져 오는데 웬만해서는 처음에 한번 돌릴 꺼면 기본 장비들만 사용하게 될 듯. 
		// 같은 클래스면 설령 장비가 달라도 어지간하면 부모 material 이라도 같을 테니 어떤 장비를 입히든 큰 상관은 없을 것 같은데.. 여하간 별로 의미는 없는 코드 ㅋ
		// 실제로 여기서 중요한 건 애니메이션에 붙은 이펙트들이고 그건 클래스 단위로만 보면 됨.
		TArray<FB2Item> ThisCharEquipment;
		FB2Wing ThisCharWing;
		BladeIIGameImpl::GetLocalCharacterData().GetEquippedItems(InClass, ThisCharEquipment);
		BladeIIGameImpl::GetLocalCharacterData().GetEquippedCostumeItems(InClass, ThisCharEquipment);
		bool bHasWing = BladeIIGameImpl::GetLocalCharacterData().GetCharacterWing(InClass, ThisCharWing);
		ICharacterDataStore::GetRenderItem(&BladeIIGameImpl::GetLocalCharacterData(), InClass, ThisCharEquipment);

		ThisDummyInfo.PreRenderDummy = SpawnPreRenderMeshCommon(TheWorld); // 기본 SkeletalMeshActor Spawn

		if (ThisDummyInfo.PreRenderDummy)
		{
			// Mesh merge.. AnimBP 세팅도 들어갈 것임. 
			SetupDummyPCMeshForPreRender(InClass, NULL, ThisDummyInfo.PreRenderDummy, ThisCharEquipment, bHasWing ? &ThisCharWing : NULL);
			// AnimBP 를 통해 긁어모은 ParticleSystem 들도 뿌려준다.
			SetupDummyParticleSystemsForPreRender(ThisDummyInfo.PreRenderDummy);
		}

		TotalCompleteState.MarkPCClassPreRendered(InClass); // 셋업만 하고 걍 한 걸로 마크. 정말 다 처리 된 건지 끝까지 추적하려면 버겁기도 하고 초가삼간 태우면서 그럴 필요까지 있을 것 같지도 않고..

		AllPreRenderDummy.Add(ThisDummyInfo);
	}
}
void UB2PreRenderer::ConditionalSetupPreRenderObjectForNPCClass(const FCombinedNPCClassID InClassID, bool bForceSetup)
{
	if ((!TotalCompleteState.IsNPCClassPreRendered(InClassID) || bForceSetup)
		&& OwnerGameMode)
	{
		UWorld* TheWorld = OwnerGameMode->GetWorld();

		FB2GMPreRenderDummyInfo ThisDummyInfo;
		ThisDummyInfo.PreRenderDummy = SpawnPreRenderMeshCommon(TheWorld); // 기본 SkeletalMeshActor Spawn

		if (ThisDummyInfo.PreRenderDummy)
		{
			SetupDummyMobForPreRender(InClassID.GetNPCClassEnum(), InClassID.GetNPCVariationEnum(), NULL, ThisDummyInfo.PreRenderDummy);
			// AnimBP 를 통해 긁어모은 ParticleSystem 들도 뿌려준다.
			SetupDummyParticleSystemsForPreRender(ThisDummyInfo.PreRenderDummy);
		}

		TotalCompleteState.MarkNPCClassPreRendered(InClassID); // 셋업만 하고 걍 한 걸로 마크. 정말 다 처리 된 건지 끝까지 추적하려면 버겁기도 하고 초가삼간 태우면서 그럴 필요까지 있을 것 같지도 않고..

		AllPreRenderDummy.Add(ThisDummyInfo);
	}
}
void UB2PreRenderer::ConditionalSetupPreRenderObjectForPCSkillAnims(const TArray<FCombinedPCSkillAnimID>& InSkillIDs, bool bForceSetup)
{
	// 사용 편의 상 스킬애니메이션 하나씩이 아니고 여러개를 받도록 함.

	UB2SkillAnimInfo* SkillAnimInfo = StaticFindSkillAnimInfo(OwnerGameMode);
	if (!SkillAnimInfo)
	{
		return;
	}

	//TMap<FName, UParticleSystem*> GatheredPS;
	//for (const FCombinedPCSkillAnimID ThisSkillAnimID : InSkillIDs)
	//{
	//	if (!TotalCompleteState.IsSkillAnimPreRendered(ThisSkillAnimID) || bForceSetup)
	//	{
	//		// Async 로딩을 하더라도 사실상 여기서 필요하면 로딩을 끝까지 할 것이다.
	//		UAnimSequenceBase* ThisSkillAnim = SkillAnimInfo->GetSingleSkillAnim(ThisSkillAnimID);
	//		if (!ThisSkillAnim) {
	//			continue;
	//		}
	//		TryGatherAllBoundPSFromAnimSeq(ThisSkillAnim, GatheredPS); // 중복되는 거 감안해 가면서 모음.
	//		// 구조상 여기도 아직 셋업이 완료되지는 않았지만 걍 한 걸로 마크.
	//		TotalCompleteState.MarkSkillAnimPreRendered(ThisSkillAnimID);
	//	}
	//}
	//if (GatheredPS.Num() > 0)
	//{
	//	// ExtraDummy 는 필요한 상황에서만 꺼내와야 한다. 안 그러면 실제로 Pre-render 가 필요없는데도 한 프레임 정도 차양막이 가려지는 낭비를 하게 될 수 있다.
	//	ASkeletalMeshActor* ExtraDummySKActor = GetExtraPrerenderDummySKActor();
	//	if (ExtraDummySKActor)
	//	{
	//		// 모든 SkillAnimsToPreLoad 들의 particle system 들을 모은 이후에 Prerender 용 component 를 spawn 시킨다.
	//		for (TMap<FName, UParticleSystem*>::TConstIterator PSIt(GatheredPS); PSIt; ++PSIt)
	//		{
	//			UParticleSystem* ThisPS = PSIt.Value();
	//			// DummySKActor 에 attach 시키고 이후는 DummySKActor를 컨트롤
	//			UParticleSystemComponent* ThisSpawnedFxComp = SpawnDummyPSCForPreRender(ThisPS, ExtraDummySKActor->GetRootComponent());
	//		}
	//	}
	//}
}
void UB2PreRenderer::ConditionalSetupPreRenderObjectForDamageEffectInfo(bool bForceSetup)
{
	UB2DamageEffectInfo* DmgFxInfo = StaticFindDamageEffectInfo(OwnerGameMode);
	if ((!TotalCompleteState.IsDamageEffectInfoPreRendered() || bForceSetup)
		&& DmgFxInfo)
	{
		TArray<ASkeletalMeshActor*> DummiesForMtrlOverride; // MaterialOverride 는 각각 별개 SKActor 가 필요..
		int32 DesiredDummyNum = DmgFxInfo->GetDesiredPreRenderDummyNumForMtrlOverride();
		for (int32 DI = 0; DI < DesiredDummyNum; ++DI)
		{
			DummiesForMtrlOverride.Add(GetAdditionalPrerenderDummySKActor());
		}
		DmgFxInfo->SetupPreRenderMaterialOverride(DummiesForMtrlOverride);

		ASkeletalMeshActor* ExtraDummySKActor = GetExtraPrerenderDummySKActor(); // 아마도 바로 위에서 만든 더미 중 첫번째 것이 될 듯.
		if (ExtraDummySKActor)
		{
			DmgFxInfo->SetupPreRenderSKMeshActorForFX(ExtraDummySKActor);
		}

		TotalCompleteState.MarkDamageEffectInfoPreRendered(); // 셋업만 하고 걍 한 걸로 마크.
	}
}
void UB2PreRenderer::PostPreRenderSeupProcess()
{
	//// AllPrim 이 아닌 PreRenderDummy 를 사용하는 일반 PreRender 를 위한 기본 셋업 이후 처리.
	//for (FB2GMPreRenderDummyInfo& ThisPreRenderInfo : AllPreRenderDummy)
	//{
	//	if (ThisPreRenderInfo.PreRenderDummy && ThisPreRenderInfo.PreRenderDummy->GetSkeletalMeshComponent())
	//	{
	//		ThisPreRenderInfo.PreRenderDummy->SetActorHiddenInGame(false);
	//		ThisPreRenderInfo.PreRenderDummy->GetSkeletalMeshComponent()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	//		ThisPreRenderInfo.PreRenderDummy->GetSkeletalMeshComponent()->TickAnimation(0.01f, false); // 한틱만 렌더링하는 경우를 위해서 한번쯤은 TickAnim 을 돌려준다.
	//	}
	//	ThisPreRenderInfo.PreRenderedCount = 0;
	//}

	//OverallTickCount = 0;
	//bCanDoOverallCount = false;
	//TickCountFromSetup = 0;

	//FlushRenderingCommands(); // 이렇게 하면 좀 도움이 되려나...
}

void UB2PreRenderer::SetupPreRenderObjects_FirstLobby()
{
	// LobbyGameMode 를 위함. LobbyGameMode 에서는 특별히 게임 설치 후 첫 실행시 한번 pre-rendering 을 돌릴 오브젝트들을 처리한다. 
	// 이후에 스테이지에서 돌아오거나 할 때에도 실행되지 않음.

	check(OwnerGameMode && Cast<AB2LobbyGameMode>(OwnerGameMode));

	// 최초 pre-render 레벨 진입이 필요하고 또한 가능한지 체크만 한다.
	// 실질적으로 FirstLobby 의 의미로는 이 루트가 기본이 될 것.
	if (!IsPreRenderLevelGoneThrough() && CheckBladeIIPreRenderGameAvailability()
#if PLATFORM_IOS
		&& ShouldDoPreRenderForIOS_PreRenderGM()
#endif
		)
	{
		bPendingForPreRenderGM = true;
	}

	// 로비맵 자체의 상태도 있음.
	if (IsCurrentWorldLevelPreRendered())
	{
		return;
	}

	B2_SCOPED_TRACK_LOG(TEXT("UB2PreRenderer::SetupPreRenderObjects_FirstLobby"));

	if (!bPendingForPreRenderGM && !IsPreRenderLevelGoneThrough()
#if PLATFORM_IOS
		// 이것도 사실상 PreRenderGM 과 비슷한 의미라면 ShouldDoPreRenderForIOS_PreRenderGM 으로 아싸리 윗단에서 넘어가도록 하는 게 더 말이 될수도 있다.
		&& ShouldDoPreRenderForIOS_Basic()
#endif
		)
	{
		// Pre-render 전체를 다 하는 Android 의 경우 실질적으로 이 루트로 들어오는 일은 없어야 할 듯. 지금은 안전장치로서의 의미 뿐인가..

		// 준비가 안되었거나 맵을 찾지 못했거나.. 어떤 경우든 로비에서 직접 실행한다.
		// 이 때에는 Extra 만 실행하는 거. 로비에서 쓰는 것들이 등록된 건 Extra 뿐이니.
		SetupPreRenderObjects_ExtraNPCClass(); // ExtraNPCClass 는 여기서 안 해도 맵 로딩 하면서 하게 되지만 그냥 미리 해 두지..
		SetupPreRenderObjects_ExtraFx();
		SetupPreRenderObjects_ExtraOthers();
	}

	// 로비를 위한 pre-render 가 실행 되었다는 의미 + AllPrim 처리를 막기 위해서라도 Mark
	// PreRenderGame 을 로딩한다 하더라도 사실상 로비에서 하려던 걸 하는 거라 실행이 되었다고 마크해야.
	MarkCurrentWorldLevelPreRendered();
}

void UB2PreRenderer::SetupPreRenderObjects_ExtraFx()
{
	// 특정 분류 없이 SomeInfo 에 Pre-render 용으로 등록해 놓은 애들.
	// 월드, 캐릭터, 혹은 특정 InfoAsset 에 얽혀서 일괄적으로 처리되는 대신 특정하게 찝어서 Pre-render 처리되는 이펙트 리소스 목록
	UB2SomeInfo* SomeInfo = StaticFindSomeInfo(OwnerGameMode);

	ASkeletalMeshActor* ExtraDummySKActor = GetExtraPrerenderDummySKActor();
	if (!ExtraDummySKActor)
	{
		return;
	}

	if (CachedExtraPreRenderPS.Num() == 0) // SomeInfo 에서 여기에 사용할 리스트는 LoadAnd"Consume" 을 하는데 이쪽에서는 PreRenderGameMode 에서 다중으로 돌릴 수 있으므로 따로 캐싱해 놓음.
	{
		if (SomeInfo)
		{
			SomeInfo->LoadAndConsumeUniquePreRenderPS(CachedExtraPreRenderPS);
		}
	}
	for (UParticleSystem* ThisPS : CachedExtraPreRenderPS)
	{
		UParticleSystemComponent* ThisSpawnedFxComp = SpawnDummyPSCForPreRender(ThisPS, ExtraDummySKActor->GetRootComponent());
	}

	bHadDoneAnyPreRender = true; // 아직 실제로 한 건 아니지만 이 플래그 목적 상 이쯤에서 true 마크해도 좋겠다.
}
void UB2PreRenderer::SetupPreRenderObjects_ExtraNPCClass()
{
	// 특정 스테이지 처음 플레이 시 처음 등장하는 NPCClass 에 대해서 pre-render 가 실행되는데 여기서 하는 건 그거랑 마찬가지이다. 
	// 단, 이렇게 SomeInfo 에 등록해 놓고 pre-render 를 따로 수행할 수 있도록 한건 이런 걸 PreReder 레벨에서 미리 해 놓고 처음 전투맵 로딩할 때 pre-render 로 인한 페널티를 좀 줄이고자 하기 위함.
	// 모든 NPCClass 들이 다 이걸 통해 처리되지는 못한다. 적절히 골라서..

	UB2SomeInfo* SomeInfo = StaticFindSomeInfo(OwnerGameMode);

	if (CachedExtraPreRenderNPCClassInfo.Num() == 0) // SomeInfo 에서 여기에 사용할 리스트는 LoadAnd"Consume" 을 하는데 이쪽에서는 PreRenderGameMode 에서 다중으로 돌릴 수 있으므로 따로 캐싱해 놓음.
	{
		if (SomeInfo)
		{
			SomeInfo->LoadAndConsumeUniquePreRenderNPCClass(CachedExtraPreRenderNPCClassInfo);
		}
	}
	for (TSubclassOf<class UB2NPCSingleClassInfo> ThisInfo : CachedExtraPreRenderNPCClassInfo)
	{
		UB2NPCSingleClassInfo* ThisInfoObj = ThisInfo ? Cast<UB2NPCSingleClassInfo>(ThisInfo->GetDefaultObject()) : nullptr;
		if (ThisInfoObj)
		{
			ConditionalSetupPreRenderObjectForNPCClass(FCombinedNPCClassID(ThisInfoObj->NPCClassEnum, ThisInfoObj->ClassVariation), true);
		}
	}

	bHadDoneAnyPreRender = true; // 아직 실제로 한 건 아니지만 이 플래그 목적 상 이쯤에서 true 마크해도 좋겠다.
}

void UB2PreRenderer::SetupPreRenderObjects_ExtraOthers()
{
	// 특정 분류 없이 SomeInfo 에 Pre-render 용으로 등록해 놓은 애들.. 중에서도 나머지..
	UB2SomeInfo* SomeInfo = StaticFindSomeInfo(OwnerGameMode);

	if (CachedExtraPreRenderSKMesh.Num() == 0) // SomeInfo 에서 여기에 사용할 리스트는 LoadAnd"Consume" 을 하는데 이쪽에서는 PreRenderGameMode 에서 다중으로 돌릴 수 있으므로 따로 캐싱해 놓음.
	{
		if (SomeInfo)
		{
			SomeInfo->LoadAndConsumeUniquePreRenderSKMesh(CachedExtraPreRenderSKMesh);
		}
	}
	for (USkeletalMesh* ThisMesh : CachedExtraPreRenderSKMesh)
	{
		FB2GMPreRenderDummyInfo NewExtraDummyInfo;
		NewExtraDummyInfo.PreRenderDummy = SpawnPreRenderMeshCommon(OwnerGameMode ? OwnerGameMode->GetWorld() : GetWorld());
		if (NewExtraDummyInfo.PreRenderDummy && NewExtraDummyInfo.PreRenderDummy->GetSkeletalMeshComponent())
		{
			NewExtraDummyInfo.PreRenderDummy->GetSkeletalMeshComponent()->SetSkeletalMesh(ThisMesh);
		}
		AllPreRenderDummy.Add(NewExtraDummyInfo);
	}

	bHadDoneAnyPreRender = true;
}

void UB2PreRenderer::ConditionalSetupPreRenderLightEnvForDynPL(bool bForceSetup)
{
	//const bool bShouldSetupDummyDynPointLight = (!bLightEnvironmentSetupForDynPointLight || bForceSetup);
	//const bool bShouldSetupPlayerHeadLight = (!bLocalPlayerHeadLightModified || bForceSetup);

	//if (!GIsEditor && (bShouldSetupDummyDynPointLight || bShouldSetupPlayerHeadLight))
	//{
	//	AB2StageGameModeBase* CastedSGM = Cast<AB2StageGameModeBase>(OwnerGameMode);
	//	if (CastedSGM)
	//	{ // 원래 이거 목적은 연출때만 켜지도록 의도해서 설치해 놓은 Dynamic PointLight 들을 일괄적으로 꺼 놓기 위함인데 StageGameMode BeginPlay 때 실행이 된다.
	//	  // 여기서 미리 한차례 해 줌. 영향을 미치는 PointLight 를 여기서 셋업하는 걸로 확실히 한정하기 위해.
	//		// 근데 이렇게 해도 결국 PreRender 돌아가는 도중에 연출이 시작되면서 연출에서 사용하는 PointLight 를 켜게 되는데.. AllPrim phase 시작 시 걸러냄.
	//		CastedSGM->DeactivateWorldMovablePointLight();
	//	}
	//}

	//if (bShouldSetupDummyDynPointLight)
	//{
	//	// DynamicPointLight 를 추가로 하나 생성. 플레이어 캐릭터 PointLight 설정만으로는 안 먹힐 때가 있다.. 우리 게임은 모바일에서 두개까지 가능하도록 설정해 놓았으니 이것까지.
	//	UWorld* TheWorld = OwnerGameMode ? OwnerGameMode->GetWorld() : GetWorld();
	//	if (TheWorld)
	//	{
	//		FActorSpawnParameters SpawnInfo;
	//		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//		SpawnInfo.ObjectFlags |= RF_Transient;
	//		FTransform SpawnTransform;
	//		SpawnTransform.SetLocation(GetPreRenderDummyDesiredLocation(TheWorld) + FVector(0.0f,0.0f,10000.0f)); // 위치를 좀 높여보자. 혹여나 기존 높이에서는 가리기라도 할런지.. ㅡㅡ;;
	//		PreRenderDummyPointLight = TheWorld->SpawnActor<APointLight>(APointLight::StaticClass(), SpawnTransform, SpawnInfo);
	//		if (PreRenderDummyPointLight)
	//		{
	//			PreRenderDummyPointLight->SetMobility(EComponentMobility::Movable);
	//			UPointLightComponent* PLComp = Cast<UPointLightComponent>(PreRenderDummyPointLight->GetLightComponent());
	//			if (PLComp)
	//			{
	//				PLComp->bAffectsWorld = true;
	//				PLComp->Activate();
	//				PLComp->RegisterComponent();
	//				PLComp->SetAttenuationRadius(1000000.0f);
	//				PLComp->SetIntensity(1000000.0f);
	//				PLComp->MarkRenderStateDirty();
	//			}

	//			bLightEnvironmentSetupForDynPointLight = true;
	//		}
	//	}
	//}

	//// bLightEnvironmentSetupForDynPointLight 안쪽에 위치시킬 수도 있긴 한데.. 따로 가자..
	//if (bShouldSetupPlayerHeadLight)
	//{
	//	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(OwnerGameMode));
	//	if (LocalB2Player && LocalB2Player->LightComponent)
	//	{
	//		// 설령 플레이어 point light 를 허용하지 않는 게임모드라 해도 여기선 켠다. 캐릭터 별 Pre-render 가 한번만 돌아가게 되는데 하필 라이트 꺼진 채로 돌아가면 효과가 반감되니..

	//		LastCachedLocalPCLightIntensity = LocalB2Player->LightComponent->Intensity;
	//		LastCachedLocalPCLightRadius = LocalB2Player->LightComponent->AttenuationRadius;

	//		// 반경을 무지 넓게 해서 Pre-render 를 걸려는 월드 오브젝트들에 영향이 가도록 함.
	//		LocalB2Player->LightComponent->SetAttenuationRadius(1000000.0f);
	//		LocalB2Player->LightComponent->SetIntensity(1000000.0f); // Intensity 는 혹시나 해서 ㅋ

	//		// 꺼져 있을 수도 있음. 사실 꺼져 있으면 차라리 하나를 spawn 시키는 게 나을 듯도. 
	//		// 모바일에서 Dynamic Pointlight 개수 제한이 있다 보니 뭔가 그게 잘 안먹혀서 그러긴 했음.
	//		// FormalSetPointLightOn 를 거치지 않고 직접 (몰래 슬쩍) 켠다. 조심해서 다뤄야 함.
	//		LocalB2Player->LightComponent->bAffectsWorld = true; // bHeadPointLightIsFormallyOn 은 이전 값을 유지한 채로 bAffectsWorld 만 켬
	//		LocalB2Player->LightComponent->Activate();
	//		LocalB2Player->LightComponent->RegisterComponent();
	//		LocalB2Player->LightComponent->MarkRenderStateDirty();

	//		bLocalPlayerHeadLightModified = true; // bLightEnvironmentSetupForDynPointLight 과는 별도로 상태를 마크해둔다. 혹여나 타이밍 때문에 다를 수도 있으므로.
	//	}
	//}
}
void UB2PreRenderer::ConditionalRestoreFromPreRenderLightEnvForDynPL(bool bRestoreAll)
{
	//// 플레이어 캐릭터 light 돌려놓음. 이건 RestoreAll 을 줄 때에만, 즉, RestoreAll 을 주지 않으면 더미만 돌려놓는다.
	//if (bLocalPlayerHeadLightModified && bRestoreAll)
	//{
	//	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(OwnerGameMode));
	//	if (LocalB2Player && LocalB2Player->LightComponent)
	//	{
	//		LocalB2Player->LightComponent->SetIntensity(LastCachedLocalPCLightIntensity);
	//		LocalB2Player->LightComponent->SetAttenuationRadius(LastCachedLocalPCLightRadius);

	//		// 정식 루트로 켜져 있는 경우는 켜진 채 놔두고 아니면 끔
	//		if (!LocalB2Player->IsHeadPointLightAllowed() || !LocalB2Player->IsHeadPointLightFormallyOn())
	//		{
	//			LocalB2Player->FormalSetPointLightOn(false);
	//		}
	//	}
	//	bLocalPlayerHeadLightModified = false;
	//}
	//// 추가 더미도 끄고
	//if (bLightEnvironmentSetupForDynPointLight)
	//{		
	//	if (PreRenderDummyPointLight && PreRenderDummyPointLight->GetLightComponent())
	//	{
	//		PreRenderDummyPointLight->GetLightComponent()->bAffectsWorld = false;
	//		PreRenderDummyPointLight->GetLightComponent()->UnregisterComponent();
	//		PreRenderDummyPointLight->GetLightComponent()->Deactivate();
	//		PreRenderDummyPointLight->GetLightComponent()->MarkRenderStateDirty();
	//		PreRenderDummyPointLight->Destroy();
	//		PreRenderDummyPointLight = nullptr;
	//	}

	//	bLightEnvironmentSetupForDynPointLight = false;
	//}
}

void UB2PreRenderer::MinimizePlayerPointLight()
{
	//// PointLight 환경을 사실상 0 개로 만들고자 할 때 쓴다. 
	//// ConditionalRestoreFromPreRenderLightEnvForDynPL 만 가지고는 플레이어 캐릭터 라이트가 완전히 꺼지지 않고 플레이어 캐릭터로부터 일정 반경 안에서는 여전히 한 개의 PointLight 가 쓰이게 된다.
	//// ConditionalSetupPreRenderLightEnvForDynPL 을 통해 이미 넓은 반경으로 셋업이 된 상황에서만 사용하도록 한다.
	//// 그래야 ConditionalRestoreFromPreRenderLightEnvForDynPL 에서 원래대로 돌아갈 것이다.
	//if (bLocalPlayerHeadLightModified && LastCachedLocalPCLightRadius > 0.0f)
	//{
	//	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(OwnerGameMode));
	//	if (LocalB2Player && LocalB2Player->LightComponent && LocalB2Player->LightComponent->bAffectsWorld)
	//	{
	//		// LastCachedLocalPCLightRadius 에 이미 정상적인 상황에서의 값이 복사된 것으로 간주하고 여기서 값을 캐싱하지 않는다.
	//		// 이게 쓰이는 상황에서는 AttenuationRadius 가 아마 매우 크게 세팅된 상황일 것이다.
	//		LocalB2Player->LightComponent->SetAttenuationRadius(0.01f);
	//	}
	//}
}

void UB2PreRenderer::ConditionalSetupPreRenderLightEnvForDirL(EComponentMobility::Type LightMobility, bool bForceSetup, bool bSetupForCSM)
{
	// DirectionalLight 환경을 바꾸는 건 사실상 PreRender 전용 게임모드 아니면 다루기 까다로울 것 같다. 
	// 적어도 지금 구현으로는..
	check(Cast<AB2PreRenderGameMode>(OwnerGameMode));

	if (!bLightEnvironmentSetupForDynDirLight || bForceSetup)
	{
		// 일단 레벨에 배치된 게 있는지부터 찾아본다.
		PreRenderDirLight = FindDirectionalLighOfMobility(LightMobility);
		// 배치된 게 없으면 하나 만들어.
		if (!PreRenderDirLight)
		{
			UWorld* TheWorld = OwnerGameMode ? OwnerGameMode->GetWorld() : GetWorld();
			if (TheWorld)
			{
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnInfo.ObjectFlags |= RF_Transient;
				// 방향광이니 위치야 별 상관없고.. 사실 방향도 어떻게든 빛을 받기만 하면 되는데 그래도 가능한 위에서 아래로 해 보자
				FTransform SpawnTransform(FRotator(-90.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f));
				PreRenderDirLight = TheWorld->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), SpawnTransform, SpawnInfo);
				if (PreRenderDirLight)
				{
					PreRenderDirLight->SetMobility(LightMobility);
					UDirectionalLightComponent* DLComp = Cast<UDirectionalLightComponent>(PreRenderDirLight->GetLightComponent());
					if (DLComp)
					{
						DLComp->SetIntensity(1000000.0f);
					}
				}
			}
		}

		if (PreRenderDirLight)
		{
			UDirectionalLightComponent* DLComp = Cast<UDirectionalLightComponent>(PreRenderDirLight->GetLightComponent());
			if (DLComp)
			{
				DLComp->bAffectsWorld = true;

				if (bSetupForCSM && LightMobility != EComponentMobility::Static)
				{ // 별도의 셰이더 컴파일이 필요한 CascadedShadowMap 용 셋업
					DLComp->bCastModulatedShadows = false;
					if (LightMobility == EComponentMobility::Stationary)
					{
						DLComp->DynamicShadowDistanceStationaryLight = 20000.0f;
					}
					else if (LightMobility == EComponentMobility::Movable)
					{
						DLComp->DynamicShadowDistanceMovableLight = 20000.0f;
					}
					DLComp->bUseInsetShadowsForMovableObjects = false;
				}
				else
				{ // 아닌 경우 Modulated shadow ..
					DLComp->bCastModulatedShadows = true;
					DLComp->DynamicShadowDistanceStationaryLight = 0.0f;
					DLComp->DynamicShadowDistanceMovableLight = 0.0f;
				}
				DLComp->UnregisterComponent(); // 사용 환경에 따라 한 차례 unregister 를 한 담에 register 를 해야 Scene 에 제대로 add 되는 경우가 있음. 그래서 바보 짓.

				DLComp->Activate();
				DLComp->RegisterComponent();
				DLComp->MarkRenderStateDirty();
			}

			bLightEnvironmentSetupForDynDirLight = true;
		}
	}
}
void UB2PreRenderer::ConditionalRestoreFromPreRenderLightEnvForDirL()
{
	// DirectionalLight 환경을 바꾸는 건 사실상 PreRender 전용 게임모드 아니면 다루기 까다로울 것 같다. 
	// 적어도 지금 구현으로는..
	check(Cast<AB2PreRenderGameMode>(OwnerGameMode));

	if (bLightEnvironmentSetupForDynDirLight)
	{
		if (PreRenderDirLight && PreRenderDirLight->GetLightComponent())
		{
			PreRenderDirLight->GetLightComponent()->bAffectsWorld = false;
			// 레벨에 설치된 것일 수도 있으니 Destroy 는 하지 말고.. 그러나 UnregisterComponent 는 해 주어야 바로 다른 directional light 를 켤 때 렌더쓰레드 타이밍 상 문제가 적어진다.
			PreRenderDirLight->GetLightComponent()->UnregisterComponent();
			PreRenderDirLight->GetLightComponent()->Deactivate();
			PreRenderDirLight->GetLightComponent()->MarkRenderStateDirty();

			PreRenderDirLight = nullptr;
		}

		bLightEnvironmentSetupForDynDirLight = false;
	}
}
ADirectionalLight* UB2PreRenderer::FindDirectionalLighOfMobility(EComponentMobility::Type LightMobility)
{
	// 월드에 설치된 DirectionalLight 를 찾는 거. DirectionalLight 가 필요한 라이팅 패스에서 설치된 것 부터 찾아보고 없으면 spawn 시키려고

	UWorld* TheWorld = OwnerGameMode ? OwnerGameMode->GetWorld() : GetWorld();
	for (FActorIterator ActorIt(TheWorld); ActorIt; ++ActorIt)
	{
		ADirectionalLight* ThisDirL = Cast<ADirectionalLight>(*ActorIt);
		if (ThisDirL && ThisDirL->GetLightComponent() && ThisDirL->GetLightComponent()->Mobility == LightMobility)
		{
			return ThisDirL;
		}
	}
	return nullptr;
}

void UB2PreRenderer::CompletelyTurnOffAllLights()
{
	// 혹여나 이런 상태에서 pre-render 가 필요할지도..
	for (TObjectIterator<ULightComponent> It; It; ++It)
	{
		ULightComponent* ThisLightComp = *It;
		if (ThisLightComp)
		{
			ThisLightComp->bAffectsWorld = false;
			ThisLightComp->MarkRenderStateDirty();
		}
	}
}

ASkeletalMeshActor* UB2PreRenderer::SpawnPreRenderMeshCommon(UWorld* InSpawnWorld)
{
	//if (InSpawnWorld)
	//{
	//	FActorSpawnParameters SpawnInfo;
	//	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//	SpawnInfo.ObjectFlags |= RF_Transient;

	//	FTransform SpawnTransform;
	//	SpawnTransform.SetLocation(GetPreRenderDummyDesiredLocation(InSpawnWorld));

	//	ASkeletalMeshActor* SpawnedSKActor = InSpawnWorld->SpawnActor<ASkeletalMeshActor>(ASkeletalMeshActor::StaticClass(), SpawnTransform, SpawnInfo);
	//	if (SpawnedSKActor) { // 렌더링만 할 꺼고 겹친 자리에 다른 것들도 줄줄이 spawn 할 꺼라 collision 을 확실히 끄도록.
	//		SpawnedSKActor->SetActorEnableCollision(false);
	//		// Pre-render 가 확실하게 되도록 하기 위해 렌더쓰레드에서 컬링되지 않도록 설정. 추가 셰이더 타입에 대한 처리도 함.
	//		SpawnedSKActor->GetSkeletalMeshComponent()->SetExcessiveRenderProcessing(true);
	//	}
	//	return SpawnedSKActor;
	//}
	return NULL;
}


FVector UB2PreRenderer::GetPreRenderDummyDesiredLocation(UObject* WorldContextObject)
{
	//FVector ViewCenterWorldPos = UB2UnitedWidgetBase::GetWorldPosBeneathScreen(UGameplayStatics::GetLocalPlayerController(WorldContextObject), FVector2D(0.5f, 0.5f), PreRenderSpawnDistFromView);
	//// 사실 이는 멀리 떨어뜨려 놓고 화면 가림막을 하지 않으려는 거였는데 어차피 Pre-render 가 미친듯이 성능을 잡아먹을 꺼고 TimeScale 도 작게 줄거니 그런 의미는 별로 없을 듯.
	//return bPreRenderAtFarAway ? (ViewCenterWorldPos + FVector(0.0f, 0.0f, -10000.0f)) : ViewCenterWorldPos;
	return FVector(0.0f, 0.0f, -10000.0f);
}

UParticleSystemComponent* UB2PreRenderer::SpawnDummyPSCForPreRender(UParticleSystem* InPS, USceneComponent* InRootComp)
{
	if (!InPS || !InRootComp)
	{
		return NULL;
	}

	//// UGameplayStatics::SpawnEmitterAttached 쓰니까 뭔가 즉석에서 SceneProxy 가 생기지 않아서 좀 다른 방식으로 spawn 시킴
	//// 그렇다고 일반적인 게임코드에서 UGameplayStatics::SpawnEmitterAttached 를 쓰는 게 문제를 일으키지는 않고, 단지 여기선 바로 SceneProxy 에 뭔가 신호를 날리려고

	//UParticleSystemComponent* CreatedComp = NewObject<UParticleSystemComponent>(InRootComp->GetOwner(), NAME_None, RF_Transient);
	//if (CreatedComp)
	//{
	//	CreatedComp->SetTemplate(InPS);
	//	CreatedComp->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	//	CreatedComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//	CreatedComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	//	CreatedComp->AttachToComponent(InRootComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	//	CreatedComp->RegisterComponent();
	//	CreatedComp->Activate(true);

	//	// Pre-render 목적 상 delay 최대한 없이 빨리 렌더링 되도록 한다.
	//	// 파티클 시스템 원본 값은 건드리지 못하니 최대한 이렇게라도..
	//	CreatedComp->EmitterDelay = 0.0f;
	//	for (FParticleEmitterInstance* EMI : CreatedComp->EmitterInstances)
	//	{
	//		if (EMI){
	//			EMI->SetupEmitterDuration();
	//			EMI->CurrentDelay = 0.0f;
	//		}
	//	}

	//	CreatedComp->SetExcessiveRenderProcessing(true); // 렌더쓰레드 InitView 에서의 SceneVisibility 를 보장.

	//	return CreatedComp;
	//}
	return NULL;
}

FB2GMPreRenderDummyInfo& UB2PreRenderer::GetExtraPrerenderDummyInfo()
{
	// PCClass 나 NPCClass 단위로 pre-render 를 할 때에는 해당 클래스에 기반한 pre-render dummy Actor 를 만들어서 필요한 컴포넌트들을 거기에 붙이지만
	// 그렇지 않은 경우는 미리 만들어진 것이 있으면 그걸 사용하던지 아니면 더미를 하나 spawn 시킨다.	
	if (AllPreRenderDummy.Num() == 0)
	{
		GetAdditionalPrerenderDummyInfo();
	}
	check(AllPreRenderDummy.Num() > 0);
	return AllPreRenderDummy[0];
}
ASkeletalMeshActor* UB2PreRenderer::GetExtraPrerenderDummySKActor()
{
	FB2GMPreRenderDummyInfo& ExtraDummyInfo = GetExtraPrerenderDummyInfo();
	check(ExtraDummyInfo.PreRenderDummy);
	return ExtraDummyInfo.PreRenderDummy;
}
FB2GMPreRenderDummyInfo& UB2PreRenderer::GetAdditionalPrerenderDummyInfo()
{
	FB2GMPreRenderDummyInfo NewExtraDummyInfo;
	// OwnerGameMode 의 GetWorld 가 더 확실한 듯 한데..
	NewExtraDummyInfo.PreRenderDummy = SpawnPreRenderMeshCommon(OwnerGameMode ? OwnerGameMode->GetWorld() : GetWorld());
	// Dummy 의 경우 클래스는 가장 기본적인 해골바가지 병사로 ㅋ
	SetupDummyMobForPreRender(ENPCClass::ENC_Mob_Skell_Sword, ENPCClassVariation::ENCV_Normal, NULL, NewExtraDummyInfo.PreRenderDummy);
	int32 AddedIndex = AllPreRenderDummy.Add(NewExtraDummyInfo);
	return AllPreRenderDummy[AddedIndex];
}
ASkeletalMeshActor* UB2PreRenderer::GetAdditionalPrerenderDummySKActor()
{
	FB2GMPreRenderDummyInfo& AdditionalDummyInfo = GetAdditionalPrerenderDummyInfo();
	check(AdditionalDummyInfo.PreRenderDummy);
	return AdditionalDummyInfo.PreRenderDummy;
}

void UB2PreRenderer::SetupDummyParticleSystemsForPreRender(ASkeletalMeshActor* InDummySKActor)
{
	//if (!InDummySKActor) {
	//	return;
	//}
	//// AnimBP 랑 AnimNotify 통해서 ParticleSystem 들을 최대한 긁어모은다.
	//TMap<FName, UParticleSystem*> GatheredPS;
	//TryGatherAllBoundPSFromSKComp(InDummySKActor->GetSkeletalMeshComponent(), GatheredPS);

	//for (TMap<FName, UParticleSystem*>::TConstIterator PSIt(GatheredPS); PSIt; ++PSIt)
	//{
	//	UParticleSystem* ThisPS = PSIt.Value();
	//	// DummySKActor 에 attach 시키고 이후는 DummySKActor를 컨트롤
	//	UParticleSystemComponent* ThisSpawnedFxComp = SpawnDummyPSCForPreRender(ThisPS, InDummySKActor->GetRootComponent());
	//}
}

void UB2PreRenderer::SetupDummyMobForPreRender(ENPCClass InNPCClass, ENPCClassVariation InVari, ABladeIICharacter* InOwnerChar, ASkeletalMeshActor* InDummySKActorForRender)
{
	if (InNPCClass == ENPCClass::ENC_End || !InDummySKActorForRender || !InDummySKActorForRender->GetSkeletalMeshComponent())
	{
		return;
	}

	UB2NPCClassInfoBox* MobInfoBox = StaticFindMobClassInfoBox(OwnerGameMode);
	// MobClassInfo 의 Async 로딩을 백그라운드로 사용하고 싶은 상황이라면 이것이 무효화 될 듯. 그냥 Async 로딩의 멀티코어 활용만 사용하겠다면 모르겠지만.
	UB2NPCSingleClassInfo* ThisClassInfo = MobInfoBox ? MobInfoBox->GetNPCSingleClassInfo(InNPCClass, InVari) : nullptr;
	if (!ThisClassInfo)
	{
		return;
	}

	// ClassInfo 에서 Dummy pre-render 에 필수적인 정보만 세팅. Attach 하는 리소스 설정들도 있지만 그닥 많은 건 아님..
	InDummySKActorForRender->GetSkeletalMeshComponent()->SetSkeletalMesh(ThisClassInfo->MeshAssetOverride);
	if (ThisClassInfo->AnimBPOverride)
	{
		InDummySKActorForRender->GetSkeletalMeshComponent()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		InDummySKActorForRender->GetSkeletalMeshComponent()->SetAnimInstanceClass(ThisClassInfo->AnimBPOverride);
	}
}

void UB2PreRenderer::SetupDummyPCMeshForPreRender(EPCClass InCharClass, ABladeIIPlayer* InOwnerPlayer, ASkeletalMeshActor* InDummySKActorForRender, const TArray<FB2Item>& InEquippedItems, const FB2Wing* InWingData)
{
	if (InCharClass == EPCClass::EPC_End || !InDummySKActorForRender || !InDummySKActorForRender->GetSkeletalMeshComponent())
	{
		return;
	}

	//// PCClassInfo 의 Async 로딩을 백그라운드로 사용하고 싶은 상황이라면 이것이 무효화 될 듯. 그냥 Async 로딩의 멀티코어 활용만 사용하겠다면 모르겠지만.
	//UB2PCClassInfoBox* PCBox = StaticFindPCClassInfoBox(OwnerGameMode);
	//UB2PCClassInfo* ThisClassInfo = PCBox ? PCBox->GetSingleClassInfo(InCharClass) : NULL;
	//if (!ThisClassInfo) {
	//	return;
	//}

	//USkeletalMesh* PrebuiltMeshOrResult = NULL;
	//{ // PreRender dummy 용으로는 아마도 InOwnerPlayer 가 안 들어올 거라 네트워크 모드 용으로 기능 확장 시 엉뚱한 모델을 가져오게 될 수 있다.
	//  //PrebuiltMeshOrResult = OwnerGameMode->GetCachedCompositeMesh(InCharClass, InOwnerPlayer, InEquippedItems, (InWingData && InWingData->bShouldBeVisible), InWingData, false);
	//}

	//// 로비에서는 InGameOnlyInfo 를 꺼내오지 않도록 한다. 쓸데없는 메모리 낭비.
	//UB2InGameOnlyPCClassInfo* InGameOnlyInfo = (GetB2GameModeType(this) != EB2GameMode::Lobby) ? ThisClassInfo->GetInGameOnlyInfo(this) : NULL;

	//if (UB2PCClassInfo::SetupSKCompForParts(InCharClass, InDummySKActorForRender, InDummySKActorForRender->GetSkeletalMeshComponent(), PrebuiltMeshOrResult,
	//	ThisClassInfo->BaseMeshAsset, ThisClassInfo->DefaultPartsInfo, InEquippedItems, InWingData, (InGameOnlyInfo ? InGameOnlyInfo->AnimBPOverride : NULL), 
	//	false, false))
	//{
	//	{// PreRender dummy 용으로는 아마도 InOwnerPlayer 가 안 들어올 거라 네트워크 모드 용으로 기능 확장 시 엉뚱한 모델을 캐싱하게 될 수 있다.
	//	 //OwnerGameMode->SetCachedCompositeMesh(InCharClass, InOwnerPlayer, PrebuiltMeshOrResult, InEquippedItems, (InWingData && InWingData->bShouldBeVisible), InWingData, false);
	//	}
	//}
}

void UB2PreRenderer::UpdateForPreRenderObjects(bool bForceContinue /*= false*/) // Pre-render 진행 도중 Tick 에 해당
{
	checkSlow(OwnerGameMode && OwnerGameMode->IsInPreRenderPhase());
	//
	//	if (bPendingForPreRenderGM)
	//	{
	//		if (StartBladeIIPreRenderGame(OwnerGameMode)) // 로비에서 직접 pre-render 하는 대신 전용 레벨 로딩하는 경우
	//		{
	//			bPendingForPreRenderGM = false;
	//			return;
	//		}
	//	}
	//
	//	if (bCanDoOverallCount)
	//	{
	//		++OverallTickCount;
	//	}
	//	// 이게 true 면 설령 렌더링 되지 않은 게 있더라도 제거. 모두 다 렌더링이 된다는 보장이 없으니.. 
	//	const bool bOverallCountDone = (OverallTickCount >= OverallPreRenderLimit);
	//
	//	// 아무 조건 없이 셋업 이후로 특정 틱 지나면 Pre-render 를 강제 종료. 이건 최후의 safety 를 위해.
	//	++TickCountFromSetup;
	//	// 실제로 여기에 걸리는 건 바람직한 상황은 아니다.
	//	const bool bHardLimitReached = (TickCountFromSetup >= PreRenderHardLimit);
	//
	//	const bool bShouldFinishDummiesBySomeLimit = (bOverallCountDone || bHardLimitReached) && !bForceContinue;
	//
	//	const FVector DummyDesiredLocation = GetPreRenderDummyDesiredLocation(OwnerGameMode); // 현재 view 에서 가져오게 한다면 매번 바뀔 것임.
	//
	//#if !UE_BUILD_SHIPPING
	//	int32 FinishBySomeLimitCount = 0;
	//	FString FinishBySomeLimitMsg = TEXT("");
	//#endif
	//
	//	// PreRender 된 거 틱 돌면서 꺼 줌.
	//	for (int32 PRDI = 0; PRDI < AllPreRenderDummy.Num(); ++PRDI)
	//	{
	//		FB2GMPreRenderDummyInfo& ThisDummyInfo = AllPreRenderDummy[PRDI];
	//		// 대체로 이 SkeletalMeshComponent 기준으로 보긴 할껀데 이것 뿐만이 아니라 여기에 이펙트들이 잔뜩 붙어 있을 수 있다.
	//		USkeletalMeshComponent* ThisDummySKComp = ThisDummyInfo.PreRenderDummy->GetSkeletalMeshComponent();
	//
	//		// 일정 카운트 이상 렌더링 되면 끈다.. 
	//		if (ThisDummyInfo.PreRenderDummy && !ThisDummyInfo.PreRenderDummy->bHidden)
	//		{
	//			if (ThisDummySKComp->LastRenderTime > 0.0f)
	//			{
	//				bCanDoOverallCount = true; // OverallCount 는 적어도 하나라도 렌더링 된 이후에 시작한다. 
	//			}
	//
	//			const bool bThisDummyDoneForNormalCondition = ThisDummyInfo.UpdateDummy(bForceContinue);
	//
	//			if ((bThisDummyDoneForNormalCondition || bShouldFinishDummiesBySomeLimit) && !bForceContinue)
	//			{
	//#if !UE_BUILD_SHIPPING // 실제 렌더링 신호는 안 왔는데 그냥 제한이 차서 끝내는 건 로깅을 함
	//				if (!bThisDummyDoneForNormalCondition)
	//				{
	//					++FinishBySomeLimitCount;
	//					// PC 용으로 조합한 건 딱히 의미있는 이름은 안 나오겠지만..
	//					FinishBySomeLimitMsg += FString::Printf(TEXT("%d. Root SkeletalMesh : %s\r\n"), FinishBySomeLimitCount, ThisDummySKComp->SkeletalMesh ? (*ThisDummySKComp->SkeletalMesh->GetName()) : TEXT("NULL"));
	//				}
	//#endif
	//
	//				ThisDummyInfo.PreRenderDummy->SetActorHiddenInGame(true);
	//				ThisDummySKComp->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	//				// 파티클 시스템도 붙은 이상 파괴해 주는 게 좋겠다. 배열도 비우고.. 그래야 다 돌아갔는지 확인하기도 좋고.
	//				ThisDummyInfo.PreRenderDummy->Destroy();
	//				ThisDummyInfo.PreRenderDummy = NULL;
	//				AllPreRenderDummy.RemoveAt(PRDI);
	//				--PRDI;
	//			}
	//			else
	//			{ // 아직 렌더링이 완전히 안되었거나 강제로 계속하거나.. 카메라 위치가 급격히 바뀌고 할 수 있으니 다시금 location 셋업.
	//				ThisDummyInfo.PreRenderDummy->SetActorLocation(DummyDesiredLocation);
	//			}
	//		}
	//	}
	//
	//	if (OwnerGameMode->GetPreRenderPhase() == EPreRenderPhase::EPRP_Selected && ProgressStepCountForSelectedPhase < MaxPreRenderCount)
	//	{ // 일반 PreRender Selected phase 의 진행도 증가.
	//		// 기본 Tick 제한만큼만 여기서 카운트 한다. 정확하지는 않지만 그렇게 정확할 필요까지도 없음.
	//		StepPreRenderScreenProgress();
	//		++ProgressStepCountForSelectedPhase;
	//	}
	//
	//	FlushRenderingCommands(); // 도움이 될지도.
	//
	//	if (bShouldFinishDummiesBySomeLimit)
	//	{
	//		AllPreRenderDummy.Empty(); // 확인사살
	//
	//#if !UE_BUILD_SHIPPING
	//		FinishBySomeLimitMsg = FString::Printf(TEXT("[PreRender] %d Pre-render info are done by some limitation while not all rendered.\r\n"), FinishBySomeLimitCount) + FinishBySomeLimitMsg;
	//		UE_LOG(LogBladeII, Display, TEXT("%s"), *FinishBySomeLimitMsg);
	//#endif
	//	}
}

void UB2PreRenderer::SetupDefaultPreRenderScreen()
{
	// 특정 타이밍의 로딩 스크린 기능이었는데 이런 식으로 재활용하는군 ㅋ
	//if (OwnerGameMode)
	//{
	//	check(!Cast<AB2PreRenderGameMode>(OwnerGameMode)); // 일반 게임모드 용. PreRender 전용 게임모드는 나름의 방식으로 가림막 구성
	//	OwnerGameMode->OpenPreLoadingScreen(EPreLoadingScreenType::PLST_PreRender_Default);
	//	CachedPreLoadingScreen = OwnerGameMode->GetCurrCreatedPreLoadingScreen(EPreLoadingScreenType::PLST_PreRender_Default);
	//	check(CachedPreLoadingScreen);
	//	if (CachedPreLoadingScreen)
	//	{
	//		// 일반 PreRender 에서의 진행도 셋업.
	//		CachedPreLoadingScreen->SetupPreRenderProgress(false, false, true);

	//		EstimatedTotalProgressStep = 0;
	//		// Selected phase 에 대한 처리 리스트가 있을 경우 기본 틱 제한만큼 스텝
	//		if (AllPreRenderDummy.Num() > 0)
	//		{
	//			EstimatedTotalProgressStep += MaxPreRenderCount;
	//		}
	//		// AllPrim Phase 틱 제한만큼의 스텝 추가.
	//		if (bPreRenderAllPrimitives && !IsCurrentWorldLevelPreRendered())
	//		{
	//			EstimatedTotalProgressStep += GetTotalPreRenderAllPrimTickLimit();
	//		}

	//		CurrPrerenderProgressStep = 0;
	//		ProgressStepCountForSelectedPhase = 0;
	//		// 맨 끝까지 가기 전에 1.0 진행도를 채우기 위해 스텝 하나는 뺌.
	//		EstimatedTotalProgressStep = FMath::Max(EstimatedTotalProgressStep - 1, 0);
	//	}
	//}
}

void UB2PreRenderer::StepPreRenderScreenProgress()
{
	check(OwnerGameMode && !Cast<AB2PreRenderGameMode>(OwnerGameMode));

	//if (CachedPreLoadingScreen && EstimatedTotalProgressStep > 0)
	//{
	//	// 일반 맵로딩 progress 나 PreRenderGameMode progress 보다는 단순하게 이거 콜 할때마다 한스텝 씩 증가.
	//	CurrPrerenderProgressStep = FMath::Min(CurrPrerenderProgressStep + 1, EstimatedTotalProgressStep);
	//	// 여러 프레임 부드럽게 흘러가지는 않을 것이므로 한번 갱신할 때 바로 타겟 값을 채우도록 한다.
	//	CachedPreLoadingScreen->UpdatePreRenderProgressScale((float)CurrPrerenderProgressStep / (float)EstimatedTotalProgressStep, true);
	//}
}

bool UB2PreRenderer::ShouldRenderForAllPrimPhase(UPrimitiveComponent* InCompToCheck) const
{
	// PreRenderAll 에서는 월드에 떠 있는 모든 Primitive 컴포넌트들을 가지고 렌더링을 돌리는데 이것까진 넘 낭비일 수 있어서 
	// 여기선 실제로 중요한 material 차원에서의 체크를 좀 한다.
	if (MaterialBasedCompleteState.ArePrimCompAllMaterialsPreRendered(InCompToCheck))
	{
		return false;
	}

	// 아직 타입 필터링이 완전하지가 않아서..
	return true; // Cast<UMeshComponent>(InCompToCheck) || Cast<UParticleSystemComponent>(InCompToCheck);
}

bool UB2PreRenderer::SetupPreRenderAllPrimPhase()
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2PreRenderer::SetupPreRenderAllPrimPhase"));

	checkSlow(OwnerGameMode && OwnerGameMode->GetPreRenderPhase() == EPreRenderPhase::EPRP_Selected);

	PreRenderAllPrimVisbilityCache.Empty();
	PreRenderAllActorVisbilityCache.Empty();
	PreRenderAllPLStateCache.Empty();

	// 사실 이 기능은 자체로 위험성이 있는데 여기서 세팅하고 종료할 때까지 게임 코드에 의해 특정 액터나 컴포넌트의 visibility 값이 변하게 되면 그게 무용지물이 될 것이기 때문.
	// 대체로 이게 실행되는 동안은 시작 연출이 진행중일 거라 마티니 트랙에 의한 조절 외에는 그럴 일이 없을 것이고 Time Delta 도 매우 작게 스케일링 하여 가능성이 상당히 낮긴 할 것.
//
//	if (!IsCurrentWorldLevelPreRendered()
//#if PLATFORM_IOS
//		&& ShouldDoPreRenderForIOS_AllPrim()
//#endif
//		)
//	{
//		for (TObjectIterator<UPrimitiveComponent> It; It; ++It)
//		{
//			UPrimitiveComponent* ThisPrimComp = *It;
//			if (ThisPrimComp && ShouldRenderForAllPrimPhase(ThisPrimComp))
//			{
//				// 상태 변수가 두 개가 있는데 자칫 잘못 다루면 실제 보이는 상황을 헛짚을 수 있음. 그래서 뭔가 많이 저장.
//				FPreRenderPrimCompVisibilityCache ThisPrimVisibleState;
//				ThisPrimVisibleState.bTotalVisible = ThisPrimComp->IsVisible();
//				ThisPrimVisibleState.bVisible = ThisPrimComp->bVisible;
//				ThisPrimVisibleState.bHiddenInGame = ThisPrimComp->bHiddenInGame;
//				PreRenderAllPrimVisbilityCache.Add(ThisPrimComp, ThisPrimVisibleState);
//
//				ThisPrimComp->SetVisibility(true);
//				ThisPrimComp->SetHiddenInGame(false);
//				ThisPrimComp->SetExcessiveRenderProcessing(true);
//
//				// 앞으로 몇 프레임 돌려야 완료가 되긴 할 껀데 이쯤서 마크 ㅋ
//				MaterialBasedCompleteState.MarkPrimCompMaterials(ThisPrimComp);
//			}
//		}
//
//		// Dynamic(Movable)PointLight 를 별도로 제어하긴 하지만
//		// 이쯤이면 첫 연출 시작하면서 별도로 켜진 PointLight 가 있을 것. 그것도 끈다. 상태는 보존하고..
//		for (TObjectIterator<UPointLightComponent> It; It; ++It)
//		{
//			UPointLightComponent* ThisPL = *It;
//			if (ThisPL && ThisPL->Mobility == EComponentMobility::Movable && ThisPL->bAffectsWorld)
//			{
//				AActor* PLOwnerActor = ThisPL->GetOwner();
//				ABladeIIPlayer* PlayerOwner = Cast<ABladeIIPlayer>(PLOwnerActor);
//				APointLight* PLActorOwner = Cast<APointLight>(PLOwnerActor);
//								
//				if (
//					PLOwnerActor && 
//					!PlayerOwner &&  // PlayerOwner 가 있으면 별도로 관리하는 플레이어 상단에 붙는 라이트이니 제외
//					!(PLActorOwner && PLActorOwner == PreRenderDummyPointLight) // 이 경우도 여기서 특별히 관리하는 것이니 제외
//					)
//				{
//					// 이거 끝나면 돌려놔야 된다는 걸 기록해 놓고 잠시 꺼둔다.
//					PreRenderAllPLStateCache.Add(ThisPL, true);
//					ThisPL->bAffectsWorld = false;
//					ThisPL->MarkRenderStateDirty();
//				}
//
//			}
//		}
//
//		// Owner Actor 가 있는 PrimitiveComponent 들은 Actor 를 켜주지 않으면 소용이 없는 듯. 따로 처리.
//		UWorld* TheWorld = OwnerGameMode ? OwnerGameMode->GetWorld() : nullptr;
//		if (TheWorld)
//		{
//			for (FActorIterator ActorIt(TheWorld); ActorIt; ++ActorIt)
//			{
//				AActor* ThisActor = Cast<AActor>(*ActorIt);
//				if (ThisActor)
//				{
//					bool bIsVisible = !(ThisActor->bHidden);
//					PreRenderAllActorVisbilityCache.Add(ThisActor, bIsVisible);
//					ThisActor->SetActorHiddenInGame(false);
//				}
//			}
//		}
//
//		bHadDoneAnyPreRender = true; // 아직 실제로 한 건 아니지만 이 플래그 목적 상 이쯤에서 true 마크해도 좋겠다.
//	}
//
//	FlushRenderingCommands();
//	PreRenderAllPrimTickCount = 0;

	return (PreRenderAllPrimVisbilityCache.Num() > 0 || PreRenderAllActorVisbilityCache.Num() > 0);
}

bool UB2PreRenderer::CheckAndFinishPreRenderAllPrimPhase()
{
	B2_SCOPED_TRACK_LOG(TEXT("UB2PreRenderer::FinishPreRenderAllPrimPhase"));

	checkSlow(OwnerGameMode && OwnerGameMode->GetPreRenderPhase() == EPreRenderPhase::EPRP_AllPrim);

	//PreRenderAllPrimTickCount++;

	//StepPreRenderScreenProgress(); // Pre-render 진행도.

	//// Pre-render 도중 돌아가는 연출을 강제 진행 하는데 
	//// 원래 튜토리얼 첫 연출에서 카메라워크가 진행되면서 셰이더 컴파일 끊김이 심해서 이런 짓을 하게 되었지만
	//// 상당히 효과적이었어서 보다 일반적으로 적용해 봄.
	//if (OwnerGameMode && GetTotalPreRenderAllPrimTickLimit() > 0)
	//{
	//	AB2StageEventDirector* CurrPlayingEvent = OwnerGameMode->GetCurrentPlayingStageEventDirector();
	//	if (CurrPlayingEvent)
	//	{ // 화면을 가리고 플레이하므로 렌더링은 문제될 거 없지만 사운드가 플레이 될 수 있는데
	//		// PreRender 중에는 사운드 트랙 꺼 놓음 (TemporaryDisableTracksForPreRender)
	//		CurrPlayingEvent->ForceSetMatineePlayingPosScale(
	//			(float)PreRenderAllPrimTickCount / (float)(GetTotalPreRenderAllPrimTickLimit()));
	//	}
	//}

	//if (PreRenderAllPrimTickCount < GetTotalPreRenderAllPrimTickLimit())
	//{
	//	// 이건 LastRenderTime 같은 거 안 보고 딱 정해진 틱 수만 돈다. 단, 이게 돌아가는 동안 액터들의 Visibility 가 변하지 않는다는 가정이 있으므로 너무 많이 돌아선 안 됨.
	//	// LastRenderTime 으로 실제 렌더링이 되었는지 보려니 실제 렌더링이 들어가지 않는 엉뚱한 컴포넌트들이 체크됨.

	//	// 설정에 따라 라이트 환경 바꿔서도 돌린다.
	//	if (PreRenderAllPrimTickCount >= PreRenderAllPrimLit2PassTickLimit + PreRenderAllPrimLit1PassTickLimit)
	//	{// 둘 다 끔.
	//		ConditionalRestoreFromPreRenderLightEnvForDynPL(false);
	//		// 플레이어 캐릭터 Head Light 의 경우는 반경을 극히 좁게 잡아서 사실상 꺼지도록 한다. 이렇게 해도 연출에서 별도의 PointLight 를 사용한다면 PointLight 가 하나는 있겠지만..
	//		MinimizePlayerPointLight();
	//	}
	//	else if (PreRenderAllPrimTickCount >= PreRenderAllPrimLit2PassTickLimit)
	//	{
	//		// 2 개의 dynamic point light 중 하나만 끔
	//		ConditionalRestoreFromPreRenderLightEnvForDynPL(false);
	//	}
	//	return false; 
	//}

	//// 모두 보이도록 했던 visibility 를 원래 값으로 복구시켜준다.

	//for (TObjectIterator<UPrimitiveComponent> It; It; ++It)
	//{
	//	UPrimitiveComponent* ThisPrimComp = *It;
	//	if (ThisPrimComp)
	//	{
	//		// 상태 변수가 두 개가 있는데 자칫 잘못 다루면 실제 보이는 상황을 헛짚을 수 있음.
	//		FPreRenderPrimCompVisibilityCache *ThisPrimVisibleState = PreRenderAllPrimVisbilityCache.Find(ThisPrimComp);
	//		if (ThisPrimVisibleState)
	//		{
	//			// 뻘짓같아 보이지만 상태 변수가 두개가 있다보니 뭔가 상태가 꼬이는 듯 해서 확실히 처리하려고..
	//			if (!ThisPrimVisibleState->bTotalVisible) { // 이전에 어떤 사유로든 visible 하지 않았다고 나왔던 애들만 값을 복구..
	//				ThisPrimComp->SetVisibility(ThisPrimVisibleState->bVisible);
	//				ThisPrimComp->SetHiddenInGame(ThisPrimVisibleState->bHiddenInGame);
	//			}
	//		}
	//		ThisPrimComp->SetExcessiveRenderProcessing(false); // 이건 유지할 이유가 없으고 그냥 두면 성능에 악영향을 끼치게 되니 무조건 꺼줌.
	//	}
	//}

	//for (TMap<UPointLightComponent*, bool>::TIterator CachedPLIt(PreRenderAllPLStateCache); CachedPLIt; ++CachedPLIt)
	//{
	//	UPointLightComponent* ThisPL = CachedPLIt.Key();
	//	if (ThisPL)
	//	{
	//		ThisPL->bAffectsWorld = CachedPLIt.Value(); // 아마도 true 겠지만
	//		ThisPL->MarkRenderStateDirty();
	//	}
	//}

	//check(OwnerGameMode);
	//UWorld* TheWorld = OwnerGameMode ? OwnerGameMode->GetWorld() : nullptr;
	//if (TheWorld)
	//{
	//	for (FActorIterator ActorIt(TheWorld); ActorIt; ++ActorIt)
	//	{
	//		AActor* ThisActor = Cast<AActor>(*ActorIt);
	//		if (ThisActor && ThisActor->IsValidObj())
	//		{
	//			bool* CachedVisibilityValue = PreRenderAllActorVisbilityCache.Find(ThisActor);
	//			if (CachedVisibilityValue && !(*CachedVisibilityValue)) {
	//				ThisActor->SetActorHiddenInGame(!(*CachedVisibilityValue)); // 꺼져 있던 경우만 다시 꺼줌
	//			}
	//		}
	//	}
	//}

	//PreRenderAllPrimVisbilityCache.Empty();
	//PreRenderAllActorVisbilityCache.Empty();
	//PreRenderAllPLStateCache.Empty();

	//FlushRenderingCommands();

	return true;
}

void UB2PreRenderer::OnPreRenderComplete()
{
	ConditionalRestoreFromPreRenderLightEnvForDynPL(true); // 완전 정상으로 되돌림
	//CachedExtraPreRenderPS.Empty();
	//CachedExtraPreRenderSKMesh.Empty();
	//CachedExtraPreRenderNPCClassInfo.Empty();
	//CachedPreLoadingScreen = nullptr; // 이거 Close 는 GameMode 쪽서
	//
	//// ExcessiveRenderProcessing 리셋 확실히.. AllPrimPhase 끝날 때 리셋해주는 주는 걸로는 부족할 수 있는데 
	//// AllPrimPhase 없이 다른 pre-render 만 하는 경우도 있고, 그런 경우도 component 가 한동안 남아있을 수 있기 때문.
	//for (TObjectIterator<UPrimitiveComponent> It; It; ++It)
	//{
	//	UPrimitiveComponent* ThisPrimComp = *It;
	//	if (ThisPrimComp)
	//	{
	//		ThisPrimComp->SetExcessiveRenderProcessing(false);
	//	}
	//}
	//
	//// 이번에 실제로 pre-render 를 했다면.. 셰이더 캐쉬도 갱신이 되도록 한다.
	//if (bHadDoneAnyPreRender)
	//{
	//	EnsureSaveCaches();
	//}
}

void UB2PreRenderer::EnsureSaveCaches()
{
	//#if PLATFORM_ANDROID // Vulkan 의 경우인데.. 이게 기본적으로는 앱 종료시에만 업데이트 되도록 해 놓아서 혹여나 크래쉬라도 겪으면 PreRender 상태는 저장되지만 셰이더 캐쉬는 무용지물이 됨. 이런 안습한 사태를 방지하고자 이쯤서도 셰이더 캐쉬 업데이트.
	//	if (FAndroidMisc::ShouldUseVulkan())
	//	{
	//		const auto& OnSavePipelineCacheCallback = FAndroidMisc::GetOnSavePipelineCache();
	//		if (OnSavePipelineCacheCallback)
	//		{
	//			OnSavePipelineCacheCallback();
	//		}
	//	}
	//#endif
}

void UB2PreRenderer::MarkCurrentWorldLevelPreRendered()
{
	UWorld* CurrentWorld = OwnerGameMode ? OwnerGameMode->GetWorld() : nullptr;
	TotalCompleteState.MarkWorldLevelPreRendered(CurrentWorld);
}

bool UB2PreRenderer::IsCurrentWorldLevelPreRendered() const
{
	UWorld* CurrentWorld = OwnerGameMode ? OwnerGameMode->GetWorld() : nullptr;
	return TotalCompleteState.IsWorldLevelPreRendered(CurrentWorld);
}

void UB2PreRenderer::MarkPreRenderLevelGoneThrough()
{
	TotalCompleteState.MarkPreRenderLevelGoneThrough();
}
bool UB2PreRenderer::IsPreRenderLevelGoneThrough()
{
	return TotalCompleteState.IsPreRenderLevelGoneThrough();
}

///////////////////////////////////////////////////////////////////////////////////
// PreRenderGameMode 를 위한 인터페이스들.. 근본적인 기능은 제일 기본 SetupPreRenderObjects 에서 하는 거랑 마찬가지. 약간 다르게 사용할 뿐.

bool UB2PreRenderer::IsInPreRenderGameMode() const
{
	return OwnerGameMode && Cast<AB2PreRenderGameMode>(OwnerGameMode);
}
void UB2PreRenderer::InitForPreRenderGameMode(AB2PreRenderGameMode* InGM)
{
	OwnerGameMode = InGM;
	check(OwnerGameMode);

}
void UB2PreRenderer::SetupForPreRenderGM_PCClassPass(EPCClass InPCClass, bool bIncludeSkillAnims)
{
	checkSlow(AllPreRenderDummy.Num() == 0); // 각 패스 간에 PreRenderDummy 를 재활용해서 사용하는 식이 된다면 이 check 가 맞지 않겠지..

	// Pre-render 만을 위한 전용 모드에서 돌리는 것이므로 ForceSetup 을 준다.
	// 당장 그렇게 안한다고 해서 문제될 건 없겠지만.. 혹여나 셰이더 캐쉬가 날아간다거나 하는 상황에서 이 PreRenderGameMode 를 반복해서 돌리는 등의 여지를 생각해서 ㅋ
	ConditionalSetupPreRenderObjectForPCClass(InPCClass, true);

	if (bIncludeSkillAnims)
	{
		// 명시한 클래스의 SkillAnim 들. 각 PCClass 별 AnimBP 에 들어가 있는 전투 애니메이션들과 근본적으로 다를 것 없음. 단지 로딩되는 데이터 절감 면에서 따로 떼서 관리하는 거.
		const TArray<FCombinedPCSkillAnimID> SkillAnimsToPreLoad = ABladeIIGameMode::GetAllPCSkillAnimsOfClass(InPCClass);
		ConditionalSetupPreRenderObjectForPCSkillAnims(SkillAnimsToPreLoad, true);
	}

	PostPreRenderSeupProcess();
	if (AllPreRenderDummy.Num() > 0)
	{
		bHadDoneAnyPreRender = true; // 아직 실제로 한 건 아니지만 이 플래그 목적 상 이쯤서 true 마크
	}
}

void UB2PreRenderer::SetupForPreRenderGM_PCSkillAnimPass(EPCClass InPCClass)
{
	checkSlow(AllPreRenderDummy.Num() == 0); // 각 패스 간에 PreRenderDummy 를 재활용해서 사용하는 식이 된다면 이 check 가 맞지 않겠지..

	const TArray<FCombinedPCSkillAnimID> SkillAnimsToPreLoad = ABladeIIGameMode::GetAllPCSkillAnimsOfClass(InPCClass);
	ConditionalSetupPreRenderObjectForPCSkillAnims(SkillAnimsToPreLoad, true); // 여기도 ForceSetup

	PostPreRenderSeupProcess();
	if (AllPreRenderDummy.Num() > 0)
	{
		bHadDoneAnyPreRender = true; // 아직 실제로 한 건 아니지만 이 플래그 목적 상 이쯤서 true 마크
	}
}

void UB2PreRenderer::SetupForPreRenderGM_DamageEffectInfo()
{
	checkSlow(AllPreRenderDummy.Num() == 0); // 각 패스 간에 PreRenderDummy 를 재활용해서 사용하는 식이 된다면 이 check 가 맞지 않겠지..

	ConditionalSetupPreRenderObjectForDamageEffectInfo(true);

	PostPreRenderSeupProcess();
	if (AllPreRenderDummy.Num() > 0)
	{
		bHadDoneAnyPreRender = true; // 아직 실제로 한 건 아니지만 이 플래그 목적 상 이쯤서 true 마크
	}
}
void UB2PreRenderer::SetupForPreRenderGM_Extra(bool bForNPCClass, bool bForFx, bool bForOther)
{
	//checkSlow(AllPreRenderDummy.Num() == 0); 이거 바로 앞에 다른 PreRender 를 세팅하는 사용 사례가 있어서 걸림. 딱히 중요한 check 는 아님.

	// ExtraNPCClass 와 Other 를 Fx 보다 먼저 하는데 Fx 패스 셋업할 때의 더미가 생겨서 굳이 일부러 하나 생성할 필요가 없어지게 되기 때문.
	if (bForNPCClass)
	{
		SetupPreRenderObjects_ExtraNPCClass();
	}
	if (bForOther)
	{
		SetupPreRenderObjects_ExtraOthers();
	}
	if (bForFx)
	{
		SetupPreRenderObjects_ExtraFx();
	}

	PostPreRenderSeupProcess();
	if (AllPreRenderDummy.Num() > 0)
	{
		bHadDoneAnyPreRender = true; // 아직 실제로 한 건 아니지만 이 플래그 목적 상 이쯤서 true 마크
	}
}

#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] 
/*
 * iOS 에선 셰이더 컴파일 성능이 좋은 편이고 Pre-render 의 메모리 소모가 더 문제가 될 수 있어서 따로 조건을 건다.
 * 상황에 따라 일부만 연다거나 할 수 있다.
 */
bool UB2PreRenderer::ShouldDoPreRenderForIOS()
{ // 이게 return true 라고 해서 모든 걸 똑같이 하게 되는 건 아니다.
	return ShouldDoPreRenderForIOS_Basic() ||
		ShouldDoPreRenderForIOS_AllPrim() ||
		ShouldDoPreRenderForIOS_PreRenderGM();
}
bool UB2PreRenderer::ShouldDoPreRenderForIOS_Basic()
{
	// 이름은 Basic 이지만 ShouldDoPreRenderForIOS_PreRenderGM 이 false 이고 이게 true 이면 더욱더 큰 부담으로 똑같은 걸 처리하게 될 수 있다.
	// 사실상 명목상으로 있는 거고 다른 sub-condition 이 true 일 수는 있어도 이걸 true 로 하기는 어려울 것이다.
	return false;
}
bool UB2PreRenderer::ShouldDoPreRenderForIOS_AllPrim()
{
	return true; // 테스트를 해 보니 이거 정도는 필요할 거 같다.
}
bool UB2PreRenderer::ShouldDoPreRenderForIOS_PreRenderGM()
{
	return false;
}
#endif

///////////////////////////////////////////////////////////////////////////////////

int32 UB2PreRenderer::MaxPreRenderCount = 1; // 1보다 크게 할 이유는 거의 없다. 1 혹은 0.
int32 UB2PreRenderer::OverallPreRenderLimit = 10; // 하나라도 처음 렌더링이 된 이후에 나머지 애들 렌더링 될 때까지 기다리는 틱 수.
int32 UB2PreRenderer::PreRenderHardLimit = 20;
float UB2PreRenderer::PreRenderSpawnDistFromView = 100.0f;
bool UB2PreRenderer::bPreRenderAtFarAway = false;
bool UB2PreRenderer::bPreRenderAllPrimitives = true;
int32 UB2PreRenderer::PreRenderAllPrimLit2PassTickLimit = 2;
int32 UB2PreRenderer::PreRenderAllPrimLit1PassTickLimit = 2;
int32 UB2PreRenderer::PreRenderAllPrimUnlitPassTickLimit = 2;

void UB2PreRenderer::LoadSettings()
{
	if (GConfig)
	{
		GConfig->GetInt(TEXT("PreRender"), TEXT("MaxPreRenderCount"), MaxPreRenderCount, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("OverallPreRenderLimit"), OverallPreRenderLimit, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("PreRenderHardLimit"), PreRenderHardLimit, GGameIni);
		GConfig->GetFloat(TEXT("PreRender"), TEXT("PreRenderSpawnDistFromView"), PreRenderSpawnDistFromView, GGameIni);
		GConfig->GetBool(TEXT("PreRender"), TEXT("PreRenderAtFarAway"), bPreRenderAtFarAway, GGameIni);
		GConfig->GetBool(TEXT("PreRender"), TEXT("PreRenderAllPrimitives"), bPreRenderAllPrimitives, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("PreRenderAllPrimLit2PassTickLimit"), PreRenderAllPrimLit2PassTickLimit, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("PreRenderAllPrimLit1PassTickLimit"), PreRenderAllPrimLit1PassTickLimit, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("PreRenderAllPrimUnlitPassTickLimit"), PreRenderAllPrimUnlitPassTickLimit, GGameIni);
	}
}
void UB2PreRenderer::LoadSettings_PreRenderGM()
{
	// Pre-render 전용 게임모드에서 설정을 다르게 쓰려고 함. 특수 모드이므로 그게 가능
	if (GConfig)
	{
		GConfig->GetInt(TEXT("PreRender"), TEXT("MaxPreRenderCount_PreRenderGM"), MaxPreRenderCount, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("OverallPreRenderLimit_PreRenderGM"), OverallPreRenderLimit, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("PreRenderHardLimit_PreRenderGM"), PreRenderHardLimit, GGameIni);
	}
}

void UB2PreRenderer::SaveCompleteState()
{
	TotalCompleteState.SaveState();
	GConfig->Flush(false, GB2PreRenderSavedStateIni);
}
void UB2PreRenderer::LoadCompleteState()
{
	TotalCompleteState.LoadState();
}
void UB2PreRenderer::ClearCompleteState()
{
	TotalCompleteState.ClearState(); // 흔히 쓸 기능은 아니고 원래 테스트 용 기능인데 혹 셰이더 캐쉬 리셋 같은 상황에서 필요한 기능이 될지도? 
	MaterialBasedCompleteState.ClearState();
}