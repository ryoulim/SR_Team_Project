// 내 클래스 이름 : FPS_Camera
// 부모 클래스 이름 : Camera

#include "FPS_Camera.h"

CFPS_Camera::CFPS_Camera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera{ pGraphic_Device }
{
}

CFPS_Camera::CFPS_Camera(const CFPS_Camera& Prototype)
	: CCamera(Prototype)
{
}

HRESULT CFPS_Camera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFPS_Camera::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	DESC* pDesc = static_cast<DESC*>(pArg);
	m_fMouseSensor = pDesc->fMouseSensor;

	return S_OK;
}

void CFPS_Camera::Priority_Update(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	if (m_bMouseFixMod)
	{
		Mouse_Move();
		Mouse_Fix();
	}

	if (m_bShake)
	{
		Update_Camera_Shake(fTimeDelta);
	}

	if (m_bRecoil)
	{
		Update_Weapon_Recoil(fTimeDelta);
	}

	if (m_bZoom)
	{
		Update_Zoom(fTimeDelta);
	}
}

EVENT CFPS_Camera::Update(_float fTimeDelta)
{
	if (!m_bActive)
		return EVN_NONE;

	m_pGameInstance->Set_Listener_Position(m_pTransformCom, {});
	Update_View_Matrix();
	Bind_Resource();
	return EVN_NONE;
}

