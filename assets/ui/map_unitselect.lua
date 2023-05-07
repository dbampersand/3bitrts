function setup()
    SetMapSprite("assets/ui/map_unitselect.png")
    CreateObject("assets/enemies/trainingdummy/trainingdummy.lua",60.00,12.00,TYPE_ENEMY,0);

    CreateObject("assets/enemies/healingdummy/healingdummy.lua",256-60,24,TYPE_FRIENDLY,0)

    PlayMusic("assets/audio/unitselect.wav",0.5,8);

    SetSpawnPoint(77,126)


    CreateObject("assets/enemies/apprentice_lich/apprentice_lich.lua",222.00,134.00,TYPE_ENEMY,0.00)
end

function update()

end 



function mapend()
    GoShop(false)
    return true
end 