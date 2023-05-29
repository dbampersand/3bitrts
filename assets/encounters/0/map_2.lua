local miniboss = -1

function setup()
    SetMapSprite("assets/encounters/0/map_2.png")

    SetDecoration(CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",66.00,60.00,TYPE_ENEMY,0),false);
    CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",167.00,49.00,TYPE_ENEMY,0);
    CreateObject("assets/enemies/kobold_wizard/kobold_wizard.lua",191.00,148.00,TYPE_ENEMY,0);

    miniboss = CreateObject("assets/enemies/nix/nix.lua",143.00,116.00,TYPE_ENEMY,0); 
    Print(miniboss);
    SetDecoration(miniboss,true);
    SetInvincible(miniboss,true);

    SetSpawnPoint(47,232)
    PlayMusic("assets/audio/music/encounters/kobold/kobold_encounter.wav",0.5,12); 
    
    SetAutoWin(false);
end

function update(dt)
    if (NumObjectsOwnedByPlayer(TYPE_ENEMY) == 1) then
        SetDecoration(miniboss,false);
        SetInvincible(miniboss,false);
    end
    if (NumObjectsOwnedByPlayer(TYPE_ENEMY) == 0) then
        SetAutoWin(true);
    end

end

function kill()
end

function mapend()
    ChangeMap("assets/encounters/0/map_3.lua")
    return false;
end

function objectdied(obj)
end