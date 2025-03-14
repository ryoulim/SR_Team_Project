#include "Gravity.h"
#include "Transform.h"

const _float3*  CGravity::m_pTerrainVtxPos = { nullptr };
_uint			CGravity::m_iTerrainVtxNumX = {};
_uint			CGravity::m_iTerrainVtxNumZ = {};
_float			CGravity::m_fTerrainScale{};

CGravity::CGravity(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent{pGraphic_Device}
{
}

CGravity::CGravity(const CGravity& Prototype)
	:CComponent( Prototype )
{
}

HRESULT CGravity::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGravity::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	m_pMyTransform = static_cast<CTransform*>(pArg);

	return S_OK;
}

void CGravity::Set_TerrainInfo(const DESC& Desc)
{
	m_pTerrainVtxPos = Desc.pTerrainVtxPos;
	m_iTerrainVtxNumX = Desc.iTerrainVtxNumX;
	m_iTerrainVtxNumZ = Desc.iTerrainVtxNumZ;
	m_fTerrainScale = Desc.fTerrainScale;
}

void CGravity::Update()
{
	pPosition = m_pMyTransform->Get_State();


	_uint	iIndex = _uint(m_pPosition->z / m_fTerrainScale) * m_iTerrainVtxNumX 
		+ _uint(m_pPosition->x / m_fTerrainScale);

	_float	fRatioX = m_pPosition->x - m_pTerrainVtxPos[iIndex + m_iTerrainVtxNumX].x * m_fTerrainScale;
	_float	fRatioZ = m_pTerrainVtxPos[iIndex + m_iTerrainVtxNumX].z - m_pPosition->z * m_fTerrainScale;

	D3DXPLANE	Plane;

	
	_float3 VtxPos[4]{
		m_pTerrainVtxPos[iIndex + m_iTerrainVtxNumX] * m_fTerrainScale,
		m_pTerrainVtxPos[iIndex + m_iTerrainVtxNumX + 1] * m_fTerrainScale,
		m_pTerrainVtxPos[iIndex + 1] * m_fTerrainScale,
		m_pTerrainVtxPos[iIndex] * m_fTerrainScale
	};

	// ¿ì»ó´Ü
	if (fRatioX > fRatioZ)
	{
		D3DXPlaneFromPoints(&Plane,
			&VtxPos[0],
			&VtxPos[1],
			&VtxPos[2]);
	}

	else
	{
		D3DXPlaneFromPoints(&Plane,
			&VtxPos[0],
			&VtxPos[2],
			&VtxPos[3]);
	}

	// ax + by + cz + d
	// 
	// -by = ax + cz + d 

	//y = (-ax - cz - d) / b

	m_pPosition->y = (-Plane.a * m_pPosition->x - Plane.c * m_pPosition->z - Plane.d) / Plane.b;
}

CGravity* CGravity::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGravity* pInstance = new CGravity(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGravity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CGravity::Clone(void* pArg)
{
	CGravity* pInstance = new CGravity(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CGravity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGravity::Free()
{
	__super::Free();
}
