#include "WaterBoat.h"
#include "GameInstance.h"


CWaterBoat::CWaterBoat(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CPSystem(pGraphicDev, _strObjName)
{
}

CWaterBoat::CWaterBoat(const CPSystem& Prototype)
	: CPSystem(Prototype)
{
}

HRESULT CWaterBoat::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fFrame = GetRandomFloat(0.f, m_fAnimationMaxFrame);

	if (m_fMin == 0 && m_fMax == 0)
	{
		m_fMin = -120;
		m_fMax = 120;
	}

	return S_OK;
}

HRESULT CWaterBoat::Ready_Particle()
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

HRESULT CWaterBoat::Ready_Components(void* pArg)
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

void CWaterBoat::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;

	// 1. 위치: 발 근처 (캐릭터 중앙 기준 좌우로 살짝 오프셋)
	int sideOffset = (rand() % 2 == 0) ? -1 : 1;
	attribute->_Position = m_vPosition;
	attribute->_Position.x += float(sideOffset * 8);
	attribute->_Position.y += -10.f;
	attribute->_Position.z += +10.f;

	// 2. 방향: 좌우로 벌어지며 퍼지는 효과
	float xSpeed = (sideOffset >= 0)
		? GetRandomFloat(10.f, m_fMax) : GetRandomFloat(m_fMin, -10.f);

	attribute->_Velocity = { xSpeed, GetRandomFloat(10.f, 20.f), GetRandomFloat(m_fNum - 200, m_fNum)};

	// 3. 중력: 아주 약하게 아래로 끌림 (혹은 없음)
	attribute->_Accelerator = { 0.0f, -60.0f, 0.0f };

	// 4. 비주얼
	attribute->_Size = GetRandomFloat(5.0f, 8.0f);  // 물방울 크기

	// 5. 수명
	attribute->_Age = 0.0f;
	attribute->_LifeTime = GetRandomFloat(0.05f, m_fLifeTime);
}

void CWaterBoat::SetPosition(_float3 Position)
{
	m_vPosition = Position;
}

EVENT CWaterBoat::Update(_float timeDelta)
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

HRESULT CWaterBoat::SetUp_RenderState()
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
HRESULT CWaterBoat::Render()
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

		if (numParticlesInBatch)
		{
			m_pGraphic_Device->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, numParticlesInBatch);
		}

		//다음블록
		m_vbOffset += m_vbBatchSize;

	}

	//렌더정리
	Release_RenderState();

	return S_OK;

}
HRESULT CWaterBoat::Release_RenderState()
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

CWaterBoat* CWaterBoat::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CWaterBoat* pInstance = new CWaterBoat(pGraphicDev, _strObjName);

	//파티클 정보
	pInstance->m_vbSize = 100;				//  GPU가 한번에 그릴 수 있는 파티클 개수, CPU가 GPU로 파티클 정점 버퍼에 담을 수 있는 개수
	pInstance->m_fSize = 10.f;				//  파티클의 크기
	pInstance->m_vbOffset = 0;				//  세그먼트의 배치사이즈를 옮길때 쓰는 오프셋(0고정)
	pInstance->m_vbBatchSize = 100;			//  세그먼트 배치사이즈 크기(한번에 옮길 수 있는 정점들의 개수)
	pInstance->m_vMin = _float3{ 0.f,0.f,0.f };				//  바운딩박스의 최소크기
	pInstance->m_vMax = _float3{ 1.f,1.f,1.f };				//  바운딩박스의 최대크기

	return pInstance;
}

CGameObject* CWaterBoat::Clone(void* pArg)
{
	CWaterBoat* pInstance = new CWaterBoat(*this);
	pInstance->m_isClone = true;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CWaterBoat");
		Safe_Release(pInstance);
	}

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CWaterBoat::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
}
