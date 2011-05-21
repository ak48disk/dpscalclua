#include "stdafx.h"
#include "EventSystem.h"
#include "Aura.h"
#include "Player.h"
#include "Spell.h"
#include "LuaIntergration.h"
#define printf
void ReportDamage(const char* strSpellName,uint32 iDamage,uint32 iGameTime,bool isCrit);

#pragma region EventSystem
CEventSystem::CEventSystem()
{
	this->m_iGameTime = 0;
	m_bExitable = false;
}
CEventSystem::~CEventSystem()
{
	EventMap::iterator it;
	for(it=m_eventMap.begin(); it != m_eventMap.end() ;++it)
	{
		std::for_each(it->second.begin(),it->second.end(),[&](CEvent* e){delete e;});
	}
	m_eventMap.clear();
}
void CEventSystem::AddEvent(CEvent* pEvent,uint32 DelayTime)
{
	//m_eventMap.insert(std::make_pair<uint32,CEvent*>(this->Get_iGameTime()+DelayTime,pEvent));
	uint32 iGameTime = Get_iGameTime() + DelayTime;
	EventMap::iterator it = m_eventMap.find(iGameTime);
	
	pEvent->SetEventSystem(this);
	
	if (it != m_eventMap.end())  {
		EventList& li = it->second;
		li.push_back(pEvent);
	} else {
		m_eventMap.insert(
			std::make_pair<int,EventList>( iGameTime,EventList(1,pEvent) )
			);
	}

}

bool CEventSystem::IsExitable()
{
	return m_bExitable;
}

bool CEventSystem::ProcessEvent(uint32 NextPlayerAvailableTime)
{
	EventMap::iterator it=m_eventMap.begin();

	while(it!=m_eventMap.end())
	{
		if (it->first <= this->Get_iGameTime())
		{
			EventList& l = it->second;
			if (l.begin() != l.end())
			{
				CEvent* pEvent = *l.begin();

				SetCurrentEvent(pEvent);
				SetCurrentTarget(pEvent->m_theTarget);

				if (pEvent->operator()(0))
					delete pEvent;

				SetCurrentTarget(NULL);
				SetCurrentEvent(NULL);
				
				l.pop_front();
			}
			if (l.empty())
				m_eventMap.erase(it);
			it = m_eventMap.begin();
			if (it == m_eventMap.end() || it->first > this->Get_iGameTime())
				return true;
		} else {
			break;
		}
	}// while
	if (it != m_eventMap.end()) {
		if (NextPlayerAvailableTime)
				m_iGameTime = std::min(it->first,m_iGameTime+NextPlayerAvailableTime);
		else
				m_iGameTime = it->first;
	} else {
		m_iGameTime = m_iGameTime+NextPlayerAvailableTime;
	}
	return false;
}
#pragma endregion




CEvent::CEvent(CPlayer* thePlayer)
{
	m_iAdditionalDamage = 0;
	for (int i=0;i<16;++i)
		m_iAdditionalDamagePrecentage[i] = 0;
	m_iAdditionalCritRaiting = 0;
	m_iAdditionalHasteRaiting = 0;
	m_iAdditionalCastTime = 0;
	m_iAdditionalCooldown = 0;
	m_iAdditionalHitRaiting = 0;
	m_iAdditionalHastePrecentage = 0;
	m_thePlayer = thePlayer;
	m_Parent = NULL;
}
CEvent::~CEvent()
{	
}

void CEvent::ResetAdditionals()
{
	m_iAdditionalDamage = 0;
	for (int i=0;i<16;++i)
		m_iAdditionalDamagePrecentage[i] = 0;
	m_iAdditionalCritRaiting = 0;
	m_iAdditionalHasteRaiting = 0;
	m_iAdditionalCastTime = 0;
	m_iAdditionalCooldown = 0;
	m_iAdditionalHitRaiting = 0;
	m_iAdditionalHastePrecentage = 0;
}

double CEvent::Get_CritChance()
{
	double CritChance = m_thePlayer->Get_CritPrecentage();
	CritChance += m_thePlayer->RatingToPrecentage(this->Get_AdditionalCritRaiting(),
		 PP_CRIT_PRECENTAGE );
	return CritChance;
}

uint32 CEvent::Get_Damage()
{
	// NOT AVAILABLE FOR A GENERIC EVENT
	assert(0);
	return 0;
}

