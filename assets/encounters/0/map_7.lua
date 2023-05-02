function setup()

    SetMapSprite("assets/encounters/0/map_7.png")
    CreateObject("assets/enemies/wraith/wraith.lua",200.00,163.00,TYPE_ENEMY,0.00)
    SetSpawnPoint(143,833)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crusher/kobold_crusher.lua",168.00,699.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",188.00,659.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",148.00,559.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",224.00,567.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",164.00,391.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",228.00,387.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",200.00,371.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",288.00,415.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",76.00,427.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",76.00,515.00,TYPE_ENEMY,0.00),2)
    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",144.00,291.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",240.00,307.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",192.00,459.00,TYPE_ENEMY,0.00)
end
function update()

end
function mapend()

   ChangeMap("assets/encounters/0/map_final.lua")
end