#include "BulletTracer.h"
#include "GameInstance.h"
#include "Transform.h"

CBulletTracer::CBulletTracer(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CPSystem(pGraphicDev, _strObjName)
{
}

CBulletTracer::CBulletTracer(const CPSystem& Prototype)
	: CPSystem(Prototype)
{
}

void CBulletTracer::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;
	
	_float4x4 matCamWorld;

	//카메라의 위치저장
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };
	

	//카메라 라이트,업,룩 벡터 저장
	_float3 vCameraRight = { matCamWorld._11, matCamWorld._12, matCamWorld._13 };
	_float3 vCameraUp = { matCamWorld._21, matCamWorld._22, matCamWorld._23 };
	_float3 vCameraLook = { matCamWorld._31, matCamWorld._32, matCamWorld._33 };

	//파티클 위치 설정 (카메라가 바라보는 화면의 오른쪽 아래에서 시작)
	attribute->_Position = vCameraPos;
	attribute->_Position += vCameraRight * 1.5f; // 오른쪽으로 이동
	attribute->_Position += vCameraUp * -1.0f; // 아래쪽으로 이동

	attribute->_Position += vCameraLook * 3.f;

	// 탄피의 초기 속도 방향 (카메라의 오른쪽 방향)
	attribute->_Velocity = vCameraRight * GetRandomFloat(10.0f, 30.0f);

	// 탄피가 위로 살짝 튀도록 Y축 속도 추가
	attribute->_Velocity.y += GetRandomFloat(1.0f, 30.0f);

	// 탄피가 날아가는 동안 중력 적용 (서서히 떨어지게)
	attribute->_Accelerator = { 0.0f, -98.0f, 0.0f }; // 중력 가속도 적용 (y 방향으로 하락)


	//각종 파라미터값
	attribute->_Age = 0.f;								// 나이
	attribute->_Color = WHITE;							// 색상
	attribute->_ColorFade = WHITE;						// 디졸브색상
	attribute->_LifeTime = 0.5f;						// 라이프타임


}

EVENT CBulletTracer::Update(_float timeDelta)
{
	list<Attribute>::iterator i;
	for (i = m_Particles.begin(); i != m_Particles.end(); i++)
	{
		//생존한 파티클만 갱신한다.
		if (i->_isAlive)
		{
			// 속도 업데이트 (중력 적용)
			i->_Velocity.y += i->_Accelerator.y * timeDelta;
			i->_Position += i->_Velocity * timeDelta;
			i->_Age += timeDelta;

			if (i->_Age > i->_LifeTime)
			{
				i->_isAlive = false;
				//resetParticle(&(*i));
			}
		}
	}


	//죽은 놈 삭제하기
	removeDeadParticle();


	FrameUpdate(timeDelta);
	Late_Update();

	return EVN_NONE;
}

HRESULT CBulletTracer::Render()
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

HRESULT CBulletTracer::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	int numparticles = 1;
	for (int i = 0; i < numparticles; i++)
	{
		addParticle();
	}

	return S_OK;
}

HRESULT CBulletTracer::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PC_BulletShell"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}


CBulletTracer* CBulletTracer::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CBulletTracer* pInstance = new CBulletTracer(pGraphicDev, _strObjName);

	//스노우 파티클 정보
	pInstance->m_vbSize = 2048;				//  GPU가 한번에 그릴 수 있는 파티클 개수, CPU가 GPU로 파티클 정점 버퍼에 담을 수 있는 개수
	pInstance->m_fSize = 0.1f;				//  파티클의 크기
	pInstance->m_vbOffset = 0;				//  세그먼트의 배치사이즈를 옮길때 쓰는 오프셋(0고정)
	pInstance->m_vbBatchSize = 1;			//  세그먼트 배치사이즈 크기(한번에 옮길 수 있는 정점들의 개수)
	pInstance->m_vMin = _float3{ 0.f,0.f,0.f };				//  바운딩박스의 최소크기
	pInstance->m_vMax = _float3{ 1.f,1.f,1.f };				//  바운딩박스의 최대크기

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

float CBulletTracer::GetRandomColor(float lowBound, float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

void CBulletTracer::FrameUpdate(float timeDelta)
{
	m_fFrame += 50.f * timeDelta;

	if (6.f < m_fFrame)
		m_fFrame = 0;
}

CGameObject* CBulletTracer::Clone(void* pArg)
{
	CBulletTracer* pInstance = new CBulletTracer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CBulletTracer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBulletTracer::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
