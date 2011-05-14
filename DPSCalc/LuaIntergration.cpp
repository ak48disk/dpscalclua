#include "stdafx.h"
#include "Global.h"
#include "EventSystem.h"
#include "Spell.h"
#include "Player.h"
#include "LuaIntergration.h"
#include "assert.h"

CEventSystem* pEventSystem = NULL;
CEvent*		pCurrentEvent = NULL;
CCreature*	pCurrentTarget = NULL;
CPlayer*	pCurrentPlayer = NULL;
lua_State*	pLuaState	 =	NULL;
CAuraDB*		pAuraDB;
CSpellDB*		pSpellDB;
CCreatureDB*	pCreatureDB;
#define theAuraDB (*pAuraDB)
#define theSpellDB (*pSpellDB)
#define theCreatureDB (*pCreatureDB)

#define LUAEXPORT


bool SetCurrentPlayer(CPlayer* pPlayer)
{
	if (pCurrentPlayer && pPlayer)
	{
		assert(0);
		return false;
	}
	pCurrentPlayer = pPlayer;
	return true;
}

bool SetCurrentTarget(CCreature* pTarget)
{
	if (pCurrentTarget && pTarget) {
		assert(0);
		return false;
	}
	pCurrentTarget = pTarget;
	return true;
}

bool SetCurrentEvent(CEvent*	pEvent)
{
	if (pCurrentEvent && pEvent)  {
		assert(0);
		return false;
	}
	pCurrentEvent = pEvent;
	return true;
}

bool SetEventSystem(CEventSystem* pES)  {
	if (pEventSystem && pES)  {
		assert(0);
		return false;
	}
	pEventSystem = pES;
	return true;
}

CEventSystem* GetEventSystem()  {
	return pEventSystem;
}

CEvent* GetCurrentEvent() 
{
	return pCurrentEvent;
}

CCreature* GetCurrentTarget()
{
	return pCurrentTarget;
}

CPlayer* GetCurrentPlayer()
{
	return pCurrentPlayer;
}


static int traceback (lua_State *L) {
  if (!lua_isstring(L, 1))  /* 'message' not a string? */
    return 1;  /* keep it intact */
  lua_getfield(L, LUA_GLOBALSINDEX, "debug");
  if (!lua_istable(L, -1)) {
    lua_pop(L, 1);
    return 1;
  }
  lua_getfield(L, -1, "traceback");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 2);
    return 1;
  }
  lua_pushvalue(L, 1);  /* pass error message */
  lua_pushinteger(L, 2);  /* skip this function and traceback */
  lua_call(L, 2, 1);  /* call debug.traceback */
  // print it by default
  if ( !lua_isnil(L, -1)) {
    const char *msg = lua_tostring(L, -1);
    if (msg == NULL) msg = "(error object is not a string)";
    fprintf(stderr,"%s\n", msg);
    lua_pop(L, 1);
  }
  return 0;
}


static int docall (lua_State *L, int narg) {
  int status;
  int base = lua_gettop(L) - narg;  /* function index */
  lua_pushcfunction(L, traceback);  /* push traceback function */
  lua_insert(L, base);  /* put it under chunk and args */
  status = lua_pcall(L, narg, LUA_MULTRET , base);
  lua_remove(L, base);  /* remove traceback function */
  /* force a complete garbage collection in case of errors */
  if (status != 0) lua_gc(L, LUA_GCCOLLECT, 0);
  return status;
}

void CallLuaFunction(const char * strFunctionName,int nParam)
{
	lua_getglobal(pLuaState,strFunctionName);
	if (nParam)  {
		lua_pushnumber(pLuaState,nParam);
		docall(pLuaState,1);
	}
	else
		docall(pLuaState,0);
}


/*********
Lua Exports

Aura

id = AuraAdd(name,isPeriodic,PeriodicDamageMin (per tick),PeriodicDamageMax,PeriodicEventTime,spellSchool)
id = AuraNameToID (name)
name = AuraIDToName (id)
*/

int LUAEXPORT L_AuraAdd(lua_State* L)
{
	const char* AuraName; bool isPeriodic;
	uint32 PeriodicDamageMin; uint32 PeriodicDamageMax;
	uint32 PeriodicEventTime; uint32 spellSchool;
	CAura* pAura;
	LUA_TO_STRING(L,1,AuraName);
	LUA_TO_BOOLEAN(L,2,isPeriodic);
	if (isPeriodic)	{
		LUA_TO_INTEGER(L,3,PeriodicDamageMin);
		LUA_TO_INTEGER(L,4,PeriodicDamageMax);
		LUA_TO_INTEGER(L,5,PeriodicEventTime);
		LUA_TO_INTEGER(L,6,spellSchool);
		pAura = new CAura(AuraName,1,true,PeriodicDamageMin,
			PeriodicDamageMax,spellSchool,false,false,PeriodicEventTime);
	}  else  {
		pAura = new CAura(AuraName);
	}
	
	if (pAura) {
		theAuraDB.AddAura(pAura);
		lua_pushnumber(L,pAura->Get_iAuraID());
	} else	{
		lua_pushnil(L);
	}

	return 1;
}

