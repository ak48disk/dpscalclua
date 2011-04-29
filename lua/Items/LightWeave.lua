--[[
Lightweave Embroidery
4 second cast
Requires Cloth, Leather, Mail, Plate
Reagents:
[Moonshroud], [Eternium Thread]
Embroiders a subtle pattern of light into your cloak, giving you a chance to increase your spell power by 295 for 15 sec when casting a spell.

Embroidering your cloak will cause it to become soulbound. Requires 400 Tailoring to use.
]]
--function EffectWithCooldownProto:Create(applyFunc,fadeFunc,name,useable,cooldown,fadeTime,procOnCastChance,procOnChannelChance,procOnPeriodicChance)
---[[
LightWeave = EffectWithCooldownProto:Create( function() PlayerAddStats(PS_SPELL_POWER,295) end,
	function() PlayerAddStats(PS_SPELL_POWER,-295) end,
	"LightWeave",false,45000,15000,0.5
)
--]]
--[[
LightWeave = LightWeave or {}
LightWeave.LastCast = nil

function LightWeave:Enable()

	local _AuraApplicationRoutine = AuraApplicationRoutine
	local _AuraFadeRoutine = AuraFadeRoutine
	local _PostCastingSpellRoutine = PostCastingSpellRoutine
	
	function PostCastingSpellRoutine(spellID)
		_PostCastingSpellRoutine(spellID)
		if math.random() < 0.5 then
			local NowTime = GetGameTime()
			if (not self.LastCast) or ( (NowTime - self.LastCast) > 45000 ) then
				self.LastCast = NowTime
				UnitApplyAura("player",self.Aura,1,15000,false)
			end
		end
	end
	
	
	function AuraApplicationRoutine(auraID)
		_AuraApplicationRoutine(auraID)
		if auraID == self.Aura then
			PlayerAddStats(PS_SPELL_POWER,295)
		end
	end

	function AuraFadeRoutine(auraID)
		_AuraFadeRoutine(auraID)
		if auraID == self.Aura then
			PlayerAddStats(PS_SPELL_POWER,-295)
		end
	end
	
	LightWeave.Aura = AuraAdd("LightWeave")
end

function LightWeave:IsReady()
	return false
end

function LightWeave:Use()
end
--]]