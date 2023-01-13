function setup()
    SetAbilityRange(999)
    SetCooldown(20)
    SetAbilityHint(HINT_CIRCLE,30,true);
    AbilitySetCastType(ABILITY_POINT);
    AbilitySetPortrait("assets/enemies/minotaur/ability_smash.png");
    SetDescription("[b]Throw Concoction\n\nDeals a large amount of damage to all units in an area.")

end

function casted(x,y,obj,headingx,headingy)


    PlaySound("assets/enemies/minotaur/audio/nuke.wav",1)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 40;  


    CreateAOE(GetX(obj),GetY(obj),"", 30, 15, 1, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, true, obj, {f1})
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
    
end