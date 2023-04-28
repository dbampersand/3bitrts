local timer = 0

function setup()
    SetMapSprite("assets/encounters/0/map_1.png")

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",291.00,368.00,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",311.00,368.00,TYPE_ENEMY,0),1);

    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",299.00,240.00,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",347.00,244.00,TYPE_ENEMY,0),2);

    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",255.00,108.00,TYPE_ENEMY,0),3);
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",359.00,120.00,TYPE_ENEMY,0),3);
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",319.00,156.00,TYPE_ENEMY,0),3);

 
    SetSpawnPoint(295,507)
    PlayMusic("assets/audio/music/encounters/kobold_encounter.wav",0.5); 
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",239.00,162.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",116.00,137.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",151.00,53.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kobold_foreman/kobold_foreman.lua",119.00,81.00,TYPE_ENEMY,0.00),4)
end

function update(dt)
    --ChangeMap("assets/encounters/01/map_2.lua")
        
    timer = timer + dt;
    if (timer > 5) then
        f1 = {};
        f1["trigger"] = TRIGGER_INSTANT;
        f1["type"] = EFFECT_CURE;
        f1["value"] = 1;
        timer = 0
        --local aoe = CreateAOE(RandRange(72,160),RandRange(32,215),"",20,0.25,5,false,ATTACK_HITS_FRIENDLIES,COLOR_HEAL,DITHER_VERTICAL_HALF,false,-1,{f1});
        --SetAttackCircle(aoe,true);

    end

end

function kill()

end

function mapend()
    GoShop(true);
    ChangeMap("assets/encounters/0/map_2.lua")
    return false;
end