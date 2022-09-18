
local attackSpeed = attackSpeedMin
local lastAttacked = -1

local maxHP = 400;
local aoe = -1
function setup()
    SetSprite("assets/enemies/drummer/drummer.png");

    SetDamage(5);
    SetMaxHP(maxHP,true)
    SetSpeed(80)
    SetRange(40);
    SetAttackSpeed(2)

end
function untoggle()

end
function update(dt)
    if (aoe == -1) then
        f1 = {};
        f1["trigger"] = TRIGGER_CONST;
        f1["type"] = EFFECT_SPEED;
        --f1["numTriggers"] = 5
        f1["value"] = 10;
        f1["duration"] = 1.5;
        f1["triggersPerSecond"] = 1
        f1["name"] = "Speed"
        f1["overwrites"] = true
    
        f2 = {};
        f2["trigger"] = TRIGGER_CONST;
        f2["type"] = EFFECT_ATTACKDAMAGE;
        f2["value"] = 2;
        f2["duration"] = 1.5;
        f2["triggersPerSecond"] = 1
        f2["name"] = "AttackDamage"
        f2["overwrites"] = true
        
        f3 = {};
        f3["trigger"] = TRIGGER_CONST;
        f3["type"] = EFFECT_ATTACKSPEED;
        f3["value"] = -0.2;
        f3["duration"] = 1.5;
        f3["triggersPerSecond"] = 1
        f3["name"] = "AttackSpeed"
        f3["overwrites"] = true
    
        aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"", 30, 1, 10, false, ATTACK_HITS_FRIENDLIES,COLOR_DEFAULT,DITHER_HALF,true, -1, {f1,f2,f3})
    
    end
    if (aoe > -1) then
        MoveAttack(aoe,GetX(GetObjRef()),GetY(GetObjRef()));
        SetAttackLifetime(aoe, 10)
    end

end

function kill()
    RemoveAttack(aoe)
    aoe = -1
end

function OnAttack(obj)

end
