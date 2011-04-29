
local idInnerFire
local idShadowWeave
local PlegueDamage
local idDotVT
local idDotSWP
local idDotDP
local idMF
local idMB
local idSpiritTap
local PlagueAdditionalDamage
local TwistedFaithAddSP
local _AuraApplicationRoutine = AuraApplicationRoutine
local _AuraFadeRoutine = AuraFadeRoutine
local _PostCastingSpellRoutine = PostCastingSpellRoutine
local _PreCastingSpellRoutine = PreCastingSpellRoutine
local _PreSpellChannelRoutine = PreSpellChannelRoutine
local _PostSpellChannelRoutine = PostSpellChannelRoutine
local _PrePeriodicEventRoutine = PrePeriodicEventRoutine
local _PostPeriodicEventRoutine = PostPeriodicEventRoutine
local _PlayerSetStats = PlayerSetStats
local _PlayerAddStats = PlayerAddStats
local _PlayerAddStatsMultiplier = PlayerAddStatsMultiplier
local SpiritTapApplied

if not PriestTalent then
	PriestTalent = {}
	TwistedFaithAddSP = 0
	SpiritTapApplied = false
	idMB = SpellNameToID("Mind Blast")
	idMF = SpellNameToID("Mind Flay")
	idDotVT = AuraNameToID("Vampiric Touch" )
	idDotSWP = AuraNameToID("Shadow Word: Pain" )
	idDotDP = AuraNameToID("Devoring Plague" )
	AuraEnableCrit( AuraNameToID("Vampiric Touch" )) 
	AuraEnableCrit( AuraNameToID("Shadow Word: Pain" )) 
	AuraEnableCrit( AuraNameToID("Devoring Plague" ))
	AuraEnableHaste( AuraNameToID("Vampiric Touch" ))
	AuraEnableHaste( AuraNameToID("Devoring Plague" ))
end

PriestTalent.ProtoType = PriestTalent.ProtoType or {}
function PriestTalent.ProtoType:Enable()
	self.Enabled = 1
end

function PriestTalent.ProtoType:IsReady()
	return false
end

function PriestTalent.ProtoType:Create()
	local newobj = {}
	setmetatable(newobj,self)
	self.__index = self
	return newobj
end

PriestTalent.TwinDisciplines = PriestTalent.ProtoType:Create() --5% instant index 0 imped
PriestTalent.ImprovedInnerFire = PriestTalent.ProtoType:Create() --+120*45% sp imped
PriestTalent.ImprovedSpiritTap = PriestTalent.ProtoType:Create() --+0.1 spirit after mb crit and 0.5 mf crit
PriestTalent.Darkness = PriestTalent.ProtoType:Create() --0.1 shadow damage index 0 imped
PriestTalent.ImprovedSWP = PriestTalent.ProtoType:Create() --0.06 swp damage index 0 
PriestTalent.ShadowFocus = PriestTalent.ProtoType:Create() --0.03 hit imped
PriestTalent.ImprovedMB = PriestTalent.ProtoType:Create() -- -2.5 mb cd imped
PriestTalent.ShadowWeaving = PriestTalent.ProtoType:Create() -- 0.1 shadow damage index 7 imped
PriestTalent.MindMelt = PriestTalent.ProtoType:Create() -- 4 mb mf crit 6 dot crit imped
PriestTalent.ImprovedDP = PriestTalent.ProtoType:Create() --0.15 dp dmg 0.3 dp inst
PriestTalent.Shadowform = PriestTalent.ProtoType:Create() -- 0.15 shadow dmg index 7 imped
PriestTalent.ShadowPower = PriestTalent.ProtoType:Create() -- 0.5 crit ++
PriestTalent.Misery = PriestTalent.ProtoType:Create() --mb mf sp hit add imped
PriestTalent.PainAndSuffering = PriestTalent.ProtoType:Create() --refresh mf
PriestTalent.TwistedFaith = PriestTalent.ProtoType:Create() --0.2 sp->sp 0.1 mb mf dmg when sw:p index 7 imped


function PriestTalent.ImprovedInnerFire:Enable()
	self.Enabled = 1
	InnerFireSpellPower = 174
end

function PriestTalent.ImprovedSpiritTap:Enable()
	self.Enabled = 1
	idSpiritTap = AuraAdd("Improved Spirit Tap")
end

function PriestTalent.ShadowWeaving:Enable()
	self.Enabled = 1
	idShadowWeave = AuraAdd("Shadow Weave")
end

function PriestTalent.Misery:Enable()
	self.Enabled = 1
	idMisery = AuraAdd("Misery")
	SpellSetSPPrecentage(idMF,0.771+0.15)
	SpellSetSPPrecentage(idMB,1.5/3.5+0.15)
end


