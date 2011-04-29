--[[
Hyperspeed Accelerators
Permanently attaches hyperspeed accelerators to a pair of gloves, 
allowing a skilled engineer to increase their haste rating by 240 for 12 sec. 
 The gloves can only be activated every minute. 
 ]]--
 
 ---[[
 HyperSpeedAcc = EffectWithCooldownProto:Create( function() PlayerAddStats(PS_HASTE_RATING,240) end,
	function() PlayerAddStats(PS_HASTE_RATING,-240) end,
	"HyperSpeedAcc",true,60000,12000,nil,nil,nil
)
 --]]
 
 --[[
HyperSpeedAcc = HyperSpeedAcc or {}

HyperSpeedAcc.HA_idItem = AuraAdd("Hyperspeed Accelerators")
HyperSpeedAcc.LastCast = nil 
  
function HyperSpeedAcc:Enable()
	local _AuraApplicationRoutine = AuraApplicationRoutine
	local _AuraFadeRoutine = AuraFadeRoutine
	local HA_ItemApplied = nil
	
	function AuraApplicationRoutine(auraID)
		_AuraApplicationRoutine(auraID)
		if auraID == self.HA_idItem and not HA_ItemApplied then
			PlayerAddStats(PS_HASTE_RATING,240)
			HA_ItemApplied = 1
		end
	end

	function AuraFadeRoutine(auraID)
		_AuraFadeRoutine(auraID)
		if auraID == self.HA_idItem and HA_ItemApplied then
			PlayerAddStats(PS_HASTE_RATING,-240)
			HA_ItemApplied = nil
		end
	end
	
end

function HyperSpeedAcc:IsReady()
	local NowTime = GetGameTime()
	return (not self.LastCast) or (NowTime - self.LastCast > 60000)
end

function HyperSpeedAcc:Use()
	if self:IsReady() then
		self.LastCast = GetGameTime()
		UnitApplyAura("player",self.HA_idItem,1,12000,false)
	end
end
--]]