#include "Smoke.h"
#include "GameInstance.h"


CSmoke::CSmoke(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CPSystem(pGraphicDev, _strObjName)
{
}

CSmoke::CSmoke(const CPSystem& Prototype)
	: CPSystem(Prototype)
{
}

HRESULT CSmoke::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	DESC* SmokeDESC = static_cast<DESC*>(pArg);
	m_vPosition = SmokeDESC->vPosition;

	int numparticles = 10;
	for (int i = 0; i < numparticles; i++)
	{
		addParticle();
	}


	return S_OK;
}

HRESULT CSmoke::Ready_Particle()
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

HRESULT CSmoke::Ready_Components()
{
	//트랜스폼 컴포넌트 장착
	CTransform::DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransForm), &TransformDesc)))
		return E_FAIL;

	//텍스처 컴포넌트 장착
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Small_Smoke"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CSmoke::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;

	//분수형 폭팔 로직
	attribute->_Position = m_vPosition;
	_float3 min = { -2.f,  0.f, -2.f };
	_float3 max = { 2.f,  1.f,  2.f };
	GetRandomVector(&attribute->_Velocity, &min, &max);

	attribute->_Velocity.y = GetRandomFloat(1.f, 5.f);
	D3DXVec3Normalize(&attribute->_Velocity, &attribute->_Velocity);
	attribute->_Velocity *= 100.f;


	attribute->_Accelerator = { 0.0f, 50.f, 0.0f };		// 가속도
	attribute->_Age = 0.f;								// 나이
	attribute->_Color = WHITE;							// 색상
	attribute->_ColorFade = WHITE;						// 디졸브색상
	attribute->_LifeTime = GetRandomFloat(0.5f, 2.f); 	// 라이프타임

	//파티클 개별 사이즈는 그래픽 카드에서 지원안함 gg
	attribute->_Size = GetRandomFloat(m_fSize - 3.f, m_fSize + 3.f);
	//파티클 최대크기 지원안함 gg
	//뭐 지원하는게 없냐 애는..
}

EVENT CSmoke::Update(_float timeDelta)
{
	list<Attribute>::iterator i;
	for (i = m_Particles.begin(); i != m_Particles.end(); i++)
	{
		//생존한 파티클만 갱신한다.
		if (i->_isAlive)
		{
			// 속도 업데이트 (중력 적용)
			i->_Velocity += i->_Accelerator * timeDelta;
			i->_Position += i->_Velocity * timeDelta;

			i->_Velocity.x += GetRandomFloat(-0.1f, 0.1f) * timeDelta;
			i->_Velocity.z += GetRandomFloat(-0.1f, 0.1f) * timeDelta;

			//i->_Position += i->_Velocity * timeDelta;
			i->_Age += timeDelta;

			if (i->_Age > i->_LifeTime)
			{
				i->_isAlive = false;
				//resetParticle(&(*i));
			}
		}
	}


	//이거 고치긴해야함
	FrameUpdate(timeDelta);
	Late_Update();


	return EVN_NONE;
}


void CSmoke::FrameUpdate(float timeDelta)
{
	m_fFrame += 15.f * timeDelta;

	if (20.f < m_fFrame)
		m_fFrame = 0;
}

HRESULT CSmoke::Render()
{
	if (!m_Particles.empty())
	{
		//렌더 상태를 지정한다.
		SetUp_RenderState();

		//m_pTextureCom->Bind_Resource(0);
		m_pTextureCom->Bind_Resource((_uint)m_fFrame);
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


CSmoke* CSmoke::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CSmoke* pInstance = new CSmoke(pGraphicDev, _strObjName);

	//파티클 정보
	pInstance->m_vbSize = 2048;				//  GPU가 한번에 그릴 수 있는 파티클 개수, CPU가 GPU로 파티클 정점 버퍼에 담을 수 있는 개수
	pInstance->m_fSize = 10.f;				//  파티클의 크기
	pInstance->m_vbOffset = 0;				//  세그먼트의 배치사이즈를 옮길때 쓰는 오프셋(0고정)
	pInstance->m_vbBatchSize = 512;			//  세그먼트 배치사이즈 크기(한번에 옮길 수 있는 정점들의 개수)
	pInstance->m_vMin = _float3{ 0.f,0.f,0.f };				//  바운딩박스의 최소크기
	pInstance->m_vMax = _float3{ 1.f,1.f,1.f };				//  바운딩박스의 최대크기

	return pInstance;
}

float CSmoke::GetRandomColor(float lowBound, float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

CGameObject* CSmoke::Clone(void* pArg)
{
	CSmoke* pInstance = new CSmoke(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSmoke");
		Safe_Release(pInstance);
	}

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSmoke::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
