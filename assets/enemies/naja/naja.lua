local bind = 0

function setup()
    SetSprite("assets/enemies/naja/naja.png");

    SetDamage(20);
    SetMaxHP(2000,true)
    SetSpeed(80)
    SetRange(50);

    bind = AddAbility(GetObjRef(),"assets/enemies/naja/ability_bind.lua",0);
    SetChannelingSprite("assets/enemies/naja/naja_channeling.png")
end

function update(dt)
    if (IsInCombat()) then

        local target = {};

        local targ = GetRandomUnit(TYPE_ENEMY,TYPE_ANY,999);


        target["target"] = targ;
        if (target["target"] ~= nil) then
            CastAbility(bind,2,{target});
        end
    end
end

function kill()

end