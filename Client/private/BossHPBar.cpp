// �� Ŭ���� �̸� : BossHPBar
// �θ� Ŭ���� �̸� : UI

#include "BossHPBar.h"
#include "GameInstance.h"

#define HPBARMAX 191.f

CBossHPBar::CBossHPBar(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI{ pGraphic_Device }
{
}

CBossHPBar::CBossHPBar(const CBossHPBar& Prototype)
	: CUI(Prototype)
{
}

HRESULT CBossHPBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBossHPBar::Initialize(void* pArg)
{
	m_szTextureID = TEXT("BossHPBar");
	m_szBufferType = TEXT("Rect");


	// 254 40 1? �޸�
	// 191 11 1? �� 
	return __super::Initialize(pArg);
}

void CBossHPBar::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CBossHPBar::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CBossHPBar::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBossHPBar::Render()
{
	__super::Render();

	_float fHPPercent = *m_pBossHP / static_cast<_float>(m_iBossMaxHP);
	if (fHPPercent >= 1.f)
		fHPPercent = 1.f;
	_float fsize = HPBARMAX * fHPPercent;

	//����UI�� ������ 0,0�� ��
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		{ HPBARMAX * fHPPercent * 0.5f, // ���� HP ������ŭ�� ũ�� �� �߰� ������ŭ +�� ��ġ
		(g_iWinSizeY * 0.5f) - 45.f,
		0.01f });
	m_pTransformCom->Scaling({ fsize, 11.f, 1.f });


	if (FAILED(m_pTextureCom->Bind_Resource(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

CBossHPBar* CBossHPBar::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBossHPBar* pInstance = new CBossHPBar(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBossHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBossHPBar::Clone(void* pArg)
{
	CBossHPBar* pInstance = new CBossHPBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CBossHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossHPBar::Free()
{
	__super::Free();
}