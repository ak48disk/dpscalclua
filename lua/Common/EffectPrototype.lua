EffectSimple = {}

function EffectSimple:Create(enableFunc)
	local newobj = {}
	newobj.enableFunc = enableFunc
	setmetatable(newobj,self)
	self.__index = self
	return newobj
end

function EffectSimple:Enable()
	if self.enableFunc then self.enableFunc(self) end
end

function EffectSimple:IsReady()
	return false
end

function EffectSimple:Use()
end

function EffectSimple:SetParam(Param)
	self.param = Param
end

EffectWithCooldownProto = {}


function EffectWithCooldownProto:Create(applyFunc,fadeFunc,name,useable,cooldown,fadeTime,procOnCastChance,procOnChannelChance,procOnPeriodicChance,conditionFunc)
	local newobj = {}
	newobj.applyFunc = applyFunc
	newobj.fadeFunc = fadeFunc
	newobj.name = name
	newobj.useable = useable
	newobj.cooldown = cooldown
	newobj.fadeTime = fadeTime
	newobj.procOnCastChance = procOnCastChance
	newobj.procOnChannelChance = procOnChannelChance
	newobj.procOnPeriodicChance = procOnPeriodicChance
	newobj.conditionFunc = conditionFunc
	setmetatable(newobj,self)
	self.__index = self
	return newobj
end

function EffectWithCooldownProto:IsReady()
	if not self.useable then return false end
	if self.applied then return false end
	return self:IsCooldown()
end

function EffectWithCooldownProto:IsCooldown()
	if (not self.LastTime) or (GetGameTime() - self.LastTime > self.cooldown) then return true else return false end
end

function EffectWithCooldownProto:Use()
	if self:IsReady() then 
		self.LastTime = GetGameTime()
		UnitApplyAura("player",self.aura,1,self.fadeTime,false) 
	end
end

function EffectWithCooldownProto:Proc(procChance)
	if self.conditionFunc and (not self.conditionFunc()) then return false end
	if self:IsCooldown() then
		if math.random() < procChance then
			self.LastTime = GetGameTime()
			UnitApplyAura("player",self.aura,1,self.fadeTime,false)
		end
	end
end

function EffectWithCooldownProto:Enable()
	local _AuraApplicationRoutine = AuraApplicationRoutine
	local _AuraFadeRoutine = AuraFadeRoutine
	local _PostCastingSpellRoutine = PostCastingSpellRoutine
	local _PostSpellChannelRoutine = PostSpellChannelRoutine
	local _PostPeriodicEventRoutine = PostPeriodicEventRoutine
	
	self.aura = AuraAdd(self.name)
	
	if self.procOnCastChance and self.procOnCastChance>0 then
		function PostCastingSpellRoutine(spellID)
			_PostCastingSpellRoutine(spellID)
			self:Proc(self.procOnCastChance)
		end
	end
	
	if self.procOnChannelChance and self.procOnChannelChance > 0 then
		function PostSpellChannelRoutine(spellID)
			_PostSpellChannelRoutine(spellID)
			self:Proc(self.procOnChannelChance)
		end
	end
	
	if self.procOnPeriodicChance and self.procOnPeriodicChance > 0 then
		function PostPeriodicEventRoutine(auraID)
			_PostPeriodicEventRoutine(auraID)
			self:Proc(self.procOnPeriodicChance)
		end
	end
	
	function AuraApplicationRoutine(auraID)
		_AuraApplicationRoutine(auraID)
		if auraID == self.aura then
			self.applyFunc(self)
			self.applied = 1
		end
	end
	
	function AuraFadeRoutine(auraID)
		_AuraFadeRoutine(auraID)
		if auraID == self.aura then
			self.fadeFunc(self)
			self.applied = nil
		end
	end
	
end

EffectStackableProto = {}

function EffectStackableProto:Create(applyFunc,fadeFunc,name,cooldown,fadeTime,maxStacks,procOnCastChance,procOnChannelChance,procOnPeriodicChance,conditionFunc)
	local newobj = {}
	newobj.applyFunc = applyFunc
	newobj.fadeFunc = fadeFunc
	newobj.name = name
	newobj.cooldown = cooldown
	newobj.fadeTime = fadeTime
	newobj.procOnCastChance = procOnCastChance
	newobj.procOnChannelChance = procOnChannelChance
	newobj.procOnPeriodicChance = procOnPeriodicChance
	newobj.maxStacks = maxStacks
	newobj.conditionFunc = conditionFunc
	setmetatable(newobj,self)
	self.__index = self
	return newobj
end

function EffectStackableProto:Use()
end

function EffectStackableProto:IsReady()
	return false
end

function EffectStackableProto:IsCooldown()
	if (not self.LastTime) or (GetGameTime() - self.LastTime > self.cooldown) then return true else return false end
end

function EffectStackableProto:Proc(procChance)
	if self.conditionFunc and  (not self.conditionFunc()) then return false end
	if self:IsCooldown() then
		if math.random() < procChance then
			self.LastTime = GetGameTime()
			if self.stacks < self.maxStacks then
				UnitApplyAura("player",self.aura,1,self.fadeTime,false)
			else
				UnitApplyAura("player",self.aura,0,self.fadeTime,false)
			end
		end
	end
end

function EffectStackableProto:Enable()
	local _AuraApplicationRoutine = AuraApplicationRoutine
	local _AuraFadeRoutine = AuraFadeRoutine
	local _PostCastingSpellRoutine = PostCastingSpellRoutine
	local _PostSpellChannelRoutine = PostSpellChannelRoutine
	local _PostPeriodicEventRoutine = PostPeriodicEventRoutine
	
	self.aura = AuraAdd(self.name)
	self.stacks = 0
	
	if self.procOnCastChance and self.procOnCastChance>0 then
		function PostCastingSpellRoutine(spellID)
			_PostCastingSpellRoutine(spellID)
			self:Proc(self.procOnCastChance)
		end
	end
	
	if self.procOnChannelChance and self.procOnChannelChance > 0 then
		function PostSpellChannelRoutine(spellID)
			_PostSpellChannelRoutine(spellID)
			self:Proc(self.procOnChannelChance)
		end
	end
	
	if self.procOnPeriodicChance and self.procOnPeriodicChance > 0 then
		function PostPeriodicEventRoutine(auraID)
			_PostPeriodicEventRoutine(auraID)
			self:Proc(self.procOnPeriodicChance)
		end
	end
	
	function AuraApplicationRoutine(auraID)
		_AuraApplicationRoutine(auraID)
		if auraID == self.aura then
			if self.stacks < self.maxStacks then
				self.applyFunc(self)
				self.stacks = self.stacks and self.stacks + 1 or 1
			end
		end
	end
	
	function AuraFadeRoutine(auraID)
		_AuraFadeRoutine(auraID)
		local i
		if auraID == self.aura then
			for i=1,self.stacks do 
				self.fadeFunc(self)
			end
			self.stacks = 0
		end
	end
end