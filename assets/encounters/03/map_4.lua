function setup()

    SetSpawnPoint(366,1061)
    SetMapSprite("assets/encounters/03/map_4.png")
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",320.00,753.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",416.00,697.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",440.00,729.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",352.00,729.00,TYPE_ENEMY,0.00),2)
    SetAggroGroup(CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",313.00,935.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",418.00,908.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",366.00,909.00,TYPE_ENEMY,0.00),1)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",322.00,523.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",299.00,541.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",337.00,548.00,TYPE_ENEMY,0.00),5)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",313.00,553.00,TYPE_ENEMY,0.00),5)
    CreateObject("assets/enemies/skeleton_basher/skeleton_basher.lua",432.00,577.00,TYPE_ENEMY,0.00)
    SetAggroGroup(CreateObject("assets/enemies/wraith/wraith.lua",267.00,224.00,TYPE_ENEMY,0.00),6)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",282.00,325.00,TYPE_ENEMY,0.00),6)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",197.00,262.00,TYPE_ENEMY,0.00),6)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",160.00,402.00,TYPE_ENEMY,0.00),7)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_ranged/skeleton_minion_ranged.lua",124.00,392.00,TYPE_ENEMY,0.00),7)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",158.00,433.00,TYPE_ENEMY,0.00),7)
    SetAggroGroup(CreateObject("assets/enemies/skeleton_minion_melee/skeleton_minion_melee.lua",122.00,423.00,TYPE_ENEMY,0.00),7)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",174.00,453.00,TYPE_ENEMY,0.00),7)
    SetAggroGroup(CreateObject("assets/enemies/exploding_zombie/exploding_zombie.lua",128.00,476.00,TYPE_ENEMY,0.00),7)
    PlayMusic("assets/audio/music/encounters/cusith/cusith_encounter_2.wav",0.5,0); 
end
function update()

end
function mapend()

   ChangeMap("assets/encounters/03/map_5.lua")
end