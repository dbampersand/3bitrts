function setup()
    AbilitySetPortrait("assets/friendly/arcane_mage/elemental/icon_burnout.png");
    AbilitySetCastType(ABILITY_INSTANT);
    SetDescription("Burnout\n\nKills the elemental, dealing damage proportional to its current health in an area.");
    SetCooldown(1);
    SetAbilityRange(9999);    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/cloak.wav",1.25)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = GetHP(GetObjRef());

    
    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", 50, 1, 1, false, ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_STAR_EIGTH,false, -1, {f1})
    KillObj(GetObjRef())
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
end