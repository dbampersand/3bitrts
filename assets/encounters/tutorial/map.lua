function setup()
    SetMapSprite("assets/encounters/tutorial/map.png")
    
    CreateObject("assets/encounters/01/boss.lua",32,32,TYPE_ENEMY);
    CreateObject("assets/friendly/warrior/warrior.lua",128,170,TYPE_FRIENDLY);

end

local shownIntro = false
local descriptionBox = 0

function update()
    if shownIntro == false then
        PushMessage("To select a unit, left click and drag a box around the unit.",0,110,256,0,false);
        PushMessage("Click and drag here!",80,110,40,50,true);

        shownIntro = true;
    end
    if (GetCurrentMessage() == "Click and drag over the unit.") then
        if (GetNumberOfUnitsSelected() > 0) then
            ClearMessages();
        end
        
    end
end

function kill()

end