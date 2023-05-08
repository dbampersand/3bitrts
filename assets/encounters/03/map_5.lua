

function setup()
    SetMapSprite("assets/encounters/03/map_5.png")
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);
    CreateObject("assets/enemies/giant_ooze/giant_ooze.lua",132.00,270.00,TYPE_ENEMY,5);

    SetSpawnPoint(77,428);

    CreateObject("assets/decor/pillar24x24.lua",78.00,248.00,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",164.00,327.00,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",98.00,227.00,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",185.00,305.00,TYPE_DECORATION,0);

end

function update(dt)
end

function kill()

end

function mapend()
    ChangeMap("assets/encounters/03/map_6.lua")
    return false;
end