int LUAEXPORT L_AuraNameToID(lua_State* L)
{
	const char* AuraName;const CAura* pAura;
	LUA_TO_STRING(L,1,AuraName);
	pAura = theAuraDB.GetAuraByName(AuraName);
	if (pAura)
		lua_pushnumber(L,pAura->Get_iAuraID());
	else
		lua_pushnil(L);
	return 1;
}

int LUAEXPORT L_AuraIDToName(lua_State* L)
{
	uint32 AuraID;const CAura* pAura;
	LUA_TO_INTEGER(L,1,AuraID);
	pAura = theAuraDB.GetAuraByID(AuraID);
	if (pAura)
		lua_pushstring(L,pAura->Get_strAuraName());
	else
		lua_pushnil(L);
	return 1;
}

/*
These functions do nothing on non-periodic auras
AuraEnableCrit(auraID)
AuraDisableCrit(auraID)
AuraEnableHaste(auraID)
AuraDisableHaste(auraID)
*/

int LUAEXPORT L_AuraEnableCrit(lua_State* L)  {
	uint32 AuraID; CAura* pAura;
	LUA_TO_INTEGER(L,1,AuraID);
	pAura = const_cast<CAura*>(theAuraDB.GetAuraByID(AuraID));
	if (pAura)
		pAura->Set_bCritEnabled(true);
	else  {
		lua_pushstring(L,"Aura not found");
		lua_error(L);
	}
	return 0;
}

int LUAEXPORT L_AuraDisableCrit(lua_State* L)  {
	uint32 AuraID; CAura* pAura;
	LUA_TO_INTEGER(L,1,AuraID);
	pAura = const_cast<CAura*>(theAuraDB.GetAuraByID(AuraID));
	if (pAura)
		pAura->Set_bCritEnabled(false);
	else  {
		lua_pushstring(L,"Aura not found");
		lua_error(L);
	}
	return 0;
}

int LUAEXPORT L_AuraEnableHaste(lua_State* L)  {
	uint32 AuraID; CAura* pAura;
	LUA_TO_INTEGER(L,1,AuraID);
	pAura = const_cast<CAura*>(theAuraDB.GetAuraByID(AuraID));
	if (pAura)
		pAura->Set_bHasteEnabled(true);
	else  {
		lua_pushstring(L,"Aura not found");
		lua_error(L);
	}
	return 0;
}

int LUAEXPORT L_AuraDisableHaste(lua_State* L)  {
	uint32 AuraID; CAura* pAura;
	LUA_TO_INTEGER(L,1,AuraID);
	pAura = const_cast<CAura*>(theAuraDB.GetAuraByID(AuraID));
	if (pAura)
		pAura->Set_bHasteEnabled(false);
	else  {
		lua_pushstring(L,"Aura not found");
		lua_error(L);
	}
	return 0;
}

int LUAEXPORT L_AuraGetSchool(lua_State* L)
{
	uint32 iAuraID; const CAura* pAura;
	LUA_TO_INTEGER(L,1,iAuraID);
	pAura = theAuraDB.GetAuraByID(iAuraID);
	if (pAura) 
		lua_pushnumber(L,pAura->Get_iAuraSchool());
	else
		lua_pushnil(L);
	return 1;
}

int LUAEXPORT L_AuraSetSPPrecentagePreTick(lua_State* L)
{
	uint32 AuraID; CAura* pAura;
	double dPrecentage;
	LUA_TO_INTEGER(L,1,AuraID);
	LUA_TO_NUMBER(L,2,dPrecentage);

	pAura = const_cast<CAura*>(theAuraDB.GetAuraByID(AuraID));
	if (pAura)
		pAura->Set_dSpellPowerFactorPreTick(dPrecentage);
	else  {
		lua_pushstring(L,"Aura not found");
		lua_error(L);
	}

	return 0;
}
/*

AuraGetPlayerCritChance(auraID)  note that the Crit Chance will be set to 1 in a post-action if the spell did a crit 
AuraGetPlayerTimePreTick(auraID)
AuraGetPlayerTickDamage(auraID)
SpellGetPlayerDamageMix(spellID)
SpellGetPlayerDamageMax(spellID)
SpellGetPlayerCritChance(spellID)
SpellGetPlalerHitChance(spellID) // this include the 83% base hit chance 
//This will be 1 when this spell is hit
//and will be 0(not nil) when this spell is not hit
//in a post-action Routine
SpellGetPlayerCastTime(spellID)
***Decerpted***
//These auraID can be ommited if is in a Routine that is a pre-casting event
// or a post-casting event
// and if so, returns the real-time properties of these auras
//These function above are only vaild in a pre-action
*/

/*



These functions are vaild only in pre-spellcasting Routine
EventAddDamagePrecentage(value)
EventAddCritRaiting(value)
EventAddHitRaiting(value)
EventAddHasteRaiting(value)
EventAddPlayerCooldown(value)
EventAddCastingTime(value)

//This function is also vaild in post-spellcating Routine
EventAddDamage(value)
*/

