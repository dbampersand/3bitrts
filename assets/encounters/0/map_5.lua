function setup()
    SetMapSprite("assets/encounters/0/map_5.png")

    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",80.00,228.00,TYPE_ENEMY,0),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",100.00,228.00,TYPE_ENEMY,0),1)

    CreateObject("assets/enemies/cannon/cannon.lua",44.00,140.00,TYPE_ENEMY,0);
    CreateObject("assets/enemies/cannon/cannon.lua",46.00,198.00,TYPE_ENEMY,0);

    CreateObject("assets/enemies/cannon/cannon.lua",152,175,TYPE_ENEMY,0);
    CreateObject("assets/enemies/cannon/cannon.lua",152,250,TYPE_ENEMY,0);

    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",80.00,132.00,TYPE_ENEMY,0),2)

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",100.00,160.00,TYPE_ENEMY,0),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",100.00,132.00,TYPE_ENEMY,0),2)

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",119.00,43.00,TYPE_ENEMY,0),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",147.00,53.00,TYPE_ENEMY,0),3)

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",301.00,48.00,TYPE_ENEMY,0),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",308.00,76.00,TYPE_ENEMY,0),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",280.00,120.00,TYPE_ENEMY,0),4)

    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",268.00,64.00,TYPE_ENEMY,0),4)

    SetSpawnPoint(84,481)

    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",404.00,264.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",336.00,216.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",360.00,267.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crusher/kobold_crusher.lua",102.00,200.00,TYPE_ENEMY,0.00),1)

    PlayMusic("assets/audio/music/encounters/kobold/kobold_encounter_3.wav",0.5,0); 

end

function update(dt)

end

function kill()



end

function mapend()
    ChangeMap("assets/encounters/0/map_6.lua")
    return false;
end