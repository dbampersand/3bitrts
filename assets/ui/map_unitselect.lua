function setup()
    SetMapSprite("assets/ui/map_unitselect.png")
    SetAggroGroup(CreateObject("assets/enemies/trainingdummy/trainingdummy.lua",88.00,36.00,TYPE_ENEMY,0),0);

    CreateObject("assets/enemies/healingdummy/healingdummy.lua",168.00,36.00,TYPE_FRIENDLY,0)


    SetSpawnPoint(77,145)

    

    CreateObject("assets/enemies/sea_goblin/sea_goblin.lua",220.00,134.00,TYPE_ENEMY,0.00)
end

function update()

end 



function mapend()
    GoShop(false)
    return true
end 