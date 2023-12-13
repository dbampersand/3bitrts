
local numAoEs = 10

local tickrate = 0.5
local dps = 20

local cooldown = 30

local radius = 15
function setup()
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_summon_adds.png");
    SetAbilityName("Tsunami");
    SetDescription("[b]Tsunami\n\nCreates a powerful wave of attacks.");
    SetCooldown(cooldown)

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/enemies/wyrm_boss/audio/summon_add.wav",1)

    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickrate
    

    for i = 1, numAoEs do
        local speed = 0
        if (math.fmod(i,2) == 0) then
            speed = 8
        else
            speed = 5
        end
        CreateAOE(GetMapWidth()/numAoEs*i,1,"",radius,tickrate,9999,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_HALF,false,-1,0,1,speed,{f1});
    end 
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
end