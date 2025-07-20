// Fill out your copyright notice in the Description page of Project Settings.
#include "B2PreRenderGameMode.h"
#include "B2PreRenderer.h"

#include "CommonStruct.h"
#include "Runtime/Engine/Public/Engine.h"


#include "Event.h"
//#include "B2PreLoadingScreen.h"
#include "B2GameLoadingProgress.h"
#include "B2DLCFrontGameMode.h"
#include "../BladeII/BladeIIUtil.h"

///////////////////////////////////////////////////////////////////////////////////////////
//
//	B2PreRenderGameMode
//

int32 AB2PreRenderGameMode::GetTotalContentTypePassNum() const
{ 
	// 2 개의 ContentPass 는 실제로는 PCClassNum 만큼임.
	return (((int32)EPRGMContentTypePass::CTP_End - 3) + (GetMaxPCClassNum() * 2)); 
}

float AB2PreRenderGameMode::GetTotalWeightedPassNum() const
{
	// 라이트 패스 중 하나는 가중치 곱해서..
	return (
		(GetFistLightPassProgressWeight() * (float)GetTotalContentTypePassNum()) +
		((GetTotalLightPassNum() - 1) * (float)GetTotalContentTypePassNum())
		);
}

AB2PreRenderGameMode::AB2PreRenderGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentLightPass = EPRGMLightPass::LP_None;
	CurrentContentTypePass = EPRGMContentTypePass::CTP_None;
	CurrentPCClassPass = EPCClass::EPC_End;
	PreRenderTimeScale = 1.f;
	SinglePassLeastTime = 1.f;
	TimeSinceLastPassBegin = 0.0f;
	WeightedPassSumSoFar = 0.0f;
	InternalLit2PassTickLimit = 3;
	InternalLit1PassTickLimit = 3;
	CurrentPassTickCount = 0;
}

void AB2PreRenderGameMode::PreBeginPlay()
{
	Super::PreBeginPlay();	

	if (GConfig)
	{
		GConfig->GetFloat(TEXT("PreRender"), TEXT("PreRenderGMTimeScale"), PreRenderTimeScale, GGameIni);
		GConfig->GetFloat(TEXT("PreRender"), TEXT("PreRenderGMSinglePassLeastTime"), SinglePassLeastTime, GGameIni);

		GConfig->GetInt(TEXT("PreRender"), TEXT("PreRenderGMInternalLit2PassTickLimit"), InternalLit2PassTickLimit, GGameIni);
		GConfig->GetInt(TEXT("PreRender"), TEXT("PreRenderGMInternalLit1PassTickLimit"), InternalLit1PassTickLimit, GGameIni);
	}
}

void AB2PreRenderGameMode::BeginPlay()
{
	//// 여기서 나름대로 표시하는 Progress 말고 이 PreRender 레벨을 로딩하는 progress 이쯤서 만땅.
	//B2GMLoadingProgCollector::StepToMax();

	//Super::BeginPlay();
	//
	//// Virtual joystick 없애기.
	//APlayerController* LocalPCon = UGameplayStatics::GetLocalPlayerController(this);
	//if (LocalPCon)
	//	LocalPCon->ActivateTouchInterface(nullptr);
}

void AB2PreRenderGameMode::SetupNextGameLoadingScreen()
{
	if (!AB2DLCFrontGameMode::GetSelectedFrontTitleMovieName().IsEmpty())
	{ // DLCFront 를 통해 왔다면 맨 처음 플레이했던 타이틀 동영상을 로딩 화면으로 다시 띄운다.
		AB2DLCFrontGameMode::TrySetupLoadingScreenWithInitialTitleMovie(this);
	}
	else
	{
		Super::SetupNextGameLoadingScreen();
	}
}

void AB2PreRenderGameMode::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

	// UpdateForPreRenderObjects 는 상위단에서 부르게 됨.

	TimeSinceLastPassBegin += DeltaSecond;
	++CurrentPassTickCount;

	// 각 하위 패스 안에서도 dynamic point light 조절을 좀 한다. 여기에 따라 셰이더 컴파일 영향을 좀 받는 듯.
	if (PreRenderer)
	{
		if (CurrentPassTickCount >= InternalLit1PassTickLimit + InternalLit2PassTickLimit)
		{
			if (CurrentPassTickCount == InternalLit1PassTickLimit + InternalLit2PassTickLimit)
			{
				// 둘 다 끔. PointLight 0 개 셰이더도 나름 필요..
				PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDynPL(false);
				// 플레이어 캐릭터 Head Light 의 경우는 반경을 극히 좁게 잡아서 사실상 꺼지도록 한다. 아싸리 꺼 버리려면 좀 더 복잡하고.. ㅡㅡ;
				PreRenderer->MinimizePlayerPointLight();
			}
		}
		else if (CurrentPassTickCount > InternalLit2PassTickLimit)
		{
			PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDynPL(false); // 하나 남기기
		}
	}
}