void CFPS_Camera::Late_Update(_float fTimeDelta)
{
	//m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CFPS_Camera::Render() // 안할거야 알아하쇼
{
	if (FAILED(m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		return E_FAIL;
	return S_OK;
}

void CFPS_Camera::Mouse_Move()
{
	_float		fMouseMoveX = { static_cast<_float>(m_pGameInstance->Get_DIMMoveState(DIMM_X)) };
	_float		fMouseMoveY = { static_cast<_float>(m_pGameInstance->Get_DIMMoveState(DIMM_Y)) };

	_float3		vRotationAxis = (*m_pTransformCom->Get_State(CTransform::STATE_RIGHT) * fMouseMoveY)
		+ (*m_pTransformCom->Get_State(CTransform::STATE_UP) * fMouseMoveX);

	_float fAngle = RADIAN(_float3(fMouseMoveX, fMouseMoveY, 0).Length() * m_fMouseSensor);

	_float3 vLook = *m_pTransformCom->Get_State(CTransform::STATE_LOOK);

	_float4x4 matRot{ vRotationAxis,fAngle };
	vLook.TransformNormal(matRot);
	m_pTransformCom->LookAt(*m_pTransformCom->Get_State(CTransform::STATE_POSITION) + vLook);
}

void CFPS_Camera::Mouse_Fix()
{
	POINT		ptMouse{ g_iWinSizeX >> 1, g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CFPS_Camera::StartShake(_float fIntensity, _float fDuration, _float fShakeFreqPos, _float fShakeFreqRot)
{
	m_fShakeIntensity = fIntensity;
	m_fShakeDuration = fDuration;
	m_fShakeFreqPos = fShakeFreqPos;
	m_fShakeFreqRot = fShakeFreqRot;

	m_fShakeTime = 0.f;
	m_bShake = TRUE;
}

void CFPS_Camera::Zoom(_float fFOV, _float Time)
{
	m_bZoom = TRUE;
	m_fZoomTimer = 0.f;
	m_fOriginFov = m_fFov;
	m_fTargetFov = fFOV;
	m_fZoomTimeLimit = Time;
}

void CFPS_Camera::Update_View_Matrix()
{
	m_pTransformCom->Move(m_vCurrentShakePos);
	m_pTransformCom->Quaternion_Turn(RADIAN(m_vCurrentShakeRot)); // 회전 적용 (Yaw, Pitch, Roll)}

	__super::Update_View_Matrix();

	m_pTransformCom->Move(-m_vCurrentShakePos);
	m_pTransformCom->Quaternion_Turn(RADIAN(-m_vCurrentShakeRot)); // _float3로 회전 제거
}

void CFPS_Camera::Update_Projection_Matrix()
{
	m_ProjMatrix.MakePerspectiveProjMat(m_fFov, m_fAspect, m_fNear, m_fFar);
}

HRESULT CFPS_Camera::Ready_Components(void* pArg)
{
	/* For.Prototype_Component_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	return S_OK;
}


// 쉐이크 갱신 함수
void CFPS_Camera::Update_Camera_Shake(_float fTimedelta)
{
	m_fShakeTime += fTimedelta;

	if (m_fShakeTime >= m_fShakeDuration)
	{
		m_bShake = FALSE;
		m_vCurrentShakePos = { 0.f, 0.f, 0.f };
		m_vCurrentShakeRot = { 0.f, 0.f, 0.f };
		return;
	}

	// 2. 감쇠 적용
	_float decay = 1.f - (m_fShakeTime / m_fShakeDuration);
	_float shakeStrength = m_fShakeIntensity * decay;

	_float t = m_fShakeTime;

	// 3. 부드러운 위치 흔들림 (sin/cos 기반)
	_float3 offsetPos = {
		sin(t * m_fShakeFreqPos) * shakeStrength * 0.5f,
		cos(t * m_fShakeFreqPos * 0.8f) * shakeStrength * 0.4f,
		sin(t * m_fShakeFreqPos * 1.2f) * shakeStrength * 0.3f
	};

	// 4. 부드러운 회전 흔들림 (YawPitchRoll 순서 기준)
	_float3 offsetRot = {
		cos(t * m_fShakeFreqRot * 1.5f) * shakeStrength * 1.5f, // Pitch (X)
		sin(t * m_fShakeFreqRot) * shakeStrength * 2.0f, // Yaw   (Y)
		cos(t * m_fShakeFreqRot * 0.7f) * shakeStrength * 0.8f  // Roll  (Z)
	};

	// 5. 적용
	m_vCurrentShakePos = offsetPos;
	m_vCurrentShakeRot = offsetRot;
}

void CFPS_Camera::StartRecoil(_float fIntensity, _float fDuration)
{
	m_fRecoilTime = 0.f;
	m_bRecoil = TRUE;

	m_vCurrentRecoilRot = {};
	m_vAccumulatedRecoil = {};
	m_fRecoilIntensity = fIntensity;
	m_fRecoilDuration = fDuration;
	m_fRecoilIntensityYaw = fIntensity * 0.25f;

	// 초기 목표 반동값 설정 (위로 살짝, 좌우 랜덤)
	_float pitch = m_fRecoilIntensity;
	_float yaw = m_fRecoilIntensityYaw * m_pGameInstance->RandomFloat(-1.f, 1.f);
	m_vTargetRecoil = { pitch, yaw, 0.f };
}

void CFPS_Camera::Update_Weapon_Recoil(_float fTimeDelta)
{
	// 1. 이전 프레임 회전 제거
	m_pTransformCom->Quaternion_Turn(RADIAN(-m_vCurrentRecoilRot));

	if (!m_bRecoil)
		return;

	// 2. 시간 증가 및 비율 계산
	m_fRecoilTime += fTimeDelta;
	_float t = m_fRecoilTime / m_fRecoilDuration;
	t = min(t, 1.f);

	// 3. 곡선 비율 계산 (EaseOut 상승 → EaseIn 복귀)
	const _float fUpRatio = 0.2f;
	_float curveRatio = 0.f;

	if (t < fUpRatio)
	{
		// 상승 구간
		_float normT = t / fUpRatio;
		// EaseOut (빠르게 올라감)
		curveRatio = 1.f - powf(1.f - normT, 3.f);
	}
	else
	{
		// 복귀 구간
		_float normT = (t - fUpRatio) / (1.f - fUpRatio);
		// EaseIn (서서히 내려옴)
		_float eased = powf(normT, 3.f);
		// 1 → 0 선형 보간 대신 직접 계산
		curveRatio = 1.f - eased;
	}

	// 4. 반동 값 계산 및 적용
	m_vAccumulatedRecoil = m_vTargetRecoil * curveRatio;
	m_vCurrentRecoilRot = m_vAccumulatedRecoil;
	m_pTransformCom->Quaternion_Turn(RADIAN(m_vCurrentRecoilRot));

	// 5. 종료 처리
	if (t >= 1.f)
	{
		m_bRecoil = FALSE;
		m_vCurrentRecoilRot = {};
		m_vAccumulatedRecoil = {};
	}
}


void CFPS_Camera::Reset_Shake_And_Recoil()
{
	m_pTransformCom->Move(-m_vCurrentShakePos);
	m_pTransformCom->Quaternion_Turn(RADIAN(-m_vCurrentShakeRot));
}

void CFPS_Camera::Update_Zoom(_float fTimedelta)
{
	m_fZoomTimer += fTimedelta;
	if (m_fZoomTimer > m_fZoomTimeLimit)
		m_bZoom = FALSE;

	_float t = m_fZoomTimer / m_fZoomTimeLimit;
	m_fFov = LERP(m_fOriginFov, m_fTargetFov, t);
	Update_Projection_Matrix();
}


CFPS_Camera* CFPS_Camera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFPS_Camera* pInstance = new CFPS_Camera(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFPS_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCamera* CFPS_Camera::Clone(void* pArg)
{
	CFPS_Camera* pInstance = new CFPS_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CFPS_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFPS_Camera::Free()
{
	__super::Free();
}