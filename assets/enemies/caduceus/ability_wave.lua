local height = 50
local dps = 90
local tickrate = 0.25

function setup()
    SetAbilityRange(30)
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Wave\n\nSummons areas of damage at the ends of the map.")
    AbilitySetCastType(ABILITY_INSTANT);
end
function casted(x,y,obj,headingx,headingy)
    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickrate;


    local w = GetMapWidth();
    local radius = 20
    local speed = 50

    local xPos = 0;

    for i = 0, w/radius do 

        local y = 0;

        if (math.floor(math.fmod(i,2)) == 0) then
            y = GetUIHeight();
        end
        local aoe = CreateAOE(xPos,y,"",radius,tickrate,9999,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_VERTICAL_HALF,false,-1,{f1});
        SetAttackVelocity(aoe,speed);

        if (math.floor(math.fmod(i,2)) == 0) then
            SetAttackMoveAngle(aoe,0,-1)
        else 
            SetAttackMoveAngle(aoe,0,1)
        end

        xPos = xPos + radius*2;


    end


    SetAttackCircle(aoe,true);

    PlaySound("assets/enemies/caduceus/audio/cure_aoe.wav",0.7);

    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/caduceus/caduceus_cast_wave.png")
end