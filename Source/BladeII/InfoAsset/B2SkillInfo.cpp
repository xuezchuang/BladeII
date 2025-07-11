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
{ // �ٸ� ��� TAsset ��ó�� �����Ϳ��� �� �ε��� ��..
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
	// LoadSynchronous ���ؾ� �ؼ� const ����.. �Ѥ�
	FSingleSkillInfo* SingleInfo = const_cast<FSingleSkillInfo*>(GetSingleInfoOfID(SkillInfoID));
	const bool bIsWeaponSkill = (SingleInfo && SingleInfo->WeaponSkillType != EWeaponSkillType::EWS_None);
	// �⺻���� Style ������ �ϰ������� ���� SingleSkillInfo �� ���󼭴� �����ܸ�.
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
{ // SkillAnim �����͸� �����ϴ� key ��ü�� FCombinedPCSkillAnimID �� ������ �ʰ� �ε��� ĳ�� ���� key �θ� ���.
	// FCombinedPCSkillAnimID �� InfoAsset �� ����ϰ� �ϸ� �״��� ���� ���� �� ���� �ʾƼ�. �� ���� �׷����� FCombinedPCSkillAnimID �� BlueprintType ���� �ٲ�߰���.
	return FCombinedPCSkillAnimID(InPCClass, InSkillAnimType);
}

bool UB2SkillAnimInfo::bManagedSkillAnimUnload = true;

UB2SkillAnimInfo::UB2SkillAnimInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// ManagedSkillAnimUnload �� ���� PCClassInfo �� �������� ������ �ϴ� ������ ������ ini �׸��� ���.
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
	if (GIsEditor) // TMap �� ��� Ư�� ������ Ȥ���� �߸��� �� �ִ� ���� üũ
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
				FString::Printf(TEXT("[SkillAnimInfo] �߸��� PCClass (%d ��) Ȥ�� SkillAnimType (%d ��) Ű�� ������ �ֽ��ϴ�."), 
				PCClassWrongSettingCount, SkillAnimTypeWrongSettingCount)));
#endif
		}
	}
#endif
}

bool UB2SkillAnimInfo::ShouldUnloadAllSkillAnimOnLevelTransition()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS �� ���� �޸� ��ĥ ���� ��������..
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

		// Animation�� AsyncWaiting�� ���� �ʰ� �ٷ� Load�ϴ°��� ������ �Ǵ��̶� ����
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
{ // �̸��� sync �ڸ� ���� �� Async �ε� ������ �� ������ ����ؼ���.
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
			Object->RemoveFromRoot(); // �⺻������ �ε��ϸ鼭 ��Ʈ�� ó���� �ϱ� ������ RemoveFromRoot �ؾ� ��.
			UnloadAsset(EstimateStringAsset(Object));
		}
	}

	LoadedPtrMap.Empty();
}

void UB2SkillAnimInfo::UnloadAnimAsset(const FCombinedPCSkillAnimID& CombineID)
{
	if (auto* AnimObj = FindSingleSkillAnim(CombineID))
	{
		AnimObj->RemoveFromRoot(); // �⺻������ �ε��ϸ鼭 ��Ʈ�� ó���� �ϱ� ������ RemoveFromRoot �ؾ� ��.
		LoadedPtrMap.Remove(GetTypeHash(CombineID));
	}

	//auto AssetPtr = GetAnimationAsset(CombineID);
	//if (AssetPtr.IsValid())
	//	UnloadAsset(AssetPtr);
}

void UB2SkillAnimInfo::UnloadExcept(const TArray<FCombinedPCSkillAnimID>& InExceptIDs)
{
	// ManagedSkillAnimUnload ���� ����� ����. ��� ��ε� ���ϰ� ���ܵη���..
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

	// ��� ���ܵη��� Ŭ�������� ���� �ε��� ���¸� �ȵǰ���.
	check(LoadedPtrMap.Num() <= InExceptIDs.Num());
}

void UB2SkillAnimInfo::UnloadAllOrExceptSelected(UObject* WorldContextObject)
{
	// ���� transition Ȥ�� StageClear ������ �Ҹ�. ManagedSkillAnimUnload �� ���� � �� ���������� ���ܵ��� �Ǻ�..
	if (ShouldUnloadAllSkillAnimOnLevelTransition())
	{
		UnloadAll(); // �����ϰ� �˴� �������� ���� �ε��� ���������
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

	//// ���� ���õ� ���� ����/�±� �÷��̾� ĳ������ ������ ��ų �ִϸ��̼��� �����. �̶��� ���� ǥ���� �ó����� ��� ������ �Ǿ�� ��.
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
			LoadedAssetObject->AddToRoot(); // �ϴ� RootSet �� �־���� ���� ������ �� �Ϻ� ������ �͵��� ���ܳ��� ���������� RootSet ���� �ٽ� ��.
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
	// �� �ٺ�����.. �׳� MyPCClass �� ���� �־��ְ� �Ҳ� �׷���...
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

	EnsureDataIntegrity(); // ����Ǵ� ���� �ִ� �� �ƴ����� �����Ϳ��� ���� �׸� �߰��� ���� Ȯ���� �ʿ��� �� ������..
}
#endif

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP /* Doesn't look like "TEMP" anymore.. */
void UB2SkillAnimInfo::CleanupOnPreSave()
{
	LoadedPtrMap.Empty(); // ��ĩ ����Ǹ� ������ �� �ִ� ������ Ȯ���� ����.
}
void UB2SkillAnimInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	CleanupOnPreSave();
}
#endif