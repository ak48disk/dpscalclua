#pragma once
#include "Global.h"
#include "EventSystem.h"
#include "Player.h"
#include "Aura.h"
#include "Spell.h"

class CTestSystem
{
public:
	CTestSystem(void);
	~CTestSystem(void);
private:
	CEventSystem theEventSystem;
	CPlayer thePlayer;
	CSpellDB theSpellDB;
	CAuraDB theAuraDB;
	lua_State* L;
};

