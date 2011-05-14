#include "stdafx.h"
#include "Reporter.h"

CDefaultReporter::CDefaultReporter(bool isSave)
{
	for(int i=0;i<100;++i)
	{
		if (pDamageInfo[i])
		{
			delete pDamageInfo[i];
			pDamageInfo[i] = NULL;
		}
	}
	_isSave = isSave;
}

void CDefaultReporter::ReportDamage(const char * strSpellName,uint32 iDamage,uint32 iGameTime,bool isCrit)
{
	DamageInfo* _d = new DamageInfo;
	strcpy(_d->strSpellName,strSpellName);
	_d->iDamage = iDamage;
	_d->iGameTime = iGameTime;
	_d->isCrit = isCrit;
	_v.push_back(_d);
}

CDefaultReporter::~CDefaultReporter()
{
	struct less_str
	{	// functor for operator<
	bool operator()(const char* _Left, const char* _Right) const
		{	// apply operator< to operands
		return (strcmp(_Left , _Right)<0);
		}
	};
	typedef std::map<const char*,uint32,less_str> DamageMap;
	DamageMap _m;DamageMap _m2;DamageMap _mcrit;
	uint32 iTotalDamage = 0;

	FILE* fp; 
	FILE* fp2;

	if (_isSave) {
		fp = fopen("CombatLog.txt","a");
		fp2 = fopen("CombatLogLite.txt","a");
	}

	DamageVector::iterator it;
	for(it=_v.begin();it!=_v.end();++it)
	{
		DamageInfo* _d = *it;
		
		if (_isSave)
		{
			if (_d->isCrit)
				fprintf(fp,"[%d]%s(%d)(crit)\n",_d->iGameTime,_d->strSpellName,_d->iDamage);
			else
				fprintf(fp,"[%d]%s(%d)\n",_d->iGameTime,_d->strSpellName,_d->iDamage);
		}

		iTotalDamage += _d->iDamage;
		auto it2 = _m.find(_d->strSpellName);
		if (it2!= _m.end())
			it2->second += _d->iDamage;
		else
			_m.insert(std::make_pair(_d->strSpellName,_d->iDamage));

		it2 = _m2.find(_d->strSpellName);
		if (it2!= _m2.end())
			it2->second++;
		else
			_m2.insert(std::make_pair(_d->strSpellName,1));

		if (_d->isCrit) {
			it2 = _mcrit.find(_d->strSpellName);
			if (it2 != _mcrit.end() )
				it2->second++;
			else
				_mcrit.insert(std::make_pair(_d->strSpellName,1));
		}

	}
	
	if (_isSave)
		fprintf(fp2,"Total Damage = %d\n DPS = %f\n",iTotalDamage,(float)iTotalDamage / 5000.0f);

	int j = 0;
	for (auto it3 = _m.begin();it3 != _m.end();++it3) {
		auto it4 = _m2.find(it3->first);
		auto it5 = _mcrit.find(it3->first);

		pDamageInfo[j] = new DamageInfoVB;
		pDamageInfo[j]->Count = it4->second;
		pDamageInfo[j]->CritCount = it5->second;
		pDamageInfo[j]->Damage = it3->second;
		pDamageInfo[j]->Index = j;
		strcpy(pDamageInfo[j]->lpSpellName,it3->first);
		j++;

		if (_isSave) {
			fprintf(fp2,"%s %d(%d)[%f] [%f crit]\n",it3->first,it3->second,it4->second,
				(float)it3->second / (float)iTotalDamage,
				(float)it5->second / (float)it4->second );
			printf("%s %d(%d)[%f] [%f crit]\n",it3->first,it3->second,it4->second,
				(float)it3->second / (float)iTotalDamage,
				(float)it5->second / (float)it4->second );
		}

	}
	if (_isSave) {
		fprintf(fp,"\n\n\n");
		fprintf(fp2,"\n\n\n");
		fclose(fp);
		fclose(fp2);
	}

	for(it=_v.begin();it!=_v.end();++it)
		delete *it;
}