double CEvent::Get_HitChance()
{
	double IdealHitChance = m_thePlayer->Get_HitPrecentage() +
		m_thePlayer->RatingToPrecentage ( this->Get_AdditionalHitRaiting() ,  PP_HIT_PRECENTAGE);
	IdealHitChance = (IdealHitChance > 1.0) ? 1.0 : IdealHitChance;
	return IdealHitChance;
}

double CEvent::GetCastTime()
{
	// NOT AVAILABLE FOR A GENERIC EVENT EITHER
	assert(0);
	return 0;
}
uint32 CEvent::GetCastTimeWithHaste(uint32 iOriginalTime)
{
	double HastePrecentage = m_thePlayer->Get_HastePrecentage() + 
		m_thePlayer->RatingToPrecentage (this->Get_AdditionalHasteRaiting() ,  PP_HASTE_PRECENTAGE);

	return static_cast<uint32> (
		static_cast<double> (iOriginalTime) / (1.0 + HastePrecentage) / (1.0 + m_iAdditionalHastePrecentage)
		);
}





CAuraEvent::CAuraEvent(const CAura *pAura,CPlayer* thePlayer, CCreature *theTarget,
					   uint32 iAddStacks , uint32 iFadeTimeTillNow , bool bNotSetFade )
					   :CEvent(thePlayer)
{
	m_pAura = pAura;
	m_theTarget = theTarget;
	m_iAddStacks = iAddStacks;
	m_iFadeTimeTillNow = iFadeTimeTillNow;
	m_bSetFadeTime = !bNotSetFade;
	CalculatedDamage = 0;
	m_pSpell = NULL;
	m_pPeriodicEvent = NULL;
	m_aboutRemove = false;

	m_EventType = EVENT_AURA_APPLICATION;

	crit = false;

}


CAuraEvent::CAuraEvent (const CAura* pAura,CCreature* theTarget,CPlayer* thePlayer,uint32 ToRemoveGUID)
	:CEvent(thePlayer)
{
	m_pAura = pAura;
	m_theTarget = theTarget;

	m_EventType = EVENT_AURA_REMOVAL;
	
}

double CAuraEvent::Get_CritChance()
{
	if ( !m_pAura->Get_bCritEnabled() )
		if (m_EventType & EVENT_POST)
			if (crit)
				return 1;
			else
				return 0;
		else
			return CEvent::Get_CritChance();
	else
		return 0;
}

uint32 CAuraEvent::Get_Damage()
{
	// todo
	if (!m_pAura->Get_bIsPeriodic() )
		return 0;
	if (CalculatedDamage)
		return CalculatedDamage;

	uint32 Damage = 0;
	uint32 DamageMin = m_pAura->Get_iPeriodicDamageMin();
	uint32 DamageMax = m_pAura->Get_iPeriodicDamageMax();
	double SpellPowerFactor = 0;
	// It shouldn't happen here
	// TODO: ELIMINATE THIS IF
	/*
	if (strstr(m_pAura->Get_strAuraName(),"Pain") ) 
		SpellPowerFactor = 1.1;
	else
		if (strstr(m_pAura->Get_strAuraName(),"Touch") ) 
			SpellPowerFactor = m_pSpell->Get_iAuraFadeTime() / 15000.0 * 2;
		else if (m_pSpell)
			SpellPowerFactor = m_pSpell->Get_iAuraFadeTime() / 15000.0 ;
	*/

	SpellPowerFactor = m_pAura->Get_dSpellPowerFactorPreTick();
	
	if (!SpellPowerFactor)
		SpellPowerFactor = (m_pSpell->Get_iAuraFadeTime() / 15000.0) / (m_pSpell->Get_iAuraFadeTime() / m_pAura->Get_iPeriodicEventTime() );

	CAura* pTargetAura = m_theTarget->GetAuraByIDForModification( m_pAura->Get_iAuraID() );

	uint32 iSpellPower;

	if (pTargetAura && pTargetAura->Get_iSpellPower())
		iSpellPower = pTargetAura->Get_iSpellPower();
	else
		iSpellPower = m_thePlayer->Get_SpellPower();

	DamageMin += SpellPowerFactor * iSpellPower;
	DamageMax += SpellPowerFactor * iSpellPower;

	DamageMin *= (1.0 + CEvent::Get_AdditionalDamagePrecentage() );
	DamageMax *= (1.0 + CEvent::Get_AdditionalDamagePrecentage() );

	DamageMin += CEvent::Get_AdditionalDamage();
	DamageMax += CEvent::Get_AdditionalDamage();
	
	DamageMin *= (1.0 + CEvent::Get_AdditionalDamagePrecentageSpecial() );
	DamageMax *= (1.0 + CEvent::Get_AdditionalDamagePrecentageSpecial() );

	Damage = DamageMin + (DamageMax - DamageMin) * ( (rand() % 1001) / 1000.0 );
	return Damage;
}

