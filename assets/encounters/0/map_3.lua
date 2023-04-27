
function setup()
    SetMapSprite("assets/encounters/0/map_3.png")

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",30.00,239.00,TYPE_ENEMY,0),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",40.00,266.00,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",63.00,276.00,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",69.00,241.00,TYPE_ENEMY,0),1);

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",226.00,231.00,TYPE_ENEMY,0),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",215.00,255.00,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",194.00,272.00,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",196.00,229.00,TYPE_ENEMY,0),2);


    CreateObject("assets/enemies/kobold_crystal/kobold_crystal.lua",132.00,296.00,TYPE_ENEMY,0);
  
    SetSpawnPoint(135,31)
    
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