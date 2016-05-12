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
#include "Logger.hpp"
#include <string>
#include <sstream>
#include "Shared/Version.h"

std::ofstream Logger::m_logFile;

void Logger::Init()
{
	m_logFile.open( "gta2++.log" );
	/*std::stringstream version;
	std::string str;

	version.put( Version::major );
	version.put( '.' );
	version.put( Version::minor );
	version.put( '_' );
	version.put( Version::patch );
	str << version.str;#
		

	Out( str.c_str(), 2 );*/
	Out( "Copyright(C) 2015-2016 Dennis Walsh\n-------------------------------------------\n", 2 );
}

void Logger::Close()
{
	if( m_logFile.is_open() )
	{
		Out( "Log end!" );
		m_logFile.close();
	}
}

void Logger::Out( const char* str, const int tag )
{
	if( m_logFile.is_open() )
	{
		char* t;
		switch( tag )
		{
		case 0:
			t = "[MSG]\t";
			break;
		case 1:
			t = "[ERR]\t";
			break;
		case 2:
			t = "-----\t";
			break;
		}

		m_logFile << t << str << '\n';
	}
}
