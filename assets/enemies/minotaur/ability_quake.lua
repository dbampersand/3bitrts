local ticker = -1
local shouldAddAOE = 0

function setup()
    SetAbilityRange(20)
    SetCooldown(60);
    AbilitySetPortrait("assets/enemies/minotaur/ability_quake.png");
    SetDescription("[b]Quake\n\nSmash the ground, causing stalactites to fall.")
    
end

function casted(x,y,obj,headingx,headingy)
    SetObjChannelling(GetObjRef(),10);
    ticker = CreateAOE(1,1,"", 0, 10, 10,true,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_HALF,false, -1, {})
    return true; 
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target, attack)
    if (attack == ticker) then
        ticker = -1;
    end
end

function abilitytick(x, y, timeUntilAttackTick, parent, target, dt, attackRef, totalAttackDurationLeft)

    --Print(totalAttackDurationLeft);
    if (attackRef == ticker) then
        
        SetMovePoint(GetObjRef(),128,128,false,false);
        if (math.fmod((totalAttackDurationLeft),0.2) <= 0.1) then
            if (shouldAddAOE == true) then
                local f1 = {};
                f1["trigger"] = TRIGGER_INSTANT;
                f1["type"] = EFFECT_HURT;
                f1["value"] = 40;  
                CreateAOE(RandRange(0,255),RandRange(0,255),"", 10, 6, 6, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_NONE, false, GetObjRef(), {f1})
            
            end
            shouldAddAOE = false
        else
            shouldAddAOE = true
        end
    end
end