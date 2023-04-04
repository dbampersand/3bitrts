
function setup()
    SetMapSprite("assets/encounters/0/map_3.png")

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",140,207,TYPE_ENEMY,0),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",160,207,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",180,207,TYPE_ENEMY,0),1);
    
    SetAggroGroup(CreateObject("assets/enemies/nuul/nuul.lua",175,313,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",225,360,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",125,360,TYPE_ENEMY,0),2);

  
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
    ChangeMap("assets/encounters/0/map_5.lua")
    return false;
end