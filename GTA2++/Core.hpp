/*
*	GTA2++
*	Copyright(C) 2015-2016 Dennis Walsh
*
*	This program is free software : you can redistribute it and / or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "Logger.hpp"
#include "GTA2\Structures\PlayerInfo.hpp"
#include "Raknet\WindowsIncludes.h"
#include "Raknet\RakPeerInterface.h"

class Core
{
public:
	Core();
	~Core();
	void Run();
	void Update();

protected:
	bool UpdatePlayerInfo();
	bool CheckMultiplayerFlag();
	void Patch_KillLifeLoss( bool set );

public:
	bool IsGamePlaying();
	bool IsGameStarted();

private:
	bool m_running;

	PlayerInfo m_playerInfo;
	DWORD* m_isGamePaused = (DWORD*)0x005DD310;
	DWORD* m_isGameStarted = (DWORD*)0x005E2030;

	RakNet::RakPeerInterface *m_peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor m_socket;
};
