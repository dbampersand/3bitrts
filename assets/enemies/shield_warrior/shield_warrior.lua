    function setup()
    SetSprite("assets/enemies/shield_warrior/shield_warrior.png");

    SetDamage(20);
    SetMaxHP(400,true)
    SetSpeed(80)
    SetRange(5);

    AddAbility("assets/enemies/shield_warrior/ability_shieldstance.lua",0);

end
local timer = 0
function update(dt)
    if (IsInCombat()) then
        CastAbility(0,0,{});
    end
end

function kill()

end