Vector2 = require 'Vector2' -- include 2d vector lib 

function finishCasting()
	local castTarget = getCastTarget()
	local current = Vector2:new(getOwnerX(), getOwnerY())
	if current:distance(Vector2:new(castTarget:getX(), castTarget:getY())) <= 550 then	
		addProjectileTarget( castTarget )
	else
		print("Target is too far away")
	end
	
end

function applyEffects()
	local castTarget = getCastTarget()

    if ( ( not ( castTarget == 0 ) ) and ( not isDead( castTarget ) ) ) then
		print(getEffectValue(0))
		print(getEffectValue(1))
		local owner = getOwner();
		local damage = getEffectValue(0) + owner:getStats():getTotalAp()

		owner:dealDamageTo( castTarget, damage, DAMAGE_TYPE_MAGICAL, DAMAGE_SOURCE_SPELL );
			
		local amt = getEffectValue(1)/100
		local buff = Buff.new("IceBlast", 2.5, BUFFTYPE_TEMPORARY, castTarget)
		buff:setMovementSpeedPercentModifier(-speedIncrease)
		addBuff(buff, castTarget)
	end

    destroyProjectile()
end
