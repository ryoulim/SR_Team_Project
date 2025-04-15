#include "Signboard.h"

CSignboard::CSignboard(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue { pGraphic_Device }
{
}

CSignboard::CSignboard(const CSignboard& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CSignboard::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSignboard::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Signboard");
	m_szBufferType = TEXT("Signboard");

	m_eLevelID = static_cast<DESC*>(pArg)->eLevelID;

	Ready_Components(pArg);

	return S_OK;
}

void CSignboard::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CSignboard::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CSignboard::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CSignboard::Render()
{
	MultiTextureShaderRender(5);

	return S_OK;
}

HRESULT CSignboard::Ready_Components(void* pArg)
{
	__super::Ready_Components(pArg);

	if (nullptr != pArg)
	{
		/* For.Com_Collider */

		CCollider::DESC ColliderDesc{};
		ColliderDesc.pTransform = m_pTransformCom;
		_float3 vTmpScale = {
			m_pTransformCom->Compute_Scaled().x * 2.f,
			m_pTransformCom->Compute_Scaled().y * 2.f,
			m_pTransformCom->Compute_Scaled().z * 0.25f
		};
		ColliderDesc.pOwner = this;
		ColliderDesc.iColliderGroupID = CG_BLOCK;
		ColliderDesc.iColliderID = CI_BLOCK_COMMON;
		
		/* 계산을 용이하게 하기 위해서 위쪽 데이터를 기본으로하고 */
		/* 스케일은 계산을 한번 더 거치겠습니다 */
		
		//1. 왼쪽 기둥 콜라이더
		ColliderDesc.vOffSet = {
			0.f,
			0.f,
			-vTmpScale.x * 0.5f * 0.75f
		};
		ColliderDesc.vScale = {
			m_pTransformCom->Compute_Scaled().x * 0.25f,
			m_pTransformCom->Compute_Scaled().y * 2.f,
			m_pTransformCom->Compute_Scaled().z * 0.25f
		};

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB_Cube"),
			TEXT("Com_Collider_Left_Pillar"), reinterpret_cast<CComponent**>(&m_pColliderCom[0]), &ColliderDesc)))
			return E_FAIL;

		//2. 왼쪽 주춧돌 콜라이더//
		ColliderDesc.vOffSet = {
			0.f,
			-vTmpScale.y * 0.5f * 0.8125f,
			-vTmpScale.x * 0.5f * 0.75f
		};
		ColliderDesc.vScale = {
			m_pTransformCom->Compute_Scaled().x * 0.5f,
			m_pTransformCom->Compute_Scaled().y * 0.375f,
			m_pTransformCom->Compute_Scaled().z * 0.5f
		};

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB_Cube"),
			TEXT("Com_Collider_Left_Bottom"), reinterpret_cast<CComponent**>(&m_pColliderCom[1]), &ColliderDesc)))
			return E_FAIL;
	
		//3. 오른쪽 기둥 콜라이더
		ColliderDesc.vOffSet = {
			0.f,
			0.f,
			vTmpScale.x * 0.5f * 0.75f
		};
		ColliderDesc.vScale = {
			m_pTransformCom->Compute_Scaled().x * 0.25f,
			m_pTransformCom->Compute_Scaled().y * 2.f,
			m_pTransformCom->Compute_Scaled().z * 0.25f
		};

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB_Cube"),
			TEXT("Com_Collider_Right_Pillar"), reinterpret_cast<CComponent**>(&m_pColliderCom[2]), &ColliderDesc)))
			return E_FAIL;

		//4. 오른쪽 주춧돌 콜라이더
		ColliderDesc.vOffSet = {
			0.f,
			-vTmpScale.y * 0.5f * 0.8125f,
			vTmpScale.x * 0.5f * 0.75f
		};
		ColliderDesc.vScale = {
			m_pTransformCom->Compute_Scaled().x * 0.5f,
			m_pTransformCom->Compute_Scaled().y * 0.375f,
			m_pTransformCom->Compute_Scaled().z * 0.5f
		};

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB_Cube"),
			TEXT("Com_Collider_Right_Bottom"), reinterpret_cast<CComponent**>(&m_pColliderCom[3]), &ColliderDesc)))
			return E_FAIL;
	}

	return S_OK;
}

CSignboard* CSignboard::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSignboard* pInstance = new CSignboard(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSignboard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSignboard::Clone(void* pArg)
{
	CSignboard* pInstance = new CSignboard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CSignboard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSignboard::Free()
{
	__super::Free();

	for (_uint i = 0; i < 4; i++)
		Safe_Release(m_pColliderCom[i]);
}
