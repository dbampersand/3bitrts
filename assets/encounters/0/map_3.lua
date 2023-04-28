
function setup()
    SetMapSprite("assets/encounters/0/map_3.png")

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",46.00,239.00,TYPE_ENEMY,0),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",48.00,269.00,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",71.00,286.00,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",78.00,241.00,TYPE_ENEMY,0),1);

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",304.00,218.00,TYPE_ENEMY,0),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",291.00,249.00,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",271.00,280.00,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",264.00,237.00,TYPE_ENEMY,0),2);


    CreateObject("assets/enemies/kobold_crystal/kobold_crystal.lua",168.00,288.00,TYPE_ENEMY,0);
  
    SetSpawnPoint(167,22)
    
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
    GoShop(true);
    ChangeMap("assets/encounters/0/map_4.lua")
    return false;
end