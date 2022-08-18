function setup()
    SetSprite("assets/enemies/caduceus/caduceus.png");
    SetDamage(0);
    SetMaxHP(200,true)
    SetSpeed(0)

    AddAbility("assets/enemies/caduceus/ability_summon.lua",0);

end

function update(dt)
    if (IsInCombat() == true) then
        CastAbility(0,0,{});
    end
end

function kill()

end