function PlayerAddStatsMultiplier(stats,value)
	local retVal = _PlayerAddStatsMultiplier(stats,value)
	if PriestTalent.TwistedFaith.Enabled then
		if stats == PS_SPIRIT then
			PlayerAddStats(PS_SPELL_POWER, (-1) * TwistedFaithAddSP )
			TwistedFaithAddSP = PlayerGetStats(PS_SPIRIT) * 0.2
			PlayerAddStats(PS_SPELL_POWER, TwistedFaithAddSP )
		end
	end
	return retVal
end


function PlayerSetStats(stats,value)
	local retVal = _PlayerSetStats(stats,value)
	if PriestTalent.TwistedFaith.Enabled then
		if stats == PS_SPIRIT then
			PlayerAddStats(PS_SPELL_POWER, (-1) * TwistedFaithAddSP )
			TwistedFaithAddSP = PlayerGetStats(PS_SPIRIT) * 0.2
			PlayerAddStats(PS_SPELL_POWER, TwistedFaithAddSP )
		end
	end
	return retVal
end

function PlayerAddStats(stats,value)

	
	local retVal = _PlayerAddStats(stats,value)
	if PriestTalent.TwistedFaith.Enabled then
		if stats == PS_SPIRIT then
			PlayerAddStats(PS_SPELL_POWER, (-1) * TwistedFaithAddSP )
			TwistedFaithAddSP = PlayerGetStats(PS_SPIRIT) * 0.2
			PlayerAddStats(PS_SPELL_POWER, TwistedFaithAddSP )
		end
	end
	return retVal
end


function AuraApplicationRoutine(auraID)
	_AuraApplicationRoutine(auraID)

	if auraID == idDotDP then
		if PlagueAdditionalDamage == 0 then
			PlagueAdditionalDamage = EventGetStats(CS_DAMAGE) * 8 * 0.3
		end
	end
	
	if auraID == idSpiritTap and not SpiritTapApplied then
		PlayerAddStatsMultiplier(PS_SPIRIT,0.1)
		SpiritTapApplied = true
	end
end

function AuraFadeRoutine(auraID)
	_AuraFadeRoutine(auraID)
	if auraID == idSpiritTap and SpiritTapApplied then
		PlayerAddStatsMultiplier(PS_SPIRIT,-0.1)
		SpiritTapApplied = false
	end
end

function PreCastingSpellRoutine(spellID)
	_PreCastingSpellRoutine(spellID)
	local AuraStacks
--Discipline

--Twin
	if PriestTalent.TwinDisciplines.Enabled then
		if( PlayerGetSpellCastTime(spellID) == 0) then
			EventAddStats(CS_DAMAGE_PRECENTAGE,0.05,0)
		end
	end
	
	if (SpellGetSchool(spellID) == SPELL_SCHOOL_SHADOW) then
		if PriestTalent.Darkness.Enabled then
			EventAddStats(CS_DAMAGE_PRECENTAGE,0.1,0)
		end
		
		if PriestTalent.ShadowFocus.Enabled then
			EventAddStats(CS_HIT_PRECENTAGE,0.03)
		end
		
		if PriestTalent.ShadowWeaving.Enabled then
			AuraStacks = UnitAuraApplied("player",idShadowWeave)
			if (AuraStacks == nil or AuraStacks <5) then
				UnitApplyAura("player",idShadowWeave,1,15000,false)
			else
				UnitApplyAura("player",idShadowWeave,0,15000,false)
			end
			if AuraStacks then EventAddStats(CS_DAMAGE_PRECENTAGE,0.02*AuraStacks,8) end
		end
		
		if PriestTalent.Shadowform.Enabled then
			EventAddStats(CS_DAMAGE_PRECENTAGE,0.15,8)
		end
		
		if PriestTalent.Misery.Enabled then
			if (not UnitAuraApplied("target",idMisery) ) then
				UnitApplyAura("target",idMisery,1,24000,false)
			else
				UnitApplyAura("target",idMisery,0,24000,false)
			end
		end
		
		
	end
	
--Shadow
--Darkness


	if (spellID == SpellNameToID("Mind Blast")) then
	
		if PriestTalent.ImprovedMB.Enabled then
			EventAddStats(CS_COOLDOWN,-2500)
		end
		
		if PriestTalent.MindMelt.Enabled then
			EventAddStats(CS_CRIT_PRECENTAGE,0.04)
		end

		if PriestTalent.TwistedFaith.Enabled then
			if UnitAuraApplied("target",idDotSWP) then
				EventAddStats(CS_DAMAGE_PRECENTAGE,0.1,8)
			end
		end
		
	end

	if(idMisery and UnitAuraApplied("target",idMisery)) then
		EventAddStats(CS_HIT_PRECENTAGE,0.03)
	end

end

