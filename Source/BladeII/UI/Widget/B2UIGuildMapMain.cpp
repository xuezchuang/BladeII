// Fill out your copyright notice in the Description page of Project Settings.
#include "B2UIGuildMapMain.h"
#include "B2UIManager.h"
#include "B2UIGuildInfoPart.h"
#include "B2UIGuildMapEntryDirect.h"

#include "B2UIGuildMapCharPopup.h"
#include "B2LobbyGameMode.h"
#include "B2GuildCastleActor.h"
#include "B2UIDocHelper.h"
#include "Retailer.h"
#include "B2UIGuildBattleSiuation.h"
////#include "Matinee/MatineeActor.h"


#include "BladeIIGameImpl.h"
#include "BladeIIUtil.h"
#include "B2UIChatting.h"

int32 UB2UIGuildMapMain::MaxAttackCount = 2;
int32 UB2UIGuildMapMain::MaxMedal = 8;
int32 UB2UIGuildMapMain::MaxMapSlotCount = 5;
bool  UB2UIGuildMapMain::MyGuildInfoState = false;

void UB2UIGuildMapMain::CacheAssets()
{
	Super::CacheAssets();
	GET_SLOT(UB2RichTextBlock, TB_Time);
	GET_SLOT(UB2Button, BTN_FormationInfo);
	GET_SLOT(UWidgetAnimation, Anim_Open);
	GET_SLOT(UWidgetAnimation, Anim_SpecialOpen);
	GET_SLOT(UB2Button, BTN_BattleList);
	GET_SLOT(UB2Button, BTN_OtherGuild);
	GET_SLOT(UB2UIGuildMapEntryDirect, UIP_GuildMapEntryDirect);
	if(UIP_GuildMapEntryDirect.IsValid()) UIP_GuildMapEntryDirect->Init();



	GET_SLOT(UB2UIGuildMapCharPopup, UIP_GuildMapInfoPopup);
	if (UIP_GuildMapInfoPopup.IsValid())
	{
		UIP_GuildMapInfoPopup->Init();
	}


	GET_SLOT(UTextBlock, TB_GuildMemberCount);

	GET_SLOT(UVerticalBox, VB_MapQuipSlot);

	GET_SLOT(UB2ScrollBox, SB_MapScroll);

	GET_SLOT(UB2Image, IMG_ScrollSlot);

	GET_SLOT(UTextBlock, TB_AttackCount);

	GET_SLOT(UB2Button, BTN_Back);

	//if (IMG_ScrollSlot.IsValid())
	//	IMG_ScrollSlot->GetBrush().ImageSize = GEngine->GameViewport->Viewport->GetSizeXY();

	GET_SLOT(UB2RichTextBlock, TB_BTNOtherGuildText);
	GET_SLOT(UB2UIGuildBattleSiuation, GuildBattleSituationPopup);
	if (GuildBattleSituationPopup.IsValid())
		GuildBattleSituationPopup->Init();

	GET_SLOT(UVerticalBox, VB_MapScroll);

	GET_SLOT(UB2Button, BTN_Chat)

	GET_SLOT(UB2Button, BTN_QuipUp)
	GET_SLOT(UB2Button, BTN_QuipDown)

	ScrollLv = 0;
	NowScrollLv = 0;
	SelectIndex = 0;

}

void UB2UIGuildMapMain::BindDelegates()
{
	Super::BindDelegates();
	//BIND_CLICK_FUNC_TO_BTN(BTN_FormationInfo, &UB2UIGuildMapMain::OnClickBTN_FormationInfo);
	//BIND_CLICK_FUNC_TO_BTN(BTN_BattleList, &UB2UIGuildMapMain::OnClickBTN_BattleList);
	//BIND_CLICK_FUNC_TO_BTN(BTN_OtherGuild, &UB2UIGuildMapMain::OnClickBTN_OtherGuild);
	//BIND_CLICK_FUNC_TO_BTN(BTN_Back, &UB2UIGuildMapMain::OnClickBTN_Back);
	//BIND_CLICK_FUNC_TO_BTN(BTN_Chat, &UB2UIGuildMapMain::OnClickBTN_Chat);
	//BIND_CLICK_FUNC_TO_BTN(BTN_QuipUp, &UB2UIGuildMapMain::OnClickBTN_QuipUp);
	//BIND_CLICK_FUNC_TO_BTN(BTN_QuipDown, &UB2UIGuildMapMain::OnClickBTN_QuipDown);


	//if (Anim_Open.IsValid())
	//{
	//	Anim_Open->OnAnimationFinished.AddDynamic(this, &UB2UIGuildMapMain::OpenAnimFinish);
	//}

	//if (Anim_SpecialOpen.IsValid())
	//{
	//	Anim_SpecialOpen->OnAnimationFinished.AddDynamic(this, &UB2UIGuildMapMain::OpenAnimFinish);
	//}
}

void UB2UIGuildMapMain::CloseWidgetDelegate()
{
	OnClickBTN_Back();
}

