function setup()

    SetSpawnPoint(87,1055)
    SetMapSprite("assets/encounters/04/map_8.png")
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",120.00,874.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",144.00,872.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",154.00,834.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/sea_witch/sea_witch.lua",229.00,668.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",192.00,954.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/tentacle/tentacle.lua",212.00,531.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/tentacle/tentacle.lua",184.00,570.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/tentacle/tentacle.lua",180.00,510.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/web_spinner/web_spinner.lua",218.00,968.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",129.00,723.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",114.00,749.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",309.00,513.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",286.00,550.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/prowler/prowler.lua",243.00,358.00,TYPE_ENEMY,0.00),6)
    SetAggroGroup(CreateObject("assets/enemies/kelpie_miniboss/kelpie_miniboss.lua",191.00,180.00,TYPE_ENEMY,0.00),7)
    SetAggroGroup(CreateObject("assets/enemies/spitter/spitter.lua",212.00,99.00,TYPE_ENEMY,0.00),7)
    SetAggroGroup(CreateObject("assets/enemies/apprentice_lich/apprentice_lich.lua",158.00,115.00,TYPE_ENEMY,0.00),7)
end
function update()

end
function mapend()
    GoShop(true)

   ChangeMap("assets/encounters/04/map_final.lua")
end