double CAuraEvent::Get_HitChance()
{
	return 1.0;  //Aura are not influenced by hit
}
double CAuraEvent::GetCastTime()
{
	if ( !m_pAura->Get_bHasteEnabled() )
		return m_pAura->Get_iPeriodicEventTime();
	else
		return CEvent::GetCastTimeWithHaste(m_pAura->Get_iPeriodicEventTime());
	return 0;
}

int CAuraEvent::operator () (int param)
{
	if ( (m_EventType & EVENT_AURA_APPLICATION) == EVENT_AURA_APPLICATION) 
		return AuraApplication(param);

	if ( (m_EventType & EVENT_AURA_REMOVAL) == EVENT_AURA_REMOVAL)
		return AuraRemoval(param);

	if ( (m_EventType & EVENT_AURA_PERIODIC_EVENT) == EVENT_AURA_PERIODIC_EVENT) 
		return AuraPeriodic(param);

	//invaild type,delete it
	return 1;
}
int CAuraEvent::AuraApplication(int param)
{
	// Aura Application

	CallLuaFunction("AuraApplicationRoutine",m_pAura->Get_iAuraID());

#pragma region Step 1, Check the target if it has the aura
	CAura* pTargetAura = m_theTarget->GetAuraByIDForModification(m_pAura->Get_iAuraID () );

	if (pTargetAura) // Aura already exists, it is a refresh
	{
		//Add the stacks
		pTargetAura->Set_iTicks(pTargetAura->Get_iTicks() + m_iAddStacks);
		//Regain its spell power
		if (m_pAura->Get_bIsPeriodic())
			pTargetAura->Set_iSpellPower(m_thePlayer->Get_SpellPower());

		//Recalculate fade time AND add periodic events if necessary
		
		if (m_bSetFadeTime)
		{
			if (m_pAura->Get_bHasteEnabled())
				m_iFadeTimeTillNow = this->GetCastTimeWithHaste(m_iFadeTimeTillNow);

			if ( m_iFadeTimeTillNow )
				pTargetAura->Set_iFadeTime( m_iFadeTimeTillNow +	m_pEventSystem->Get_iGameTime () + 1 );
			else
				pTargetAura->Set_iFadeTime(0);

		}
		return 1; // This event has come to an end
	}
#pragma endregion

#pragma region Step 2, Apply new aura to the target

		pTargetAura = new CAura(m_pAura);
		pTargetAura->Set_iTicks(m_iAddStacks);

		if ( m_pAura->Get_bHasteEnabled() && m_iFadeTimeTillNow )
		{
			//Deal with HASTE here , we must be VERY careful to
			//maintain the number of ticks doesn't change with haste
			//  ***m_iFadeTimeTillNow = this->GetCastTimeWithHaste(m_iFadeTimeTillNow);*** THIS IS WRONG
			m_iFadeTimeTillNow = this->GetCastTime() * /*floor*/(uint32)( m_iFadeTimeTillNow / m_pAura->Get_iPeriodicEventTime() );
		}

		if ( m_iFadeTimeTillNow )
			pTargetAura->Set_iFadeTime( this->m_iFadeTimeTillNow +	m_pEventSystem->Get_iGameTime ()  + 1);
		else
			pTargetAura->Set_iFadeTime(0);

		m_theTarget->ApplyAura(pTargetAura);
#pragma endregion

#pragma region Step 3, Create removal and periodic events 

		if ( m_iFadeTimeTillNow )
		{
			CAuraEvent* newEvent = new CAuraEvent(m_pAura,m_thePlayer,m_theTarget);
			newEvent->m_EventType = EVENT_AURA_REMOVAL;
			newEvent->SetApplySpell(m_pSpell);
			newEvent->m_AuraGUID = pTargetAura->Get_iGUID();
			m_pEventSystem->AddEvent(newEvent,m_iFadeTimeTillNow + 1);
		}
		
		if ( m_pAura->Get_bIsPeriodic() )
		{
			CAuraEvent* newEvent = new CAuraEvent(m_pAura,m_thePlayer,m_theTarget);
			newEvent->m_EventType = EVENT_AURA_PERIODIC_EVENT;
			newEvent->SetApplySpell(m_pSpell);
			newEvent->m_AuraGUID = pTargetAura->Get_iGUID();
			//Haste bug in wow: Haste rating is not updated immediately for existing auras
			//The exception is corruption for warlocks, which means that 
			//Haste IS upadted when refresh
			newEvent->m_iPeriodicTime = GetCastTime();
			m_pEventSystem->AddEvent(newEvent,newEvent->m_iPeriodicTime);
			//Spell power bug in wow: Spell power is not updated immediately for existing auras
			//but IS updated when refresh
			pTargetAura->Set_iSpellPower(m_thePlayer->Get_SpellPower());
		}


#pragma endregion

		return 1;
}

