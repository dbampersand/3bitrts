local timer = 0
local spawnedBoss = false

local objsSpawned = 0
local obsjToSpawn = 6

local firstWave = false
local secondWave = false
local thirdWave = false


function setup()
    SetMapSprite("assets/encounters/03/map.png")
    SetAutoWin(false);
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);
    CreateObject("assets/enemies/cusith/cusith.lua",120,22,TYPE_ENEMY,5);

    CreateObject("assets/decor/pillar24x24.lua",66,70,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",181,70,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",66,150,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",181,150,TYPE_DECORATION,0);


end

function update(dt)
    do return end;
    --if (_DEBUG) then
      --  if (spawnedBoss == false) then

        --    CreateObject("assets/encounters/02/boss.lua",120,22,TYPE_ENEMY,5);
          --  spawnedBoss = true;
           -- do return end;
      --  end
    --end

    timer = timer + dt
    if (spawnedBoss == false and timer > 60) then
        CreateObject("assets/encounters/02/boss.lua",120,22,TYPE_ENEMY,5);
        spawnedBoss = true
    end

    if (spawnedBoss == false) then
        if (firstWave == false and timer > 15) then
            local enemyObjs = GetAllObjsByFriendliness(TYPE_ENEMY);
            local numObjs = #enemyObjs;
            local ob = CreateObject("assets/enemies/shield_warrior/shield_warrior.lua",120,54,TYPE_ENEMY,2);
            SetObjAggroRadius(ob,999)
        
            ob = CreateObject("assets/enemies/berzerker/berzerker.lua",120,22,TYPE_ENEMY,2);
            SetObjAggroRadius(ob,999)       

            

            firstWave = true;    

        end
        if (secondWave == false and timer > 30) then
            local enemyObjs = GetAllObjsByFriendliness(TYPE_ENEMY);
            local numObjs = #enemyObjs;
            local ob = CreateObject("assets/enemies/berzerker/berzerker.lua",120,22,TYPE_ENEMY,2);
            SetObjAggroRadius(ob,999)
        
            ob = CreateObject("assets/enemies/berzerker/berzerker.lua",120,22,TYPE_ENEMY,2);
            SetObjAggroRadius(ob,999)    
            secondWave = true;    
        end
        if (thirdWave == false and timer > 45) then
            local enemyObjs = GetAllObjsByFriendliness(TYPE_ENEMY);
            local numObjs = #enemyObjs;
            local ob = CreateObject("assets/enemies/shield_warrior/shield_warrior.lua",120,22,TYPE_ENEMY,2);
            SetObjAggroRadius(ob,999)
        
            ob = CreateObject("assets/enemies/berzerker/berzerker.lua",120,22,TYPE_ENEMY,2);
            SetObjAggroRadius(ob,999)
            
            ob = CreateObject("assets/enemies/drummer/drummer.lua",120,22,TYPE_ENEMY,5);
            SetObjAggroRadius(ob,999)       

            thirdWave = true;    
        end


    end

end

function kill()

end