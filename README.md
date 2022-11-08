# lobby-system

[![sampctl](https://img.shields.io/badge/sampctl-lobby--system-2f2f2f.svg?style=for-the-badge)](https://github.com/Aiuraa/lobby-system)

Vanilla lobby system with core functionality, and it's ready to use too!

**NOTE**: this is only the baremetal functions, i didn't add any teleportations yet. Take this repo for you to learn how to make lobby system.

## Installation

Simply install to your project:

```bash
sampctl package install Aiuraa/lobby-system
```

Include in your code and begin using the library:

```pawn
#include <lobby_system>
```

## Function Lists

```pawn
// Checks if lobby is valid
forward Lobby_IsValid(Lobby:lobby);

// Counts how many player have joined the lobby
forward Lobby_Count(Lobby:lobby);

// Get all players in current lobby, stored to `playerData` array
forward Lobby_GetPlayers(Lobby:lobby, playerData[MAX_PLAYERS]);

// Put player in lobby slot when player doesn't join to any lobbies (will call `OnPlayerJoinedLobby(playerid, Lobby:lobby)`)
forward Lobby_Join(playerid, Lobby:lobby);

// Leave player from current lobby
forward Lobby_Leave(playerid);

// Switch from one lobby to another lobby
forward Lobby_Switch(playerid, Lobby:lobby);

// Same as Lobby_Switch, but it will join next lobby
forward Lobby_JoinNext(playerid);

// Same as Lobby_Switch, but it will join previous lobby
forward Lobby_JoinPrevious(playerid);
````

## Testing

<!--
Depending on whether your package is tested via in-game "demo tests" or
y_testing unit-tests, you should indicate to readers what to expect below here.
-->

To test, simply run the package:

```bash
sampctl package run
```
