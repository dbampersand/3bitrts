local miniboss = -1

function setup()
    SetMapSprite("assets/encounters/0/map_2.png")

    CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",70,62,TYPE_ENEMY,0);
    CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",205,64,TYPE_ENEMY,0);
    CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",205,184,TYPE_ENEMY,0);

    miniboss = CreateObject("assets/enemies/nix/nix.lua",140,108,TYPE_ENEMY,0); 
    SetDecoration(miniboss,true);

    SetSpawnPoint(47,232)
    PlayMusic("assets/audio/music/encounters/kobold_encounter.wav",0.5); 
    
end

function update(dt)

end

function kill()
end

function mapend()
    ChangeMap("assets/encounters/0/map_3.lua")
    return false;
end