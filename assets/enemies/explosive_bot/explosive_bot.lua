local explode = 0
local lifetime = 10
function setup()
    SetSprite("assets/enemies/explosive_bot/explosive_bot.png");

    SetDamage(0);
    SetMaxHP(250,true)
    SetSpeed(0)
    SetRange(5);

    explode = AddAbility(GetObjRef(),"assets/enemies/explosive_bot/explode.lua",0);
end

function update(dt)
    lifetime = lifetime - dt;
    if (lifetime <= 0) then
        CastAbility(explode,0,{});
    end
end

function kill()
end