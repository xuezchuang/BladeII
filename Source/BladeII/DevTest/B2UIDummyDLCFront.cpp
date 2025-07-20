
#include "B2UIDummyDLCFront.h"

#include "UnrealEngine.h"
#include "Global.h"
#include "BladeIIUtil.h"

UB2UIDummyDLCFront::UB2UIDummyDLCFront(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2UIDummyDLCFront::ShowFakeDLCUpdate(bool bShow)
{
	if (P_FakeDLCUpdate.IsValid())
	{
		P_FakeDLCUpdate->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UB2UIDummyDLCFront::ShowFakeKakaoLogin(bool bShow)
{
	if (P_FakeKakaoConnect.IsValid())
	{
		P_FakeKakaoConnect->SetVisibility(bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
	if (BTN_FakeKakaoConnect.IsValid())
	{
		BTN_FakeKakaoConnect->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UB2UIDummyDLCFront::OnClickBtnFakeUpdate()
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1

	//
	// 여긴 실제 로비로 진입하기 위해 (훼이크로) DLC 업데이트 체크 및 다운로드를 실행하는 것의 의미를 가지고 있다.
	// 실제로는 로비가 떠 있는 상황이므로 컨텐츠 다운로드 면에서 DLC 프론트를 완전히 모의할 수는 없다.
	// DLC Front 에서부터 네트워크 연결 차원의 처리에 영향을 미치는 부분이 있으면 여기서 비슷하게 따라할 수 있다.
	//



	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		// 만약 여기서 실제로 의미있는 무언가를 한다면 OnFakeUpdate 의 Execute 는 그게 완료될때까지 지연시킨다.
		// 일단은 실질적으로 바로 UpdateComplete 처리가 되도록 매우 짧은 타이머를 설치.
		TheWorld->GetTimerManager().SetTimer(FakeUpdateCompleteTH, this, &UB2UIDummyDLCFront::OnFakeUpdateComplete, 0.01f, false);
	}
#endif
}

void UB2UIDummyDLCFront::OnClickBtnFakeKakaoConnect()
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	// 첫 카카오 계정 연결이 되었음. 마크.
	GConfig->SetBool(TEXT("DummyDLCFront"), TEXT("HasFakeKakaoConnection"), true, GB2GeneralSavedStateIni);
	GConfig->Flush(false, GB2GeneralSavedStateIni);
	FakeKakaoLoginHandlingCommon();
#endif
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
void UB2UIDummyDLCFront::FakeKakaoLoginHandlingCommon()
{

	//
	// DLCFront 의 일부인 카카오 로그인. 이게 네트워크 초기화랑 밀접한 연관이 있어서 절차 모의 차원에서 .
	// 카카오 로그인 시점에 뭔가 의미있는 정보를 받는 게 있다면 여기서 그걸 모의해 볼 수도.
	//


	ShowFakeKakaoLogin(false);
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		// 여기에 들어올 수 있는 작업에 따라 OnFakeKakaoLogin 의 Execute 를 지연시킨다.
		// 일단은 실질적으로 바로 LoginComplete 처리가 되도록 매우 짧은 타이머를 설치.
		TheWorld->GetTimerManager().SetTimer(FakeKakaoLoginTH, this, &UB2UIDummyDLCFront::OnFakeKakaoLoginComplete, 0.01f, false);
	}
}

/** 기본적으로 IsFinalDLCCookRun 으로 체크를 하겠지만 그게 아니어도 DLCFrontMap 을 통과해서 오는 상황이 발생할 수 있어서 추가 트랙킹을 한다. */
bool UB2UIDummyDLCFront::bWentThroughRealDLCFront = false;

void UB2UIDummyDLCFront::OnFakeUpdateComplete()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(FakeUpdateCompleteTH);
	}

	// 아래에 타이틀 화면이 드러나도록 DLC Update 관련한 패널은 숨긴다.
	ShowFakeDLCUpdate(false); // 어차피 UIDummyDLCFront 자체가 destroy 되겠지만..

	OnFakeUpdate.ExecuteIfBound();
}

void UB2UIDummyDLCFront::BeginFakeKakaoLogin()
{
	bool bHasKakaoConnection = false;
	GConfig->GetBool(TEXT("DummyDLCFront"), TEXT("HasFakeKakaoConnection"), bHasKakaoConnection, GB2GeneralSavedStateIni);
	if (!bHasKakaoConnection && P_FakeKakaoConnect.IsValid() && BTN_FakeKakaoConnect.IsValid())
	{ // 첫 카카오 로그인. 계정 연결 UI 를 보임.
		ShowFakeKakaoLogin(true);	
	}
	else
	{ // 그게 아니면 바로 로그인을 실행하는 걸로 친다. 겉보기 작동 보고 그냥 따라하는 거
		FakeKakaoLoginHandlingCommon();
	}
}

void UB2UIDummyDLCFront::OnFakeKakaoLoginComplete()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(FakeKakaoLoginTH);
	}

	OnFakeKakaoLogin.ExecuteIfBound();
}

TSubclassOf<class UB2UIDummyDLCFront> UB2UIDummyDLCFront::GetUniqueClass()
{
	// 개발용 기능이고 쉽핑에서 최대한 많이 빠지도록 굳이 ini 에서 읽어들이지 않는 걸로.
	FString WidgetClassPath = TEXT("/Game/UI/DevOnlyWidget/DummyDLCFrontWidget");

	ConstructorHelpers::StripObjectClass(WidgetClassPath, true);
	TSubclassOf<UB2UIDummyDLCFront> FoundWidgetClass = ConstructorHelpersInternal::FindOrLoadClass(WidgetClassPath, UB2UIDummyDLCFront::StaticClass());
	return FoundWidgetClass;
}

int32 UB2UIDummyDLCFront::GetViewportZOrder()
{
	return BII_WIDGET_ZORDER_SEPARATE_ABOVE_UIMAIN;
}

void UB2UIDummyDLCFront::DestroySelf()
{
	OnFakeUpdate.Unbind();
	OnFakeKakaoLogin.Unbind();

	UB2UnitedWidgetBase::DestroySelfCommon();
}

void UB2UIDummyDLCFront::NativeConstruct()
{
	Super::NativeConstruct();

	CacheAssets();
}

void UB2UIDummyDLCFront::CacheAssets()
{
	GET_SLOT(UPanelWidget, P_FakeDLCUpdate);
	GET_SLOT(UB2Button, BTN_FakeDLCUpdate);
	BIND_CLICK_FUNC_TO_BTN(BTN_FakeDLCUpdate, &UB2UIDummyDLCFront::OnClickBtnFakeUpdate);

	GET_SLOT(UPanelWidget, P_FakeKakaoConnect);
	GET_SLOT(UB2Button, BTN_FakeKakaoConnect);
	BIND_CLICK_FUNC_TO_BTN(BTN_FakeKakaoConnect, &UB2UIDummyDLCFront::OnClickBtnFakeKakaoConnect);

	ShowFakeDLCUpdate(false);
	ShowFakeKakaoLogin(false);
}
#endif