UB2UIGuildMapMain::UB2UIGuildMapMain(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UB2UIGuildMapMain::Init()
{
	SetClose(false);

	Super::Init();

	UWorld* TheWorld = GetWorld();
	if (TheWorld)
	{
		TheWorld->GetTimerManager().SetTimer(Timerhandle, this, &UB2UIGuildMapMain::RequestBattleInfo, 10.0f, true);
	}
}

void UB2UIGuildMapMain::DestroySelf(UB2UIManager* InUIManager)
{

	Super::DestroySelf(InUIManager);


	MedalDestroySelf(InUIManager,ArrUIGuildInfoMedal1_1);
	MedalDestroySelf(InUIManager,ArrUIGuildInfoMedal1);
	MedalDestroySelf(InUIManager,ArrUIGuildInfoMedal2);
	MedalDestroySelf(InUIManager,ArrUIGuildInfoMedal2_1);

}


void UB2UIGuildMapMain::MedalDestroySelf(UB2UIManager* InUIManager, TArray<UB2UIGuildInfoMedalCastlePart*>& InMedalUI)
{
	//for (auto El : InMedalUI)
	//{
	//	El->RemoveFromViewport();
	//}

	InMedalUI.Empty();
}

void UB2UIGuildMapMain::OnOpen(bool RightNow /*= false*/)
{
	Super::OnOpen(RightNow);


	MyGuildInfoState = false;
	MatineeState = false;

	SB_MapScroll->SetAnimateScroll(true);
	InitOpenGuildMap();

	UIHidden(true);
	if (BTN_OtherGuild.IsValid())
		BTN_OtherGuild->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (SB_MapScroll.IsValid())
		SB_MapScroll->SetVisibility(ESlateVisibility::HitTestInvisible);
}

#define GUILDMEDALINFO_REMOVE(ArrMedal)												\
	for(auto& El : ArrMedal){														\
		El->RemoveFromParent();}													\
	ArrMedal.Empty();																\

void UB2UIGuildMapMain::OnClose(bool RightNow /*= false*/)
{
	Super::OnClose(RightNow);
	GUILDMEDALINFO_REMOVE(ArrUIGuildInfoMedal1);
	GUILDMEDALINFO_REMOVE(ArrUIGuildInfoMedal2);

}

void UB2UIGuildMapMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (SB_MapScroll->IsScrolling())
	{
		GEngine->HandleCeCommand(GetWorld(), TEXT("UIScrollMoveEvent"), *GLog);
		ScrollState = false;
		MedalMoveUI();
		UpdateQuipSlot();
		
	}
	else
	{
		if (!ScrollState &&	!MatineeState)
		{
			ScrollState = true;
			UpdateQuipSlot();
			MedalMoveUI();
			GEngine->HandleCeCommand(GetWorld(), TEXT("DoneUIScrollMoveEvent"), *GLog);
		}
	}

	GuildBattleTimeTick();

}

void UB2UIGuildMapMain::InitOpenGuildMap()
{

	auto* GuildDoc = UB2UIDocHelper::GetDocGuild();

	if (!GuildDoc)
		return;

	GuildBattleEndTime = GuildDoc->GetGuildBattleEndTime();

	ScrollState = false;

	auto& ClientData = BladeIIGameImpl::GetClientDataStore();
	auto GuildInfo = BladeIIGameImpl::GetClientDataStore().GetMyGuildBattleInfo();
	auto OtherGuildInfo = BladeIIGameImpl::GetClientDataStore().GetOtherGuildBattleInfo();

	if (TB_AttackCount.IsValid())			//傍拜雀荐
	{
		b2network::B2GuildBattleMemberPtr MyGuildInfo = nullptr;
		for (auto El : GuildInfo->members)
		{
			if (ClientData.GetAccountId() == El->account_id)
			{
				MyGuildInfo = El;
				break;
			}

		}
		BTN_FormationInfo->SetIsEnabled(MyGuildInfo ? true : false);
		TB_AttackCount->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), MyGuildInfo ? MyGuildInfo->left_attack : 0 , MaxAttackCount)));
	}


	SetGuildInfo();

	if(TB_GuildMemberCount.IsValid())
		TB_GuildMemberCount->SetText(FText::AsNumber(MyGuildInfoState ? GuildInfo->members.Num() : OtherGuildInfo->members.Num()));

	ScrollSetting(true, MyGuildInfoState ? ((GuildInfo->members.Num() + MaxMapSlotCount - 1) / MaxMapSlotCount) : ((OtherGuildInfo->members.Num() + MaxMapSlotCount - 1) / MaxMapSlotCount));
	SetGuildChangeText();
	SB_MapScroll->SetScrollOffset(0);

	GEngine->HandleCeCommand(GetWorld(), TEXT("CastleSettingEvent"), *GLog);
}


void UB2UIGuildMapMain::CameraPosSetting(float PosY1, float PosY2, float PosY3)
{
	FirstCameraY = PosY1;
	SecondCameraY = PosY2;
	ThirdCameraY = PosY3;
}

