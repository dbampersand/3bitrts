local warrior = 0
local trainingdummy = 0
local doll = 0

local shownIntro = false
local descriptionBox = 0

local unitSelected = false
local moved = false
local attacked = false
local abilityCasted1 = false
local abilityCasted2 = false
local abilityCasted3 = false

local selectedBoth = false

local killedMob = false

local ctrlGroupSet = false

local ctrlGroupRecalled = false



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

local selectedHealerString = "This is a priest class, a basic healer."
local selectedHealerString2 = "When selected, you can switch between the active units by pressing tab."
local selectedHealerString3 = "Defeat the enemy to continue."

local controlGroupMessage = "You can assign control groups to your units to make controlling them easier."
local controlGroupMessage2 = "Holding control, press a number key to assign a group."
local controlGroupMessage3 = "Select a different group of units and assign a second group, holding control and pressing a number key."
local controlGroupMessage4 = "Recall the group you set by pressing the number key without holding control."

local attackString2 = "The circle represents an area of affect attack on your unit."
local attackString3 = "Move out of it before it damages you too much!"


local dotTimer = 0



local textbox = {};

function setup()
    SetMapSprite("assets/encounters/tutorial/map.png")
    
    --CreateObject("assets/encounters/01/boss.lua",32,32,TYPE_ENEMY);
    warrior = CreateObject("assets/friendly/warrior/warrior.lua",128,170,TYPE_FRIENDLY);

    textbox.x = DIALOGUE_X;
    textbox.w = DIALOGUE_W;
    textbox.y = DIALOGUE_Y;
    textbox.h = DIALOGUE_H;

    SetAutoWin(false);

end





