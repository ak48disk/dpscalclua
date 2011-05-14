#include "Global.h"
#include "Creature.h"

#pragma once

class CSpell;
class CEventSystem;
class CSpellEvent;

class CPlayer :
	public CCreature
{
	typedef std::map<uint32,uint32> CooldownMap;
public:
	CPlayer(const CEventSystem*);
	~CPlayer(){};

	//CLASS_ELEMENT_UINT32(Level);

	CLASS_ELEMENT_UINT32(Intelligence);
	CLASS_ELEMENT_UINT32(Spirit);
	CLASS_ELEMENT_UINT32(SpellPower);
	CLASS_ELEMENT_UINT32(CritRating);
	CLASS_ELEMENT_UINT32(HasteRating);
	CLASS_ELEMENT_UINT32(HitRating);
	CLASS_ELEMENT(const CSpellEvent*,CastingSpell);
	CLASS_ELEMENT(double,dCritBonus);

	//CLASS_ELEMENT(bool,bIdle);

	bool IsIdle() const;
	bool StopCasting ();

	double Get_CritPrecentage() const;
	double Get_HastePrecentage() const;
	double Get_HitPrecentage() const;

	double RatingToPrecentage(uint32 iRating,eL_PlayerPrecentage e_PrecentageType) const;
	uint32 PrecentageToRating(double dPrecentage,eL_PlayerPrecentage e_PrecentageType) const;

	double Get_SpellCastTime(const CSpell* pSpell) const;

	double Get_GlobalCooldownTime() const;

	uint32 Get_SpellCooldown(uint32 iSpellID) const;
	void   Set_SpellCooldown(uint32 iSpellID,uint32 iCooldownTime);
	void   Set_GlobalCooldownTime(uint32 iGCDTime);

	bool   IsSpellAvailable(uint32 iSpellID) const;


private:
	//uint32 m_iLevel;
	uint32 m_iIntelligence;
	uint32 m_iSpirit;
	uint32 m_iSpellPower;
	uint32 m_iCritRating;
	uint32 m_iHasteRating;
	uint32 m_iHitRating;
	int32 m_iAdditionalIntelligence;
	int32 m_iAdditionalSpirit;
	int32 m_iAdditionalSpellPower;
	int32 m_iAdditionalCritRating;
	int32 m_iAdditionalHasteRating;
	int32 m_iAdditionalHitRating;
	double m_dIntelligenceMultiplier;
	double m_dSpiritMultiplier;
	double m_dSpellPowerMultiplier;
	double m_dCritRatingMultiplier;
	double m_dHasteRatingMultiplier;
	double m_dHitRatingMultiplier;
	double m_dCritBonus;

	uint32 m_iGlabalCooldownRecovery;
	CooldownMap m_CooldownMap;
	const CEventSystem*	const m_pEventSystem ;
#ifdef _DEBUG
public:
#endif
	const CSpellEvent*	m_CastingSpell;
	bool   m_bIsCasting;

};