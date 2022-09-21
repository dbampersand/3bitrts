local maxHP = 250;

function setup()
    SetSprite("assets/friendly/shaman/shaman.png");
    AddAbility(GetObjRef(),"assets/friendly/shaman/stoneskin_totem.lua",0)    
    AddAbility(GetObjRef(),"assets/friendly/shaman/swift_totem.lua",1)    
    AddAbility(GetObjRef(),"assets/friendly/shaman/flame_totem.lua",2)    
    AddAbility(GetObjRef(),"assets/friendly/shaman/healing_stream.lua",3)    
    AddAbility(GetObjRef(),"assets/friendly/shaman/healing_rain.lua",4)    


    AddAttackSprite("assets/ui/slash_fx3.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx2.png",16,16,0.05);
    AddAttackSprite("assets/ui/slash_fx.png",32,32,0.05);   

    SetDamage(5);
    SetRange(100)
    SetMaxHP(maxHP,true)
    SetManaRegen(15);

    SetObjType(TYPE_HEALER);
    IsPlayerChoosable(true);

    SetCategory(TYPE_HEALER);

    ObjectUsesMana(GetObjRef(),true);



end

function update(dt)
end

function kill()

end