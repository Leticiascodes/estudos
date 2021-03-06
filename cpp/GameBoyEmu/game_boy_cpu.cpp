#include "game_boy_cpu.hpp"

//////////////////////////////////////////////////////////////

GameBoyCPU::GameBoyCPU()
{
	m_ProgramCounter   = 0x0100;
	m_StackPointer.reg = 0xFFFE;

	m_RegisterAF.reg = 0x01B0;
	m_RegisterBC.reg = 0x0013;
	m_RegisterDE.reg = 0x00D8;
	m_RegisterHL.reg = 0x014D;

	m_Rom[0xFF05] = 0x00;
	m_Rom[0xFF06] = 0x00;
	m_Rom[0xFF07] = 0x00;
	m_Rom[0xFF10] = 0x80;
	m_Rom[0xFF11] = 0xBF;
	m_Rom[0xFF12] = 0xF3;
	m_Rom[0xFF14] = 0xBF;
	m_Rom[0xFF16] = 0x3F;
	m_Rom[0xFF17] = 0x00;
	m_Rom[0xFF19] = 0xBF;
	m_Rom[0xFF1A] = 0x7F;
	m_Rom[0xFF1B] = 0xFF;
	m_Rom[0xFF1C] = 0x9F;
	m_Rom[0xFF1E] = 0xBF;
	m_Rom[0xFF20] = 0xFF;
	m_Rom[0xFF21] = 0x00;
	m_Rom[0xFF22] = 0x00;
	m_Rom[0xFF23] = 0xBF;
	m_Rom[0xFF24] = 0x77;
	m_Rom[0xFF25] = 0xF3;
	m_Rom[0xFF26] = 0xF1;
	m_Rom[0xFF40] = 0x91;
	m_Rom[0xFF42] = 0x00;
	m_Rom[0xFF43] = 0x00;
	m_Rom[0xFF45] = 0x00;
	m_Rom[0xFF47] = 0xFC;
	m_Rom[0xFF48] = 0xFF;
	m_Rom[0xFF49] = 0xFF;
	m_Rom[0xFF4A] = 0x00;
	m_Rom[0xFF4B] = 0x00;
	m_Rom[0xFFFF] = 0x00;
}

//////////////////////////////////////////////////////////////

GameBoyCPU::~GameBoyCPU()
{
}

//////////////////////////////////////////////////////////////

BYTE GameBoyCPU::readMemory ( WORD address )
{

	// are we reading from the rom memory bank?
	if ( ( address >= BANKNN_BEGIN ) && ( address <= BANKNN_END ) )
	{
		WORD newAddress = address - BANKNN_BEGIN;
		return m_CartridgeMemory[newAddress + m_CurrentROMBank * 0x4000];
	}

	// are we reading from ram memory bank?
	else if ( ( address >= EXTERNALRAM_BEGIN ) && ( address <= EXTERNALRAM_END ) )
	{
		WORD newAddress = address - EXTERNALRAM_BEGIN;
		return m_RAMBanks[newAddress + m_CurrentRAMBank * 0x2000];
	}

	// else return memory
	return m_Rom[address];
	
}

//////////////////////////////////////////////////////////////

void GameBoyCPU::writeMemory ( WORD address, BYTE data )
{
	// dont allow any writing to the read only memory
	if ( address < ROM_SIZE )
		return;

	// this area is restricted
	if ( ( address >= 0xFEA0 ) && ( address < 0xFE00 ) )
		return;

	// writing to ECHO ram also writes in RAM
	if ( ( address >= ECHO_BEGIN ) && ( address <= ECHO_END ) )
	{
		m_Rom[address] = data;
		writeMemory ( address - 0x2000, data );
		return;
	}

	// no control needed over this area so write to memory
	m_Rom[address] = data;

}

//////////////////////////////////////////////////////////////
