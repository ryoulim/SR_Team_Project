#include "StreetLampHead.h"
#include "GameInstance.h"

CStreetLampHead::CStreetLampHead(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CStatue(pGraphic_Device)
{
}

CStreetLampHead::CStreetLampHead(const CStreetLampHead& Prototype)
	: CStatue(Prototype)
{
}

HRESULT CStreetLampHead::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStreetLampHead::Initialize(void* pArg)
{
	m_szTextureID = TEXT("StreetLampHead");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Setting_LightDesc();

	DESC* pDesc = static_cast<DESC*>(pArg);
	m_iLightNumber = pDesc->iLightNumber;

	return S_OK;
}

void CStreetLampHead::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CStreetLampHead::Update(_float fTimeDelta)
{
	if (!m_bLightOn)
		m_pGraphic_Device->LightEnable(m_iLightNumber, false);

	return __super::Update(fTimeDelta);
}

void CStreetLampHead::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CStreetLampHead::Render()
{
	if(m_bLightOn)
		Turn_on_Light(m_iLightNumber);

	return __super::Render();
}

void CStreetLampHead::Setting_LightDesc()
{
#pragma region 조명의 속성 정의
	// 광원의 타입 : 점 광원
	m_LightDesc.Type = D3DLIGHT_POINT;

	// 광원의 위치
	m_LightDesc.Position = *m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// 조명이 미치는 범위
	m_LightDesc.Range = 650.f;

	// 광원으로부터 멀어질 때의 감쇄값
	m_LightDesc.Attenuation1 = 0.001f;

	// 기본 설정
	m_LightDesc.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	m_LightDesc.Ambient = D3DXCOLOR(0.4f, 0.3f, 0.2f, 1.f);
#pragma endregion
}

void CStreetLampHead::Turn_on_Light(_uint LightNumber)
{
	//m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

#pragma region 조명 설치
	// n번 광원을 설치한다.
	m_pGraphic_Device->SetLight(LightNumber, &m_LightDesc);
#pragma endregion

	// n번 광원을 켠다
	//m_pGraphic_Device->LightEnable(LightNumber, true);
}

CStreetLampHead* CStreetLampHead::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStreetLampHead* pInstance = new CStreetLampHead(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStreetLampHead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStreetLampHead::Clone(void* pArg)
{
	CStreetLampHead* pInstance = new CStreetLampHead(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CStreetLampHead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStreetLampHead::Free()
{
	__super::Free();
}
