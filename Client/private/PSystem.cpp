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

	//�׷��� ����̽��� ���۷��� ī��Ʈ�� + 1 �� �ȴ�.
	HRESULT hr;
	hr = m_pGraphic_Device->CreateVertexBuffer(
		m_vbSize * m_dwVtxSize,
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		Particle::FVF,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0
	);

	//�׷��� ����̽� ����� �� ��ƼŬ���� �ѹ��� �� ���ָ� ����������? < ���̵��
	
	//������Ÿ���� ���� ��ƼŬ�� �Ⱥθ����ʳ�? -> �� �Լ��� ȣ���� �ȵȴ� -> ������Ÿ���� ���۷���ī��Ʈ�� �ø����ʴ´�.

	//�� ���� ��, Ŭ���Ҷ����� ���۷��� ī��Ʈ�� 1 �����Ѵ�.
	//�ٵ� Free�� ������Ÿ���� �����ɶ��� ȣ���� �Ǳ⶧���� �����ȿ��� ������ �Լ��� �θ��� �ߺ������� �Ǿ������.

	//�� , Ŭ���� �����ɶ��� �Ҹ��� ���۷��� ���� �Լ��� �ʿ��ϴ�.
	
	// Ŭ���� �����ɶ� bool ������ Ʈ��� ����� ������ �����.

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
		//���������Լ�
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

	//�����׷쿡 �־��ش�.
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;
}

HRESULT CPSystem::SetUp_RenderState()
{

	//�⺻ ����
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE, FtoDW(m_fSize));
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.1f));
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//�Ÿ��� ���� ��ƼŬ ũ������
	//m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(1.0f)); 
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.1f)); 
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(0.05f));



#pragma region �ؽ�ó�� ����(��/����)
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���� �׽�Ʈ�� ���� (Į���� �ڸ��� �� ����)
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
#pragma endregion


	//���� ��ȯ
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, m_pTransForm->Get_WorldMatrix());

	return S_OK;
}

HRESULT CPSystem::Render()
{
	if (!m_Particles.empty())
	{
		//���� ���¸� �����Ѵ�.
		SetUp_RenderState();

		m_pGraphic_Device->SetTexture(0, nullptr);
		m_pGraphic_Device->SetFVF(Particle::FVF);
		m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(Particle));

		//���ؽ� ���۸� ��� ��� ó������ �����Ѵ�.
		if (m_vbOffset >= m_vbSize)
			m_vbOffset = 0;

		Particle* v = 0;

		m_pVB->Lock(
			m_vbOffset * sizeof(Particle),
			m_vbBatchSize * sizeof(Particle),
			(void**) &v,
			m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);


		DWORD numParticlesInBatch = 0;

		//��� ��ƼŬ�� ������ �� ����
		list<Attribute>::iterator i;
		for (i = m_Particles.begin(); i != m_Particles.end(); i++)
		{
			if (i->_isAlive)
			{
				//�� �ܰ��� ���׸�Ʈ���� ������ ��ƼŬ�� ���� ���ؽ� ���� ���׸�Ʈ�� �����Ѵ�.
				v->_Position = i->_Position;
				v->_Color = (D3DCOLOR)i->_Color;
				v->_Size = i->_Size;
				v++;
				numParticlesInBatch++;

				//���� �ܰ��� ���ؽ����۰� ��� ä�����ִ°�?
				if (numParticlesInBatch == m_vbBatchSize)
				{
					//���ؽ� ���۷� ����� ������ �ܰ��� ��ƼŬ�� �׸���.
					m_pVB->Unlock();

					m_pGraphic_Device->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, m_vbBatchSize);

					//�ܰ谡 �׷����� ���� ���� �ܰ踦 ��ƼŬ�� ä���.
					m_vbOffset += m_vbBatchSize;

					//���ؽ� ������ ��踦 �Ѵ� �޸𸮷� ������ ����X , ���� ��� ó������
					if (m_vbOffset >= m_vbSize)
						m_vbOffset = 0;

					m_pVB->Lock(
						m_vbOffset * sizeof(Particle),
						m_vbBatchSize * sizeof(Particle),
						(void**)&v,
						m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
					
					numParticlesInBatch = 0; //���� �ܰ踦 ���� ����
				}
			}
		}

		m_pVB->Unlock();

		//������ �ܰ谡 �����������ʴ� ����� ����ó��

		if (numParticlesInBatch)
		{
			m_pGraphic_Device->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, numParticlesInBatch);
		}

		//�������
		m_vbOffset += m_vbBatchSize;

	}

	//��������
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

	// ���� ������ ���� (�⺻��: false) - > �̰� �ؿ��� �� ����? �ϴ� �ؿ� ���� 
	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// ���� ��带 �⺻������ ���� (SRCALPHA �� �⺻���� SRC)
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	// �ؽ�ó ���������� ���� ���� ��Ȱ��ȭ
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	//m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//�̰� ������ �Լ����ε� �츮 ������Ʈ���� �� �ѳ��� �ϴ°ɱ� �� ����?


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