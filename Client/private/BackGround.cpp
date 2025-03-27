// 내 클래스 이름 : BackGround
// 부모 클래스 이름 : Map

#include "BackGround.h"
#include "GameInstance.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMap{ pGraphic_Device }
{
}

CBackGround::CBackGround(const CBackGround& Prototype)
	: CMap(Prototype)
{
}

HRESULT CBackGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackGround::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_GAMEPLAY;
	m_szTextureID = TEXT("Test");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBackGround::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBackGround::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CBackGround::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CBackGround::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	VTXPOSNORTEX* pVertices = nullptr;
	if (SUCCEEDED(m_pVIBufferCom->Get_VB()->Lock(0, 0, (void**)&pVertices, D3DLOCK_READONLY)))
	{
		for (int i = 0; i < 4; i++) // 4는 임시, 정점 개수에 맞게 수정
		{

			pVertices[i].vTexcoord.x;
			pVertices[i].vTexcoord.y;

		}
		m_pVIBufferCom->Get_VB()->Unlock();
	}

	if(FAILED(__super::Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

HRESULT CBackGround::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);

	if (nullptr != pArg)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		if (pDesc->bCollision)
		{
			CCollider::DESC ColliderDesc{};
			ColliderDesc.pTransform = m_pTransformCom;
			ColliderDesc.vScale = static_cast<DESC*>(pArg)->vScale;
			ColliderDesc.vOffSet = {};

			/* For.Com_Collider */
			if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Rect"),
				TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
				return E_FAIL;
		}
	}

	return S_OK;
}

CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBackGround* pInstance = new CBackGround(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBackGround::Clone(void* pArg)
{
	CBackGround* pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();
}