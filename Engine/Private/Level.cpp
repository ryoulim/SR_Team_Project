#include "Level.h"
#include "GameInstance.h"

#include "GameObject.h"

CLevel::CLevel(LPDIRECT3DDEVICE9 pGraphic_Device)
    : m_pGraphic_Device { pGraphic_Device }
    , m_pGameInstance { CGameInstance::Get_Instance() }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CLevel::Initialize(CLevelData* pLevelData)
{
    m_pData = pLevelData;
    Safe_AddRef(m_pData);

    return S_OK;
}


void CLevel::Update(_float fTimeDelta)
{
}


HRESULT CLevel::Render()
{
    return S_OK;
}

_wstring CLevel::Compute_PrototypeName(const _wstring& strPrototypeTag)
{
    size_t Pos = strPrototypeTag.rfind(TEXT("_"));
    if (Pos != _wstring::npos)
        return strPrototypeTag.substr(Pos + 1);

    return strPrototypeTag; // "_"이 없는 경우 원본 반환
}

HRESULT CLevel::Load_Map(_uint eLevelIdx, const _wstring& FileName)
{
    return S_OK;
}

HRESULT CLevel::Load_VertexBuffer(CGameObject* _pGameObject, HANDLE phFile, _ulong* pByte)
{
	_bool bResult = {};
	D3DVERTEXBUFFER_DESC VBDesc = {};
	bResult = ReadFile(phFile, &VBDesc, sizeof(D3DVERTEXBUFFER_DESC), pByte, NULL);

	CVIBuffer* pVIBuffer = dynamic_cast<CVIBuffer*>(_pGameObject->Find_Component(TEXT("Com_VIBuffer")));

	if (nullptr != pVIBuffer)
	{
		LPDIRECT3DVERTEXBUFFER9 pLoadVB = { nullptr };
		void* pData = { nullptr };
		if (FAILED(m_pGraphic_Device->CreateVertexBuffer(VBDesc.Size, VBDesc.Usage, VBDesc.FVF, VBDesc.Pool, &pLoadVB, nullptr)))
			return E_FAIL;

		if (SUCCEEDED(pLoadVB->Lock(0, 0, &pData, 0)))
		{
			bResult = ReadFile(phFile, pData, VBDesc.Size, pByte, NULL);
			pLoadVB->Unlock();

			pVIBuffer->Set_VertexBuffer(pLoadVB);
			Safe_Release(pLoadVB);
		}
	}

	return S_OK;
}

void CLevel::Free()
{
    __super::Free();

    Safe_Release(m_pData);
    Safe_Release(m_pGameInstance);
    Safe_Release(m_pGraphic_Device);
    Safe_Release(m_pData);
}
