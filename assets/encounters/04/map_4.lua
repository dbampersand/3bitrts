function setup()

    SetSpawnPoint(94,932)
    SetMapSprite("assets/encounters/04/map_4.png")
    SetAggroGroup(CreateObject("assets/enemies/sea_goblin/sea_goblin.lua",96.00,763.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/selkie/selkie.lua",80.00,628.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/selkie/selkie.lua",116.00,636.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/selkie/selkie.lua",332.00,628.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/wraith/wraith.lua",288.00,572.00,TYPE_ENEMY,0.00),3)
    CreateObject("assets/enemies/sea_witch/sea_witch.lua",152.00,351.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/kelpie/kelpie.lua",183.00,463.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/kelpie/kelpie.lua",219.00,425.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/sea_goblin/sea_goblin.lua",258.00,236.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/sea_goblin/sea_goblin.lua",220.00,224.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/sea_witch/sea_witch.lua",343.00,104.00,TYPE_ENEMY,0.00),6)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",286.00,59.00,TYPE_ENEMY,0.00),6)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",385.00,126.00,TYPE_ENEMY,0.00),6)
end
function update()

end
function mapend()

   ChangeMap("assets/encounters/04/map_5.lua")
end