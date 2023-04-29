

function setup()

    SetMapSprite("assets/encounters/0/map_3.png")

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",133.00,169.00,TYPE_ENEMY,0),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",186.00,181.00,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",235.00,170.00,TYPE_ENEMY,0),1);

    SetAggroGroup(CreateObject("assets/enemies/nuul/nuul.lua",164.00,278.00,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",205.00,303.00,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",130.00,301.00,TYPE_ENEMY,0),2);

    SetSpawnPoint(180,34)
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