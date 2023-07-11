local efficiency = 1.2


local heal = 600
local duration = 20

local radius = 60

function setup()
    AbilitySetPortrait("assets/friendly/herbalist/icon_mix.png");
    SetDescription("Mix\n\nHeals all friendly targets over time, stacking up to 3 times.")

    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetHintColor(EFFECT_HEAL)


    SetCooldown(2);
    SetManaCost(30);

end


function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/friendly/herbalist/audio/mix.wav",1.25)
    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER
    f1["type"] = EFFECT_HEAL
    f1["triggersPerSecond"] = 0.5
    f1["value"] = heal / duration * f1["triggersPerSecond"]; 
    f1["duration"] = duration;
    f1["name"] = "Mix"

    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",radius,0.1,0.1,false,ATTACK_HITS_FRIENDLIES,COLOR_HEAL,DITHER_HEAL_HALF,false,-1,0,0,0,{f1});
    After(ShakeScreen,0.1,true,2,0.1)    

    return true; 
end

function untoggle()
    KillObj(totem);
    totem = -1;
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
end