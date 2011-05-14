#include "stdafx.h"
#include "global.h"
#include "Aura.h"

uint32  CAura::m_iGUIDCounter = 0;

CAuraDB::CAuraDB()
{
	m_iIDCounter = 0;
}

CAuraDB::~CAuraDB()
{
	AuraMap::iterator it;
	for (it = m_AuraMap.begin() ; it != m_AuraMap.end() ;++ it)
	{
		delete it->second;
	}
	m_AuraMap.clear();
}

uint32 CAuraDB::AddAura(CAura *pAura)
{
	m_iIDCounter ++;
	pAura->m_iAuraID = m_iIDCounter;
	m_AuraMap.insert(std::make_pair<uint32,CAura*>(m_iIDCounter,pAura));
	return m_iIDCounter;
}

const CAura* CAuraDB::GetAuraByID(uint32 iAuraID) const
{
	AuraMap::const_iterator it = m_AuraMap.find(iAuraID);
	if (it != m_AuraMap.end())
		return it->second;
	else
		return NULL;
}

const CAura* CAuraDB::GetAuraByName(const char *strAuraName) const
{
	AuraMap::const_iterator it;
	for (it = m_AuraMap.begin() ; it != m_AuraMap.end() ;++ it)
	{
		if(strcmp(it->second->Get_strAuraName(),strAuraName) == 0)
			return it->second;
	}
	return NULL;
}
