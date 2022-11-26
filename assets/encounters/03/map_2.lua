

function setup()
    SetMapSprite("assets/encounters/03/map_2.png")
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);
    CreateObject("assets/enemies/prowler/prowler.lua",140,97,TYPE_ENEMY,5);

    SetSpawnPoint(133,280);
end

function update(dt)
end

function kill()

end

function mapend()
    ChangeMap("assets/encounters/03/map_final.lua")
    return false;
end