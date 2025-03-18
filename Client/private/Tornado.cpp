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

void CTornado::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;

	// ����̵��� �߽� ��ġ (����̵��� m_vPosition�� �߽����� ȸ��)
	attribute->_Position = m_vPosition;

	// ������ ���� ���� (����̵� ũ��)
	float radius = GetRandomFloat(10.f, 20.f);  // ��ƼŬ�� ������ �ݰ�

	// �ʱ� ���� ���� (0 ~ 360�� ����)
	float angle = GetRandomFloat(0.f, D3DX_PI * 2.f);  // ������ ���� ���� (���� ��)

	// X, Z ��ġ�� �������� ��ġ
	attribute->_Position.x += cosf(angle) * radius;
	attribute->_Position.z += sinf(angle) * radius;

	// �ʱ� �ӵ� ���� (ȸ�� + ���)
	float angularSpeed = GetRandomFloat(30.f, 300.f);  // ȸ�� �ӵ� (������ ȸ���Ҽ��� ���ڸ� Ű��)
	float upwardSpeed = GetRandomFloat(50.f, 200.f); // ��� �ӵ�

	// �ӵ� ���� ���� (ȸ�� + ��� ����)
	attribute->_Velocity.x = -sinf(angle) * angularSpeed;
	attribute->_Velocity.z = cosf(angle) * angularSpeed;
	attribute->_Velocity.y = upwardSpeed;  // ���� ���

	// ���ӵ� ���� (���� ���� ����������)
	attribute->_Accelerator = { 0.0f, GetRandomFloat(10.f, 30.f), 0.0f };

	// ��ƼŬ ���� ���� ����
	attribute->_Age = 0.f;
	attribute->_Color = WHITE;
	attribute->_ColorFade = WHITE;
	attribute->_LifeTime = GetRandomFloat(0.5f, 2.f);  // ���� ����
}

EVENT CTornado::Update(_float timeDelta)
{
	list<Attribute>::iterator i;
	for (i = m_Particles.begin(); i != m_Particles.end(); i++)
	{
		//������ ��ƼŬ�� �����Ѵ�.
		if (i->_isAlive)
		{
			// �ӵ� ������Ʈ (�߷� ���� + ���� ���)
			i->_Velocity += i->_Accelerator * timeDelta;
			i->_Position += i->_Velocity * timeDelta;

			// ȸ�� ȿ�� ��ȭ (�ӵ��� Ŭ���� ���� �˵� ����)
			float angle = atan2f(i->_Velocity.z, i->_Velocity.x); // ���� ������ ����
			float rotationSpeed = 5.0f * timeDelta;  // ȸ�� �ӵ� ����

			angle += rotationSpeed; // �������� ȸ��
			float radius = sqrtf(i->_Velocity.x * i->_Velocity.x + i->_Velocity.z * i->_Velocity.z); // ���� �ݰ� ����

			// ���ο� ȸ�� �ӵ� ����
			i->_Velocity.x = cosf(angle) * radius;
			i->_Velocity.z = sinf(angle) * radius;

			// ������ ��鸲 �߰� (�ٶ��� �ֳ����� ����)
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


	//�̰� ��ġ���ؾ���
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


CTornado* CTornado::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CTornado* pInstance = new CTornado(pGraphicDev, _strObjName);

	//��ƼŬ ����
	pInstance->m_vbSize = 2048;				//  GPU�� �ѹ��� �׸� �� �ִ� ��ƼŬ ����, CPU�� GPU�� ��ƼŬ ���� ���ۿ� ���� �� �ִ� ����
	pInstance->m_fSize = 2.f;				//  ��ƼŬ�� ũ��
	pInstance->m_vbOffset = 0;				//  ���׸�Ʈ�� ��ġ����� �ű涧 ���� ������(0����)
	pInstance->m_vbBatchSize = 64;			//  ���׸�Ʈ ��ġ������ ũ��(�ѹ��� �ű� �� �ִ� �������� ����)
	pInstance->m_vMin = _float3{ 0.f,0.f,0.f };				//  �ٿ���ڽ��� �ּ�ũ��
	pInstance->m_vMax = _float3{ 1.f,1.f,1.f };				//  �ٿ���ڽ��� �ִ�ũ��

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
