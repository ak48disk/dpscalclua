PowerInfusion = EffectWithCooldownProto:Create( function() PlayerAddStats(PS_HASTE_PRECENTAGE,0.2) end ,
	function() PlayerAddStats(PS_HASTE_PRECENTAGE,-0.2)  end,
	"PowerInfusion",true,96000,15000,nil,nil,nil
)

BlessingOfKings = EffectSimple:Create(function() 
	PlayerAddStatsMultiplier(PS_INTELLIGENCE,0.1)
	PlayerAddStatsMultiplier(PS_SPIRIT,0.1)
end )

BuffAddDamage = EffectSimple:Create(function(self)
	local _PreCastingSpellRoutine = PreCastingSpellRoutine
	local _PreSpellChannelRoutine = PreSpellChannelRoutine
	local _PrePeriodicEventRoutine = PrePeriodicEventRoutine
	
	function PreCastingSpellRoutine(spellID)
		_PreCastingSpellRoutine(spellID)
		EventAddStats(CS_DAMAGE_PRECENTAGE,self.param,15)
	end
	
	function PreSpellChannelRoutine(spellID)
		_PreSpellChannelRoutine(spellID)
		EventAddStats(CS_DAMAGE_PRECENTAGE,self.param,15)
	end
	
	function PrePeriodicEventRoutine(spellID)
		_PrePeriodicEventRoutine(spellID)
		EventAddStats(CS_DAMAGE_PRECENTAGE,self.param,15)
	end
end )

BuffAddHaste = EffectSimple:Create( function(self) PlayerAddStats(PS_HASTE_PRECENTAGE,self.param) end )

BuffAddCrit = EffectSimple:Create( function(self) PlayerAddStats(PS_CRIT_RATING,PlayerPrecentageToRating(PS_CRIT_RATING,self.param)) end )

BuffAddSpellPower = EffectSimple:Create( function(self) PlayerAddStats(PS_SPELL_POWER,self.param) end )

MarkOfTheWild = EffectSimple:Create( function() PlayerAddStats(PS_INTELLIGENCE,37) PlayerAddStats(PS_SPIRIT,37) end )

ArcantIntellect = EffectSimple:Create( function() PlayerAddStats(PS_INTELLIGENCE,60) end )
--FIX IT
TrickTreat = EffectWithCooldownProto:Create( function(self) self.Enabled = 1 end ,
	function(self) self.Enabled = nil end,
	"TrickTreat",true,30000,6000,nil,nil,nil
)

Heroism = EffectWithCooldownProto:Create( function() PlayerAddStats(PS_HASTE_PRECENTAGE,0.3) end ,
	function() PlayerAddStats(PS_HASTE_PRECENTAGE,-0.3)  end,
	"Heroism",true,600000,40000,nil,nil,nil
)

function TrickTreat:SetParam(param)
	self.cooldown = 30000 / param
end

function Heroism:SetParam(param)
	self.cooldown = param * 60000
end

function PowerInfusion:SetParam(param)
	self.cooldown = 96000 / param
end


local TrickTreat = TrickTreat
local _PreCastingSpellRoutine = PreCastingSpellRoutine
local _PreSpellChannelRoutine = PreSpellChannelRoutine
local _PrePeriodicEventRoutine = PrePeriodicEventRoutine

function PreCastingSpellRoutine(spellID)
	_PreCastingSpellRoutine(spellID)
	if TrickTreat.Enabled then
		EventAddStats(CS_DAMAGE_PRECENTAGE,0.15,15)
	end
end

function PreSpellChannelRoutine(spellID)
	_PreSpellChannelRoutine(spellID)
	if TrickTreat.Enabled then
		EventAddStats(CS_DAMAGE_PRECENTAGE,0.15,15)
	end
end

function PrePeriodicEventRoutine(spellID)
	_PrePeriodicEventRoutine(spellID)
	if TrickTreat.Enabled then
		EventAddStats(CS_DAMAGE_PRECENTAGE,0.15,15)
	end
end