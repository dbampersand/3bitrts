local maxHP = 9999;
local testAbility = 0

function setup()
    SetSprite("assets/enemies/trainingdummy/trainingdummy.png");

    SetDamage(0);
    SetRange(200)
    SetMaxHP(maxHP,true)
    SetSpeed(0)
    SetObjectPush(true);
    ObjIsStunnable(GetObjRef(),false);
    SetObjIsBoss(GetObjRef(),true)

    testAbility = AddAbility(GetObjRef(),"assets/enemies/scorpion/ability_spit.lua",0)   

end 

local timer = 0
local totalTimeInCombat = 0;
local timerActivated = false
local dpsStr = "DPS: 0.00";
local hpLastFrame = maxHP;
local damageDone = 0
function update(dt)

   -- testTarget = {};
    --testTarget["target"] = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,999);
    --CastAbility(testAbility,4,{testTarget});

    if (GetHP(GetObjRef()) < maxHP) then
        timer = 0;
        timerActivated = true
        damageDone = damageDone + maxHP - GetHP(GetObjRef());
    end
    if (timerActivated == true) then
        timer = timer + dt;
        totalTimeInCombat = totalTimeInCombat + dt
    end

    if (timer > 3) then

        timerActivated = false;
        timer = 0;
        damageDone = 0;
        totalTimeInCombat = 0;
    end

    hpLastFrame = GetHP(GetObjRef());
    SetHP(maxHP);

    local dps = 0
    if (totalTimeInCombat > 0) then
        dps = damageDone / totalTimeInCombat;
    end
    dpsStr = string.format("DPS: %.0f",dps);
    
    local strX = GetX(GetObjRef()) + GetWidth(GetObjRef())/2 + 5;
    local strY = GetY(GetObjRef());
    ShowString(dpsStr,strX,strY);

    ClearCommandQueue(GetObjRef());
end

function kill()

end