/*
EventAddStats(stats,value)
EventGetStats(value)
AuraGetStats(auraID,stats)
SpellGetStats(spellID,stats)
*/
bool __forceinline EventStats(uint32 iStatsType,bool isAdd,double& Value,uint32 iIndex = 0)  {
	CEvent* pEvent = GetCurrentEvent();

	if (!pEvent)
		return false;
	if (isAdd) {
		switch (iStatsType)  {
		case CS_DAMAGE:
			pEvent->AddDamage(Value);
			break;
		case CS_DAMAGE_PRECENTAGE:
			pEvent->AddDamagePrecentage(Value,iIndex);
			break;
		case CS_CRIT_PRECENTAGE:
			pEvent->AddCritRaiting( GetCurrentPlayer()->PrecentageToRating(Value,PP_CRIT_PRECENTAGE) );
			break;
		case CS_HIT_PRECENTAGE:
			pEvent->AddHitRaiting ( GetCurrentPlayer()->PrecentageToRating(Value,PP_HIT_PRECENTAGE) );
			break;
		case CS_CRIT_RATING:
			pEvent->AddCritRaiting( Value );
			break;
		case CS_HIT_RATING:
			pEvent->AddHitRaiting( Value );
			break;
		case CS_HASTE_RATING:
			pEvent->AddHasteRaiting( Value );
			break;
		case CS_CAST_TIME:
			pEvent->AddCastTime (Value );
			break;
		case CS_COOLDOWN:
			pEvent->AddCooldown(Value);
			break;
		default:
			return false;
		
	}
	} else {
		switch (iStatsType)  {
			//ONLY A FEW AVAILABLE
			case CS_DAMAGE:
				Value = pEvent->Get_Damage();
				break;
			case CS_CRIT_PRECENTAGE:
				Value = pEvent->Get_CritChance();
				break;
			case CS_HIT_PRECENTAGE:
				Value = pEvent->Get_HitChance();
				break;
			case CS_CAST_TIME:
				Value = pEvent->GetCastTime();
				break;
			default:
				return false;
		}
	}

	return true;
	
}

int LUAEXPORT L_EventAddStats(lua_State* L)
{
	uint32 iStats; double dValue;
	uint32 iIndex = 0;
	LUA_TO_INTEGER(L,1,iStats);
	LUA_TO_NUMBER(L,2,dValue);

	if (lua_isnumber(L,3))
	{
		iIndex = lua_tointeger(L,3);
		if (iIndex < 0 || iIndex >= 16)
		{
			LUA_ERROR(L,"Invaild Index");
		}
	}

	if (!GetCurrentEvent()) {
		LUA_ERROR(L,"Not in a event");
	}
	else
	{
		if (!EventStats(iStats,true,dValue,iIndex))
		{
			LUA_ERROR(L,"Invaild param #1, COMBAT_STATS expected");
		}
	}

	return 0;
}

int LUAEXPORT L_EventGetStats(lua_State* L)  {
	uint32 iStats; double dValue;
	LUA_TO_INTEGER(L,1,iStats);
	
	if (!GetCurrentEvent()) {
		LUA_ERROR(L,"Not in a event");
	}
	else
	{
		if (!EventStats(iStats,false,dValue))
		{
			LUA_ERROR(L,"Invaild param #1, COMBAT_STATS expected");
		}
	}

	lua_pushnumber(L,dValue);
	return 1;
}

/*
int LUAEXPORT AuraGetStats(lua_State* L)  {
	uint32 iAuraID; uint32 iStats;
	LUA_TO_INTEGER(L,1,iAuraID);
	LUA_TO_INTEGER(L,2,iStats);
	if (GetCurrentEvent() &&
		GetCurrentEvent()->Get_eType() & EVENT_AURA == EVENT_AURA &&
		static_cast<CAuraEvent*> ( GetCurrentEvent() ) ->GetAuraID() == iAuraID )
	{
		
	}
	return 1;
}
*/
/*
Spell
id = SpellAdd(name,SpellType, castTime(ms),cooldownTime,spellSchool,iDamageMin(per tick) , iDamageMax ,auraToApply,auraFadeTime,ChannelTicks ) 
id = SpellNametoID (name)
name = SpellTimeToID (spellID)
*/

int LUAEXPORT L_SpellAdd(lua_State* L)
{
	const char* spellName; 
	CSpell* pSpell = NULL;
	uint32 spellType(0),castTime(0),cooldownTime(0),spellSchool(0),damageMin(0),damageMax(0),auraToApply(0),auraFadeTime(0),channelTicks(0);
	const CAura* pAura = NULL;

	LUA_TO_STRING(L,1,spellName);
	LUA_TO_INTEGER(L,2,spellType);
	LUA_TO_INTEGER(L,3,castTime);
	LUA_TO_INTEGER(L,4,cooldownTime);
	
	if ( (spellType | SPELL_DAMAGE) == spellType)
	{
		LUA_TO_INTEGER(L,5,spellSchool);
		LUA_TO_INTEGER(L,6,damageMin);
		LUA_TO_INTEGER(L,7,damageMax);
	}
	if ( (spellType | SPELL_AURA_APPLICATION) == spellType)
	{
		LUA_TO_INTEGER(L,5,spellSchool);
		LUA_TO_INTEGER(L,8,auraToApply);
		LUA_TO_INTEGER(L,9,auraFadeTime);
		pAura = theAuraDB.GetAuraByID(auraToApply);
		if (!pAura)  {
			LUA_ERROR(L,"Invaild aura id");
		}
	}
	if ( (spellType | SPELL_CHANNEL) == spellType)
	{
		LUA_TO_INTEGER(L,5,spellSchool);
		LUA_TO_INTEGER(L,6,damageMin);
		LUA_TO_INTEGER(L,7,damageMax);
		LUA_TO_INTEGER(L,10,channelTicks);
	}

	pSpell = new CSpell(spellName,spellType,damageMin,damageMax,
		castTime,cooldownTime,spellSchool,pAura,auraFadeTime,channelTicks);

	if (pSpell)  {
		theSpellDB.AddSpell(pSpell);
		lua_pushnumber(L,pSpell->Get_iSpellID());
	}  else  {
		lua_pushnil(L);
	}

	return 1;
			
}

