#include "stdafx.h"

#include "Creature.h"
#include "Aura.h"

CCreatureDB::CCreatureDB()
{
}
void CCreatureDB::AddCreature(CCreature *pCreature)
{
	m_CreatureV.push_back(pCreature);
}
void CCreatureDB::RemoveCreatureByName(const char *strCreatureName)
{
	CreatureVector::iterator it;
	for(it = m_CreatureV.begin() ; it != m_CreatureV.end() ; ++it)
	{
		if (strcmp( (*it)->Get_strName() , strCreatureName)  == 0)
		{
			delete (*it);
			m_CreatureV.erase(it);
			break;
		}
	}
}

CCreature* CCreatureDB::GetCreatureByName(const char *strCreatureName)
{
	CreatureVector::iterator it;
	for(it = m_CreatureV.begin() ; it != m_CreatureV.end() ; ++it)
	{
		if (strcmp( (*it)->Get_strName() , strCreatureName)  == 0)
		{
			return *it;
		}
	}
	return NULL;
}

CCreatureDB::~CCreatureDB()
{
	CreatureVector::iterator it;
	for(it = m_CreatureV.begin() ; it != m_CreatureV.end() ; ++it)
		delete (*it);
	m_CreatureV.clear();
}

CCreature::CCreature()
{

}

CCreature::CCreature(const char *strName)
{
	strcpy(m_strName,strName);
}

CCreature::~CCreature()
{
}

const CAura* CCreature::GetAuraByID(uint32 iAuraID) const {
	AuraMap::const_iterator it = m_auraMap.find(iAuraID);
	if (it!= m_auraMap.end())
		return it->second;
	return NULL;
}

CAura* CCreature::GetAuraByIDForModification(uint32 iAuraID) {
	return const_cast<CAura*> ( GetAuraByID(iAuraID) );
}

bool CCreature::ApplyAura(CAura *theAura)
{
	if (m_auraMap.find(theAura->Get_iAuraID()) == m_auraMap.end() )  {
		m_auraMap.insert(std::make_pair<uint32,CAura*>(theAura->Get_iAuraID(),theAura));
		return true;
	}
	return false;
}

bool CCreature::IsAuraAppliedByID(uint32 iAuraID) const  {
	if (m_auraMap.find(iAuraID) == m_auraMap.end()  )
		return false;
	else
		return true;
}

bool CCreature::RemoveAura(CAura *theAura)  {
	AuraMap::iterator it;
	for(it = m_auraMap.begin() ; it!= m_auraMap.end() ;++it)  {
		if (theAura == it->second)  {
			m_auraMap.erase(it);
			return true;
			//break;
		}
	}
	return false;
}

bool CCreature::RemoveAuraByID(uint32 iAuraID)  {
	AuraMap::iterator it = m_auraMap.find(iAuraID);
	if (it != m_auraMap.end() )
		m_auraMap.erase(it);
	return (it!= m_auraMap.end());
}
