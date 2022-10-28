function setup()
    SetMapSprite("assets/encounters/01/map_final.png")
    CreateObject("assets/encounters/01/boss.lua",144,0,TYPE_ENEMY);
    SetEncounterMusic("assets/audio/first_boss.wav");


end
local timer = 10;
local numAoesMade = 0;
function update(dt)

    timer = timer + dt;
    if (timer > 10) then
        f1 = {};
        f1["trigger"] = TRIGGER_INSTANT;
        f1["type"] = EFFECT_CURE;
        f1["value"] = 1;
        timer = 0
        local aoe = -1;
        if (math.fmod(numAoesMade, 2) == 0) then
            aoe = CreateAOE(63.5,63.5,"",20,0.25,10,false,ATTACK_HITS_FRIENDLIES,COLOR_HEAL,DITHER_VERTICAL_HALF,false,-1,{f1});
        else
            aoe = CreateAOE(223.5,159.5,"",20,0.25,10,false,ATTACK_HITS_FRIENDLIES,COLOR_HEAL,DITHER_VERTICAL_HALF,false,-1,{f1});
        end
        numAoesMade = numAoesMade + 1;
        SetAttackCircle(aoe,true);
    end

end

function kill()

end

function mapend()
    return true;
end