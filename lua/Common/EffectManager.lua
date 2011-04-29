--[[
Effect Prototype

Effect:Enable() Enable Effect
Effect:IsReady() Is the effect ready. If the effect is not a useable effect, this will always return false.
Effect:Use() Use the effect(item)

]]

EffectManager = EffectManager or {}
EffectManager.Table = EffectManager.Table or {}

function EffectManager:Attach(obj)
	if not self.Table[obj] then
		self.Table[obj] = 0
	end
end

function EffectManager:Dettach(obj)
	self.Table[obj] = nil
end

function EffectManager:UseAllUseable()
	local k,v
	for k,v in pairs(self.Table) do
		if k:IsReady() then
			k:Use()
		end
	end
end

function EffectManager:EnableAll()
	local k,v
	for k,v in pairs(self.Table) do
		if v ~= 1 then
			k:Enable()
			self.Table[k] = 1
		end
	end
end