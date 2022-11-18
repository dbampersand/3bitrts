
local summon = 0
local cureaoe = 0

function setup()
    SetSprite("assets/enemies/caduceus/caduceus.png");
    SetDamage(0);
    SetMaxHP(750,true)
    SetSpeed(0)

    summon = AddAbility(GetObjRef(),"assets/enemies/caduceus/ability_summon.lua",0);
    cureaoe = AddAbility(GetObjRef(),"assets/enemies/caduceus/ability_cureaoe.lua",1);

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
    SetAbilityCooldownTimer(cureaoe,0)
    CastAbility(cureaoe,0,{{x=cx,y=cy}});
end