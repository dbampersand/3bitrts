local large = 0
local small = 0


function setup()
    SetSprite("assets/enemies/rock_tosser/rock_tosser.png");

    SetDamage(10);
    SetMaxHP(750,true)
    SetSpeed(80)
    SetRange(60);

    large = AddAbility(GetObjRef(),"assets/enemies/rock_tosser/ability_throw_large_rock.lua",0);
    small = AddAbility(GetObjRef(),"assets/enemies/rock_tosser/ability_throw_small_rock.lua",1);

    SetChannelingSprite("assets/enemies/rock_tosser/rock_tosser_channeling.png")
end

function update(dt)
    if (IsInCombat()) then

        local target = {};

        local targ = GetRandomUnit(TYPE_ENEMY,TYPE_ANY,999);


        target["target"] = targ;
        if (target["target"] ~= nil) then
            CastAbility(large,2,{target});
            CastAbility(small,6,{target});

        end
    end
end

function kill()

end