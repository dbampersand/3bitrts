function setup()
    SetAbilityRange(20)
    SetCooldown(30);
    AbilitySetPortrait("assets/enemies/minotaur/ability_bellow.png");
    SetDescription("[b]Bellow\n\nDeals damage to all enemies")
    
end

function casted(x,y,obj,headingx,headingy)
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 100;  

    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", 0, 999, 0,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_NONE,false, -1, {f1})
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target, attack)
end

function abilitytick(x, y, timeUntilAttackTick, parent, target, dt, attackRef, totalAttackDurationLeft)

end