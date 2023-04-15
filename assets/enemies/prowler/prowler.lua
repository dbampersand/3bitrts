
local timer = 0;
local castTimer = 0
function setup()
    SetSprite("assets/enemies/prowler/prowler.png");

    SetDamage(0);
    SetMaxHP(4500,true)
    SetSpeed(0)
    SetRange(0);

    spines = AddAbility(GetObjRef(),"assets/enemies/prowler/ability_spines.lua",0);


end
function update(dt)
    if (IsInCombat()) then

        timer = timer + dt;

        local objToAttack = GetRandomUnit(TYPE_ENEMY,TYPE_ANY,GetAbilityRange(GetObjRef(),spines))[1]

        local target = GetHeadingVector(GetX(GetObjRef()),GetY(GetObjRef()),GetX(objToAttack),GetY(objToAttack));
        target["target"]  = objToAttack;


        CastAbility(spines,0.95,{target});
    end

end

function kill()
end

