#include "MonsterMissile.h"
#include "GameInstance.h"
#include <unordered_map>

CMonsterMissile::CMonsterMissile(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CPSystem(pGraphicDev, _strObjName)
{
}

CMonsterMissile::CMonsterMissile(const CPSystem& Prototype)
	: CPSystem(Prototype)
{
}

HRESULT CMonsterMissile::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CMonsterMissile::Ready_Particle()
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

HRESULT CMonsterMissile::Ready_Components(void* pArg)
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

void CMonsterMissile::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;

	// 4. 위치 및 속도 설정
	attribute->_Position = m_vPosition;

	// 5. 중력 (원하면 살짝 적용, 원치 않으면 0)
	attribute->_Accelerator = { 0.0f, 0.f, 0.0f };

	// 6. 파티클 기본 설정
	attribute->_Age = 0.f;
	attribute->_Color = WHITE;
	attribute->_ColorFade = WHITE;
	attribute->_LifeTime = GetRandomFloat(0.1f, 1.f);
	//attribute->_Animation = GetRandomFloat(0.f, 19.f);
}

EVENT CMonsterMissile::Update(_float timeDelta)
{
	list<Attribute>::iterator i;
	for (i = m_Particles.begin(); i != m_Particles.end(); i++)
	{
		//생존한 파티클만 갱신한다.
		if (i->_isAlive)
		{
			/* [ 파티클 개인의 애니메이션 적용 ] */
			//FrameUpdateAge(timeDelta, i->_Animation, i->_Age);
				
			i->_Velocity += i->_Accelerator * timeDelta;
			i->_Position += i->_Velocity * timeDelta;

			i->_Age += timeDelta;
			if (i->_Age > i->_LifeTime)
			{
				//i->_isAlive = false;
				resetParticle(&(*i));
			}
		}
	}

	if (m_bDead)
	{
		return EVN_DEAD;
	}

	return EVN_NONE;
}

HRESULT CMonsterMissile::Render()
{
	if (!m_Particles.empty())
	{
		//렌더 상태를 지정한다.
		SetUp_RenderState();
		m_pShaderCom->Begin(CShader::SMOKE);

		// [1] 파티클을 프레임 기준으로 그룹화
		unordered_map<_uint, vector<Attribute*>> groupedParticles;

		for (auto& particle : m_Particles)
		{
			if (!particle._isAlive)
				continue;

			_uint frame = static_cast<_uint>(particle._Animation);
			groupedParticles[frame].push_back(&particle);
		}

		// [2] 그룹 단위로 처리
		for (auto& group : groupedParticles)
		{
			_uint frame = group.first;
			auto& particles = group.second;

			// 텍스처 바인딩 (프레임별로 1번만)
			m_pTextureCom->Bind_Shader_To_Texture(m_pShaderCom, "g_Paricle", frame);

			// 셰이더 상수 설정 (뷰 행렬, 블러, etc)
			_float4x4 matView, matProj, matViewProj;
			m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
			m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
			matViewProj = matView * matProj;
			m_pShaderCom->Bind_Matrix("g_ViewMatrix", &matViewProj);

			D3DXVECTOR4 texelSize(1.0f / 64.f, 1.0f / 64.f, 0.0f, 0.0f);
			m_pShaderCom->SetVector("g_vTexelSize", &texelSize);
			m_pShaderCom->SetFloat("g_fBlurAmount", 2.0f);

			// 버텍스 버퍼 셋업
			m_pGraphic_Device->SetFVF(Particle::FVF);
			m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(Particle));

			if (m_vbOffset >= m_vbSize)
				m_vbOffset = 0;

			Particle* v = nullptr;
			m_pVB->Lock(
				m_vbOffset * sizeof(Particle),
				m_vbBatchSize * sizeof(Particle),
				(void**)&v,
				m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

			DWORD numParticlesInBatch = 0;

			for (auto& p : particles)
			{
				float age = p->_Age;
				float maxAge = p->_LifeTime;
				float fLifeRatio = 1.0f - (age / maxAge);
				//m_pShaderCom->SetFloat("g_fLifeRatio", fLifeRatio);

				v->_Position = p->_Position;
				v->_Color = (D3DCOLOR)p->_Color;
				v->_Size = p->_Size;
				v++;
				numParticlesInBatch++;

				if (numParticlesInBatch == m_vbBatchSize)
				{
					m_pVB->Unlock();

					m_pGraphic_Device->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, m_vbBatchSize);

					m_vbOffset += m_vbBatchSize;
					if (m_vbOffset >= m_vbSize)
						m_vbOffset = 0;

					m_pVB->Lock(
						m_vbOffset * sizeof(Particle),
						m_vbBatchSize * sizeof(Particle),
						(void**)&v,
						m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0;
				}
			}

			m_pVB->Unlock();

			if (numParticlesInBatch)
			{
				m_pGraphic_Device->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, numParticlesInBatch);
				m_vbOffset += m_vbBatchSize;
			}
		}

	}

	//렌더정리
	Release_RenderState();

	m_pShaderCom->End();
	return S_OK;

}


CMonsterMissile* CMonsterMissile::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CMonsterMissile* pInstance = new CMonsterMissile(pGraphicDev, _strObjName);

	//pInstance->m_vPosition = _Origin;
	pInstance->m_vbSize = 2048;				//  GPU가 한번에 그릴 수 있는 파티클 개수, CPU가 GPU로 파티클 정점 버퍼에 담을 수 있는 개수
	pInstance->m_fSize = 0.5f;				//  파티클의 크기
	pInstance->m_vbOffset = 0;				//  세그먼트의 배치사이즈를 옮길때 쓰는 오프셋(0고정)
	pInstance->m_vbBatchSize = 1;			//  세그먼트 배치사이즈 크기(한번에 옮길 수 있는 정점들의 개수)
	pInstance->m_vMin = _float3{ 0.f,0.f,0.f };				//  바운딩박스의 최소크기
	pInstance->m_vMax = _float3{ 1.f,1.f,1.f };				//  바운딩박스의 최대크기

	return pInstance;
}

float CMonsterMissile::GetRandomColor(float lowBound, float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

void CMonsterMissile::SetPosition(_float3 Position)
{
	m_vPosition = Position;
}

CGameObject* CMonsterMissile::Clone(void* pArg)
{
	CMonsterMissile* pInstance = new CMonsterMissile(*this);
	pInstance->m_isClone = true;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CMonsterMissile");
		Safe_Release(pInstance);
	}

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMonsterMissile::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
