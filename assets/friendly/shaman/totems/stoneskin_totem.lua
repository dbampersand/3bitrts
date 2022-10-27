local maxHP = 250;
local aoe = -1
function setup()
    SetSprite("assets/friendly/shaman/totems/stoneskin_totem.png");

    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(0);
    SetRange(0)
    SetSpeed(0);
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
        f1["type"] = EFFECT_ARMOR;
        f1["value"] = 10;
        f1["duration"] = 0.5
        f1["overwrites"] = true;
        f1["name"] = "Stoneskin"

        aoe = CreateAOE(GetX(GetObjRef()),GetY(GetObjRef(y)),"",60,0.1,10,false,ATTACK_HITS_FRIENDLIES,COLOR_FRIENDLY_SHIELD,DITHER_QUARTER,false,-1,{f1});

    end
    SetAttackLifetime(aoe,10);
end
function kill()
    RemoveAttack(aoe);
    aoe = -1;
end