function setup()
    SetSprite("Assets/Enemies/wyrm.png");
    AddAbility("Assets/enemies/wyrm_boss/ability_bite.lua",0)   
    AddAbility("Assets/enemies/wyrm_boss/ability_fire.lua",1)    
    AddAbility("Assets/enemies/wyrm_boss/ability_nuke.lua",2)    
    AddAbility("Assets/enemies/wyrm_boss/ability_firebreath.lua",3)    

    SetDamage(10);
    SetMaxHP(2500,true)
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
           -- CastAbility(1,{targ});
        end
        targ2 = {};
        targ2["target"] = GetRandomUnit(TYPE_ENEMY,TYPE_TANK)
        CastAbility(3,2,{targ2});

    end

    
    --if (j != nil)
     --   CastAbility(0,{ddasd});
end

function kill()

end