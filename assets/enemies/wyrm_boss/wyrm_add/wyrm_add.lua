local maxHP = 100

function setup()
    SetSprite("assets/enemies/wyrm_boss/wyrm_add/smallsnake.png");
    AddAbility("assets/enemies/wyrm_boss/ability_bite.lua",0)   
    SetDamage(20);
    SetMaxHP(maxHP,true)
    SetSpeed(10)
end

function update(dt)
end

function kill()

end