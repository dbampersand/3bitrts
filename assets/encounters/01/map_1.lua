local timer = 0

function setup()
    SetMapSprite("assets/encounters/01/map_1.png")

    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",115,296,TYPE_ENEMY,0),1);
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",131,296,TYPE_ENEMY,0),1);
   
    CreateObject("assets/decor/curebox.lua",104,260,TYPE_ENEMY,0);
    CreateObject("assets/decor/curebox.lua",120,260,TYPE_ENEMY,0);
    CreateObject("assets/decor/curebox.lua",136,260,TYPE_ENEMY,0);

    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",80,184, TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/viper/viper.lua",100,180,TYPE_ENEMY,0),2);
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",164,184,TYPE_ENEMY,0),2);


    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",130,48,TYPE_ENEMY,0),3);
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",180,48,TYPE_ENEMY,0),3);

    SetAggroGroup(CreateObject("assets/enemies/caduceus/caduceus.lua",156,48,TYPE_ENEMY,0),3);

    SetSpawnPoint(120,460)
    
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