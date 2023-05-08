function setup()

    SetSpawnPoint(167,523)
    SetMapSprite("assets/encounters/03/map_6.png")
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",128.00,374.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",164.00,362.00,TYPE_ENEMY,0.00),1)
    CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",120.00,330.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",149.00,231.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",113.00,228.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",124.00,194.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/viper/viper.lua",160.00,106.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/web_spinner/web_spinner.lua",180.00,114.00,TYPE_ENEMY,0.00)
end
function update()

end
function mapend()

end