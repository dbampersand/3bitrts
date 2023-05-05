function setup()

    SetSpawnPoint(127,368)
    SetMapSprite("assets/encounters/01/map_9.png")
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",96.00,224.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",156.00,221.00,TYPE_ENEMY,0.00),1)
    CreateObject("assets/enemies/giant_worm/giant_worm.lua",126.00,48.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",134.00,197.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",206.00,154.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",225.00,164.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",228.00,270.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",206.00,270.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",222.00,85.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",202.00,62.00,TYPE_ENEMY,0.00),4)
end
function update()

end
function mapend()
    GoShop(true)
   ChangeMap("assets/encounters/01/map_final.lua")
end