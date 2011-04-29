--[[
Phylactery of the Nameless Lich
Equip: Each time one of your spells deals periodic damage, you have a chance to gain 1074 spell power for 20 sec.
H:1207
chance 30% icd: 90s
]]


Phylactery = EffectWithCooldownProto:Create( function() PlayerAddStats(PS_SPELL_POWER,1074) end,
	function() PlayerAddStats(PS_SPELL_POWER,-1074) end,
	"Phylactery",false,90000,20000,nil,nil,0.3
)

PhylacteryH = EffectWithCooldownProto:Create( function() PlayerAddStats(PS_SPELL_POWER,1207) end,
	function() PlayerAddStats(PS_SPELL_POWER,-1207) end,
	"PhylacteryH",false,90000,20000,nil,nil,0.3
)
