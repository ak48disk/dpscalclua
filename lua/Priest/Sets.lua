PriestSets = PriestSets or {}

PriestSets.TwoT10 = PriestSets.TwoT10 or {}

function PriestSets.TwoT10:Enable()
	local _PrePeriodicEventRoutine = PrePeriodicEventRoutine
	local idDotVT
	local idDotSWP
	local idDotDP
	
	idDotVT = AuraNameToID("Vampiric Touch" )
	idDotSWP = AuraNameToID("Shadow Word: Pain" )
	idDotDP = AuraNameToID("Devoring Plague" )

	function PrePeriodicEventRoutine(auraID)
		_PrePeriodicEventRoutine(auraID)
		if auraID == idDotVT or auraID == idDotSWP or auraID == idDotDP then
			EventAddStats(CS_CRIT_PRECENTAGE,0.05)
		end
	end
end

function PriestSets.TwoT10:IsReady()
	return false
end

function PriestSets.TwoT10:Use()
end
