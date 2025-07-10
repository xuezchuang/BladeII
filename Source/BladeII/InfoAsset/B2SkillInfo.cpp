// Fill out your copyright notice in the Description page of Project Settings.
// Project BladeII, Action Square


#include "B2SkillInfo.h"
#include "Materials/Material.h"
#include "Animation/AnimSequenceBase.h"
#include "BladeIIUtil.h"
#include "BladeIIGameImpl.h"
//#include "B2SkillRTManager.h"
#if WITH_EDITOR
#include "FB2ErrorMessage.h"
#endif

FSingleSkillInfo::FSingleSkillInfo()
{
	SkillID						= SKILL_INVALID_ID;
	CoolTime					= 0.0f;	
	WeaponSkillType				= EWeaponSkillType::EWS_None;
	EnableWeaponSkillPoint		= 8;
	SkillShootingRangeForAI		= 1000000.0f;
}

#if WITH_EDITOR
void FSingleSkillInfo::EditorOnPostLoad()
{ // 다른 몇몇 TAsset 들처럼 에디터에선 다 로딩해 줌..
	ButtonIcon_Normal.LoadSynchronous();
	ButtonIcon_Hovered.LoadSynchronous();
	ButtonIcon_Pressed.LoadSynchronous();
	ButtonIcon_Disabled.LoadSynchronous();
}
#endif

UB2SkillInfo::UB2SkillInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

