function setup()
    SetMapSprite("assets/encounters/0/map_5.png")

    CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",86.00,237.00,TYPE_ENEMY,0)
    CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",108.00,239.00,TYPE_ENEMY,0)

    CreateObject("assets/enemies/cannon/cannon.lua",44.00,140.00,TYPE_ENEMY,0);
    CreateObject("assets/enemies/cannon/cannon.lua",46.00,198.00,TYPE_ENEMY,0);

    CreateObject("assets/enemies/cannon/cannon.lua",152,175,TYPE_ENEMY,0);
    CreateObject("assets/enemies/cannon/cannon.lua",152,250,TYPE_ENEMY,0);

    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",87.00,172.00,TYPE_ENEMY,0)
    CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",84.00,143.00,TYPE_ENEMY,0)

    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",107.00,171.00,TYPE_ENEMY,0)
    CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",108.00,142.00,TYPE_ENEMY,0)

    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",121.00,40.00,TYPE_ENEMY,0)
    CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",144.00,56.00,TYPE_ENEMY,0)

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",301.00,48.00,TYPE_ENEMY,0),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",316.00,84.00,TYPE_ENEMY,0),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",288.00,128.00,TYPE_ENEMY,0),4)

    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",277.00,72.00,TYPE_ENEMY,0),4)

    SetSpawnPoint(81,484)

    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",404.00,264.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",344.00,227.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",360.00,267.00,TYPE_ENEMY,0.00),5)
end

function update(dt)

end

function kill()



end

function mapend()
    ChangeMap("assets/encounters/0/map_final.lua")
    return false;
end