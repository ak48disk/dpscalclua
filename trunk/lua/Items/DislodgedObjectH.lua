DislodgedObjectH = DislodgedObjectH or {}

DislodgedObjectH.Stacks = 0
DislodgedObjectH.ProcChance = 0.1
DislodgedObjectH.LastTickTime = nil
DislodgedObjectH.Aura = nil

function DislodgedObjectH:Proc()
	if math.random() < DislodgedObjectH.ProcChance  then
		local NowTime = GetGameTime()
		if (not self.LastTickTime) or ( (NowTime - self.LastTickTime) > 45000 ) then
			self.LastTickTime = NowTime
			UnitApplyAura("player",DislodgedObjectH.Aura,1,2000,false)
			PlayerAddStats ( PS_SPELL_POWER , 121 )
		end
	end
end

function DislodgedObjectH:Enable()
	local _PostCastingSpellRoutine = PostCastingSpellRoutine
	local _PostSpellChannelRoutine = PostSpellChannelRoutine
	local _AuraFadeRoutine = AuraFadeRoutine
	
	function PostCastingSpellRoutine(spellID)
		_PostCastingSpellRoutine(spellID)
		self:Proc()
	end
	
	function PostSpellChannelRoutine(spellID)
		_PostSpellChannelRoutine(spellID)
		self:Proc()
	end
		
	function AuraFadeRoutine(auraID)
		_AuraFadeRoutine(auraID)
		if auraID == DislodgedObjectH.Aura then
			DislodgedObjectH.Stacks = DislodgedObjectH.Stacks + 1
			if DislodgedObjectH.Stacks < 10 then 
				UnitApplyAura("player",DislodgedObjectH.Aura,1,2000,false)
				PlayerAddStats ( PS_SPELL_POWER , 121 )

			else
				PlayerAddStats ( PS_SPELL_POWER , -1210 )
				DislodgedObjectH.Stacks = 0

			end
		end
	end
	
	DislodgedObjectH.LastTickTime = nil
	DislodgedObjectH.Aura = AuraAdd("DFOH Funny")
end

function DislodgedObjectH:IsReady()
	return false
end

function DislodgedObjectH:Use()
end