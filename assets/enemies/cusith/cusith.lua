
local maxHP = 8000;

local howl = 0
function setup()
    SetSprite("assets/enemies/cusith/cusith.png");
    SetChannelingSprite("assets/enemies/cusith/cusith_channelling.png")
    SetDamage(50);
    SetMaxHP(maxHP,true)
    SetSpeed(80)
    SetRange(40);
    SetAttackSpeed(1)

    howl = AddAbility(GetObjRef(),"assets/enemies/cusith/ability_howl.lua",0)   

end
function untoggle()

end
function update(dt)
    CastAbility(howl,3,{});
end

function kill()
end

function OnAttack(obj)

end
