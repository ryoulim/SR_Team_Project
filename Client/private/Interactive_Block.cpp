// �� Ŭ���� �̸� : Interactive_Block
// �θ� Ŭ���� �̸� : Map

#include "Interactive_Block.h"

CInteractive_Block::CInteractive_Block(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CInteractive_Block::CInteractive_Block(const CInteractive_Block& Prototype)
	: CMap(Prototype),
	m_iHp{Prototype.m_iHp},
	m_iMaxHp{Prototype.m_iMaxHp}
{
}

void CInteractive_Block::Free()
{
	__super::Free();
}