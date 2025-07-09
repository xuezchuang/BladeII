// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeII.h"
#include "BladeIIUserWidget.h"
#include "B2LobbyUISwitcher.h"
#include "B2UIManager_Lobby.h"
#include "B2LobbyUserWidget.h"
#include "B2LobbyGameMode.h"
#include "BladeIIGameImpl.h"
#include "B2DynItemIcon.h"

UB2LobbyUISwitcher::UB2LobbyUISwitcher(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UB2LobbyUISwitcher::InitUISwitcher(class AB2LobbyGameMode* InLobbyGM)
{
	SetupCacheOnLoadList();
	SetupRoosetPagesList();
	SetupLoadOnStartupPagesList(InLobbyGM ? InLobbyGM->DJLegacy_GetLobbyUIManager() : nullptr);

#if WITH_EDITOR
	if (GIsEditor)
	{
		// BladeIIGameImpl::StartupModule ���� �ε��ϴ� �͵鵵 �ִµ� �̰� ���⼭ �ε��ص� �� ��. 
		// Ȥ�ö� ������ ����� LobbyUISwitcher ��ü�� �ٸ� ������ ���� �� �ְ� �ϰ� BladeIIGameImpl �ʿ��� EditorLoadAll �� �� ��
		EditorLoadAll();
	}
#endif
}

void UB2LobbyUISwitcher::SetupCacheOnLoadList()
{
#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS �޸� ����� ����..
#if WITH_EDITOR
	if (GIsEditor) {
		return; // �����͵� �̷� �� �ʿ����.
	}
#endif

	//TArray<ELobbyUIPages> CacheOnLoadList;
	//TArray<FString> ParsedCacheOnLoadListStr;
	//GConfig->GetArray(TEXT("/Script/BladeII.B2UIManager"), TEXT("CacheOnLoadUI_DJLegacy"), ParsedCacheOnLoadListStr, GGameIni);
	//// ��� Enum ���� ��ȯ�� �ؾ�..
	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, ELobbyUIPages_ENUM_PACKAGE_NAME, true);
	//if (EnumPtr)
	//{
	//	for (const FString& ThisUIName : ParsedCacheOnLoadListStr)
	//	{
	//		ELobbyUIPages CastedUIPageEnum = (ELobbyUIPages)(EnumPtr->GetValueByName(FName(*ThisUIName)));
	//		if (CastedUIPageEnum != ELobbyUIPages::ELUP_End)
	//		{
	//			CacheOnLoadList.AddUnique(CastedUIPageEnum);
	//		}
	//	}
	//}

	//for (ELobbyUIPages ThisPageEnum : CacheOnLoadList)
	//{
	//	for (int32 CI = 0; CI < LobbyPageClassMapping.Num(); ++CI)
	//	{
	//		FLobbyUIPageClassMapping& ThisMapping = LobbyPageClassMapping[CI];
	//		if (ThisMapping.PageEnum == ThisPageEnum)
	//		{
	//			// UIManager ���� ������ ��ɰ� ���������� �ᱹ ��� �ϳ� �������ڰ� �� ���� ��.
	//			// UB2UIManager::SetupCacheOnLoadList ����.
	//			ThisMapping.SetCacheOnLoad(true);
	//			break;
	//		}
	//	}
	//}
#endif
}

static void FromStringToLobbyUIPageEnum(const TArray<FString>& inUIPageStringArray, TArray<ELobbyUIPages>& OutUIPageEnum)
{
	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, ELobbyUIPages_ENUM_PACKAGE_NAME, true);
	//if (EnumPtr)
	//{
	//	for (const FString& ThisUIName : inUIPageStringArray)
	//	{
	//		ELobbyUIPages CastedUIPageEnum = (ELobbyUIPages)(EnumPtr->GetValueByName(FName(*ThisUIName)));
	//		if (CastedUIPageEnum != ELobbyUIPages::ELUP_End)
	//		{
	//			OutUIPageEnum.AddUnique(CastedUIPageEnum);
	//		}
	//	}
	//}
}

void UB2LobbyUISwitcher::SetupRoosetPagesList()
{
#if !PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS �޸� ����� ����..
	if (RootSetPages.Num() == 0)
	{
		TArray<FString> RootsetPageListStr;
		GConfig->GetArray(TEXT("/Script/BladeII.B2UIManager"), TEXT("RootSetOnLoadUI_DJLegacy"), RootsetPageListStr, GGameIni);
		// ��� Enum ���� ��ȯ�� �ؾ�..
		FromStringToLobbyUIPageEnum(RootsetPageListStr, RootSetPages);
	}
#endif
}
void UB2LobbyUISwitcher::SetupLoadOnStartupPagesList(UB2UIManager_Lobby* InOwnerUIManager)
{
#if	PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS �޸� ����� ����..
	return;
#endif

	if (LoadOnStartupPages.Num() > 0)
		return;

	TArray<FString> UIPageEnums;
	GConfig->GetArray(TEXT("/Script/BladeII.B2UIManager"), TEXT("LoadOnStartupUI_DJLegacy"), UIPageEnums, GGameIni);
	FromStringToLobbyUIPageEnum(UIPageEnums, LoadOnStartupPages);

	for(auto PageEnum : LoadOnStartupPages)
		GetWidgetClass(PageEnum);
}