TArray<EPCClass> AB2PreRenderGameMode::GetPCClassesToPreLoad()
{
	TArray<EPCClass> RetArray;
	// 기본 기능 루트를 통해 무작정 로딩되지 않도록.. 필요할 때 그때그때 로딩할 것.
	/*for (int32 PCI = 0; PCI < GetMaxPCClassNum(); ++PCI)
	{
		RetArray.Add(IntToPCClass((PCI)));
	}*/
	return RetArray;
}

TArray<FCombinedPCSkillAnimID> AB2PreRenderGameMode::GetPCSkillAnimsToPreLoad()
{
	TArray<FCombinedPCSkillAnimID> RetArray;

	// 기본 기능 루트를 통해 무작정 로딩되지 않도록.. 필요할 때 그때그때 로딩할 것.

	return RetArray;
}

void AB2PreRenderGameMode::SetupPreRenderObjects()
{
	check(!PreRenderer);
	PreRenderer = NewObject<UB2PreRenderer>(this, NAME_None, RF_Transient);
	check(PreRenderer);
	PreRenderer->InitForPreRenderGameMode(this);

	UB2PreRenderer::LoadSettings_PreRenderGM(); // 전용 설정 따로 로딩. 끝나고 되돌려야 함.
//
//	// Pre-render 셋업 첫 진입점.
//
//	// 가장 첫 라이트 패스부터 해서 시작.
//	SetupForLightPass(GetFirstLightPass());
//	TimeSinceLastPassBegin = 0.0f;
//	CurrentPassTickCount = 0;
//
//	OpenPreLoadingScreen(EPreLoadingScreenType::PLST_PreRender_First);
//	check(CreatedPreLoadingScreen_PreRender);
//	if (CreatedPreLoadingScreen_PreRender)
//	{ // PreRenderGameMode 용 텍스트 모드로 세팅. 진행도는 텍스트 말고 프로그레스 바로 표시해 보자..
//		CreatedPreLoadingScreen_PreRender->SetupPreRenderProgress(true, false, true);
//	}
//
//	WeightedPassSumSoFar = 0.5f; // 처음에 약간 채워놓고 시작 ㅋ
//	UpdateProgressDisplay();	
//
//	// 기본 게임모드에서 수행하는 prerender 는 게임 로직과 얽히지 않도록 이걸 매우 작게 주어야 하는데 여기선 그럴 필요가 없다.
//	// 오히려 파티클 이펙트 같은 것들이 빨리 디스플레이 되어서 렌더링이 최대한 많이 되도록 TimeScale 을 약간 키울 수도..
//	SetWholeGameTimeScale(
//#if PLATFORM_WINDOWS // 여긴 의미가 없고 그냥 기능 확인 차원이므로 타임 스케일을 크게 주어서 빨리 넘어가도록
//		PreRenderTimeScale * 10.0f
//#else
//		PreRenderTimeScale
//#endif
//	);
	
	B2GMLoadingProgCollector::StepCustom(0.7f); // 여기서 나름대로 표시하는 Progress 말고 이 PreRender 레벨을 로딩하는 progress.
}

bool AB2PreRenderGameMode::IsInPreRenderPhase() const
{
	return (CurrentLightPass != EPRGMLightPass::LP_None && CurrentLightPass != EPRGMLightPass::LP_End &&
		CurrentContentTypePass != EPRGMContentTypePass::CTP_None && CurrentContentTypePass != EPRGMContentTypePass::CTP_End);
	// 여기서 CurrentPCClassPass 는 체크하지 않는다.
}

void AB2PreRenderGameMode::UpdateForPreRenderObjects()
{	
	checkSlow(IsInPreRenderPhase() && PreRenderer);

	if (PreRenderer)
	{
		const bool LeastTimeMet = (TimeSinceLastPassBegin >= SinglePassLeastTime);
		// 내부에서는 tick 카운트도 하고 여기선 넘어가기 위한 최소 시간도 측정
		PreRenderer->UpdateForPreRenderObjects(!LeastTimeMet);

		if (LeastTimeMet && !PreRenderer->HasPreRenderDummyToProcess())
		{
			ProceedToNextPass(); // 다음 단계로 넘어가든지 혹은 종료.
		}
	}
}

