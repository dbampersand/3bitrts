local length = 15


function setup()
    SetAbilityRange(256)
    SetCooldown(45);
    AbilitySetPortrait("assets/enemies/cusith/ability_spin.png");
    SetDescription("[b]Throw Bot\n\nThrows an explosive bot, which explodes after " .. time .. " seconds unless killed.")

end

function casted(x,y,obj,headingx,headingy)

    CreateObject("assets/enemies/explosive_bot/explosive_bot.lua",RandRange(0,255),RandRange(0,255),TYPE_FRIENDLY,2);

    return true; 
end

function onhit(x,y,objhit)

end
function ontimeout(x,y,obj,dt,target)
    atk = -1

end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/cusith/cusith_channelling_spin.png");
end