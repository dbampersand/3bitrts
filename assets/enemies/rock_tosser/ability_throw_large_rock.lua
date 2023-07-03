local duration = 0.1
local atk = -1

function setup()

    SetAbilityRange(999)
    AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_CIRCLE,30,true);
    SetCooldown(16); 
    AbilitySetPortrait("assets/enemies/rock_tosser/ability_throw_large_rock.png");
    SetDescription("[b]Throw Large Rock\n\nDeals soak damage in an area.")
end
function casted(x,y,obj,headingx,headingy)

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 200;  

    atk = CreateAOE(GetX(obj),GetY(obj),"", 30, duration, duration, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HORIZONTAL_QUARTER, true, obj, 0,0,0,{f1})
    PlaySound("assets/enemies/rock_tosser/audio/toss_onchannel.wav",0.5);


    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    if (atk >= 0 and attackRef == atk) then
        x2 = GetX(target);
        y2 = GetY(target);
        MoveAttack(atk,x2,y2);
    end

end

function ontimeout(x,y,obj,dt,target)
    atk = -1
    PlaySound("assets/enemies/rock_tosser/audio/toss_large.wav",1);
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/rock_tosser/rock_tosser_cast_throw_large_rock.png")
end