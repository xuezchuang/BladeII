
#include "B2UIWidget.h"
#include "MovieScene.h"
#include "Animation/WidgetAnimation.h"
#include "B2UIManager.h"
#include "Event.h"

UB2UIWidget::UB2UIWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bPendingPause = false;
	MyUIFName = NAME_None;
}

UB2UIWidget::~UB2UIWidget()
{
}

void UB2UIWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UB2UIWidget::Init()
{
	Super::Init();
}

void UB2UIWidget::DestroySelf(UB2UIManager* InUIManager)
{
	Super::DestroySelf(InUIManager);

	// UB2UIManager::InternalCloseUI 俊辑 窍绰 窿捞变 茄单 父俊 窍唱 CloseUI 甫 芭摹瘤 臼绊 流立 摧酒霖促搁 UIManager 率狼 饭欺繁胶啊 巢酒乐霸 登骨肺 概俺函荐肺 逞败林档废 秦辑 力芭秦 淋.
	// 弊矾唱 老何 UIWidget 狼 版快 眠啊 贸府甫 窍绰 霸 粱 乐绢辑 咯扁辑 UIManager 率 贸府甫 窍扁 帮鄂茄 巴档 乐澜. NULL 捞搁 公矫窍档废.
	FB2UIWidgetData* RelevantWidgetData = InUIManager ? InUIManager->GetWidgetDataByWidget(this) : NULL;
	if (RelevantWidgetData)
	{
		RelevantWidgetData->Widget = NULL;
	}
}

void UB2UIWidget::OnOpen(bool RightNow)
{
	if (RightNow)
	{
		//咯扁辑 坷锹 局丛阑 付瘤阜俊辑 谗绢 犁积秦霖促

		OnOpenComplete();
	}
	else
	{
		OpenState = EUIOpenState::OnOpening;

		// GetAnimation 力措肺 累悼 救 窃.
		UWidgetAnimation* OpenCloseAnim = GetAnimation(TEXT("ANIM_OpenClose"));
		if (OpenCloseAnim)
		{
			//咯扁辑 坷锹 局丛阑 犁积秦霖促.

			//泅犁 坷锹 局丛 包访 累诀 救沁栏聪 凯赴吧肺
			OnOpenComplete();
		}
		else
			//局丛捞 绝栏搁 凯赴吧肺
			OnOpenComplete();
	}

	OnOpen_BP();
}

void UB2UIWidget::OnClose(bool RightNow)
{
	if (RightNow)
	{
		//咯扁辑 努肺令 局丛阑 付瘤阜俊辑 谗绢 犁积秦霖促

		OnCloseComplete();
	}
	else
	{
		OpenState = EUIOpenState::OnClosing;

		 //GetAnimation 力措肺 累悼 救 窃.
		UWidgetAnimation* OpenCloseAnim = GetAnimation(TEXT("ANIM_OpenClose"));
		if (OpenCloseAnim)
		{
			//咯扁辑 努肺令 局丛阑 犁积秦霖促.

			//泅犁 努肺令 局丛 包访 累诀 救沁栏聪 摧腮吧肺
			OnCloseComplete();
		}
		else
			OnCloseComplete();
	}

	OnClose_BP();
}

void UB2UIWidget::OnSceneOpen(EUIScene InOpenedScene)
{
	// OnOpen 捞唱 Init 篮 磊脚阑 器窃茄 Scene 捞 凯府绰 惑炔俊辑 Scene 狼 促弗 Widget 捞 凯啡绰瘤 咯何俊 措茄 焊厘篮 绝促.
	// 捞巴捞 阂府绰 鸥捞怪篮 磊脚阑 器窃茄 Scene 傈眉啊 凯赴 捞饶烙.
}

void UB2UIWidget::OnHistoryOpen(bool IsHistory)
{
	//第肺啊扁肺 甸绢吭阑版快 阂妨柳促
}

void UB2UIWidget::HandlePendingPause()
{
	if (bPendingPause)
	{
		UGameplayStatics::SetGamePaused(GetOwningPlayer(), true);
		bPendingPause = false;
	}
}

void UB2UIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	HandlePendingPause();
}

void UB2UIWidget::CloseSelf()
{
	UB2UIManager* pUIManager = UB2UIManager::GetInstance();

	if (pUIManager)
		pUIManager->CloseUI(MyUIFName);

	// 概聪廉 救烹窍备 弊成 父甸绢 嘿看阑荐档 乐澜.
	if (MyUIFName == NAME_None)
	{
		RemoveFromParent();
	}
}

void UB2UIWidget::OnOpenComplete()
{
	OpenState = EUIOpenState::Opened;

	UIWidgetOpenCompleteClass<FName>::GetInstance().Signal(this->GetMyUIFName());
}

void UB2UIWidget::OnCloseComplete()
{
	OpenState = EUIOpenState::Closed;
	UB2UIManager::GetInstance()->CloseUI(GetMyUIFName()); //唱 楷免 场车绢 摧酒拎
}

void UB2UIWidget::SetPendingPause()
{
	bPendingPause = true;
}
