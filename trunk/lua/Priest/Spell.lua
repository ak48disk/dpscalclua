-- This files consists the priest spells
-- And it will be loaded before talent 
-- id = SpellAdd(name,SpellType, castTime(ms),coolDown,spellSchool,iDamageMin(per tick) , iDamageMax ,auraToApply,auraFadeTime,ChannelTicks ) 


--Holy

SpellAdd("Holy Fire",SPELL_DAMAGE + SPELL_AURA_APPLICATION , 2000 , 10000 ,SPELL_SCHOOL_HOLY ,890,1130,AuraNameToID("Holy Fire"),7000 )
SpellAdd("Smite",SPELL_DAMAGE,2500,0,SPELL_SCHOOL_HOLY,707,793)

--Discipline

--Shadow
SpellAdd("Mind Blast",SPELL_DAMAGE,1500,8000,SPELL_SCHOOL_SHADOW,992,1048)
SpellAdd("Shadow Word: Death",SPELL_DAMAGE,0,12000,SPELL_SCHOOL_SHADOW,639,741)


if TwoT9Enabled then
SpellAdd("Vampiric Touch",SPELL_AURA_APPLICATION,1500,0,SPELL_SCHOOL_SHADOW,nil,nil,AuraNameToID("Vampiric Touch"),21000)
else
SpellAdd("Vampiric Touch",SPELL_AURA_APPLICATION,1500,0,SPELL_SCHOOL_SHADOW,nil,nil,AuraNameToID("Vampiric Touch"),15000)
end

SpellAdd("Shadow Word: Pain",SPELL_AURA_APPLICATION,0,0,SPELL_SCHOOL_SHADOW,nil,nil,AuraNameToID("Shadow Word: Pain"),18000)
SpellAdd("Improved Devoring Plague",SPELL_DAMAGE + SPELL_AURA_APPLICATION,0,0,SPELL_SCHOOL_SHADOW,0,0,AuraNameToID("Devoring Plague"),24000)


FourT10Enabled = 1

local idMF
if FourT10Enabled then
	idMF = SpellAdd("Mind Flay",SPELL_CHANNEL,2500,0,SPELL_SCHOOL_SHADOW,196,196,nil,nil,3)
else
	idMF = SpellAdd("Mind Flay",SPELL_CHANNEL,3000,0,SPELL_SCHOOL_SHADOW,196,196,nil,nil,3)
end

SpellSetSPPrecentage(idMF,0.771)


