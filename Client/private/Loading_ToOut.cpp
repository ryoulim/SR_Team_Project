// 내 클래스 이름 : Loading_ToOut
// 부모 클래스 이름 : LoadingCutscene

#include "Loading_ToOut.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CLoading_ToOut::CLoading_ToOut(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLoadingCutscene{ pGraphic_Device }
{
}

CLoading_ToOut::CLoading_ToOut(const CLoading_ToOut& Prototype)
	: CLoadingCutscene(Prototype)
{
}

HRESULT CLoading_ToOut::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoading_ToOut::Initialize(void* pArg)
{
	m_szTextureID = TEXT("Loading_ToOut");
	m_szBufferType = TEXT("Rect");

	m_arrTexSizePos[0] = { { 297.f, 594.f, 1.f }, { iUIX + 188.f, iUIY + 594.f, 0.9f  } };
	m_arrTexSizePos[1] = { { 241.f, 491.f, 1.f }, { iUIX + 705.f, iUIY - 245.f, 0.9f  } };
	m_arrTexSizePos[2] = { { 375.f, 659.f, 1.f }, { iUIX + 1023.f, iUIY - 390.f - 100.f, 0.9f } };
	m_arrTexSizePos[3] = { { 314.f, 293.f, 1.f }, { iUIX + 173.f, iUIY - 143.f, 0.9f  } };
	m_arrTexSizePos[4] = { { 580.f, 561.f, 1.f }, { iUIX + 519.f, iUIY - 414.f, 0.9f  } };
	m_arrTexSizePos[5] = { { 431.f, 1063.f, 1.f}, { iUIX + 995.f, iUIY - 188 - 1063.f, 0.9f  } };

	m_arrTexLastPos[0] = { iUIX + 188.f, iUIY - 297.f, 0.9f  };
	m_arrTexLastPos[1] = { iUIX + 705.f, iUIY - 245.f, 0.9f  };
	m_arrTexLastPos[2] = { iUIX + 1023.f,iUIY - 390.f, 0.9f  };
	m_arrTexLastPos[3] = { iUIX + 173.f, iUIY - 143.f, 0.9f  };
	m_arrTexLastPos[4] = { iUIX + 519.f, iUIY - 414.f, 0.9f  };
	m_arrTexLastPos[5] = { iUIX + 995.f, iUIY - 188.f, 0.9f  };

	for (size_t i = 0; i < 6; i++)
	{
		m_bRenderOk[i] = false;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	 Ready_ShaderComponent();

	if (FAILED(m_pTextureCom->Get_TextureSize(0, &m_vSize)))
		return E_FAIL;
	// 세로비 대로 이미지 맞춤
	m_vSize.x *= g_iWinSizeY / m_vSize.y; m_vSize.y = g_iWinSizeY;
	m_pTransformCom->Scaling(m_vSize);

	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sound_Loading_ToOut"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	m_pSoundCom->SetVolume("largecannon2", 0.5f);
	m_pSoundCom->SetVolume("gunshell_01", 1.f);
	m_pSoundCom->SetVolume("shot_004", 1.f);
	m_pSoundCom->SetVolume("tile_run001", 0.8f);
	m_pSoundCom->SetVolume("tile_run002", 0.8f);
	m_pSoundCom->SetVolume("tile_run003", 0.8f);
	m_fWindVolume = 0.4f;
	m_pSoundCom->SetVolume("wind_longloop1", m_fWindVolume);
	m_pSoundCom->SetVolume("wind_heavy", m_fWindOutVolume);
	// 아래에 셋볼륨 업데이트중
	m_pSoundCom->Play("wind_longloop1");
	m_pSoundCom->Set_Loop("wind_longloop1", -1);

	m_pSoundCom->Play("wind_heavy");
	m_pSoundCom->Set_Loop("wind_heavy", -1);


	m_pSoundCom->Play("scream_crowd");
	m_pSoundCom->Play("largecannon2");

	return S_OK;
}

void CLoading_ToOut::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

EVENT CLoading_ToOut::Update(_float fTimeDelta)
{
	Set_CutPosition(fTimeDelta);
	return __super::Update(fTimeDelta);
}

void CLoading_ToOut::Late_Update(_float fTimeDelta)
{
	CUI::Late_Update(fTimeDelta);
}

HRESULT CLoading_ToOut::Render()
{
	Render_Background();
	if (m_iCurCut < 3)
	{
		Render_FirstCutscene();
	}
	else
	{
		Render_SecondCutscene();
	}
	return S_OK;
}

HRESULT CLoading_ToOut::Render_Background()
{
	m_pTransformCom->Scaling(_float3{1280.f, 720.f, 1.f});
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3{0.f, 0.f, 0.9f});

	if (FAILED(m_pTextureCom->Bind_Resource(_int(6))))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CLoading_ToOut::Render_FirstCutscene()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (_uint i = 0; i < 3; i++)
	{
		if (m_bRenderOk[i] == false)
			break;
		if (FAILED(m_pTextureCom->Bind_Resource((i))))
			return E_FAIL;

		m_pTransformCom->Scaling(m_arrTexSizePos[i].vSize);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_arrTexSizePos[i].vPos);

		if (FAILED(m_pTransformCom->Bind_Resource()))
			return E_FAIL;
		m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "Tex", i); // 텍스쳐 바인딩
		if (i == 2)
		{
			_float dist = fabsf(m_arrTexLastPos[i].y - m_arrTexSizePos[i].vPos.y);
			m_pShaderCom->SetFloat("opacity", 1.f - dist / 100.f);
		}
		else
			m_pShaderCom->SetFloat("opacity", 1.f);

		m_pShaderCom->Begin(CShader::ALPHA);
		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
		m_pShaderCom->End();
	}
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

