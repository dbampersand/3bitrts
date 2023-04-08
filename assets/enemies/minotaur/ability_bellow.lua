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

    CreateAOE(1,1,"", 999, 0, 0,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_NONE,false, -1, {f1})
    enemies = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,10,999)
    for i=1,#enemies+1 do
        PushObj(GetX(GetObjRef()),GetY(GetObjRef()),enemies[i],200,0.1)
    end
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target, attack)
end

function abilitytick(x, y, timeUntilAttackTick, parent, target, dt, attackRef, totalAttackDurationLeft)

end