local attackSpeedMin = 2
local attackSpeedMax = 0.25

local attackSpeed = attackSpeedMin
local lastAttacked = -1

local maxHP = 250;

function setup()
    SetSprite("assets/enemies/berzerker/berzerker.png");

    SetDamage(5);
    SetMaxHP(maxHP,true)
    SetSpeed(80)
    SetRange(40);
    SetMaxHP(400,true))
    SetAttackSpeed(GetObjRef(),2)


end

function update(dt)
end

function kill()

end

function OnAttack(obj)
    if lastAttacked == obj then
        attackSpeed = attackSpeed - 0.1
        if (attackSpeed < attackSpeedMax) then
            attackSpeed = attackSpeedMax
        end
        SetAttackSpeed(GetObjRef(),attackSpeed);    
    else 
        lastAttacked = obj
        attackSpeed = attackSpeedMin
        SetAttackSpeed(GetObjRef(),attackSpeed);    
    end
end