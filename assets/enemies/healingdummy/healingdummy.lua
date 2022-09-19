local maxHP = 9999;

function setup()
    SetSprite("assets/enemies/healingdummy/healingdummy.png");

    SetDamage(0);
    SetMaxHP(maxHP,true)
    SetHPIgnoreAugment(math.floor(maxHP/2))

    SetSpeed(0)
    SetObjectPush(true);
    ObjIsStunnable(GetObjRef(),false);

end

local timer = 4
local totalTimeInCombat = 0;
local timerActivated = false
local dpsStr = "HPS: 0.00";
local hpLastFrame = maxHP;
local damageDone = 0
function update(dt)

    if (math.floor(GetHP(GetObjRef())) > math.floor(maxHP / 2)) then
        timer = 0;
        timerActivated = true
        damageDone = damageDone + (GetHP(GetObjRef()) - (maxHP/2));
    end
    if (timerActivated == true) then
        timer = timer + dt;
        totalTimeInCombat = totalTimeInCombat + dt
    end

    if (timer > 10) then

        timerActivated = false;
        timer = 0;
        damageDone = 0;
        totalTimeInCombat = 0;
    end

    hpLastFrame = GetHP(GetObjRef());
    SetHPIgnoreAugment(math.floor(maxHP/2));

    local dps = 0
    if (totalTimeInCombat > 0) then
        dps = damageDone / totalTimeInCombat;
    end
    dpsStr = string.format("HPS: %.0f",dps);

    ShowString(dpsStr,120,6);

    ClearCommandQueue(GetObjRef());
end

function kill()

end