function setup()
    SetSprite("Assets/Enemies/wyrm.png");
    AddAbility("Assets/enemies/wyrm_boss/ability_bite.lua",0)    
    SetDamage(10);
    SetMaxHP(10000,true)
    SetSpeed(10)
end

function update()
    j = GetThreatRank()

    --ddasd = {};
    --ddasd["target"] = j;

    
    --if (j != nil)
     --   CastAbility(0,{ddasd});
end

function kill()

end