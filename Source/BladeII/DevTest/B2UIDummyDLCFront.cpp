
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
	// ���� ���� �κ�� �����ϱ� ���� (����ũ��) DLC ������Ʈ üũ �� �ٿ�ε带 �����ϴ� ���� �ǹ̸� ������ �ִ�.
	// �����δ� �κ� �� �ִ� ��Ȳ�̹Ƿ� ������ �ٿ�ε� �鿡�� DLC ����Ʈ�� ������ ������ ���� ����.
	// DLC Front �������� ��Ʈ��ũ ���� ������ ó���� ������ ��ġ�� �κ��� ������ ���⼭ ����ϰ� ������ �� �ִ�.
	//



	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		// ���� ���⼭ ������ �ǹ��ִ� ���𰡸� �Ѵٸ� OnFakeUpdate �� Execute �� �װ� �Ϸ�ɶ����� ������Ų��.
		// �ϴ��� ���������� �ٷ� UpdateComplete ó���� �ǵ��� �ſ� ª�� Ÿ�̸Ӹ� ��ġ.
		TheWorld->GetTimerManager().SetTimer(FakeUpdateCompleteTH, this, &UB2UIDummyDLCFront::OnFakeUpdateComplete, 0.01f, false);
	}
#endif
}

void UB2UIDummyDLCFront::OnClickBtnFakeKakaoConnect()
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
	// ù īī�� ���� ������ �Ǿ���. ��ũ.
	GConfig->SetBool(TEXT("DummyDLCFront"), TEXT("HasFakeKakaoConnection"), true, GB2GeneralSavedStateIni);
	GConfig->Flush(false, GB2GeneralSavedStateIni);
	FakeKakaoLoginHandlingCommon();
#endif
}

#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV1
void UB2UIDummyDLCFront::FakeKakaoLoginHandlingCommon()
{

	//
	// DLCFront �� �Ϻ��� īī�� �α���. �̰� ��Ʈ��ũ �ʱ�ȭ�� ������ ������ �־ ���� ���� �������� .
	// īī�� �α��� ������ ���� �ǹ��ִ� ������ �޴� �� �ִٸ� ���⼭ �װ� ������ �� ����.
	//


	ShowFakeKakaoLogin(false);
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		// ���⿡ ���� �� �ִ� �۾��� ���� OnFakeKakaoLogin �� Execute �� ������Ų��.
		// �ϴ��� ���������� �ٷ� LoginComplete ó���� �ǵ��� �ſ� ª�� Ÿ�̸Ӹ� ��ġ.
		TheWorld->GetTimerManager().SetTimer(FakeKakaoLoginTH, this, &UB2UIDummyDLCFront::OnFakeKakaoLoginComplete, 0.01f, false);
	}
}

/** �⺻������ IsFinalDLCCookRun ���� üũ�� �ϰ����� �װ� �ƴϾ DLCFrontMap �� ����ؼ� ���� ��Ȳ�� �߻��� �� �־ �߰� Ʈ��ŷ�� �Ѵ�. */
bool UB2UIDummyDLCFront::bWentThroughRealDLCFront = false;

void UB2UIDummyDLCFront::OnFakeUpdateComplete()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().ClearTimer(FakeUpdateCompleteTH);
	}

	// �Ʒ��� Ÿ��Ʋ ȭ���� �巯������ DLC Update ������ �г��� �����.
	ShowFakeDLCUpdate(false); // ������ UIDummyDLCFront ��ü�� destroy �ǰ�����..

	OnFakeUpdate.ExecuteIfBound();
}

void UB2UIDummyDLCFront::BeginFakeKakaoLogin()
{
	bool bHasKakaoConnection = false;
	GConfig->GetBool(TEXT("DummyDLCFront"), TEXT("HasFakeKakaoConnection"), bHasKakaoConnection, GB2GeneralSavedStateIni);
	if (!bHasKakaoConnection && P_FakeKakaoConnect.IsValid() && BTN_FakeKakaoConnect.IsValid())
	{ // ù īī�� �α���. ���� ���� UI �� ����.
		ShowFakeKakaoLogin(true);	
	}
	else
	{ // �װ� �ƴϸ� �ٷ� �α����� �����ϴ� �ɷ� ģ��. �Ѻ��� �۵� ���� �׳� �����ϴ� ��
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
	// ���߿� ����̰� ���ο��� �ִ��� ���� �������� ���� ini ���� �о������ �ʴ� �ɷ�.
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