HRESULT CLoading_ToOut::Render_SecondCutscene()
{
	for (_uint i = 3; i < 6; i++)
	{
		if (m_bRenderOk[i] == false)
			break;
		if (FAILED(m_pTextureCom->Bind_Resource((i))))
			return E_FAIL;

		m_pTransformCom->Scaling(m_arrTexSizePos[i].vSize);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_arrTexSizePos[i].vPos);

		if (FAILED(m_pTransformCom->Bind_Resource()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
		if (m_bRenderOk[3] == true && m_bRenderOk[5] == false)
		{
			m_fWindVolume = LERP(m_fWindVolume, 0.f, 0.003f);
			m_fWindOutVolume = LERP(m_fWindOutVolume, 0.5f, 0.003f);
			m_pSoundCom->SetVolume("wind_longloop1", m_fWindVolume);
			m_pSoundCom->SetVolume("wind_heavy", m_fWindOutVolume);
		}

	}
	return S_OK;
}

void CLoading_ToOut::Set_CutPosition(_float fTimeDelta)
{
// 아 진심 실화냐? 이게뭐냐? 영상으로틀고싶다..

#define CUTSPEED 700.f;


	/* 컷씬 스킵 엔터 ㅋ */
	if (KEY_DOWN(DIK_RETURN)) { m_arrTexSizePos[m_iCurCut].vPos.y = m_arrTexLastPos[m_iCurCut].y; m_fWaitForNextCut = 0.f; m_iCurCut++; if (m_iCurCut > 5) { m_iCurCut = 5; m_isReadyToChangeLevel = true; } }
	
	m_bRenderOk[m_iCurCut] = true; // 컷이 재생되기 시작함
	switch (m_iCurCut)
	{
	case 0: // 0번째 컷
		m_arrTexSizePos[m_iCurCut].vPos.y -= fTimeDelta * CUTSPEED; // 컷 이동시키기
		if (m_arrTexSizePos[m_iCurCut].vPos.y <= m_arrTexLastPos[m_iCurCut].y) // 컷 이동 완료 시
		{
			m_arrTexSizePos[m_iCurCut].vPos.y = m_arrTexLastPos[m_iCurCut].y;
			m_fWaitForNextCut += fTimeDelta*1.5f; // 다음 컷 재생 까지 조금 기다림
			if (m_fWaitForNextCut >= 2.f)
			{
				m_fWaitForNextCut = 0.f; // 다 기다렸으면 변수 초기화 후 
				m_iCurCut = 1;			 // 다음 컷으로 이동	
				m_pSoundCom->Play("shot_004");
				m_pSoundCom->Play("gunshell_01");
			}
		}
		break;
	case 1: // 그걸졸라반복하기
		m_fWaitForNextCut += fTimeDelta * 1.5f; // 다음 컷 재생 까지 조금 기다림
		if (m_fWaitForNextCut >= 1.f)
		{
			m_fWaitForNextCut = 0.f; // 다 기다렸으면 변수6 초기화 후 
			m_iCurCut = 2;			 // 다음 컷으로 이동	
		}
		break;
	case 2:
		m_arrTexSizePos[m_iCurCut].vPos.y += fTimeDelta * 0.3f * CUTSPEED; // 컷 이동시키기
		if (m_arrTexSizePos[m_iCurCut].vPos.y >= m_arrTexLastPos[m_iCurCut].y) // 컷 이동 완료 시
		{
			m_arrTexSizePos[m_iCurCut].vPos.y = m_arrTexLastPos[m_iCurCut].y;
			m_fWaitForNextCut += fTimeDelta * 1.5f; // 다음 컷 재생 까지 조금 기다림
			if (m_fWaitForNextCut >= 2.f)
			{
				m_fWaitForNextCut = 0.f; // 다 기다렸으면 변수 초기화 후 
				m_iCurCut = 3;			 // 다음 컷으로 이동	
				m_pSoundCom->Play("tile_run001");
			}
		}
		break;
	case 3:
		m_fWaitForNextCut += fTimeDelta * 1.5f; // 다음 컷 재생 까지 조금 기다림
		if (_int(m_fWaitForNextCut * 1000.f) == 500)
			m_pSoundCom->Play("tile_run002");
		if (_int(m_fWaitForNextCut * 1000.f) == 1000)
			m_pSoundCom->Play("tile_run003");
		if (m_fWaitForNextCut >= 1.5f)
		{
			m_fWaitForNextCut = 0.f; // 다 기다렸으면 변수 초기화 후 
			m_iCurCut = 4;			 // 다음 컷으로 이동	
			m_pSoundCom->Play("tile_run002");
		}
		break;
	case 4:
		m_fWaitForNextCut += fTimeDelta * 1.5f; // 다음 컷 재생 까지 조금 기다림
		if (_int(m_fWaitForNextCut * 1000.f) == 500)
			m_pSoundCom->Play("tile_run003");
		if (_int(m_fWaitForNextCut * 1000.f) == 1000)
			m_pSoundCom->Play("tile_run001");
		if (m_fWaitForNextCut >= 1.5f)
		{
			m_fWaitForNextCut = 0.f; // 다 기다렸으면 변수 초기화 후 
			m_iCurCut = 5;			 // 다음 컷으로 이동
			m_pSoundCom->Play("tile_run003");
			m_pSoundCom->Stop("wind_longloop1");
			//m_pSoundCom->Play("")
		}
		break;
	case 5:
		m_arrTexSizePos[m_iCurCut].vPos.y += fTimeDelta * 0.6f * CUTSPEED; // 컷 이동시키기
		if (m_arrTexSizePos[m_iCurCut].vPos.y >= m_arrTexLastPos[m_iCurCut].y) // 컷 이동 완료 시
		{
			m_arrTexSizePos[m_iCurCut].vPos.y = m_arrTexLastPos[m_iCurCut].y;
			m_fWaitForNextCut += fTimeDelta * 1.5f; // 다음 컷 재생 까지 조금 기다림
			if (m_fWaitForNextCut >= 2.5f)
			{
				m_fWaitForNextCut = 0.f; // 다 기다렸으면 변수 초기화 후 
				m_isReadyToChangeLevel = true;
				m_iCurCut = 6;			 // 다음 컷으로 이동	
			}
		}
		break;
	default:
		m_isReadyToChangeLevel = true;
		return;
	}
}

CLoading_ToOut* CLoading_ToOut::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoading_ToOut* pInstance = new CLoading_ToOut(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoading_ToOut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoading_ToOut::Clone(void* pArg)
{
	CLoading_ToOut* pInstance = new CLoading_ToOut(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLoading_ToOut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_ToOut::Free()
{
	__super::Free();
}