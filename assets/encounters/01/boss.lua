local maxHP = 400

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
    SetSprite("assets/enemies/wyrm.png");
    bite = AddAbility("assets/enemies/wyrm_boss/ability_bite.lua",0)   
    fire = AddAbility("assets/enemies/wyrm_boss/ability_fire.lua",1)    
    nuke = AddAbility("assets/enemies/wyrm_boss/ability_nuke.lua",2)    
    firebreath = AddAbility("assets/enemies/wyrm_boss/ability_firebreath.lua",3)    
    summonAdd = AddAbility("assets/enemies/wyrm_boss/ability_summon_adds.lua",4)    
    bomb = AddAbility("assets/enemies/wyrm_boss/ability_bomb.lua",5);    

    SetDamage(10);
    SetMaxHP(maxHP,true)
    SetSpeed(10)
    SetObjectPush(true);
end

function update(dt)

    decisionTime = decisionTime + dt;
    if (decisionTime < makeChoice) then
        do return end
    end
    decisionTime = 0;

    local bombtarg = {}
    bombtarg["target"] = GetRandomUnit(TYPE_ENEMY,Bor(TYPE_HEALER,TYPE_RANGEDDPS))

    CastAbility(bomb,0.1,{bombtarg});
    do return end;

    j = GetThreatRank()

    target = {};    
    target["target"] = j[1];
    
    


    if (IsInCombat()) then
        if (inCombat == false) then
            inCombat = true;

        end
        --CastAbility(0,target);

        targ0 = {};
        targ0["target"] = GetHighestThreat();
        CastAbility(bite,4,{targ0});

        if (AbilityIsOnCooldown(GetObjRef(),1) == false) then

            aoeTarget = GetRandomUnit(TYPE_ENEMY,TYPE_HEALER)
            targ1 = {};
            targ1["target"] = aoeTarget;
            CastAbility(fire,0.1,{targ1});
        end
        
        targ2 = {}
        targ2["target"] = GetRandomUnit(TYPE_ENEMY,TYPE_ALL);
        CastAbility(nuke,0.1,{targ2});

        targ3 = {};
        targ3["target"] = GetRandomUnit(TYPE_ENEMY,TYPE_TANK)
        CastAbility(firebreath,4,{targ3});


    end

    if (GetHP() <= maxHP/2) then
        CastAbility(summonAdd,0.1);
        makeChoice = 1;
        if (enraged == false) then
            SetDamage(30);
            SetSpeed(30)

        end
        enraged = true
    end
end

function kill()

end