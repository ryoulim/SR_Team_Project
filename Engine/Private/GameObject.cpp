#include "GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device { pGraphic_Device }
	, m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pGameInstance);
}

CGameObject::CGameObject(const CGameObject& Prototype) 
	: m_pGraphic_Device{ Prototype.m_pGraphic_Device }
	, m_pGameInstance{ Prototype.m_pGameInstance }
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CGameObject::Reset(void* pArg)
{
	return S_OK;
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
}

EVENT CGameObject::Update(_float fTimeDelta)
{
	return EVN_NONE;
}

void CGameObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

void CGameObject::FrameUpdate(float timeDelta, float _MaxFrame, float fSpeed, bool isLoop)
{
	if (isLoop)
	{
<<<<<<< Updated upstream
		if (_MaxFrame < m_fAnimationFrame)
			m_fAnimationFrame = 0;
=======
	m_fAnimationFrame += fSpeed * timeDelta;
		//현재 프레임이 맥스프레임보다 크면 ? 0 으로 바꿔라.
		if (_MaxFrame <= m_fAnimationFrame)
		{
			m_fAnimationFrame = 0.f;
		}
>>>>>>> Stashed changes
	}
	else
	{
		if (_MaxFrame <= m_fAnimationFrame)
			return;
	}

}

void CGameObject::Compute_ViewZ(const _float3* pPos)
{
	_float4x4	matCamWorld;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	_float3		vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_float3));

	_float3 vDiff = vCamPos - *pPos;
	m_fViewZ = D3DXVec3Length(&vDiff);
}

CComponent* CGameObject::Find_Component(const _wstring& strComponentTag)
{
	auto Iter = m_Components.find(strComponentTag);
	if (Iter == m_Components.end())
		return nullptr;

	return Iter->second;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg)
{
	if (m_Components.end() != m_Components.find(strComponentTag))
		return S_SKIP;

	CComponent*		pComponent = dynamic_cast<CComponent*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_COMPONENT, iLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	return S_OK;
}

void CGameObject::Free()
{
	__super::Free();

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
}
