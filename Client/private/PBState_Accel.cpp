#include "PBState_Accel.h"
#include "PBState_Decel.h"

CPBState_Accel::CPBState_Accel()
{
}

void CPBState_Accel::Enter(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta)
{
	//초기 속도 설정
	m_fAccelSpeed = fTimeDelta * 5.f;
}

void CPBState_Accel::Execute(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta)
{
	m_fPosZ = pPlayerOnBoat->Compute_CurPosZ();

	m_fAccelSpeed *= 1.01f;

	pPlayerOnBoat->Go_Straight(m_fAccelSpeed);
	
	if (m_fPosZ > -5600.f && m_fPosZ < -5500.f)
		Exit(pPlayerOnBoat);
}

void CPBState_Accel::Exit(CPlayerOnBoat* pPlayerOnBoat)
{
	m_fPosZ = 0.f;
	
	//끝날 때 속도 저장
	pPlayerOnBoat->StoreSlidingSpeed(m_fAccelSpeed);

	//속도 초기화
	m_fAccelSpeed = 0.f;

	pPlayerOnBoat->Set_State(CPlayerOnBoat::DECEL);
}

void CPBState_Accel::Free()
{
	__super::Free();
}
