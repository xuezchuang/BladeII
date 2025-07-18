
#include "B2UIHeroMgntRotateEnableArea.h"
//#include "BladeII.h"
#include "B2LobbyGameMode.h"

FReply UB2UIHeroMgntRotateEnableArea::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (LobbyGameMode)
		LobbyGameMode->SetTouchingRotateAreaInHeroMgnt(true);

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UB2UIHeroMgntRotateEnableArea::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (LobbyGameMode)
		LobbyGameMode->SetTouchingRotateAreaInHeroMgnt(false);

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UB2UIHeroMgntRotateEnableArea::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (LobbyGameMode)
		LobbyGameMode->SetTouchingRotateAreaInHeroMgnt(false);

	return Super::NativeOnMouseLeave(InMouseEvent);
}

FReply UB2UIHeroMgntRotateEnableArea::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (LobbyGameMode)
		LobbyGameMode->SetTouchingRotateAreaInHeroMgnt(true);

	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

FReply UB2UIHeroMgntRotateEnableArea::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	auto* LobbyGameMode = Cast<AB2LobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (LobbyGameMode)
		LobbyGameMode->SetTouchingRotateAreaInHeroMgnt(false);

	return Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
}