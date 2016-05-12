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
#include <iostream>

DWORD Injector::m_processID = NULL;
const char* Injector::m_fileToInject = "GTA2++.dll";
const char* Injector::m_processName = "gta2.exe";

DWORD Injector::FindProcess( const char* process )
{
	DWORD ret = 0;
	PROCESSENTRY32 pe = { sizeof( PROCESSENTRY32 ) };
	HANDLE processSnapshot = NULL;

	int attempts = 0;
	std::cout << "\nSearching for " << process << "..." << std::endl;

	std::cout << "Loading";
	while( !ret && attempts < 5 )
	{
		// printf
		processSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
		if( processSnapshot == INVALID_HANDLE_VALUE )
		{
			std::cout << "Unable to create toolhelp snapshot!" << std::endl;
			return 0;
		}

		std::cout << '.';
		Process32First( processSnapshot, &pe );
		while( Process32Next( processSnapshot, &pe ) )
		{
			if( !strcmp( pe.szExeFile, process ) )
			{
				std::cout << "\nFound: " << pe.szExeFile << " @ " << pe.th32ProcessID << std::endl;
				ret = pe.th32ProcessID;
				CloseHandle( processSnapshot );
				return ret;
			}
		}

		attempts++;
		Sleep( 1000 ); // stop CPU overload lol.
	}

	std::cout << "\nFailed to find " << process << "!" << std::endl;

	CloseHandle( processSnapshot );
	return 0;
}

bool Injector::Prepare()
{
	std::cout << "Waiting for gta2.exe process..." << std::endl;
	while( !m_processID )
	{
		m_processID = FindProcess( m_processName );
		Sleep( 3000 );
	}
	return true;
}

int Injector::Inject( int delay )
{
	std::cout << "Injecting..." << std::endl;
	Sleep( delay );
	HANDLE processHandle;
	LPVOID remoteAddress, loadLocation;

	if( !m_processID )
	{
		std::cout << "No process to inject into." << std::endl;
		return 1;
	}

	processHandle = OpenProcess( PROCESS_ALL_ACCESS, false, m_processID );
	if( !processHandle )
	{
		std::cout << "Unable to open process: " << GetLastError() << std::endl;
		return 2;
	}

	loadLocation = (LPVOID)GetProcAddress( LoadLibrary( (LPCSTR)"KERNEL32" ), "LoadLibraryA" );
	remoteAddress = (LPVOID)VirtualAllocEx( processHandle, NULL, strlen( m_fileToInject ) + 1, MEM_COMMIT, PAGE_READWRITE );
	int result = WriteProcessMemory( processHandle, remoteAddress, m_fileToInject, strlen( m_fileToInject ) + 1, NULL ); // will get this warning later
	if( !result )
	{
		std::cout << "Failed to inject!" << std::endl;
		return 3;
	}

	CreateRemoteThread( processHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)loadLocation, (LPVOID)remoteAddress, NULL, NULL );

	CloseHandle( processHandle );

	std::cout << "GTA2++ should be injected!" << std::endl;
	return 4;
}

bool Injector::StartGTA()
{
	std::cout << "Executing gta2manager.exe!" << std::endl;
	int x = (int)ShellExecute( 0, "open", "gta2manager.exe", NULL, NULL, NULL );

	if( x == ERROR_FILE_NOT_FOUND || x == ERROR_PATH_NOT_FOUND )
	{
		std::cout << "Failed to start gta2manager.exe..." << std::endl;
		return false;
	}

	std::cout << "Started..." << std::endl;
	std::cout << "When ready start GTA2." << std::endl;
	return true;
}
