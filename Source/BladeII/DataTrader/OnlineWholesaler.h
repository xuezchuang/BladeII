#pragma once
//#include "BladeII.h"
#include "IWholesaler.h"

class issue;
using issue_ptr = std::shared_ptr<issue>;

namespace data_trader
{
	class ExecHandler
	{
	public:
		static ExecHandler& GetInstance();
		
		bool	ExecuteShellCommand(const TCHAR* Cmd);
		bool	ExecuteShellCommandAccount(const TCHAR* Cmd);
		bool	ExecuteShellCommandNetWork(const TCHAR* Cmd);
		bool	ExecuteShellCommandEtc(const TCHAR* Cmd);
		bool	ExecuteShellCommandItem(const TCHAR* Cmd);
		bool	ExecuteShellCommandMatch(const TCHAR* Cmd);
		bool	ExecuteShellCommandStage(const TCHAR* Cmd);
		bool	ExecuteShellCommandHeroTower(const TCHAR* Cmd);
		bool	ExecuteShellCommandCounter(const TCHAR* Cmd);
		bool	ExecuteShellCommandDimension(const TCHAR* Cmd);
		bool	ExecuteShellCommandCharacter(const TCHAR* Cmd);
		bool	ExecuteShellCommandSkill(const TCHAR* Cmd);
		bool	ExecuteShellCommandMail (const TCHAR* Cmd);
		bool	ExecuteShellCommandFriend(const TCHAR* Cmd);
		bool	ExecuteShellCommandQuest(const TCHAR* Cmd);
		bool	ExecuteShellCommandGuild(const TCHAR* Cmd);
		bool	ExecuteShellCommandMission(const TCHAR* Cmd);
		bool	ExecuteShellCommandShop(const TCHAR* Cmd);
		bool	ExecuteShellCommandCollection(const TCHAR* Cmd);
		bool	ExecuteShellCommandAttendance(const TCHAR* Cmd);
		bool	ExecuteShellCommandGuildBattle(const TCHAR* Cmd);
		bool	ExecuteShellCommandKakaoPlatform(const TCHAR* Cmd);
		bool	ExecuteShellCommandChannelWorld(const TCHAR* Cmd);
		bool    ExecuteShellCommandServerCmd(const TCHAR* Cmd);

	private:
		ExecHandler() {}

		ExecHandler(ExecHandler const&) = delete;
		ExecHandler& operator = (ExecHandler const&) = delete;
	};

	class OnlineWholesaler : public IWholesaler
	{
	public:
		OnlineWholesaler();
		~OnlineWholesaler();

		void SubscribeInterested();

	private:
		TArray<issue_ptr>	Issues;
	};
}