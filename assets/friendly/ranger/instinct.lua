local radius = 40

function setup()
    AbilitySetPortrait("assets/friendly/ranger/icon_instinct.png");
    AbilitySetCastType(ABILITY_POINT);
    SetAbilityHint(HINT_CIRCLE,radius);
    SetAbilityRange(50)
    SetDescription("Instinct\n\nIncreases autoattack damage for units around the target position.")
    SetCooldown(70)
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/ranger/audio/instinct.wav",1.25)

    f1 = {};
    f1["trigger"] = TRIGGER_CONST;
    f1["type"] = EFFECT_ATTACKDAMAGE;
    --f1["numTriggers"] = 5
    f1["value"] = 15;
    f1["duration"] = 1;
    f1["triggersPerSecond"] = 1
    f1["overwrites"] = true
    f1["name"] = "Instinct"


    aoe = CreateAOE(x,y,"", radius, 1, 10, false, ATTACK_HITS_FRIENDLIES,COLOR_FRIENDLY_DAMAGE,DITHER_ATTACK_DAMAGE_QUARTER, false,-1,{f1})
    SetAttackCircle(aoe,true);
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end