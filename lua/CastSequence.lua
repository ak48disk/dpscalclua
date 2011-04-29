local _PlayerIdleRoutine = PlayerIdleRoutine
local _PostSpellChannelRoutine = PostSpellChannelRoutine
local idDot1
local idDot2
local idDot3
local MFClips
local idMindFlay

function PostSpellChannelRoutine(spellID)
	_PostSpellChannelRoutine(spellID)
	--[[
	if (spellID == idMindFlay) then
		MFClips = MFClips and MFClips + 1 or 1
		--if MFClips == 2 then StopCasting() end
		local spellUrgent = nil
		if not UnitAuraApplied("target1",idDot3) then
			spellUrgent = 1
		end
		local tk,fd = UnitAuraApplied("target1",idDot1)
		if (not tk) or (fd<1200) then spellUrgent = 1 end
		if spellUrgent and MFClips == 2 then StopCasting() end
	end
	--]]
end

function PlayerIdleRoutine()
	_PlayerIdleRoutine()

	--[[
	if HyperSpeedAcc.isready() then
		HyperSpeedAcc.use()
	end
	--]]
	--[[
	if NeverMeltIC.isready() then
		NeverMeltIC.use()
	end
	]]
	if CalculationTime and GetGameTime() >= CalculationTime then FinalizeCalculation() return end
	
	if (not LatencyApplied) and Latency and Latency > 0 then
		PlayerSleep(Latency)
		LatencyApplied = true
		return
	else
		LatencyApplied = false
	end
	
	EffectManager:UseAllUseable()
	
	if not UnitAuraApplied("target1",idDot2) then
		CastSpellByName("Shadow Word: Pain","target1")
		return
	end
	if not UnitAuraApplied("target1",idDot3) then
		CastSpellByName("Devoring Plague","target1")
		return 
	end
	local tk,fd = UnitAuraApplied("target1",idDot1)
	if (not tk) or (fd<1100) then
		CastSpellByName("Vampiric Touch","target1")
		return
	end
	---[[
	local MBRemain = PlayerGetSpellCooldown( SpellNameToID( "Mind Blast" ) )
	if ( (MBRemain < 200) and  (MBRemain > 0) ) then PlayerSleep(MBRemain) return end
	if PlayerSpellIsAvailable("Mind Blast") then
		CastSpellByName("Mind Blast","target1")
		return
	end
	--]]
	CastSpellByName("Mind Flay","target1")
	MFClips = 0
end



TargetCreate("target1")
idDot1 = AuraNameToID("Vampiric Touch" )
idDot2 = AuraNameToID("Shadow Word: Pain" )
idDot3 = AuraNameToID("Devoring Plague" )
idMindFlay = SpellNameToID("Mind Flay")
