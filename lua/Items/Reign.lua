--[[
Reign of the Unliving
Binds when picked up
Unique
Trinket
Requires Level 80
Item Level 245

Equip: Increases spell power by 150.(168 Heroic)

Equip: You gain a Mote of Flame each time you cause a damaging spell critical strike. 
When you reach 3 Motes, they will release, firing a Pillar of Flame for 1741 to 2023 (1959 to 2275 Heroic)damage.
 Mote of Flame cannot be gained more often than once every 2 sec.

]]

Reign = {} 
ReignH = {}
Reign.DamageMin = 1741
Reign.DamageMax = 2023
ReignH.DamageMin = 1959
ReignH.DamageMax = 2275

function Reign:IsReady()
	return false
end

function Reign:Use()
end

function Reign:Enable()
	local _PostCastingSpellRoutine = PostCastingSpellRoutine
	local _PostSpellChannelRoutine = PostSpellChannelRoutine
	
	function PostCastingSpellRoutine(spellID)
		_PostCastingSpellRoutine(spellID)
		if EventGetStats(CS_CRIT_PRECENTAGE) > 0 then
			if not self.LastTime or GetGameTime() - self.LastTime > 2000 then
				self.LastTime = GetGameTime()
				self.MoteOfFire = self.MoteOfFire and self.MoteOfFire + 1 or 1
				if self.MoteOfFire >= 3 then
					self.MoteOfFire = 0
					local dmg = math.random(self.DamageMin,self.DamageMax) / 2 * ( 1 + 0.5 * PlayerGetPrecentage(PS_CRIT_RATING) )
					EventAddStats(CS_DAMAGE,dmg)
				end
			end
		end
	end
	
	function PostSpellChannelRoutine(spellID)
		_PostSpellChannelRoutine(spellID)
		if EventGetStats(CS_CRIT_PRECENTAGE) > 0 then
			if not self.LastTime or GetGameTime() - self.LastTime > 2000 then
				self.LastTime = GetGameTime()
				self.MoteOfFire = self.MoteOfFire and self.MoteOfFire + 1 or 1
				if self.MoteOfFire >= 3 then
					self.MoteOfFire = 0
					local dmg = math.random(self.DamageMin,self.DamageMax) / 2 * ( 1 + 0.5 * PlayerGetPrecentage(PS_CRIT_RATING) )
					EventAddStats(CS_DAMAGE,dmg)
				end
			end
		end
	end
	
end


ReignH.Enable = Reign.Enable
ReignH.IsReady = Reign.IsReady
ReignH.Use = Reign.Use