float UB2UIGuildMapMain::ScrollCameraSetting()
{
	if (!SB_MapScroll.Get())
		return 0;

	float OffSet = SB_MapScroll->GetScrollOffset();

	const FVector2D ScreenSize = GEngine->GameViewport->Viewport->GetSizeXY();

	int32 Lv = (OffSet / ScreenSize.Y);
	float MoveScoll = OffSet - (Lv * ScreenSize.Y);
	float Ratio = MoveScoll / ScreenSize.Y;

	ScrollLv = Lv;
	if (Lv % 2 == 0 && ScrollLv == Lv)
	{
		return FirstCameraY + (SecondCameraY - FirstCameraY) * Ratio;
	}
	else
	{
		float ScrollOffSet = (ThirdCameraY - SecondCameraY) * Ratio;

		if (Ratio > 0.5)
		{
			WorldMapUIUpdate(Lv + 1);
			return FirstCameraY + (((ThirdCameraY - SecondCameraY) - ScrollOffSet) * -1);
		}
		else
		{
			WorldMapUIUpdate(Lv - 1);
			return SecondCameraY + ScrollOffSet;
		}
	}

	return 0;
}

float UB2UIGuildMapMain::DoneMoveCameraSetting()
{
	if (!SB_MapScroll.Get())
		return 0;


	float OffSet = SB_MapScroll->GetScrollOffset();

	const FVector2D ScreenSize = GEngine->GameViewport->Viewport->GetSizeXY();

	int32 Lv = FMath::RoundToInt((OffSet / ScreenSize.Y));

	if (Lv % 2 == 0)
		return FirstCameraY;
	else
		return SecondCameraY;

	WorldMapUIUpdate(Lv);

}

void UB2UIGuildMapMain::FinishMatineeSetting()
{

	SetCastleInfo();
	MatineeState = true;
}

void UB2UIGuildMapMain::ScrollSetting(bool MyMap, int32 Member)
{
	//if (!IMG_ScrollSlot.IsValid())
	//	return;


	//////磊扁辨靛牢瘤 惑措辨靛牢瘤 汲沥秦具窍绰何盒
	//SB_MapScroll->ClearChildren();
	//VB_MapQuipSlot->ClearChildren();

	//SlotMax = Member - 1;


	//FVector2D ScreenSize;
	//GEngine->GameViewport->GetViewportSize(ScreenSize);

	//
	//
	//for (int32 i = 0; i < Member; ++i)
	//{
	//	
	//	IMG_ScrollSlot->GetBrush().ImageSize = ScreenSize;

	//	FSlateBrush imgBrush = IMG_ScrollSlot->Brush;
	//	IMG_ScrollSlot->SetBrush(imgBrush);
	//	UB2Image* SlotImage = DuplicateObject<UB2Image>(IMG_ScrollSlot.Get(), IMG_ScrollSlot.Get());
	//	auto* MepSlot = CreateWidget<UB2UIGuildMapSlot>(GetWorld(), MapSlot);


	//	if (!SlotImage && !MepSlot)
	//		return;

	//	MepSlot->Init();

	//	//UScrollBoxSlot
	//	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(SB_MapScroll->Slot);


	//	if (PanelSlot)
	//	{
	//		FMargin ScrollOffset = PanelSlot->GetOffsets();
	//		ScrollOffset.Left = (ScreenSize.X) * -1;
	//		ScrollOffset.Top = (ScreenSize.Y / 2) *-1;
	//		ScrollOffset.Right = ScreenSize.X * 2;
	//		ScrollOffset.Bottom = ScreenSize.Y;
	//		PanelSlot->SetOffsets(ScrollOffset);
	//		//PanelSlot->SetSize(ScreenSize);
	//	}

	//	SB_MapScroll->AddChild(SlotImage);

	//	UVerticalBoxSlot* VerticalSlot = Cast<UVerticalBoxSlot>(VB_MapQuipSlot->AddChild(MepSlot));

	//	if (VerticalSlot)
	//	{
	//		VerticalSlot->SetSize(ESlateSizeRule::Fill);
	//		VerticalSlot->SetHorizontalAlignment(HAlign_Fill);
	//		VerticalSlot->SetVerticalAlignment(VAlign_Fill);
	//	}

	//	i == 0 ? MepSlot->OnSelectMap() : MepSlot->OffSelectMap();
	//	
	//	MepSlot->SetMyCastlePos(false);
	//	MepSlot->SetMapIndex(i);
	//	MepSlot->SetDelegate(FGuildMapquickSlot::CreateLambda([this](int32 MapIndex) {this->QuickScrollMove(MapIndex); }));
	//	MepSlot->SetVisibility(ESlateVisibility::Visible);

	//}
}


void UB2UIGuildMapMain::SetGuildInfo()
{
	auto& ClientData = BladeIIGameImpl::GetClientDataStore();

	auto MyGuildInfo = ClientData.GetMyGuildBattleInfo();
	auto OtherGuildInfo = ClientData.GetOtherGuildBattleInfo();


	if (UIP_GuildMapEntryDirect.IsValid())
	{

		UIP_GuildMapEntryDirect->SetMyGuildInfo(MyGuildInfo);
		UIP_GuildMapEntryDirect->SetOtherGuildInfo(OtherGuildInfo);

		auto GuildDoc = UB2UIDocHelper::GetDocGuild();

		if (GuildDoc)
		{
			if (ChangeGuildBattleId(GuildDoc->GetGuildbattleStateNew().season_id, GuildDoc->GetGuildbattleStateNew().turn))
			{
				UIP_GuildMapEntryDirect->SpecialPlayAnimation();
				SpecialVisibleAni();
			}
			else
			{
				UIP_GuildMapEntryDirect->BasePlayAnimation();
				PlayVisibleAni();
			}
		}
	}
}

