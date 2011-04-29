
local SpiritAddDamage

local _AuraApplicationRoutine = AuraApplicationRoutine
local _AuraFadeRoutine = AuraFadeRoutine
local _PostCastingSpellRoutine = PostCastingSpellRoutine
local _PreCastingSpellRoutine = PreCastingSpellRoutine
local _PreSpellChannelRoutine = PreSpellChannelRoutine
local _PostSpellChannelRoutine = PostSpellChannelRoutine
local _PrePeriodicEventRoutine = PrePeriodicEventRoutine
local _PostPeriodicEventRoutine = PostPeriodicEventRoutine


local idShadowGlyph


function AuraApplicationRoutine(auraID)
	_AuraApplicationRoutine(auraID)
	if auraID == idShadowGlyph then
		if SpiritAddDamage > 0 then
			PlayerAddStats(PS_SPELL_POWER, (-1) * SpiritAddDamage)
			SpiritAddDamage = 0
		end
		SpiritAddDamage = PlayerGetStats(PS_SPIRIT) * 0.3
		PlayerAddStats(PS_SPELL_POWER, SpiritAddDamage)
	end
end

function AuraFadeRoutine(auraID)
	_AuraFadeRoutine(auraID)
	if auraID == idShadowGlyph then
		if SpiritAddDamage > 0 then
			PlayerAddStats(PS_SPELL_POWER, (-1) * SpiritAddDamage)
			SpiritAddDamage = 0
		end
	end
end


function PostCastingSpellRoutine(spellID)
	_PostCastingSpellRoutine(spellID)
	if EventGetStats(CS_CRIT_PRECENTAGE) > 0 then
		if (not UnitAuraApplied("target",idShadowGlyph) ) then
			UnitApplyAura("target",idShadowGlyph,1,10000,false)
		else
			UnitApplyAura("target",idShadowGlyph,0,10000,false)
		end
	end
end

function PreSpellChannelRoutine(spellID)
	_PreSpellChannelRoutine(spellID)
	EventAddStats(CS_DAMAGE_PRECENTAGE,0.1,8)
end

function PostSpellChannelRoutine(spellID)
	_PostSpellChannelRoutine(spellID)
	if EventGetStats(CS_CRIT_PRECENTAGE) > 0 then
		if (not UnitAuraApplied("target",idShadowGlyph) ) then
			UnitApplyAura("target",idShadowGlyph,1,10000,false)
		else
			UnitApplyAura("target",idShadowGlyph,0,10000,false)
		end
	end
end


idShadowGlyph = AuraAdd("Shadow Insight")
SpiritAddDamage = 0
