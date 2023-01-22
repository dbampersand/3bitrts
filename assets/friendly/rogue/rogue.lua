local maxHP = 400;

function setup()
    SetSprite("assets/friendly/rogue/rogue.png");

    AddAbility(GetObjRef(),"assets/friendly/rogue/shiv.lua",0)
    AddAbility(GetObjRef(),"assets/friendly/rogue/cloak.lua",1)
    AddAbility(GetObjRef(),"assets/friendly/rogue/backstab.lua",2)
    AddAbility(GetObjRef(),"assets/friendly/rogue/toxic.lua",3)
    AddAbility(GetObjRef(),"assets/friendly/rogue/summon_cabal.lua",4)
    --AddAbility(GetObjRef(),"assets/friendly/rogue/dash.lua",4)


    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);

    SetDamage(15);
    SetRange(1)
    SetSpeed(60)
    SetMaxHP(maxHP,true);

    SetObjType(TYPE_MELEEDPS);

    IsPlayerChoosable(true);    
    SetCategory(TYPE_MELEEDPS);
    
    SetObjectPush(false);

    SetObjPurchaseScreenSprite("assets/friendly/rogue/rogue_full.png")
    SetObjName("Rogue");
    SetObjDescription("Having studied herbology, the Rogue is a master of all that is poisonous and toxic. For all his knowledge, his husband, poisoned by an unknown source, lays dying in bed, unable to be diagnosed. Upon hearing rumours of a particularly potent strain of healing herb growing in the mountain, he resolved to put his skills to use.")

end

function update(dt)
end

function kill()

end