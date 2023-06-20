local timer = 0

function setup()
    SetMapSprite("assets/encounters/01/map_1.png")

    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",106.00,363.00,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",129.00,358.00,TYPE_ENEMY,0),1);
   
    CreateObject("assets/decor/curebox.lua",92.00,313.00,TYPE_ENEMY,0);
    CreateObject("assets/decor/curebox.lua",113.00,312.00,TYPE_ENEMY,0);
    CreateObject("assets/decor/curebox.lua",135.00,316.00,TYPE_ENEMY,0);

    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",87.00,242.00, TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",116.00,216.00,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",148.00,204.00,TYPE_ENEMY,0),2);


    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",104.00,114.00,TYPE_ENEMY,0),3);
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",158.00,98.00,TYPE_ENEMY,0),3);

    SetAggroGroup(CreateObject("assets/enemies/caduceus/caduceus.lua",182.00,150.00,TYPE_ENEMY,0),3);

    SetSpawnPoint(120,537)
    
    CreateObject("assets/enemies/viper/viper.lua",164.00,260.00,TYPE_ENEMY,0.00)

    --PlayMusic("assets/audio/music/encounters/wyrm/wyrm_encounter_1.wav",0.5,16.36); 

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
    ChangeMap("assets/encounters/01/map_2.lua")
    return false;
end