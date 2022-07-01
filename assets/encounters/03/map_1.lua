function setup()
    SetMapSprite("assets/encounters/03/map_1.png")
    CreateObject("assets/enemies/wyrm_boss/wyrm_add/wyrm_add.lua",115,57,TYPE_ENEMY);
    SetAutoWin(false);
end

function update(dt)
    
    if (NumObjectsOwnedByPlayer(TYPE_ENEMY) == 0) then
        ChangeMap("assets/encounters/03/map_2.lua")
    end
end

function kill()

end