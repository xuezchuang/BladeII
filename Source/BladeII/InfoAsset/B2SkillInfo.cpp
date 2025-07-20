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
#include "B2CommonSoundInfo.h"

FSingleSkillInfo::FSingleSkillInfo()
{
	SkillID						= SKILL_INVALID_ID;
	CoolTime					= 0.0f;	
	//WeaponSkillType				= EWeaponSkillType::EWS_None;
	EnableWeaponSkillPoint		= 8;
	SkillShootingRangeForAI		= 1000000.0f;
}

#if WITH_EDITOR
void FSingleSkillInfo::EditorOnPostLoad()
{ // 促弗 割割 TAsset 甸贸烦 俊叼磐俊急 促 肺爹秦 淋..
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
	//// LoadSynchronous 妮秦具 秦辑 const 力芭.. ぱぱ
	//FSingleSkillInfo* SingleInfo = const_cast<FSingleSkillInfo*>(GetSingleInfoOfID(SkillInfoID));
	//const bool bIsWeaponSkill = (SingleInfo && SingleInfo->WeaponSkillType != EWeaponSkillType::EWS_None);
	//// 扁夯利牢 Style 汲沥篮 老褒利栏肺 甸绢啊绊 SingleSkillInfo 俊 蝶扼辑绰 酒捞能父.
	//FButtonStyle RetStyle = bIsWeaponSkill ? CommonButtonStyle_WeaponSkill : CommonButtonStyle_Regular;

	//if (SingleInfo)
	//{
	//	RetStyle.Normal.SetResourceObject(SingleInfo->ButtonIcon_Normal.LoadSynchronous());
	//	RetStyle.Hovered.SetResourceObject(SingleInfo->ButtonIcon_Hovered.LoadSynchronous());
	//	RetStyle.Pressed.SetResourceObject(SingleInfo->ButtonIcon_Pressed.LoadSynchronous());
	//	RetStyle.Disabled.SetResourceObject(SingleInfo->ButtonIcon_Disabled.LoadSynchronous());
	//}

	//return RetStyle;
	return  CommonButtonStyle_WeaponSkill;
}

EAttackState UB2SkillInfo::GetAttackState(int32 SkillInfoID) const
{
	const FSingleSkillInfo* SingleInfo = GetSingleInfoOfID(SkillInfoID);
	return /*SingleInfo ? SingleInfo->AttackAnimState :*/ EAttackState::ECS_None;
}

bool UB2SkillInfo::IsTagSkill(int32 SkillInfoID) const
{
	const FSingleSkillInfo* SingleInfo = GetSingleInfoOfID(SkillInfoID);
	return SingleInfo ? SingleInfo->PassiveType == EPassiveType::EPassiveType_Tag : false;
}

float UB2SkillInfo::GetEnableWeaponSkillPoint(int32 SkillInfoID) const
{
	const FSingleSkillInfo* SingleInfo = GetSingleInfoOfID(SkillInfoID);
	return /*SingleInfo && SingleInfo->WeaponSkillType != EWeaponSkillType::EWS_None ? float(SingleInfo->EnableWeaponSkillPoint) :*/ 0.0f;
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
{ // SkillAnim 单捞磐甫 利犁窍绰 key 磊眉甫 FCombinedPCSkillAnimID 肺 窍瘤绰 臼绊 肺爹等 某矫 甘狼 key 肺父 荤侩.
	// FCombinedPCSkillAnimID 肺 InfoAsset 俊 殿废窍霸 窍搁 弊促瘤 焊扁 亮阑 芭 鞍瘤 臼酒辑. 酒 拱沸 弊矾妨搁 FCombinedPCSkillAnimID 甫 BlueprintType 栏肺 官层具摆瘤.
	return FCombinedPCSkillAnimID(InPCClass, InSkillAnimType);
}

bool UB2SkillAnimInfo::bManagedSkillAnimUnload = true;

UB2SkillAnimInfo::UB2SkillAnimInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// ManagedSkillAnimUnload 俊 措秦 PCClassInfo 狼 付蛮啊瘤 开且阑 窍绰 函荐客 悼老茄 ini 亲格阑 荤侩.
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
	if (GIsEditor) // TMap 狼 殿废 漂己 锭巩俊 趣咯唱 肋给瞪 荐 乐绰 汲沥 眉农
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
				FString::Printf(TEXT("[SkillAnimInfo] 肋给等 PCClass (%d 俺) 趣篮 SkillAnimType (%d 俺) 虐蔼 汲沥捞 乐嚼聪促."), 
				PCClassWrongSettingCount, SkillAnimTypeWrongSettingCount)));
#endif
		}
	}
#endif
}

bool UB2SkillAnimInfo::ShouldUnloadAllSkillAnimOnLevelTransition()
{
#if PLATFORM_IOS // [IOS_SPECIFIC_MEMORY_SAVING] iOS 绰 酒流 皋葛府 逞磨 巴捞 公辑况辑..
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
	//const uint32 AssetKey = GetTypeHash(InCombinedID);

	//if (auto* LoadedAnimObj = FindSingleSkillAnim(InCombinedID))
	//	return LoadedAnimObj;

	//else if (FSkillAnimPerPCInfoSet* FoundPerPCInfoSet = InfoData.Find(InCombinedID.GetPCClassEnum()))
	//{
	//	const auto& Asset = GetAnimationAsset(InCombinedID);

	//	// Animation篮 AsyncWaiting阑 窍瘤 臼绊 官肺 Load窍绰巴捞 泅疙茄 魄窜捞扼 荤丰凳
	//	auto* AnimSequenceAsset = LoadSynchronous<UAnimSequenceBase>(Asset);

	//	if (AnimSequenceAsset != nullptr)
	//		LoadedPtrMap.Add(AssetKey, AnimSequenceAsset);
	//	
	//	return AnimSequenceAsset;
	//}
	
	return nullptr;
}