void AB2PreRenderGameMode::ProceedToNextPass()
{
	// 현 Pass 가 끝난 시점에 다음 Pass 로 넘어가든지, 혹은 이번이 완전 마지막 pass 이면 종료.
	// PreRenderPhase 는 쓰지 않고 대신 CurrentLightPass 와 CurrentContentTypePass 를 사용.

	CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);

	if (PreRenderer) {
		PreRenderer->EnsureSaveCaches(); // 혹여나 모를 크래쉬를 대비한 세이브?
	}

	if (IsLastLightPass(CurrentLightPass) && IsLastContentTypePass(CurrentContentTypePass))
	{
		OnCompleteAll();
	}
	else if (IsLastContentTypePass(CurrentContentTypePass))
	{
		SetupForLightPass((EPRGMLightPass)((int32)CurrentLightPass + 1));
	}
	else
	{
		// ContentTypePass 를 넘겨야 하는 경우인데 PC 나 PCSkillAnim 의 경우 하위 pass 가 또 있으니 그걸 체크
		if (CurrentContentTypePass == EPRGMContentTypePass::CTP_PC && !IsLastPCClassPass(CurrentPCClassPass))
		{
			SetupForPCClassPass(IntToPCClass(PCClassToInt(CurrentPCClassPass) + 1));
		}
		else if (CurrentContentTypePass == EPRGMContentTypePass::CTP_PC_SkillAnim && !IsLastPCClassPass(CurrentPCClassPass))
		{
			SetupForPCSkillAnimPass(IntToPCClass(PCClassToInt(CurrentPCClassPass) + 1));
		}
		else
		{
			SetupForContentTypePass((EPRGMContentTypePass)((int32)CurrentContentTypePass + 1));
		}
	}

	// 원칙대로라면 SetupForLightPass 의 일부가 되어야 하는데 예외 케이스가 좀 있어서.. 바보같지만 dynamic point light 셋업을 여기서 다시 ㅡㅡ
	if (PreRenderer)
	{
		PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDynPL();
		PreRenderer->ConditionalSetupPreRenderLightEnvForDynPL(true);
	}

	TimeSinceLastPassBegin = 0.0f;
	CurrentPassTickCount = 0;
	
	// Pass 넘어갈 때 진행도 업데이트
	WeightedPassSumSoFar += ((CurrentLightPass == GetFirstLightPass()) ? GetFistLightPassProgressWeight() : 1.0f);
	UpdateProgressDisplay();
}

void AB2PreRenderGameMode::SetupForLightPass(EPRGMLightPass InSetupLightPass)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("AB2PreRenderGameMode::SetupForLightPass %d"), (int32)InSetupLightPass));

	check(InSetupLightPass != EPRGMLightPass::LP_None && InSetupLightPass != EPRGMLightPass::LP_End);
	CurrentLightPass = InSetupLightPass;
	
	if (PreRenderer)
	{ // 라이팅 셋업하기 전에 리셋부터.
		PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDynPL();
		PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDirL();
	}

	switch (CurrentLightPass)
	{
		// 너무 많은 종류의 light pass 는 지양.. 처음에 여기에 너무 많은 시간을 들이는 것도 문제가 될 수 있고, 어떤 것들은 실질적인 효용성이 의심되기도 하고..

	case EPRGMLightPass::LP_StationaryDirectionalLight:
		if (PreRenderer) {
			PreRenderer->ConditionalSetupPreRenderLightEnvForDirL(EComponentMobility::Stationary, true);
			PreRenderer->ConditionalSetupPreRenderLightEnvForDynPL(true); // DirectionalLight 패스에서는 PointLight 도 같이 셋업.. 우리 실제 조명 환경으로
		}
		break;
#if _PRE_RENDER_GM_MORE_LIGHT_PASS && !_PRE_RENDER_GM_MOVABLE_EXTRA_ONLY
	case EPRGMLightPass::LP_MovableDirectionalLight:
		if (PreRenderer) {
			PreRenderer->ConditionalSetupPreRenderLightEnvForDirL(EComponentMobility::Movable, true);
			PreRenderer->ConditionalSetupPreRenderLightEnvForDynPL(true); // DirectionalLight 패스에서는 PointLight 도 같이 셋업.. 우리 실제 조명 환경으로
		}
		break;
#endif
	}
	
	// LightPass 는 ContentTypePass 보다 바깥이기 때문에 ContentTypePass 를 처음부터 시작한다.
	SetupForContentTypePass(GetFirstContentTypePass());
}

