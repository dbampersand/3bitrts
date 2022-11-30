
local maxHP = 8000;



function setup()
    SetSprite("assets/enemies/cusith/cusith.png");
    SetChannelingSprite("assets/enemies/cusith/cusith_channelling.png")
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


end

function kill()
end

function OnAttack(obj)

end
