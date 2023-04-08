local shieldstance = -1
local bash = -1
local timer = 0
function setup()
    SetSprite("assets/enemies/shield_warrior/shield_warrior.png");
    shieldstance = AddAbility(GetObjRef(),"assets/enemies/shield_warrior/ability_shieldstance.lua",0);
    bash = AddAbility(GetObjRef(),"assets/enemies/shield_warrior/ability_bash.lua",1);
    SetDamage(20);
    SetMaxHP(400,true)
    SetSpeed(80)
    SetRange(5);

    timer = RandRange(3,7);
end
local timer = 0
function update(dt)
    if (IsInCombat()) then

        timer = timer - dt
        CastAbility(shieldstance,0,{});

        if (timer < 0) then
            CastAbility(bash,0.5,{{target=GetRandomUnit(TYPE_ENEMY,TYPE_TANK,GetAbilityRange(GetObjRef(),bash),1)[1]}})
        end
    end
end

function kill()

end