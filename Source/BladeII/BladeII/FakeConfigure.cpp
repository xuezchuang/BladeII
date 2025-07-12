

#include "FakeConfigure.h"

#if !UE_BUILD_SHIPPING
NetFakeConfigure& NetFakeConfigure::GetInstance()
{
	static NetFakeConfigure	inst;
	return inst;
}

void NetFakeConfigure::ParseCommandline()
{
	int32	Num;
	FString Temp;

	if (FParse::Value(FCommandLine::Get(), TEXT("PVPRoomName"), Temp))
	{
		PVPRoomName = Temp.Replace(TEXT("="), TEXT("")).Replace(TEXT("\""), TEXT(""));
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("RaidRoomName"), Temp))
	{
		RaidRoomName = Temp.Replace(TEXT("="), TEXT("")).Replace(TEXT("\""), TEXT(""));
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("ControlRoomName"), Temp))
	{
		ControlRoomName = Temp.Replace(TEXT("="), TEXT("")).Replace(TEXT("\""), TEXT(""));
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("CustomDeviceId"), Temp))
	{
		CustomDeviceId = Temp.Replace(TEXT("="), TEXT("")).Replace(TEXT("\""), TEXT(""));
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("NumToPlayRaid"), Num))
	{
		NumToPlayRaid = Num;
	}

	if (FParse::Param(FCommandLine::Get(), TEXT("SkipEndGameByTime")))
	{
		SkipEndGameByTime = true;
	}
}
#endif

#if WITH_EDITOR
EditorFakeConfigure& EditorFakeConfigure::GetInstance()
{
	static EditorFakeConfigure	inst;
	return inst;
}
#endif