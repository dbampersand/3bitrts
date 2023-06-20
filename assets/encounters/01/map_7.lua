function setup()

    SetMapSprite("assets/encounters/01/map_7.png")
    SetSpawnPoint(208,409)
    SetAggroGroup(CreateObject("assets/enemies/spitter/spitter.lua",86.00,201.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",132.00,232.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",139.00,200.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",166.00,52.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",190.00,46.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crusher/kobold_crusher.lua",188.00,70.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",252.00,188.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",300.00,208.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",307.00,174.00,TYPE_ENEMY,0.00),3)
    --PlayMusic("assets/audio/music/encounters/wyrm/wyrm_encounter_3.wav",0.5,5.14); 
end
function update()

end
function mapend()

   ChangeMap("assets/encounters/01/map_8.lua")
end