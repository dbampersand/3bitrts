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
local abilityCasted1 = false
local abilityCasted2 = false
local abilityCasted3 = false

local selectString1 = "To select a unit, left click and drag a box around the unit.";
local selectString2 = "Click and drag over the unit.";

local moveString = "Right click anywhere to move."

local attackString = "Right click on the enemy here to attack it."

local abilityString = "To cast an ability, press the Q, W, E, or R buttons or click on the abilities at the bottom of the screen."
local abilityString2 = "Some abilities are casted on a target. Use your first ability (Q) on the target dummy."

local abilityString3 = "Other abilities are casted immediately. Use your second ability (W)."
local abilityString4 = "This ability gives you armor, temporarily increasing your health."
local abilityString5 = "Use your second ability (W)."

local abilityString6 = "Certain abilities can also be casted on the ground."
local abilityString7 = "Use your fourth ability (R) and click on the ground to charge."

local abilityString8 = "You can view what your abilities do by mousing over them on the interface."
local abilityString9 = "Select both units by holding left click and dragging around them."



function update()
    SetAbilityCooldownTimer(warrior,0,60);
    SetAbilityCooldownTimer(warrior,1,60);
    SetAbilityCooldownTimer(warrior,2,60);
    SetAbilityCooldownTimer(warrior,3,60);



    if shownIntro == false then
        PushMessage(selectString1,0,110,256,0,false);
        PushMessage(selectString2,80,110,40,50,true);

        shownIntro = true;
    end
    if (unitSelected == false and GetCurrentMessage() == selectString2) then
        if (GetNumberOfUnitsSelected() > 0) then
            unitSelected = true
            ClearCommandQueue(warrior);
            ClearMessages();
            PushMessage(moveString,80,110,40,50,true);
        end
    end
    if (moved == false and GetCurrentMessage() == moveString) then
        if (GetUnitCurrentCommand(warrior) == COMMAND_MOVE) then
           ClearMessages(); 
           moved = true
           ClearCommandQueue(warrior);
           PushMessage(attackString,80,110,40,50,true);
           CreateObject("assets/enemies/trainingdummy/trainingdummy.lua",128,128,TYPE_ENEMY);
        
        end
    end

    if (attacked == false and GetCurrentMessage() == attackString) then
        if (GetUnitCurrentCommand(warrior) == COMMAND_ATTACK) then
           ClearMessages(); 
           attacked = true
           ClearCommandQueue(warrior);
           PushMessage(abilityString,80,110,40,50,false);
           PushMessage(abilityString2,80,110,40,50,true);
        end
    end
    if (attacked == true and abilityCasted1 == false) then
        SetAbilityCooldownTimer(warrior,0,0);
    end

    if (abilityCasted1 == false and GetCurrentMessage() == abilityString2) then
        if (GetUnitCurrentCommand(warrior) == COMMAND_CAST) then
            SetAbilityCooldownTimer(warrior,0,60);
            ClearMessages(); 
            abilityCasted1 = true

            ClearCommandQueue(warrior);
            PushMessage(abilityString3,80,110,40,50,false);
            PushMessage(abilityString4,80,110,40,50,false);
            PushMessage(abilityString5,80,110,40,50,true);
        end
    end
    if (abilityCasted1 == true and abilityCasted2 == false) then
        SetAbilityCooldownTimer(warrior,1,0);
    end
    if (abilityCasted2 == false and GetCurrentMessage() == abilityString5) then
        if (GetShield(warrior) > 0) then
            ClearMessages(); 
            abilityCasted2 = true
            ClearCommandQueue(warrior);
            PushMessage(abilityString6,80,110,40,50,false);
            PushMessage(abilityString7,80,110,40,50,true);
        end
    end
    if (abilityCasted2 == true and abilityCasted3 == false) then
        SetAbilityCooldownTimer(warrior,3,0);
    end

    if (abilityCasted3 == false and GetCurrentMessage() == abilityString7) then
        if (GetUnitCurrentCommand(warrior) == COMMAND_CAST) then
            ClearMessages();
            abilityCasted3 = true;
            PushMessage(abilityString8,80,110,40,50,false);
            PushMessage(abilityString9,80,110,40,50,true);
            CreateObject("assets/friendly/priest/priest.lua",GetX(warrior)+GetW(warrior),GetY(warrior))
            
        end
    end
    if (GetCurrentMessage() == abilityString9) then
        if (GetNumberOfUnitsSelected() == 2) then

        end
    end






end

function kill()

end