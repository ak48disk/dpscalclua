--[[
Nevermelting Ice Crystal
Trinket
Requires Level 80

Equip: Increases spell power by 111.
Use: Increases your critical strike rating by 920 for 20 sec.
 Every time one of your non-periodic spells deals a critical strike, the bonus is reduced by 184 critical strike rating. (3 Min Cooldown)
 --]]
 NeverMeltIC = {}
 
 NeverMeltIC.AdditionalCrit = 0
 
function NeverMeltIC:Enable()
	local _PostCastingSpellRoutine = PostCastingSpellRoutine
	local _PrePeriodicEventRoutine = PrePeriodicEventRoutine
	local _PostSpellChannelRoutine = PostSpellChannelRoutine
	local idPain = AuraNameToID("Shadow Word: Pain" )
	
	function PostCastingSpellRoutine(spellID)
		_PostCastingSpellRoutine(spellID)
		if (self.AdditionalCrit > 0) and EventGetStats(CS_CRIT_PRECENTAGE) > 0 then
			PlayerAddStats(PS_CRIT_RATING,(-1)*self.AdditionalCrit)
			self.AdditionalCrit = self.AdditionalCrit - 184
			PlayerAddStats(PS_CRIT_RATING,self.AdditionalCrit)
		end
	end
	
	function PrePeriodicEventRoutine(auraID)
		_PrePeriodicEventRoutine(auraID)
		if auraID == idPain then
			EventAddStats(CS_CRIT_RATING,920-self.AdditionalCrit)
		end
	end
	
	function PostSpellChannelRoutine(spellID)
		_PostSpellChannelRoutine(spellID)
		if (self.AdditionalCrit > 0) and EventGetStats(CS_CRIT_PRECENTAGE) > 0 then
			PlayerAddStats(PS_CRIT_RATING,(-1)*self.AdditionalCrit)
			self.AdditionalCrit = self.AdditionalCrit - 184
			PlayerAddStats(PS_CRIT_RATING,self.AdditionalCrit)
		end
	end
	
	
	self.equiped = 1
 end

function NeverMeltIC:IsReady()
 
	if not self.equiped then return false end
	
 	local NowTime = GetGameTime()
 	return (not self.LastCast) or (NowTime - self.LastCast > 180000)
 end
 
function NeverMeltIC:Use()
 	if self:IsReady() then
 		self.LastCast = GetGameTime()
 		if (self.AdditionalCrit > 0) then
 			PlayerAddStats(PS_CRIT_RATING,(-1)*self.AdditionalCrit)
 		end
 		self.AdditionalCrit = 920
		PlayerAddStats(PS_CRIT_RATING,self.AdditionalCrit)
 	end
 end