#define CharSkillArrayToMap(CharClassName)	\
	for (int32 i = 0; i < SkillInfoArray_##CharClassName.Num(); ++i)											\
	{																											\
		SkillInfoArray_##CharClassName[i].UserClass = EPCClass::EPC_##CharClassName;							\
		SkillInfoMap.Add(SkillInfoArray_##CharClassName[i].SkillID, &SkillInfoArray_##CharClassName[i]);		\
	}

void UB2SkillInfo::PostLoad()
{
	CharSkillArrayToMap(Gladiator);
	CharSkillArrayToMap(Assassin);
	CharSkillArrayToMap(Wizard);
	CharSkillArrayToMap(Fighter);
	
	Super::PostLoad();

#if WITH_EDITOR
	if (GIsEditor)
	{
		for (FSingleSkillInfo& ThisSkillInfo : SkillInfoArray_Gladiator) {
			ThisSkillInfo.EditorOnPostLoad();
		}
		for (FSingleSkillInfo& ThisSkillInfo : SkillInfoArray_Assassin) {
			ThisSkillInfo.EditorOnPostLoad();
		}
		for (FSingleSkillInfo& ThisSkillInfo : SkillInfoArray_Wizard) {
			ThisSkillInfo.EditorOnPostLoad();
		}
		for (FSingleSkillInfo& ThisSkillInfo : SkillInfoArray_Fighter) {
			ThisSkillInfo.EditorOnPostLoad();
		}
	}
#endif
}

const FSingleSkillInfo* UB2SkillInfo::GetSingleInfoOfID(int32 SkillInfoID) const
{
	auto SkillInfo = SkillInfoMap.Find(SkillInfoID);

	return SkillInfo ? *SkillInfo : nullptr;
}

const FSingleSkillInfo* UB2SkillInfo::GetTagSkillInfo(EPCClass PCClass) const
{
	for (TMap<int32, FSingleSkillInfo*>::TConstIterator it(SkillInfoMap); it; ++it)
	{
		if (it.Value()->UserClass == PCClass && it.Value()->PassiveType == EPassiveType::EPassiveType_Tag)
			return it.Value();
	}

	return nullptr;
}

const FSingleSkillInfo* UB2SkillInfo::GetCounterSkillInfo(EPCClass PCClass) const
{
	for (TMap<int32, FSingleSkillInfo*>::TConstIterator it(SkillInfoMap); it; ++it)
	{
		if (it.Value()->UserClass == PCClass && it.Value()->PassiveType == EPassiveType::EPassiveType_Counter)
			return it.Value();
	}

	return nullptr;
}

const TMap<int32, FSingleSkillInfo*>& UB2SkillInfo::GetSkillInfoMap() const
{
	return SkillInfoMap;
}

TArray<ESkillOption> UB2SkillInfo::GetSpecialDisplaySkillOption(int32 SkillInfoID, int32 SkillLevel)
{
	TArray<ESkillOption> SpecialDisplayOptions;
	const FSingleSkillInfo* SingleSkillInfo = GetSingleInfoOfID(SkillInfoID);
	if (SingleSkillInfo)
	{
		for (auto& LevelOption : SingleSkillInfo->SpecialDisplaySkillOption)
		{
			if (SkillLevel == LevelOption.SkillLevel)
				SpecialDisplayOptions.Add(LevelOption.SkillOption);
		}
	}
		
	return SpecialDisplayOptions;
}

int32 UB2SkillInfo::GetNextSpecialDisplaySkillLevel(int32 SkillInfoID, int32 SkillLevel)
{
	int32 DisplaySkillLevel = INDEX_NONE;
	const FSingleSkillInfo* SingleSkillInfo = GetSingleInfoOfID(SkillInfoID);
	if (SingleSkillInfo)
	{
		for (auto& LevelOption : SingleSkillInfo->SpecialDisplaySkillOption)
		{
			if (SkillLevel + 1 < LevelOption.SkillLevel &&(DisplaySkillLevel == INDEX_NONE || DisplaySkillLevel > LevelOption.SkillLevel))
				DisplaySkillLevel = LevelOption.SkillLevel;
		}
	}

	return DisplaySkillLevel;
}

float UB2SkillInfo::GetOriginalCoolTime(int32 SkillInfoID) const
{
	const FSingleSkillInfo* SingleInfo = GetSingleInfoOfID(SkillInfoID);
	return SingleInfo ? SingleInfo->CoolTime : 0.0f;
}

float UB2SkillInfo::GetOriginalSkillShootingRangeForAI(int32 SkillInfoID) const
{
	const FSingleSkillInfo* SingleInfo = GetSingleInfoOfID(SkillInfoID);
	return SingleInfo ? SingleInfo->SkillShootingRangeForAI : 0.0f;
}

int32 UB2SkillInfo::GetEnablePlayerLevel(int32 SkillInfoID)
{
	return BladeIIGameImpl::GetClientDataStore().GetSkillLearnLevel(SkillInfoID);
}

FButtonStyle UB2SkillInfo::GetButtonIconStyle(int32 SkillInfoID)
{
	// LoadSynchronous 콜해야 해서 const 제거.. ㅡㅡ
	FSingleSkillInfo* SingleInfo = const_cast<FSingleSkillInfo*>(GetSingleInfoOfID(SkillInfoID));
	const bool bIsWeaponSkill = (SingleInfo && SingleInfo->WeaponSkillType != EWeaponSkillType::EWS_None);
	// 기본적인 Style 설정은 일괄적으로 들어가고 SingleSkillInfo 에 따라서는 아이콘만.
	FButtonStyle RetStyle = bIsWeaponSkill ? CommonButtonStyle_WeaponSkill : CommonButtonStyle_Regular;

	if (SingleInfo)
	{
		RetStyle.Normal.SetResourceObject(SingleInfo->ButtonIcon_Normal.LoadSynchronous());
		RetStyle.Hovered.SetResourceObject(SingleInfo->ButtonIcon_Hovered.LoadSynchronous());
		RetStyle.Pressed.SetResourceObject(SingleInfo->ButtonIcon_Pressed.LoadSynchronous());
		RetStyle.Disabled.SetResourceObject(SingleInfo->ButtonIcon_Disabled.LoadSynchronous());
	}

	return RetStyle;
}

EAttackState UB2SkillInfo::GetAttackState(int32 SkillInfoID) const
{
	const FSingleSkillInfo* SingleInfo = GetSingleInfoOfID(SkillInfoID);
	return SingleInfo ? SingleInfo->AttackAnimState : EAttackState::ECS_None;
}

bool UB2SkillInfo::IsTagSkill(int32 SkillInfoID) const
{
	const FSingleSkillInfo* SingleInfo = GetSingleInfoOfID(SkillInfoID);
	return SingleInfo ? SingleInfo->PassiveType == EPassiveType::EPassiveType_Tag : false;
}

float UB2SkillInfo::GetEnableWeaponSkillPoint(int32 SkillInfoID) const
{
	const FSingleSkillInfo* SingleInfo = GetSingleInfoOfID(SkillInfoID);
	return SingleInfo && SingleInfo->WeaponSkillType != EWeaponSkillType::EWS_None ? float(SingleInfo->EnableWeaponSkillPoint) : 0.0f;
}

#if WITH_EDITOR
void UB2SkillInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SkillInfoMap.Empty();

	CharSkillArrayToMap(Gladiator);
	CharSkillArrayToMap(Assassin);
	CharSkillArrayToMap(Wizard);
	CharSkillArrayToMap(Fighter);
}
#endif


////////////////////////////////////////////////////////////////////////////////
// SkillAnimInfo below..
//

FCombinedPCSkillAnimID FSkillAnimPerPCInfoSet::GetLoadedPtrMapKey(EPCClass InPCClass, ESkillAnimType InSkillAnimType)
{ // SkillAnim 데이터를 적재하는 key 자체를 FCombinedPCSkillAnimID 로 하지는 않고 로딩된 캐시 맵의 key 로만 사용.
	// FCombinedPCSkillAnimID 로 InfoAsset 에 등록하게 하면 그다지 보기 좋을 거 같지 않아서. 아 물론 그러려면 FCombinedPCSkillAnimID 를 BlueprintType 으로 바꿔야겠지.
	return FCombinedPCSkillAnimID(InPCClass, InSkillAnimType);
}

bool UB2SkillAnimInfo::bManagedSkillAnimUnload = true;

UB2SkillAnimInfo::UB2SkillAnimInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// ManagedSkillAnimUnload 에 대해 PCClassInfo 의 마찬가지 역할을 하는 변수와 동일한 ini 항목을 사용.
	if (GConfig)
	{
		GConfig->GetBool(TEXT("/Script/BladeII.B2PCClassInfo"), TEXT("ManagedPCClassUnload"), bManagedSkillAnimUnload, GGameIni);
	}
}

