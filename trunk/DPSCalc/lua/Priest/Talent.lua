
local idPI
local idInnerFire
local idShadowWeave
local PlegueDamage
local idDot1
local idDot2
local idDot3
local idMF
local PlagueAdditionalDamage
local _AuraApplicationRoutine = AuraApplicationRoutine
local _AuraFadeRoutine = AuraFadeRoutine
local _PostCastingSpellRoutine = PostCastingSpellRoutine
local _PreCastingSpellRoutine = PreCastingSpellRoutine
local _PreSpellChannelRoutine = PreSpellChannelRoutine
local _PostSpellChannelRoutine = PostSpellChannelRoutine
local _PrePeriodicEventRoutine = PrePeriodicEventRoutine
local _PostPeriodicEventRoutine = PostPeriodicEventRoutine
local _PlayerSetStats = PlayerSetStats
local _PlayerAddStats = PlayerAddStats


function PlayerSetStats(stats,value)
	if stats == PS_SPIRIT then
		PlayerAddStats(PS_SPELL_POWER, (-1) * PlayerGetStats(PS_SPIRIT) * 0.2 )
		PlayerAddStats(PS_SPELL_POWER, value * 0.2 )
	end
	return _PlayerSetStats(stats,value)
end

function PlayerAddStats(stats,value)
	if stats == PS_SPIRIT then
		PlayerAddStats(PS_SPELL_POWER,value * 0.2 )	
	end
	return _PlayerAddStats(stats,value)
	
end


function AuraApplicationRoutine(auraID)
	_AuraApplicationRoutine(auraID)
	if auraID == idPI then
		PlayerAddStats(PS_HASTE_RAITING,PrecentageToRating(PS_HASTE_RAITING,0.2))
	elseif auraID == idInnerFire then
		PlayerAddStats(PS_SPELL_POWER,54)
	elseif auraID == idDot3 then
		if PlagueAdditionalDamage == 0 then
			PlagueAdditionalDamage = EventGetStats(CS_DAMAGE) * 8 * 0.3
		end
	end
end

function AuraFadeRoutine(auraID)
	_AuraFadeRoutine(auraID)
	if auraID == idPI then
		PlayerAddStats(PS_HASTE_RAITING,(-1) * PrecentageToRating(PS_HASTE_RAITING,0.2))
	elseif auraID == idInnerFire then
		PlayerAddStats(PS_SPELL_POWER,-54)
	end
end

function PreCastingSpellRoutine(spellID)
	_PreCastingSpellRoutine(spellID)
local AuraStacks
--Discipline

--Twin
	if( PlayerGetSpellCastTime(spellID) == 0) then
		EventAddStats(CS_DAMAGE_PRECENTAGE,0.1)
	end
--Shadow
--Darkness
	if (SpellGetSchool(spellID) == SPELL_SCHOOL_SHADOW) then
		EventAddStats(CS_DAMAGE_PRECENTAGE,0.1)
		EventAddStats(CS_HIT_PRECENTAGE,0.03)
		AuraStacks = UnitAuraApplied("player",idShadowWeave)
		if (AuraStacks == nil or AuraStacks <5) then
			UnitApplyAura("player",idShadowWeave,1,15000,false)
		else
			UnitApplyAura("player",idShadowWeave,0,15000,false)
		end
		if AuraStacks then EventAddStats(CS_DAMAGE_PRECENTAGE,0.02*AuraStacks) end

		EventAddStats(CS_DAMAGE_PRECENTAGE,0.15)
		
		if (not UnitAuraApplied("target",idMisery) ) then
			UnitApplyAura("target",idMisery,1,24000,false)
		else
			UnitApplyAura("target",idMisery,0,24000,false)
		end
	end

	if (spellID == SpellNameToID("Mind Blast")) then
		EventAddStats(CS_COOLDOWN,-2500)
		EventAddStats(CS_CRIT_PRECENTAGE,0.02)
		EventAddStats(CS_DAMAGE,PlayerGetStats(PS_SPELL_POWER)*0.15)
		if UnitAuraApplied("target",AuraNameToID("Shadow Word: Pain")) then
			EventAddStats(CS_DAMAGE_PRECENTAGE,0.1)
		end
	end

	if(UnitAuraApplied("target",idMisery)) then
		EventAddStats(CS_HIT_PRECENTAGE,0.03)
	end

