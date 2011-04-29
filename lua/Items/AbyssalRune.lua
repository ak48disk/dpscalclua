--Abyssal Rune
--[[
Equip: Improves haste rating by 84
Equip: Each time you cast a harmful spell, you have a chance to gain 590 spell power for 10 sec.
This item has a 25% proc chance. 
This item has a hidden internal cooldown of 45s. 
--]]
 
AbyssalRune = EffectWithCooldownProto:Create( function() PlayerAddStats(PS_SPELL_POWER,590) end,
	function() PlayerAddStats(PS_SPELL_POWER,-590) end,
	"AbyssalRune",false,45000,10000,0.25
)
