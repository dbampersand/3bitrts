function setup()

    SetMapSprite("assets/encounters/01/map_6.png")
    SetSpawnPoint(112,494)
    CreateObject("assets/enemies/poison_elemental/poison_elemental.lua",204.00,84.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",80.00,204.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",265.00,189.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",251.00,171.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",242.00,222.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",284.00,213.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",28.00,212.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",20.00,173.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",50.00,168.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",145.00,358.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",102.00,339.00,TYPE_ENEMY,0.00),3)
    --PlayMusic("assets/audio/music/encounters/wyrm/wyrm_encounter_2.wav",0.5,0); 
    SetMapGoldMultiplier(1.6)

end
function update()

end
function mapend()

   ChangeMap("assets/encounters/01/map_7.lua")
    GoShop(true)
end