function PostCastingSpellRoutine(spellID)
	_PostCastingSpellRoutine(spellID)
	if spellID == SpellNameToID("Devoring Plague") then
		if PriestTalent.ImprovedDP.Enabled then
			EventAddStats(CS_DAMAGE,PlagueAdditionalDamage)
		end
	end
	if (spellID == SpellNameToID("Mind Blast")) then
		if EventGetStats(CS_CRIT_PRECENTAGE) > 0 then
			if PriestTalent.ImprovedSpiritTap.Enabled then
				if (not UnitAuraApplied("target",idSpiritTap) ) then
					UnitApplyAura("target",idSpiritTap,1,10000,false)
				else
					UnitApplyAura("target",idSpiritTap,0,10000,false)
				end
			end
		end
	end
end

function PreSpellChannelRoutine(spellID)
	_PreSpellChannelRoutine(spellID)
	
	if (spellID == idMF) then
		
		if PriestTalent.MindMelt.Enabled then
			EventAddStats(CS_CRIT_PRECENTAGE,0.04)
		end
		
		if PriestTalent.Shadowform.Enabled then
			EventAddStats(CS_DAMAGE_PRECENTAGE,0.15,8)
		end
		
		if PriestTalent.Darkness.Enabled then
			EventAddStats(CS_DAMAGE_PRECENTAGE,0.1,0)
		end
		
		if PriestTalent.ShadowFocus.Enabled then
			EventAddStats(CS_HIT_PRECENTAGE,0.03)
		end
		
		if PriestTalent.ShadowWeaving.Enabled then
			local AuraStacks = UnitAuraApplied("player",idShadowWeave)
			if AuraStacks then EventAddStats(CS_DAMAGE_PRECENTAGE,0.02*AuraStacks,8) end
		end
		
		if UnitAuraApplied("target",AuraNameToID("Shadow Word: Pain")) then
		
			if PriestTalent.PainAndSuffering.Enabled then
				UnitApplyAura("target",AuraNameToID("Shadow Word: Pain"),0,18000,false)
			end
			
			if PriestTalent.TwistedFaith.Enabled then
				EventAddStats(CS_DAMAGE_PRECENTAGE,0.1,8)
			end
			
		end
		
	end
	
	
	if(idMisery and UnitAuraApplied("target",idMisery)) then
		EventAddStats(CS_HIT_PRECENTAGE,0.03)
	end
	
	
end


function PostSpellChannelRoutine(spellID)
	_PostSpellChannelRoutine(spellID)
	if (spellID == idMF) and math.random() < 0.5 then
		if EventGetStats(CS_CRIT_PRECENTAGE) > 0 then
			if PriestTalent.ImprovedSpiritTap.Enabled then
				if (not UnitAuraApplied("target",idSpiritTap) ) then
					UnitApplyAura("target",idSpiritTap,1,10000,false)
				else
					UnitApplyAura("target",idSpiritTap,0,10000,false)
				end
			end
		end
	end
end



function PrePeriodicEventRoutine(auraID)
	_PrePeriodicEventRoutine(auraID)
	
	if (auraID == idDotSWP or auraID == idDotDP) then
		if PriestTalent.TwinDisciplines.Enabled then
			EventAddStats(CS_DAMAGE_PRECENTAGE,0.05,0)
		end
	end
	
	if auraID == idDotSWP then
		if PriestTalent.ImprovedSWP.Enabled then
			EventAddStats(CS_DAMAGE_PRECENTAGE,0.06) 
		end
	end

	if auraID == idDotDP then
		if PriestTalent.ImprovedDP.Enabled then
			EventAddStats(CS_DAMAGE_PRECENTAGE,0.15) 
		end
	end
	
	if (AuraGetSchool(auraID) == SPELL_SCHOOL_SHADOW) then
		
		if PriestTalent.ShadowWeaving.Enabled then
			local AuraStacks = UnitAuraApplied("player",idShadowWeave)
			if AuraStacks then EventAddStats(CS_DAMAGE_PRECENTAGE,0.02*AuraStacks,8) end
		end
	
		if PriestTalent.MindMelt.Enabled then
			EventAddStats(CS_CRIT_PRECENTAGE,0.06)
		end
		
		if PriestTalent.Shadowform.Enabled then
			EventAddStats(CS_DAMAGE_PRECENTAGE,0.15,8)
		end
		
		if PriestTalent.Darkness.Enabled then
			EventAddStats(CS_DAMAGE_PRECENTAGE,0.1,0)
		end
	end
	
		
end

function PostPeriodicEventRoutine(auraID)
	_PostPeriodicEventRoutine(auraID)
	if auraID == idDotDP then
		if PriestTalent.ImprovedDP.Enabled then
			PlagueAdditionalDamage = EventGetStats(CS_DAMAGE) * 8 * 0.3
		end
	end
end


--]]

PlagueAdditionalDamage = 0
