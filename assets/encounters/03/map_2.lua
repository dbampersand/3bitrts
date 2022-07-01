function setup()
    SetMapSprite("assets/encounters/03/map_2.png")
    CreateObject("assets/encounters/03/boss.lua",84,40,TYPE_ENEMY);
    SetAutoWin(true);
end

function update(dt)
    if (NumObjectsOwnedByPlayer(TYPE_ENEMY) == 0) then
        ChangeMap("assets/encounters/03/map_02.lua")
    end
end

function kill()

end