#define SETGUILDCASTLE_INFO(Arr1, Arr2, Arr3, Arr4)												\
	Arr1.Empty();	Arr2.Empty();		Arr3.Empty();	Arr4.Empty();							\
	Arr1.SetNum(5);	Arr2.SetNum(5);		Arr3.SetNum(5);	Arr4.SetNum(5);							\
	for (TActorIterator<AB2GuildCastleActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)		\
	{																							\
		AB2GuildCastleActor* GuildCastleActor = *ActorItr;										\
		int32 TagNumber = FCString::Atoi(*GuildCastleActor->Tags[0].ToString());				\
		if(TagNumber < 0)																		\
			Arr3[(TagNumber * -1) -1] = GuildCastleActor;										\
		else if (TagNumber < 10)																\
			Arr1[TagNumber] = GuildCastleActor;													\
		else if(TagNumber < 20)																	\
			Arr2[TagNumber%10] = GuildCastleActor;												\
		else																					\
			Arr4[TagNumber % 20] = GuildCastleActor;											\
	}																							\
		

void UB2UIGuildMapMain::MedalMoveUI()
{
	if (!MatineeState)
		return;

	//3D谅钎肺 框流捞扁
	TArray<AB2GuildCastleActor*> BeforeFirstCastleInfo;
	TArray<AB2GuildCastleActor*> FirstCastleInfo;
	TArray<AB2GuildCastleActor*> ScondCastleInfo;
	TArray<AB2GuildCastleActor*> AfterSecondCastleInfo;


	

	SETGUILDCASTLE_INFO(FirstCastleInfo, ScondCastleInfo, BeforeFirstCastleInfo, AfterSecondCastleInfo);

	SetMedalMoveUI(BeforeFirstCastleInfo, ArrUIGuildInfoMedal1_1);
	SetMedalMoveUI(FirstCastleInfo, ArrUIGuildInfoMedal1);
	SetMedalMoveUI(ScondCastleInfo, ArrUIGuildInfoMedal2);
	SetMedalMoveUI(AfterSecondCastleInfo, ArrUIGuildInfoMedal2_1);

}

void UB2UIGuildMapMain::SetMedalMoveUI(TArray<class AB2GuildCastleActor*>& InCasleInfo, TArray<class UB2UIGuildInfoMedalCastlePart*>& InMedalUI)
{
	APlayerController* OwningPC = GetOwningPlayer();
	FVector2D vProjectedLocation;
	FVector		CastlePos;

	int32 i = 0;
	for (auto& El : InCasleInfo)
	{
		CastlePos = El->GetActorLocation();
		CastlePos.Y += CastleSlotYPos;
		if (OwningPC->ProjectWorldLocationToScreen(CastlePos, vProjectedLocation))
		{
			InMedalUI[i]->ForceLayoutPrepass();

			vProjectedLocation.X -= ArrUIGuildInfoMedal1[i]->GetDesiredSize().X / 2;
			vProjectedLocation.Y -= (ArrUIGuildInfoMedal1[i]->GetDesiredSize().Y / 2) + 30;


			InMedalUI[i]->SetPositionInViewport(vProjectedLocation);
			//El->SetActorHiddenInGame(true);
		}

		++i;
	}
}