// UB2UIManager::IsRootSetWidgetName �� �ش�.
TArray<ELobbyUIPages> UB2LobbyUISwitcher::RootSetPages;
TArray<ELobbyUIPages> UB2LobbyUISwitcher::LoadOnStartupPages;
bool UB2LobbyUISwitcher::IsRootSetPage(ELobbyUIPages InCheckPage)
{
	return RootSetPages.Contains(InCheckPage) || LoadOnStartupPages.Contains(InCheckPage);
}

TSubclassOf<class UB2LobbyUserWidget> UB2LobbyUISwitcher::GetWidgetClass(ELobbyUIPages UIPage)
{
	auto ClassAsset = GetWidgetClassAsset(UIPage);
	return LoadSynchronous(ClassAsset);
}

TSoftClassPtr<class UB2LobbyUserWidget> UB2LobbyUISwitcher::GetWidgetClassAsset(ELobbyUIPages UIPage)
{
	if (auto* PageInfo = GetUIPageMapping(UIPage))
		return PageInfo->PageWidgetClass;

	return nullptr;
}

TSubclassOf<class UB2LobbyUserWidget> UB2LobbyUISwitcher::GetSubPoupClass(ELobbySubPopups SubPopupEnum)
{
	auto ClassAsset = GetSubPopupClassAsset(SubPopupEnum);
	return LoadSynchronous(ClassAsset);
}

TSoftClassPtr<class UB2LobbyUserWidget> UB2LobbyUISwitcher::GetSubPopupClassAsset(ELobbySubPopups SubPopupEnum)
{
	if (auto* SubPopupInfo = GetSubPopupMapping(SubPopupEnum))
		return SubPopupInfo->SubPopupClass;

	return nullptr;
}

int32 UB2LobbyUISwitcher::GetSubPopupZOrder(ELobbySubPopups SubPopup)
{
	static const int32 DefaultPopupZOrder = 5;
	int32 PopupZOrder = DefaultPopupZOrder;

	if (auto* PopupInfo = GetSubPopupMapping(SubPopup))
		PopupZOrder = PopupInfo->ZOrder;

	return PopupZOrder;
}

FLobbyUIPageClassMapping* UB2LobbyUISwitcher::GetUIPageMapping(ELobbyUIPages InPageEnum)
{
	for(auto& PageInfo : LobbyPageClassMapping)
	{
		if (PageInfo.PageEnum == InPageEnum)
			return &PageInfo;
	}

	return nullptr;
}

FLobbySubPopupClassMapping* UB2LobbyUISwitcher::GetSubPopupMapping(ELobbySubPopups InSubPopupEnum)
{
	for (auto& SubPopupInfo : LobbySubPopupClassMapping)
	{
		if (SubPopupInfo.SubPopupEnum == InSubPopupEnum)
			return &SubPopupInfo;
	}

	return nullptr;
}

UClass* UB2LobbyUISwitcher::GetInventoryItemIconClass(bool bIsForEquipped)
{
	auto ClassAsset = bIsForEquipped ? InvenEquippedItemIconClass : InvenStoredItemIconClass;
	return LoadSynchronous(ClassAsset);
}

void UB2LobbyUISwitcher::UnloadTAssets(bool bUnloadeAllCached, UB2UIManager_Lobby* InOwnerUIManager)
{
	// 1. UIPage Class Asset Unload
	for (auto& PageInfo : LobbyPageClassMapping)
	{
		const bool bUnloadClassAsset = bUnloadeAllCached || (PageInfo.ShouldCacheOnLoad() == false);

		if (bUnloadClassAsset)
		{
			UnloadAsset(PageInfo.PageWidgetClass.ToSoftObjectPath());
			InOwnerUIManager->RemoveCachedWidgetClasse(PageInfo.PageEnum);
		}

		if (bUnloadeAllCached)
			PageInfo.SetCacheOnLoad(false);
	}

	// 2. Sub Popup Class Asset Unload
	for (auto& SubPopupInfo : LobbySubPopupClassMapping)
		UnloadAsset(SubPopupInfo.SubPopupClass.ToSoftObjectPath());

	// 3. Cached ClassAsset Unload
	if (InOwnerUIManager && bUnloadeAllCached)
	{
		// UIPage Class Asset ���۷��� ����
		InOwnerUIManager->RemoveCachedWidgetClasses();

		// �� Class Asset Unlad �� ���۷��� ����
		UnloadAsset(InvenStoredItemIconClass.ToSoftObjectPath());
		UnloadAsset(InvenEquippedItemIconClass.ToSoftObjectPath());
		InOwnerUIManager->RemoveCachedSubWidgetClasses();
	}
}

#if WITH_EDITOR
void UB2LobbyUISwitcher::EditorLoadAll()
{
	for (auto& PageInfo : LobbyPageClassMapping)
		GetWidgetClass(PageInfo.PageEnum);
	
	for (auto& SubPopupInfo : LobbySubPopupClassMapping)
		GetSubPoupClass(SubPopupInfo.SubPopupEnum);
}
#endif

FString GetUIPageEnumString(ELobbyUIPages UIPageEnum)
{
	//if (const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, ELobbyUIPages_ENUM_PACKAGE_NAME, true))
	//	return EnumPtr->GetNameByValue(static_cast<int32>(UIPageEnum)).ToString();

	return TEXT("");
}
