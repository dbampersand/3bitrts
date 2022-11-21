local squeeze = 0;
local timer = 0;
function setup()
    SetSprite("assets/enemies/giant_ooze/giant_ooze.png");
    SetChannelingSprite("assets/enemies/giant_ooze/giant_ooze_channelling.png");

    SetDamage(40);
    SetMaxHP(3000,true)
    SetSpeed(60)
    SetRange(10);

    squeeze = AddAbility(GetObjRef(),"assets/enemies/giant_ooze/squeeze.lua",0);

end

function update(dt)
    timer = timer + dt;
    if (timer > 5) then
        CastAbility(0,5,{});
    end
end

function kill()

end

function mapend()
    ChangeMap("assets/encounters/03/map_final.lua")
    return false;
end