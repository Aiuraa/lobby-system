/*
	Lobby system - Ebi
	Licensed under GPL v3.0
*/

#include <a_samp>

#if !defined MAX_LOBBY_ROOM 
	#define MAX_LOBBY_ROOM 		5
#endif

#if !defined MAX_LOBBY_SLOT
	#define MAX_LOBBY_SLOT 		30
#endif

#if defined DONT_USE_LOBBY_TAG
	#define Lobby:
#endif

#define INVALID_LOBBY_ID 		Lobby:-1

#define ERROR_INVALID_LOBBY 	-1
#define ERROR_PLAYER_IN_LOBBY 	-2
#define ERROR_SAME_LOBBY 		-3
#define ERROR_LOBBY_FULL 		-4

/* Lobby Data*/
static enum e_LOBBY_DATA
{
	bool:e_LOBBY_EXISTS,
	e_LOBBY_SKIN,
	Float:e_LOBBY_POS_X, 
	Float:e_LOBBY_POS_Y,
	Float:e_LOBBY_POS_Z,
	Float:e_LOBBY_POS_A,
	e_LOBBY_INTERIOR,
	e_LOBBY_WORLD,
	e_LOBBY_GUN_1,
	e_LOBBY_AMMO_1,
	e_LOBBY_GUN_2,
	e_LOBBY_AMMO_2,
	e_LOBBY_GUN_3,
	e_LOBBY_AMMO_3,
}

static 
	Lobby_g_sLobbyData[Lobby:MAX_LOBBY_ROOM][e_LOBBY_DATA] = 
	{
		false, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0, ...
	},
	Lobby_g_sLobbyRooms[Lobby:MAX_LOBBY_ROOM][MAX_LOBBY_SLOT] = 
	{
		{INVALID_PLAYER_ID, ...}, ...
	},
	Lobby_g_sLobbySlots[Lobby:MAX_LOBBY_ROOM] = 
	{
		0, ...
	};

static
	Lobby:Lobby_g_sPlayerlobby[MAX_PLAYERS] = {Lobby:INVALID_LOBBY_ID, ...};

forward OnPlayerJoinLobby(playerid, Lobby:lobby);
forward OnPlayerLeaveLobby(playerid, Lobby:lobby);

/* Lobby Input checks */
stock bool:Lobby_IsValid(Lobby:lobby)
{
	return (Lobby:0 <= lobby < Lobby:MAX_LOBBY_ROOM) && Lobby_g_sLobbyData[lobby][e_LOBBY_EXISTS];
}

stock Lobby:Lobby_Create(
	skin,
	Float:positionX, Float:positionY, Float:positionZ, Float:positionAng, 
	interior = 0, virtualworld = 0, 
	gun1 = 0, ammo1 = 0,
	gun2 = 0, ammo2 = 0, 
	gun3 = 0, ammo3 = 0
)
{
	for (new Lobby:i = 0; i < Lobby:MAX_LOBBY_ROOM; i ++)
	{
		if (!Lobby_IsValid(i))
		{
			Lobby_g_sLobbyData[i][e_LOBBY_EXISTS] = true;

			Lobby_g_sLobbyData[i][e_LOBBY_SKIN] = skin;

			Lobby_g_sLobbyData[i][e_LOBBY_POS_X] = positionX;
			Lobby_g_sLobbyData[i][e_LOBBY_POS_Y] = positionY;
			Lobby_g_sLobbyData[i][e_LOBBY_POS_Z] = positionZ;
			Lobby_g_sLobbyData[i][e_LOBBY_POS_A] = positionAng;

			Lobby_g_sLobbyData[i][e_LOBBY_INTERIOR] = interior;
			Lobby_g_sLobbyData[i][e_LOBBY_WORLD] = virtualworld;

			Lobby_g_sLobbyData[i][e_LOBBY_GUN_1] = gun1;
			Lobby_g_sLobbyData[i][e_LOBBY_AMMO_1] = ammo1;

			Lobby_g_sLobbyData[i][e_LOBBY_GUN_2] = gun2;
			Lobby_g_sLobbyData[i][e_LOBBY_AMMO_2] = ammo2;

			Lobby_g_sLobbyData[i][e_LOBBY_GUN_3] = gun3;
			Lobby_g_sLobbyData[i][e_LOBBY_AMMO_3] = ammo3;

			return i;
		}
	}
	return INVALID_LOBBY_ID;
}

