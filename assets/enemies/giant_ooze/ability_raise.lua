local numZombies = 3
function setup()

    SetAbilityRange(999)
    AbilitySetCastType(ABILITY_INSTANT);
    SetAbilityHint(HINT_CIRCLE,80);
    SetCooldown(15); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Raise\n\nRaises "..numZombies .. " zombies.")
end
function onchanneled()
    SetChannelingSprite("assets/enemies/giant_ooze/giant_ooze_channelling.png");
end

function casted(x,y,obj,headingx,headingy)

    for i = 1, 3 do
        local spawnX = RandRange(0,GetMapWidth())
        local spawnY = RandRange(0,GetMapHeight())
        SetObjAggroRadius(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",spawnX,spawnY,TYPE_FRIENDLY,0,0),999);
    end
    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end