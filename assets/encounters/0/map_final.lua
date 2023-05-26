function setup()
    SetMapSprite("assets/encounters/0/map_final.png")
    SetEncounterMusic("assets/audio/music/boss/kobold_king.wav");

    CreateObject("assets/enemies/kobold_king/kobold_king.lua",256,0,TYPE_ENEMY,0);

end
local timer = 10;
local numAoesMade = 0;
function update(dt)


end

function kill()

end

function mapend()
    UnlockEncounter("assets/encounters/01");
    return true;
end