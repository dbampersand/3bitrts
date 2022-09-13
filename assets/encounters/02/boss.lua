
function setup()
    SetSprite("assets/encounters/02/minotaur/minotaur.png");
    AddAbility("assets/enemies/minotaur/ability_smash.lua",0)   
    AddAbility("assets/enemies/wyrm_boss/ability_fire.lua",1)    
    AddAbility("assets/enemies/wyrm_boss/ability_nuke.lua",2)    
    AddAbility("assets/enemies/wyrm_boss/ability_firebreath.lua",3)    

    SetDamage(10);
    SetMaxHP(10000,true)
    SetSpeed(10)
    SetObjectPush(true);
end

function update(dt)
    if (IsInCombat()) then
        j = GetThreatRank()

        target = {};
        target["target"] = j[1];
        
        CastAbility(0,4,{target})
    end

end

function kill()

end