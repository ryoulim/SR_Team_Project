#include "RaceBossDie.h"
#include "GameInstance.h"


CRaceBossDie::CRaceBossDie(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CPSystem(pGraphicDev, _strObjName)
{
}

CRaceBossDie::CRaceBossDie(const CPSystem& Prototype)
	: CPSystem(Prototype)
{
}

HRESULT CRaceBossDie::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fFrame = GetRandomFloat(0.f, m_fAnimationMaxFrame);

	return S_OK;
}

HRESULT CRaceBossDie::Ready_Particle()
{

	m_dwFVF = Particle::FVF;
	m_dwVtxSize = sizeof(Particle);


	HRESULT hr;
	hr = m_pGraphic_Device->CreateVertexBuffer(
		m_vbSize * m_dwVtxSize,
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		Particle::FVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0
	);

	return S_OK;
}

HRESULT CRaceBossDie::Ready_Components(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	//트랜스폼 컴포넌트 장착
	CTransform::DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransForm), &TransformDesc)))
		return E_FAIL;

	//텍스처 컴포넌트 장착
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_Texture_")) + pDesc->szTextureTag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	//셰이더 장착
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Particle"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

void CRaceBossDie::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;

	// 1. 위치: 보스 중심 기준, 살짝 퍼지게
	attribute->_Position = m_vPosition;
	attribute->_Position.x += GetRandomFloat(-200.f, 200.f);
	attribute->_Position.y += GetRandomFloat(-3.f, 3.f);
	attribute->_Position.z += GetRandomFloat(-200.f, 200.f);  // 살짝만 퍼짐

	// 2. 속도: 플레이어 쪽(-Z)으로 강하게 + X, Y로 퍼지기
	attribute->_Velocity.x = GetRandomFloat(-20.f, 20.f);  // 좌우로 약간
	attribute->_Velocity.y = GetRandomFloat(20.f, 1000.f);   // 위로 튐 (물보라 느낌)
	attribute->_Velocity.z = GetRandomFloat(-150.f, m_fNum);  // 강하게 Z-방향으로!

	// 3. 중력: 아래로 떨어짐
	attribute->_Accelerator = { 0.0f, -980.0f, 0.0f }; // 현실적인 중력값

	// 4. 크기: 작고 뾰족한 물방울 느낌
	attribute->_Size = GetRandomFloat(3.0f, 6.0f);

	// 5. 수명: 빠르게 사라짐
	attribute->_Age = 0.0f;
	attribute->_LifeTime = GetRandomFloat(0.3f, 4.5f);
	attribute->_isInitialized += 1;
}

void CRaceBossDie::SetPosition(_float3 Position)
{
	m_vPosition = Position;
}

EVENT CRaceBossDie::Update(_float timeDelta)
{
	list<Attribute>::iterator i;
	for (i = m_Particles.begin(); i != m_Particles.end(); i++)
	{
		//생존한 파티클만 갱신한다.
		if (i->_isAlive)
		{
			i->_Velocity += i->_Accelerator * timeDelta;
			i->_Position += i->_Velocity * timeDelta;


			i->_Age += timeDelta;

			if (i->_Age > i->_LifeTime)
			{
				resetParticle(&(*i));
			}
		}
	}

	if (m_bDead)
	{
		int a = 0;
		return EVN_DEAD;
	}

	return EVN_NONE;
}

HRESULT CRaceBossDie::SetUp_RenderState()
{
	//기본 셋팅
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE, FtoDW(m_fSize));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.1f));
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//김현석 : 레이싱 맵 조명 떄문에 넣었습니다.
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	//거리에 따른 파티클 크기조절
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(1.0f));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.05f));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(0.005f));



