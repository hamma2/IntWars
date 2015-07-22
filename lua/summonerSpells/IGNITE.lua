Vector2 = require 'Vector2' -- include 2d vector lib 

function finishCasting()
	local castTarget = getCastTarget()
	local current = Vector2:new(getOwnerX(), getOwnerY())
	if current:distance(Vector2:new(castTarget:getX(), castTarget:getY())) <= 625 then
		local buff = Buff.new("SummonerIgnite", 5.0, BUFFTYPE_TEMPORARY, castTarget, getOwner())
		addBuff(buff, castTarget)
		addParticleTarget("global_ss_ignite.troy", castTarget)
	else
		print("Target is too far away")
	end
end

function applyEffects()
end
