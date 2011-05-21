#include "Global.h"

class CAura;
class CPlayer;
class CSpell;
class CCreature;
class CEvent;

class CEventSystem {
	typedef std::list<CEvent*> EventList;
	typedef std::map<uint32,EventList> EventMap;
public:
	CEventSystem();
	~CEventSystem();

	CLASS_ELEMENT_CONST(uint32,iGameTime);

	void AddEvent(CEvent* pEvent,uint32 DelayTime);
	bool ProcessEvent(uint32 NextPlayerAvailableTime);
	bool IsExitable();
	void ReadyExit() { m_bExitable = true; }
	
private:
	uint32 m_iGameTime;
	EventMap m_eventMap;
	bool m_bExitable;
};

class CEvent
{
	friend class CEventSystem;
public:
	~CEvent();
	virtual int operator () (int param)=0;
	virtual e_EventType Get_eType(){ return m_EventType;}
	virtual double Get_CritChance();
	virtual uint32 Get_Damage();
	virtual double Get_HitChance();
	virtual double GetCastTime();
	void ResetAdditionals();


	EVENT_ADDITIONAL_ELEMENT(int32,Damage);

	double Get_AdditionalDamagePrecentage() const
	{
		double returnVal = 1.0;
		for (int i=0;i<15;++i)//Modified 2011-4-25 Index 15is applied after Additional Damage has been calculated

			returnVal *= (1.0 +  m_iAdditionalDamagePrecentage[i] );
		return returnVal-1.0;
	}
	double Get_AdditionalDamagePrecentageSpecial() const
	{
		return m_iAdditionalDamagePrecentage[15];
	}
	void AddDamagePrecentage(const double DamagePrecentage,uint32 iIndex){
		//m_iAdditionalDamagePrecentage = (1.0 +m_iAdditionalDamagePrecentage) * (1.0+DamagePrecentage) - 1.0;
		assert(iIndex < 16);
		if (iIndex < 8)
			m_iAdditionalDamagePrecentage[iIndex] += DamagePrecentage;
		else
			m_iAdditionalDamagePrecentage[iIndex] = (1.0 +m_iAdditionalDamagePrecentage[iIndex]) * (1.0+DamagePrecentage) - 1.0;
	}

	EVENT_ADDITIONAL_ELEMENT(int32,CritRaiting);
	EVENT_ADDITIONAL_ELEMENT(int32,HasteRaiting);
	EVENT_ADDITIONAL_ELEMENT(int32,HitRaiting);
	EVENT_ADDITIONAL_ELEMENT(int32,CastTime);
	EVENT_ADDITIONAL_ELEMENT(int32,Cooldown);
	EVENT_ADDITIONAL_ELEMENT(double,HastePrecentage);

protected:
	e_EventType m_EventType;
	CEvent(CPlayer* thePlayer);
	CPlayer*		m_thePlayer;
	CCreature*	m_theTarget;
	const CEvent*   m_Parent;
	uint32 GetCastTimeWithHaste(uint32 iOriginalTime);
	CEventSystem* m_pEventSystem;

private:
	virtual int PreAction (int param)=0;
	virtual int PostAction (int param)=0;
	void SetEventSystem(CEventSystem* pEventSystem){m_pEventSystem = pEventSystem;}
	int32 m_iAdditionalDamage;
	double m_iAdditionalDamagePrecentage[16];
	double m_iAdditionalHastePrecentage;
	int32 m_iAdditionalCritRaiting;
	int32 m_iAdditionalHasteRaiting;
	int32 m_iAdditionalCastTime;
	int32 m_iAdditionalCooldown;
	int32 m_iAdditionalHitRaiting;
	
};

class CSpellEvent :
			public CEvent
{

public:
	~CSpellEvent();
	CSpellEvent(const CSpell* pSpell,CPlayer* thePlayer,
		const CCreature* theTarget )
	: CEvent(thePlayer)
	{
		m_EventType = EVENT_SPELL_CAST | EVENT_PRE;
		m_pSpell = pSpell;
		m_theTarget = const_cast<CCreature*> (theTarget);
		crit = false;
		CalculatedDamage = 0;
		//m_thePlayer = thePlayer;
	}
	virtual double Get_CritChance();
	virtual uint32 Get_Damage();
	virtual double Get_HitChance();
	virtual double GetCastTime();

	virtual int operator () (int param);

	CLASS_ELEMENT_CONST(const CSpell*,pSpell);
	CLASS_ELEMENT_CONST(const CPlayer*,thePlayer);
	CLASS_ELEMENT_CONST(const CCreature*,theTarget);


private:
	const CSpell*		m_pSpell;

	virtual int PreAction(int param);
	virtual int PostAction(int param);
	bool	crit;
	bool	hit;
	uint32	CalculatedDamage;
	uint32  GCDRecovery;
};


class CAuraEvent :
	public CEvent
{
public:
	CAuraEvent(const CAura* pAura,CPlayer* thePlayer,CCreature* theTarget,
		uint32 iAddStacks = 1,uint32 iFadeTimeTillNow = 0,bool bNotSetFade = false) ;
	//Removal Event
	CAuraEvent(const CAura* pAura,CCreature* theTarget,CPlayer* thePlayer,uint32 ToRemoveGUID);

	~CAuraEvent(){};
	virtual int operator () (int param);
	virtual double Get_CritChance();
	virtual uint32 Get_Damage();
	virtual double Get_HitChance();
	virtual double GetCastTime();
	void	SetApplySpell(const CSpell* pSpell) { m_pSpell = pSpell; }
private:
	const CAura*		m_pAura;
	const CSpell*		m_pSpell;
	uint32				m_iFadeTimeTillNow;
	bool				m_bSetFadeTime;
	uint32				m_iAddStacks;
	bool				crit;
	uint32				CalculatedDamage;
	uint32				m_iPeriodicTime;
	bool				m_aboutRemove;
	CAuraEvent*			m_pPeriodicEvent;
	uint32				m_AuraGUID;

	
	virtual int PreAction(int param){assert(0); return 1;}
	virtual int PostAction(int param){assert(0);return 1;} //TODO: Remove this.
	virtual int AuraApplication(int param);
	virtual int AuraRemoval(int param);
	virtual int AuraPeriodic(int param);

};