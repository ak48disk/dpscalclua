#include "stdafx.h"

#include "Player.h"
#include "Aura.h"
#include "Spell.h"
#include "EventSystem.h"

const uint32 DEFAULT_PLAYER_LEVEL = 80;
const double CRIT_RATING_PRE_PRECENT = 45.91;
const double HIT_RATING_PRE_PRECENT = 26.232;
const double HASTE_RATING_PRE_PRECENT = 32.79;
const double CRIT_PRECENT_PRE_INTELLECT = 166.6667;

CPlayer::CPlayer(const CEventSystem* pEventSystem) :
		m_pEventSystem(pEventSystem)
{
	Set_strName("player");
	//Set_Level(DEFAULT_PLAYER_LEVEL);
	Set_CritRating(0);
	Set_HasteRating(0);
	Set_HitRating(0);
	Set_Intelligence(0);
	Set_Spirit(0);
	Set_SpellPower(0);
	m_iAdditionalIntelligence = 0;
	m_iAdditionalSpirit = 0;
	m_iAdditionalSpellPower = 0;
	m_iAdditionalCritRating = 0;
	m_iAdditionalHasteRating = 0;
	m_iAdditionalHitRating = 0;
	m_dSpiritMultiplier = 0;
	m_dIntelligenceMultiplier = 0;
	m_dSpellPowerMultiplier = 0;
	m_dCritRatingMultiplier = 0;
	m_dHitRatingMultiplier = 0;
	m_dHasteRatingMultiplier = 0;
	m_iGlabalCooldownRecovery = 0;
	m_dCritBonus = 2.0;
	m_bIsCasting = false;
	m_CastingSpell = NULL;
}

uint32 CPlayer::PrecentageToRating(double dPrecentage, eL_PlayerPrecentage e_PrecentageType) const
{
	switch (e_PrecentageType)  {
		case PP_CRIT_PRECENTAGE:
			return static_cast<uint32>(dPrecentage * CRIT_RATING_PRE_PRECENT * 100.0 );
		case PP_HASTE_PRECENTAGE:
			return static_cast<uint32>(dPrecentage * HASTE_RATING_PRE_PRECENT * 100.0 );
		case PP_HIT_PRECENTAGE:
			return static_cast<uint32>(dPrecentage * HIT_RATING_PRE_PRECENT * 100.0 );
	}
	return 0;
}

double CPlayer::RatingToPrecentage(uint32 iRating, eL_PlayerPrecentage e_PrecentageType) const
{
	switch (e_PrecentageType)  {
		case PP_CRIT_PRECENTAGE:
			return static_cast<double>(iRating) / CRIT_RATING_PRE_PRECENT /100.0;
		case PP_HASTE_PRECENTAGE:
			return static_cast<double>(iRating) / HASTE_RATING_PRE_PRECENT /100.0;
		case PP_HIT_PRECENTAGE:
			return static_cast<double>(iRating) / HIT_RATING_PRE_PRECENT /100.0;
	}

	return 0;
}

double CPlayer::Get_HastePrecentage() const	{
	return RatingToPrecentage(Get_HasteRating(),PP_HASTE_PRECENTAGE);
}

double CPlayer::Get_HitPrecentage() const  {
	double IdealHitPrecentage  = 0.83 + RatingToPrecentage(Get_HitRating(),PP_HIT_PRECENTAGE) ;
	return (IdealHitPrecentage > 1.0 ? 1.0 : IdealHitPrecentage);
}

double CPlayer::Get_CritPrecentage() const {
	double CritProvidedByRating = RatingToPrecentage(Get_CritRating(),PP_CRIT_PRECENTAGE);
	double CritProvidedByIntellect = static_cast<double> ( Get_Intelligence() ) / CRIT_PRECENT_PRE_INTELLECT / 100.0;
	double CritProvidedByClass = 1.24 / 100.0;

	return CritProvidedByRating + CritProvidedByIntellect
		+ CritProvidedByClass;

}

double CPlayer::Get_SpellCastTime(const CSpell *pSpell)  const
{
	return static_cast<double>(pSpell->Get_SpellCastTime()) /(1.0+Get_HastePrecentage());
}

double CPlayer::Get_GlobalCooldownTime()  const
{
	double IdealGCD = 1.5 /(1.0+Get_HastePrecentage());
	return (IdealGCD < 1.0 ? 1000 : IdealGCD*1000);
}

bool CPlayer::IsIdle() const
{
	return (m_CastingSpell == NULL/*!m_bIsCasting*/) && (m_iGlabalCooldownRecovery <= m_pEventSystem->Get_iGameTime());
}

uint32 CPlayer::Get_SpellCooldown(uint32 iSpellID) const
{
	CooldownMap::const_iterator it;
	uint32 iSpellRemain = 0;
	uint32 iGCDRemain = 0;
	if (m_iGlabalCooldownRecovery > m_pEventSystem->Get_iGameTime())
		iGCDRemain = m_iGlabalCooldownRecovery - m_pEventSystem->Get_iGameTime();

	uint32 iIdealCooldown = 0;
	it = m_CooldownMap.find(iSpellID);
	if (it != m_CooldownMap.end())
		if (it->second > m_pEventSystem->Get_iGameTime())
			iSpellRemain = it->second - m_pEventSystem->Get_iGameTime();

	iIdealCooldown = std::max(iSpellRemain,iGCDRemain);
	if (iIdealCooldown < 0)
		return 0;
	else
		return iIdealCooldown;
}

bool CPlayer::IsSpellAvailable(uint32 iSpellID) const
{
	return ( IsIdle() ) && (Get_SpellCooldown(iSpellID) == 0);
}

void   CPlayer::Set_SpellCooldown(uint32 iSpellID,uint32 iCooldownTime)
{
	CooldownMap::iterator it = m_CooldownMap.find(iSpellID);
	if (it != m_CooldownMap.end() )
		it->second = iCooldownTime;
	else
	{
		m_CooldownMap.insert( std::make_pair<uint32,uint32> ( iSpellID , iCooldownTime ) );
	}
}
void   CPlayer::Set_GlobalCooldownTime(uint32 iGCDTime)
{
	m_iGlabalCooldownRecovery = iGCDTime;
}

bool CPlayer::StopCasting()
{
	if (m_CastingSpell){
		m_CastingSpell = NULL;
		return true;
	}
	return false;
}