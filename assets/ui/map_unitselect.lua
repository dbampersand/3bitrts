function setup()
    SetMapSprite("assets/ui/map_unitselect.png")

    CreateObject("assets/enemies/trainingdummy/trainingdummy.lua",49.00,11.00,TYPE_ENEMY,0);

    CreateObject("assets/enemies/healingdummy/healingdummy.lua",256-60,24,TYPE_FRIENDLY,0)

    PlayMusic("assets/audio/unitselect.wav",0.5,8);

    SetSpawnPoint(77,126)

    CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",196.00,136.00,TYPE_ENEMY,0.00)

end

function update()

end 



function mapend()

end 