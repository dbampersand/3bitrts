local timer = 0
local spawnedBoss = false

function setup()
    SetMapSprite("assets/encounters/02/map.png")
    local ob = CreateObject("assets/enemies/shield_warrior/shield_warrior.lua",120,54,TYPE_ENEMY);
    SetObjAggroRadius(ob,999)

    ob = CreateObject("assets/enemies/berzerker/berzerker.lua",120,22,TYPE_ENEMY);
    SetObjAggroRadius(ob,999)

    SetAutoWin(false);
    --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);
end

function update(dt)
    if (spawnedBoss == false) then

        local enemyObjs = GetAllObjsByFriendliness(TYPE_ENEMY);
        local numObjs = #enemyObjs;
        if (numObjs == 0) then
            local ob = CreateObject("assets/enemies/shield_warrior/shield_warrior.lua",120,54,TYPE_ENEMY);
            SetObjAggroRadius(ob,999)
        
            ob = CreateObject("assets/enemies/berzerker/berzerker.lua",120,22,TYPE_ENEMY);
            SetObjAggroRadius(ob,999)        
        end
    end

end

function kill()

end