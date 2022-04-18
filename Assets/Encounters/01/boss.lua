function setup()
    SetSprite("Assets/Enemies/wyrm.png");
    AddAbility("Assets/enemies/wyrm_boss/ability_bite.lua",0)   
    AddAbility("Assets/enemies/wyrm_boss/ability_fire.lua",1)    

    SetDamage(10);
    SetMaxHP(10000,true)
    SetSpeed(10)
    SetObjectPush(true);
end

function update()
    j = GetThreatRank()

    target = {};
    target["target"] = j[0];
    
    


    if (IsInCombat()) then
        --CastAbility(0,target);


        if (AbilityIsOnCooldown(GetObjRef(),1) == false) then

            aoeTarget = GetRandomUnit(TYPE_ENEMY,TYPE_HEALER)
            targ = {};
            targ["target"] = aoeTarget;
            CastAbility(1,{targ});
        end

    end

    
    --if (j != nil)
     --   CastAbility(0,{ddasd});
end

function kill()

end