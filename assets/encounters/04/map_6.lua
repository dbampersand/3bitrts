function setup()

    SetSpawnPoint(123,716)
    SetMapSprite("assets/encounters/04/map_6.png")
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",234.00,522.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/explosive_bomb/explosive_bomb.lua",240.00,488.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/staar/staar.lua",378.00,255.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/web_spinner/web_spinner.lua",429.00,131.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",120.00,568.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",90.00,566.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kelpie/kelpie.lua",284.00,363.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kelpie/kelpie.lua",324.00,404.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",226.00,462.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",214.00,470.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",142.00,594.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",110.00,586.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/spriggan/spriggan.lua",566.00,149.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/spriggan/spriggan.lua",591.00,228.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/tentacle/tentacle.lua",662.00,269.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/tentacle/tentacle.lua",657.00,138.00,TYPE_ENEMY,0.00),5)
end
function update()

end
function mapend()
    GoShop(true)

   ChangeMap("assets/encounters/04/map_7.lua")
end