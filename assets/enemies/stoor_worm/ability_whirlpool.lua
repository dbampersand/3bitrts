

local tickrate = 2
local dps = 120

local cooldown = 10

local radius = 15

local duration = 12
function setup()
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_summon_adds.png");
    SetAbilityName("Whirlpool");
    SetDescription("[b]Whirlpool\n\nCreates a damaging area that spits out projectiles.");
    SetCooldown(cooldown)

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/enemies/wyrm_boss/audio/summon_add.wav",1)

    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps * tickrate

    local f2 = {}
    f2["trigger"] = TRIGGER_CONST;
    f2["type"] = EFFECT_SPEED;
    f2["value"] = -40
    f2["duration"] = 0.5



    CreateAOE(RandRange(0,GetMapWidth()),RandRange(64,GetMapHeight()),"",radius,tickrate,duration,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_HALF,false,-1,0,0,0,{f1,f2});
    
    

    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
end
function applyattack(atk,x,y)
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 8;  
    CreateCircularProjectiles(x,y,"",ATTACK_PROJECTILE_ANGLE,10,999,false,ATTACK_HITS_ENEMIES,8,COLOR_DAMAGE,RandRange(0,360),{f1})
end