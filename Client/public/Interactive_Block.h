// �� Ŭ���� �̸� : Interactive_Block
// �θ� Ŭ���� �̸� : Map

#pragma once
#include "Map.h"

BEGIN(Client)

class CInteractive_Block abstract : public CMap
{
protected:
	CInteractive_Block(LPDIRECT3DDEVICE9 pGraphic_Device);
	CInteractive_Block(const CInteractive_Block& Prototype);
	virtual ~CInteractive_Block() = default;

protected:
	_bool m_bDead = { false };
	_int  m_iHp = {};
	_int  m_iMaxHp = {};

public:
	virtual void Free();
};

END