local radius = 75

function setup()
    AbilitySetPortrait("assets/friendly/priest/icon_minorheal.png");
    SetDescription("Circle of Healing\n\nLarge area heal.")

    AbilitySetCastType(ABILITY_INSTANT);

    SetAbilityRange(20)
    SetCooldown(0);
    SetManaCost(65);
end


function casted(x,y,obj,headingx,headingy)

    
    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HEAL;
    f1["value"] = 100;  

    local aoe = CreateAOE(GetX(obj),GetY(obj),"", radius, 0.25, 0.25, false, ATTACK_HITS_FRIENDLIES,COLOR_HEAL,DITHER_HALF,false, -1, {f1})
    SetAttackCircle(aoe,true);
    

    return true; 
end

function untoggle()
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)

    MoveAttack(GetAttackRef(),GetX(obj),GetY(obj));

end