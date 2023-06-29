

function setup()
    SetMapSprite("assets/encounters/03/map_final.png")
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);
    CreateObject("assets/enemies/cusith/cusith.lua",164.00,92.00,TYPE_ENEMY,5);

    CreateObject("assets/decor/pillar24x24.lua",108.00,97.00,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",229.00,91.00,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",108.00,206.00,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",235.00,205.00,TYPE_DECORATION,0);


    SetSpawnPoint(152,221)
    SetEncounterMusic("assets/audio/music/boss/cusith/cusith.wav");
    SetMapGoldMultiplier(20)

end

function update(dt)
end

function kill()

end
function mapend()end
