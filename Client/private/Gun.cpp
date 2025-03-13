#include "Gun.h"
#include "GameInstance.h"
#include "Transform.h"

CGun::CGun(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CPSystem(pGraphicDev, _strObjName)
{
}

CGun::CGun(const CPSystem& Prototype)
	: CPSystem(Prototype)
{
}

void CGun::resetParticle(Attribute* attribute)
{
	_float4x4 matCamWorld;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);

	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	attribute->_isAlive = true;

	//ī�޶��� ��ġ����
	_float3 vCameraPos;
	vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };
	
	//ī�޶��� �躤�� ����
	_float3 vCameraLook;
	vCameraLook = { matCamWorld._31, matCamWorld._32, matCamWorld._33 };
	
	attribute->_Position = vCameraPos;
	attribute->_Position.y -= 0.5f;
	attribute->_Position.x += 1.f;

	attribute->_Velocity = vCameraLook * 200.f;


	//���� �Ķ���Ͱ�
	D3DXCOLOR Color = { 1.f , 1.f, 1.f, 1.f };

	attribute->_Accelerator = _float3(0.f, 0.f, 0.f);	// ���ӵ�
	attribute->_Age = 0.f;								// ����
	attribute->_Color = Color;							// ����
	attribute->_ColorFade = WHITE;						// ���������
	attribute->_LifeTime = 0.1f;						// ������Ÿ��

	//��ƼŬ ���� ������ : �׷��� ī�忡�� �������� gg
	//attribute->_Size = GetRandomFloat(m_fSize - 3.f, m_fSize + 3.f);

}

EVENT CGun::Update(_float timeDelta)
{
	list<Attribute>::iterator i;
	for (i = m_Particles.begin(); i != m_Particles.end(); i++)
	{		
		i->_Position += i->_Velocity * timeDelta;

		i->_Age += timeDelta;

		if (i->_Age > i->_LifeTime)
			i->_isAlive = false;
	}

	//���� �� �����ϱ�
	removeDeadParticle();
	FrameUpdate(timeDelta);

	//�̰� ��ġ���ؾ���
	Late_Update();


	return EVN_NONE;
}

HRESULT CGun::SetUp_RenderState()
{
	CPSystem::SetUp_RenderState();

	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, false);

	return S_OK;
}

HRESULT CGun::Render()
{
	if (!m_Particles.empty())
	{
		//���� ���¸� �����Ѵ�.
		SetUp_RenderState();
		
		m_pTextureCom->Bind_Resource(0);
		//m_pTextureCom->Bind_Resource((_uint)m_fFrame);

		m_pGraphic_Device->SetFVF(Particle::FVF);
		m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(Particle));

		//���ؽ� ���۸� ��� ��� ó������ �����Ѵ�.
		if (m_vbOffset >= m_vbSize)
			m_vbOffset = 0;

		Particle* v = 0;

		m_pVB->Lock(
			m_vbOffset * sizeof(Particle),
			m_vbBatchSize * sizeof(Particle),
			(void**)&v,
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

HRESULT CGun::Release_RenderState()
{
	CPSystem::Release_RenderState();
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	return S_OK;
}

HRESULT CGun::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	int numparticles = 10;
	for (int i = 0; i < numparticles; i++)
	{
		addParticle();
	}

	return S_OK;
}

HRESULT CGun::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Bullet"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}


CGun* CGun::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CGun* pInstance = new CGun(pGraphicDev, _strObjName);

	//����� ��ƼŬ ����
	pInstance->m_vbSize = 2048;				//  GPU�� �ѹ��� �׸� �� �ִ� ��ƼŬ ����, CPU�� GPU�� ��ƼŬ ���� ���ۿ� ���� �� �ִ� ����
	pInstance->m_fSize = 0.05f;				//  ��ƼŬ�� ũ��
	pInstance->m_vbOffset = 0;				//  ���׸�Ʈ�� ��ġ����� �ű涧 ���� ������(0����)
	pInstance->m_vbBatchSize = 10;			//  ���׸�Ʈ ��ġ������ ũ��(�ѹ��� �ű� �� �ִ� �������� ����)
	pInstance->m_vMin = _float3{ 0.f,0.f,0.f };				//  �ٿ���ڽ��� �ּ�ũ��
	pInstance->m_vMax = _float3{ 1.f,1.f,1.f };				//  �ٿ���ڽ��� �ִ�ũ��

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

float CGun::GetRandomColor(float lowBound, float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

void CGun::FrameUpdate(float timeDelta)
{
	m_fFrame += 32.f * timeDelta;

	if (4.f < m_fFrame)
		m_fFrame = 0;
}

CGameObject* CGun::Clone(void* pArg)
{
	CGun* pInstance = new CGun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGun::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