void UB2UIGuildMapMain::WorldMapUIUpdate(int32 Lv)
{

	/*if (Lv != 0 / *&& NowScrollLv == Lv* /)
		return;*/

	if (Lv < 0)
		Lv = 0;

	auto& ClientData = BladeIIGameImpl::GetClientDataStore();
	b2network::B2GuildBattleGuildPtr GuildInfo;

	MyGuildInfoState ? GuildInfo = ClientData.GetMyGuildBattleInfo() : GuildInfo = ClientData.GetOtherGuildBattleInfo();

	TArray<AB2GuildCastleActor*> BeforeFirstCastleInfo;
	TArray<AB2GuildCastleActor*> FirstCastleInfo;
	TArray<AB2GuildCastleActor*> ScondCastleInfo;
	TArray<AB2GuildCastleActor*> AfterSecondCastleInfo;

	SETGUILDCASTLE_INFO(FirstCastleInfo, ScondCastleInfo, BeforeFirstCastleInfo, AfterSecondCastleInfo);

	GuildInfo->members.Sort([](b2network::B2GuildBattleMemberPtr Ele1, b2network::B2GuildBattleMemberPtr Ele2)->bool {
		if (Ele1->power_index < Ele2->power_index)
			return true;
		else
			return false;
	});

	float OffSet = SB_MapScroll->GetScrollOffset();

	const FVector2D ScreenSize = GEngine->GameViewport->Viewport->GetSizeXY();

	int32 ScrollLv11 = FMath::RoundToInt((OffSet / ScreenSize.Y));


	int32 memberIndex = (Lv - (Lv%2)) * MaxMapSlotCount;
	int32 RivisionMemberIndex = ScrollLv11 % 2 == 0 ? memberIndex - MaxMapSlotCount : memberIndex + (MaxMapSlotCount*2) ;

	int32 i = 0;

	if (ArrUIGuildInfoMedal1.Num() != FirstCastleInfo.Num() || ScondCastleInfo.Num() != ArrUIGuildInfoMedal1.Num())
		return;

	auto* Gamemode = Cast<AB2LobbyGameMode>(GetWorld()->GetAuthGameMode());

	Gamemode->SetGuildEffectsLocation(static_cast<int32>(EUIGuildSelectEffect::End), FVector(0, 0, 0));
	if (!Gamemode)
		return;

	SetWorldMapUIUpdate(RivisionMemberIndex, BeforeFirstCastleInfo, ArrUIGuildInfoMedal1_1);
	SetWorldMapUIUpdate(memberIndex, FirstCastleInfo, ArrUIGuildInfoMedal1);
	SetWorldMapUIUpdate(memberIndex + MaxMapSlotCount, ScondCastleInfo, ArrUIGuildInfoMedal2);
	SetWorldMapUIUpdate(RivisionMemberIndex, AfterSecondCastleInfo, ArrUIGuildInfoMedal2_1);

	if (bUIHideState)
	{
		UIHidden(true);
		return;
	}

	ScrollLv = Lv;

}

void UB2UIGuildMapMain::SetWorldMapUIUpdate(int32 InIndex, TArray<AB2GuildCastleActor*>& InCasleInfo, TArray<UB2UIGuildInfoMedalCastlePart*>& InMedalUI)
{

	auto& ClientData = BladeIIGameImpl::GetClientDataStore();
	b2network::B2GuildBattleGuildPtr GuildInfo;

	MyGuildInfoState ? GuildInfo = ClientData.GetMyGuildBattleInfo() : GuildInfo = ClientData.GetOtherGuildBattleInfo();

	auto* Gamemode = Cast<AB2LobbyGameMode>(GetWorld()->GetAuthGameMode());

	if (!Gamemode)
		return;

	int32 i = 0;
	for (auto& El : InCasleInfo)
	{

		if (0 > InIndex + i ||
			GuildInfo->members.Num() <= InIndex + i)
		{
			InMedalUI[i]->SetVisibility(ESlateVisibility::Hidden);
			El->SetActorHiddenInGame(true);
			++i;
			continue;
		}
		else
		{
			El->SetActorHiddenInGame(false);
			InMedalUI[i]->SetVisibility(ESlateVisibility::Visible);
		}
		InMedalUI[i]->SetGuildName(MyGuildInfoState, GuildInfo->members[InIndex + i]->power_index, GuildInfo->members[InIndex + i]->nickname);
		InMedalUI[i]->SetMedalCount(GuildInfo->members[InIndex + i]->left_medal);
		El->SetGuildInfo(InIndex + i, MyGuildInfoState);


		if (SelectIndex == InIndex + i)
			Gamemode->SetGuildEffectsLocation(static_cast<int32>(MyGuildInfoState ? EUIGuildSelectEffect::Team : EUIGuildSelectEffect::Rival), El->GetActorLocation());

		if (BladeIIGameImpl::GetClientDataStore().GetAccountId() == GuildInfo->members[InIndex + i]->account_id && MyGuildInfoState)
			Gamemode->SetGuildEffectsLocation(static_cast<int32>(EUIGuildSelectEffect::MyCastle), El->GetActorLocation());


		++i;
	}

}

void UB2UIGuildMapMain::SetCastleUISetting(TArray<AB2GuildCastleActor*>& InCasleInfo, TArray<UB2UIGuildInfoMedalCastlePart*>& InMedalUI, int32 InPage)
{
	int32 Count = 0;							
	FVector2D vProjectedLocation;
	APlayerController* OwningPC = GetOwningPlayer();

	auto& ClientData = BladeIIGameImpl::GetClientDataStore();
	b2network::B2GuildBattleGuildPtr GuildInfo;

	MyGuildInfoState ? GuildInfo = ClientData.GetMyGuildBattleInfo() : GuildInfo = ClientData.GetOtherGuildBattleInfo();

	for (auto& El : InCasleInfo)
	{																						
		auto* MedalSlot = CreateWidget<UB2UIGuildInfoMedalCastlePart>(GetWorld(), GuildInfoMedal);	
		if (!MedalSlot)																			
			return;		

		MedalSlot->AddToViewport(0);														
		MedalSlot->Init();		

		FVector CastlePos = El->GetActorLocation();												
		CastlePos.Y += CastleSlotYPos;															
		int32 IndexInfo = 0;																	
		InMedalUI.Add(MedalSlot);
		if (OwningPC->ProjectWorldLocationToScreen(CastlePos, vProjectedLocation))				
		{																						
			IndexInfo = ((ScrollLv + InPage)* MaxMapSlotCount) + Count;

			if(!GuildInfo->members.IsValidIndex(IndexInfo)) 
			{
				El->SetActorHiddenInGame(true);												
				MedalSlot->SetVisibility(ESlateVisibility::Hidden); 
				++Count; 
				continue;
			}

			MedalSlot->ForceLayoutPrepass();												
			vProjectedLocation.X -= MedalSlot->GetDesiredSize().X / 2;						
			vProjectedLocation.Y -= (MedalSlot->GetDesiredSize().Y / 2);					
			MedalSlot->SetPositionInViewport(vProjectedLocation);							
			MedalSlot->SetGuildName(MyGuildInfoState, IndexInfo + 1, GuildInfo->members[IndexInfo]->nickname);
			MedalSlot->SetMedalCount(GuildInfo->members[IndexInfo]->left_medal);			
			El->SetGuildInfo(IndexInfo, MyGuildInfoState);									
		}																						
		++Count;																			
	}																							
}

