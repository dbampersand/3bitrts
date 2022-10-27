local maxHP = 250;
local aoe = -1
function setup()
    SetSprite("assets/friendly/shaman/totems/swift_totem.png");

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(0);
    SetSpeed(0);
    SetRange(0)
    SetMaxHP(maxHP,true)
    SetManaRegen(15);

    SetObjType(TYPE_HEALER);

    SetCategory(TYPE_HEALER);

    ObjectUsesMana(GetObjRef(),true);

end

function update(dt)
    if (aoe == -1) then
        local f1 = {}
        f1["trigger"] = TRIGGER_CONST;
        f1["type"] = EFFECT_SPEED;
        f1["value"] = 40;
        f1["duration"] = 0.5    
        f1["overwrites"] = true;
        f1["name"] = "Swiftness"

        local f2 = {}
        f2["trigger"] = TRIGGER_CONST;
        f2["type"] = EFFECT_ATTACKSPEED;
        f2["value"] = -0.1;
        f2["duration"] = 0.5
        f2["overwrites"] = true;
        f2["name"] = "Swift Attack"

        aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef(y)),"",60,0.1,10,false,ATTACK_HITS_FRIENDLIES,COLOR_FRIENDLY_SPEED,DITHER_QUARTER,false,-1,{f1,f2});

    end
    SetAttackLifetime(aoe,10);
end
function kill()
    RemoveAttack(aoe);
    aoe = -1;
end