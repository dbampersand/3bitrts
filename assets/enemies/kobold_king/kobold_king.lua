
local maxHP = 8000;
local timer = 0

local throwBot = 0;
local fanOfKnives = 0;
local slash = 0;
local throwConcoction = 0;
local poisonedSpear = 0;
local callGuards = 0
local cloakOfFire = 0; 

local enraged = false;


function setup()
    SetSprite("assets/enemies/kobold_king/kobold_king.png");
    SetChannelingSprite("assets/enemies/kobold_king/kobold_king_channelling.png")
    SetDamage(40);
    SetMaxHP(maxHP,true)
    SetSpeed(80)
    SetRange(40);
    SetAttackSpeed(GetObjRef(),1)
    SetObjIsBoss(GetObjRef(),true);

    throwBot = AddAbility(GetObjRef(),"assets/enemies/kobold_king/ability_throw_bot.lua",0)   
    fanOfKnives = AddAbility(GetObjRef(),"assets/enemies/kobold_king/ability_fan_of_knives.lua",1)   
    slash = AddAbility(GetObjRef(),"assets/enemies/kobold_king/ability_slash.lua",2)   
    throwConcoction = AddAbility(GetObjRef(),"assets/enemies/kobold_king/ability_throw_concotion.lua",3)   
    poisonedSpear = AddAbility(GetObjRef(),"assets/enemies/kobold_king/ability_poisoned_spear.lua",4)
    callGuards = AddAbility(GetObjRef(),"assets/enemies/kobold_king/ability_call_guards.lua",5)
    cloakOfFire = AddAbility(GetObjRef(),"assets/enemies/kobold_king/ability_cloak_of_fire.lua",6)
    
    SetAttackSounds({
        "assets/audio/attacks/melee_sword/sword_1.wav",
        "assets/audio/attacks/melee_sword/sword_2.wav",
        "assets/audio/attacks/melee_sword/sword_3.wav",
        "assets/audio/attacks/melee_sword/sword_4.wav",
        "assets/audio/attacks/melee_sword/sword_5.wav",
        "assets/audio/attacks/melee_sword/sword_6.wav",
        "assets/audio/attacks/melee_sword/sword_7.wav"
    });

end 
function untoggle()

end
function update(dt)
    --concoctionTarget["target"] = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,999);

    
    if (IsInCombat(GetObjRef())) then

        timer = timer + dt

        local healthPercent = GetHP(GetObjRef()) / maxHP;

        if (healthPercent < 0.6 and healthPercent > 0.3) then
            CastAbility(fanOfKnives,0,{});
        end


        local target = {}
        target["target"] = GetHighestThreat();

        CastAbility(poisonedSpear,4,{target});
        if (timer > 15) then
            CastAbility(slash,2,{target});
        end

        
        local botPosition = {};
        botPosition["x"] = RandRange(0,255);
        botPosition["y"] = RandRange(0,255);

        if (timer > 10) then
            CastAbility(throwBot,3,{botPosition});
        end

        local concoctionTarget = {};
        concoctionTarget["target"] = GetRandomUnit(TYPE_ENEMY,TYPE_ALL,999);
        CastAbility(throwConcoction,3,{concoctionTarget});
        
        if (healthPercent < 0.5) then
            CastAbility(cloakOfFire,0,{});
        end
        if (timer > 30) then
            CastAbility(callGuards,0,{ x = RandRange(0,255), y = RandRange(0,255) });
        end


        
    end


end

function kill()
end

function OnAttack(obj)

end
