function setup()

    SetSpawnPoint(103,840)
    SetMapSprite("assets/encounters/0/map_8.png")
    SetAggroGroup(CreateObject("assets/enemies/royal_guard/royal_guard.lua",158.00,166.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/royal_guard/royal_guard.lua",98.00,168.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",179.00,257.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",82.00,263.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",162.00,364.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",101.00,364.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",67.00,506.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",68.00,529.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",71.00,471.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",187.00,492.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",190.00,513.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",192.00,535.00,TYPE_ENEMY,0.00),5)
    CreateObject("assets/enemies/scorpion/scorpion.lua",49.00,586.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/scorpion/scorpion.lua",211.00,654.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crusher/kobold_crusher.lua",60.00,672.00,TYPE_ENEMY,0.00),6)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",59.00,713.00,TYPE_ENEMY,0.00),6)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",69.00,691.00,TYPE_ENEMY,0.00),6)

    PlayMusic("assets/audio/music/encounters/kobold/kobold_encounter_3.wav",0.5,0); 

end
function update()

end
function mapend()
    GoShop(true)

   ChangeMap("assets/encounters/0/map_final.lua")
end