void UB2SkillAnimInfo::PostLoad()
{
	Super::PostLoad();
	EnsureDataIntegrity();

#if WITH_EDITOR 
	if (GIsEditor) // TMap 의 등록 특성 때문에 혹여나 잘못될 수 있는 설정 체크
	{
		int32 PCClassWrongSettingCount = 0;
		int32 SkillAnimTypeWrongSettingCount = 0;

		for (TMap<EPCClass, FSkillAnimPerPCInfoSet>::TIterator InfoDataIt(InfoData); InfoDataIt; ++InfoDataIt)
		{
			if (InfoDataIt.Key() == EPCClass::EPC_End) {
				++PCClassWrongSettingCount;
			}
			FSkillAnimPerPCInfoSet& ThisInfoSet = InfoDataIt.Value();
			for (TMap<ESkillAnimType, TSoftObjectPtr<UAnimSequenceBase>>::TIterator SkillAnimIt(ThisInfoSet.SkillAnims); SkillAnimIt; ++SkillAnimIt)
			{
				if ((SkillAnimIt.Key() == ESkillAnimType::ESA_End) || (SkillAnimIt.Key() == ESkillAnimType::ESA_Weapon_Normal_End)) {
					++SkillAnimTypeWrongSettingCount;
				}
			}
		}
		if (PCClassWrongSettingCount > 0 || SkillAnimTypeWrongSettingCount > 0)
		{
#if !PLATFORM_MAC
			FB2ErrorMessage::Open(EAppMsgType::Ok, FText::FromString(
				FString::Printf(TEXT("[SkillAnimInfo] 잘못된 PCClass (%d 개) 혹은 SkillAnimType (%d 개) 키값 설정이 있습니다."), 
				PCClassWrongSettingCount, SkillAnimTypeWrongSettingCount)));
#endif
		}
	}
#endif
}

bool UB2SkillAnimInfo::ShouldUnloadAllSkillAnimOnLevelTransition()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 는 아직 메모리 넘칠 것이 무서워서..
	return true;
#else
	return !bManagedSkillAnimUnload;
#endif
}

UAnimSequenceBase* UB2SkillAnimInfo::GetSingleSkillAnim(EPCClass InPCClass, ESkillAnimType InSkillAnimType)
{
	return GetSingleSkillAnim(FCombinedPCSkillAnimID(InPCClass, InSkillAnimType));
}

UAnimSequenceBase* UB2SkillAnimInfo::GetSingleSkillAnim(const FCombinedPCSkillAnimID& InCombinedID)
{
	const uint32 AssetKey = GetTypeHash(InCombinedID);

	if (auto* LoadedAnimObj = FindSingleSkillAnim(InCombinedID))
		return LoadedAnimObj;

	else if (FSkillAnimPerPCInfoSet* FoundPerPCInfoSet = InfoData.Find(InCombinedID.GetPCClassEnum()))
	{
		const auto& Asset = GetAnimationAsset(InCombinedID);

		// Animation은 AsyncWaiting을 하지 않고 바로 Load하는것이 현명한 판단이라 사료됨
		auto* AnimSequenceAsset = LoadSynchronous<UAnimSequenceBase>(Asset);

		if (AnimSequenceAsset != nullptr)
			LoadedPtrMap.Add(AssetKey, AnimSequenceAsset);
		
		return AnimSequenceAsset;
	}
	
	return nullptr;
}

