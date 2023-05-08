function setup()

    SetSpawnPoint(140,501)
    SetMapSprite("assets/encounters/03/map_6.png")
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",126.00,374.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",162.00,362.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",118.00,330.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",148.00,228.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",112.00,228.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",133.00,211.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",160.00,112.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/web_spinner/web_spinner.lua",195.00,134.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/wolf/wolf.lua",406.00,289.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/wolf/wolf.lua",358.00,292.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/wolf/wolf.lua",336.00,273.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",379.00,264.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/spitter/spitter.lua",510.00,159.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/spitter/spitter.lua",469.00,144.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",461.00,170.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",485.00,182.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",438.00,142.00,TYPE_ENEMY,0.00),5)
    CreateObject("assets/enemies/web_spinner/web_spinner.lua",482.00,249.00,TYPE_ENEMY,0.00)
end
function update()

end
function mapend()

end