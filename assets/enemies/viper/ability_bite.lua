local damage = 100
local ticksPerSec = 5
local duration = 40

function setup()

    SetAbilityRange(10)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Bite\n\nDeals damage over time.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["name"] = "Bite"
    f1["canStack"] = true;  
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage / ticksPerSec / duration;
    f1["triggersPerSecond"] = ticksPerSec;  
    f1["duration"] = duration;
    f1["portrait"] = "assets/enemies/wyrm_boss/ability_bite_effect_portrait.png"


    ApplyEffect(obj,{f1});
    PlaySound("assets/enemies/viper/audio/bite.wav",1);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/viper/viper_cast_bite.png")
end