int LUAEXPORT L_SpellNameToID(lua_State* L)
{
	const char* SpellName;const CSpell* pSpell;
	LUA_TO_STRING(L,1,SpellName);
	pSpell = theSpellDB.GetSpellByName(SpellName);
	if (pSpell)
		lua_pushnumber(L,pSpell->Get_iSpellID());
	else
		lua_pushnil(L);
	return 1;
}

int LUAEXPORT L_SpellIDToName(lua_State* L)
{
	uint32 SpellID;const CSpell* pSpell;
	LUA_TO_INTEGER(L,1,SpellID);
	pSpell = theSpellDB.GetSpellByID(SpellID);
	if (pSpell)
		lua_pushstring(L,pSpell->Get_SpellName());
	else
		lua_pushnil(L);
	return 1;
}
/*
SpellGetCooldown(spellID)
SpellGetSchool(spellID)
*/
int LUAEXPORT L_SpellGetCooldown(lua_State* L)
{
	uint32 SpellID;const CSpell* pSpell;
	LUA_TO_INTEGER(L,1,SpellID);
	pSpell = theSpellDB.GetSpellByID(SpellID);
	if (pSpell)
		lua_pushnumber(L,pSpell->Get_SpellCooldown() );
	else
		lua_pushnil(L);
	return 1;
}

int LUAEXPORT L_SpellGetSchool(lua_State* L)
{
	uint32 SpellID;const CSpell* pSpell;
	LUA_TO_INTEGER(L,1,SpellID);
	pSpell = theSpellDB.GetSpellByID(SpellID);
	if (pSpell)
		lua_pushnumber(L,pSpell->Get_iSpellSchool());
	else
		lua_pushnil(L);
	return 1;
}

int LUAEXPORT L_SpellSetSPPrecentage(lua_State* L)
{
	uint32 SpellID;CSpell* pSpell;
	double SPPrecentage;
	LUA_TO_INTEGER(L,1,SpellID);
	pSpell = const_cast<CSpell*> (theSpellDB.GetSpellByID(SpellID));
	if (pSpell)
	{
		LUA_TO_NUMBER(L,2,SPPrecentage);
		pSpell->Set_SPPrecentage(SPPrecentage);
	}
	return 0;
}
/*
//These spellID can be ommited if is in a Routine that is a pre-casting event
// or a post-casting event
PlayerAddStats(stats,value)
PlayerReset()

PlayerGetStats(stats)
PlayerSetStats(stats,value)
PlayerGetPercentage(stats)
*/
int LUAEXPORT L_PlayerAddStats(lua_State* L)
{
	uint32 e_Type; int32 iStatValue;
	LUA_TO_INTEGER(L,1,e_Type);
	LUA_TO_INTEGER(L,2,iStatValue);
	if (e_Type <= PS_LEVEL || e_Type >= PS_INVAILD_MAX)  {
		LUA_ERROR(L,"Invaild param #1");
	}
	switch ( e_Type )  {
		case PS_CRIT_RAITING:
			GetCurrentPlayer()->AddCritRating(iStatValue);
			break;
		case PS_HIT_RAITING:
			GetCurrentPlayer()->AddHitRating(iStatValue);
			break;
		case PS_HASTE_RAITING:
			GetCurrentPlayer()->AddHasteRating(iStatValue);
			break;
		case PS_SPELL_POWER:
			GetCurrentPlayer()->AddSpellPower(iStatValue);
			break;
		case PS_INTELLIGENCE:
			GetCurrentPlayer()->AddIntelligence(iStatValue);
			break;
		case PS_SPIRIT:
			GetCurrentPlayer()->AddSpirit(iStatValue);
			break;
		default:
			// Should never reach here
			assert(0);
			LUA_ERROR(L,"");
	}
	return 0;
}

