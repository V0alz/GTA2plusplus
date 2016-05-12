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

#include <Windows.h>

struct PlayerInfo
{
	int ukn1; // 0000
	DWORD* p1_ptr; // 0004
	DWORD* p2_ptr; // 0008
	DWORD* p3_ptr; // 000C
	DWORD* p4_ptr; // 0010
	DWORD* p5_ptr; // 0014
	DWORD* p6_ptr; // 0018
	DWORD* myPlayer_ptr; // 001C
	WORD ukn2; // 0020
	BYTE ukn3; // 0022
	BYTE numOfPlayers; // 0023
	BYTE myPlayerID; // 0024
	DWORD uknPad1; // 0028
	DWORD uknPad2; // 002C
	DWORD uknPad3; // 0030
	DWORD uknPad4; // 0034
	DWORD myPlayer_ptr_2; // 0038
};
