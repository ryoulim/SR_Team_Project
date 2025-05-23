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

	switch (rand() % 2)
	{
	case 0:
		m_pBGM = m_pGameInstance->Get_Single_Sound("Water_FountainLoop01");
		break;
	case 1:
		m_pBGM = m_pGameInstance->Get_Single_Sound("Water_FountainLoop02");
		break;
	}

	m_pBGM->Set3DState(0.f, 500.f);
	m_pBGM->Set_Volume(0.4f);
	m_pBGM->Play();

	return S_OK;
}

void CHydroPump::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CHydroPump::Update(_float fTimeDelta)
{
	m_pBGM->Update3DPosition(*m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_fTimeAcc += fTimeDelta;
	if (m_fDeadTime <= m_fTimeAcc)
	{
		m_pBGM->Stop();
		return EVN_DEAD;
	}


	FrameUpdate(fTimeDelta, m_fAnimationMaxFrame, m_fAnimationSpeed, true);

	return EVN_NONE;
}

void CHydroPump::Late_Update(_float fTimeDelta)
{
	_float3	vTemp = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	CGameObject::Compute_ViewZ(&vTemp);

	const _float3& vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (m_pGameInstance->IsInFrustum(vPos, m_pTransformCom->Get_Radius()))
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);

	__super::Late_Update(fTimeDelta);
}

HRESULT CHydroPump::Render()
{
	return __super::Render();
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
	Safe_Release(m_pBGM);
}