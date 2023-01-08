function setup()
    SetMapSprite("assets/ui/map_unitselect.png")
    CreateObject("assets/enemies/trainingdummy/trainingdummy.lua",60,24,TYPE_ENEMY,0);
    CreateObject("assets/enemies/healingdummy/healingdummy.lua",256-60,24,TYPE_FRIENDLY,0);


    CreateObject("assets/enemies/explosive_bot/explosive_bot.lua",256,256,TYPE_ENEMY,0);

end
function update()

end