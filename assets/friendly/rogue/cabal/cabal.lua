local maxHP = 80;
local lifetime = 15;
local timer = 0;

function setup()
    SetSprite("assets/friendly/rogue/cabal/cabal.png");
    AddAbility("assets/friendly/rogue/cabal/poison_dagger.lua",0)    


    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(15);
    SetRange(1)
    SetSpeed(60)
    SetMaxHP(maxHP,true);

    SetObjType(TYPE_MELEEDPS);
    
    SetObjectPush(false);
end

function update(dt)
    HurtObj(GetObjRef(),(dt*maxHP)/lifetime);
end

function kill()

end