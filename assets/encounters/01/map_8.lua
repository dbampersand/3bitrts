function setup()

    SetMapSprite("assets/encounters/01/map_8.png")
    SetAggroGroup(CreateObject("assets/enemies/web_spinner/web_spinner.lua",320.00,123.00,TYPE_ENEMY,0.00),1)
    CreateObject("assets/enemies/web_spinner/web_spinner.lua",172.00,287.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/web_spinner/web_spinner.lua",376.00,339.00,TYPE_ENEMY,0.00)
    SetSpawnPoint(270,514)
    SetAggroGroup(CreateObject("assets/enemies/web_spinner/web_spinner.lua",220.00,79.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",272.00,347.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",224.00,355.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",302.00,389.00,TYPE_ENEMY,0.00),2)
end
function update()

end
function mapend()

   ChangeMap("assets/encounters/01/map_final.lua")
    GoShop(true)
end