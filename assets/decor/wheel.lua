local maxHP = 100

function setup()
    SetSprite("assets/decor/wheel.png");
    SetDecoration(GetObjRef(),true);
    SetInvincible(GetObjRef(),true);
end
local angle = 0

function update(dt)
    if (GetGamestate() == GAMESTATE_TRANSITION_TO_INGAME) then
        Print("gg");
        angle = angle +  dt*5;
        SetRotation(angle);
    end
end

function kill()

end 