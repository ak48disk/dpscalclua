


FrostBadgeTrinket = EffectWithCooldownProto:Create( function() PlayerAddStats(PS_SPELL_POWER,716) end,
	function() PlayerAddStats(PS_SPELL_POWER,-716) end,
	"FrostBadgeTrinket",true,120000,20000,nil,nil,nil
)

VolatilePower = EffectWithCooldownProto:Create( function() VolatilePower.used = true end,
	function() VolatilePower.used = false end,
	"VolatilePower",true,120000,20000,nil,nil,nil )

VolatilePowerHelper = EffectStackableProto:Create( function() PlayerAddStats(PS_HASTE_RATING,57) end ,
	function() PlayerAddStats(PS_HASTE_RATING,-57) end,
	"VolatilePowerHelper",0,20000,8,1,1,nil,function() return VolatilePower.used end)

EffectManager:Attach(VolatilePowerHelper)

VolatilePowerH = EffectWithCooldownProto:Create( function() VolatilePowerH.used = true end,
	function() VolatilePowerH.used = false end,
	"VolatilePowerH",true,120000,20000,nil,nil,nil )

VolatilePowerHelperH = EffectStackableProto:Create( function() PlayerAddStats(PS_HASTE_RATING,57) end ,
	function() PlayerAddStats(PS_HASTE_RATING,-57) end,
	"VolatilePowerHelperH",0,20000,8,1,1,nil,function() return VolatilePowerH.used end)

EffectManager:Attach(VolatilePowerHelperH)

