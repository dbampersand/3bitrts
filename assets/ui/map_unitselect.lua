function setup()
    SetMapSprite("assets/ui/map_unitselect.png")
    SetAggroGroup(CreateObject("assets/enemies/trainingdummy/trainingdummy.lua",71.00,23.00,TYPE_ENEMY,0),0);

    CreateObject("assets/enemies/healingdummy/healingdummy.lua",256-60,24,TYPE_FRIENDLY,0)

    PlayMusic("assets/audio/unitselect.wav",0.5,8);

    SetSpawnPoint(77,126)

    
end

function update()

end 



function mapend()
    GoShop(false)
    return true
end 