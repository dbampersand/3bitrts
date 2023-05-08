local damage = 80
local spawnRadius = -30
function setup()

    SetAbilityRange(200)
    --AbilitySetCastType(ABILITY_TARGET_ENEMY);
    SetAbilityHint(HINT_LINE,80);
    SetCooldown(16); 
    AbilitySetPortrait("assets/enemies/skeleton_basher/ability_bash.png");
    SetDescription("[b]Raise\n\nRessurects a ranged or melee minion.")
end
function casted(x,y,obj,headingx,headingy)

    local spawnPoint = {} 
    spawnPoint.x = GetX(GetObjRef()) + RandRange(-spawnRadius, spawnRadius)
    spawnPoint.y = GetY(GetObjRef()) + RandRange(-spawnRadius, spawnRadius)

    local typeSpawn = Round(RandRange(0,1))
    if (typeSpawn == 0) then
        CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",spawnPoint.x,spawnPoint.y,TYPE_FRIENDLY,1.5);
    else
        CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",spawnPoint.x,spawnPoint.y,TYPE_FRIENDLY,1.5);
    end


    PlaySound("assets/enemies/viper/audio/bite.wav",1);
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end