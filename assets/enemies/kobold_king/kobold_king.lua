
local maxHP = 8000;



function setup()
    SetSprite("assets/enemies/kobold_king/kobold_king.png");
    SetChannelingSprite("assets/enemies/kobold_king/kobold_king_channeling.png")
    SetDamage(0);
    SetMaxHP(maxHP,true)
    SetSpeed(80)
    SetRange(40);
    SetAttackSpeed(GetObjRef(),1)
    SetObjIsBoss(GetObjRef(),true);

end 
function untoggle()

end
function update(dt)
    local healthPercent = GetHP(GetObjRef()) / maxHP;
    

end

function kill()
end

function OnAttack(obj)

end
