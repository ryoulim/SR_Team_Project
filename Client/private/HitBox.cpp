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
	if (m_bDead) // ��� üũ
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

#ifdef _COLLIDERRENDER

	static bool bRenderMode{ FALSE };
	if (KEY_DOWN(DIK_F8))
		bRenderMode = !bRenderMode;
	if (m_pCollider && bRenderMode)
		m_pCollider->Render();
#endif

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

	/* ���� ������Ʈ */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

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

	return S_OK;
}


void CHitBox::On_Collision(_uint MyColliderID, _uint OtherColliderID)
{

}

void CHitBox::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pTargetPlayer);
}
