#include "HitBox.h"


CHitBox::CHitBox(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CHitBox::CHitBox(const CHitBox& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CHitBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHitBox::Initialize(void* pArg)
{
	/* �÷��̾ �˰� �־�� */
	m_pTargetPlayer = GET_PLAYER;
	Safe_AddRef(m_pTargetPlayer);

	/* �ؽ�ó, Ʈ������, ��Ʈ����, �ݶ��̴� ������Ʈ �غ�(��ġ�ʱ�ȭ) */
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CHitBox::Priority_Update(_float fTimeDelta)
{
}

EVENT CHitBox::Update(_float fTimeDelta)
{
	/* [ ��Ʈ�ڽ� ���ŷ��� ] */
	m_fStartTime += fTimeDelta;
	if (m_fStartTime > m_fMaxTime)
	{
		//�Ϻη� ���� ������ ����
		m_bDead = true;
	}

	if (m_bDead)
	{
		return EVN_DEAD;
	}

	return EVN_NONE;
}

void CHitBox::Late_Update(_float fTimeDelta)
{
	//�ݶ��̴� ������Ʈ
	m_pCollider->Update_Collider();

	//�����׷� ������Ʈ
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CHitBox::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

HRESULT CHitBox::Render()
{
	m_pTransformCom->Bind_Resource();
	m_pGraphic_Device->SetTexture(0, nullptr);
	m_pVIBufferCom->Bind_Buffers();

	SetUp_RenderState();

	m_pVIBufferCom->Render();

	Release_RenderState();

	return S_OK;
}

HRESULT CHitBox::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}


HRESULT CHitBox::Ready_Components(void* pArg)
{
	/* Ʈ������ ������Ʈ */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	/* ��ġ, ������ �ʱ�ȭ */
	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
		m_pTransformCom->Scaling(pDesc->vScale);
		m_szBufferType = pDesc->szTextureTag;
		m_bHitDead = pDesc->bHitDead;
		m_fMaxTime = pDesc->fMaxTime;
		m_eLevelID = pDesc->eLevel;

		/* �ݶ��̵� ������Ʈ */
		CCollider_Capsule::DESC ColliderDesc{};
		ColliderDesc.pTransform = m_pTransformCom;
		ColliderDesc.vOffSet = {};
		ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
		ColliderDesc.pOwner = this;
		ColliderDesc.iColliderGroupID = CG_BOSS;
		ColliderDesc.iColliderID = pDesc->eID;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Capsule"),
			TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
			return E_FAIL;
	}

	/* ���� ������Ʈ */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}


void CHitBox::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{
	if (m_bHitDead)
		m_bDead = true;
}

CHitBox* CHitBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHitBox* pInstance = new CHitBox(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHitBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHitBox::Clone(void* pArg)
{
	CHitBox* pInstance = new CHitBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CHitBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CHitBox::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pTargetPlayer);
}