int CAuraEvent::AuraRemoval(int param)
{
#pragma region	First check the aura and GUID 
	CAura* pTargetAura = m_theTarget->GetAuraByIDForModification( m_pAura->Get_iAuraID() );
	if ( (!pTargetAura) || pTargetAura->Get_iGUID() != m_AuraGUID )
		return 1; //NOT Match, the aura are somehow removed, exiting event
#pragma endregion

#pragma region Then check the aura s fade time
	if (pTargetAura->Get_iFadeTime() >	m_pEventSystem->Get_iGameTime() )
	{
		//NOT now, delay removal
		m_pEventSystem->AddEvent(this,	pTargetAura->Get_iFadeTime() -	m_pEventSystem->Get_iGameTime()	);
		return 0;//do NOT destroy event
	}
#pragma endregion

#pragma region Peform removal
	CallLuaFunction("AuraFadeRoutine",m_pAura->Get_iAuraID());
	m_theTarget->RemoveAura(pTargetAura);
	delete pTargetAura;
#pragma endregion

	return 1;

	//
}

int CAuraEvent::AuraPeriodic(int param)
{

#pragma region	First check the aura and GUID 
	CAura* pTargetAura = m_theTarget->GetAuraByIDForModification( m_pAura->Get_iAuraID() );
	if ( (!pTargetAura) || pTargetAura->Get_iGUID() != m_AuraGUID )
		return 1; //NOT Match, the aura are somehow removed, exiting event
#pragma endregion

#pragma region Peform periodic event
	CallLuaFunction("PrePeriodicEventRoutine",	m_pAura->Get_iAuraID() );

	crit = ( (rand() % 1000) < CEvent::Get_CritChance() * 1000 );

	CalculatedDamage = this->Get_Damage();

	if (crit)
		ReportDamage(m_pAura->Get_strAuraName(),CalculatedDamage*m_thePlayer->Get_dCritBonus(),m_pEventSystem->Get_iGameTime(),crit);
	else
		ReportDamage(m_pAura->Get_strAuraName(),CalculatedDamage,m_pEventSystem->Get_iGameTime(),crit);

	CallLuaFunction("PostPeriodicEventRoutine",	m_pAura->Get_iAuraID() );
#pragma endregion 

#pragma region To be continued

	this->ResetAdditionals();
	CalculatedDamage = 0;

	if (m_pEventSystem->Get_iGameTime() + this->m_iPeriodicTime <= pTargetAura->Get_iFadeTime() )
	{
		//TO BE continued
		m_pEventSystem->AddEvent ( this , this->m_iPeriodicTime ); 
		return 0;
	}
#pragma endregion

	return 1;

}


CSpellEvent::~CSpellEvent()
{
}
double CSpellEvent::Get_CritChance()
{
	if ( (Get_eType() & EVENT_POST) == EVENT_POST)
		if (crit)
			return 1.0;
		else
			return 0.0;
	else
		return CEvent::Get_CritChance();
}

