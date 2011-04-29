Meta2Intel = EffectSimple:Create(function() PlayerAddStatsMultiplier(PS_INTELLIGENCE,0.02) end)

Meta2Chaoic = EffectSimple:Create(function() PlayerSetCritBonus(2*1.03) end )

Human3Spirit = EffectSimple:Create(function() PlayerAddStatsMultiplier(PS_SPIRIT,0.03) end)

TrollFrenzy = EffectWithCooldownProto:Create( function() PlayerAddStats(PS_HASTE_RATING,240) end,
	function() PlayerAddStats(PS_HASTE_RATING,-240) end,
	"TrollFrenzy",true,60000,12000,nil,nil,nil
)

