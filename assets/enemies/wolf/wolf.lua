local targ = -1

local packAnimal = 0

function setup()
    SetSprite("assets/enemies/wolf/wolf.png");

    SetDamage(3);
    SetMaxHP(250,true)
    SetSpeed(120)
    SetRange(5);

    packAnimal = AddAbility(GetObjRef(),"assets/enemies/wolf/ability_pack_animal.lua",0);

    local targ = GetRandomUnit(TYPE_ENEMY,Bor(TYPE_HEALER,TYPE_RANGEDDPS),999)

end

function update(dt)
    AddThreat(targ,GetObjRef(),100);
    CastAbility(0,0,{});
end

function kill()

end