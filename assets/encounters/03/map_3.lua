

function setup()
    SetMapSprite("assets/encounters/03/map_1.png")
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);
    CreateObject("assets/enemies/giant_ooze/giant_ooze.lua",120,22,TYPE_ENEMY,5);

    SetSpawnPoint(77,210);
end

function update(dt)
end

function kill()

end

function mapend()
    ChangeMap("assets/encounters/03/map_2.lua")
    return false;
end