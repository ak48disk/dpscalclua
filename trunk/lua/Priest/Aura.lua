--AuraAdd(name,isPeriodic,PeriodicDamageMin (per tick),PeriodicDamageMax,PeriodicEventTime,spellSchool)


local idDivineSpirit
local idInnerFire
local _AuraApplicationRoutine = AuraApplicationRoutine
local _AuraFadeRoutine = AuraFadeRoutine
InnerFireSpellPower = 120
AddedInnerSpellPower = 0

function AuraApplicationRoutine(auraID)
	_AuraApplicationRoutine(auraID)
	if auraID == idDivineSpirit then
		PlayerAddStats(PS_SPIRIT,80)
	elseif auraID == idInnerFire then
		PlayerAddStats(PS_SPELL_POWER,(-1) * AddedInnerSpellPower)
		AddedInnerSpellPower = InnerFireSpellPower
		PlayerAddStats(PS_SPELL_POWER,AddedInnerSpellPower)
	end
end


function AuraFadeRoutine(auraID)
	_AuraFadeRoutine(auraID)
	if auraID == idDivineSpirit then
		PlayerAddStats(PS_SPIRIT,-80)
	elseif auraID == idInnerFire then
		PlayerAddStats(PS_SPELL_POWER,(-1) * AddedInnerSpellPower)
		AddedInnerSpellPower = 0
	end
end

-- Holy
AuraAdd("Holy Fire",true,50,50,1000,SPELL_SCHOOL_HOLY)

-- Shadow
local id
id = AuraAdd("Vampiric Touch",true,170,170,3000,SPELL_SCHOOL_SHADOW)
AuraSetSPPrecentagePreTick(id,0.4)

id = AuraAdd("Shadow Word: Pain",true,230,230,3000,SPELL_SCHOOL_SHADOW)
AuraSetSPPrecentagePreTick(id,1.1/6.0)

id = AuraAdd("Devoring Plague",true,172,172,3000,SPELL_SCHOOL_SHADOW)
AuraSetSPPrecentagePreTick(id,0.185)
-- Discipline

idDivineSpirit = AuraAdd("Divine Spirit")
idInnerFire = AuraAdd("Inner Fire")