int LUAEXPORT L_PlayerGetStats(lua_State* L)
{
	uint32 e_Type; uint32 iStatValue;
	LUA_TO_INTEGER(L,1,e_Type);
	if (e_Type <= PS_LEVEL || e_Type >= PS_INVAILD_MAX)  {
		LUA_ERROR(L,"Invaild param #1");
	}
	switch ( e_Type )  {
		case PS_CRIT_RAITING:
			iStatValue = GetCurrentPlayer()->Get_CritRating();
			break;
		case PS_HIT_RAITING:
			iStatValue = GetCurrentPlayer()->Get_HitRating();
			break;
		case PS_HASTE_RAITING:
			iStatValue = GetCurrentPlayer()->Get_HasteRating();
			break;
		case PS_SPELL_POWER:
			iStatValue = GetCurrentPlayer()->Get_SpellPower();
			break;
		case PS_INTELLIGENCE:
			iStatValue = GetCurrentPlayer()->Get_Intelligence();
			break;
		case PS_SPIRIT:
			iStatValue = GetCurrentPlayer()->Get_Spirit();
			break;
		default:
			// Should never reach here
			assert(0);
			LUA_ERROR(L,"");
	}
	lua_pushnumber(L,iStatValue);
	return 1;
}

int LUAEXPORT L_PlayerAddStatsMultiplier(lua_State* L)
{
	uint32 e_Type; double Value;
	LUA_TO_INTEGER(L,1,e_Type);
	LUA_TO_NUMBER(L,2,Value);
	if (e_Type <= PS_LEVEL || e_Type >= PS_INVAILD_MAX)  {
		LUA_ERROR(L,"Invaild param #1");
	}
	switch ( e_Type )  {
		case PS_CRIT_RAITING:
			GetCurrentPlayer()->AddCritRatingMultiplier(Value);
			break;
		case PS_HIT_RAITING:
			GetCurrentPlayer()->AddHitRatingMultiplier(Value);
			break;
		case PS_HASTE_RAITING:
			GetCurrentPlayer()->AddHasteRatingMultiplier(Value);
			break;
		case PS_SPELL_POWER:
			GetCurrentPlayer()->AddSpellPowerMultiplier(Value);
			break;
		case PS_INTELLIGENCE:
			GetCurrentPlayer()->AddIntelligenceMultiplier(Value);
			break;
		case PS_SPIRIT:
			GetCurrentPlayer()->AddSpiritMultiplier(Value);
			break;
		default:
			// Should never reach here
			assert(0);
			LUA_ERROR(L,"");
	}
	
	return 0;
}


int LUAEXPORT L_PlayerSetStats(lua_State* L)
{
	uint32 e_Type; int32 iStatValue;
	LUA_TO_INTEGER(L,1,e_Type);
	LUA_TO_INTEGER(L,2,iStatValue);
	if (e_Type <= PS_LEVEL || e_Type >= PS_INVAILD_MAX)  {
		LUA_ERROR(L,"Invaild param #1");
	}
	switch ( e_Type )  {
		case PS_CRIT_RAITING:
			GetCurrentPlayer()->Set_CritRating(iStatValue);
			break;
		case PS_HIT_RAITING:
			GetCurrentPlayer()->Set_HitRating(iStatValue);
			break;
		case PS_HASTE_RAITING:
			GetCurrentPlayer()->Set_HasteRating(iStatValue);
			break;
		case PS_SPELL_POWER:
			GetCurrentPlayer()->Set_SpellPower(iStatValue);
			break;
		case PS_INTELLIGENCE:
			GetCurrentPlayer()->Set_Intelligence(iStatValue);
			break;
		case PS_SPIRIT:
			GetCurrentPlayer()->Set_Spirit(iStatValue);
			break;
		default:
			// Should never reach here
			assert(0);
			LUA_ERROR(L,"");
	}
	return 0;
}

int LUAEXPORT L_PlayerGetPrecentage(lua_State* L)  {
	uint32 e_Type; double dPrecentage;
	LUA_TO_INTEGER(L,1,e_Type);
	
	if (e_Type <= PP_INVAILD_MIN || e_Type >=  PP_INVAILD_MAX)  {
		LUA_ERROR(L,"Invaild parameter #1,e_Type expected");
	}
	if (CPlayer* thePlayer = GetCurrentPlayer())  {
		switch (e_Type)  {
			case PP_CRIT_PRECENTAGE:
				dPrecentage = thePlayer->Get_CritPrecentage();
				break;
			case PP_HIT_PRECENTAGE:
				dPrecentage = thePlayer->Get_HitPrecentage();
				break;
			case PP_HASTE_PRECENTAGE:
				dPrecentage = thePlayer->Get_HastePrecentage();
				break;
			default:
				assert(0);
				LUA_ERROR(L,"");
		}
		lua_pushnumber(L,dPrecentage);
	}
	else
		lua_pushnil(L);
	return 1;
}
/*
PlayerGetSpellCastTime(spellID)
PlayerGetSpellCooldown(spellID)
PlayerSpellIsAvailable(spellID)
PlayerIsIdle()
*/

int LUAEXPORT L_PlayerGetSpellCastTime(lua_State* L)
{
	uint32 SpellID;const CSpell* pSpell;
	LUA_TO_INTEGER(L,1,SpellID);
	pSpell = theSpellDB.GetSpellByID(SpellID);
	if (pSpell)
		lua_pushnumber(L,GetCurrentPlayer()->Get_SpellCastTime(pSpell) );
	else
		lua_pushnil(L);
	return 1;
}

int LUAEXPORT L_PlayerGetSpellCooldown(lua_State* L)  {
	uint32 SpellID;const CSpell* pSpell;
	LUA_TO_INTEGER(L,1,SpellID);
	pSpell = theSpellDB.GetSpellByID(SpellID);
	if (pSpell)
		lua_pushnumber(L,GetCurrentPlayer()->Get_SpellCooldown(SpellID) );
	else
		lua_pushnil(L);
	return 1;
}

