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
#include "Core.hpp"
#include "Raknet\WindowsIncludes.h" 
#include <sstream>

#define MAX_CLIENTS 6
#define SERVER_PORT 6669

Core::Core()
{
	m_running = false;
	Logger::Init();
	Logger::Out( "Core::Core - Constructed." );

	RakNet::StartupResult result;
	result = m_peer->Startup( 1, &m_socket, 1 );
	if( result == RakNet::StartupResult::RAKNET_STARTED )
	{
		Logger::Out( "Core::Core - RakNet Started." );
	}
	RakNet::RakPeerInterface::DestroyInstance( m_peer );
}

Core::~Core()
{
	Logger::Close();
	Logger::Out( "Core::Core - Deconstruced." );
}

void Core::Run()
{
	while( !m_running )
	{
		if( !IsGameStarted() )
		{
			continue;
		}
		Logger::Out( "Core::Run - Passed menu check!" );

		if( CheckMultiplayerFlag() )
		{
			return;
		}
		Logger::Out( "Core::Run - Muiltiplayer Flag check passed!" );

		if( IsGameStarted() )
		{
			Sleep( 1000 );
			UpdatePlayerInfo();
			Sleep( 1000 );

			Patch_KillLifeLoss( true );
			m_running = true;
			break;
		}

		Sleep( 1000 );
	}

	while( m_running )
	{
		switch( IsGamePlaying() )
		{
		case true:
		{
			Update();
		}
			break;
		case false:
		{
		}
			break;
		}

		Sleep( 1000 );
	}

	MessageBoxA( NULL, "EXIT", "EXIT", NULL );
}

void Core::Update()
{
	std::string a = std::to_string( sizeof( PlayerInfo ) );

	a = std::to_string( m_playerInfo.ukn1 );
	Logger::Out( "Core::Update - Display MessageBox!" );
	MessageBoxA( NULL, a.c_str(), "TEST", MB_ICONWARNING );
}

bool Core::UpdatePlayerInfo()
{
	DWORD* ptrToPlayerInfo = (DWORD*)0x005eb4fc;
	if( *ptrToPlayerInfo == 0 )
	{
		Logger::Out( "Core::UpdatePlayerInfo - Player info is NULL!", 1 );
		return false;
	}
	else
	{
		Logger::Out( "Core::UpdatePlayerInfo - Player info found!" );
		memcpy( &m_playerInfo, (PVOID)*ptrToPlayerInfo, sizeof( PlayerInfo ) );
		return true;
	}
}

bool Core::CheckMultiplayerFlag()
{
	Logger::Out( "Core::CheckMultiplayerFlag - Checking if in multiplayer." );
	if( *(BYTE*)0x00673E2C == 1 ) // when multi
	{
		Logger::Out( "Core::CheckMultiplayerFlag - Your in multiplayer!" );
		Logger::Out( "Core::CheckMultiplayerFlag - We don't support GTA2 in multiplayer!", 1 );
		return true;
	}
	else if( *(BYTE*)0x00673E2C == 0 ) // when singleplayer
	{
		Logger::Out( "Core::CheckMultiplayerFlag - We're in singleplayer!" );
		return false;
	}

	Logger::Out( "Core::CheckMultiplayerFlag - Shouldn't reach this." );
	return true;
}

void Core::Patch_KillLifeLoss( bool set )
{
	DWORD oldProtection;
	VirtualProtect( (PVOID)0x0044B2EA, 4, PAGE_EXECUTE_READWRITE, &oldProtection );

	if( set )
	{
		memcpy( (void*)0x0044B2EA, "\x90\x90\x90\x90", 4 );
		Logger::Out( "Core::Patch_KillLifeLoss - Removed life loss!" );
	}
	else
	{
		memcpy( (void*)0x0044B2EA, "\x03\xC2\x89\x01", 4 );
		Logger::Out( "Core::Patch_KillLifeLoss - Re-applied life loss!" );
	}

	VirtualProtect( (PVOID)0x0044B3EA, 4, oldProtection, NULL );
}

bool Core::IsGamePlaying()
{
	return *m_isGamePaused;
}

bool Core::IsGameStarted()
{
	return *m_isGameStarted;
}
