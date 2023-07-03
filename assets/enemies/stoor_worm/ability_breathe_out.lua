
local numAoEs = 20

local aoeDPS = 20
local aoeRadius = 15
local tickrate = 0.5
local aoeMinDuration = 6
local aoeMaxDuration = 12


function setup()
    AbilitySetCastType(ABILITY_INSTANT);
    AbilitySetPortrait("assets/enemies/wyrm_boss/ability_summon_adds.png");
    SetAbilityName("Breathe Out");
    SetDescription("[b]Breathe Out\n\nPushes all enemy units away and summons a field of damaging spores.");
    SetCooldown(12)

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/enemies/wyrm_boss/audio/summon_add.wav",1)
    local enemies = GetAllObjsByFriendliness(GetOppositeFriendliness(GetObjRef()))
    for i = 1,#enemies do
        local fromX = GetX(GetObjRef())
        local fromY = GetY(GetObjRef())
        
        PushObj(fromX,fromY,enemies[i],999,0.1)
    end
    for i = 1,numAoEs do
        local f1 = {};
        f1["trigger"] = TRIGGER_INSTANT;
        f1["type"] = EFFECT_HURT;
        f1["value"] = aoeDPS * tickrate;
        
        local duration = RandRange(aoeMinDuration,aoeMaxDuration)
        local radius = RandRange(aoeRadius-1,aoeRadius+1)
        After(CreateAOE,RandRange(0.1,0.5),true,RandRange(0,GetMapWidth()),RandRange(0,GetMapHeight()),"",radius,tickrate,duration,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,DITHER_DAMAGE_EIGTH,false,-1,0,0,0,{f1});
        end
    return true; 
end

function onhit(x,y,objhit)

end

function abilitytick(x, y, durationLeft)

end