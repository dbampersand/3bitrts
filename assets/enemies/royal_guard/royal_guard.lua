local charge = 0

function setup()
    SetSprite("assets/enemies/royal_guard/royal_guard.png");

    SetDamage(7);
    SetMaxHP(750,true)
    SetSpeed(120)
    SetRange(6);

    charge = AddAbility(GetObjRef(),"assets/enemies/wolf/ability_pack_animal.lua",0);

end

function update(dt)
    local randUnit = {target = GetRandomUnit(TYPE_ENEMY,Band(TYPE_ALL,Bnot(TYPE_TANK)))};
    CastAbility(charge,2,{randUnit});
end

function kill()

end