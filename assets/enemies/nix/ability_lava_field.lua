local length = 15
local time = 12
local numAoEsSpawned = 5

function setup()
    SetAbilityRange(256)
    SetCooldown(12);
    AbilitySetPortrait("assets/enemies/nix/ability_lava_field.png");
    SetDescription("[b]Lava Field\n\nThrows out "..numAoEsSpawned.." lava fields on top of enemy units. If there isn't enough units in range, they are targetted randomly.");
    SetAbilityName("Lava Field"); 
    SetAbilityHint(HINT_LINE);

end


local function makeAOE(x,y)
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 60;

    local time = RandRange(0,0.35)
    After(CreateAOE,time,true,x,y,"", 25, 1, 5, false, ATTACK_HITS_ENEMIES, COLOR_DAMAGE, DITHER_DAMAGE_EIGTH, false, -1, {f1})
    After(PlaySound,time,true,"assets/enemies/nix/audio/lava_field.wav",0.25)
end

function casted(x,y,obj,headingx,headingy)

    local enemies = GetAllObjsByFriendliness(GetOppositeFriendliness(GetObjRef()));
    local numToSpawnOnEnemies = math.min(#enemies,numAoEsSpawned)
    local numToSpread = numAoEsSpawned - numToSpawnOnEnemies;
    for i=1, numToSpawnOnEnemies do
        makeAOE(GetX(enemies[i]),GetY(enemies[i]));
    end
    for i=1, numToSpread do
        makeAOE(RandRange(0,GetMapWidth()),RandRange(0,GetMapHeight()))
    end
    

    return true; 
end

function onhit(x,y,objhit)
end
function ontimeout(x,y,obj,dt,target)
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/nix/nix_casting_lava_field.png")

end
function applyattack(a,x,y) 
    PlaySound("assets/enemies/nix/audio/lava_field_tick.wav",0.1,x,y)
end