void AB2PreRenderGameMode::SetupForContentTypePass(EPRGMContentTypePass InContentTypePass)
{
	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("AB2PreRenderGameMode::SetupForContentTypePass %d"), (int32)InContentTypePass));

	check(InContentTypePass != EPRGMContentTypePass::CTP_None && InContentTypePass != EPRGMContentTypePass::CTP_End);
	CurrentContentTypePass = InContentTypePass;

	switch (CurrentContentTypePass)
	{
	case EPRGMContentTypePass::CTP_DamageEffectInfo:
		if (PreRenderer) {
			PreRenderer->SetupForPreRenderGM_DamageEffectInfo();
		}
		break;
	case EPRGMContentTypePass::CTP_PC:
		// PC Pass 의 경우 각 클래스 별 하위 패스가 있으며 그 첫 PCClass 패스를 시작.
		SetupForPCClassPass(GetFirstPCClassPass());

		break;
	case EPRGMContentTypePass::CTP_PC_SkillAnim:
		// PC SkillAnim Pass 도 경우 각 클래스 별 하위 패스 중 첫 PCClass 패스를 시작.
		SetupForPCSkillAnimPass(GetFirstPCClassPass());
		break;
	case EPRGMContentTypePass::CTP_Extra_NPC:
		if (PreRenderer) {
			PreRenderer->SetupForPreRenderGM_Extra(true, false, false);
		}
		break;
	case EPRGMContentTypePass::CTP_Extra_PC_NPC_CSM:
		SetupForExtraCSMPass();
		break;
	case EPRGMContentTypePass::CTP_Extra_FX:
		SetupForExtraFxAndOtherPass();
		break;
#if _PRE_RENDER_GM_MOVABLE_EXTRA_ONLY
	case EPRGMContentTypePass::CTP_Extra_MovableDirLight:
		SetupForMovableLightExtraOnlyPass();
		break;
#endif
	}

	if ((int32)CurrentContentTypePass >= (int32)EPRGMContentTypePass::CTP_Extra_NPC)
	{
		// 완전 끝까지는 아니더라도 적당히 후반부에서 완료 마크.
		UB2PreRenderer::MarkPreRenderLevelGoneThrough();
		UB2PreRenderer::SaveCompleteState();
	}
}

void AB2PreRenderGameMode::SetupForPCClassPass(EPCClass InPCClass)
{
	// EPRGMContentTypePass 중 CTP_PC 하위 패스

	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("AB2PreRenderGameMode::SetupForPCClass %d"), PCClassToInt(InPCClass)));

	check(InPCClass != EPCClass::EPC_End);
	CurrentPCClassPass = InPCClass;

	if (PreRenderer)
	{
		PreRenderer->SetupForPreRenderGM_PCClassPass(CurrentPCClassPass);
	}
}

void AB2PreRenderGameMode::SetupForPCSkillAnimPass(EPCClass InPCClass)
{
	// EPRGMContentTypePass 중 CTP_PC_SkillAnim 하위 패스

	B2_SCOPED_TRACK_LOG(FString::Printf(TEXT("AB2PreRenderGameMode::SetupForPCSkillAnimPass %d"), PCClassToInt(InPCClass)));

	check(InPCClass != EPCClass::EPC_End);
	CurrentPCClassPass = InPCClass;

	if (PreRenderer)
	{
		PreRenderer->SetupForPreRenderGM_PCSkillAnimPass(CurrentPCClassPass);
	}
}

void AB2PreRenderGameMode::SetupForExtraCSMPass()
{
	if (PreRenderer 
		&& ShouldDoExtraCSMPass() // 스킵하게 되면 그냥 아무것도 없는 채로 시간이 지나면 다음 패스로 넘어가게 됨.
		)
	{ // LightPass 처럼 라이팅 환경을 셋업하는데 ContentTypePass 의 일부만 사용하므로 LightPass 에 넣기도 애매하고 해서..
		PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDynPL();
		PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDirL();
		PreRenderer->ConditionalSetupPreRenderLightEnvForDirL(EComponentMobility::Stationary, true, true);
		PreRenderer->ConditionalSetupPreRenderLightEnvForDynPL(true);

		// PCClass 하나랑 NPCClass 셋업
		PreRenderer->SetupForPreRenderGM_PCClassPass(EPCClass::EPC_Assassin);
		PreRenderer->SetupForPreRenderGM_Extra(true, false, false);
	}
}

