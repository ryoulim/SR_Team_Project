#include "Rain.h"
#include "GameInstance.h"

CRain::CRain(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CPSystem(pGraphicDev, _strObjName)
{
}

CRain::CRain(const CPSystem& Prototype)
	: CPSystem(Prototype)
{
}

HRESULT CRain::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_Rain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

void CRain::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;

	//눈송이의 위치 지정을 위해임의의 x, z 좌표를 얻는다.
	GetRandomVector(&attribute->_Position,&m_vMin,&m_vMax);

	
	//눈송이는 아래쪽으로 떨어지며 약간 왼쪽을 향한다.
	attribute->_Velocity.x = GetRandomFloat(0.0f, 1.0f) * -3.0f;
	attribute->_Velocity.y = GetRandomFloat(1.0f, 5.0f) * -120.0f;
	attribute->_Velocity.z = 0.f;

	//각종 파라미터값
	attribute->_Accelerator = _float3(0.f, 0.f, 0.f);	// 가속도
	attribute->_Age = 0.f;								// 나이
	attribute->_Color = WHITE;							// 색상
	attribute->_ColorFade = WHITE;						// 디졸브색상
	attribute->_LifeTime = 1.f;							// 라이프타임

}

EVENT CRain::Update(_float timeDelta)
{
	list<Attribute>::iterator i;
	for (i = m_Particles.begin(); i != m_Particles.end(); i++)
	{
		i->_Position += i->_Velocity * timeDelta;

		// 포인트가 경계를 벗어났는가?  
		if (i->_Position.x < m_vMin.x || i->_Position.x > m_vMax.x ||
			i->_Position.y < m_vMin.y || i->_Position.y > m_vMax.y ||
			i->_Position.z < m_vMin.z || i->_Position.z > m_vMax.z)
		{
			//경계를 벗어난 파티클을 재활용하자
			resetParticle(&(*i));
		}
	}


	//이거 고치긴해야함
	Late_Update();

	return EVN_NONE;
}

HRESULT CRain::Render()
{
	if (!m_Particles.empty())
	{
		//렌더 상태를 지정한다.
		SetUp_RenderState();

		m_pTextureCom->Bind_Resource(0);
		//m_pTextureCom->Bind_Resource((_uint)m_fFrame);

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

HRESULT CRain::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

CRain* CRain::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName, int numparticles, _float3 MinBox, _float3 MaxBox)
{
	CRain* pInstance = new CRain(pGraphicDev, _strObjName);

	//스노우 파티클 정보
	pInstance->m_vbSize = 2048;		//  GPU가 한번에 그릴 수 있는 파티클 개수, CPU가 GPU로 파티클 정점 버퍼에 담을 수 있는 개수
	pInstance->m_fSize = 1.5f;		//  파티클의 크기
	pInstance->m_vbOffset = 0;		//  세그먼트의 배치사이즈를 옮길때 쓰는 오프셋(0고정)
	pInstance->m_vbBatchSize = 512;	//  세그먼트 배치사이즈 크기(한번에 옮길 수 있는 정점들의 개수)
	pInstance->m_vMin = MinBox;		//  바운딩박스의 최소크기
	pInstance->m_vMax = MaxBox;		//  바운딩박스의 최대크기

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	for (int i = 0; i < numparticles; i++)
	{
		pInstance->addParticle();
	}

	return pInstance;
}

CGameObject* CRain::Clone(void* pArg)
{
	CRain* pInstance = new CRain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CRain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRain::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