/* Lobby Get/Set */
stock Lobby_Count(Lobby:lobby)
{
	if (!Lobby_IsValid(lobby))
	{
		return ERROR_INVALID_LOBBY;
	}

	return Lobby_g_sLobbySlots[lobby];
}

stock Lobby_GetPlayers(Lobby:lobby, playerData[MAX_PLAYERS])
{
	if (!Lobby_IsValid(lobby))
	{
		return ERROR_INVALID_LOBBY;
	}

	playerData = Lobby_g_sLobbyRooms[lobby];
	return 1;
}

stock Lobby_GetSkin(Lobby:lobby, &skin)
{
	if (!Lobby_IsValid(lobby))
	{
		return ERROR_INVALID_LOBBY;
	}

	skin = Lobby_g_sLobbyData[lobby][e_LOBBY_SKIN];
	return 1;
}

stock Lobby_GetPos(Lobby:lobby, &Float:x, &Float:y, &Float:z)
{
	if (!Lobby_IsValid(lobby))
	{
		return ERROR_INVALID_LOBBY;
	}

	x = Lobby_g_sLobbyData[lobby][e_LOBBY_POS_X];
	y = Lobby_g_sLobbyData[lobby][e_LOBBY_POS_Y];
	z = Lobby_g_sLobbyData[lobby][e_LOBBY_POS_Z];
	return 1;
}

stock Lobby_GetAngle(Lobby:lobby, &Float:a)
{
	if (!Lobby_IsValid(lobby))
	{
		return ERROR_INVALID_LOBBY;
	}

	a = Lobby_g_sLobbyData[lobby][e_LOBBY_POS_A];
	return 1;
}

stock Lobby_GetInterior(Lobby:lobby, &interior)
{
	if (!Lobby_IsValid(lobby))
	{
		return ERROR_INVALID_LOBBY;
	}

	interior = Lobby_g_sLobbyData[lobby][e_LOBBY_INTERIOR];
	return 1;
}

stock Lobby_GetVirtualWorld(Lobby:lobby, &virtualworld)
{
	if (!Lobby_IsValid(lobby))
	{
		return ERROR_INVALID_LOBBY;
	}

	virtualworld = Lobby_g_sLobbyData[lobby][e_LOBBY_WORLD];
	return 1;
}

stock Lobby_GetGuns(Lobby:lobby, guns[6])
{
	if (!Lobby_IsValid(lobby))
	{
		return ERROR_INVALID_LOBBY;
	}

	guns[0] = Lobby_g_sLobbyData[lobby][e_LOBBY_GUN_1];
	guns[2] = Lobby_g_sLobbyData[lobby][e_LOBBY_GUN_2];
	guns[4] = Lobby_g_sLobbyData[lobby][e_LOBBY_GUN_3];

	ammo[1] = Lobby_g_sLobbyData[lobby][e_LOBBY_AMMO_1];
	ammo[3] = Lobby_g_sLobbyData[lobby][e_LOBBY_AMMO_2];
	ammo[5] = Lobby_g_sLobbyData[lobby][e_LOBBY_AMMO_3];
	return 1;
}

stock Lobby_SetSkin(Lobby:lobby, skin)
{
	if (!Lobby_IsValid(lobby))
	{
		return ERROR_INVALID_LOBBY;
	}

	Lobby_g_sLobbyData[lobby][e_LOBBY_SKIN] = skin;
	return 1;
}

