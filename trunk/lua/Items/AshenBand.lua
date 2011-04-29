--[[

]]

AshenBand = AshenBand or {}
AshenBand.LastCast = nil

function AshenBand:Enable()

	local _AuraApplicationRoutine = AuraApplicationRoutine
	local _AuraFadeRoutine = AuraFadeRoutine
	local _PostCastingSpellRoutine = PostCastingSpellRoutine
	
	function PostCastingSpellRoutine(spellID)
		_PostCastingSpellRoutine(spellID)
		if math.random() < 0.1 then
			local NowTime = GetGameTime()
			if (not self.LastCast) or ( (NowTime - self.LastCast) > 60000 ) then
				self.LastCast = NowTime
				UnitApplyAura("player",self.Aura,1,10000,false)
			end
		end
	end
	
	
	function AuraApplicationRoutine(auraID)
		_AuraApplicationRoutine(auraID)
		if auraID == self.Aura then
			PlayerAddStats(PS_SPELL_POWER,285)
		end
	end

	function AuraFadeRoutine(auraID)
		_AuraFadeRoutine(auraID)
		if auraID == self.Aura then
			PlayerAddStats(PS_SPELL_POWER,-285)
		end
	end
	
	AshenBand.Aura = AuraAdd("AshenBand")
end

function AshenBand:IsReady()
	return false
end

function AshenBand:Use()
end