#pragma region 텍스처의 알파(온/오프)
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
#pragma endregion


	//월드 변환
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());

	return S_OK;
}
HRESULT CRaceBossDie::Render()
{
	//렌더 상태를 지정한다.
	SetUp_RenderState();
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	if (!m_Particles.empty())
	{
		//m_pTextureCom->Bind_Resource(0);
		m_pGraphic_Device->SetTexture(0, nullptr);
		m_pGraphic_Device->SetFVF(Particle::FVF);
		m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(Particle));

		//버텍스 버퍼를 벗어날 경우 처음부터 시작한다.
		if (m_vbOffset >= m_vbSize)
			m_vbOffset = 0;

		Particle* v = 0;

		m_pVB->Lock(
			m_vbOffset * sizeof(Particle),
			m_vbBatchSize * sizeof(Particle),
			(void**)&v,
			m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);


		DWORD numParticlesInBatch = 0;

		//모든 파티클이 렌더될 때 까지

		list<Attribute>::iterator i;
		for (i = m_Particles.begin(); i != m_Particles.end(); i++)
		{
			i->_Color = D3DXCOLOR(
				GetRandomFloat(10.f, 100.f) / 255.f,   // R:  아주 진하거나, 청록에 가까운 느낌
				GetRandomFloat(100.f, 200.f) / 255.f,  // G:  청록~푸른 초록
				GetRandomFloat(180.f, 255.f) / 255.f,  // B:  파란 느낌 유지
				1.f
			);


			if (i->_isAlive)
			{
				//한 단계의 세그먼트에서 생존한 파티클을 다음 버텍스 버퍼 세그먼트로 복사한다.
				v->_Position = i->_Position;
				v->_Color = (D3DCOLOR)i->_Color;
				v->_Size = i->_Size;
				v++;
				numParticlesInBatch++;

				//현재 단계의 버텍스버퍼가 모두 채워져있는가?
				if (numParticlesInBatch == m_vbBatchSize)
				{
					//버텍스 버퍼로 복사된 마지막 단계의 파티클을 그린다.
					m_pVB->Unlock();

					if (i->_isInitialized == 1)
						m_pGraphic_Device->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, m_vbBatchSize);

					//단계가 그려지는 동안 다음 단계를 파티클로 채운다.
					m_vbOffset += m_vbBatchSize;

					//버텍스 버퍼의 경계를 넘는 메모리로 오프셋 설정X , 넘을 경우 처음부터
					if (m_vbOffset >= m_vbSize)
						m_vbOffset = 0;

					m_pVB->Lock(
						m_vbOffset * sizeof(Particle),
						m_vbBatchSize * sizeof(Particle),
						(void**)&v,
						m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0; //다음 단계를 위한 리셋
				}
			}
		}

		m_pVB->Unlock();

		//마지막 단계가 렌더링되지않는 경우의 예외처리

		//if (numParticlesInBatch)
		//{
		//	m_pGraphic_Device->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, numParticlesInBatch);
		//}

		//다음블록
		m_vbOffset += m_vbBatchSize;

	}

	//렌더정리
	Release_RenderState();

	return S_OK;

}
HRESULT CRaceBossDie::Release_RenderState()
{

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, true);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// 블렌드 모드를 기본값으로 변경 (SRCALPHA → 기본값은 SRC)
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	return S_OK;
}

CRaceBossDie* CRaceBossDie::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CRaceBossDie* pInstance = new CRaceBossDie(pGraphicDev, _strObjName);

	//파티클 정보
	pInstance->m_vbSize = 100;				//  GPU가 한번에 그릴 수 있는 파티클 개수, CPU가 GPU로 파티클 정점 버퍼에 담을 수 있는 개수
	pInstance->m_fSize = 10.f;				//  파티클의 크기
	pInstance->m_vbOffset = 0;				//  세그먼트의 배치사이즈를 옮길때 쓰는 오프셋(0고정)
	pInstance->m_vbBatchSize = 100;			//  세그먼트 배치사이즈 크기(한번에 옮길 수 있는 정점들의 개수)
	pInstance->m_vMin = _float3{ 0.f,0.f,0.f };				//  바운딩박스의 최소크기
	pInstance->m_vMax = _float3{ 1.f,1.f,1.f };				//  바운딩박스의 최대크기

	return pInstance;
}

CGameObject* CRaceBossDie::Clone(void* pArg)
{
	CRaceBossDie* pInstance = new CRaceBossDie(*this);
	pInstance->m_isClone = true;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CRaceBossDie");
		Safe_Release(pInstance);
	}

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CRaceBossDie::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
}
