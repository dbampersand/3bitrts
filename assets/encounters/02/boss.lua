local timer = 1
local rampage = 0
local smash = 0
local quake = 0
local chuck = 0

function setup()
    SetSprite("assets/encounters/02/minotaur/minotaur.png");
    SetChannelingSprite("assets/enemies/minotaur/minotaur_channeling.png");

    smash = AddAbility(GetObjRef(),"assets/enemies/minotaur/ability_smash.lua",0)   
    rampage = AddAbility(GetObjRef(),"assets/enemies/minotaur/ability_rampage.lua",1)    
    quake = AddAbility(GetObjRef(),"assets/enemies/minotaur/ability_quake.lua",2)    
    chuck = AddAbility(GetObjRef(),"assets/enemies/minotaur/ability_chuck.lua",3)    
    charge = AddAbility(GetObjRef(),"assets/enemies/minotaur/ability_charge.lua",4)    
    summon = AddAbility(GetObjRef(),"assets/enemies/minotaur/ability_summon.lua",5)    

    SetDamage(10);
    SetMaxHP(10000,true)
    SetSpeed(10)
    SetObjectPush(true);
    ObjIsStunnable(GetObjRef(),false);
    SetObjIsBoss(GetObjRef(),true);

end

function update(dt)
    if (IsInCombat()) then
        local chargeTarget = {}
        chargeTarget["target"] = GetRandomUnit(TYPE_ENEMY,Bor(TYPE_MELEEDPS,Bor(TYPE_RANGEDDPS,TYPE_HEALER)),999)
        CastAbility(charge,5,{chargeTarget})
        local summonPosition = {}
        summonPosition["x"] = GetX();
        summonPosition["y"] = GetY();

        CastAbility(summon,0,{summonPosition})
        CastAbility(quake,0,{})

        local chuckTarget = {}
        chuckTarget["target"] = GetRandomUnit(TYPE_ENEMY,TYPE_HEALER,999)
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