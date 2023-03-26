
function setup()
    SetMapSprite("assets/encounters/0/map_3.png")

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",40,217,TYPE_ENEMY,0),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",40,233,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",40,249,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",58,233,TYPE_ENEMY,0),1);

    CreateObject("assets/enemies/kobold_crystal/kobold_crystal.lua",164,304,TYPE_ENEMY,0);
  
    SetSpawnPoint(161,28)
    
end

function update(dt)
    --ChangeMap("assets/encounters/01/map_2.lua")
        
    --if (NumObjectsOwnedByPlayer(TYPE_ENEMY) == 0) then
      --  ChangeMap("assets/encounters/01/map_final.lua")
    --end

end

function kill()

end

function mapend()
    ChangeMap("assets/encounters/0/map_final.lua")
    return false;
end