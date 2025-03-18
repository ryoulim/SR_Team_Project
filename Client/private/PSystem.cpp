#include "PSystem.h"
#include "Transform.h"
#include "GameInstance.h"

CPSystem::CPSystem(LPDIRECT3DDEVICE9 pGraphic_Device, wstring _strObjName)
	:CGameObject(pGraphic_Device)
	, m_pTextureCom(nullptr)
{
}

CPSystem::CPSystem(const CPSystem& Prototype)
	:CGameObject(Prototype)
	, m_dwFVF(Prototype.m_dwFVF)
	, m_dwVtxSize(Prototype.m_dwVtxSize)
	, m_fEmitRate(Prototype.m_fEmitRate)
	, m_fSize(Prototype.m_fSize)
	, m_iMaxParticle(Prototype.m_iMaxParticle)
	, m_vPosition(Prototype.m_vPosition)
	, m_Particles(Prototype.m_Particles)
	, m_pShaderCom(Prototype.m_pShaderCom)
	, m_pTextureCom(Prototype.m_pTextureCom)
	, m_pTransForm(Prototype.m_pTransForm)
	, m_pVB(Prototype.m_pVB)
	, m_vbBatchSize(Prototype.m_vbBatchSize)
	, m_vbOffset(Prototype.m_vbOffset)
	, m_vbSize(Prototype.m_vbSize)
	, m_vMax(Prototype.m_vMax)
	, m_vMin(Prototype.m_vMin)
{
}


HRESULT CPSystem::Ready_Particle()
{

	m_dwFVF = Particle::FVF;
	m_dwVtxSize = sizeof(Particle);

	//그래픽 디바이스의 레퍼런스 카운트가 + 1 이 된다.
	HRESULT hr;
	hr = m_pGraphic_Device->CreateVertexBuffer(
		m_vbSize * m_dwVtxSize,
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		Particle::FVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0
	);

	//그래픽 디바이스 릴리즈를 각 파티클마다 한번씩 걍 해주면 되지않을까? < 아이디어
	
	//프로토타입은 레디 파티클을 안부르지않나? -> 이 함수가 호출이 안된다 -> 프로토타입은 레퍼런스카운트를 올리지않는다.

	//그 말은 즉, 클론할때마다 레퍼런스 카운트가 1 증가한다.
	//근데 Free는 프로토타입이 삭제될때도 호출이 되기때문에 프리안에서 릴리즈 함수를 부르면 중복삭제가 되어버린다.

	//즉 , 클론이 삭제될때만 불리는 레퍼런스 감소 함수가 필요하다.
	
	// 클론이 생성될때 bool 변수를 트루로 만드는 로직을 만든다.

	return S_OK;
}


HRESULT CPSystem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPSystem::Initialize(void* pArg)
{
	if (FAILED(Ready_Particle()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		DESC* pDesc = static_cast<DESC*>(pArg);
		m_pTransForm->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
		m_fAnimationMaxFrame = pDesc->fMaxFrame;
	
		for (int i = 0; i < pDesc->iParticleNums; i++)
		{
			addParticle();
		}
	}

	return S_OK;
}


void CPSystem::reset()
{
	list<Attribute>::iterator i;
	for (i = m_Particles.begin(); i != m_Particles.end(); i++)
	{
		//순수가상함수
		resetParticle(&(*i));
	}
}

void CPSystem::addParticle()
{
	Attribute attribute;

	resetParticle(&attribute);

	m_Particles.push_back(attribute);
}


EVENT CPSystem::Update(_float timeDelta)
{
	return EVN_NONE;
}


void CPSystem::Late_Update()
{
	_float3	vTemp = *m_pTransForm->Get_State(CTransform::STATE_POSITION);
	CGameObject::Compute_ViewZ(&vTemp);

	//렌더그룹에 넣어준다.
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;
}

HRESULT CPSystem::SetUp_RenderState()
{

	//기본 셋팅
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE, FtoDW(m_fSize));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.1f));
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//거리에 따른 파티클 크기조절
	//m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(1.0f)); 
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.1f)); 
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(0.05f));



#pragma region 텍스처의 알파(온/오프)
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 알파 테스트를 끈다 (칼같이 자르는 걸 방지)
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
#pragma endregion


	//월드 변환
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());

	return S_OK;
}

HRESULT CPSystem::Render()
{
	if (!m_Particles.empty())
	{
		//렌더 상태를 지정한다.
		SetUp_RenderState();

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
			(void**) &v,
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

HRESULT CPSystem::Release_RenderState()
{
	
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// 알파 블렌딩을 끈다 (기본값: false) - > 이거 밑에줄 다 삭제? 일단 밑에 줄은 
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// 블렌드 모드를 기본값으로 변경 (SRCALPHA → 기본값은 SRC)
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	// 텍스처 스테이지의 알파 블렌딩 비활성화
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//이게 돌리는 함수들인데 우리 프로젝트에서 뭘 켜놔야 하는걸까 다 지워?


	return S_OK;
}


bool CPSystem::isEmpty()
{
	return m_Particles.empty();
}
bool CPSystem::isDead()
{
	for (const auto& particle : m_Particles)
	{
		if (particle._isAlive) 
		{
			return false;
		}
	}
	return true;
}
void CPSystem::removeDeadParticle()
{
	list<Attribute>::iterator i;
	i = m_Particles.begin();

	while (i != m_Particles.end())
	{
		if (i->_isAlive == false)
		{
			i = m_Particles.erase(i);
		}
		else
		{
			i++;
		}
	}
}



void CPSystem::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pTransForm);
	Safe_Release(m_pTextureCom);

	if (m_isClone)
		Safe_Release(m_pGraphic_Device);
}