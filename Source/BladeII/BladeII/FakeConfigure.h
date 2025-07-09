
#ifndef __FAKECONFIGURE_H__
#define __FAKECONFIGURE_H__

#if !UE_BUILD_SHIPPING
class NetFakeConfigure
{
public:
	static NetFakeConfigure& GetInstance();
	void ParseCommandline();

	const FString&	GetPVPRoomName() const
	{
		return PVPRoomName;
	}

	const FString&	GetRaidRoomName() const
	{
		return RaidRoomName;
	}

	const FString&	GetControlRoomName() const
	{
		return ControlRoomName;
	}

	const FString&	GetDeviceId() const
	{
		return CustomDeviceId;
	}

	int32 GetNumToPlayRaid() const
	{
		return NumToPlayRaid;
	}

	bool GetSkipEndGameByTime() const
	{
		return SkipEndGameByTime;
	}

	int32 GetNetMessageLOD() const
	{
		return NetMessageLOD;
	}

	float GetNetMessageFrequency() const
	{
		return NetMessageFrequency;
	}

	float GetUpdateLocationRadius() const
	{
		return UpdateLocationRadius;
	}

	bool IsPVPRoomName() const
	{
		return !PVPRoomName.IsEmpty();
	}

	bool IsRaidRoomName() const
	{
		return !RaidRoomName.IsEmpty();
	}

	bool IsControlRoomName() const
	{
		return !ControlRoomName.IsEmpty();
	}

	bool IsCustomDeviceId() const
	{
		return !CustomDeviceId.IsEmpty();
	}

	bool IsNumToPlayRaid() const
	{
		return NumToPlayRaid != -1;
	}

	bool GetHateRaidPenalty() const
	{
		return IsHatePenalty;
	}

	bool GetForcedUnitySkillOpen() const
	{
		return IsUnitySkillOpen;
	}

	void SetNumToPlayRaid(int32 InNumToPlayRaid)
	{
		NumToPlayRaid = InNumToPlayRaid;
	}

	void SetRaidRoomName(FString InRaidRoomName)
	{
		RaidRoomName = InRaidRoomName;
	}

	void SetControlRoomName(FString InControlRoomName)
	{
		ControlRoomName = InControlRoomName;
	}

	void SetSkipEndGameByTime(bool v)
	{
		SkipEndGameByTime = v;
	}

	void SetNetMessageLOD(int32 lod)
	{
		NetMessageLOD = lod;
	}

	void SetNetMessageFrequency(int32 frequency)
	{
		NetMessageFrequency = static_cast<float>(frequency / 1000.0f);
	}

	void SetUpdateLocationRadius(float radius)
	{
		UpdateLocationRadius = radius;
	}

	void SetHateRaidPenalty(bool IsHate)
	{
		IsHatePenalty = IsHate;
	}

	void SetForcedUnitySkillOpen(bool IsOpen)
	{
		IsUnitySkillOpen = IsOpen;
	}

private:
	NetFakeConfigure() : NumToPlayRaid(-1), SkipEndGameByTime(false), NetMessageLOD(0), NetMessageFrequency(0.05f), UpdateLocationRadius(200.0f), IsHatePenalty(false)
	{
	}
	~NetFakeConfigure()
	{
	}
	NetFakeConfigure(NetFakeConfigure const&) = delete;
	NetFakeConfigure operator=(NetFakeConfigure const&) = delete;

	FString		PVPRoomName;
	FString		RaidRoomName;
	FString		ControlRoomName;
	FString		CustomDeviceId;
	int32		NumToPlayRaid;
	bool		SkipEndGameByTime;
	int32		NetMessageLOD;
	float		NetMessageFrequency;
	float		UpdateLocationRadius;
	bool		IsHatePenalty;
	bool		IsUnitySkillOpen;
};
#endif

#if WITH_EDITOR
class EditorFakeConfigure
{
public:
	static EditorFakeConfigure& GetInstance();

	EditorFakeConfigure(EditorFakeConfigure const&) = delete;
	EditorFakeConfigure operator=(EditorFakeConfigure const&) = delete;

	bool HavePowerToSave() const
	{
		return CanSaveDefaultGameMode;
	}

	void GivePowerToSave()
	{
		CanSaveDefaultGameMode = !CanSaveDefaultGameMode;
	}

private:
	EditorFakeConfigure() : CanSaveDefaultGameMode(false)
	{
	}
	~EditorFakeConfigure()
	{
	}

	bool	CanSaveDefaultGameMode;
};
#endif


#endif