class UAnimSequenceBase* UB2SkillAnimInfo::FindSingleSkillAnim(const FCombinedPCSkillAnimID& InCombinedID)
{
	//const uint32 AssetKey = GetTypeHash(InCombinedID);

	//if (UObject** LoadedAnimObj = LoadedPtrMap.Find(AssetKey))
	//	return Cast<UAnimSequenceBase>(*LoadedAnimObj);

	return nullptr;
}

TSoftObjectPtr<UAnimSequenceBase> UB2SkillAnimInfo::GetAnimationAsset(const FCombinedPCSkillAnimID& CombineID)
{
	//if (const FSkillAnimPerPCInfoSet* SkillAnimInfo = InfoData.Find(CombineID.GetPCClassEnum()))
	//{
	//	const auto& AnimSet = SkillAnimInfo->SkillAnims;
	//	if (const TSoftObjectPtr<UAnimSequenceBase>* AnimAssetPtr = AnimSet.Find(CombineID.GetSkillAnimType()))
	//	{
	//		return *AnimAssetPtr;
	//	}
	//}

	return nullptr;
}

void UB2SkillAnimInfo::SyncPreloadAssets(const TArray<FCombinedPCSkillAnimID>& InLoadAnimIDs)
{ // 捞抚俊 sync 磊甫 嘿牢 扒 Async 肺爹 备泅捞 瞪 巴栏肺 绊妨秦辑烙.
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
			Object->RemoveFromRoot(); // 扁夯利栏肺 肺爹窍搁辑 风飘悸 贸府甫 窍扁 锭巩俊 RemoveFromRoot 秦具 窃.
			UnloadAsset(EstimateStringAsset(Object));
		}
	}

	LoadedPtrMap.Empty();
}

void UB2SkillAnimInfo::UnloadAnimAsset(const FCombinedPCSkillAnimID& CombineID)
{
	//if (auto* AnimObj = FindSingleSkillAnim(CombineID))
	//{
	//	AnimObj->RemoveFromRoot(); // 扁夯利栏肺 肺爹窍搁辑 风飘悸 贸府甫 窍扁 锭巩俊 RemoveFromRoot 秦具 窃.
	//	LoadedPtrMap.Remove(GetTypeHash(CombineID));
	//}

	//auto AssetPtr = GetAnimationAsset(CombineID);
	//if (AssetPtr.IsValid())
	//	UnloadAsset(AssetPtr);
}

void UB2SkillAnimInfo::UnloadExcept(const TArray<FCombinedPCSkillAnimID>& InExceptIDs)
{
	//// ManagedSkillAnimUnload 俊辑 荤侩阑 困窃. 割俺绰 攫肺靛 救窍绊 巢败滴妨绰..
	//for (TMap<EPCClass, FSkillAnimPerPCInfoSet>::TIterator InfoDataIt(InfoData); InfoDataIt; ++InfoDataIt)
	//{
	//	FSkillAnimPerPCInfoSet& InfoSet = InfoDataIt.Value();
	//	for (TMap<ESkillAnimType, TSoftObjectPtr<class UAnimSequenceBase>>::TIterator AnimIt(InfoSet.SkillAnims); AnimIt; ++AnimIt)
	//	{
	//		FCombinedPCSkillAnimID CombineID(InfoDataIt.Key(), AnimIt.Key());

	//		if(InExceptIDs.Contains(CombineID) == false)
	//			UnloadAnimAsset(CombineID);
	//	}
	//}

	// 利绢档 巢败滴妨绰 努贰胶焊促 腹捞 肺爹等 惑怕搁 救登摆瘤.
	check(LoadedPtrMap.Num() <= InExceptIDs.Num());
}

void UB2SkillAnimInfo::UnloadAllOrExceptSelected(UObject* WorldContextObject)
{
	// 饭骇 transition 趣篮 StageClear 矫痢俊 阂覆. ManagedSkillAnimUnload 俊 蝶扼 绢恫 吧 急喊利栏肺 巢败笛瘤 魄喊..
	if (ShouldUnloadAllSkillAnimOnLevelTransition())
	{
		UnloadAll(); // 救傈窍霸 了促 郴府瘤父 促澜 肺爹捞 辨绢瘤摆瘤
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

	//// 泅犁 急琶等 肺拿 皋牢/怕弊 敲饭捞绢 某腐磐狼 厘馒等 胶懦 局聪皋捞记阑 巢变促. 捞锭绰 啊厘 钎霖牢 矫唱府坷 葛靛 扁霖捞 登绢具 窃.
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
			LoadedAssetObject->AddToRoot(); // 老窜 RootSet 俊 持绢初绊 饭骇 郴妨哎 锭 老何 蜡瘤且 巴甸篮 巢败初绊 唱赣瘤甸篮 RootSet 俊辑 促矫 独.
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
	// 粱 官焊鞍唱.. 弊成 MyPCClass 甫 蔼阑 持绢林霸 且铂 弊法唱...
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

	EnsureDataIntegrity(); // 历厘登绰 蔼阑 持绰 扒 酒聪瘤父 俊叼磐俊辑 购啊 亲格 眠啊茄 流饶 犬牢捞 鞘夸且 荐 乐栏聪..
}
#endif

#if TEMP_LAZYLOADING_MISHANDLING_CLEANUP /* Doesn't look like "TEMP" anymore.. */
void UB2SkillAnimInfo::CleanupOnPreSave()
{
	LoadedPtrMap.Empty(); // 磊末 历厘登搁 救亮阑 荐 乐绰 单捞磐 犬角洒 朝覆.
}
void UB2SkillAnimInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	CleanupOnPreSave();
}
#endif