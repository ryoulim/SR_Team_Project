#include "Tornado.h"
#include "GameInstance.h"


CTornado::CTornado(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CPSystem(pGraphicDev, _strObjName)
{
}

CTornado::CTornado(const CPSystem& Prototype)
	: CPSystem(Prototype)
{
}

HRESULT CTornado::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fFrame = GetRandomFloat(0.f, m_fAnimationMaxFrame);
	return S_OK;
}

HRESULT CTornado::Ready_Particle()
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

HRESULT CTornado::Ready_Components(void* pArg)
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

void CTornado::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;

	// 토네이도의 중심 위치 (토네이도는 m_vPosition을 중심으로 회전)
	attribute->_Position = m_vPosition;

	// 반지름 범위 설정 (토네이도 크기)
	float radius = GetRandomFloat(10.f, 20.f);  // 파티클이 퍼지는 반경

	// 초기 각도 설정 (0 ~ 360도 범위)
	float angle = GetRandomFloat(0.f, D3DX_PI * 2.f);  // 랜덤한 시작 각도 (라디안 값)

	// X, Z 위치를 원형으로 배치
	attribute->_Position.x += cosf(angle) * radius;
	attribute->_Position.z += sinf(angle) * radius;

	// 초기 속도 설정 (회전 + 상승)
	float angularSpeed = GetRandomFloat(30.f, 300.f);  // 회전 속도 (빠르게 회전할수록 숫자를 키움)
	float upwardSpeed = GetRandomFloat(50.f, 200.f); // 상승 속도

	// 속도 벡터 설정 (회전 + 상승 조합)
	attribute->_Velocity.x = -sinf(angle) * angularSpeed;
	attribute->_Velocity.z = cosf(angle) * angularSpeed;
	attribute->_Velocity.y = upwardSpeed;  // 위로 상승

	// 가속도 적용 (점점 위로 빨라지도록)
	attribute->_Accelerator = { 0.0f, GetRandomFloat(10.f, 30.f), 0.0f };

	// 파티클 수명 관련 설정
	attribute->_Age = 0.f;
	attribute->_Color = WHITE;
	attribute->_ColorFade = WHITE;
	attribute->_LifeTime = GetRandomFloat(0.5f, 2.f);  // 랜덤 수명
}

EVENT CTornado::Update(_float timeDelta)
{
	list<Attribute>::iterator i;
	for (i = m_Particles.begin(); i != m_Particles.end(); i++)
	{
		//생존한 파티클만 갱신한다.
		if (i->_isAlive)
		{
			// 속도 업데이트 (중력 적용 + 위로 상승)
			i->_Velocity += i->_Accelerator * timeDelta;
			i->_Position += i->_Velocity * timeDelta;

			// 회전 효과 강화 (속도가 클수록 원형 궤도 증가)
			float angle = atan2f(i->_Velocity.z, i->_Velocity.x); // 현재 방향의 각도
			float rotationSpeed = 5.0f * timeDelta;  // 회전 속도 조절

			angle += rotationSpeed; // 점진적인 회전
			float radius = sqrtf(i->_Velocity.x * i->_Velocity.x + i->_Velocity.z * i->_Velocity.z); // 현재 반경 유지

			// 새로운 회전 속도 적용
			i->_Velocity.x = cosf(angle) * radius;
			i->_Velocity.z = sinf(angle) * radius;

			// 랜덤한 흔들림 추가 (바람에 휘날리는 느낌)
			i->_Velocity.x += GetRandomFloat(-0.2f, 0.2f) * timeDelta;
			i->_Velocity.z += GetRandomFloat(-0.2f, 0.2f) * timeDelta;
			i->_Age += timeDelta;

			if (i->_Age > i->_LifeTime)
			{
				//i->_isAlive = false;
				resetParticle(&(*i));
			}
		}
	}


	//이거 고치긴해야함
	FrameUpdate(timeDelta);
	Late_Update();


	return EVN_NONE;
}


void CTornado::FrameUpdate(float timeDelta)
{
	m_fFrame += 15.f * timeDelta;

	if (m_fAnimationMaxFrame < m_fFrame)
		m_fFrame = 0;
}

HRESULT CTornado::Render()
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


CTornado* CTornado::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CTornado* pInstance = new CTornado(pGraphicDev, _strObjName);

	//파티클 정보
	pInstance->m_vbSize = 2048;				//  GPU가 한번에 그릴 수 있는 파티클 개수, CPU가 GPU로 파티클 정점 버퍼에 담을 수 있는 개수
	pInstance->m_fSize = 2.f;				//  파티클의 크기
	pInstance->m_vbOffset = 0;				//  세그먼트의 배치사이즈를 옮길때 쓰는 오프셋(0고정)
	pInstance->m_vbBatchSize = 64;			//  세그먼트 배치사이즈 크기(한번에 옮길 수 있는 정점들의 개수)
	pInstance->m_vMin = _float3{ 0.f,0.f,0.f };				//  바운딩박스의 최소크기
	pInstance->m_vMax = _float3{ 1.f,1.f,1.f };				//  바운딩박스의 최대크기

	return pInstance;
}

CGameObject* CTornado::Clone(void* pArg)
{
	CTornado* pInstance = new CTornado(*this);
	pInstance->m_isClone = true;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CTornado");
		Safe_Release(pInstance);
	}

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTornado::Free()
{
	__super::Free();
}
