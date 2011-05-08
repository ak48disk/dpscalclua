TargetHelper = TargetHelper or {}
TargetHelper.Table = TargetHelper.Table or {}

local TargetProto = {}

function TargetProto:SetLife(deadGameTime)
	self.deadGameTime = deadGameTime
end

function TargetProto:Create(targetName)
	local newobj = {}
	setmetatable(newobj,self)
	self.__index = self
	newobj.name = targetName
	TargetCreate(targetName)
	return newobj
end

function TargetProto:Dispose()
	if self.name then TargetDestory(self.name) end
end

function TargetProto:CheckAura(auraID)
	return UnitAuraApplied(self.name,auraID)
end

function TargetHelper:NewTarget(targetName)
	if not self.Table[targetName] then 
		self.Table[targetName] = TargetProto:Create(targetName)
	end
	return self.Table[targetName]
end

function TargetHelper:GetTarget(criteria)
	local k,v
	for k,v in pairs(self.Table) do
		if criteria(v) then return v end
	end
end

function TargetHelper:CheckAll()
	local k,v
	local gt = GetGameTime()
	for k,v in pairs(self.Table) do 
		if v.deadGameTime and gt > v.deadGameTime then 
			v:Dispose()
			self.Table[k] = nil
		end
	end
end

function TargetHelper:Iterate(iterFunc)
	-- iterFunc(targetName,target) returns nil if not iterate and returns 1 if it wants to continue iteration
	local k,v
	for k,v in pairs(self.Table) then
		if not iterFunc(k,v) then return false end
	end
	return true
end

