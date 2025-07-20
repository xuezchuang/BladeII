#pragma once
//#include "BladeII.h"
#include "B2UIEnum.h"

struct FB2UILinkBase
{
	const EUIScene UIScene;
	FB2UILinkBase(EUIScene TargetScene) : UIScene(TargetScene) {}

	virtual void LinkUIScene(int32 Optional1 = 0, int32 Optional2 = 0) {}
	virtual void ExcuteUIScene() {}
};
class FB2UILinkManager
{
private:

	static FB2UILinkManager* UILinkManager;
	TMap<EUIScene, FB2UILinkBase*>	UILinkMap;
	static EUIScene CurrentLinkerScene;

private:

	void GenerateUILinkBase();
	FB2UILinkBase*	GetUILinkInfo(EUIScene InLinkUIScene);

public:

	static FB2UILinkManager& GetInstance();
	static bool LinkUIScene(EUIScene UIScene, int32 Optional1 = 0, int32 Optional2 = 0);
	static void ExcuteUIScene();
	static void SetCurrentLinkScene(EUIScene InScene);
};

/*
	clang++ compiler make a problem when using '##' with '::'
	example ) EUIScene::##SceneName -> makes compile error

	but this works on c++ compiler perfectly
*/
#define DECLARE_UISCENE_LINK(SceneName, UISceneEnum)							\
	struct FB2UILink_##SceneName : public FB2UILinkBase {						\
		FB2UILink_##SceneName() : FB2UILinkBase(UISceneEnum) {}					\
		void LinkUIScene_##SceneName(int32 Optional1, int32 Optional2);			\
		void ExcuteUIScene_##SceneName();										\
		virtual void LinkUIScene(int32 Optional1, int32 Optional2) override {	\
			LinkUIScene_##SceneName(Optional1, Optional2); }					\
		virtual void ExcuteUIScene() override {									\
			ExcuteUIScene_##SceneName(); }										\
	};																			\


#define REGISTER_UISCENE_LINK(SceneName, UISceneEnum)							\
	UILinkMap.Add(UISceneEnum, new FB2UILink_##SceneName());					\
			



DECLARE_UISCENE_LINK(Chapter, EUIScene::Chapter);
DECLARE_UISCENE_LINK(BattleStageInfo, EUIScene::BattleStageInfo);
DECLARE_UISCENE_LINK(CollectBookMain, EUIScene::CollectBookMain);
DECLARE_UISCENE_LINK(HeroTowerReady, EUIScene::HeroTowerReady);
DECLARE_UISCENE_LINK(Friend, EUIScene::Friend);
DECLARE_UISCENE_LINK(CounterAttackDGMain, EUIScene::CounterAttackDGMain);
DECLARE_UISCENE_LINK(Store, EUIScene::Store);
DECLARE_UISCENE_LINK(TeamMatchUI, EUIScene::TeamMatchUI);
DECLARE_UISCENE_LINK(PVPOneOnOne, EUIScene::PVPOneOnOne);
DECLARE_UISCENE_LINK(Inventory, EUIScene::Inventory);
DECLARE_UISCENE_LINK(Skill, EUIScene::Skill);
DECLARE_UISCENE_LINK(Brevet, EUIScene::Brevet);
DECLARE_UISCENE_LINK(Wing, EUIScene::Wing);
DECLARE_UISCENE_LINK(Relic, EUIScene::Relic);
DECLARE_UISCENE_LINK(GuildMain, EUIScene::GuildMain);
DECLARE_UISCENE_LINK(RaidMain, EUIScene::RaidMain);
DECLARE_UISCENE_LINK(Mail, EUIScene::Mail);
DECLARE_UISCENE_LINK(Mission, EUIScene::Mission);
DECLARE_UISCENE_LINK(EnterDungeonMain, EUIScene::EnterDungeonMain);
DECLARE_UISCENE_LINK(EnterBattleMain, EUIScene::EnterBattleMain);
DECLARE_UISCENE_LINK(LotteryStore, EUIScene::LotteryStore); //LotteryShop in Server side
DECLARE_UISCENE_LINK(ControlMain, EUIScene::ControlMain); 
DECLARE_UISCENE_LINK(EtherMain, EUIScene::EtherMain);
DECLARE_UISCENE_LINK(HallOfFame, EUIScene::HallOfFame);
DECLARE_UISCENE_LINK(FairyMain, EUIScene::FairyMain);
DECLARE_UISCENE_LINK(PackageStore, EUIScene::PackageStore);
DECLARE_UISCENE_LINK(DimensionMain, EUIScene::DimensionMain);
DECLARE_UISCENE_LINK(Totem, EUIScene::Totem);
DECLARE_UISCENE_LINK(Costume, EUIScene::Costume);
DECLARE_UISCENE_LINK(ItemForge, EUIScene::ItemForge);
DECLARE_UISCENE_LINK(SealBoxResult, EUIScene::SealBoxResult);
