function setup()
    SetSprite("assets/enemies/caduceus/caduceus.png");
    SetDamage(0);
    SetMaxHP(600,true)
    SetSpeed(0)

    AddAbility("assets/enemies/caduceus/ability_summon.lua",0);
    AddAbility("assets/enemies/caduceus/ability_cureaoe.lua",1);

end

function update(dt)
    if (IsInCombat() == true) then
        local cx = GetX(GetObjRef())
        local cy = GetY(GetObjRef())

        CastAbility(1,0,{{x=cx,y=cy}});
        CastAbility(0,0,{{x=cx,y=cy}});

    end


end

function kill()

end