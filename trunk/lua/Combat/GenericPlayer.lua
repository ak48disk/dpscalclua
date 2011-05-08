local _PlayerIdleRoutine = PlayerIdleRoutine

--[[
Varibles that defined by PreScriptFunc
Includes:
PlayerMethod
BossFight
etc

]]
function PlayerIdleRoutine()
	_PlayerIdleRoutine()
	
	if not BossFight.init then BossFight:OnInit() end
	
	if CalculationTime and GetGameTime() >= CalculationTime then FinalizeCalculation() return end
	
	BossFight:OnEvent()
	
	if (not LatencyApplied) and Latency and Latency > 0 then
		PlayerSleep(Latency)
		LatencyApplied = true
		return
	else
		LatencyApplied = false
	end
	
	for k,v in PlayerMethod:GetIterateFunc() do
		if not TargetHelper:Iterate(v) then return false end
	end
	
	return true
end

