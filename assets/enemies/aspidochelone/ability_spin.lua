local damage = 75
local damageProjectile = 20
local radius = 80
local aoe = -1
function setup()

    SetAbilityRange(256)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(12); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Spin\n\nDeals damage and sheds spikes in an area around the user.")
end
function casted(x,y,obj,headingx,headingy)
    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = damage;


    aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", radius, 0.1, 0.1, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_HALF, false, -1, 0,0,0, {f1})
    
    SetAttackInactive(aoe,3)

    local f2 = {}
    f2["trigger"] = TRIGGER_INSTANT;
    f2["type"] = EFFECT_HURT;
    f2["value"] = damageProjectile;

    After(CreateCircularProjectiles,3,false,GetX(GetObjRef()),GetY(GetObjRef()),"",ATTACK_PROJECTILE_ANGLE,50,999,false,ATTACK_HITS_ENEMIES,16,COLOR_DAMAGE,0,{f2})
    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,parent,dt,target,attack)
    if (attack == aoe) then
        attack = -1
    end
end
function abilitytick(x, y, durationLeft,parent,target,dt,attackRef)
    if (attackRef == aoe) then
        MoveAttack(attackRef,GetX(GetObjRef()),GetY(GetObjRef()))
    end
end

function applyattack(atk,x,y)
end
function timerbeforetick(t,func)
    UpdateTimerArgument(t,1,GetX(GetObjRef()))
    UpdateTimerArgument(t,2,GetY(GetObjRef()))


end