void AB2PreRenderGameMode::SetupForExtraFxAndOtherPass()
{
	check(CurrentContentTypePass == EPRGMContentTypePass::CTP_Extra_FX);

	//if (PreRenderer && ShouldDoExtraFxPass()/*ShouldDoExtraFxPass 는 ExtraFX 에만 영향. Others 는 그냥 하고 ㅋ*/) 
	//{
	//	PreRenderer->SetupForPreRenderGM_Extra(false, true, true);
	//}

	//{
	//	// 첫 전투에서 슬쩍 끊김을 유발하는 DamageNum 인데 여기에 꼽사리 끼워넣음
	//	// BuffModeEffectInfo 도 하면 좋으려나
	//	ABladeIIPlayer* LocalB2Player = Cast<ABladeIIPlayer>(UGameplayStatics::GetLocalPlayerCharacter(this));
	//	if (LocalB2Player)
	//	{
	//		CharacterTakeDamageClass<ABladeIICharacter*, float, bool>::GetInstance().Signal(LocalB2Player, 1000.0f, true);
	//	}

	//	// 정확히 Extra 패스와 관련된 건 아닌데 이쯤에서 Pre-render 레벨에 설치한 오브젝트들의 렌더링이 보다 확실히 들어가도록 하기 위한 셋업을 해 봄.
	//	for (TObjectIterator<UPrimitiveComponent> It; It; ++It)
	//	{
	//		UPrimitiveComponent* ThisPrimComp = *It;
	//		if (ThisPrimComp &&
	//			(Cast<UMeshComponent>(ThisPrimComp) || Cast<UParticleSystemComponent>(ThisPrimComp))
	//			)
	//		{
	//			ThisPrimComp->SetVisibility(true);
	//			ThisPrimComp->SetHiddenInGame(false);
	//			ThisPrimComp->SetExcessiveRenderProcessing(true);
	//		}
	//	}
	//}
}

#if _PRE_RENDER_GM_MOVABLE_EXTRA_ONLY
void AB2PreRenderGameMode::SetupForMovableLightExtraOnlyPass()
{
	check(CurrentContentTypePass == EPRGMContentTypePass::CTP_Extra_MovableDirLight);

	// MovableDirLight 가 실제로 사용되는 환경이 로비뿐이다 보니 MovableDirLight 패스를 전체적으로 하지 않고 특정 대상들에 대해서만 시도하는 거.
	if (PreRenderer && ShouldDoMovableLightExtraOnlyPass())
	{
		// 단, 이 경우 여기서 라이트 환경을 바꾸므로.. ContentTypePass 에서는 마지막일 필요가 있다. 안 그럼 꼬임.
		check(IsLastContentTypePass(EPRGMContentTypePass::CTP_Extra_MovableDirLight));
		PreRenderer->ConditionalRestoreFromPreRenderLightEnvForDirL();
		PreRenderer->ConditionalSetupPreRenderLightEnvForDirL(EComponentMobility::Movable, true);
				
		PreRenderer->SetupForPreRenderGM_Extra(false, false, true); // Extra Other 만..
	}
}
#endif

bool AB2PreRenderGameMode::ShouldDoExtraCSMPass() const
{
	// Scalability 설정이야 사용자 선택에 의해 바뀔 수도 있지만 PreRenderGameMode 가 처음 실행에 사용되는 것임을 생각하면 기본설정에서 shadow 가 사용되지 않을 저사양에선 스킵.
	static IConsoleVariable* ShadowQualityCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ShadowQuality"));
	check(ShadowQualityCVar);
	return (ShadowQualityCVar && ShadowQualityCVar->GetInt() > 0);
}
bool AB2PreRenderGameMode::ShouldDoExtraFxPass() const
{
	//// Effect quality 레벨이 낮다고 해서 셰이더 pre-compile 이 필요없는 건 아니지만 pre-render 의 부담이 클 수 있으므로 스킵
	//return (Scalability::GetQualityLevels_Effects() > 0);
	return true;
}

#if _PRE_RENDER_GM_MOVABLE_EXTRA_ONLY
bool AB2PreRenderGameMode::ShouldDoMovableLightExtraOnlyPass() const
{
	return ShouldDoExtraCSMPass(); // 적당히 시스템 사양 봐 가면서 동일한 기준을 적용하려는 거.
}
#endif

