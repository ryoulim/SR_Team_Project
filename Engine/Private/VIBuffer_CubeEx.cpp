#include "VIBuffer_CubeEx.h"

CVIBuffer_CubeEx::CVIBuffer_CubeEx(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer {pGraphic_Device}
{
}

CVIBuffer_CubeEx::CVIBuffer_CubeEx(const CVIBuffer_CubeEx& Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_CubeEx::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_CubeEx::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_CubeEx* CVIBuffer_CubeEx::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_CubeEx* pInstance = new CVIBuffer_CubeEx(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_CubeEx");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_CubeEx::Clone(void* pArg)
{
	CVIBuffer_CubeEx* pInstance = new CVIBuffer_CubeEx(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_CubeEx");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_CubeEx::Free()
{
	__super::Free();
}