int LUAEXPORT L_PlayerSpellIsAvailable(lua_State* L)  {
	const char* strSpell;const CSpell* pSpell;
	LUA_TO_STRING(L,1,strSpell);
	pSpell = theSpellDB.GetSpellByName(strSpell);
	if (pSpell)
		lua_pushboolean(L,GetCurrentPlayer()->IsSpellAvailable(pSpell->Get_iSpellID() ) );
	else
		lua_pushboolean(L,0);
	return 1;
}

int LUAEXPORT L_PlayerIsIdle(lua_State* L)  {
	lua_pushboolean(L,GetCurrentPlayer()->IsIdle());
	return 1;
}

/*
TargetCreate(name)
TargetDestory(name)

TargetNames : Can be anyting except these reserved names:

target ---	only vaild while pre-casting and post-casting process , refers to the target
			of the casting spell
player ---	refers to the player of this testing system
*/

CCreature*	GetCreatureByName(const char* strCreatureName)
{
	if (strcmp(strCreatureName,"player") == 0)
		return GetCurrentPlayer();
	else if (strcmp(strCreatureName,"target") == 0)
		return GetCurrentTarget();
	else
		return theCreatureDB.GetCreatureByName(strCreatureName);
}

int LUAEXPORT L_TargetCreate(lua_State* L)
{
	const char* strCreatureName;
	CCreature* pCreature;
	LUA_TO_STRING(L,1,strCreatureName);
	if ( ( strcmp(strCreatureName,"player")  == 0) ||
		 ( strcmp(strCreatureName,"target")  == 0) ||
		 GetCreatureByName(strCreatureName) )  {
			 LUA_ERROR(L,"Target already exists");
	}
	pCreature = new CCreature(strCreatureName);
	if (pCreature)
		theCreatureDB.AddCreature(pCreature);
	return 0;
}

int LUAEXPORT L_TargetDestory(lua_State* L)  {
	const char* strCreatureName;
	LUA_TO_STRING(L,1,strCreatureName);
	theCreatureDB.RemoveCreatureByName(strCreatureName);
	return 0;
}

/*
CastSpellByName(spellname,targetname) -- note that unitid "target" is not vaild for this function
									  -- this is only vaild when the player is in an "idle" state
									  -- use PlayerIsIdle() for more information

UnitApplyAura(unit,auraid,incstacks,fadetime(time after apply) ,donotsetfadetime )
Note that when fadetime is 0,the aura will never fade until be canceled

stacks,remainTime = UnitAuraApplied (unit,auraid)
*/
int LUAEXPORT L_CastSpellByName(lua_State* L)  {
	CPlayer* thePlayer = GetCurrentPlayer();
	const char* strSpellName; const char* strTargetName;
	LUA_TO_STRING(L,1,strSpellName);
	LUA_TO_STRING(L,2,strTargetName);
	const CSpell*	 pSpell = theSpellDB.GetSpellByName(strSpellName);
	CCreature* pTarget = GetCreatureByName(strTargetName);
	CSpellEvent* pEvent;
	if (GetCurrentPlayer()->IsIdle() == false || !pSpell || !pTarget)
		return 0;
	pEvent = new CSpellEvent(pSpell,thePlayer,pTarget);
	if (pEvent)
		pEventSystem->AddEvent(pEvent,0);
	
	return 0;
}

int LUAEXPORT L_UnitApplyAura(lua_State* L)  {
	const char* strTargetName; uint32 auraID;
	uint32 iAddStacks = 1; uint32 iFadeTime = 0;
	const CAura* pAura = NULL; CCreature* theTarget;
	CEvent* pEvent;

	bool bNotSetFade = false;
	LUA_TO_STRING(L,1,strTargetName);
	LUA_TO_INTEGER(L,2,auraID);
	if (lua_isnumber(L,3))
		iAddStacks = lua_tointeger(L,3);
	if (lua_isnumber(L,4))
		iFadeTime = lua_tointeger(L,4);
	LUA_TO_BOOLEAN(L,5,bNotSetFade);

	theTarget = GetCreatureByName(strTargetName);
	if (theTarget)  {
		pAura = theAuraDB.GetAuraByID(auraID);
		if (pAura)  {
			pEvent = new CAuraEvent(pAura,GetCurrentPlayer(),theTarget,iAddStacks,iFadeTime,bNotSetFade);
			if (pEvent)
				pEventSystem->AddEvent(pEvent,0);
		}
	}
	return 0;
}

int LUAEXPORT L_UnitAuraApplied (lua_State* L)  {
	const char* UnitID; uint32 auraID;
	CCreature* theUnit; const CAura* theAura;
	LUA_TO_STRING(L,1,UnitID);
	LUA_TO_INTEGER(L,2,auraID);
	theUnit = GetCreatureByName(UnitID);
	if ( theUnit && (theAura = theUnit->GetAuraByID(auraID)) )  {
		lua_pushnumber(L,theAura->Get_iTicks());
		if (theAura->Get_iFadeTime())
			lua_pushnumber(L, theAura->Get_iFadeTime() - pEventSystem->Get_iGameTime() );
		else
			lua_pushnil(L);
	} else {
		lua_pushnil(L);
		lua_pushnil(L);
	}
	return 2;
}

