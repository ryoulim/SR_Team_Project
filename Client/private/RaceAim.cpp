// 내 클래스 이름 : RaceAim
// 부모 클래스 이름 : UI

#include "RaceAim.h"

CRaceAim::CRaceAim(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CRaceAim::CRaceAim(const CRaceAim& Prototype)
	: CUI(Prototype)
{
}

HRESULT CRaceAim::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRaceAim::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("RaceAim");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CRaceAim::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CRaceAim::Update(const _float3& vPosition, _float fTimeDelta)
{
	_float4x4 view, proj, viewproj;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &view);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &proj);
	viewproj = view * proj;

	_float3 clip{};

	// 월드 → 클립 공간
	D3DXVec3TransformCoord(&clip, &vPosition, &viewproj);

	// 화면 중심 기준 직교 공간 좌표로 변환
	_float3 vOutScreenPos;
	vOutScreenPos.x = clip.x * (FWINCX * 0.5f);
	vOutScreenPos.y = clip.y * (FWINCY * 0.5f);
	vOutScreenPos.z = 0.f; // 직교 투영이므로 깊이는 무시

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vOutScreenPos);

	return EVN_NONE;
}

void CRaceAim::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CRaceAim::Render()
{
	return __super::Render();
}

CRaceAim* CRaceAim::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRaceAim* pInstance = new CRaceAim(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRaceAim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRaceAim::Clone(void* pArg)
{
	CRaceAim* pInstance = new CRaceAim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CRaceAim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRaceAim::Free()
{
	__super::Free();
}