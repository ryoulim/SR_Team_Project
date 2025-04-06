#include "CutSceneSmoke.h"
#include "GameInstance.h"

CCutSceneSmoke::CCutSceneSmoke(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CPSystem(pGraphicDev, _strObjName)
{
}

CCutSceneSmoke::CCutSceneSmoke(const CPSystem& Prototype)
	: CPSystem(Prototype)
{
}

HRESULT CCutSceneSmoke::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCutSceneSmoke::Ready_Particle()
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

HRESULT CCutSceneSmoke::Ready_Components(void* pArg)
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

void CCutSceneSmoke::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;

	//중심에서 퍼져나갈 각도 계산 (0 ~ 2PI)
	float angle = GetRandomFloat(0.0f, D3DX_PI * 2.0f);
	float speed = GetRandomFloat(100.0f, 300.0f); // 먼지 확산 속도

	//파티클 위치 = 중심
	attribute->_Position = m_vPosition;

	//수평 방향으로 퍼지는 속도
	attribute->_Velocity.x = cos(angle) * speed;
	attribute->_Velocity.y = GetRandomFloat(10.0f, 100.0f); // 살짝 위로 뜨게
	attribute->_Velocity.z = sin(angle) * speed;

	//중력 (약간 내리는 힘)
	attribute->_Accelerator = _float3(0.0f, -20.0f, 0.0f);

	//수명 및 색상 설정
	attribute->_Age = 0.f;
	attribute->_Color = WHITE;
	attribute->_ColorFade = WHITE;
	attribute->_LifeTime = GetRandomFloat(0.5f, 1.5f);

}

EVENT CCutSceneSmoke::Update(_float timeDelta)
{
	list<Attribute>::iterator i;
	for (i = m_Particles.begin(); i != m_Particles.end(); i++)
	{
		//생존한 파티클만 갱신한다.
		if (i->_isAlive)
		{
			/* [ 파티클 개인의 애니메이션 적용 ] */
			FrameUpdateAge(timeDelta, i->_Animation, i->_Age);

			i->_Velocity += i->_Accelerator * timeDelta;
			i->_Position += i->_Velocity * timeDelta;

			i->_Age += timeDelta;
			if (i->_Age > i->_LifeTime)
			{
				i->_isAlive = false;
				//resetParticle(&(*i));
			}
		}
	}

	removeDeadParticle();
	if (m_Particles.empty())
	{
		return EVN_DEAD;
	}

	return EVN_NONE;
}

HRESULT CCutSceneSmoke::Render()
{
	if (!m_Particles.empty())
	{
		//렌더 상태를 지정한다.
		SetUp_RenderState();

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
		m_pShaderCom->Begin(CShader::SMOKE);
		list<Attribute>::iterator i;
		for (i = m_Particles.begin(); i != m_Particles.end(); i++)
		{
			/* [ 파티클 개인의 애니메이션 적용 ] */
			m_pTextureCom->Bind_Resource((_uint)i->_Animation);

			D3DXVECTOR4 texelSize(1.0f / 64.f, 1.0f / 64.f, 0.0f, 0.0f);
			m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "g_Texture", (_uint)i->_Animation);
			m_pShaderCom->SetVector("g_vTexelSize", &texelSize);

			//블러 세기
			float fBlurAmount = 1.f;
			m_pShaderCom->SetFloat("g_fBlurAmount", fBlurAmount);

			//나이에 따른 투명도
			float age = i->_Age;
			float maxAge = i->_LifeTime;
			float fLifeRatio = 1.0f - (age / maxAge);
			m_pShaderCom->SetFloat("g_fLifeRatio", fLifeRatio);

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

	m_pShaderCom->End();
	return S_OK;

}


CCutSceneSmoke* CCutSceneSmoke::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CCutSceneSmoke* pInstance = new CCutSceneSmoke(pGraphicDev, _strObjName);

	//pInstance->m_vPosition = _Origin;
	pInstance->m_vbSize = 2048;				//  GPU가 한번에 그릴 수 있는 파티클 개수, CPU가 GPU로 파티클 정점 버퍼에 담을 수 있는 개수
	pInstance->m_fSize = 5.f;				//  파티클의 크기
	pInstance->m_vbOffset = 0;				//  세그먼트의 배치사이즈를 옮길때 쓰는 오프셋(0고정)
	pInstance->m_vbBatchSize = 1;			//  세그먼트 배치사이즈 크기(한번에 옮길 수 있는 정점들의 개수)
	pInstance->m_vMin = _float3{ 0.f,0.f,0.f };				//  바운딩박스의 최소크기
	pInstance->m_vMax = _float3{ 1.f,1.f,1.f };				//  바운딩박스의 최대크기

	return pInstance;
}

float CCutSceneSmoke::GetRandomColor(float lowBound, float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

void CCutSceneSmoke::SetPosition(_float3 Position)
{
	m_vPosition = Position;
}

CGameObject* CCutSceneSmoke::Clone(void* pArg)
{
	CCutSceneSmoke* pInstance = new CCutSceneSmoke(*this);
	pInstance->m_isClone = true;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCutSceneSmoke");
		Safe_Release(pInstance);
	}

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCutSceneSmoke::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
