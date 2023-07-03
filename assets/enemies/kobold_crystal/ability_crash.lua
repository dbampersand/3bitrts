local length = 15
local time = 12

function setup()
    SetAbilityRange(999)
    SetCooldown(15);
    AbilitySetPortrait("assets/enemies/kobold_crystal/ability_crash.png");
    SetDescription("[b]Crash\n\nDeals damage to *all* units and stuns them.");
    SetAbilityName("Crash"); 
    SetAbilityHint(HINT_CIRCLE,30);

end

function casted(x,y,obj,headingx,headingy)
    

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 80;

    local f2 = {};
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_STUN;
    f2["value"] = 2;

    CreateAOE(GetX(obj),GetY(obj),"", 30, 0, 0, false, Bor(ATTACK_HITS_ENEMIES,ATTACK_HITS_FRIENDLIES), COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, false, obj,0,0,0,{f1,f2})
    PlaySound("assets/enemies/kobold_crystal/audio/crash.wav",0.5)

    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,obj,dt,target)
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    
end
function onchanneled() 
    PlaySound("assets/enemies/kobold_crystal/audio/crash_onchannel.wav",0.25)

end