
function setup()
    SetMapSprite("assets/encounters/0/map_5.png")

    --SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",180,207,TYPE_ENEMY,0),1);
    
    CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",110,240,TYPE_ENEMY,0)
    CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",117,250,TYPE_ENEMY,0)

    CreateObject("assets/enemies/cannon/cannon.lua",54,50,TYPE_ENEMY,0);
    CreateObject("assets/enemies/cannon/cannon.lua",54,100,TYPE_ENEMY,0);
    CreateObject("assets/enemies/cannon/cannon.lua",54,150,TYPE_ENEMY,0);
    CreateObject("assets/enemies/cannon/cannon.lua",54,200,TYPE_ENEMY,0);
    CreateObject("assets/enemies/cannon/cannon.lua",54,250,TYPE_ENEMY,0);

    CreateObject("assets/enemies/cannon/cannon.lua",152,175,TYPE_ENEMY,0);
    CreateObject("assets/enemies/cannon/cannon.lua",152,215,TYPE_ENEMY,0);
    CreateObject("assets/enemies/cannon/cannon.lua",152,250,TYPE_ENEMY,0);

    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",105,173,TYPE_ENEMY,0)
    CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",101,160,TYPE_ENEMY,0)


    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",105,173,TYPE_ENEMY,0)
    CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",101,160,TYPE_ENEMY,0)

    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",139,20,TYPE_ENEMY,0)
    CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",138,37,TYPE_ENEMY,0)

    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",309,31,TYPE_ENEMY,0)
    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",326,93,TYPE_ENEMY,0)
    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",323,130,TYPE_ENEMY,0)

    CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",286,82,TYPE_ENEMY,0)

    SetSpawnPoint(86,497)
    
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