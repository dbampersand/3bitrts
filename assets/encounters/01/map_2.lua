local timer = 0

function setup()
    SetMapSprite("assets/encounters/01/map_2.png")

    CreateObject("assets/enemies/viper/viper.lua",221,102,TYPE_ENEMY,0);
    CreateObject("assets/enemies/cobra/cobra.lua",209,134, TYPE_ENEMY,0);

    CreateObject("assets/enemies/viper/viper.lua",344,211,TYPE_ENEMY,0) ;
    CreateObject("assets/enemies/cobra/cobra.lua",400,211,TYPE_ENEMY,0);
    CreateObject("assets/enemies/viper/viper.lua",324,265,TYPE_ENEMY,0) ;
    CreateObject("assets/enemies/cobra/cobra.lua",389,283,TYPE_ENEMY,0);
    
    CreateObject("assets/enemies/scorpion/scorpion.lua",389,262,TYPE_ENEMY,0);
    CreateObject("assets/enemies/scorpion/scorpion.lua",422,260,TYPE_ENEMY,0);


    CreateObject("assets/enemies/wadjet/wadjet.lua",180,344,TYPE_ENEMY,0);

    CreateObject("assets/decor/curebox.lua",374,307,TYPE_ENEMY,0);
    CreateObject("assets/decor/curebox.lua",284,251,TYPE_ENEMY,0);


    SetAutoWin(false);
    SetSpawnPoint(35,115)
    
end

function update(dt)
    --ChangeMap("assets/encounters/01/map_2.lua")
        
    if (NumObjectsOwnedByPlayer(TYPE_ENEMY) == 0) then
        ChangeMap("assets/encounters/01/map_final.lua")
    end
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