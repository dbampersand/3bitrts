function setup()

    SetSpawnPoint(127,368)
    SetMapSprite("assets/encounters/01/map_9.png")
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",124.00,232.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",169.00,221.00,TYPE_ENEMY,0.00),1)
    CreateObject("assets/enemies/giant_worm/giant_worm.lua",164.00,60.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",134.00,197.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",206.00,154.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",225.00,164.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",220.00,280.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",195.00,274.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",222.00,85.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",202.00,62.00,TYPE_ENEMY,0.00),4)
    --PlayMusic("assets/audio/music/encounters/wyrm/wyrm_encounter_3.wav",0.5,5.14); 
    SetMapGoldMultiplier(1.6)

end
function update()

end
function mapend()
    GoShop(true)
   ChangeMap("assets/encounters/01/map_final.lua")
end