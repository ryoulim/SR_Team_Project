// 내 클래스 이름 : HydroPump
// 부모 클래스 이름 : Effect

#include "HydroPump.h"
#include "GameInstance.h"
#include "CameraManager.h"

CHydroPump::CHydroPump(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect{ pGraphic_Device },
	m_pCameraManager{CAMERA_MANAGER}
{
	Safe_AddRef(m_pCameraManager);
}

CHydroPump::CHydroPump(const CHydroPump& Prototype)
	: CEffect(Prototype),
	m_pCameraManager{Prototype.m_pCameraManager}
{
	Safe_AddRef(m_pCameraManager);
}

HRESULT CHydroPump::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHydroPump::Initialize(void* pArg)
{
	m_szTextureID = TEXT("HydroPump");
	m_szBufferType = TEXT("Rect");

	DESC* pDesc = static_cast<DESC*>(pArg);

	m_fAnimationFrame = 0.f;
	m_fAnimationMaxFrame = pDesc->fMaxFrame;
	m_fAnimationSpeed = pDesc->fAniSpeed;
	m_eLevelID = pDesc->eLevelID;
	m_fDeadTime = pDesc->m_fDeadTime;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	_float3 vCameraPos = *dynamic_cast<CTransform*>(m_pCameraManager->Get_Camera(CCameraManager::ID::FPS)->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);


	_float3 vCameraDir = vCameraPos - *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vCameraDir.Normalize();

	_float3 vPosition = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.z -= vCameraDir.z * 0.1f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

void CHydroPump::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CHydroPump::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_fDeadTime <= m_fTimeAcc)
		return EVN_DEAD;



	FrameUpdate(fTimeDelta, m_fAnimationMaxFrame, m_fAnimationSpeed, true);

	return EVN_NONE;
}

void CHydroPump::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CHydroPump::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 40);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (FAILED(m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_pTransformCom->Billboard())))
		return E_FAIL;

	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fAnimationFrame))))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CHydroPump* CHydroPump::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHydroPump* pInstance = new CHydroPump(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHydroPump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHydroPump::Clone(void* pArg)
{
	CHydroPump* pInstance = new CHydroPump(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CHydroPump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHydroPump::Free()
{
	__super::Free();
	Safe_Release(m_pCameraManager);
}