void UB2UIGuildMapMain::SetCastleInfo()
{
	if (ArrUIGuildInfoMedal1.Num())
		return;

	auto& ClientData = BladeIIGameImpl::GetClientDataStore();
	b2network::B2GuildBattleGuildPtr GuildInfo;

	MyGuildInfoState ? GuildInfo = ClientData.GetMyGuildBattleInfo() : GuildInfo = ClientData.GetOtherGuildBattleInfo();

	ArrUIGuildInfoMedal1_1.Empty();
	ArrUIGuildInfoMedal2_1.Empty();
	ArrUIGuildInfoMedal1.Empty();
	ArrUIGuildInfoMedal2.Empty();

	TArray<AB2GuildCastleActor*> BeforeFirstCastleInfo;
	TArray<AB2GuildCastleActor*> FirstCastleInfo;
	TArray<AB2GuildCastleActor*> ScondCastleInfo;
	TArray<AB2GuildCastleActor*> AfterSecondCastleInfo;



	SETGUILDCASTLE_INFO(FirstCastleInfo, ScondCastleInfo, BeforeFirstCastleInfo , AfterSecondCastleInfo)

	SetCastleUISetting(BeforeFirstCastleInfo, ArrUIGuildInfoMedal1_1, -1);
	SetCastleUISetting(FirstCastleInfo, ArrUIGuildInfoMedal1, 0);
	SetCastleUISetting(ScondCastleInfo, ArrUIGuildInfoMedal2, 1);
	SetCastleUISetting(AfterSecondCastleInfo, ArrUIGuildInfoMedal2_1, 2);

	UIHidden(true);
}


void UB2UIGuildMapMain::UpdateQuipSlot()
{
	
	if (!SB_MapScroll.Get())
		return;

	float OffSet = SB_MapScroll->GetScrollOffset();

	const FVector2D ScreenSize = GEngine->GameViewport->Viewport->GetSizeXY();

	int32 Lv = FMath::RoundToInt((OffSet / ScreenSize.Y));


	if (NowScrollLv == Lv)
		return;

	UB2UIGuildMapSlot* TargetSlot = Cast<UB2UIGuildMapSlot>(VB_MapQuipSlot->GetChildAt(NowScrollLv));

	if (TargetSlot)
		TargetSlot->OffSelectMap();
	
	TargetSlot = Cast<UB2UIGuildMapSlot>(VB_MapQuipSlot->GetChildAt(Lv));

	if (TargetSlot)
		TargetSlot->OnSelectMap();


	WorldMapUIUpdate(Lv);


	NowScrollLv = Lv;


	return;
}

