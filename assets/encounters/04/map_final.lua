

function setup()
    SetMapSprite("assets/encounters/04/map_final.png")
        --CreateObject("assets/encounters/02/boss.lua",32,32,TYPE_ENEMY);



    SetSpawnPoint(152,221)
    SetEncounterMusic("assets/audio/music/boss/cusith/cusith.wav");
    SetMapGoldMultiplier(20)

    CreateObject("assets/enemies/stoor_worm/stoor_worm.lua",0,0,TYPE_ENEMY,0.00)
end

function update(dt)
end

function kill()

end
function mapend()end
