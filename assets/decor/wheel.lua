local maxHP = 100

function setup()
    SetSprite("assets/decor/wheel.png");
    SetDecoration(GetObjRef(),true);
    SetInvincible(GetObjRef(),true);
end
local angle = 0

function update(dt)
    angle = angle +  dt*5;
    SetRotation(angle);
end

function kill()

end 