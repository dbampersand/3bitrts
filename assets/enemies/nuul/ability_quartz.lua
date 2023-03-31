local area = -1

function setup()
    SetAbilityRange(32)
    SetCooldown(0);
    AbilitySetPortrait("assets/enemies/kobold_miner/ability_throw_bomb.png");
    SetDescription("[b]Quartz\n\nCreates an area of damaging fire.");
    SetAbilityName("Quartz"); 

    AbilitySetCastType(ABILITY_TOGGLE);

end

function casted(x,y,obj,headingx,headingy)
    

    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT; 
    f1["type"] = EFFECT_HURT;
    f1["value"] = 1;

    --CreateAttackArea({{0,0},{20,0},{20,20},{0,20}},5,10,"",0.5,12,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,0,false, {f1});
    --CreateAttackArea({{100,100},{200,100},{200,200},{100,200}},1,10,"",0.5,12,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,0,false, {f1});
    --local area = CreateAttackArea({{0,0},{50,-50},{-50,-50}},GetX(GetObjRef()),GetY(GetObjRef()),"",0.5,60,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,0,false, {f1});
    
    --local area = CreateAttackArea({{0,0},{-50,-50},{-50,50}},GetX(GetObjRef()),GetY(GetObjRef()),"",0.5,60,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,0,false, {f1});
    area = CreateAttackArea({
        {-GetMapWidth(),-GetMapHeight()},
        {GetMapWidth(),-GetMapHeight()},
        {GetMapWidth(),GetMapHeight()},
        {-GetMapWidth(),GetMapHeight()}
    },
        GetX(GetObjRef()),GetY(GetObjRef()),"",0.5,60,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,0,false, {f1});
    local angle = 30
    local move = 360 / 5

    CutoutArea(area,{{0,0},{-100,-100},{-100,-200}});

    for i=0,5 do
        local p1 = {100,100};
        local p2 = {100,100};



        p1 = RotatePoint(p1.x,p1.y,GetX(GetObjRef()),GetY(GetObjRef()),angle+(move*(i)))
        p2 = RotatePoint(p2.x,p2.y,GetX(GetObjRef()),GetY(GetObjRef()),-angle+(move*(i)))
        p1.x = p1.x - GetX(GetObjRef());
        p1.y = p1.y - GetY(GetObjRef());

        p2.x = p2.x - GetX(GetObjRef());
        p2.y = p2.y - GetY(GetObjRef());


        --p1 = Normalize(p1.x,p1.y);
       -- p2 = Normalize(p2.x,p2.y);
        --p1.x = p1.x * 100;
        --p1.y = p1.y * 100;
        --p2.x = p2.x * 100;
        --p2.y = p2.y * 100;


        CutoutArea(area,{{0,0},p1,p2});

    end
    --RotateAttackArea(area,RandRange(0,360));
   
    --CreateAttackArea({{-50,-50},{50,-50},{50,50},{-50,50}},GetX(GetObjRef()),GetY(GetObjRef()),"",0.5,12,false,ATTACK_HITS_ENEMIES,COLOR_DAMAGE,0,false, {f1});
    
    return true; 
end

function onhit(x,y,objhit)
end 
function ontimeout(x,y,obj,dt,target)
end
function abilitytick(x, y, durationLeft, parent, target, dt, attackRef)
    RotateAttackArea(attackRef,10 * dt);
    MoveAttack(attackRef,GetX(GetObjRef()),GetY(GetObjRef()));
    SetAttackLifetime(attackRef,100);
end
function onchanneled() 
end

function untoggle()
    Print("Coming from untoggle");
    RemoveAttack(area);
    area = -1
end