
function setup()
    AbilitySetPortrait("assets/friendly/arcane_mage/mana_sword/icon_dash.png");
    AbilitySetCastType(ABILITY_POINT);
    SetDescription("Dash\n\nDashes at the target. Deals damage and stuns around the target.");
    SetCooldown(8);
    SetAbilityRange(50);    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/cloak.wav",1.25)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 40;
    f1["duration"] = 6

    f2 = {};
    f2["trigger"] = TRIGGER_INSTANT
    f2["type"] = EFFECT_STUN
    f2["value"] = 1


    Teleport(GetObjRef(),x,y);
    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",20,0.1,0.1,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_VERTICAL_HALF,false,-1,{f1,f2});

    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
    
end