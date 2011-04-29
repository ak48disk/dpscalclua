PartialResistance = PartialResistance or {}

function PartialResistance:Enable()
	local PartialResistanceRate = -0.06
	local _PreCastingSpellRoutine = PreCastingSpellRoutine
	local _PrePeriodicEventRoutine = PrePeriodicEventRoutine
	local _PreSpellChannelRoutine = PreSpellChannelRoutine
	
	function PreCastingSpellRoutine(spellID)
		_PreCastingSpellRoutine(spellID)
		EventAddStats(CS_DAMAGE_PRECENTAGE,PartialResistanceRate,15)
	end
	
	function PrePeriodicEventRoutine(spellID)
		_PrePeriodicEventRoutine(spellID)
		EventAddStats(CS_DAMAGE_PRECENTAGE,PartialResistanceRate,15)
	end
	
	function PreSpellChannelRoutine(spellID)
		_PreSpellChannelRoutine(spellID)
		EventAddStats(CS_DAMAGE_PRECENTAGE,PartialResistanceRate,15)
	end
end

function PartialResistance:IsReady()
	return false
end

function PartialResistance:Use()
end