void UB2UIGuildMapMain::SetGuildChangeText()
{
	if (MyGuildInfoState)
		TB_BTNOtherGuildText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_WeGuild")));
	else
		TB_BTNOtherGuildText->SetText(BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_OtherGuild")));
}

void UB2UIGuildMapMain::OnGuildRightGuildInfo(bool bMyGuild,const b2network::B2GuildBattleMemberPtr& GuildMember, const TArray<b2network::B2GuildBattleAttackPtr>& AttackInfo, int32 CastleIndex)
{
	UIP_GuildMapInfoPopup->SetVisibilityPopup(true);
	b2network::B2GuildBattleMemberPtr MyGuildCastleInfo = GetMyGuildMemberInfo();
	if(bMyGuild)
		UIP_GuildMapInfoPopup->OpenGuildInfo(GuildMember, AttackInfo);
	else if(MyGuildCastleInfo)
		UIP_GuildMapInfoPopup->OpenGuildInfo(MyGuildCastleInfo, GuildMember);
	else
		UIP_GuildMapInfoPopup->SetVisibilityPopup(false);

	BTN_FormationInfo->SetIsEnabled(MyGuildCastleInfo ? true : false);

	SelectIndex = CastleIndex;
}

void UB2UIGuildMapMain::QuickScrollMove(int32 MapIndex)
{
	//if (BTN_OtherGuild->GetVisibility() == ESlateVisibility::HitTestInvisible)
	//	return;

	//const FVector2D ScreenSize = GEngine->GameViewport->Viewport->GetSizeXY();

	//float NewOffset = (float(MapIndex) * ScreenSize.Y);

	//SB_MapScroll->SetNatualScrollOffSet(NewOffset);
}

void UB2UIGuildMapMain::ChangeGuildCastleUI()
{
	if (MyGuildInfoState)
		MyGuildInfoState = false;
	else
		MyGuildInfoState = true;

	MatineeState = true;
	ScrollLv = SelectIndex = INDEX_NONE;
	InitOpenGuildMap();
	WorldMapUIUpdate(0);
	UIHidden(true);


	auto& ClientData = BladeIIGameImpl::GetClientDataStore();
	b2network::B2GuildBattleGuildPtr GuildInfo = ClientData.GetMyGuildBattleInfo();
	b2network::B2GuildBattleMemberPtr MyGuildInfo = GetMyGuildMemberInfo();

	int32 TagetSlotIndex = 0;
	if (MyGuildInfo)
	{
		MyGuildCastleIndex = MyGuildInfo->power_index;
		TagetSlotIndex = FMath::TruncToInt((static_cast<float>(MyGuildCastleIndex) - 0.1f) / 5.f);
	}

	UB2UIGuildMapSlot* TargetSlot = Cast<UB2UIGuildMapSlot>(VB_MapQuipSlot->GetChildAt(TagetSlotIndex));

	if (TargetSlot && MyGuildInfoState)
		TargetSlot->SetMyCastlePos(true);
}

void UB2UIGuildMapMain::UIHidden(bool State)
{
	int32 UINumber = ArrUIGuildInfoMedal1.Num();

	if (UINumber != ArrUIGuildInfoMedal2.Num())
		return;

	bUIHideState = State;

	if (State == true)
	{
		for (int32 i = 0; i < UINumber; ++i)
		{
			ArrUIGuildInfoMedal1[i]->SetVisibility(ESlateVisibility::Hidden);
			ArrUIGuildInfoMedal2[i]->SetVisibility(ESlateVisibility::Hidden);
			ArrUIGuildInfoMedal1_1[i]->SetVisibility(ESlateVisibility::Hidden);
			ArrUIGuildInfoMedal2_1[i]->SetVisibility(ESlateVisibility::Hidden);


			/*if (!State)
				WorldMapUIUpdate(0);*/
		}
	}
	else
		WorldMapUIUpdate(0);


	if (BTN_OtherGuild.IsValid())
		BTN_OtherGuild->SetVisibility(State ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);

	if (SB_MapScroll.IsValid())
		SB_MapScroll->SetVisibility(State ? ESlateVisibility::HitTestInvisible : ESlateVisibility::SelfHitTestInvisible);
}

void UB2UIGuildMapMain::EnterMatineePlay(class ALevelSequenceActor * EnterA, class ALevelSequenceActor * EnterB)
{
	//EnterA->SetPosition(0.f);
	//EnterA->Play();
	//ChangeGuildCastleUI();



	//PlayVisibleAni();
}

void UB2UIGuildMapMain::GuildBattleTimeTick()
{
	//if (GuildBattleEndTime != INDEX_NONE)
	//{
	//	FDateTime Time = FDateTime::UtcNow();

	//	int64 RemainTime = GuildBattleEndTime - (Time.ToUnixTimestamp() * 1000);

	//	FTimespan RemainTime2 = FDateTime::UtcNow() - FDateTime::FromUnixTimestamp(GuildBattleEndTime / 1000);

	//	int32 RemainDay = RemainTime2.GetDays();
	//	int32 RemainHour = RemainTime2.GetHours();
	//	int32 RemainMin = RemainTime2.GetMinutes();

	//	if (RemainTime <= 0)
	//	{
	//		RemainTime = 0;
	//		GuildBattleEndTime = INDEX_NONE;

	//		UB2UIManager::GetInstance()->OpenMsgPopup(EUIMsgPopup::Simple,
	//			BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Chatting_MessageHeader_PublicNoti")), FText::Format(
	//				BladeIIGetLOCText(B2LOC_CAT_GENERAL, TEXT("Guild_BattleEnd")), FText::FromString(BladeIIGameImpl::GetClientDataStore().GetMyGuildInfo().GuildInfo->guild_name)), 0, true, true, EUIMsgPopupButtonGroup::Confirm
	//			, FMsgPopupOnClick::CreateLambda([this]() {
	//			LobbyChangeSceneByUISceneClass<EUIScene>::GetInstance().Signal(EUIScene::GuildMain); }
	//			));

	//	}

	//	TB_Time->SetText(FText::FromString(UB2UIManager::FormatSecondsAsTime(RemainTime / 1000, TEXT("hh:mm:ss"))));
	//}
}

void UB2UIGuildMapMain::RefreshInfo()
{
	UpdateQuipSlot();
	SetCastleInfo();
	WorldMapUIUpdate(ScrollLv);

	auto& ClientData = BladeIIGameImpl::GetClientDataStore();

	auto MyGuildInfo = ClientData.GetMyGuildBattleInfo();
	auto OtherGuildInfo = ClientData.GetOtherGuildBattleInfo();


	if (UIP_GuildMapEntryDirect.IsValid())
	{

		UIP_GuildMapEntryDirect->SetMyGuildInfo(MyGuildInfo);
		UIP_GuildMapEntryDirect->SetOtherGuildInfo(OtherGuildInfo);
	}

}

void UB2UIGuildMapMain::RequestBattleInfo()
{
	data_trader::Retailer::GetInstance().RequestGetGuildBattleInfo();
}

bool UB2UIGuildMapMain::IsPagePopup()
{
	auto SimplePopup = UB2UIManager::GetInstance()->GetUI<UB2UIMsgPopupSimple>(UIFName::MsgPopupSimple);
	auto ChapPopup = UB2UIManager::GetInstance()->GetUI<UB2UIChatting>(UIFName::Chatting);

	bool PopupState = false;

	if (SimplePopup && SimplePopup->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
		PopupState = true;

	if (ChapPopup && ChapPopup->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
		PopupState = true;


	if (UIP_GuildMapInfoPopup->GetVisibility() == ESlateVisibility::Hidden &&
		GuildBattleSituationPopup->GetVisibility() == ESlateVisibility::Hidden &&
		!PopupState)
		return false;

	return true;

}

b2network::B2GuildBattleMemberPtr UB2UIGuildMapMain::GetMyGuildMemberInfo()
{
	auto& ClientData = BladeIIGameImpl::GetClientDataStore();
	b2network::B2GuildBattleGuildPtr GuildInfo = ClientData.GetMyGuildBattleInfo();

	b2network::B2GuildBattleMemberPtr MyGuildInfo;

	for (auto El : GuildInfo->members)
	{
		if (ClientData.GetAccountId() == El->account_id)
		{
			return El;
		}
	}

	return nullptr;
}

void UB2UIGuildMapMain::OnClickBTN_FormationInfo()
{

	auto& ClientData = BladeIIGameImpl::GetClientDataStore();
	
	b2network::B2GuildBattleGuildPtr GuildInfo = ClientData.GetMyGuildBattleInfo();
	b2network::B2GuildBattleMemberPtr MyGuildInfo = GetMyGuildMemberInfo();


	MyGuildCastleIndex = MyGuildInfo->power_index;
	TArray<b2network::B2GuildBattleAttackPtr> AttackInfo;
	for (auto El : GuildInfo->attacks)
	{
		if (El->account_id == BladeIIGameImpl::GetClientDataStore().GetAccountId())
		{
			AttackInfo.Add(El);
		}
	}

	UIP_GuildMapInfoPopup->SetVisibilityPopup(true);
	UIP_GuildMapInfoPopup->OpenGuildInfo(MyGuildInfo, AttackInfo);
}

void UB2UIGuildMapMain::OnClickBTN_BattleList()
{
	if (GuildBattleSituationPopup.IsValid())
	{
		GuildBattleSituationPopup->SetVisibility(ESlateVisibility::Visible);
		GuildBattleSituationPopup->OnOpen(true);
	}
}

void UB2UIGuildMapMain::OnClickBTN_OtherGuild()
{

	//UIHidden(true);

	//if (BTN_OtherGuild.IsValid())
	//	BTN_OtherGuild->SetVisibility(ESlateVisibility::HitTestInvisible);

	//if (SB_MapScroll.IsValid())
	//	SB_MapScroll->SetVisibility(ESlateVisibility::HitTestInvisible);


	//GuildStateChangeEventClass<int32>::GetInstance().Signal(NowScrollLv);

	//MyGuildInfoState ? MyGuildIndex = NowScrollLv : OtherGuildIndex = NowScrollLv;


	//PlayHiddenAni();
	//SetClose(false);
}

void UB2UIGuildMapMain::OnClickBTN_Back()
{
	if (UB2UIManager* UIManager = UB2UIManager::GetInstance())
	{
		EUIScene PrevUIScene = UIManager->GetCurrUIScene();

		//PlaySoundOnBackBtn(PrevUIScene);

		//UIScene History俊 狼秦 磊悼栏肺 Back等促. DJLegacy HeroMgmt 葛靛牢 版快档 贸府瞪 巴.
		data_trader::Retailer::GetInstance().RequestGuildDetailInfo(0);
		UIManager->ChangeUISceneBack();
	}

}

void UB2UIGuildMapMain::OnClickBTN_Chat()
{
	if (UB2UIManager* UIMgrInst = UB2UIManager::GetInstance())
		UIMgrInst->OpenUI(UIFName::Chatting);
}

void UB2UIGuildMapMain::OnClickBTN_QuipUp()
{
	int32 UpIndex = NowScrollLv - 1  < 0 ? 0 : NowScrollLv - 1;

	QuickScrollMove(UpIndex);
}

void UB2UIGuildMapMain::OnClickBTN_QuipDown()
{
	int32 UpIndex = NowScrollLv + 1 > SlotMax ? SlotMax : NowScrollLv + 1;

	QuickScrollMove(UpIndex);
}

void UB2UIGuildMapMain::OpenAnimFinish()
{
	SetClose(true);
}