stock Lobby_SetPos(Lobby:lobby, Float:x, Float:y, Float:z)
{
	if (!Lobby_IsValid(lobby))
	{
		return ERROR_INVALID_LOBBY;
	}

	Lobby_g_sLobbyData[lobby][e_LOBBY_POS_X] = x;
	Lobby_g_sLobbyData[lobby][e_LOBBY_POS_Y] = y;
	Lobby_g_sLobbyData[lobby][e_LOBBY_POS_Z] = z;
	return 1;
}

stock Lobby_SetAngle(Lobby:lobby, Float:a)
{
	if (!Lobby_IsValid(lobby))
	{
		return ERROR_INVALID_LOBBY;
	}

	Lobby_g_sLobbyData[lobby][e_LOBBY_POS_A] = a;
	return 1;
}

stock Lobby_SetInterior(Lobby:lobby, interior)
{
	if (!Lobby_IsValid(lobby))
	{
		return ERROR_INVALID_LOBBY;
	}

	Lobby_g_sLobbyData[lobby][e_LOBBY_INTERIOR] = interior;
	return 1;
}

stock Lobby_SetVirtualWorld(Lobby:lobby, virtualworld)
{
	if (!Lobby_IsValid(lobby))
	{
		return ERROR_INVALID_LOBBY;
	}

	Lobby_g_sLobbyData[lobby][e_LOBBY_WORLD] = virtualworld;
	return 1;
}

stock Lobby_SetGuns(Lobby:lobby, guns[6])
{
	if (!Lobby_IsValid(lobby))
	{
		return ERROR_INVALID_LOBBY;
	}

	Lobby_g_sLobbyData[lobby][e_LOBBY_GUN_1] = guns[0];
	Lobby_g_sLobbyData[lobby][e_LOBBY_GUN_2] = guns[2];
	Lobby_g_sLobbyData[lobby][e_LOBBY_GUN_3] = guns[4];

	Lobby_g_sLobbyData[lobby][e_LOBBY_AMMO_1] = guns[1];
	Lobby_g_sLobbyData[lobby][e_LOBBY_AMMO_2] = guns[3];
	Lobby_g_sLobbyData[lobby][e_LOBBY_AMMO_3] = guns[5];
	return 1;
}

