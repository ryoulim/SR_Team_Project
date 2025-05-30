// 내 클래스 이름 : Font_Racing
// 부모 클래스 이름 : Font

#include "Font_Racing.h"
#include "GameInstance.h"

CFont_Racing::CFont_Racing(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CFont{ pGraphic_Device }
{
}

CFont_Racing::CFont_Racing(const CFont_Racing& Prototype)
	: CFont(Prototype)
{
}

HRESULT CFont_Racing::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFont_Racing::Initialize(void* pArg)
{
	m_eLevelID = LEVEL_STATIC;
	m_szTextureID = TEXT("Font_Racing");
	m_szBufferType = TEXT("Rect");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTextureCom->Get_TextureSize(static_cast<_uint>(0), &m_vSize);
	m_vSize *= 1.42f;


	return S_OK;
}

void CFont_Racing::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CFont_Racing::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CFont_Racing::Late_Update(_float fTimeDelta)
{
	if (!LEVEL_RACING(m_pGameInstance->Get_CurrentLevelIndex()))
		return;
	__super::Late_Update(fTimeDelta);
}

HRESULT CFont_Racing::Render()
{
	return __super::Render();
	return S_OK;
}

HRESULT CFont_Racing::Render_Number(const _int _Val)
{
	_int iCopy = _Val;
	_float startPosX = m_vSize.x;

	if (iCopy == 0)
	{
		_int iNum = iCopy % 10;
		m_fTextureNum = _float(iNum);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(startPosX, g_iWinSizeY * -0.5f + 53.f, 0.5f));
		Render();
		return S_OK;
	}
	else if (iCopy > 1000)
		iCopy = 999;
	while (iCopy)
	{
		_int iNum = iCopy % 10;
		m_fTextureNum = _float(iNum);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(startPosX, g_iWinSizeY * -0.5f + 53.f, 0.5f));
		Render();
		startPosX -= m_vSize.x;
		iCopy /= 10;
	}

	return S_OK;
}

CFont_Racing* CFont_Racing::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFont_Racing* pInstance = new CFont_Racing(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFont_Racing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFont_Racing::Clone(void* pArg)
{
	CFont_Racing* pInstance = new CFont_Racing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CFont_Racing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFont_Racing::Free()
{
	__super::Free();
}