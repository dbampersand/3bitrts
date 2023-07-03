local atk = -1;

local bigAoEDuration = 2.5
local smallAoEDuration = 2.35


local maxLooseAim = 50

function setup()
    SetAbilityRange(999)
    SetCooldown(6)
    AbilitySetPortrait("assets/enemies/cusith/ability_crush.png");
    SetAbilityName("Crush")
    SetDescription("[b]Crush\n\nStomps the ground, causing an earthquake around targets.")
    SetAbilityHint(HINT_LINE);
    SetAbilityCastTypeHint(Bor(ABILITY_POINT,ABILITY_TARGET_ENEMY));
end

function casted(x,y,obj,headingx,headingy)
    --large aoe
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 120;
    
    --small aoe, loosely targeted
    local f2 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 40;


    CreateAOE(GetX(obj),GetY(obj),"", 25, bigAoEDuration, bigAoEDuration, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_EIGTH, false, -1,0,0,0, {f1})
    CreateAOE(GetX(obj)+RandRange(0,maxLooseAim),GetY(obj)+RandRange(0,maxLooseAim),"", 10, smallAoEDuration, smallAoEDuration, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_NONE, false, -1,0,0,0, {f2})
    CreateAOE(GetX(obj)+RandRange(0,maxLooseAim),GetY(obj)+RandRange(0,maxLooseAim),"", 10, smallAoEDuration, smallAoEDuration, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_NONE, false, -1,0,0,0, {f2})
    CreateAOE(GetX(obj)+RandRange(0,maxLooseAim),GetY(obj)+RandRange(0,maxLooseAim),"", 10, smallAoEDuration, smallAoEDuration, true, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_NONE, false, -1,0,0,0, {f2})
    
    PlaySound("assets/enemies/cusith/audio/crush.wav",1)


    return true; 
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target)
end


function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
end