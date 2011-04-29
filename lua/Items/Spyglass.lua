--[[
Muradin's Spyglass 
Binds when picked up
Unique
Trinket
Requires Level 80
Item Level 251

Equip: Improves critical strike rating by 152.
Equip: Each time you deal spell damage to an opponent, you gain 18 spell power for the next 10 sec, stacking up to 10 times.
--]]


Spyglass = EffectStackableProto:Create( function() PlayerAddStats(PS_SPELL_POWER,18) end , function() PlayerAddStats(PS_SPELL_POWER,-18) end,
	"Spyglass",0,10000,10,1,1,1)

--[[
Spyglass = Spyglass or {}

Spyglass.Stacks = 0

function Spyglass:IncreaseStack()
	PlayerAddStats ( PS_SPELL_POWER , (-1) * self.Stacks * 18 )
	if self.Stacks < 10 then self.Stacks = self.Stacks + 1 end
	PlayerAddStats ( PS_SPELL_POWER , self.Stacks * 18 )
end

function Spyglass:Enable()
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

function Spyglass:IsReady()
	return false
end

function Spyglass:Use()
end
--]]