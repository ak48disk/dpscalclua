#include "Global.h"
#pragma once

class CAura 
{
	friend class CAuraDB;
public:
	CAura(const char* strAuraName , uint32 iTicks = 1,
		bool isPeriodic = false , uint32 iPeriodicDamageMin = 0 ,
		uint32 iPeriodicDamageMax = 0,uint32 iAuraSchool = 0,
		bool bCritEnabled = false,bool bHasteEnabled = false,uint32 PeriodicEventTime = 0){ 
			strcpy(m_strAuraName,strAuraName); 
			m_iTicks = iTicks;
			m_bIsPeriodic = isPeriodic; m_iPeriodicDamageMin = iPeriodicDamageMin;
			m_iPeriodicDamageMax = iPeriodicDamageMax;
			m_iAuraSchool = iAuraSchool;
			m_bCritEnabled = bCritEnabled;
			m_bHasteEnabled = bHasteEnabled;
			m_iPeriodicEventTime = PeriodicEventTime;
			m_dSpellPowerFactorPreTick = 0;
			m_iSpellPower = 0;
			//WHEN IT IS COPIED IT GENERATED A BNRAND NEW GUID
			//THIS PREVENTS EVENTS PERFORM UNNECESSARY ACTIONS FOR REMOVED BUT RE-APPLIED AURAS THAT HAS A DIFFERENT GUID
			m_iGUID = ++m_iGUIDCounter;
	}
	CAura ( const CAura* pAura)
	{
		strcpy(m_strAuraName,pAura->Get_strAuraName());
		m_iTicks = pAura->Get_iTicks();
		if ( m_bIsPeriodic = pAura->Get_bIsPeriodic() )
		{
			m_iPeriodicDamageMin = pAura->Get_iPeriodicDamageMin();
			m_iPeriodicDamageMax = pAura->Get_iPeriodicDamageMax();
			m_iAuraSchool		 = pAura->Get_iAuraSchool();
			m_iPeriodicEventTime = pAura->Get_iPeriodicEventTime();
			m_bCritEnabled		 = pAura->Get_bCritEnabled();
			m_bHasteEnabled		 = pAura->Get_bHasteEnabled();
			m_dSpellPowerFactorPreTick = pAura->Get_dSpellPowerFactorPreTick();
			m_iSpellPower = 0;

		}
		m_iGUID = ++m_iGUIDCounter;
		m_iAuraID = pAura->Get_iAuraID();
	}
	~CAura(){};

	CLASS_ELEMENT_CONST(uint32,iAuraID);
	CLASS_ELEMENT_CONST(const char*,strAuraName);
	CLASS_ELEMENT_CONST(bool,bIsPeriodic);
	CLASS_ELEMENT_CONST(uint32,iPeriodicDamageMin);
	CLASS_ELEMENT_CONST(uint32,iPeriodicDamageMax);
	CLASS_ELEMENT_CONST(uint32,iAuraSchool);
	CLASS_ELEMENT_CONST(uint32,iPeriodicEventTime);
	CLASS_ELEMENT_CONST(uint32,iGUID);
	CLASS_ELEMENT(bool,bCritEnabled);
	CLASS_ELEMENT(bool,bHasteEnabled);
	CLASS_ELEMENT(uint32,iTicks);
	CLASS_ELEMENT(uint32,iFadeTime);
	CLASS_ELEMENT(double,dSpellPowerFactorPreTick);
	CLASS_ELEMENT(uint32,iSpellPower);

private:
	uint32	m_iAuraID;
	uint32	m_iTicks;
	uint32	m_iPeriodicEventTime;
	char	m_strAuraName [ MAX_STRING_LENGTH ];
	bool	m_bIsPeriodic;
	bool	m_bCritEnabled;
	bool	m_bHasteEnabled;
	uint32	m_iPeriodicDamageMin;
	uint32	m_iPeriodicDamageMax;
	uint32	m_iFadeTime;
	uint32	m_iAuraSchool;
	uint32  m_iGUID;
	double	m_dSpellPowerFactorPreTick;
	uint32  m_iSpellPower;
	static uint32 m_iGUIDCounter;
};

class CAuraDB 
{
	typedef std::map<uint32,CAura*> AuraMap;
public:
	CAuraDB();
	~CAuraDB();
	uint32 AddAura(CAura* pAura);
	const CAura* GetAuraByID(uint32 iAuraID) const;
	const CAura* GetAuraByName ( const char* strAuraName ) const;

private:
	AuraMap m_AuraMap;
	uint32	m_iIDCounter;
};