/* Lobby Entrance/Exit */
stock Lobby_Join(playerid, Lobby:lobby)
{
	if (!IsPlayerConnected(playerid))
	{
		return 0;
	}

	if (!Lobby_IsValid(lobby))
	{
		return ERROR_INVALID_LOBBY;
	}

	if (Lobby_g_sPlayerlobby[playerid] != INVALID_LOBBY_ID)
	{
		return ERROR_PLAYER_IN_LOBBY;
	}

	if (Lobby_g_sPlayerlobby[playerid] == lobby)
	{
		return ERROR_SAME_LOBBY;
	}

	if (Lobby_g_sLobbySlots[lobby] == Lobby:MAX_LOBBY_ROOM)
	{
		return ERROR_LOBBY_FULL;
	}

	Lobby_g_sLobbyRooms[lobby] = playerid;
	Lobby_g_sLobbySlots[lobby] ++;

	Lobby_g_sPlayerlobby[playerid] = lobby;

	SetSpawnInfo(playerid, 0, 
		Lobby_g_sLobbyData[i][e_LOBBY_SKIN],
		Lobby_g_sLobbyData[i][e_LOBBY_POS_X],
		Lobby_g_sLobbyData[i][e_LOBBY_POS_Y],
		Lobby_g_sLobbyData[i][e_LOBBY_POS_Z],
		Lobby_g_sLobbyData[i][e_LOBBY_POS_A],
		Lobby_g_sLobbyData[i][e_LOBBY_GUN_1],
		Lobby_g_sLobbyData[i][e_LOBBY_AMMO_1],
		Lobby_g_sLobbyData[i][e_LOBBY_GUN_2],
		Lobby_g_sLobbyData[i][e_LOBBY_AMMO_2],
		Lobby_g_sLobbyData[i][e_LOBBY_GUN_3],
		Lobby_g_sLobbyData[i][e_LOBBY_AMMO_3],
	);

	if (IsPlayerSpectating(playerid) && !IsPlayerSpawned(playerid))
	{
		TogglePlayerSpectating(playerid, false);
	}
	else
	{
		SpawnPlayer(playerid);
	}

	SetPlayerInterior(playerid, Lobby_g_sLobbyData[i][e_LOBBY_INTERIOR]);
	SetPlayerVirtualWorld(playerid, Lobby_g_sLobbyData[i][e_LOBBY_WORLD]);

	CallLocalFunction(#OnPlayerJoinLobby, "ii", playerid, _:lobby);
	return 1;
}

stock Lobby_Leave(playerid)
{
	if (!IsPlayerConnected(playerid))
	{
		return 0;
	}

	if (Lobby_g_sPlayerlobby[playerid] == INVALID_LOBBY_ID)
	{
		return 0;
	}

	for (new Lobby:i = Lobby:0; i < Lobby:MAX_LOBBY_ROOM; i ++)
	{
		for (new j = 0; j < Lobby_g_sLobbyRooms[i]; j ++)
		{
			if (Lobby_g_sLobbyRooms[i][j] == playerid)
			{
				Lobby_g_sLobbyRooms[i][j] = INVALID_PLAYER_ID;
				Lobby_g_sLobbySlots[i] --;

				Lobby_g_sPlayerlobby[playerid] = INVALID_LOBBY_ID;
				CallLocalFunction(#OnPlayerLeaveLobby, "ii", playerid, _:i);
				return 1;

			}
		}
	}
	return 0;
}

/* Lobby Utils */
stock Lobby_Switch(playerid, Lobby:lobby)
{
	if (!Lobby_Leave(playerid))
	{
		return 0;
	}
	
	return Lobby_Join(playerid, lobby);
}

stock Lobby_JoinNext(playerid)
{
	if (!Lobby_Leave(playerid))
	{
		return 0;
	}

	new Lobby:nextlobby = Lobby:min(Lobby_g_sPlayerlobby[playerid] + 1, MAX_LOBBY_ROOM - 1);
	return Lobby_Join(playerid, nextlobby);
}

stock Lobby_JoinPrevious(playerid)
{
	if (!Lobby_Leave(playerid))
	{
		return 0;
	}

	new Lobby:nextlobby = Lobby:max(Lobby_g_sPlayerlobby[playerid] - 1, 0);
	return Lobby_Join(playerid, nextlobby);
}

/* Lobby hooks */
public OnPlayerDisconnect(playerid, reason)
{
	if (Lobby_g_sPlayerlobby[playerid] != INVALID_LOBBY_ID)
	{
		for (new Lobby:i = Lobby:0; i < Lobby:MAX_LOBBY_ROOM; i ++)
		{
			for (new j = 0; j < Lobby_g_sLobbySlots[i]; j ++)
			{
				if (Lobby_g_sLobbyRooms[i][j] == playerid)
				{
					Lobby_g_sLobbyRooms[i][j] = INVALID_PLAYER_ID;
					Lobby_g_sLobbySlots[i] --;

					Lobby_g_sPlayerlobby[playerid] = INVALID_LOBBY_ID;
					CallLocalFunction(#OnPlayerLeaveLobby, "ii", playerid, _:i);

					#if defined Lobby_OnPlayerDisconnect
						return Lobby_OnPlayerDisconnect(playerid, reason);
					#else
						return 1;
					#endif
				}
			}
		}
	}
	#if defined Lobby_OnPlayerDisconnect
		return Lobby_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect Lobby_OnPlayerDisconnect
#if defined Lobby_OnPlayerDisconnect
	forward Lobby_OnPlayerDisconnect(playerid, reason);
#endif
