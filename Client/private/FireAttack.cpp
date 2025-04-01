#include "FireAttack.h"
#include "GameInstance.h"
#include <iostream>

CFireAttack::CFireAttack(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CPSystem(pGraphicDev, _strObjName)
{
}

CFireAttack::CFireAttack(const CPSystem& Prototype)
	: CPSystem(Prototype)
{
}

HRESULT CFireAttack::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//만약 보스가 1마리만 남으면 구분이 사라지게
	if (m_pGameInstance->Find_Objects(LEVEL_GAMEPLAY, L"Layer_Boss")->size() < 2)
		m_iNum = 0;

	return S_OK;
}

HRESULT CFireAttack::Ready_Particle()
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

HRESULT CFireAttack::Ready_Components(void* pArg)
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

void CFireAttack::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;

	// 1. 보스의 현재 위치
	auto pBossTransform = static_cast<CTransform*>(
		m_pGameInstance->Find_Object(LEVEL_GAMEPLAY, L"Layer_Boss", m_iNum)->Find_Component(L"Com_Transform")
		);
	_float3 BossPos = *pBossTransform->Get_State(CTransform::STATE_POSITION);

	// 2. 플레이어 위치
	_float3 vPlayerPos = *static_cast<CTransform*>(GET_PLAYER->Find_Component(L"Com_Transform"))->Get_State(CTransform::STATE_POSITION);
	vPlayerPos.y += -15;

	// 3. 보스 → 플레이어 방향 벡터
	_float3 vDir = vPlayerPos - BossPos;
	D3DXVec3Normalize(&vDir, &vDir); // 방향 정규화

	// 3.5. 흔들림용 Right 벡터 얻기
	_float3 vRight = *pBossTransform->Get_State(CTransform::STATE_RIGHT);
	D3DXVec3Normalize(&vRight, &vRight);

	// 흔들림 적용 (좌우 무작위)
	float fShakeAmount = GetRandomFloat(m_fMin, m_fMax); // ±0.1 정도 흔들림
	_float3 vShake = vRight * fShakeAmount;

	// 최종 방향 = 원래 방향 + 흔들림
	_float3 finalDir = vDir + vShake;
	D3DXVec3Normalize(&finalDir, &finalDir);

	// 4. 위치 및 속도 설정
	attribute->_Position = m_vPosition;
	attribute->_Position.y += 35;
	attribute->_Velocity = finalDir * 500.f; // 직선 방향 속도

	// 5. 중력 (원하면 살짝 적용, 원치 않으면 0)
	attribute->_Accelerator = { 0.0f, -300.f, 0.0f };

	// 6. 파티클 기본 설정
	attribute->_Age = 0.f;
	attribute->_Color = WHITE;
	attribute->_ColorFade = WHITE;
	attribute->_LifeTime = 2.f;
	attribute->_Size = GetRandomFloat(m_fSize - 3.f, m_fSize + 3.f);
}

EVENT CFireAttack::Update(_float timeDelta)
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
				i->_isAlive = false;
				//resetParticle(&(*i));
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


void CFireAttack::FrameUpdate(float timeDelta)
{
	m_fFrame += 30.f * timeDelta;

	if (m_fAnimationMaxFrame < m_fFrame)
		m_fFrame = 0;
}

HRESULT CFireAttack::Render()
{
	if (!m_Particles.empty())
	{
		//렌더 상태를 지정한다.
		SetUp_RenderState();
		
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

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
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	Release_RenderState();

	return S_OK;

}


CFireAttack* CFireAttack::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CFireAttack* pInstance = new CFireAttack(pGraphicDev, _strObjName);

	//스노우 파티클 정보
	//pInstance->m_vPosition = _Origin;
	pInstance->m_vbSize = 2048;				//  GPU가 한번에 그릴 수 있는 파티클 개수, CPU가 GPU로 파티클 정점 버퍼에 담을 수 있는 개수
	pInstance->m_fSize = 1.f;				//  파티클의 크기
	pInstance->m_vbOffset = 0;				//  세그먼트의 배치사이즈를 옮길때 쓰는 오프셋(0고정)
	pInstance->m_vbBatchSize = 512;			//  세그먼트 배치사이즈 크기(한번에 옮길 수 있는 정점들의 개수)
	pInstance->m_vMin = _float3{ 0.f,0.f,0.f };				//  바운딩박스의 최소크기
	pInstance->m_vMax = _float3{ 1.f,1.f,1.f };				//  바운딩박스의 최대크기

	return pInstance;
}

float CFireAttack::GetRandomColor(float lowBound, float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

CGameObject* CFireAttack::Clone(void* pArg)
{
	CFireAttack* pInstance = new CFireAttack(*this);
	pInstance->m_isClone = true;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CFireAttack");
		Safe_Release(pInstance);
	}

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFireAttack::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
