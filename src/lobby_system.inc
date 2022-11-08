/*
	Lobby system - Ebi
	lmao pisan kang
*/

#include <a_samp>

#if !defined MAX_LOBBY_ROOM 
	#define MAX_LOBBY_ROOM 		5
#endif

#if !defined MAX_LOBBY_SLOT
	#define MAX_LOBBY_SLOT 		30
#endif

#define INVALID_LOBBY_ID 		Lobby:-1

#define ERROR_INVALID_LOBBY 	-1
#define ERROR_PLAYER_IN_LOBBY 	-2
#define ERROR_SAME_LOBBY 		-3
#define ERROR_LOBBY_FULL 		-4

static 
	Lobby_g_sLobbyRooms[Lobby:MAX_LOBBY_ROOM][MAX_LOBBY_SLOT],
	Lobby_g_sLobbySlots[Lobby:MAX_LOBBY_ROOM];

static
	Lobby:Lobby_g_sPlayerlobby[MAX_PLAYERS];

/* Lobby Input checks */
stock bool:Lobby_IsValid(Lobby:lobby)
{
	return (Lobby:0 <= lobby < Lobby:MAX_LOBBY);
}

/* Lobby Data */
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

	playerData = Lobby_g_sLobbyRooms;
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
