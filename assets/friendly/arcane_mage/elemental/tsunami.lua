local aoe = -1
local timer = 0
local directionMoving = {}

local speed = 10
function setup()
    AbilitySetPortrait("assets/friendly/monk/icon_palm.png");
    AbilitySetCastType(ABILITY_ANGLE);
    SetDescription("Tsunami\n\nDeals damage and slows in a moving tidal wave.");
    SetCooldown(1);
    SetAbilityRange(50);    
end
function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/friendly/rogue/audio/cloak.wav",1.25)

    f1 = {};
    f1["trigger"] = TRIGGER_INSTANT
    f1["type"] = EFFECT_HURT
    f1["value"] = 20;

    
    aoe = CreateAOE(x,y,"", 30, 1, 10, false, ATTACK_HITS_ENEMIES,COLOR_DEFAULT,DITHER_STAR_HALF,false, -1, {f1})

    local dirX = x - GetX(GetObjRef());
    local dirY = y - GetY(GetObjRef());


    directionMoving = Normalize(dirX,dirY);
    Print("" .. directionMoving.x .. ", " .. directionMoving.y)
    timer = 0
    return true;
end
function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    if (attackRef == aoe) then
        timer = timer + dt;
        --if (timer > 0.25) then
          --  timer = 0;
          Print("dt: " .. dt)
          Print("speed: " .. speed)
          Print("directionMoving.x: " .. directionMoving.x)
          Print("directionMoving.y: " .. directionMoving.y)


            local pos = GetAttackPosition(aoe);
            MoveAttack(aoe, pos.x + directionMoving.x * speed * dt, pos.y + directionMoving.y * speed * dt)

        --end
    end
end