local delay = 5
local radius = 30

function setup()
    SetAbilityRange(256)
    SetCooldown(30);
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_bomb.png");
    SetDescription("[b]Bomb\n\nDeals damage over time after " .. delay .. " seconds")

end

local atk = -1
local attackActive = false;

function casted(x,y,obj,headingx,headingy)
    atk = CreateAOE(GetX(obj),GetY(obj),"", radius, delay, delay, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_NONE, false, obj, {})
    
    return true; 
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target)
    
    if (atk >= 0) then
        PlaySound("assets/enemies/wyrm_boss/audio/bomb.wav",1)

        f1 = {};
        f1["trigger"] = TRIGGER_TIMER;
        f1["type"] = EFFECT_HURT;
        f1["triggersPerSecond"] = 2
        f1["value"] = 1.5;  
        f1["duration"] = 10;
        f1["portrait"] = "assets/enemies/wyrm_boss/ability_bite_effect_portrait.png"
        CreateAOE(x, y, "", radius, 1, 15, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_VERTICAL_QUARTER, false, target, {f1})
       

        atk = -1;
    end

end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    if (atk >= 0 and attackRef == atk) then
        x2 = GetX(target);
        y2 = GetY(target);
        MoveAttack(atk,x2,y2);
    end
end