function setup()
    SetMapSprite("assets/encounters/0/map_3.png")

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",81.00,315.00,TYPE_ENEMY,0),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",86.00,341.00,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",104.00,352.00,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",116.00,323.00,TYPE_ENEMY,0),1);

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",293.00,315.00,TYPE_ENEMY,0),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",278.00,335.00,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",258.00,355.00,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",258.00,322.00,TYPE_ENEMY,0),2);

    CreateObject("assets/enemies/kobold_crystal/kobold_crystal.lua",171.00,404.00,TYPE_ENEMY,0);

    --PlayMusic("assets/audio/music/encounters/kobold/kobold_encounter.wav",0.5,12); 
    
    SetSpawnPoint(172,88)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",222.00,257.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",183.00,263.00,TYPE_ENEMY,0.00),3)
    SetMapGoldMultiplier(1.2)

end

function update(dt)
end

function kill()
end
function mapend()
    GoShop(true);
    ChangeMap("assets/encounters/0/map_4.lua")
    return false;
end