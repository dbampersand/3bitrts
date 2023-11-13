function setup()

    SetSpawnPoint(174,613)
    SetMapSprite("assets/encounters/04/map_2.png")
    SetAggroGroup(CreateObject("assets/enemies/kelpie_miniboss/kelpie_miniboss.lua",252.00,136.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/spriggan/spriggan.lua",132.00,480.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/spriggan/spriggan.lua",192.00,468.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",144.00,306.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",191.00,301.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/tentacle_healer/tentacle_healer.lua",160.00,259.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/tentacle_healer/tentacle_healer.lua",184.00,237.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/tentacle_healer/tentacle_healer.lua",248.00,97.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/tentacle_healer/tentacle_healer.lua",211.00,124.00,TYPE_ENEMY,0.00),3)
end
function update()

end
function mapend()

end