int LUAEXPORT L_StopCasting(lua_State* L)
{
	CPlayer* pPlayer = GetCurrentPlayer();
	if (pPlayer)
		pPlayer->StopCasting();

	return 0;
}

int LUAEXPORT L_PlayerSleep(lua_State* L)
{
	int iSleepTime;
	CPlayer* pPlayer = GetCurrentPlayer();
	LUA_TO_INTEGER(L,1,iSleepTime);
	if ( pPlayer && pPlayer->IsIdle() )  {
		pPlayer->Set_GlobalCooldownTime(
			GetEventSystem()->Get_iGameTime() + iSleepTime );
	}
	return 0;
}
/*
Register Functions

Register for these following events
When player get idle
When casting a spell
When aura applicition/fade
When periodic event

All these events except player gets idle will have two choice
Pre-event and Post-event


*/


/*
PlayerIdleRoutine()
PreCastingSpellRoutine(spellID)
PostCastingSpellRoutine(spellID)
PreSpellChannelRoutine(spellID)
PostSpellChannelRoutine(spellID)
AuraApplicationRoutine(auraID)
AuraFadeRoutine(auraID)
PrePeriodicEventRoutine(auraID)
PostPeriodicEventRoutine(auraID)
*/


LUA_DUMMY_FUNCTION(PlayerIdleRoutine);
LUA_DUMMY_FUNCTION(PreCastingSpellRoutine);
LUA_DUMMY_FUNCTION(PostCastingSpellRoutine);
LUA_DUMMY_FUNCTION(PreSpellChannelRoutine);
LUA_DUMMY_FUNCTION(PostSpellChannelRoutine);
LUA_DUMMY_FUNCTION(AuraApplicationRoutine);
LUA_DUMMY_FUNCTION(AuraFadeRoutine);
LUA_DUMMY_FUNCTION(PrePeriodicEventRoutine);
LUA_DUMMY_FUNCTION(PostPeriodicEventRoutine);
LUA_DUMMY_FUNCTION(OnTestStartRoutine);
LUA_DUMMY_FUNCTION(OnTestFinishRoutine);


/*
Util Functions
PlayerRatingToPrecnetage(rating,type)
PlayerPrecentageToRating(rating,type)
*/

int LUAEXPORT L_PlayerRatingToPrecentage(lua_State* L)
{
	uint32 e_Type; uint32 iRating;
	LUA_TO_INTEGER(L,2,iRating);
	LUA_TO_INTEGER(L,1,e_Type);
	
	if (e_Type <= PP_INVAILD_MIN || e_Type >=  PP_INVAILD_MAX)  {
		LUA_ERROR(L,"Invaild parameter #1,e_Type expected");
	}
	if (CPlayer* thePlayer = GetCurrentPlayer())  {
		lua_pushnumber(L,thePlayer->RatingToPrecentage(iRating,
			static_cast<eL_PlayerPrecentage>(e_Type) ));
	}
	else
		lua_pushnil(L);
	return 1;
}

int LUAEXPORT L_PlayerPrecentageToRating(lua_State* L)
{
	uint32 e_Type; double dPrecentage;
	LUA_TO_NUMBER(L,2,dPrecentage);
	LUA_TO_INTEGER(L,1,e_Type);
	
	if (e_Type <= PP_INVAILD_MIN || e_Type >=  PP_INVAILD_MAX)  {
		LUA_ERROR(L,"Invaild parameter #1,e_Type expected");
	}
	if (CPlayer* thePlayer = GetCurrentPlayer())  {
		lua_pushnumber(L,thePlayer->PrecentageToRating(dPrecentage,
			static_cast<eL_PlayerPrecentage>(e_Type) ));
	}
	else
		lua_pushnil(L);
	return 1;
}

int LUAEXPORT L_GetGameTime(lua_State* L)
{
	lua_pushnumber(L,GetEventSystem()->Get_iGameTime());
	return 1;
}
/*
FinalizeCalculation()

*/
int LUAEXPORT L_FinalizeCalculation(lua_State* L)  {
	GetEventSystem()->ReadyExit();
	return 0;
}

int LUAEXPORT L_PlayerSetCritBonus(lua_State* L)  {
	double dCritBonus;
	LUA_TO_NUMBER(L,1,dCritBonus);
	if (CPlayer* thePlayer = GetCurrentPlayer())
		thePlayer->Set_dCritBonus(dCritBonus);
	return 0;
}

