local delay = 5
local timer = 0
local radius = 30

function setup()
    SetAbilityRange(2)
    SetCooldown(20);
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
        f1 = {};
        f1["trigger"] = TRIGGER_TIMER;
        f1["type"] = EFFECT_HURT;
        f1["triggersPerSecond"] = 2
        f1["value"] = 2;  
        f1["duration"] = 10;
        f1["portrait"] = "assets/enemies/wyrm_boss/ability_bite_effect_portrait.png"
        CreateAOE(x, y, "", radius, 1, 120, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_VERTICAL_QUARTER, false, obj, {f1})
       

        atk = -1;
        timer = 0;
    end

end
function abilitytick(x, y, durationLeft, parent, target, dt)
    if (atk >= 0) then
        x2 = GetX(target);
        y2 = GetY(target);
        MoveAttack(x2,y2);
    end
end