local coneSize = 80

function setup()

    SetAbilityRange(60)
    --AbilitySetCastType(ABILITY_CONE);
    SetAbilityHint(HINT_CONE,coneSize);
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/cobra/ability_spit.png");
    SetDescription("[b]Spit\n\nDeals damage over time in a cone.")

end
function casted(x,y,obj,headingx,headingy)


    f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 10;  
    f1["duration"] = 10
    f1["triggersPerSecond"] = 1
    f1["canStack"]= true;
    f1["name"] = "Spit"
    center = GetCentre(GetObjRef());
    centertarget = GetCentre(obj);
    objTargetting = obj;
    cone = CreateCone(center.x,center.y,centertarget.x,centertarget.y,"", 80, 0, 0, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER,coneSize, {f1})
    SetAttackTarget(cone,objTargetting);

    PlaySound("assets/enemies/cobra/audio/spit.wav",0.5)
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end