#include "PBState_Decel.h"
#include "PBState_Normal.h"

CPBState_Decel::CPBState_Decel()
{
}

void CPBState_Decel::Enter(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta)
{
	m_fVariableSpeed = fTimeDelta * 5.f;
}

void CPBState_Decel::Execute(CPlayerOnBoat* pPlayerOnBoat, _float fTimeDelta)
{
	m_fPosZ = pPlayerOnBoat->Compute_CurPosZ();

	m_fVariableSpeed *= 0.995f;

	pPlayerOnBoat->Go_Straight(m_fVariableSpeed);

	if(m_fPosZ > 700.f)
		Exit(pPlayerOnBoat);
}

void CPBState_Decel::Exit(CPlayerOnBoat* pPlayerOnBoat)
{
	m_fPosZ = 0.f;

	pPlayerOnBoat->Set_State(CPlayerOnBoat::NORMAL);
}

void CPBState_Decel::Free()
{
	__super::Free();
}
