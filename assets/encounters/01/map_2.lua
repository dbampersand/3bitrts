function setup()
    SetMapSprite("assets/encounters/01/map.png")
    CreateObject("assets/encounters/01/boss.lua",64,64,TYPE_ENEMY);
    SetAutoWin(true);

end
local timer = 0;
function update(dt)

    timer = timer + dt;
    if (timer > 5) then
        f1 = {};
        f1["trigger"] = TRIGGER_INSTANT;
        f1["type"] = EFFECT_CURE;
        f1["value"] = 1;
        timer = 0
        local aoe = CreateAOE(RandRange(190,240),RandRange(60,190),"",20,0.25,5,false,ATTACK_HITS_FRIENDLIES,COLOR_HEAL,DITHER_VERTICAL_HALF,false,-1,{f1});
        SetAttackCircle(aoe,true);
    end

end

function kill()

end