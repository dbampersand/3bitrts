
function setup()
    AbilitySetPortrait("assets/friendly/ranger/icon_galeshot.png");
    AbilitySetCastType(ABILITY_POINT);
    SetDescription("Galeshot\n\nDeals damage in an area.")
    SetCooldown(40);
    SetAbilityRange(50);

end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/summon.wav",1.25)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 20;


    local aoe = CreateAOE(x,y,"", 40, 1, 6, false, ATTACK_HITS_ENEMIES,COLOR_FRIENDLY_DAMAGE,DITHER_DAMAGE_QUARTER, false, -1, {f1})

   return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end