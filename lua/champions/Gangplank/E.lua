function finishCasting()
	local speed = getEffectValue( 3 )
	local owner = getOwner()
	local myTeam = owner:getTeam()
	local units = getUnitsInRange( owner, 1300, true )

    print("Speed increase" .. speed)

	for key,value in pairs( units ) do
		if myTeam == value:getTeam() then
		    local buff = Buff.new( "DragonSlayerx1", 7.0, BUFFTYPE_TEMPORARY, value )
		    buff:setMovementSpeedPercentModifier( getEffectValue(3) )
		    addBuff( buff, value )
	        --addParticleTarget( "pirate_raiseMorale_tar.troy", value )
			local chorizo = "Riftwalk_Flashback.troy"
			addParticleTarget( chorizo, value )
			--addParticle(chorizo, getOwnerX()+10, getOwnerY()+10;
		end
	end
end

function applyEffects()
end
