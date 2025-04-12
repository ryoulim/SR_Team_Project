// 내 클래스 이름 : Boat
// 부모 클래스 이름 : Monster

#include "Boat.h"
#include "GameInstance.h"

CBoat::CBoat(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CBoat::CBoat(const CBoat& Prototype)
	: CMonster(Prototype)
{
}

HRESULT CBoat::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoat::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Boat");
	m_szBufferType = TEXT("Rect");

	m_fDivOffset = 22.5f;
	m_bActive = false;
	m_vScale = _float3(254.f, 115.f, 1.f) * 0.4f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_iState = 0;
	m_fAnimationMaxFrame = 1.f;

	return S_OK;
}

void CBoat::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

EVENT CBoat::Update(_float fTimeDelta)
{
	return EVN_NONE;
}

void CBoat::Late_Update(_float fTimeDelta)
{
	PlayerDistance();
	CalculateVectorToPlayer();

	m_pCollider->Update_Collider();

	Compute_ViewAngle();
	Set_TextureType();

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;
}

HRESULT CBoat::Render()
{
	return __super::Render();
}

HRESULT CBoat::Ready_Components(void* pArg)
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_VIBuffer_")) + m_szBufferType,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		CMonster::DESC* pDesc = static_cast<DESC*>(pArg);
		m_eLevelID = pDesc->eLevel;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
		m_pTransformCom->Scaling(m_vScale);
	}

	CCollider::DESC ColliderDesc{};
	ColliderDesc.pTransform = m_pTransformCom;
	ColliderDesc.vScale = m_pTransformCom->Compute_Scaled();
	ColliderDesc.pOwner = this;
	ColliderDesc.iColliderGroupID = CG_TRIGGER;
	ColliderDesc.iColliderID = CI_TRIGGER;

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &ColliderDesc)))
		return E_FAIL;

	//셰이더 장착
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Particle"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(Ready_Textures()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoat::Ready_Textures()
{
	for (_uint i = 0; i < D_END; i++)
	{
		_wstring sPrototypeTag = L"Prototype_Component_Texture_Boat_";
		_uint num = static_cast<_uint>(i * m_fDivOffset);
		_tchar buf[32];
		_itow_s((int)num, buf, 10);
		sPrototypeTag += buf;
		if (FAILED(__super::Add_Component(m_eLevelID, sPrototypeTag,
			_wstring(TEXT("Com_Texture")) + L"_Boat_" + buf, reinterpret_cast<CComponent**>(&(m_pTextureMap[0][i])))))
			return E_FAIL;
	}

	return S_OK;
}

CBoat* CBoat::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoat* pInstance = new CBoat(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBoat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoat::Clone(void* pArg)
{
	CBoat* pInstance = new CBoat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBoat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoat::Free()
{
	__super::Free();
}