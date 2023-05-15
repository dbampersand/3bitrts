function setup()

    SetMapSprite("assets/encounters/0/map_6.png")
    SetSpawnPoint(72,337)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",93.00,138.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",156.00,148.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crusher/kobold_crusher.lua",124.00,100.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",353.00,119.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",319.00,132.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",350.00,166.00,TYPE_ENEMY,0.00),2)
    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",280.00,52.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",248.00,44.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",253.00,313.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",225.00,300.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",134.00,255.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",175.00,238.00,TYPE_ENEMY,0.00),4)
end
function update()

end
function mapend()

   ChangeMap("assets/encounters/0/map_7.lua")
end