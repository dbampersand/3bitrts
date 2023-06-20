function setup()

    SetMapSprite("assets/encounters/0/map_7.png")
    CreateObject("assets/enemies/wraith/wraith.lua",200.00,160.00,TYPE_ENEMY,0.00)
    SetSpawnPoint(143,833)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crusher/kobold_crusher.lua",168.00,699.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",188.00,659.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",151.00,564.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",224.00,564.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",164.00,388.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",228.00,384.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",200.00,368.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",288.00,412.00,TYPE_ENEMY,0.00),0)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",96.00,460.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",94.00,552.00,TYPE_ENEMY,0.00),2)
    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",152.00,288.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",240.00,304.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",158.00,467.00,TYPE_ENEMY,0.00),3)

    --PlayMusic("assets/audio/music/encounters/kobold/kobold_encounter_3.wav",0.5,0); 

    SetMapGoldMultiplier(1.6)

end
function update()

end
function mapend()

   ChangeMap("assets/encounters/0/map_8.lua")
end