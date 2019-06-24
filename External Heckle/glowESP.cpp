#include "Includes.h"

cGlowEsp* glowEsp;

ProcMem processMemory;

DWORD localPlayer;

void cGlowEsp::Run( ) {
	processMemory.Process( ( char* )"csgo.exe" );

	DWORD clientDLL;
	processMemory.Module( ( char* )"client_panorama.dll", clientDLL );

	if ( !localPlayer ) {
		 localPlayer = processMemory.Read<DWORD>( clientDLL + Offsets::localPlayer );
	}
	
	auto localTeam = processMemory.Read<int>( localPlayer + Offsets::Team );
	auto glowObject = processMemory.Read<DWORD>( clientDLL + Offsets::glowObjectManager );


	for ( int i = 0; i < 64; i++ ) {
		auto Entity = processMemory.Read<int>( clientDLL + Offsets::entityList + i * 0x10 );
		auto Dormant = processMemory.Read<bool>( Entity + Offsets::Dormant );
		auto glowIndex = processMemory.Read<int>( Entity + Offsets::glowIndex );
		auto Team = processMemory.Read<int>( Entity + Offsets::Team );

		if ( !Entity || Dormant || Team == localTeam )
			continue;

		processMemory.Write<float>( ( glowObject + ( ( glowIndex * 0x38 ) + 0x4 ) ), 1 );
		processMemory.Write<float>( ( glowObject + ( ( glowIndex * 0x38 ) + 0x8 ) ), 0 );
		processMemory.Write<float>( ( glowObject + ( ( glowIndex * 0x38 ) + 0xC ) ), 0 );
		processMemory.Write<float>( ( glowObject + ( ( glowIndex * 0x38 ) + 0x10 ) ), 0.6 );
		processMemory.Write<bool>( ( glowObject + ( ( glowIndex * 0x38 ) + 0x24 ) ), true );
		processMemory.Write<bool>( ( glowObject + ( ( glowIndex * 0x38 ) + 0x25 ) ), false );
	}
}