local timer = 1
local rampage = 0
local smash = 0
local quake = 0

function setup()
    SetSprite("assets/encounters/02/minotaur/minotaur.png");
    SetChannelingSprite("assets/enemies/minotaur/minotaur_channeling.png");

    smash = AddAbility("assets/enemies/minotaur/ability_smash.lua",0)   
    rampage = AddAbility("assets/enemies/minotaur/ability_rampage.lua",1)    
    quake = AddAbility("assets/enemies/minotaur/ability_quake.lua",2)    

    AddAbility("assets/enemies/wyrm_boss/ability_firebreath.lua",3)    

    SetDamage(10);
    SetMaxHP(10000,true)
    SetSpeed(10)
    SetObjectPush(true);
end

function update(dt)
    if (IsInCombat()) then
        CastAbility(quake,0,{})

        timer = timer + dt;
        if (AbilityIsOnCooldown(rampage)) then
            do return end;
        end
        if (math.fmod(math.floor(timer),30)) then
            CastAbility(1, 0.1, {})
            do return end;
        end

        j = GetThreatRank()

        target = {};
        target["target"] = j[1];
        
        CastAbility(0,4,{target})

    end

end

function kill()

end