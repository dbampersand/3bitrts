local shoot = 0
local timer = 0

local nextShotAngle = {headingx = -1, headingy = 0}
local numShots = 1

function setup()
    SetSprite("assets/enemies/repeater/repeater.png");

    SetDamage(0);
    SetMaxHP(750,true)
    SetSpeed(0)
    SetRange(60);
    SetInvincible(GetObjRef(),true)
    RemoveFromCount(GetObjRef(),true);
    SetDecoration(GetObjRef(),true);

    shoot = AddAbility(GetObjRef(),"assets/enemies/repeater/ability_shoot.lua",0);

    SetAttackSounds(
        {
        "assets/audio/attacks/ranged_bow/bow_1.wav",
        "assets/audio/attacks/ranged_bow/bow_2.wav",
        "assets/audio/attacks/ranged_bow/bow_3.wav",
        "assets/audio/attacks/ranged_bow/bow_4.wav",
        "assets/audio/attacks/ranged_bow/bow_5.wav",
        "assets/audio/attacks/ranged_bow/bow_6.wav",
        "assets/audio/attacks/ranged_bow/bow_7.wav",
        "assets/audio/attacks/ranged_bow/bow_8.wav"
    }
    );

end
local function rotate()
    local shot = math.fmod(numShots,4);
    if (shot == 0) then
        nextShotAngle = {headingx = -1, headingy = 0}
    end
    if (shot == 1) then
        nextShotAngle = {headingx = 0, headingy = -1}
    end
    if (shot == 2) then
        nextShotAngle = {headingx = 1, headingy = 0}
    end
    if (shot == 3) then
        nextShotAngle = {headingx = 0, headingy = 1}
    end

    numShots = numShots + 1;
end

function update(dt)
    if (CastAbility(shoot,0,{nextShotAngle})) then
        rotate();
    end
end

function kill()

end