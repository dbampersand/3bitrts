function setup()
    SetAbilityRange(40)
    SetCooldown(10)
    SetAbilityHint(HINT_CIRCLE,30);
    AbilitySetCastType(ABILITY_POINT);
    AbilitySetPortrait("assets/enemies/minotaur/ability_smash.png");
    SetDescription("[b]Smash\n\nDeals massive damage and stuns.")

end

function casted(x,y,obj,headingx,headingy)


    PlaySound("assets/enemies/minotaur/audio/nuke.wav",1)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 350;  

    f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_STUN;
    f2["value"] = 4;  


    CreateAOE(GetX(obj),GetY(obj),"", 30, 0, 0, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, false, obj, {f1,f2})
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft, obj, target)
    
end