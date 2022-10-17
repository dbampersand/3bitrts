function setup()
    SetMapSprite("assets/ui/map_unitselect.png")
    CreateObject("assets/enemies/trainingdummy/trainingdummy.lua",60,16,TYPE_ENEMY,0);
    CreateObject("assets/enemies/healingdummy/healingdummy.lua",256-60,16,TYPE_FRIENDLY,0);

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_CURE;
    f1["value"] = 1;
    timer = 0

    for i = 1, 256 do
        --CreateAOE(63.5,63.5,"",20,0.25,10,false,ATTACK_HITS_FRIENDLIES,COLOR_HEAL,DITHER_STAR_EIGTH,false,-1,{f1});

    end
end
function update()

    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["duration"] = 0.2
    f1["triggersPerSecond"] = 1
    f1["type"] = EFFECT_HURT_PERCENT;
    f1["value"] = 0.34;  
    f1["name"] = "Howl"
    f1["overwrites"] = true

    --CreateCircularProjectiles(128,128,"",ATTACK_PROJECTILE_ANGLE,80,999,false,ATTACK_HITS_ENEMIES,128,COLOR_DAMAGE,{f1})

end