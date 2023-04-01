local maxHP = 7000

local bite = 0
local fire = 0
local nuke = 0
local firebreath = 0
local summonAdd = 0
local bomb = 0

local decisionTime = 0
local makeChoice = 3

local enraged = false

local inCombat = false;

function setup()
    SetSprite("assets/enemies/wyrm_boss/wyrm.png");
    SetChannelingSprite("assets/enemies/wyrm_boss/wyrm_channeling.png");
    bite = AddAbility(GetObjRef(),"assets/enemies/wyrm_boss/ability_bite.lua",0)   
    fire = AddAbility(GetObjRef(),"assets/enemies/wyrm_boss/ability_fire.lua",1)    
    nuke = AddAbility(GetObjRef(),"assets/enemies/wyrm_boss/ability_nuke.lua",2)    
    firebreath = AddAbility(GetObjRef(),"assets/enemies/wyrm_boss/ability_firebreath.lua",3)    
    summonAdd = AddAbility(GetObjRef(),"assets/enemies/wyrm_boss/ability_summon_adds.lua",4)    
    bomb = AddAbility(GetObjRef(),"assets/enemies/wyrm_boss/ability_bomb.lua",5);    
    rage = AddAbility(GetObjRef(),"assets/enemies/wyrm_boss/ability_rage.lua",6);    

    SetDamage(50);
    SetMaxHP(maxHP,true)
    SetSpeed(10)
    SetObjectPush(true);
    ObjIsStunnable(GetObjRef(),false);
    SetObjIsBoss(GetObjRef(),true);
    
    SetAttackSounds({
        "assets/audio/attacks/attack_spit/spit_1.wav",
        "assets/audio/attacks/attack_spit/spit_2.wav",

        "assets/audio/attacks/melee_bite/bite_1.wav",
        "assets/audio/attacks/melee_bite/bite_2.wav",
        "assets/audio/attacks/melee_bite/bite_3.wav",
        "assets/audio/attacks/melee_bite/bite_4.wav",
        "assets/audio/attacks/melee_bite/bite_5.wav"
    });

end

local enrageTransitionTimer = 0
function update(dt)
    decisionTime = decisionTime + dt;

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_HURT;
    f1["value"] = 20;  
    

    if (enraged == true and enrageTransitionTimer < 10) then
        enrageTransitionTimer = enrageTransitionTimer + dt;
        CastAbility(rage,1.2,{});
        SetMovePoint(GetObjRef(),128,106,false,false)
    
        do return end;
    end

    if (decisionTime < makeChoice) then
        do return end
    end
    decisionTime = 0;


    j = GetThreatRank()

    target = {};    
    target["target"] = j[1];
    
    


    if (IsInCombat()) then
        if (inCombat == false) then
            inCombat = true;

        end

        local bombtarg = {}
        bombtarg["target"] = GetRandomUnit(TYPE_ENEMY,Bor(Bor(TYPE_HEALER,TYPE_RANGEDDPS),TYPE_MELEEDPS),999)[1]
    
        CastAbility(bomb,0.1,{bombtarg});
    

        targ0 = {};
        targ0["target"] = GetHighestThreat();
        CastAbility(bite,1,{targ0});

        if (AbilityIsOnCooldown(GetObjRef(),1) == false) then

            aoeTarget = GetRandomUnit(TYPE_ENEMY,TYPE_HEALER,999)[1]
            targ1 = {};
            targ1["target"] = aoeTarget;
            --CastAbility(fire,0.1,{targ1});
        end
        
        targ2 = {}
        targ2["target"] = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,999)[1];
        CastAbility(nuke,0.1,{targ2});

        targ3 = {};
        targ3["target"] = GetRandomUnit(TYPE_ENEMY,TYPE_TANK,999)[1]
        --CastAbility(firebreath,4,{targ3});


    end

    if (GetHP(GetObjRef()) <= maxHP/2) then
        CastAbility(summonAdd,0.1);
        --Choose spell to cast every second instead of every 3
        makeChoice = 1;
        if (enraged == false) then
            SetDamage(60);
            SetSpeed(30)
            SetSprite("assets/enemies/wyrm_boss/wyrm_enraged.png")
            
        end
        enraged = true
    end
end

function kill()

end