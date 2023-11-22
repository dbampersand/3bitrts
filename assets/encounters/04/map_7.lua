function setup()

    SetSpawnPoint(159,1019)
    SetMapSprite("assets/encounters/04/map_7.png")
    SetAggroGroup(CreateObject("assets/enemies/firebreather/firebreather.lua",236.00,847.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/firebreather/firebreather.lua",196.00,859.00,TYPE_ENEMY,0.00),1)
    CreateObject("assets/enemies/aspidochelone/aspidochelone.lua",521.00,212.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/prowler/prowler.lua",336.00,735.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",385.00,545.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",401.00,596.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",395.00,566.00,TYPE_ENEMY,0.00),3)
    SetAggroGroup(CreateObject("assets/enemies/tentacle_healer/tentacle_healer.lua",308.00,695.00,TYPE_ENEMY,0.00),2)
    CreateObject("assets/enemies/giant_worm/giant_worm.lua",474.00,555.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/sea_goblin/sea_goblin.lua",511.00,425.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/sea_goblin/sea_goblin.lua",485.00,413.00,TYPE_ENEMY,0.00),4)
    SetAggroGroup(CreateObject("assets/enemies/scorpion/scorpion.lua",508.00,395.00,TYPE_ENEMY,0.00),4)
    CreateObject("assets/enemies/spriggan/spriggan.lua",581.00,355.00,TYPE_ENEMY,0.00)
end
function update()

end
function mapend()

   ChangeMap("assets/encounters/04/map_8.lua")
end