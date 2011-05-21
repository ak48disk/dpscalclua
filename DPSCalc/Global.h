#pragma once

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef int int32;
typedef short int16;
typedef __int64 int64;
typedef unsigned __int64 uint64;

#define _CRT_SECURE_NO_WARNINGS

//enum e_EventType {
typedef uint32 e_EventType;
const uint32	EVENT_SPELL					= 0x10000000;
const uint32	EVENT_SPELL_CAST			= 0x10000001;
const uint32	EVENT_SPELL_CHANNEL			= 0x10000004;
const uint32	EVENT_AURA					= 0x20000000;
const uint32	EVENT_AURA_APPLICATION		= 0x20000001;
const uint32	EVENT_AURA_REMOVAL			= 0x20000002;
const uint32	EVENT_AURA_PERIODIC_EVENT	= 0x20000004;
const uint32	EVENT_PRE					= 0x40000000;
const uint32	EVENT_POST					= 0x80000000;
//	EVENT_SPELL_CHANNEL_SUCCESS	= 0x10000008,
//	EVENT_SPELL_CHANNEL_MISS	= 0x10000010,
//	EVENT_SPELL_CAST_MISS		= 0x10000002,
//	EVENT_PERIODIC_DAMAGE		= 0x40000001
//};

//enum e_SpellType {
const uint32	SPELL_DAMAGE				= 0x00000001;
const uint32	SPELL_AURA_APPLICATION		= 0x00000002;
const uint32	SPELL_CHANNEL				= 0x00000004;
//};

/* These spell schools will not be in enum because
	it will have compond ones
*/

const uint32 SPELL_SCHOOL_PHYSICAL	=	0x01;
const uint32 SPELL_SCHOOL_HOLY		=	0x02;
const uint32 SPELL_SCHOOL_FIRE		=	0x04;
const uint32 SPELL_SCHOOL_NATURE	=	0x08;
const uint32 SPELL_SCHOOL_FROST		=	0x10;
const uint32 SPELL_SCHOOL_SHADOW	=	0x20;
const uint32 SPELL_SCHOOL_ARCANE	=	0x40;

enum eL_PlayerStats {
	PS_INVAILD_MIN		=	0,
	PS_LEVEL			=	1,
	PS_CRIT_RAITING		=	2,
	PS_HIT_RAITING		=	3,
	PS_HASTE_RAITING	=	4,
	PS_SPELL_POWER		=	5,
	PS_INTELLIGENCE		=	6,
	PS_SPIRIT			=	7,
	PS_HASTE_PRECENTAGE	=	8,
	PS_INVAILD_MAX		=	9
};

enum eL_PlayerPrecentage {
	PP_INVAILD_MIN		=	1,
	PP_CRIT_PRECENTAGE	=	2,
	PP_HIT_PRECENTAGE	=	3,
	PP_HASTE_PRECENTAGE	=	4,
	PP_INVAILD_MAX		=	5
};

enum eL_CombatStsts  {
	CS_INVAILD_MIN		=	1,
	CS_DAMAGE			=	2,
	CS_DAMAGE_PRECENTAGE=	3,
	CS_CRIT_PRECENTAGE	=	4,
	CS_HIT_PRECENTAGE	=	5,
	CS_CRIT_RATING		=	6,
	CS_HIT_RATING		=	7,
	CS_HASTE_RATING		=	8,
	CS_CAST_TIME		=	9,
	CS_COOLDOWN			=	10,
	CS_HASTE_PRECENTAGE =	11,
	CS_INVAILD_MAX		=	12
};


#define CLASS_ELEMENT_UINT32(x) uint32 Get_##x##() const {return (m_i##x##+m_iAdditional##x##)*(1.0+m_d##x##Multiplier);} \
	void Set_##x##(const uint32 i##x##) { m_i##x## = i##x##; } \
	void Add##x##(const int32 x ) { m_iAdditional##x## += ##x##; } \
	void Add##x##Multiplier(const double x) { m_d##x##Multiplier += ##x##; }


#define CLASS_ELEMENT(type,x)	type Get_##x##() const {return m_##x##;} \
	void Set_##x##(const type x ) { m_##x## = ##x##; } 

#define EVENT_ADDITIONAL_ELEMENT(type,x)	type Get_Additional##x##() const {return m_iAdditional##x##;} \
	void Add##x##(const type x ) { m_iAdditional##x## += ##x##; } 


#define EVENT_ADDITIONAL_ELEMENT_X(type,x)	type Get_Additional##x##() const {return m_iAdditional##x##;} 

#define CLASS_ELEMENT_CONST(type,x)	type Get_##x##() const {return m_##x##;} 

#define MAX_STRING_LENGTH 256
