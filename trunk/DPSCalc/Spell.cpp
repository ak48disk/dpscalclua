#include "stdafx.h"
#include "Global.h"
#include "Spell.h"

CSpellDB::CSpellDB()
{
	m_iSpellIDCounter = 0;
}

CSpellDB::~CSpellDB()
{
	for (SpellMap::const_iterator it = m_SpellMap.begin();
		it != m_SpellMap.end(); ++ it )
	{
		delete it->second;
	}
}

uint32 CSpellDB::AddSpell(CSpell *pSpell)
{
	++m_iSpellIDCounter;
	pSpell->m_iSpellID = m_iSpellIDCounter;
	m_SpellMap.insert(std::make_pair<uint32,CSpell*>(m_iSpellIDCounter,pSpell));
	return m_iSpellIDCounter;
}

const CSpell* CSpellDB::GetSpellByID(uint32 iSpellID) const
{
	SpellMap::const_iterator it = m_SpellMap.find(iSpellID);
	if (it != m_SpellMap.end())
		return it->second;
	else
		return NULL;
}

const CSpell* CSpellDB::GetSpellByName(const char *strSpellName) const
{
	for (SpellMap::const_iterator it = m_SpellMap.begin();
		it != m_SpellMap.end(); ++ it )
	{
		if ( strcmp(it->second->Get_SpellName(),strSpellName) == 0)
			return it->second;
	}
	return NULL;
}

CSpell::CSpell(const char *SpellName, const uint32 SpellType, const uint32 iDamageMin, 
			   const uint32 iDamageMax, const uint32 SpellCastTime, const uint32 SpellCooldown,
			   const uint32 SpellSchool, const CAura* pAuraToApply, const uint32 iAuraFadeTime, const uint32 ChannelTicks)
{
	strcpy(m_SpellName,SpellName);
	m_SpellType = SpellType; m_iDamageMin = iDamageMin;
	m_iDamageMax = iDamageMax; m_SpellCastTime = SpellCastTime;
	m_SpellCooldown = SpellCooldown; m_iSpellSchool = SpellSchool;
	m_pAuraToApply = pAuraToApply; m_iAuraFadeTime = iAuraFadeTime;
	m_ChannelTicks = ChannelTicks;
	m_SPPrecentage = 0;
}

CSpell::~CSpell()
{
}