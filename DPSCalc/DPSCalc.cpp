// DPSCalc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include <windows.h>
#include "EventSystem.h"
#include "Player.h"
#include "LuaIntergration.h"
#include "Reporter.h"

#include <iostream>
#include <ctime>

uint32 iDamageCounter = 0;
#include <Windows.h>

CReporter* pReporter;

int __stdcall Perform(char* AdditionalLua,char* PreloadAdditionalLua);
int i;
DamageInfoVB* pDamageInfo[100];
FILE* fp = NULL;

#ifdef _EXE
int main(int argc, char* argv[])
{
	Perform("","");
	return 0;
}

#else
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
			for(int i=0;i<100;++i) 
			{
				pDamageInfo[i] = NULL;
			}
			if (!fp)
				fp = freopen("lua_error.txt","a",stderr);
			break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
	//	fclose(fp);
	//	fp = NULL;
		break;
	}
	return TRUE;
}
#endif

float __stdcall Perform(char* AdditionalLua,char* PreloadAdditionalLua,int isSave,int isDetail)
{
	char Buffer[256];
	iDamageCounter = 0;
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	srand(li.LowPart);
	
	CEventSystem* theEventSystem = new CEventSystem;
	SetEventSystem(theEventSystem);

	CPlayer* Player = new CPlayer(theEventSystem);
	SetCurrentPlayer(Player);

	if (isDetail)
		pReporter = new CDefaultReporter(isSave);
	else
		pReporter = NULL;

	if (!Lua_Initialize(PreloadAdditionalLua))
		return 0;


	RunLuaCode(AdditionalLua);

	CallLuaFunction("OnTestStartRoutine",0);

	while(!theEventSystem->IsExitable() && theEventSystem->Get_iGameTime() < 2000000000)  {
		theEventSystem->ProcessEvent(Player->Get_SpellCooldown(0));
		if (Player->IsIdle())
			CallLuaFunction("PlayerIdleRoutine",0);
		
	}
	
	sprintf(Buffer,"dps=%f",iDamageCounter / 5000.0f);
	RunLuaCode(Buffer);

	CallLuaFunction("OnTestFinishRoutine",0);

	float fDPS = (float)iDamageCounter / (theEventSystem->Get_iGameTime() / 1000.0f);

	SetCurrentPlayer(NULL);
	delete Player;
	SetEventSystem(NULL);
	delete theEventSystem;

	if (pReporter)
		delete pReporter;
	pReporter = NULL;

	Lua_CleanUp();

	return fDPS;
}
void ReportDamage(const char* strSpellName,uint32 iDamage,uint32 iGameTime,bool isCrit)
{
	assert(iDamage<20000);
	if (!iDamage) return;
	iDamageCounter += iDamage;
	if (pReporter)
		pReporter->ReportDamage(strSpellName,iDamage,iGameTime,isCrit);
}

int __stdcall GetDamageInfo(___DamageInfoVB* pSpell)
{
	if (pSpell->Index >= 100 || ! pDamageInfo[pSpell->Index])
	{
		pSpell->Index = 0;
		return 0;
	}
	else
	{
		pSpell->Count = pDamageInfo[pSpell->Index]->Count;
		pSpell->CritCount = pDamageInfo[pSpell->Index]->CritCount;
		pSpell->Damage = pDamageInfo[pSpell->Index]->Damage;
		strcpy(pSpell->lpSpellName,pDamageInfo[pSpell->Index]->lpSpellName);
		pSpell->Index++;
		return 1;
	}
}