// �� Ŭ���� �̸� : Interactive_Block
// �θ� Ŭ���� �̸� : Map

#include "Interactive_Block.h"

CInteractive_Block::CInteractive_Block(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CInteractive_Block::CInteractive_Block(const CInteractive_Block& Prototype)
	: CMap(Prototype)
{
}

void CInteractive_Block::Free()
{
	__super::Free();
}