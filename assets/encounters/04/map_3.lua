function setup()

    SetSpawnPoint(180,862)
    SetMapSprite("assets/encounters/04/map_3.png")
    CreateObject("assets/enemies/staar/staar.lua",235.00,211.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/kelpie/kelpie.lua",224.00,756.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kelpie/kelpie.lua",156.00,737.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kelpie/kelpie.lua",228.00,632.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kelpie/kelpie.lua",230.00,533.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kelpie/kelpie.lua",176.00,531.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kelpie/kelpie.lua",209.00,495.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/kelpie/kelpie.lua",182.00,349.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kelpie/kelpie.lua",182.00,387.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kelpie/kelpie.lua",204.00,329.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kelpie/kelpie.lua",147.00,340.00,TYPE_ENEMY,0.00),4)
end
function update()

end
function mapend()
    GoShop(true)

   ChangeMap("assets/encounters/04/map_4.lua")
end