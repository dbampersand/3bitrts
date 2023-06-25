function setup()

    SetSpawnPoint(250,967)
    SetMapSprite("assets/encounters/03/map_8.png")
    SetAggroGroup(CreateObject("assets/enemies/prowler/prowler.lua",261.00,576.00,TYPE_ENEMY,0.00),0)
    SetAggroGroup(CreateObject("assets/enemies/prowler/prowler.lua",246.00,803.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",295.00,432.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",262.00,413.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/web_spinner/web_spinner.lua",440.00,349.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",401.00,320.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",426.00,375.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",393.00,353.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/apprentice_lich/apprentice_lich.lua",334.00,158.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",399.00,185.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",359.00,176.00,TYPE_ENEMY,0.00),4)
    CreateObject("assets/enemies/ghost/ghost.lua",149.00,141.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/scorpion/scorpion.lua",170.00,181.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/shield_warrior/shield_warrior.lua",193.00,146.00,TYPE_ENEMY,0.00)
    PlayMusic("assets/audio/music/encounters/cusith/cusith_encounter_3.wav",0.5,66); 
    SetMapGoldMultiplier(1.6)

end
function update()

end
function mapend()
    GoShop(true)

   ChangeMap("assets/encounters/03/map_final.lua")
end