end

function PostCastingSpellRoutine(spellID)
	_PostCastingSpellRoutine(spellID)
	if spellID == SpellNameToID("Devoring Plague") then
		EventAddStats(CS_DAMAGE,PlagueAdditionalDamage)
	end
end

function PreSpellChannelRoutine(spellID)
	_PreSpellChannelRoutine(spellID)
	if (spellID == idMF) then
		EventAddStats(CS_CRIT_PRECENTAGE,0.03)
		EventAddStats(CS_DAMAGE_PRECENTAGE,0.15)
		EventAddStats(CS_DAMAGE_PRECENTAGE,0.1)
		EventAddStats(CS_HIT_PRECENTAGE,0.03)
		EventAddStats(CS_DAMAGE,PlayerGetStats(PS_SPELL_POWER)*0.15/3)
		--EventAddStats(CS_CRIT_PRECENTAGE,0.05) --4T9
	end
	if(UnitAuraApplied("target",idMisery)) then
		EventAddStats(CS_HIT_PRECENTAGE,0.03)
	end
	if UnitAuraApplied("target",AuraNameToID("Shadow Word: Pain")) then
		EventAddStats(CS_DAMAGE_PRECENTAGE,0.1)
		UnitApplyAura("target",AuraNameToID("Shadow Word: Pain"),0,18000,false)
	end
end

function PostSpellChannelRoutine(spellID)
	_PostSpellChannelRoutine(spellID)
end

function PrePeriodicEventRoutine(auraID)
	_PrePeriodicEventRoutine(auraID)
	if (auraID == idDot2 or auraID == idDot3) then
		EventAddStats(CS_DAMAGE_PRECENTAGE,0.1) --Twins
	end
	if auraID == idDot2 then
		EventAddStats(CS_DAMAGE_PRECENTAGE,0.06) 
	end

	
	if (AuraGetSchool(auraID) == SPELL_SCHOOL_SHADOW) then
		AuraStacks = UnitAuraApplied("player",idShadowWeave)
		if AuraStacks then EventAddStats(CS_DAMAGE_PRECENTAGE,0.02*AuraStacks) end
	
		EventAddStats(CS_CRIT_PRECENTAGE,0.03)
		EventAddStats(CS_DAMAGE_PRECENTAGE,0.1) --Darkness
		EventAddStats(CS_DAMAGE_PRECENTAGE,0.15)
	end
	if auraID == idDot3 then
		EventAddStats(CS_DAMAGE_PRECENTAGE,0.15)
	end
		
end

function PostPeriodicEventRoutine(auraID)
	_PostPeriodicEventRoutine(auraID)
	if auraID == idDot3 then
		PlagueAdditionalDamage = EventGetStats(CS_DAMAGE) * 8 * 0.3
	end
end

-- Discipline
idPI = AuraAdd("Power Infusion")
idInnerFire = AuraNameToID("Inner Fire")


-- Holy

-- Shadow
idShadowWeave = AuraAdd("Shadow Weave")
idMisery = AuraAdd("Misery")
idMF = SpellNameToID("Mind Flay")
idDot1 = AuraNameToID("Vampiric Touch" )
idDot2 = AuraNameToID("Shadow Word: Pain" )
idDot3 = AuraNameToID("Devoring Plague" )
AuraEnableCrit( AuraNameToID("Vampiric Touch" )) 
AuraEnableCrit( AuraNameToID("Shadow Word: Pain" )) 
AuraEnableCrit( AuraNameToID("Devoring Plague" ))

---[[
--print("Enable Haste? 1 to enable")
--local EnableHaste = io.read()
--if EnableHaste and EnableHaste:find("1") then
	AuraEnableHaste( AuraNameToID("Vampiric Touch" ))
	AuraEnableHaste( AuraNameToID("Devoring Plague" ))
--end
--]]

PlagueAdditionalDamage = 0