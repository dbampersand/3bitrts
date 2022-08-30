local maxHP = 9999;

function setup()
    SetSprite("assets/enemies/trainingdummy/trainingdummy.png");

    SetDamage(0);
    SetMaxHP(maxHP,true)
    SetSpeed(0)
    SetObjectPush(true);
end

local timer = 0
local timerActivated = false
local lastTenSeconds = "0.00";
function update(dt)
    if (GetHP() < maxHP) then
        timerActivated = true;
    end
    if (timerActivated == true) then
        timer = timer + dt;
    end

    if (timer > 10) then
        timerActivated = false;
        timer = 0;
        lastTenSeconds = ""
        --lastTenSeconds = lastTenSeconds .. (maxHP - GetHP())
        lastTenSeconds = string.format("%.2f",(maxHP - GetHP()));
        SetHP(maxHP);
    end

    ShowString(lastTenSeconds,6,6);

    ClearCommandQueue(GetObjRef());
end

function kill()

end