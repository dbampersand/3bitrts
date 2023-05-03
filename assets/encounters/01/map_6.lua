function setup()

    SetMapSprite("assets/encounters/01/map_6.png")
    SetSpawnPoint(112,494)
    CreateObject("assets/enemies/poison_elemental/poison_elemental.lua",168.00,80.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",80.00,204.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_saboteur/kobold_saboteur.lua",244.00,204.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_miner/kobold_miner.lua",251.00,171.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",212.00,244.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",263.00,234.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/cobra/cobra.lua",28.00,212.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",31.00,176.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",44.00,149.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",145.00,358.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/rock_tosser/rock_tosser.lua",102.00,339.00,TYPE_ENEMY,0.00),3)
end
function update()

end
function mapend()

end