bool LoadTOCFile(const char* fileName)
{
	FILE * fp;
	char Buffer[MAX_STRING_LENGTH];
	char ScriptToLoad[MAX_STRING_LENGTH];
	char* NewLinePosition;
	sprintf(ScriptToLoad,"lua\\%s",fileName);
	if (fp = fopen(ScriptToLoad,"r"))  {
		while(!feof(fp)) {
			if (!fgets(Buffer,MAX_STRING_LENGTH,fp))
				continue;
			if(Buffer[0] == '#') continue;

			sprintf(ScriptToLoad,"lua\\%s",Buffer);
			// Strip \r and \n
			if (NewLinePosition = strstr(ScriptToLoad,"\r") ) *NewLinePosition = 0;
			if (NewLinePosition = strstr(ScriptToLoad,"\n") ) *NewLinePosition = 0;


			if (strstr(ScriptToLoad,".toc"))
			{
				if (!LoadTOCFile(ScriptToLoad)) return false;
				continue;
			}
			
			if (strlen(ScriptToLoad))
			{
				if (luaL_loadfile(pLuaState, ScriptToLoad))
				{
					if (!lua_isnil(pLuaState, -1)) {
						const char *msg = lua_tostring(pLuaState, -1);
						if (msg == NULL) msg = "(error object is not a string)";
						fprintf(stderr,"%s\n",msg);
						lua_pop(pLuaState, 1);
					}
					return false;
				}
				else if ( docall(pLuaState, 0) ) return false;
			}

		}
		fclose(fp);
		return true;
	}
	return false;
}

bool Lua_Initialize(char* PreloadAdditionalLua)
{
	
	
	pAuraDB = new CAuraDB;
	pSpellDB = new CSpellDB;
	pCreatureDB = new CCreatureDB;

	if( pLuaState = lua_open())	{
		luaL_openlibs(pLuaState);
		LUA_REGISTER(PlayerIdleRoutine);
		LUA_REGISTER(PreCastingSpellRoutine);
		LUA_REGISTER(PostCastingSpellRoutine);
		LUA_REGISTER(PreSpellChannelRoutine);
		LUA_REGISTER(PostSpellChannelRoutine);
		LUA_REGISTER(AuraApplicationRoutine);
		LUA_REGISTER(AuraFadeRoutine);
		LUA_REGISTER(PrePeriodicEventRoutine);
		LUA_REGISTER(PostPeriodicEventRoutine);
		LUA_REGISTER(AuraAdd);
		LUA_REGISTER(AuraNameToID);
		LUA_REGISTER(AuraIDToName);
		LUA_REGISTER(AuraEnableCrit);
		LUA_REGISTER(AuraEnableHaste);
		LUA_REGISTER(AuraDisableCrit);
		LUA_REGISTER(AuraDisableHaste);
		LUA_REGISTER(AuraGetSchool);
		LUA_REGISTER(AuraSetSPPrecentagePreTick);

		LUA_REGISTER(EventAddStats);
		LUA_REGISTER(EventGetStats);

		LUA_REGISTER(SpellAdd);
		LUA_REGISTER(SpellNameToID);
		LUA_REGISTER(SpellIDToName);
		LUA_REGISTER(SpellGetCooldown);
		LUA_REGISTER(SpellGetSchool);
		LUA_REGISTER(SpellSetSPPrecentage);

		LUA_REGISTER(PlayerRatingToPrecentage);
		LUA_REGISTER(PlayerPrecentageToRating);
		LUA_REGISTER(PlayerGetSpellCastTime);
		LUA_REGISTER(PlayerGetSpellCooldown);
		LUA_REGISTER(PlayerSpellIsAvailable);
		LUA_REGISTER(PlayerIsIdle);
		LUA_REGISTER(PlayerGetStats);
		LUA_REGISTER(PlayerSetStats);
		LUA_REGISTER(PlayerAddStatsMultiplier);
		LUA_REGISTER(PlayerAddStats);
		LUA_REGISTER(PlayerGetPrecentage);
		LUA_REGISTER(PlayerSleep);
		LUA_REGISTER(PlayerSetCritBonus);
		LUA_REGISTER(StopCasting);


		LUA_REGISTER(CastSpellByName);
		LUA_REGISTER(UnitApplyAura);
		LUA_REGISTER(UnitAuraApplied);
		LUA_REGISTER(TargetCreate);
		LUA_REGISTER(TargetDestory);
		LUA_REGISTER(GetGameTime);

		LUA_REGISTER(FinalizeCalculation);

		LUA_REGISTER(OnTestStartRoutine);
		LUA_REGISTER(OnTestFinishRoutine);

		// Run Preload lua code
		RunLuaCode(PreloadAdditionalLua);

		// Load lua scripts
		return LoadTOCFile("main.toc");
	}
	return false;
}

bool Lua_CleanUp()
{
	if (pLuaState)
		lua_close(pLuaState);
	if (pAuraDB) {
		delete pAuraDB;
		pAuraDB = NULL;
	}
	if (pSpellDB) {
		delete pSpellDB;
		pSpellDB = NULL;
	}
	if (pCreatureDB)  {
		delete pCreatureDB;
		pCreatureDB = NULL;
	}
	return true;
}

bool RunLuaCode(const char* strCode)
{
	if(!pLuaState) return false;
	
	if (luaL_loadstring(pLuaState,strCode))
	{
		if (!lua_isnil(pLuaState, -1)) {
			const char *msg = lua_tostring(pLuaState, -1);
			if (msg == NULL) msg = "(error object is not a string)";
			fprintf(stderr,"%s\n",msg);
			lua_pop(pLuaState, 1);
		}
		return false;
	}
	else if ( docall(pLuaState, 0) ) return false;

	return true;
}