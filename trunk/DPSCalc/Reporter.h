#include "Global.h"
#include <vector>

class CReporter {
public:
	CReporter(){}
	virtual ~CReporter(){}
	virtual void ReportDamage(const char * strSpellName,uint32 iDamage,uint32 iGameTime,bool isCrit)=0;
};

class CDefaultReporter 
	: public CReporter
{
public:
	CDefaultReporter(bool isSave);
	virtual ~CDefaultReporter();
	virtual void ReportDamage(const char * strSpellName,uint32 iDamage,uint32 iGameTime,bool isCrit);
private:
	typedef struct _DamageInfo  {
		uint32 iGameTime;
		uint32 iDamage;
		bool isCrit;
		char strSpellName[MAX_STRING_LENGTH];
	}DamageInfo;
	typedef std::vector<DamageInfo*> DamageVector;
private:
	bool _isSave;
	DamageVector _v;

};

typedef struct _DamageInfoVB
{
    uint32 Index;
    uint32 Damage;
    uint32 Count;
    uint32 CritCount;
    char lpSpellName[MAX_STRING_LENGTH];
}DamageInfoVB;

typedef struct __DamageInfoVB
{
    uint32 Index;
    uint32 Damage;
    uint32 Count;
    uint32 CritCount;
    char* lpSpellName;
}___DamageInfoVB;

extern DamageInfoVB* pDamageInfo[100];