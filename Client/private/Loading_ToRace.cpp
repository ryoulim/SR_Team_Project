// �� Ŭ���� �̸� : Loading_ToRace
// �θ� Ŭ���� �̸� : LoadingCutscene

#include "Loading_ToRace.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CLoading_ToRace::CLoading_ToRace(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLoadingCutscene{ pGraphic_Device }
{
}

CLoading_ToRace::CLoading_ToRace(const CLoading_ToRace& Prototype)
	: CLoadingCutscene(Prototype)
{
}

HRESULT CLoading_ToRace::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoading_ToRace::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Loading_ToRace");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	// Ready_ShaderComponent(); �ʿ��� �� �߰�

	if (FAILED(m_pTextureCom->Get_TextureSize(0, &m_vSize)))
		return E_FAIL;
	// ���κ� ��� �̹��� ����
	m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;
	m_pTransformCom->Scaling(m_vSize);

	return S_OK;
}


void CLoading_ToRace::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLoading_ToRace::Update(_float fTimeDelta)
{
	//if (m_fLoadingGauge < m_fCurLoadingGauge)		// ���� �ε� ������ ����� ������ ��� ���
	//	m_fLoadingGauge += fTimeDelta * 0.8f;

	return __super::Update(fTimeDelta);
}

void CLoading_ToRace::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CLoading_ToRace::Render()
{
	Render_Background();
	return S_OK;
}

HRESULT CLoading_ToRace::Render_Background()
{
	if (FAILED(m_pTextureCom->Bind_Resource(_int(m_fTextureNum))))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	return S_OK;
}

CLoading_ToRace* CLoading_ToRace::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_ToRace* pInstance = new CLoading_ToRace(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoading_ToRace");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoading_ToRace::Clone(void* pArg)
{
	CLoading_ToRace* pInstance = new CLoading_ToRace(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLoading_ToRace");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_ToRace::Free()
{
	__super::Free();
}