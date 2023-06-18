local length = 15
local time = 12

function setup()
    SetAbilityRange(256)
    SetCooldown(45);
    AbilitySetPortrait("assets/enemies/kobold_miner/ability_throw_bomb.png");
    SetDescription("[b]Throw Bomb\n\nThrows an explosive bomb, which explodes after " .. time .. " seconds unless killed.")
    SetAbilityName("Throw Bomb"); 

end

function casted(x,y,obj,headingx,headingy)
    PlaySound("assets/enemies/kobold_miner/audio/throw_bomb.wav",0.5)
    CreateObject("assets/enemies/explosive_bomb/explosive_bomb.lua",GetX(GetObjRef()),GetY(GetObjRef()),TYPE_FRIENDLY,2);

    return true; 
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target)

end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    
end
function onchanneled() 
end