function setup()

    SetMapSprite("assets/encounters/01/map_5.png")
    SetSpawnPoint(80,699)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crusher/kobold_crusher.lua",116.00,568.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crusher/kobold_crusher.lua",72.00,568.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",132.00,500.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",124.00,452.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",68.00,376.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",88.00,368.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",133.00,191.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",92.00,476.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",184.00,256.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",106.00,198.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",88.00,216.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/spitter/spitter.lua",58.00,40.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/spitter/spitter.lua",160.00,45.00,TYPE_ENEMY,0.00),5)
    CreateObject("assets/enemies/spitter/spitter.lua",137.00,354.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",250.00,89.00,TYPE_ENEMY,0.00),5)
    SetMapGoldMultiplier(1.5)

end
function update()

end
function mapend()

   ChangeMap("assets/encounters/01/map_6.lua")
end