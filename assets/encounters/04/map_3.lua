function setup()

    SetSpawnPoint(88,324)
    SetMapSprite("assets/encounters/04/map_3.png")
    CreateObject("assets/enemies/staar/staar.lua",119.00,181.00,TYPE_ENEMY,0.00)
end
function update()

end
function mapend()

   ChangeMap("assets/encounters/04/map_3.lua")
end