

function setup()

    SetMapSprite("assets/encounters/0/map_4.png")

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",119.00,212.00,TYPE_ENEMY,0),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",158.00,219.00,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",194.00,221.00,TYPE_ENEMY,0),1);

    SetAggroGroup(CreateObject("assets/enemies/nuul/nuul.lua",152.00,431.00,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",187.00,434.00,TYPE_ENEMY,0),2);

    SetSpawnPoint(154,82)
    CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",129.00,337.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",208.00,327.00,TYPE_ENEMY,0.00)

    SetMapGoldMultiplier(1.4)

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