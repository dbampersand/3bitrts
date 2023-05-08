function setup()
    SetMapSprite("assets/ui/map_unitselect.png")
    SetAggroGroup(CreateObject("assets/enemies/trainingdummy/trainingdummy.lua",52.00,4.00,TYPE_ENEMY,0),0);

    CreateObject("assets/enemies/healingdummy/healingdummy.lua",256-60,24,TYPE_FRIENDLY,0)

    PlayMusic("assets/audio/unitselect.wav",0.5,8);

    SetSpawnPoint(77,126)

    CreateObject("assets/decor/pillar24x24.lua",153.00,142.00,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",153.00,118.00,TYPE_DECORATION,0);
    
    CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",220.00,137.00,TYPE_ENEMY,0.00)
end

function update()

end 



function mapend()
    GoShop(false)
    return true
end 