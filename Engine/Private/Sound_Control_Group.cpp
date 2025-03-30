#include "Sound_Control_Group.h"
#include "fmod.hpp"
#include "Sound_Core.h"

CSound_Control_Group::CSound_Control_Group(FMOD::ChannelGroup* pChannelGroup)
	:m_pChannelGroup(pChannelGroup)
{
}

void CSound_Control_Group::Add_Control_Group(CSound_Control_Group* pControl_Group)
{
}

void CSound_Control_Group::Add_Sound(CSound_Core* pSound_Core)
{
	pSound_Core->m_pChannelGroup = m_pChannelGroup;
}

CSound_Control_Group* CSound_Control_Group::Create(FMOD::ChannelGroup* pChannelGroup)
{
	return new CSound_Control_Group(pChannelGroup);
}

void CSound_Control_Group::Free()
{
	__super::Free();
	if (m_pChannelGroup)
	{
		m_pChannelGroup->release();
		m_pChannelGroup = nullptr;
	}
}
