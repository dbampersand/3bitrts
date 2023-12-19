local angle = 30

function setup()
    SetAbilityRange(999)
    SetCooldown(25); 
    AbilitySetPortrait("assets/enemies/aspidochelone/ability_summon.png");
    SetDescription("[b]Summon\n\nSummons protectors.")
    AbilitySetCastType(ABILITY_INSTANT);

end
function casted(x,y,obj,headingx,headingy)
    local cx = x--GetX(GetObjRef());
    local cy = y--GetY(GetObjRef());

    local point = {x=cx, y=cy-32};
    local parentAggroGroup = GetAggroGroup(GetObjRef());
    for i=1,2 do
        SetAggroGroup(CreateObject("assets/enemies/sea_goblin/sea_goblin.lua",point.x,point.y,TYPE_FRIENDLY,2),parentAggroGroup);
        point = RotatePoint(point.x,point.y,cx,cy,angle*i);
    end
    PlaySound("assets/enemies/caduceus/audio/summon.wav",0.5);

    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end
function onchanneled() 
    SetChannelingSprite("assets/enemies/aspidochelone/aspidochelone.png")
end