local hasSelected = false
local firstMessagesShown = false
function setup()
    SetMapSprite("assets/ui/map_unitselect.png")
    SetMapName("unitselect")
    local trainingdummy = SetAggroGroup(CreateObject("assets/enemies/trainingdummy/trainingdummy.lua",88.00,36.00,TYPE_ENEMY,0),0);

    local healingdummy = CreateObject("assets/enemies/healingdummy/healingdummy.lua",168.00,36.00,TYPE_FRIENDLY,0)


    SetSpawnPoint(77,145)

    



end

function update()
    if (HasDoneTutorial() == false and firstMessagesShown == false) then
        firstMessagesShown = true
        PushMessage("Welcome to the testing ground.",10,10,236,50,false)
        PushMessage("Here, you can test units out before you venture forth.",10,10,236,50,false)
        PushMessage("This is a training dummy. You can test damaging abilities on it!",88+32+10,10,116,50,false)
        PushMessage("This is a healing dummy. You can test healing abilities on it!",10,10,168-30,50,false)
        PushMessage("To continue, you need to select three units.",10,10,236,50,false)
        PushMessage("Left click and drag a box around three white units.",10,140,236,50,true)
    end

    if (HasDoneTutorial() == false and hasSelected == false and GetNumberOfUnitsSelected() == 3) then
        hasSelected = true
        ClearMessages();
        PushMessage("Click this button to start the encounter.",109,140,96,0,true)
    end

end 



function mapend()
    GoShop(false)
    return true
end 