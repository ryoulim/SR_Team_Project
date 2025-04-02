#include "BulletMark.h"
#include "GameInstance.h"
#include "Particle_Define.h"

CBulletMark::CBulletMark(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect{ pGraphic_Device }
{
}

CBulletMark::CBulletMark(const CBulletMark& Prototype)
	: CEffect(Prototype)
{
}

HRESULT CBulletMark::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBulletMark::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	CBulletMark::DESC* SpriteDESC = static_cast<DESC*>(pArg);

	m_eLevelID = LEVEL_STATIC;
	m_fAnimationMaxFrame = SpriteDESC->fMaxFrame;
	m_szTextureID = SpriteDESC->szTextureTag;
	m_bLoop = SpriteDESC->bLoop;
	m_iType = SpriteDESC->iType;
	m_vLook = SpriteDESC->vLook;
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* 룩벡터 방향으로 회전 */
	m_pTransformCom->Turn_BulletMark(m_vLook);
	//m_pTransformCom->Move_Forward_ByLook(0.5f);

	return S_OK;
}

void CBulletMark::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBulletMark::Update(_float fTimeDelta)
{
	/* 시간초과시 삭제로직 */
	if (m_fAnimationFrame > m_fAnimationMaxFrame)
	{
		return EVN_DEAD;
	}

	FrameUpdate(fTimeDelta, m_fAnimationMaxFrame, 1.f, m_bLoop);
	return __super::Update(fTimeDelta);
}

void CBulletMark::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBulletMark::Render()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_iType))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	SetUp_RenderState();
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	Release_RenderState();

	return S_OK;
}

CBulletMark* CBulletMark::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBulletMark* pInstance = new CBulletMark(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBulletMark");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBulletMark::Clone(void* pArg)
{
	CBulletMark* pInstance = new CBulletMark(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBulletMark");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBulletMark::Free()
{
	__super::Free();

}