#include "CutScene_Camera.h"
#include "CameraManager.h"

#define POS first
#define LOOK second

CCutScene_Camera::CCutScene_Camera(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CCamera{ pGraphicDevice }
{
}

CCutScene_Camera::CCutScene_Camera(const CCutScene_Camera& Prototype)
	:CCamera(Prototype)
{
}

HRESULT CCutScene_Camera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCutScene_Camera::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCutScene_Camera::Priority_Update(_float fTimeDelta)
{
	 // 이곳에서 카메라 컷씬을 제어할것
	if (m_pMovePoints == nullptr || 
		m_pLookPoints == nullptr)
		return;

	if (m_bShake)
	{
		Update_Camera_Shake(fTimeDelta);
	}

	m_fTimeAcc += fTimeDelta * m_fCameraSpeed;

	_uint iIndex = static_cast<_uint>(m_fTimeAcc);
	_float t = m_fTimeAcc - iIndex;

	if (iIndex >= static_cast<_uint>(m_pMovePoints->size()) - 3)
	{
		CutSceneEnd();
		return;
	}

	_float3 vPos = CatmullRom(iIndex, t, TRUE);
	_float3 vLook = CatmullRom(iIndex, t, FALSE).Normalize();

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);

	_float3 vRight = _float3{ 0.f,1.f,0.f }.Cross(vLook).Normalize();
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);

	_float3 vUp = vLook.Cross(vRight).Normalize();
	m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);
}

EVENT CCutScene_Camera::Update(_float fTimeDelta)
{
	if (!m_bActive)
		return EVN_NONE;

	Update_View_Matrix();
	Bind_Resource();

	return EVN_NONE;
}

void CCutScene_Camera::Late_Update(_float fTimeDelta)
{
	// 뭐 안함
}

HRESULT CCutScene_Camera::Render()
{
	return S_OK;
}

void CCutScene_Camera::Start_CutScene(const _float3& vPos, const _float3& vLook)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->LookAt(vPos + vLook);
}

void CCutScene_Camera::Start_CutScene(vector<_float3>* pMovePoints, vector<_float3>* pLookPoints, _float fCameraSpeed, _bool* _Out_ pEndFlag)
{
	if (pMovePoints->size() < 2 || 
		pLookPoints->size() < 2)
	{ 
		MSG_BOX("컷씬카메라의 벡터가 부족합니다.");
		return;
	}
	if (fCameraSpeed <= 0 )
	{
		MSG_BOX("컷씬카메라의 속도가 부족합니다.");
		return;
	}

	m_pEndFlag = pEndFlag;
	*m_pEndFlag = FALSE;

	m_fTimeAcc = 0.f;
	m_pMovePoints = pMovePoints;
	m_pLookPoints = pLookPoints;
	m_pTransformCom->Set_SpeedPerSec(fCameraSpeed);
	m_fCameraSpeed = fCameraSpeed;

	if (!m_pMovePoints->empty())
	{
		m_pMovePoints->insert(m_pMovePoints->begin(), m_pMovePoints->front());
		m_pMovePoints->push_back(m_pMovePoints->back());

		m_pLookPoints->insert(m_pLookPoints->begin(), m_pLookPoints->front());
		m_pLookPoints->push_back(m_pLookPoints->back());
	}
}

void CCutScene_Camera::StartShake(_float fIntensity, _float fDuration, _float fShakeFreqPos, _float fShakeFreqRot)
{
	m_fShakeIntensity = fIntensity;
	m_fShakeDuration = fDuration;
	m_fShakeFreqPos = fShakeFreqPos;
	m_fShakeFreqRot = fShakeFreqRot;

	m_fShakeTime = 0.f;
	m_bShake = TRUE;
}

void CCutScene_Camera::Update_View_Matrix()
{
	m_pTransformCom->Move(m_vCurrentShakePos);
	m_pTransformCom->Quaternion_Turn(RADIAN(m_vCurrentShakeRot)); // 회전 적용 (Yaw, Pitch, Roll)}

	__super::Update_View_Matrix();

	m_pTransformCom->Move(-m_vCurrentShakePos);
	m_pTransformCom->Quaternion_Turn(RADIAN(-m_vCurrentShakeRot)); // _float3로 회전 제거
}

void CCutScene_Camera::Update_Projection_Matrix()
{
	m_ProjMatrix.MakePerspectiveProjMat(m_fFov, m_fAspect, m_fNear, m_fFar);
}

HRESULT CCutScene_Camera::Ready_Components(void* pArg)
{
	/* For.Prototype_Component_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;

	return S_OK;
}

_float3 CCutScene_Camera::CatmullRom(_uint iIndex, _float t, _bool isPos) const
{
	const vector<_float3>* TargetPoint{};

	if (isPos)
		TargetPoint = m_pMovePoints;
	else
		TargetPoint = m_pLookPoints;

	const _float3& p0 = TargetPoint->at(iIndex + 0);
	const _float3& p1 = TargetPoint->at(iIndex + 1);
	const _float3& p2 = TargetPoint->at(iIndex + 2);
	const _float3& p3 = TargetPoint->at(iIndex + 3);

	return 0.5f * ((2.0f * p1) +
		(-p0 + p2) * t +
		(2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t * t +
		(-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t * t * t);
}

// 쉐이크 갱신 함수
void CCutScene_Camera::Update_Camera_Shake(_float fTimedelta)
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

void CCutScene_Camera::CutSceneEnd()
{
	if (m_pEndFlag)
	{
		*m_pEndFlag = TRUE;	
		m_pEndFlag = nullptr;
	}

	Safe_Delete(m_pMovePoints);
	Safe_Delete(m_pLookPoints);
}

CCutScene_Camera* CCutScene_Camera::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CCutScene_Camera* pInstance = new CCutScene_Camera(pGraphicDevice);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCutScene_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCutScene_Camera* CCutScene_Camera::Clone(void* pArg)
{
	CCutScene_Camera* pInstance = new CCutScene_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CCutScene_Camera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCutScene_Camera::Free()
{
	__super::Free();
	CutSceneEnd();
}