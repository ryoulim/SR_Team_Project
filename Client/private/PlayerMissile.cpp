// 내 클래스 이름 : PlayerMissile
// 부모 클래스 이름 : Bullet

#include "PlayerMissile.h"

CPlayerMissile::CPlayerMissile(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBullet{ pGraphic_Device }
{
}

CPlayerMissile::CPlayerMissile(const CPlayerMissile& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CPlayerMissile::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerMissile::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("PlayerMissile");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Make_TrailData(static_cast<DESC*>(pArg)->vScale.x);

	return S_OK;
}

void CPlayerMissile::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CPlayerMissile::Update(_float fTimeDelta)
{
	m_pTransformCom->Move({ 0.f,0.f,RACE_SPEED_PER_SEC }, fTimeDelta);
	m_pTransformCom->Go_Straight(fTimeDelta);

	Update_TrailData(fTimeDelta);

	FrameUpdate(fTimeDelta, 7.f, 40.f, true);
	return EVN_NONE;
}

void CPlayerMissile::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CPlayerMissile::Render()
{
	__super::Render();

	Render_TrailData();

	return S_OK;
}

HRESULT CPlayerMissile::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	DESC* pDesc = static_cast<DESC*>(pArg);
	CCollider_Line::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale.x = pDesc->vScale.x;
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_PBULLET;
	ColliderDesc.iColliderID = CI_MISSILE;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,pDesc->vPosition);
	m_pTransformCom->LookAt(pDesc->vLook + pDesc->vPosition);

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

void CPlayerMissile::Update_TrailData(_float dt)
{
	for (auto& Trail : m_TrailDatas)
	{
		Trail.pTransform->Move({ 0.f,0.f,RACE_SPEED_PER_SEC }, dt);
		Trail.pTransform->Go_Straight(dt);
	}
}

void CPlayerMissile::Render_TrailData()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	for (auto& Trail : m_TrailDatas)
	{
		m_pGraphic_Device->SetTransform(D3DTS_WORLD, &Trail.pTransform->Billboard_Y());

		// 잔상 렌더링 (예: 반투명한 텍스처 사용)
		m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "Tex", static_cast<_uint>(m_fAnimationFrame));
		m_pShaderCom->SetFloat("opacity", Trail.fRandomColor);

		m_pVIBufferCom->Bind_Buffers();

		m_pShaderCom->Begin(CShader::SANDEVISTAN);
		m_pVIBufferCom->Render();
		m_pShaderCom->End();
	}

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CPlayerMissile::Trail_Billboard(_float4x4& matWorld) const
{
	//_float4x4 CopyMatWorld = matWorld;
	_float3	vScaled = m_pTransformCom->Compute_Scaled();
	_float3	vPosition = *reinterpret_cast<_float3*>(matWorld.m[3]);

	_float4x4 matCamWorld;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	matCamWorld.MakeInverseMat(matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };

	_float3 vRight = reinterpret_cast<_float3*>(&matCamWorld.m[0][0])->Normalize() * vScaled.x;
	_float3 vLook = reinterpret_cast<_float3*>(&matCamWorld.m[2][0])->Normalize() * vScaled.z;

	memcpy(&matWorld._11, &vRight, sizeof _float3);
	memcpy(&matWorld._21, vLook.Cross(vRight), sizeof _float3);
	memcpy(&matWorld._31, &vLook, sizeof _float3);
}

void CPlayerMissile::Make_TrailData(_float InitScale)
{
	m_TrailDatas.resize(5);
	// -Look 방향으로 5개 만들자. 간격은 2정도
	_float3 vOffset = m_pTransformCom->Get_State(CTransform::STATE_LOOK)->Normalize() * -7.f;

	for (_uint i = 0; i < 5; ++i)
	{
		m_TrailDatas[i].pTransform = static_cast<CTransform*>(m_pTransformCom->Clone(nullptr));
		m_TrailDatas[i].pTransform->Move(vOffset * (i + 1.f));
		m_TrailDatas[i].pTransform->Scaling(InitScale * 0.1f * (9 - i));
		m_TrailDatas[i].fRandomColor = m_pGameInstance->RandomFloat(0.f, 1.f);
	}
}

CPlayerMissile* CPlayerMissile::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayerMissile* pInstance = new CPlayerMissile(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayerMissile");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayerMissile::Clone(void* pArg)
{
	CPlayerMissile* pInstance = new CPlayerMissile(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CPlayerMissile");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerMissile::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);

	for (auto Trail : m_TrailDatas)
		Safe_Release(Trail.pTransform);
	m_TrailDatas.clear();
}