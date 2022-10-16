function setup()
    SetSprite("assets/enemies/wadjet/wadjet.png");

    SetDamage(20);
    SetMaxHP(1500,true)
    SetSpeed(80)
    SetRange(5);

    AddAbility(GetObjRef(),"assets/enemies/wadjet/ability_bite.lua",0);

end

function update(dt)
    local threatlist = GetThreatRank();
    local target = {};
    target["target"] = threatlist[1];
    if (target["target"] ~= nil) then
        CastAbility(0,0,{target});
    end
end

function kill()

end