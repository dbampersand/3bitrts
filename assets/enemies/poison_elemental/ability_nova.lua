local dps = 10
local ticksPerSec = 2

local duration = 12

local casts = 0

function setup()
    SetAbilityRange(256)
    SetCooldown(10);
    AbilitySetPortrait("assets/enemies/poison_elemental/ability_nova.png");
    SetDescription("[b]Nova\n\nCauses a devastating projectile attack.")
    SetAbilityName("Nova"); 
    SetAbilityHint(HINT_CIRCLE,20);

end

function casted(x,y,obj,headingx,headingy)

    PlaySound("assets/enemies/poison_elemental/audio/nova.wav",0.75)


    local f1 = {};
    f1["trigger"] = TRIGGER_TIMER;
    f1["type"] = EFFECT_HURT;
    f1["value"] = dps / ticksPerSec;  
    f1["triggersPerSecond"] = 2
    f1["duration"] = duration
    f1["maxStacks"] = 10
    f1["canStack"] = true;
    f1["name"] = "Nova"  


    local offset = 0;
    if (math.fmod(casts,2) == 0) then
        offset = 90;
    end
    CreateCircularProjectiles(GetX(GetObjRef()),GetY(GetObjRef()),"",ATTACK_PROJECTILE_ANGLE,20,999,false,ATTACK_HITS_ENEMIES,10,COLOR_DAMAGE,offset,{f1})
    casts = casts + 1;


    return true; 
end

function onhit(x,y,objhit)
    PlaySound("assets/enemies/poison_elemental/audio/nova_hit.wav",0.25)
end
function ontimeout(x,y,obj,dt,target)

end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/poison_elemental/poison_elemental_casting_nova.png");

end