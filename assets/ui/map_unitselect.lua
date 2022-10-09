function setup()
    SetMapSprite("assets/ui/map_unitselect.png")
    CreateObject("assets/enemies/trainingdummy/trainingdummy.lua",60,16,TYPE_ENEMY,0);
    CreateObject("assets/enemies/healingdummy/healingdummy.lua",256-60,16,TYPE_FRIENDLY,0);



    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_CURE;
    f1["value"] = 1;
    local aoe = -1;
    aoe = CreateAOE(63.5,63.5,"",20,0.25,99999,false,ATTACK_HITS_FRIENDLIES,COLOR_HEAL,DITHER_VERTICAL_HALF,false,-1,{f1});

end