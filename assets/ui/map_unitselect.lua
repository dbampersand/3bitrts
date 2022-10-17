function setup()
    SetMapSprite("assets/ui/map_unitselect.png")
    CreateObject("assets/enemies/trainingdummy/trainingdummy.lua",60,16,TYPE_ENEMY,0);
    CreateObject("assets/enemies/healingdummy/healingdummy.lua",256-60,16,TYPE_FRIENDLY,0);
    CreateObject("assets/enemies/wadjet/wadjet.lua",128,128,TYPE_ENEMY,0);

end