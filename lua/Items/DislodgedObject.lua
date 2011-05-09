DislodgedObject = DislodgedObject or {}

DislodgedObject.Stacks = 0
DislodgedObject.ProcChance = 0.1
DislodgedObject.LastTickTime = nil
DislodgedObject.Aura = nil

function DislodgedObject:Proc()
	if math.random() < DislodgedObject.ProcChance  then
		local NowTime = GetGameTime()
		if (not self.LastTickTime) or ( (NowTime - self.LastTickTime) > 45000 ) then
			self.LastTickTime = NowTime
			UnitApplyAura("player",DislodgedObject.Aura,1,2000,false)
			PlayerAddStats ( PS_SPELL_POWER , 105 )
		end
	end
end

function DislodgedObject:Enable()
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
		if auraID == DislodgedObject.Aura then
			DislodgedObject.Stacks = DislodgedObject.Stacks + 1
			if DislodgedObject.Stacks < 10 then 
				UnitApplyAura("player",DislodgedObject.Aura,1,2000,false)
				PlayerAddStats ( PS_SPELL_POWER , 105 )

			else
				PlayerAddStats ( PS_SPELL_POWER , -1050 )
				DislodgedObject.Stacks = 0

			end
		end
	end
	
	DislodgedObject.LastTickTime = nil
	DislodgedObject.Aura = AuraAdd("DFO Funny")
end

function DislodgedObject:IsReady()
	return false
end

function DislodgedObject:Use()
end