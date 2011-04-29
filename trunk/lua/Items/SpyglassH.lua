--[[
Muradin's Spyglass (Heroic)
Binds when picked up
Unique
Trinket
Requires Level 80
Item Level 264

Equip: Improves critical strike rating by 152.
Equip: Each time you deal spell damage to an opponent, you gain 20 spell power for the next 10 sec, stacking up to 10 times.
--]]


SpyglassH = SpyglassH or {}

SpyglassH.Stacks = 0

function SpyglassH:IncreaseStack()
	PlayerAddStats ( PS_SPELL_POWER , (-1) * self.Stacks * 20 )
	if self.Stacks < 10 then self.Stacks = self.Stacks + 1 end
	PlayerAddStats ( PS_SPELL_POWER , self.Stacks * 20 )
end

function SpyglassH:Enable()
	local _PostCastingSpellRoutine = PostCastingSpellRoutine
	local _PostPeriodicEventRoutine = PostPeriodicEventRoutine
	local _PostSpellChannelRoutine = PostSpellChannelRoutine
	
	function PostCastingSpellRoutine(spellID)
		_PostCastingSpellRoutine(spellID)
		self:IncreaseStack()
	end
	
	function PostPeriodicEventRoutine(auraID)
		_PostPeriodicEventRoutine(auraID)
		self:IncreaseStack()
	end
	
	function PostSpellChannelRoutine(spellID)
		_PostSpellChannelRoutine(spellID)
		self:IncreaseStack()	
	end
end

function SpyglassH:IsReady()
	return false
end

function SpyglassH:Use()
end