

function setup()
    SetMapSprite("assets/encounters/03/map_3.png")
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);

    SetSpawnPoint(69,451);
end

function update(dt)
end

function kill()

end

function mapend()
    ChangeMap("assets/encounters/03/map_2.lua")
    return false;
end