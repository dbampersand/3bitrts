local explode = 0
local lifetime = 12
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

    local timer = string.format("%.0f",lifetime);
    ShowString(timer,GetX(GetObjRef()),GetY(GetObjRef()) - GetHeight(GetObjRef())*1.5,ALIGN_CENTER)


    if (math.ceil(timerLast) ~= math.ceil(lifetime)) then
        PlaySound("assets/enemies/explosive_bomb/audio/tick.wav",1)
    end
    timerLast = lifetime

end

function kill()
end