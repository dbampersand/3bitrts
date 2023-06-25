

function setup()
    SetMapSprite("assets/encounters/03/map_final.png")
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);
    CreateObject("assets/enemies/cusith/cusith.lua",166.00,93.00,TYPE_ENEMY,5);

    CreateObject("assets/decor/pillar24x24.lua",110.00,109.00,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",226.00,109.00,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",110.00,189.00,TYPE_DECORATION,0);
    CreateObject("assets/decor/pillar24x24.lua",226.00,189.00,TYPE_DECORATION,0);


    SetSpawnPoint(152,221)
    SetEncounterMusic("assets/audio/music/boss/cusith/cusith.wav");
    SetMapGoldMultiplier(20)

end

function update(dt)
end

function kill()

end
function mapend()end
