/*
*	GTA2++ Launcher
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
#include "Injector.hpp"
#include "Windows.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../GTA2++/Shared/Version.h"

BOOL IsAdmin()
{
	std::cout << "Checking admin rights..." << std::endl;
	BOOL ret = FALSE;
	HANDLE token = NULL;
	if( OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &token ) )
	{
		TOKEN_ELEVATION elevation;
		DWORD size = sizeof( TOKEN_ELEVATION );
		if( GetTokenInformation( token, TokenElevation, &elevation, sizeof( elevation ), &size ) )
		{
			ret = elevation.TokenIsElevated;
			if( ret == TRUE )
			{
				std::cout << "Sufficient rights found." << std::endl;
			}
		}
	}

	if( token )
	{
		CloseHandle( token );
	}

	return ret;
}

void CheckLog()
{
	std::cout << "Log:" << std::endl;
	std::ifstream logFile;
	logFile.open( "gta2++.log" );
	if( !logFile.is_open() )
	{
		std::cout << "Unable to find log..." << std::endl;
		return;
	}

	std::string line;
	while( getline( logFile, line ) )
	{
		std::cout << line << '\n';
	}
	logFile.close();
}

int main( int argc, char* charv[] )
{
	std::stringstream ss;
	std::string title;

	/*ss << "GTA2++ v";
	ss.put( Version::major );
	ss.put( '.' );
	ss.put( Version::minor );
	ss.put( '_' );
	ss.put( Version::patch );
	ss >> title; // Extract into the string.

	std::cout << title;*/
	std::cout << "Copyright(C) 2015-2016 Dennis Walsh\n\n" << std::endl;
	if( IsAdmin() == TRUE )
	{
		if( !Injector::StartGTA() )
		{
			std::cout << "Failed to start GTA..." << std::endl;
			std::getchar();
			return 1;
		}

		if( !Injector::Prepare() )
		{
			std::cout << "Failed to prepare the injector..." << std::endl;
			std::getchar();
			return 2;
		}

		if( Injector::Inject( 2000 ) < 4 )
		{
			std::cout << "Failed to inject..." << std::endl;
			std::getchar();
			return 3;
		}
	}
	else
	{
		std::cout << "You need admin permissions to use GTA2++!\nThis is to inject the GTA2++.dll into gta2.exe during launch.\n\nThis modification is tested on Windows 10, any issues contact: v0alzgames@gmail.com" << std::endl;
		std::getchar();
		return 4;
	}

	std::cout << "Waiting for gta2.exe to exit..." << std::endl;
	while( Injector::FindProcess( "gta2.exe" ) )
	{
		Sleep( 10000 );
	}
	CheckLog();
	std::getchar();
	return 0;
}
