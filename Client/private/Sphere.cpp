#include "Sphere.h"
#include "GameInstance.h"


CSphere::CSphere(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CPSystem(pGraphicDev, _strObjName)
{
}

CSphere::CSphere(const CPSystem& Prototype)
	: CPSystem(Prototype)
{
}

HRESULT CSphere::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fFrame = GetRandomFloat(0.f, m_fAnimationMaxFrame);
	return S_OK;
}

HRESULT CSphere::Ready_Particle()
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

HRESULT CSphere::Ready_Components(void* pArg)
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

	return S_OK;
}

void CSphere::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;

	//초기 위치 (반지름 30.0f 범위 내에서 랜덤한 원형 배치)
	float angle = GetRandomFloat(0.0f, D3DX_PI * 2.0f);
	m_fRadius = GetRandomFloat(m_fMin, m_fMax);
	attribute->_Position.x = cos(angle) * m_fRadius;
	attribute->_Position.y = GetRandomFloat(0.0f, -25.0f);
	attribute->_Position.z = sin(angle) * m_fRadius;


	//나선형 상승을 위한 초기 속도
	attribute->_Velocity.x = -sin(angle) * 100.0f; // 반대 방향으로 돌면서 상승
	attribute->_Velocity.y = GetRandomFloat(30.0f, 50.0f); // 위로 상승
	attribute->_Velocity.z = cos(angle) * 100.0f;


	//중심으로 서서히 끌어당기는 가속도 설정 (목표점 - 현재위치)
	attribute->_Accelerator = (m_vCenter - attribute->_Position) * 2.f;

	//회오리 효과를 추가하기 위한 미세한 방향 랜덤 값
	attribute->_Velocity.x += GetRandomFloat(-5.0f, 50.0f);
	attribute->_Velocity.z += GetRandomFloat(-5.0f, 50.0f);

	//페이드 아웃을 위한 색상 조절
	attribute->_Color = WHITE;
	attribute->_ColorFade = WHITE; // 사라지는 색상

	//수명 설정 (2~5초 동안 지속)
	attribute->_Age = GetRandomFloat(-1.f, 3.f);
	attribute->_LifeTime = GetRandomFloat(1.0f, 3.0f);
}

EVENT CSphere::Update(_float timeDelta)
{
	static float SphereElapsed = 0.0f; // 시간을 누적할 변수
	SphereElapsed += timeDelta; // 프레임마다 증가

	//목표 중심이 천천히 왕복하도록 설정 (부드럽게 움직이게)
	m_vCenter.x = 100.0f + sin(SphereElapsed * 2.f) * 50.0f;
	m_vCenter.z = 50.0f + sin(SphereElapsed * 4.f) * 100.0f;



	list<Attribute>::iterator i;
	for (i = m_Particles.begin(); i != m_Particles.end(); i++)
	{
		//생존한 파티클만 갱신한다.
		if (i->_isAlive)
		{
			//중력 및 목표점으로 가속도 적용 (점점 중심으로 수렴)
			i->_Velocity += i->_Accelerator * timeDelta;

			//나선형을 더 강조하기 위해 X, Z 회전 효과 추가
			float rotationSpeed = 5.0f;
			float newX = i->_Velocity.x * cos(rotationSpeed * timeDelta) - i->_Velocity.z * sin(rotationSpeed * timeDelta);
			float newZ = i->_Velocity.x * sin(rotationSpeed * timeDelta) + i->_Velocity.z * cos(rotationSpeed * timeDelta);
			i->_Velocity.x = newX;
			i->_Velocity.z = newZ;

			//위치 업데이트 (속도를 반영하여 이동)
			i->_Position += i->_Velocity * timeDelta;

			//페이드 아웃 효과 (시간이 지날수록 색이 어두워짐)
			i->_Color.a -= 0.3f * timeDelta;
			if (i->_Color.a < 0.0f) i->_Color.a = 0.0f;


			i->_Age += timeDelta;

			if (i->_Age > i->_LifeTime)
			{
				if (m_bIsLoop)
				{
					resetParticle(&(*i));
				}
				else
				{
					i->_isAlive = false;
				}				
			}
		}
	}

	FrameUpdate(timeDelta);

	removeDeadParticle();
	if (m_Particles.empty())
	{
		return EVN_DEAD;
	}

	return EVN_NONE;
}


void CSphere::FrameUpdate(float timeDelta)
{
	m_fFrame += 15.f * timeDelta;

	if (m_fAnimationMaxFrame < m_fFrame)
		m_fFrame = 0;
}
HRESULT CSphere::SetUp_RenderState()
{
	//기본 셋팅
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE, FtoDW(m_fSize));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.1f));
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//거리에 따른 파티클 크기조절
	//m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(1.0f));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.05f));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(0.005f));



#pragma region 텍스처의 알파(온/오프)
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 128);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
#pragma endregion


	//월드 변환
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());

	return S_OK;
}
HRESULT CSphere::Render()
{
	if (!m_Particles.empty())
	{
		//렌더 상태를 지정한다.
		SetUp_RenderState();

		m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		m_pTextureCom->Bind_Resource(0);
		//m_pGraphic_Device->SetTexture(0, nullptr);

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
HRESULT CSphere::Release_RenderState()
{

	//m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// 블렌드 모드를 기본값으로 변경 (SRCALPHA → 기본값은 SRC)
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

CSphere* CSphere::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CSphere* pInstance = new CSphere(pGraphicDev, _strObjName);

	//파티클 정보
	pInstance->m_vbSize = 2048;				//  GPU가 한번에 그릴 수 있는 파티클 개수, CPU가 GPU로 파티클 정점 버퍼에 담을 수 있는 개수
	pInstance->m_fSize = 2.f;				//  파티클의 크기
	pInstance->m_vbOffset = 0;				//  세그먼트의 배치사이즈를 옮길때 쓰는 오프셋(0고정)
	pInstance->m_vbBatchSize = 64;			//  세그먼트 배치사이즈 크기(한번에 옮길 수 있는 정점들의 개수)
	pInstance->m_vMin = _float3{ 0.f,0.f,0.f };				//  바운딩박스의 최소크기
	pInstance->m_vMax = _float3{ 1.f,1.f,1.f };				//  바운딩박스의 최대크기

	return pInstance;
}

CGameObject* CSphere::Clone(void* pArg)
{
	CSphere* pInstance = new CSphere(*this);
	pInstance->m_isClone = true;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSphere");
		Safe_Release(pInstance);
	}

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSphere::Free()
{
	__super::Free();
}
