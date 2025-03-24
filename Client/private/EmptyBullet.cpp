#include "EmptyBullet.h"
#include "GameInstance.h"
#include "Transform.h"

CEmptyBullet::CEmptyBullet(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CPSystem(pGraphicDev, _strObjName)
{
}

CEmptyBullet::CEmptyBullet(const CPSystem& Prototype)
	: CPSystem(Prototype)
{
}

void CEmptyBullet::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;
	
	_float4x4 matCamWorld;

	//ī�޶��� ��ġ����
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
	_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };
	

	//ī�޶� ����Ʈ,��,�� ���� ����
	_float3 vCameraRight = { matCamWorld._11, matCamWorld._12, matCamWorld._13 };
	_float3 vCameraUp = { matCamWorld._21, matCamWorld._22, matCamWorld._23 };
	_float3 vCameraLook = { matCamWorld._31, matCamWorld._32, matCamWorld._33 };

	//��ƼŬ ��ġ ���� (ī�޶� �ٶ󺸴� ȭ���� ������ �Ʒ����� ����)
	attribute->_Position = vCameraPos;
	attribute->_Position += vCameraRight * 1.5f; // ���������� �̵�
	attribute->_Position += vCameraUp * -1.0f; // �Ʒ������� �̵�
	
	attribute->_Position += vCameraLook * 3.f;

	// ź���� �ʱ� �ӵ� ���� (ī�޶��� ������ ����)
	attribute->_Velocity = vCameraRight * GetRandomFloat(10.0f, 30.0f);

	// ź�ǰ� ���� ��¦ Ƣ���� Y�� �ӵ� �߰�
	attribute->_Velocity.y += GetRandomFloat(1.0f, 30.0f);

	// ź�ǰ� ���ư��� ���� �߷� ���� (������ ��������)
	attribute->_Accelerator = { 0.0f, -98.0f, 0.0f }; // �߷� ���ӵ� ���� (y �������� �϶�)


	//���� �Ķ���Ͱ�
	attribute->_Age = 0.f;								// ����
	attribute->_Color = WHITE;							// ����
	attribute->_ColorFade = WHITE;						// ���������
	attribute->_LifeTime = 5.f;						// ������Ÿ��


}

EVENT CEmptyBullet::Update(_float timeDelta)
{
	list<Attribute>::iterator i;
	for (i = m_Particles.begin(); i != m_Particles.end(); i++)
	{
		//������ ��ƼŬ�� �����Ѵ�.
		if (i->_isAlive)
		{
			// �ӵ� ������Ʈ (�߷� ����)
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


	//���� �� �����ϱ�
	removeDeadParticle();


	FrameUpdate(timeDelta);
	Late_Update();

	return EVN_NONE;
}

void CEmptyBullet::Late_Update()
{
	//�����׷쿡 �־��ش�.
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;
}

HRESULT CEmptyBullet::Render()
{
	if (!m_Particles.empty())
	{
		//���� ���¸� �����Ѵ�.
		SetUp_RenderState();
		
		//m_pTextureCom->Bind_Resource(0);
		m_pTextureCom->Bind_Resource((_uint)m_fFrame);
		//m_pGraphic_Device->SetTexture(0, nullptr);
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

HRESULT CEmptyBullet::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fFrame = GetRandomFloat(0.f, m_fAnimationMaxFrame);
	m_fViewZ = 0.f;
	m_fDepth = 0.f;

	return S_OK;
}

HRESULT CEmptyBullet::Ready_Components(void* pArg)
{
	CPSystem::DESC* pDesc = static_cast<CPSystem::DESC*>(pArg);

	//Ʈ������ ������Ʈ ����
	CTransform::DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransForm), &TransformDesc)))
		return E_FAIL;
	

	//�ؽ�ó ������Ʈ ����
	if (FAILED(__super::Add_Component(LEVEL_STATIC, _wstring(TEXT("Prototype_Component_Texture_")) + pDesc->szTextureTag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	return S_OK;
}


CEmptyBullet* CEmptyBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CEmptyBullet* pInstance = new CEmptyBullet(pGraphicDev, _strObjName);

	//����� ��ƼŬ ����
	pInstance->m_vbSize = 2048;				//  GPU�� �ѹ��� �׸� �� �ִ� ��ƼŬ ����, CPU�� GPU�� ��ƼŬ ���� ���ۿ� ���� �� �ִ� ����
	pInstance->m_fSize = 0.15f;				//  ��ƼŬ�� ũ��
	pInstance->m_vbOffset = 0;				//  ���׸�Ʈ�� ��ġ����� �ű涧 ���� ������(0����)
	pInstance->m_vbBatchSize = 1;			//  ���׸�Ʈ ��ġ������ ũ��(�ѹ��� �ű� �� �ִ� �������� ����)
	pInstance->m_vMin = _float3{ 0.f,0.f,0.f };				//  �ٿ���ڽ��� �ּ�ũ��
	pInstance->m_vMax = _float3{ 1.f,1.f,1.f };				//  �ٿ���ڽ��� �ִ�ũ��

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

float CEmptyBullet::GetRandomColor(float lowBound, float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

void CEmptyBullet::FrameUpdate(float timeDelta)
{
	m_fFrame += 50.f * timeDelta;

	if (m_fAnimationMaxFrame < m_fFrame)
		m_fFrame = 0;
}

CGameObject* CEmptyBullet::Clone(void* pArg)
{
	CEmptyBullet* pInstance = new CEmptyBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CEmptyBullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEmptyBullet::Free()
{
	__super::Free();
}
