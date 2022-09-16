local timer = 1
local rampage = 0
local smash = 0
local quake = 0
local chuck = 0

function setup()
    SetSprite("assets/encounters/02/minotaur/minotaur.png");
    SetChannelingSprite("assets/enemies/minotaur/minotaur_channeling.png");

    smash = AddAbility("assets/enemies/minotaur/ability_smash.lua",0)   
    rampage = AddAbility("assets/enemies/minotaur/ability_rampage.lua",1)    
    quake = AddAbility("assets/enemies/minotaur/ability_quake.lua",2)    
    chuck = AddAbility("assets/enemies/minotaur/ability_chuck.lua",3)    
    charge = AddAbility("assets/enemies/minotaur/ability_charge.lua",4)    

    SetDamage(10);
    SetMaxHP(10000,true)
    SetSpeed(10)
    SetObjectPush(true);
end

function update(dt)
    if (IsInCombat()) then
        local chargeTarget = {}
        chargeTarget["target"] = GetRandomUnit(TYPE_ENEMY,Bor(TYPE_MELEEDPS,Bor(TYPE_RANGEDDPS,TYPE_HEALER)))
        CastAbility(charge,2,{chargeTarget})
        CastAbility(quake,0,{})

        local chuckTarget = {}
        chuckTarget["target"] = GetRandomUnit(TYPE_ENEMY,TYPE_HEALER)
        CastAbility(chuck,2,{chuckTarget})

        timer = timer + dt;
        if (math.fmod(math.floor(timer),30)) then
            CastAbility(rampage, 0.1, {})
        end

        j = GetThreatRank()

        target = {};
        target["target"] = j[1];
        
        CastAbility(smash,4,{target})

    end

end

function kill()

end