function update(dt)
    SetAbilityCooldownTimer(warrior,0,60);
    SetAbilityCooldownTimer(warrior,1,60);
    SetAbilityCooldownTimer(warrior,2,60);
    SetAbilityCooldownTimer(warrior,3,60);



    if shownIntro == false then
        PushMessage(selectString1,textbox.x,textbox.y,textbox.w,textbox.h,false);
        PushMessage(selectString2,textbox.x,textbox.y,textbox.w,textbox.h,true);

        shownIntro = true;
    end
    if (unitSelected == false and GetCurrentMessage() == selectString2) then
        if (GetNumberOfUnitsSelected() > 0) then
            unitSelected = true
            ClearCommandQueue(warrior);
            ClearMessages();
            PushMessage(moveString,textbox.x,textbox.y,textbox.w,textbox.h,true);
        end
    end
    if (moved == false and GetCurrentMessage() == moveString) then
        local targ = GetObjTargetPosition(warrior)
        if (IsNear(warrior,targ.x,targ.y) and  GetUnitCurrentCommand(warrior) == COMMAND_MOVE) then
           ClearMessages(); 
           moved = true
           ClearCommandQueue(warrior);
           HoldCommand(warrior)
           PushMessage(attackString,textbox.x,textbox.y,textbox.w,textbox.h,true);
           trainingdummy = CreateObject("assets/enemies/trainingdummy/trainingdummy.lua",128,128,TYPE_ENEMY);
        
        end
    end

    if (attacked == false and GetCurrentMessage() == attackString) then
        if (GetUnitCurrentCommand(warrior) == COMMAND_ATTACK) then
           ClearMessages(); 
           attacked = true
           ClearCommandQueue(warrior);
           PushMessage(abilityString,textbox.x,textbox.y,textbox.w,textbox.h,false);
           PushMessage(abilityString2,textbox.x,textbox.y,textbox.w,textbox.h,true);
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
            PushMessage(abilityString3,textbox.x,textbox.y,textbox.w,textbox.h,false);
            PushMessage(abilityString4,textbox.x,textbox.y,textbox.w,textbox.h,false);
            PushMessage(abilityString5,textbox.x,textbox.y,textbox.w,textbox.h,true);
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
            PushMessage(abilityString6,textbox.x,textbox.y,textbox.w,textbox.h,false);
            PushMessage(abilityString7,textbox.x,textbox.y,textbox.w,textbox.h,true);
        end
    end
    if (abilityCasted2 == true and abilityCasted3 == false) then
        SetAbilityCooldownTimer(warrior,3,0);
    end

    if (abilityCasted3 == false and GetCurrentMessage() == abilityString7) then
        if (GetUnitCurrentCommand(warrior) == COMMAND_CAST) then
            ClearMessages();
            abilityCasted3 = true;
            PushMessage(abilityString8,textbox.x,textbox.y,textbox.w,textbox.h,false);
            PushMessage(abilityString9,textbox.x,textbox.y,textbox.w,textbox.h,true);
            CreateObject("assets/friendly/priest/priest.lua",GetX(warrior)+GetW(warrior),GetY(warrior))

        end
    end
    if (selectedBoth == false and GetCurrentMessage() == abilityString9) then
        if (GetNumberOfUnitsSelected() == 2) then
            selectedBoth = true
            ClearMessages()
            PushMessage(selectedHealerString,textbox.x,textbox.y,textbox.w,textbox.h,false);
            PushMessage(selectedHealerString2,textbox.x,textbox.y,textbox.w,textbox.h,false);
            PushMessage(selectedHealerString3,textbox.x,textbox.y,textbox.w,textbox.h,true);
            KillObj(trainingdummy);
            doll = CreateObject("assets/encounters/tutorial/boss/doll.lua",128,128,TYPE_ENEMY);
        end
    end

    if (selectedBoth == true and killedMob == false and IsAlive(doll) == false) then
        killedMob = true
        ClearMessages()
        PushMessage(controlGroupMessage,textbox.x,textbox.y,textbox.w,textbox.h,false);
        PushMessage(controlGroupMessage2,textbox.x,textbox.y,textbox.w,textbox.h,true);
    end
    local hasSet = false
    if (killedMob == true and ctrlGroupSet == false) then
        for i=0,10 do
            local ctrlgroup = GetControlGroup(i);
            if #ctrlgroup > 0 then
                hasSet = true;
            end
        end
    end

    if (ctrlGroupSet == false and hasSet == true) then
        if not ((GetKey(KEY_1)) or GetKey(KEY_2)  or GetKey(KEY_3)  or GetKey(KEY_4)  or GetKey(KEY_5)  or GetKey(KEY_6)  or GetKey(KEY_7) or GetKey(KEY_8) or GetKey(KEY_9) or GetKey(KEY_0)) then
            ctrlGroupSet = true;
            ClearMessages()
            PushMessage(controlGroupMessage3,textbox.x,textbox.y,textbox.w,textbox.h,false);
            PushMessage(controlGroupMessage4,textbox.x,textbox.y,textbox.w,textbox.h,true);
        end
    end

    if (ctrlGroupRecalled == false and ctrlGroupSet == true) then
        if (GetKey(KEY_1)) or GetKey(KEY_2)  or GetKey(KEY_3)  or GetKey(KEY_4)  or GetKey(KEY_5)  or GetKey(KEY_6)  or GetKey(KEY_7) or GetKey(KEY_8) or GetKey(KEY_9) or GetKey(KEY_0) then
            ctrlGroupRecalled = true
            ClearMessages();

            f1 = {};
            f1["trigger"] = TRIGGER_TIMER;
            f1["type"] = EFFECT_HURT;
            --f1["numTriggers"] = 5
            f1["value"] = 10;
            f1["duration"] = 1;
            f1["triggersPerSecond"] = 1
        
            CreateAOE(GetX(warrior),GetY(warrior),"", 30, 1, 999999, false, ATTACK_HITS_FRIENDLIES, COLOR_DAMAGE, DITHER_HALF,false,-1, {f1})
            PushMessage(attackString2,textbox.x,textbox.y,textbox.w,textbox.h,false);
            PushMessage(attackString3,textbox.x,textbox.y,textbox.w,textbox.h,true);
            
        end

    end
    if (GetCurrentMessage() == attackString3) then
        dotTimer = dotTimer + dt;
        if (GetNumEffects(warrior) == 0 and dotTimer > 2) then
            Win();
        end
    end
end

function kill()

end