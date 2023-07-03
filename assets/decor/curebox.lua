local maxHP = 20

function setup()
    SetSprite("assets/decor/curebox.png");
    SetDecoration(GetObjRef(),true);
    SetInvincible(GetObjRef(),false);
    SetMaxHP(maxHP,true)
end
local angle = 0

function update(dt)
end

function kill()
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = EFFECT_CURE;
    f1["value"] = 1;

    local aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef()),"",20,0.25,9999,false,Bor(ATTACK_HITS_FRIENDLIES, ATTACK_HITS_ENEMIES),COLOR_HEAL,DITHER_HEAL_HALF,false,-1,0,0,0,{f1});
    SetAttackCircle(aoe,true);

end 