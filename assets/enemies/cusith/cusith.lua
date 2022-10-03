
local maxHP = 8000;

local howl = 0
local bite = 0

local howlTimer = 0
local howlCD = 30

function setup()
    SetSprite("assets/enemies/cusith/cusith.png");
    SetChannelingSprite("assets/enemies/cusith/cusith_channelling.png")
    SetDamage(50);
    SetMaxHP(maxHP,true)
    SetSpeed(80)
    SetRange(40);
    SetAttackSpeed(1)
    SetObjIsBoss(GetObjRef(),true);

    howl = AddAbility(GetObjRef(),"assets/enemies/cusith/ability_howl.lua",0)   
    bite = AddAbility(GetObjRef(),"assets/enemies/cusith/ability_bite.lua",1);
end 
function untoggle()

end
function update(dt)

    howlTimer = howlTimer + dt

    if (howlTimer > howlCD) then
        SetMovePoint(128,128);
        CastAbility(howl,3,{});
        SetAbilityCooldownTimer(GetObjRef(),howl,0);
        if (howlTimer > howlCD + GetAbilityCooldown(GetObjRef(),howl)*3) then
            howlTimer = 0
        end
        do return end;
    end
    local biteTarget = {};
    biteTarget["target"] = GetHighestThreat();

    CastAbility(bite,2,{biteTarget})



end

function kill()
end

function OnAttack(obj)

end
