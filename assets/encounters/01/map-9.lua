function setup()

    SetSpawnPoint(127,368)
    SetMapSprite("assets/encounters/01/map-9.png")
    CreateObject("assets/enemies/scorpion/scorpion.lua",106.00,220.00,TYPE_ENEMY,0.00)
    CreateObject("assets/enemies/scorpion/scorpion.lua",166.00,223.00,TYPE_ENEMY,0.00)
end
function update()

end
function mapend()

end