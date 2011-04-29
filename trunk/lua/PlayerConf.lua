
--[[
PlayerSetStats(PS_SPELL_POWER,3028)
PlayerSetStats(PS_HIT_RATING,289)
PlayerSetStats(PS_CRIT_RATING,625) -- test only
PlayerSetStats(PS_HASTE_RATING,446)
PlayerSetStats(PS_INTELLIGENCE,1226)
PlayerSetStats(PS_SPIRIT,666)
--]]

for k,v in pairs(PriestTalent) do
	EffectManager:Attach(v)
end
EffectManager:EnableAll()

UnitApplyAura("player",AuraNameToID("Inner Fire"),1,0,false)
UnitApplyAura("player",AuraNameToID("Divine Spirit"),1,0,false)

--NeverMeltIC.equip()
--AbyssalRune.equip()
--SpyGlass.equip()