void AB2PreRenderGameMode::OnCompleteAll()
{
	B2_SCOPED_TRACK_LOG(TEXT("AB2PreRenderGameMode::OnCompleteAll"));

	//check(PreRenderer);
	//if (PreRenderer) 
	//{
	//	// 현재 레벨에서 Pre-render 가 한번 완료된 것으로 기록.. 하는 게 별로 의미는 없다. 실제로는 로비맵이랑 각 타입들에 대해 마크가 되어야 할 테니. 그래도 안 해줄 이유는 없지.
	//	PreRenderer->MarkCurrentWorldLevelPreRendered();
	//	PreRenderer->OnPreRenderComplete();
	//	PreRenderer->ConditionalBeginDestroy(); // 뭔가 빨리 제거 마크라도 해 주어야 안심이 될 듯한 기분
	//}
	//PreRenderer = nullptr;

	//// 레벨에 배치된 것들이야 이걸 리셋 안해도 되지만 루트셋 오브젝트도 있고 해서.. 여하간 ExcessiveRenderProcessing 은 사용 목적이 끝나면 바로 리셋해야.
	//for (TObjectIterator<UPrimitiveComponent> It; It; ++It)
	//{
	//	UPrimitiveComponent* ThisPrimComp = *It;
	//	if (ThisPrimComp)
	//	{
	//		ThisPrimComp->SetExcessiveRenderProcessing(false);
	//	}
	//}

	//CurrentLightPass = EPRGMLightPass::LP_End;
	//CurrentContentTypePass = EPRGMContentTypePass::CTP_End;

	//// 여기서 닫으려고 했더니 한 프레임 살짝 드러남. 어차피 이렇게 안 해도 제거될 것.
	////ClosePreLoadingScreen(EPreLoadingScreenType::PLST_PreRender_First);

	//UB2PreRenderer::LoadSettings(); // PreRenderGM 버전으로 별도 설정 로딩했던 거 되돌림 

	//// 모든 준비는 끝났어. 가자, 블레이드II 의 세상으로.

	//// 실제 유저 루트로서는 이건 설치 후 처음 접하게 될 루트이지만 그렇다고 여기서 튜토리얼 맵을 로딩하면 자칫 다른 상황에서 꼬일 수 있다.. 
	//// 일단 로비로 갔다가 거기서 튜토리얼 맵으로 가는 걸로.
	//OpenBladeIILobbyCommon(this);
}

void AB2PreRenderGameMode::UpdateProgressDisplay()
{
	//float ProgressScale = FMath::Clamp( WeightedPassSumSoFar / (float)GetTotalWeightedPassNum(), 0.0f, 1.0f);

	//if (CreatedPreLoadingScreen_PreRender)
	//{ // UB2PreLoadingScreen 안에서도 자체 애니메이션이 돌아갈 것.
	//	CreatedPreLoadingScreen_PreRender->UpdatePreRenderProgressScale(ProgressScale);
	//}
}

//================================================================

bool CheckBladeIIPreRenderGameAvailability()
{
	FString PreRenderMapName;

	if (!GConfig->GetString(TEXT("PreRender"), TEXT("BladeIIPreRenderMap"), PreRenderMapName, GGameIni)) {
		return false;
	}

	// 실제 맵을 못찾으면 다른 식으로 핸들링을 하도록 false 리턴.
	if (!GEngine->MakeSureMapNameIsValid(PreRenderMapName)) {
		return false;
	}

	return true;
}

bool StartBladeIIPreRenderGame(class AGameMode* InCurrentGameMode)
{
	B2_SCOPED_TRACK_LOG(TEXT("StartBladeIIPreRenderGame"));

	FString PreRenderMapName;

	if (!GConfig->GetString(TEXT("PreRender"), TEXT("BladeIIPreRenderMap"), PreRenderMapName, GGameIni)) {
		return false;
	}

	// 이거 필요한가..
	OpenPreLoadingScreenClass<EPreLoadingScreenType, EB2GameMode>::GetInstance().Signal(EPreLoadingScreenType::PLST_StageBegin, EB2GameMode::Lobby);

	// 순수 맵 로딩만의 예상 지분 세팅.
	B2GMLoadingProgCollector::PrepareWithPureMapLoadShareForGM(InCurrentGameMode, EB2GameMode::PreRender);

	UGameplayStatics::OpenLevel(InCurrentGameMode, FName(*PreRenderMapName), true);

	return true;
}