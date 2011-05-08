local idDot1
local idDot2
local idDot3
local idMindFlay

idDot1 = AuraNameToID("Vampiric Touch" )
idDot2 = AuraNameToID("Shadow Word: Pain" )
idDot3 = AuraNameToID("Devoring Plague" )
idMindFlay = SpellNameToID("Mind Flay")

PlayerMethod = {}
PlayerMethod.IterateFunc = {}

function PlayerMethod.IterateFunc[1](targetName,target)
	EffectManager:UseAllUseable()

end

function PlayerIdleRoutine()
		
	
	
	if not UnitAuraApplied("target1",idDot2) then
		CastSpellByName("Shadow Word: Pain","target1")
		return
	end
	if not UnitAuraApplied("target1",idDot3) then
		CastSpellByName("Improved Devoring Plague","target1")
		return 
	end
	local tk,fd = UnitAuraApplied("target1",idDot1)
	if (not tk) or (fd<1100) then
		CastSpellByName("Vampiric Touch","target1")
		return
	end
	---[[
	local MBRemain = PlayerGetSpellCooldown( SpellNameToID( "Mind Blast" ) )
	if ( (MBRemain < 200) and  (MBRemain > 0) ) then PlayerSleep(MBRemain) return end
	if PlayerSpellIsAvailable("Mind Blast") then
		CastSpellByName("Mind Blast","target1")
		return
	end
	--]]
	CastSpellByName("Mind Flay","target1")
	MFClips = 0
end




