
local summon = 0
local cureaoe = 0
local wave = 0


function setup()
    SetSprite("assets/enemies/caduceus/caduceus.png");
    SetDamage(0);
    SetMaxHP(2000,true)
    SetSpeed(0)

    summon = AddAbility(GetObjRef(),"assets/enemies/caduceus/ability_summon.lua",0);
    cureaoe = AddAbility(GetObjRef(),"assets/enemies/caduceus/ability_cureaoe.lua",1);
    wave = AddAbility(GetObjRef(),"assets/enemies/caduceus/ability_wave.lua",2);
end

function update(dt)
    if (IsInCombat() == true) then
        local cx = GetX(GetObjRef())
        local cy = GetY(GetObjRef())

        CastAbility(cureaoe,0,{{x=cx,y=cy}});
         CastAbility(summon,0,{{x=cx,y=cy}});

    end
    if (GetHPPercent(GetObjRef()) <= 50) then
       CastAbility(wave,2,{})
    end


end

function kill()
    SetStunTimer(GetObjRef(),0);
    SetAbilityCooldownTimer(cureaoe,0)

    local f1 = {}
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_CURE;
    f1["value"] = 3;

    local aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",120,1,999,false,ATTACK_HITS_ENEMIES,COLOR_HEAL,DITHER_HEAL_EIGTH,false,-1,{f1});
end