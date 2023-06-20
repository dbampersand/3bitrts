local timer = 0
local spawnedBoss = false

local objsSpawned = 0
local obsjToSpawn = 6

local firstWave = false
local secondWave = false
local thirdWave = false
local fourthWave = false
local fifthWave= false
local sixthWave = false
local seventhWave = false

local debug = true;

function setup()
    SetMapSprite("assets/encounters/02/map.png")
    
    --if (_DEBUG == false) then
      --  local ob = CreateObject("assets/enemies/shield_warrior/shield_warrior.lua",120,54,TYPE_ENEMY,2,0);
        --SetObjAggroRadius(ob,999)

       -- ob = CreateObject("assets/enemies/berzerker/berzerker.lua",120,22,TYPE_ENEMY,2,0);
       -- SetObjAggroRadius(ob,999)


    --end
    SetAutoWin(false);
    --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);
    --PlayMusic("assets/audio/music/encounters/minotaur/minotaur_encounter.wav",0.5,6.40); 

end

function update(dt)
    if (debug) then
       if (spawnedBoss == false) then

            local boss = CreateObject("assets/enemies/minotaur/minotaur.lua",120,22,TYPE_ENEMY,5,100);
            spawnedBoss = true;
        PlayMusic("assets/audio/music/boss/minotaur/minotaur.wav",0.5,0); 

            do return end;
        end
    end

    timer = timer + dt

    local numEnemies = GetAllObjsByFriendliness(TYPE_ENEMY);

    if (spawnedBoss == false and seventhWave == true and #numEnemies == 0) then
        local boss = CreateObject("assets/enemies/minotaur/minotaur.lua",120,22,TYPE_ENEMY,5,100);
        spawnedBoss = true
    end

    if (spawnedBoss == false) then
        if (firstWave == false and timer > 2) then
            local enemyObjs = GetAllObjsByFriendliness(TYPE_ENEMY);
            local numObjs = #enemyObjs;
            local ob = CreateObject("assets/enemies/shield_warrior/shield_warrior.lua",120,54,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)
        
            ob = CreateObject("assets/enemies/berzerker/berzerker.lua",120,22,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)       

            local ob = CreateObject("assets/enemies/shield_warrior/shield_warrior.lua",36,93,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)
        
            ob = CreateObject("assets/enemies/berzerker/berzerker.lua",36,120,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)       


            

            firstWave = true;    
            do return end;


        end
        if (secondWave == false and firstWave == true and #numEnemies == 0) then
            local enemyObjs = GetAllObjsByFriendliness(TYPE_ENEMY);
            local numObjs = #enemyObjs;
            local ob = CreateObject("assets/enemies/berzerker/berzerker.lua",120,22,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)
        
            ob = CreateObject("assets/enemies/berzerker/berzerker.lua",120,22,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)    

            ob = CreateObject("assets/enemies/berzerker/berzerker.lua",30,115,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)    

            ob = CreateObject("assets/enemies/berzerker/berzerker.lua",228,119,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)    

            ob = CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",121,238,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)


            secondWave = true;    
            do return end;
        end
        if (thirdWave == false and secondWave == true and #numEnemies == 0) then
            local enemyObjs = GetAllObjsByFriendliness(TYPE_ENEMY);
            local numObjs = #enemyObjs;
            local ob = CreateObject("assets/enemies/shield_warrior/shield_warrior.lua",120,22,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)
        
            ob = CreateObject("assets/enemies/drummer/drummer.lua",120,22,TYPE_ENEMY,5,0);
            SetObjAggroRadius(ob,999)       

            ob = CreateObject("assets/enemies/berzerker/berzerker.lua",45,114,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)

            ob = CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",144,238,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)



            thirdWave = true;    
            do return end;

        end

        if (thirdWave == false and secondWave == true and #numEnemies == 0) then
            local enemyObjs = GetAllObjsByFriendliness(TYPE_ENEMY);
            local numObjs = #enemyObjs;
            local ob = CreateObject("assets/enemies/shield_warrior/shield_warrior.lua",120,22,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)
        
            ob = CreateObject("assets/enemies/berzerker/berzerker.lua",120,22,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)
            
            ob = CreateObject("assets/enemies/drummer/drummer.lua",120,22,TYPE_ENEMY,5,0);
            SetObjAggroRadius(ob,999)   
            
            ob = CreateObject("assets/enemies/shield_warrior/shield_warrior.lua",230,108,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)
            

            thirdWave = true;    
            do return end;

        end

        if ((fourthWave == false and thirdWave == true and #numEnemies == 0)) then
            local enemyObjs = GetAllObjsByFriendliness(TYPE_ENEMY);
            local numObjs = #enemyObjs;
            local ob = CreateObject("assets/enemies/shield_warrior/shield_warrior.lua",120,22,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)
            
            ob = CreateObject("assets/enemies/drummer/drummer.lua",120,22,TYPE_ENEMY,5,0);
            SetObjAggroRadius(ob,999)       

            ob = CreateObject("assets/enemies/firebreather/firebreather.lua",250,108,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)

            ob = CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",144,238,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)


            fourthWave = true;    
            do return end;

        end
        if ((fifthWave == false and fourthWave == true and #numEnemies == 0)) then
            local enemyObjs = GetAllObjsByFriendliness(TYPE_ENEMY);
            local numObjs = #enemyObjs;
            local ob = CreateObject("assets/enemies/shield_warrior/shield_warrior.lua",120,22,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)
            
            ob = CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",120,22,TYPE_ENEMY,5,0);
            SetObjAggroRadius(ob,999)       

            ob = CreateObject("assets/enemies/scorpion/scorpion.lua",250,108,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)

            ob = CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",144,238,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)


            fifthWave = true;    
            do return end;

        end
        if ((sixthWave == false and fourthWave == true and #numEnemies == 0)) then
            local enemyObjs = GetAllObjsByFriendliness(TYPE_ENEMY);
            local numObjs = #enemyObjs;
            local ob = CreateObject("assets/enemies/shield_warrior/shield_warrior.lua",80,33,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)
            
            ob = CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",100,33,TYPE_ENEMY,5,0);
            SetObjAggroRadius(ob,999)       

            ob = CreateObject("assets/enemies/scorpion/scorpion.lua",120,33,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)

            ob = CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",140,33,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)


            sixthWave = true;    
            do return end;

        end
        if ((seventhWave == false and fourthWave == true and #numEnemies == 0)) then
            local enemyObjs = GetAllObjsByFriendliness(TYPE_ENEMY);
            local numObjs = #enemyObjs;
            local ob = CreateObject("assets/enemies/shield_warrior/shield_warrior.lua",228,110,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)
            
            ob = CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",228,140,TYPE_ENEMY,5,0);
            SetObjAggroRadius(ob,999)       

            ob = CreateObject("assets/enemies/scorpion/scorpion.lua",12,110,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)
            
            ob = CreateObject("assets/enemies/scorpion/scorpion.lua",16,140,TYPE_ENEMY,2,0);
            SetObjAggroRadius(ob,999)


            seventhWave = true;    
            do return end;

        end



    end

end

function kill()

end
function mapend()
    UnlockEncounter("assets/encounters/03")
    return true;
end