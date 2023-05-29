local timer = 0

function setup()
    SetMapSprite("assets/encounters/01/map_2.png")

    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",212.00,100.00,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",200.00,124.00, TYPE_ENEMY,0),1);

    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",347.00,188.00,TYPE_ENEMY,0),2) ;
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",383.00,166.00,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",384.00,287.00,TYPE_ENEMY,0),3);
    
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",373.00,258.00,TYPE_ENEMY,0),3);

    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",101.00,315.00,TYPE_ENEMY,0),5);
    SetAggroGroup(CreateObject("assets/enemies/wadjet/wadjet.lua",110.00,356.00,TYPE_ENEMY,0),5);
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",101.00,288.00,TYPE_ENEMY,0),5);


    CreateObject("assets/decor/curebox.lua",374,307,TYPE_ENEMY,0);
    CreateObject("assets/decor/curebox.lua",284,251,TYPE_ENEMY,0);


    SetSpawnPoint(35,115)
    
    SetAggroGroup(CreateObject("assets/enemies/kobold_crusher/kobold_crusher.lua",331.00,294.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kobold_crossbow/kobold_crossbow.lua",223.00,303.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",252.00,329.00,TYPE_ENEMY,0.00),4)
    
    PlayMusic("assets/audio/music/encounters/wyrm/wyrm_encounter_1.wav",0.5,16.36); 

end

function update(dt)
    --ChangeMap("assets/encounters/01/map_2.lua")
        
    --if (NumObjectsOwnedByPlayer(TYPE_ENEMY) == 0) then
      --  ChangeMap("assets/encounters/01/map_final.lua")
    --end
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
    ChangeMap("assets/encounters/01/map_3.lua")
    return false;
end