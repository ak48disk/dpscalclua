#include "Global.h"
#include "Aura.h"

#pragma once

class Player;

class CSpell
{
public:
	friend class CSpellDB;
	~CSpell();
	CSpell (const char * SpellName,const uint32 SpellType,
		const uint32 iDamageMin,const uint32 iDamageMax,const uint32 SpellCastTime,
		const uint32 SpellCooldown,
		const uint32 SpellSchool,const CAura* pAuraToApply,const uint32 iAuraFadeTime,
		const uint32 ChannelTicks = 0);

	CLASS_ELEMENT_CONST(const char *,SpellName);
	CLASS_ELEMENT_CONST(uint32,SpellType);
	CLASS_ELEMENT_CONST(uint32,iDamageMin);
	CLASS_ELEMENT_CONST(uint32,iDamageMax);
	CLASS_ELEMENT_CONST(uint32,SpellCastTime);
	CLASS_ELEMENT_CONST(uint32,SpellCooldown);
	CLASS_ELEMENT_CONST(uint32,ChannelTicks);
	CLASS_ELEMENT_CONST(uint32,iSpellID);
	CLASS_ELEMENT_CONST(uint32,iSpellSchool);
	CLASS_ELEMENT_CONST(const CAura*,pAuraToApply);
	CLASS_ELEMENT_CONST(uint32,iAuraFadeTime);
	CLASS_ELEMENT(double,SPPrecentage);
	
private:
	char m_SpellName[MAX_STRING_LENGTH];
	uint32	m_SpellType;
	uint32	m_iDamageMin;
	uint32  m_iDamageMax;
	uint32	m_iSpellID;
	uint32  m_SpellCastTime;
	uint32  m_SpellCooldown;
	uint32  m_ChannelTicks;
	uint32	m_iSpellSchool;
	const CAura*	m_pAuraToApply;
	uint32	m_iAuraFadeTime;
	double	m_SPPrecentage;
};

class CSpellDB 
{
	typedef std::map<uint32,CSpell*> SpellMap;
public:
	CSpellDB();
	~CSpellDB();
	uint32 AddSpell(CSpell* pSpell); //Returns Spell ID
	const CSpell* GetSpellByName(const char* strSpellName) const;
	const CSpell* GetSpellByID (uint32 iSpellID) const;
private:
	uint32	 m_iSpellIDCounter;
	SpellMap m_SpellMap;
};


	