Meta2Intel = EffectSimple:Create(function() PlayerAddStatsMultiplier(PS_INTELLIGENCE,0.02) end)

MetaChaoic = EffectSimple:Create(function() PlayerSetCritBonus( ((1.5*1.03)-1)*2 + 1 ) end )
--[[
FIXED 2011-4-30
from http://www.wowhead.com/item=41285/chaotic-skyflare-diamond#comments
 the crit boost from the metagems is applied before the boosts from talents, which tends to make the gems even better.

The largest example is with shaman, warlock, or ice mage spells fully talented. The gem boosts total damage by 3% on a crit,
 which changes crit damage from 150% to (150% * 103%) = 154.5%, which makes the crit bonus +54.5%. 
 This 54.5% number is what the talents affect, so when you add in Elemental Fury, Pandemic, Ruin, and/or Ice Shards you get a crit bonus of +109%, for a total crit damage 209%.

]]

Human3Spirit = EffectSimple:Create(function() PlayerAddStatsMultiplier(PS_SPIRIT,0.03) end)

TrollBerserking = EffectWithCooldownProto:Create( function() PlayerAddStats(PS_HASTE_PRECENTAGE,0.2) end,
	function() PlayerAddStats(PS_HASTE_PRECENTAGE,-0.2) end,
	"TrollBerserking",true,180000,10000,nil,nil,nil
)


BlackMagic = EffectWithCooldownProto:Create( function() PlayerAddStats(PS_HASTE_RATING,250) end ,
	function() PlayerAddStats(PS_HASTE_RATING,-250) end,
	"BlackMagic",false,35000,10000,0.35,0.35,nil
)
