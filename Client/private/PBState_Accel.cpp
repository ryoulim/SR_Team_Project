#include "PBState_Accel.h"
#include "PBState_Decel.h"

CPBState_Accel::CPBState_Accel()
{
}

void CPBState_Accel::Enter(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta)
{
}

void CPBState_Accel::Execute(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta)
{
	m_fPosZ = pPlayerOnBoat->Compute_CurPosZ();

	pPlayerOnBoat->Go_Straight(fTimeDelta * 5.f);
	
	if (m_fPosZ < 100.f)
		Exit(pPlayerOnBoat);
}

void CPBState_Accel::Exit(CPlayerOnBoat* pPlayerOnBoat)
{
	m_fPosZ = 0.f;

	pPlayerOnBoat->Set_State(CPlayerOnBoat::DECEL);
}

void CPBState_Accel::Free()
{
	__super::Free();
}
