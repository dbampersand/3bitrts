function setup()
    SetAbilityRange(30)
    SetCooldown(20); 
    AbilitySetPortrait("assets/enemies/viper/ability_bite.png");
    SetDescription("[b]Summon\n\nSummons three vipers.")
    AbilitySetCastType(ABILITY_INSTANT);

end
function casted(x,y,obj,headingx,headingy)
    local cx = x--GetX(GetObjRef());
    local cy = y--GetY(GetObjRef());

    local point = {x=cx, y=cy-32};
    local parentAggroGroup = GetAggroGroup(GetObjRef());
    for i=1,2 do
        SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",point.x,point.y,TYPE_FRIENDLY,2),parentAggroGroup);
        point = RotatePoint(point.x,point.y,cx,cy,xPos*i);
    end
    PlaySound("assets/enemies/caduceus/audio/summon_adds.wav",0.7);

    return true; 
end

function onhit(x,y,objhit)
end

function abilitytick(x, y, durationLeft)
end