

function setup()
    SetMapSprite("assets/encounters/04/map_1.png")
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);

    SetSpawnPoint(104,418);

    CreateObject("assets/enemies/ghost/ghost.lua",152.00,313.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/tentacle/tentacle.lua",175.00,164.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/tentacle/tentacle.lua",361.00,195.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/tentacle/tentacle.lua",387.00,157.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/ghost/ghost.lua",207.00,171.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/web_spinner/web_spinner.lua",399.00,328.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/tentacle/tentacle.lua",448.00,382.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/tentacle/tentacle.lua",376.00,395.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/tentacle/tentacle.lua",486.00,321.00,TYPE_ENEMY,0.00),4)
end

function update(dt)
end

function kill()

end

function mapend()
    ChangeMap("assets/encounters/03/map_2.lua")
    return false;
end