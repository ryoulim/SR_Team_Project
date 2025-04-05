#include "ScreenSprite.h"
#include "GameInstance.h"
#include "Particle_Define.h"
#include "FXMgr.h"

CScreenSprite::CScreenSprite(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CEffect{ pGraphic_Device }
{
	/* [ 이 클래스에는 애니메이션이 데드카운트 ] */
}

CScreenSprite::CScreenSprite(const CScreenSprite& Prototype)
	: CEffect(Prototype)
{
}

HRESULT CScreenSprite::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScreenSprite::Initialize(void* pArg)
{
	CScreenSprite::DESC* SpriteDESC = static_cast<DESC*>(pArg);

	//뷰포트 크기
	D3DVIEWPORT9            ViewportDesc{};
	m_pGraphic_Device->GetViewport(&ViewportDesc);

	//멤버변수 저장
	m_eLevelID = LEVEL_STATIC;
	m_fAnimationMaxFrame = SpriteDESC->fMaxFrame;
	m_szTextureID = SpriteDESC->szTextureTag;
	m_fAnimationSpeed = SpriteDESC->fAniSpeed;
	m_eEffectType = SpriteDESC->eEffectType;
	m_szBufferType = TEXT("Rect");

	m_fX = SpriteDESC->vInitPos.x;
	m_fY = SpriteDESC->vInitPos.y;
	m_fSizeX = SpriteDESC->vScale.x;
	m_fSizeY = SpriteDESC->vScale.y;

	/* 컴포넌트를 붙이자 */
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	/* 이 클래스는 셰이더가 필요합니다! */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Particle"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* 스크린에 붙이자 */
	m_pTransformCom->Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		_float3(SpriteDESC->vInitPos.x - ViewportDesc.Width / 2,
			-(SpriteDESC->vInitPos.y - ViewportDesc.Height / 2),
			SpriteDESC->vInitPos.z));


	/* 만약 FX 매니저에서 이미 HIT 이 진행 중이라면? */
	if (FX_MGR->GetbHit())
		m_bDead = true;
	else 
		FX_MGR->SetbHit(true);


	/* [ 맞았을 경우 랜덤한 이펙트 ] */
	if (m_eEffectType == HIT)
		m_iRandom = GetRandomInt(0, 1);

	/* [ 힐 이펙트 조절 ] */
	if (m_eEffectType == HEAL)
		m_fAccTime = 1.f;

	/* 이 스크린의 뎁스를 정해주세요 */
	m_fDepth = 0.001f;

	return S_OK;
}

void CScreenSprite::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CScreenSprite::Update(_float fTimeDelta)
{
	/* [ 오파시티 내려가는 중 ] */
	m_fAccTime -= m_fFadeSpeed * fTimeDelta;
	if (m_fAccTime < 0.f)
		m_fAccTime = 0.f;

	/* [ 데드카운트 작동 ] */
	//FrameUpdate(fTimeDelta, m_fAnimationMaxFrame, m_fAnimationSpeed, false);
	//if (m_fAnimationMaxFrame <= m_fAnimationFrame || m_bDead)
	//{
	//	FX_MGR->SetbHit(false);
	//	return EVN_DEAD;
	//}

	/* [ 약간 상승 애니메이션 ] */
	if (m_eEffectType == HEAL)
	{
		m_pTransformCom->Go_UpCustom(fTimeDelta, 20.f, 1000.f);
	}

	return __super::Update(fTimeDelta);
}

void CScreenSprite::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CScreenSprite::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	return S_OK;
}

HRESULT CScreenSprite::Render()
{
#pragma region 정점렌더링 주석

	//SetUp_RenderState();
	//
	////텍스처 셰이더로 넘기기
	//m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "g_Texture", m_iRandom);
	//
	////메트릭스 셰이더로 넘기기
	//_float4x4 maxWorld, maxView, maxProj;
	//m_pGraphic_Device->GetTransform(D3DTS_WORLD, &maxWorld);
	//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &maxView);
	//m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &maxProj);
	//
	//if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &maxWorld)))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &maxView)))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &maxProj)))
	//	return E_FAIL;
	//
	//m_pShaderCom->SetFloat("g_fOpacity", m_fAccTime);
	//
	////셰이더 시작
	//m_pShaderCom->Begin(CShader::OPACITY);
	//
	//if (FAILED(m_pVIBufferCom->Bind_Buffers()))
	//	return E_FAIL;
	//
	//
	///* ------------------------------------------------------------------------- */
	//
	//if (FAILED(m_pVIBufferCom->Render()))
	//	return E_FAIL;
	//
	///* ------------------------------------------------------------------------- */
	//
	//m_pShaderCom->End();
	//
	//
	//Release_RenderState();

#pragma endregion

	/* 월드 / 뷰 / 직교 투영 행렬 준비 */
	_float4x4 matWorld, matView, matOrtho;

	matWorld = *m_pTransformCom->Get_WorldMatrix();
	D3DXMatrixIdentity(&matView);

	matOrtho.MakeOrthoProjMat(FWINCX, FWINCY, 0.f, 1.f);

	// 셰이더로 행렬 전달
	m_pShaderCom->Bind_Matrix("g_WorldMatrix", &matWorld);
	m_pShaderCom->Bind_Matrix("g_ViewMatrix", &matView);
	m_pShaderCom->Bind_Matrix("g_ProjMatrix", &matOrtho);

	// 기타 리소스 바인딩
	m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "g_Texture", m_iRandom);
	m_pShaderCom->SetFloat("g_fOpacity", m_fAccTime);

	SetUp_RenderState();  // 알파 블렌딩 같은 상태 세팅

	if (m_eEffectType == HEAL)
	{
		m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pShaderCom->Begin(CShader::HEAL);
	}
	else
	{
		m_pShaderCom->Begin(CShader::HIT);
	}

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pShaderCom->End();
	Release_RenderState();  // 렌더 상태 원복

	return S_OK;
}

HRESULT CScreenSprite::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

CScreenSprite* CScreenSprite::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScreenSprite* pInstance = new CScreenSprite(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CScreenSprite");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CScreenSprite::Clone(void* pArg)
{
	CScreenSprite* pInstance = new CScreenSprite(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CScreenSprite");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScreenSprite::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
}