class UAnimSequenceBase* UB2SkillAnimInfo::FindSingleSkillAnim(const FCombinedPCSkillAnimID& InCombinedID)
{
	const uint32 AssetKey = GetTypeHash(InCombinedID);

	if (UObject** LoadedAnimObj = LoadedPtrMap.Find(AssetKey))
		return Cast<UAnimSequenceBase>(*LoadedAnimObj);

	return nullptr;
}

TSoftObjectPtr<UAnimSequenceBase> UB2SkillAnimInfo::GetAnimationAsset(const FCombinedPCSkillAnimID& CombineID)
{
	if (const FSkillAnimPerPCInfoSet* SkillAnimInfo = InfoData.Find(CombineID.GetPCClassEnum()))
	{
		const auto& AnimSet = SkillAnimInfo->SkillAnims;
		if (const TSoftObjectPtr<UAnimSequenceBase>* AnimAssetPtr = AnimSet.Find(CombineID.GetSkillAnimType()))
		{
			return *AnimAssetPtr;
		}
	}

	return nullptr;
}

void UB2SkillAnimInfo::SyncPreloadAssets(const TArray<FCombinedPCSkillAnimID>& InLoadAnimIDs)
{ // 이름에 sync 자를 붙인 건 Async 로딩 구현이 될 것으로 고려해서임.
	B2_SCOPED_TRACK_LOG(TEXT("UB2SkillAnimInfo::SyncPreloadAssets"));
	for (const FCombinedPCSkillAnimID& ThisCombinedID : InLoadAnimIDs)
	{
		GetSingleSkillAnim(ThisCombinedID);
	}
}

void UB2SkillAnimInfo::UnloadAll()
{
	for (TMap<uint32, UObject*>::TIterator AssetIt(LoadedPtrMap); AssetIt; ++AssetIt)
	{
		if (UObject* Object = AssetIt.Value())
		{
			AssetIt.RemoveCurrent();
			Object->RemoveFromRoot(); // 기본적으로 로딩하면서 루트셋 처리를 하기 때문에 RemoveFromRoot 해야 함.
			UnloadAsset(EstimateStringAsset(Object));
		}
	}

	LoadedPtrMap.Empty();
}

void UB2SkillAnimInfo::UnloadAnimAsset(const FCombinedPCSkillAnimID& CombineID)
{
	if (auto* AnimObj = FindSingleSkillAnim(CombineID))
	{
		AnimObj->RemoveFromRoot(); // 기본적으로 로딩하면서 루트셋 처리를 하기 때문에 RemoveFromRoot 해야 함.
		LoadedPtrMap.Remove(GetTypeHash(CombineID));
	}

	//auto AssetPtr = GetAnimationAsset(CombineID);
	//if (AssetPtr.IsValid())
	//	UnloadAsset(AssetPtr);
}

void UB2SkillAnimInfo::UnloadExcept(const TArray<FCombinedPCSkillAnimID>& InExceptIDs)
{
	// ManagedSkillAnimUnload 에서 사용을 위함. 몇개는 언로드 안하고 남겨두려는..
	for (TMap<EPCClass, FSkillAnimPerPCInfoSet>::TIterator InfoDataIt(InfoData); InfoDataIt; ++InfoDataIt)
	{
		FSkillAnimPerPCInfoSet& InfoSet = InfoDataIt.Value();
		for (TMap<ESkillAnimType, TSoftObjectPtr<class UAnimSequenceBase>>::TIterator AnimIt(InfoSet.SkillAnims); AnimIt; ++AnimIt)
		{
			FCombinedPCSkillAnimID CombineID(InfoDataIt.Key(), AnimIt.Key());

			if(InExceptIDs.Contains(CombineID) == false)
				UnloadAnimAsset(CombineID);
		}
	}

	// 적어도 남겨두려는 클래스보다 많이 로딩된 상태면 안되겠지.
	check(LoadedPtrMap.Num() <= InExceptIDs.Num());
}

void UB2SkillAnimInfo::UnloadAllOrExceptSelected(UObject* WorldContextObject)
{
	// 레벨 transition 혹은 StageClear 시점에 불림. ManagedSkillAnimUnload 에 따라 어떤 걸 선별적으로 남겨둘지 판별..
	if (ShouldUnloadAllSkillAnimOnLevelTransition())
	{
		UnloadAll(); // 안전하게 죄다 내리지만 다음 로딩이 길어지겠지
	}
	else
	{
		TArray<FCombinedPCSkillAnimID> UnloadExceptAnims = GetCurrentPlayerSkillAnimIDs(WorldContextObject);
		UnloadExcept(UnloadExceptAnims);
	}
}

