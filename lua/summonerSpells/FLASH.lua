Vector2 = require 'Vector2' -- include 2d vector lib 

function finishCasting()
    local current = Vector2:new(getOwnerX(), getOwnerY())
    local to = Vector2:new(getSpellToX(), getSpellToY()) - current
    local trueCoords

    if to:length() > 425 then
        to = to:normalize()
        local range = to * 425
        trueCoords = current:copy() + range
    elseif(to:length() >= 212.5 and isWalkable(getSpellToX(), getSpellToY()) == false) then
        -- Apply position fix when requested teleport distance is more than half
        to = to:normalize()
        local range = to * 475
        trueCoords = current:copy() + range
    else
        trueCoords = Vector2:new(getSpellToX(), getSpellToY())
    end
    teleportTo(trueCoords.x, trueCoords.y)
end

function applyEffects()
end
