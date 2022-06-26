function setup()
    SetMapSprite("assets/encounters/tutorial/map.png")
    
    CreateObject("assets/encounters/01/boss.lua",32,32,TYPE_ENEMY);
    CreateObject("assets/friendly/warrior/warrior.lua",128,200,TYPE_FRIENDLY);

end

local shownIntro = false

function update()
    if shownIntro == false then
        PushMessage("gg")
        PushMessage("asdasf")
        shownIntro = true;

    end
end

function kill()

end