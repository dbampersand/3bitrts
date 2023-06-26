function setup()

    SetMapSprite("assets/encounters/0/map_6.png")
    SetSpawnPoint(72,387)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",92.00,186.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",156.00,198.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crusher/kobold_crusher.lua",122.00,148.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",352.00,166.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",316.00,190.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",348.00,214.00,TYPE_ENEMY,0.00),2)
    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",280.00,102.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",248.00,94.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",252.00,362.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",224.00,350.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",132.00,302.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",172.00,286.00,TYPE_ENEMY,0.00),4)
    

    SetMapGoldMultiplier(1.6)

end
function update()

end
function mapend()

   ChangeMap("assets/encounters/0/map_7.lua")
end