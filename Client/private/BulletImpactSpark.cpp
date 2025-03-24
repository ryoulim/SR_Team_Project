#include "BulletImpactSpark.h"
#include "GameInstance.h"


CBulletImpactSpark::CBulletImpactSpark(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
	:CPSystem(pGraphicDev, _strObjName)
{
}

CBulletImpactSpark::CBulletImpactSpark(const CPSystem& Prototype)
	: CPSystem(Prototype)
{
}

HRESULT CBulletImpactSpark::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fFrame = GetRandomFloat(0.f, m_fAnimationMaxFrame);


	return S_OK;
}

HRESULT CBulletImpactSpark::Reset(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);
	m_pTransForm->Set_State(CTransform::STATE_POSITION, pDesc->vPosition);
	for (int i = 0; i < pDesc->iParticleNums; i++)
		addParticle();

	return S_OK;
}

HRESULT CBulletImpactSpark::Ready_Particle()
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

HRESULT CBulletImpactSpark::Ready_Components(void* pArg)
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

void CBulletImpactSpark::resetParticle(Attribute* attribute)
{
	attribute->_isAlive = true;

	//�м��� ���� ����
	attribute->_Position = m_vPosition;
	_float3 min = { -10.0f,  0.0f, -10.0f };
	_float3 max = { 10.0f,  1.0f,  10.0f };
	GetRandomVector(&attribute->_Velocity, &min, &max);
	attribute->_Velocity.y = GetRandomFloat(5.0f, 15.0f);
	D3DXVec3Normalize(&attribute->_Velocity, &attribute->_Velocity);
	attribute->_Velocity *= 400.f; // ��ü �ӵ� ����
	attribute->_Accelerator = { 0.0f, -1550.f, 0.0f };

	//attribute->_Accelerator = _float3(0.f, 0.f, 0.f);	// ���ӵ�
	attribute->_Age = 0.f;								// ����
	attribute->_Color = WHITE;							// ����
	attribute->_ColorFade = WHITE;						// ���������
	attribute->_LifeTime = 0.2f;							// ������Ÿ��



}

EVENT CBulletImpactSpark::Update(_float timeDelta)
{
	if (m_Particles.empty())
	{
		m_pGameInstance->Deactive_Object(TEXT("ObjectPool_PC_BulletImpactSpark"), this);
		return EVN_OFF;
	}

	for (auto i = m_Particles.begin(); i != m_Particles.end();)
	{
		//������ ��ƼŬ�� �����Ѵ�.
		if (i->_isAlive)
		{
			// �ӵ� ������Ʈ (�߷� ����)
			i->_Velocity += i->_Accelerator * timeDelta;
			i->_Position += i->_Velocity * timeDelta;

			//i->_Position += i->_Velocity * timeDelta;
			i->_Age += timeDelta;

			if (i->_Age > i->_LifeTime)
			{
				i->_isAlive = false;
				i = m_Particles.erase(i);
				continue;
				//resetParticle(&(*i));
			}
		}
		i++;
	}

	FrameUpdate(timeDelta);

	return EVN_NONE;
}


void CBulletImpactSpark::FrameUpdate(float timeDelta)
{
	m_fFrame += 8.f * timeDelta;

	if (m_fAnimationMaxFrame < m_fFrame)
		m_fFrame = 0;
}

HRESULT CBulletImpactSpark::Render()
{
	if (!m_Particles.empty())
	{
		//���� ���¸� �����Ѵ�.
		SetUp_RenderState();

		//m_pTextureCom->Bind_Resource(0);
		m_pTextureCom->Bind_Resource((_uint)m_fFrame);

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


CBulletImpactSpark* CBulletImpactSpark::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring _strObjName)
{
	CBulletImpactSpark* pInstance = new CBulletImpactSpark(pGraphicDev, _strObjName);

	//����� ��ƼŬ ����
	//pInstance->m_vPosition = _Origin;
	pInstance->m_vbSize = 2048;				//  GPU�� �ѹ��� �׸� �� �ִ� ��ƼŬ ����, CPU�� GPU�� ��ƼŬ ���� ���ۿ� ���� �� �ִ� ����
	pInstance->m_fSize = 1.f;				//  ��ƼŬ�� ũ��
	pInstance->m_vbOffset = 0;				//  ���׸�Ʈ�� ��ġ����� �ű涧 ���� ������(0����)
	pInstance->m_vbBatchSize = 512;			//  ���׸�Ʈ ��ġ������ ũ��(�ѹ��� �ű� �� �ִ� �������� ����)
	pInstance->m_vMin = _float3{ 0.f,0.f,0.f };				//  �ٿ���ڽ��� �ּ�ũ��
	pInstance->m_vMax = _float3{ 1.f,1.f,1.f };				//  �ٿ���ڽ��� �ִ�ũ��

	return pInstance;
}

float CBulletImpactSpark::GetRandomColor(float lowBound, float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

CGameObject* CBulletImpactSpark::Clone(void* pArg)
{
	CBulletImpactSpark* pInstance = new CBulletImpactSpark(*this);
	pInstance->m_isClone = true;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CBulletImpactSpark");
		Safe_Release(pInstance);
	}

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBulletImpactSpark::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