uint32 CSpellEvent::Get_Damage()
{
	if ( (m_pSpell->Get_SpellType() & SPELL_DAMAGE) != SPELL_DAMAGE &&
		(m_pSpell->Get_SpellType() & SPELL_CHANNEL) != SPELL_CHANNEL )
		return 0;

	uint32 Damage;

	if (CalculatedDamage)
		return CalculatedDamage;

	uint32 DamageMin = m_pSpell->Get_iDamageMin();
	uint32 DamageMax = m_pSpell->Get_iDamageMax();

	double SpellPowerFactor = std::max(m_pSpell->Get_SpellCastTime(),1500U) / 1000.0 / 3.50;

	if ( m_pSpell->Get_SPPrecentage() != 0.0 ) 
		SpellPowerFactor = m_pSpell->Get_SPPrecentage();

	if ( m_pSpell->Get_iDamageMax() == 0)
		SpellPowerFactor = 0.0; //This MUST BE a spell with damage added by script

	if ( (m_pSpell->Get_SpellType() & SPELL_CHANNEL) == SPELL_CHANNEL )
		SpellPowerFactor /= m_pSpell->Get_ChannelTicks();

	DamageMin += m_thePlayer->Get_SpellPower() * SpellPowerFactor;
	DamageMax += m_thePlayer->Get_SpellPower() * SpellPowerFactor;

	DamageMin *= (1.0 + CEvent::Get_AdditionalDamagePrecentage() );
	DamageMax *= (1.0 + CEvent::Get_AdditionalDamagePrecentage() );

	DamageMin += CEvent::Get_AdditionalDamage();
	DamageMax += CEvent::Get_AdditionalDamage();

	DamageMin *= (1.0 + CEvent::Get_AdditionalDamagePrecentageSpecial() );
	DamageMax *= (1.0 + CEvent::Get_AdditionalDamagePrecentageSpecial() );

	
	Damage = DamageMin + (DamageMax - DamageMin) * ((rand() %1001) / 1000.0);
	//
	return Damage;
}
double CSpellEvent::Get_HitChance()
{
	if ( (Get_eType() & EVENT_POST) == EVENT_POST)
		if (hit)
			return 1.0;
		else
			return 0.0;
	else
		return CEvent::Get_HitChance();
}
double CSpellEvent::GetCastTime()
{
	return CEvent::GetCastTimeWithHaste(
		m_pSpell->Get_SpellCastTime() ) + CEvent::Get_AdditionalCastTime() ;
}

int CSpellEvent::operator () (int param)
{
	// If it is a channel spell, check player casting state
	if ( (Get_eType() & EVENT_PRE) == EVENT_PRE)  {
		if ( (m_EventType & EVENT_SPELL_CAST) == EVENT_SPELL_CAST)  {
			if (m_thePlayer->IsIdle() == false)
				return 1;
		} else {
			if (m_thePlayer->Get_CastingSpell() != m_Parent)
				return 1;
		}
		
		int result = PreAction(param);
		this->m_EventType ^= EVENT_PRE;
		this->m_EventType ^= EVENT_POST;
		if ( (m_EventType & EVENT_SPELL_CAST) == EVENT_SPELL_CAST)
		{
			if (result == 0)
				m_pEventSystem->AddEvent(this,GetCastTime());
			else
				return 1;
		}
		else
			m_pEventSystem->AddEvent(this,0);  // Channel spells are considered as instant cast with its time gained after post tick
		return 0;
	} else if ( (Get_eType() & EVENT_POST) == EVENT_POST)  {
		//check the player is really casting the spell or die
		if ( (m_EventType & EVENT_SPELL_CAST) == EVENT_SPELL_CAST) 
		{
			if (m_thePlayer->Get_CastingSpell() == this)
				PostAction(param);
		} else {
			if (m_thePlayer->Get_CastingSpell() == m_Parent)
				PostAction(param);
		}	

		return 1;
	}

	return 0;
}


