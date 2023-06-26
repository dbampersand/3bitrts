function setup()
    SetMapSprite("assets/ui/map_unitselect.png")
    SetAggroGroup(CreateObject("assets/enemies/trainingdummy/trainingdummy.lua",68.00,36.00,TYPE_ENEMY,0),0);

    CreateObject("assets/enemies/healingdummy/healingdummy.lua",196.00,47.00,TYPE_FRIENDLY,0)


    SetSpawnPoint(77,145)

    

end

function update()

end 



function mapend()
    GoShop(false)
    return true
end 