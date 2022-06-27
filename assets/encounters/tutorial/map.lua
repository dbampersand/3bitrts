local warrior = 0

function setup()
    SetMapSprite("assets/encounters/tutorial/map.png")
    
    --CreateObject("assets/encounters/01/boss.lua",32,32,TYPE_ENEMY);
    warrior = CreateObject("assets/friendly/warrior/warrior.lua",128,170,TYPE_FRIENDLY);
    SetAutoWin(false);
end

local shownIntro = false
local descriptionBox = 0

local unitSelected = false
local moved = false
local attacked = false

local selectString1 = "To select a unit, left click and drag a box around the unit.";
local selectString2 = "Click and drag over the unit.";

local moveString = "Right click anywhere to move."

local attackString = "Right click on the enemy here to attack it."


function update()
    if shownIntro == false then
        PushMessage(selectString1,0,110,256,0,false);
        PushMessage(selectString2,80,110,40,50,true);

        shownIntro = true;
    end
    if (unitSelected == false and GetCurrentMessage() == selectString2) then
        if (GetNumberOfUnitsSelected() > 0) then
            unitSelected = true
            ClearMessages();
            PushMessage(moveString,80,110,40,50,true);
        end
    end
    if (moved == false and GetCurrentMessage() == moveString) then
        if (GetUnitCurrentCommand(warrior) == COMMAND_MOVE) then
           ClearMessages(); 
           moved = true
           PushMessage(attackString,80,110,40,50,true);
           CreateObject("assets/enemies/trainingdummy/trainingdummy.lua",128,128,TYPE_ENEMY);

        end
    end

    if (attacked == false and GetCurrentMessage() == attackString) then
        if (GetUnitCurrentCommand(warrior) == COMMAND_ATTACK) then
           ClearMessages(); 
           attacked = true
           Win();
        end
    end


end

function kill()

end