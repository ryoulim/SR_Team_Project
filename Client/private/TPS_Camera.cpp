#include "TPS_Camera.h"

CTPS_Camera::CTPS_Camera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera{ pGraphic_Device }
{
}

CTPS_Camera::CTPS_Camera(const CTPS_Camera& Prototype)
	: CCamera(Prototype)
{
}

HRESULT CTPS_Camera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTPS_Camera::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTPS_Camera::Priority_Update(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	/* 이곳에서 TPS카메라 특유의 천천히 따라오는 움직임을 제어하자*/

	_float x = m_fOmega * fTimeDelta;
	_float fExp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x); // 감쇠 곡선

	const _float3& vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 change = vPos - m_vTargetPos;

	// 최대 이동 속도 제한
	//_float maxChange = FLT_MAX;
	//_float mag = sqrtf(change.x * change.x + change.y * change.y + change.z * change.z);
	//if (mag > maxChange)
	//{
	//	change = change * (maxChange / mag);
	//}

	_float3 temp = (m_vCurrentVelocity + change * m_fOmega) * fTimeDelta;
	m_vCurrentVelocity = (m_vCurrentVelocity - temp * m_fOmega) * fExp;

	_float3 output = m_vTargetPos + (change + temp) * fExp;

	// 목표에 너무 가까워졌으면 스냅
	if (((m_vTargetPos - vPos).x * (output - m_vTargetPos).x +
		(m_vTargetPos - vPos).y * (output - m_vTargetPos).y +
		(m_vTargetPos - vPos).z * (output - m_vTargetPos).z) > 0)
	{
		output = m_vTargetPos;
		m_vCurrentVelocity = _float3(0, 0, 0);
	}
}

EVENT CTPS_Camera::Update(_float fTimeDelta)
{
	if (!m_bActive)
		return EVN_NONE;
	Update_View_Matrix();
	Bind_Resource();
	return EVN_NONE;
}

void CTPS_Camera::Late_Update(_float fTimeDelta)
{
	//m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CTPS_Camera::Render()
{
	if (FAILED(m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		return E_FAIL;

	return S_OK;
}

void CTPS_Camera::Smooth_Damping(_float3 vTargetPos, _float fSmoothTime)
{
	m_vTargetPos = vTargetPos;
	fSmoothTime = max(0.0001f, fSmoothTime); // 0으로 나누는 거 방지
	_float m_fOmega = 2.0f / fSmoothTime;
}

void CTPS_Camera::Update_Projection_Matrix()
{
	m_ProjMatrix.MakePerspectiveProjMat(m_fFov, m_fAspect, m_fNear, m_fFar);
}

HRESULT CTPS_Camera::Ready_Components(void* pArg)
{
	CTransform::DESC desc = {};
	desc.fSpeedPerSec = 300.f;
	desc.fRotationPerSec = 10.f;

	/* For.Prototype_Component_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &desc)))
		return E_FAIL;

	return S_OK;
}

CTPS_Camera* CTPS_Camera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTPS_Camera* pInstance = new CTPS_Camera(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTPS_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCamera* CTPS_Camera::Clone(void* pArg)
{
	CTPS_Camera* pInstance = new CTPS_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTPS_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTPS_Camera::Free()
{
	__super::Free();
}