int CSpellEvent::PreAction (int param)
{
	//call the lua script
	if ( (m_EventType & EVENT_SPELL_CAST)== EVENT_SPELL_CAST)  {
		CallLuaFunction("PreCastingSpellRoutine",m_pSpell->Get_iSpellID());
		//Set player casting state and preserve gcd timer
		m_thePlayer->Set_CastingSpell(this);

		GCDRecovery = this->m_pEventSystem->Get_iGameTime() +
			m_thePlayer->Get_GlobalCooldownTime();

		//If it is a channel spell , gain channel events
		if ( (m_pSpell->Get_SpellType() & SPELL_CHANNEL) == SPELL_CHANNEL)
		{
			//But we should check hit first

			hit = ( (rand() % 1000) < CEvent::Get_HitChance() * 1000 );
	
			if (!hit)  {
				m_thePlayer->Set_GlobalCooldownTime(GCDRecovery);
				m_thePlayer->Set_CastingSpell(NULL);
				return 1;
			}
			

			uint32 i;
			for (i=1; i<= m_pSpell->Get_ChannelTicks(); ++i)  {
				CSpellEvent* newEvent = new CSpellEvent(m_pSpell,m_thePlayer,m_theTarget);
				newEvent->m_EventType = EVENT_SPELL_CHANNEL | EVENT_PRE;
				newEvent->m_Parent = this;
				m_pEventSystem->AddEvent(newEvent,
					this->GetCastTime() / m_pSpell->Get_ChannelTicks() * i -1 );
			}
		}

		printf("[%d]Casting spell:%s\n",m_pEventSystem->Get_iGameTime(),m_pSpell->Get_SpellName());

	}
	else  {
		// This is a channel event, we need olny to call the lua script for the main event 
		// is up to the spell cast event
		// Before we do this we must check if the spell is casting by the player
		if (m_thePlayer->Get_CastingSpell () == m_Parent)
			CallLuaFunction("PreSpellChannelRoutine",m_pSpell->Get_iSpellID());
	}
	
		
	return 0;
}
int CSpellEvent::PostAction (int param)
{
	// calc damage and determine crit
	hit = ( (rand() % 1000) < CEvent::Get_HitChance() * 1000 );
	
	if (!hit)  {
			//ReportNotHit()
		
		if ( (m_EventType & EVENT_SPELL_CAST) == EVENT_SPELL_CAST)  {
				m_thePlayer->Set_GlobalCooldownTime(GCDRecovery);
				m_thePlayer->Set_CastingSpell(NULL);
		}
			return 0;
	}

	if (m_pSpell->Get_SpellType() & SPELL_DAMAGE ||
		( (m_EventType & EVENT_SPELL_CHANNEL) == EVENT_SPELL_CHANNEL 	))  {
		crit = ( (rand() % 1000) <= CEvent::Get_CritChance() * 1000 );
	}


	// and gain aura
	if (m_pSpell->Get_SpellType() & SPELL_AURA_APPLICATION )  {
		const CAura* pAura = m_pSpell->Get_pAuraToApply();
		//First check that the aura if it is exist , if it is, remove it

		if (CAura* pTargetAura = m_thePlayer->GetAuraByIDForModification( m_pSpell->Get_pAuraToApply()->Get_iAuraID() ) )
		{
			// Remove it first
			pTargetAura->Set_iFadeTime(1); //FORCE Removal
			CAuraEvent* newEvent = new CAuraEvent(pAura,const_cast<CCreature*>(m_theTarget), m_thePlayer,pTargetAura->Get_iGUID() );
			newEvent->SetApplySpell(m_pSpell);
			m_pEventSystem->AddEvent(newEvent,0);
		}

		CAuraEvent* newEvent = new CAuraEvent(pAura,m_thePlayer,
			const_cast<CCreature*>(m_theTarget),
			1,m_pSpell->Get_iAuraFadeTime());
		newEvent->SetApplySpell(m_pSpell);
		m_pEventSystem->AddEvent(newEvent,0);
	}

	if ( (m_EventType & EVENT_SPELL_CAST) == EVENT_SPELL_CAST)  {
		//call the lua script
		CallLuaFunction("PostCastingSpellRoutine",m_pSpell->Get_iSpellID());
		//Set player cooldown and gcd (if any)
		if (m_pSpell->Get_SpellCooldown() )  {
			m_thePlayer->Set_SpellCooldown( m_pSpell->Get_iSpellID() , m_pEventSystem->Get_iGameTime()
				+ m_pSpell->Get_SpellCooldown() + this->Get_AdditionalCooldown() );
		}
		m_thePlayer->Set_GlobalCooldownTime(GCDRecovery);
		m_thePlayer->Set_CastingSpell(NULL);

		
		
	} else {
		CallLuaFunction("PostSpellChannelRoutine",m_pSpell->Get_iSpellID());

	}
	
	if ( (m_EventType & EVENT_SPELL_CAST) != EVENT_SPELL_CAST ||
		! (m_pSpell->Get_SpellType() & SPELL_CHANNEL) )  {

			CalculatedDamage = this->Get_Damage();

	}

	// Report damage
	if (crit)
		ReportDamage(m_pSpell->Get_SpellName(),CalculatedDamage*m_thePlayer->Get_dCritBonus(),m_pEventSystem->Get_iGameTime(),crit);
	else
		ReportDamage(m_pSpell->Get_SpellName(),CalculatedDamage,m_pEventSystem->Get_iGameTime(),crit);
	
	printf("[%d]Cast spell:%s [%d]\n",m_pEventSystem->Get_iGameTime(),m_pSpell->Get_SpellName(),CalculatedDamage);

	return 0;
}