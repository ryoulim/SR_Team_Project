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

	/* �̰����� TPSī�޶� Ư���� õõ�� ������� �������� ��������*/

	_float x = m_fOmega * fTimeDelta;
	_float fExp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x); // ���� �

	const _float3& vPos = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float3 change = vPos - m_vTargetPos;

	_float3 temp = (m_vCurrentVelocity + change * m_fOmega) * fTimeDelta;
	m_vCurrentVelocity = (m_vCurrentVelocity - temp * m_fOmega) * fExp;

	_float3 output = m_vTargetPos + (change + temp) * fExp;

	// ��ǥ�� �ʹ� ����������� ����
	if (((m_vTargetPos - vPos).x * (output - m_vTargetPos).x +
		(m_vTargetPos - vPos).y * (output - m_vTargetPos).y +
		(m_vTargetPos - vPos).z * (output - m_vTargetPos).z) > 0)
	{
		output = m_vTargetPos;
		m_vCurrentVelocity = _float3(0, 0, 0);
	}

	if (m_bRecoil)
	{
		Update_Weapon_Recoil(fTimeDelta);
	}
	if (m_bShake)
	{
		Update_Camera_Shake(fTimeDelta);
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, output);
}

EVENT CTPS_Camera::Update(_float fTimeDelta)
{
	if (!m_bActive)
		return EVN_NONE;

	m_pGameInstance->Set_Listener_Position(m_pTransformCom, {});
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

void CTPS_Camera::StartShake(_float fIntensity, _float fDuration, _float fShakeFreqPos, _float fShakeFreqRot)
{
	m_fShakeIntensity = fIntensity;
	m_fShakeDuration = fDuration;
	m_fShakeFreqPos = fShakeFreqPos;
	m_fShakeFreqRot = fShakeFreqRot;

	m_fShakeTime = 0.f;
	m_bShake = TRUE;
}

void CTPS_Camera::StartRecoil(_float fIntensity, _float fDuration)
{
	m_fRecoilTime = 0.f;
	m_bRecoil = TRUE;

	m_vCurrentRecoilRot = {};
	m_vAccumulatedRecoil = {};
	m_fRecoilIntensity = fIntensity;
	m_fRecoilDuration = fDuration;
	m_fRecoilIntensityYaw = fIntensity * 0.25f;

	// �ʱ� ��ǥ �ݵ��� ���� (���� ��¦, �¿� ����)
	_float pitch = m_fRecoilIntensity;
	_float yaw = m_fRecoilIntensityYaw * m_pGameInstance->RandomFloat(-1.f, 1.f);
	m_vTargetRecoil = { pitch, yaw, 0.f };
}

void CTPS_Camera::Smooth_Damping(_float3 vTargetPos, _float fSmoothTime)
{
	m_vTargetPos = vTargetPos;
	fSmoothTime = max(0.0001f, fSmoothTime); // 0���� ������ �� ����
	m_fOmega = 2.0f / fSmoothTime;
}

void CTPS_Camera::Update_View_Matrix()
{
	m_pTransformCom->Move(m_vCurrentShakePos);
	m_pTransformCom->Quaternion_Turn(RADIAN(m_vCurrentShakeRot)); // ȸ�� ���� (Yaw, Pitch, Roll)}

	__super::Update_View_Matrix();

	m_pTransformCom->Move(-m_vCurrentShakePos);
	m_pTransformCom->Quaternion_Turn(RADIAN(-m_vCurrentShakeRot)); // _float3�� ȸ�� ����
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

void CTPS_Camera::Update_Camera_Shake(_float fTimedelta)
{
	m_fShakeTime += fTimedelta;

	if (m_fShakeTime >= m_fShakeDuration)
	{
		m_bShake = FALSE;
		m_vCurrentShakePos = { 0.f, 0.f, 0.f };
		m_vCurrentShakeRot = { 0.f, 0.f, 0.f };
		return;
	}

	// 2. ���� ����
	_float decay = 1.f - (m_fShakeTime / m_fShakeDuration);
	_float shakeStrength = m_fShakeIntensity * decay;

	_float t = m_fShakeTime;

	// 3. �ε巯�� ��ġ ��鸲 (sin/cos ���)
	_float3 offsetPos = {
		sin(t * m_fShakeFreqPos) * shakeStrength * 0.5f,
		cos(t * m_fShakeFreqPos * 0.8f) * shakeStrength * 0.4f,
		sin(t * m_fShakeFreqPos * 1.2f) * shakeStrength * 0.3f
	};

	// 4. �ε巯�� ȸ�� ��鸲 (YawPitchRoll ���� ����)
	_float3 offsetRot = {
		cos(t * m_fShakeFreqRot * 1.5f) * shakeStrength * 1.5f, // Pitch (X)
		sin(t * m_fShakeFreqRot) * shakeStrength * 2.0f, // Yaw   (Y)
		cos(t * m_fShakeFreqRot * 0.7f) * shakeStrength * 0.8f  // Roll  (Z)
	};

	// 5. ����
	m_vCurrentShakePos = offsetPos;
	m_vCurrentShakeRot = offsetRot;
}

void CTPS_Camera::Update_Weapon_Recoil(_float fTimeDelta)
{
	// 1. ���� ������ ȸ�� ����
	m_pTransformCom->Quaternion_Turn(RADIAN(-m_vCurrentRecoilRot));

	if (!m_bRecoil)
		return;

	// 2. �ð� ���� �� ���� ���
	m_fRecoilTime += fTimeDelta;
	_float t = m_fRecoilTime / m_fRecoilDuration;
	t = min(t, 1.f);

	// 3. � ���� ��� (EaseOut ��� �� EaseIn ����)
	const _float fUpRatio = 0.2f;
	_float curveRatio = 0.f;

	if (t < fUpRatio)
	{
		// ��� ����
		_float normT = t / fUpRatio;
		// EaseOut (������ �ö�)
		curveRatio = 1.f - powf(1.f - normT, 3.f);
	}
	else
	{
		// ���� ����
		_float normT = (t - fUpRatio) / (1.f - fUpRatio);
		// EaseIn (������ ������)
		_float eased = powf(normT, 3.f);
		// 1 �� 0 ���� ���� ��� ���� ���
		curveRatio = 1.f - eased;
	}

	// 4. �ݵ� �� ��� �� ����
	m_vAccumulatedRecoil = m_vTargetRecoil * curveRatio;
	m_vCurrentRecoilRot = m_vAccumulatedRecoil;
	m_pTransformCom->Quaternion_Turn(RADIAN(m_vCurrentRecoilRot));

	// 5. ���� ó��
	if (t >= 1.f)
	{
		m_bRecoil = FALSE;
		m_vCurrentRecoilRot = {};
		m_vAccumulatedRecoil = {};
	}
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
