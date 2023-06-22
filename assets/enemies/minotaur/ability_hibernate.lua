local time = 15
local totalHeal = 1400
local tickRate = 0.5

function setup()

    SetAbilityRange(2)
    --AbilitySetCastType(ABILITY_CONE);
    SetCooldown(120); 
    SetAbilityName("Hibernate")
    AbilitySetPortrait("assets/enemies/minotaur/ability_hibernate.png");
    SetDescription("[b]Hibernate\n\nFalls asleep, healing over " .. time .. " seconds.")

end
local cone = -1
local objTargetting = -1
function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/enemies/minotaur/audio/hibernate.wav",1)


    SetObjChannelling(GetObjRef(),time);
    SetChannelingSprite("assets/enemies/minotaur/minotaur_channel_hibernate.png")
    local f1 = {}; 
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HEAL;
    f1["value"] = totalHeal / time * tickRate;
    f1["triggersPerSecond"] = tickRate; 
    f1["duration"] = time

    local f2 = {};
    f2["trigger"] = TRIGGER_CONST;
    f2["type"] = EFFECT_ATTACKDAMAGE;
    f2["value"] = -9999;
    f2["duration"] = time

    ApplyEffect(GetObjRef(),{f1,f2});



    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end