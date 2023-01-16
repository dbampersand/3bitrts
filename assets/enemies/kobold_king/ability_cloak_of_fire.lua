function setup()
    SetAbilityRange(120)
    SetCooldown(4);
    AbilitySetPortrait("assets/enemies/cusith/ability_focus.png");
    SetDescription("[b]Cloak of Fire\n\nDeals damage to all in melee range.")
    
end

function casted(x,y,obj,headingx,headingy)


    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 25;  

    CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", 60, 0.25, 0.25,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_NONE,false, -1, {f1})

    return true;
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target, attack)
end

function abilitytick(x, y, timeUntilAttackTick, parent, target, dt, attackRef, totalAttackDurationLeft)

end

function onchanneled() 
    SetChannelingSprite("assets/enemies/kobold_king/kobold_king_channelling_cloak_of_fire.png");
end