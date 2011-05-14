#pragma once
#include "Global.h"



class CAura;

class CCreature 
{
	typedef std::map <uint32,CAura*> AuraMap;

public:
	CCreature(const char* strName);
	~CCreature();
	CLASS_ELEMENT_CONST(const char *,strName);

	bool ApplyAura(CAura* theAura);
	bool IsAuraAppliedByID (uint32 iAuraID) const;
	bool RemoveAura(CAura* theAura);
	bool RemoveAuraByID ( uint32 iAuraID);
	const CAura* GetAuraByID(uint32 iAuraID) const;
	CAura* GetAuraByIDForModification (uint32 iAuraID);

protected:
	CCreature();
	void Set_strName(const char* strName){ strcpy(m_strName,strName); }

private:
	char m_strName [ MAX_STRING_LENGTH ];
	AuraMap m_auraMap;
};

class CCreatureDB {
	typedef std::vector<CCreature*> CreatureVector;
public:
	CCreatureDB();
	~CCreatureDB();
	CCreature* GetCreatureByName(const char* strCreateureName);
	void AddCreature ( CCreature* pCreature);
	void RemoveCreatureByName( const char* strCreatureName);
private:
	CreatureVector	m_CreatureV;
};