TArray<FCombinedPCSkillAnimID> UB2SkillAnimInfo::GetCurrentPlayerSkillAnimIDs(UObject* WorldContextObject)
{
	TArray<FCombinedPCSkillAnimID> CurrentAnimIDs;

	//// 현재 선택된 로컬 메인/태그 플레이어 캐릭터의 장착된 스킬 애니메이션을 남긴다. 이때는 가장 표준인 시나리오 모드 기준이 되어야 함.
	//FLocalCharacterData& LocalCDS = BladeIIGameImpl::GetLocalCharacterData();
	//for (int32 PCI = 0; PCI <= 1; ++PCI)
	//{
	//	const EPCClass CurrCharClass = ((PCI == 0) ? LocalCDS.GetMainPlayerClass() : LocalCDS.GetSubPlayerClass());
	//	TArray<ESkillAnimType> SkillAnimsOfClass;
	//	UB2SkillRTManager::GetCharacterRegisteredSkillAnimTypes(WorldContextObject, CurrCharClass, EB2GameMode::Scenario, SkillAnimsOfClass, &LocalCDS);
	//	for (ESkillAnimType ThisSkillAnimType : SkillAnimsOfClass)
	//	{
	//		CurrentAnimIDs.Add(FCombinedPCSkillAnimID(CurrCharClass, ThisSkillAnimType));
	//	}
	//}

	return CurrentAnimIDs;
}

void UB2SkillAnimInfo::OnAsyncLoadComplete(const FString& CompleteRequest, const TArray<FB2AsyncRequestInfo>& CompleteRequestList)
{
#if BII_SHIPPING_ALLOWED_DEV_FEATURE_LV2
	if (UB2AssetLoader::bEnableDevLog)
	{
		if (CompleteRequestList.Num() > 0)
		{
			FString LogMsg = FString::Printf(TEXT("[SkillAnimInfo] Async request of %d assets are completed and cached."), CompleteRequestList.Num());
			BII_SCREEN_LOG(LogMsg, FLinearColor(0.0, 1.0f, 0.0f, 1.0f), 14, 8.0f);
		}
	}
#endif
}

void UB2SkillAnimInfo::OnAsyncLoadComplete_EachObject(const uint32 AssetKey, UObject* LoadedAssetObject)
{
	if (LoadedAssetObject != nullptr)
	{
		LoadedPtrMap.Add(AssetKey, LoadedAssetObject);
		if(ShouldUnloadAllSkillAnimOnLevelTransition() == false)
			LoadedAssetObject->AddToRoot(); // 일단 RootSet 에 넣어놓고 레벨 내려갈 때 일부 유지할 것들은 남겨놓고 나머지들은 RootSet 에서 다시 뗌.
	}
}

TArray<FB2AsyncRequestInfo> UB2SkillAnimInfo::GetAsyncRequestAssetList(const TArray<FCombinedPCSkillAnimID>& CombineIDs)
{
	TArray<FB2AsyncRequestInfo> RequestAssets;

	for (const auto& CombineID : CombineIDs)
	{
		const auto& AnimAsset = GetAnimationAsset(CombineID);
		if (AnimAsset.IsNull() == false)
		{
			//FB2AsyncRequestInfo AsyncLoadInfo(CombineID, AnimAsset);
			//RequestAssets.Add(AsyncLoadInfo);
		}
	}

	return RequestAssets;
}

void UB2SkillAnimInfo::EnsureDataIntegrity()
{
	// 좀 바보같나.. 그냥 MyPCClass 를 값을 넣어주게 할껄 그랬나...
	for (TMap<EPCClass, FSkillAnimPerPCInfoSet>::TIterator InfoDataIt(InfoData); InfoDataIt; ++InfoDataIt)
	{
		FSkillAnimPerPCInfoSet& ThisInfoSet = InfoDataIt.Value();
		ThisInfoSet.MyPCClass = InfoDataIt.Key();
	}
}

#if WITH_EDITOR
void UB2SkillAnimInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	EnsureDataIntegrity(); // 저장되는 값을 넣는 건 아니지만 에디터에서 뭔가 항목 추가한 직후 확인이 필요할 수 있으니..
}
#endif

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP /* Doesn't look like "TEMP" anymore.. */
void UB2SkillAnimInfo::CleanupOnPreSave()
{
	LoadedPtrMap.Empty(); // 자칫 저장되면 안좋을 수 있는 데이터 확실히 날림.
}
void UB2SkillAnimInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	CleanupOnPreSave();
}
#endif