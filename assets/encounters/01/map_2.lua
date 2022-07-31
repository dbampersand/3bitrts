function setup()
    SetMapSprite("assets/encounters/01/map.png")
    CreateObject("assets/encounters/01/boss.lua",64,64,TYPE_ENEMY);
    SetAutoWin(true);

end

function update(dt)
end

function kill()

end