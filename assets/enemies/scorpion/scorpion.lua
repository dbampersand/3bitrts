local spit = 0

function setup()
    SetSprite("assets/enemies/scorpion/scorpion.png");

    SetDamage(20);
    SetMaxHP(600,true)
    SetSpeed(80)
    SetRange(50);

    spit = AddAbility(GetObjRef(),"assets/enemies/scorpion/ability_spit.lua",0);
    SetChannelingSprite("assets/enemies/scorpion/scorpion_channeling.png")
end

function update(dt)
    if (IsInCombat()) then

        local target = {};

        local targ = GetRandomUnit(TYPE_ENEMY,TYPE_ANY,999);

        target["target"] = targ;
        if (target["target"] ~= nil) then
            CastAbility(spit,2,{target});
        end
    end
end

function kill()

end