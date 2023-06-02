local percentCleave = 0.4

function setup(obj)
    SetItemName(obj,"Dark Lash")
    SetItemDescription(obj,"Causes all direct cast abilities to cleave at " .. percentCleave*100 .. "% ")
    SetItemGoldCost(obj,125)
    SetItemTier(obj,2)  
    SetItemIcon(obj, "assets/items/quality_high/dark_lash/dark_lash.png");
end

function attached(obj)

end

function update(dt)
end

function onattack(item,obj,target,dt,value)
end

function oneffect(item,obj,target,trigger,effectType,stacks,value,duration,value)

end
function onapplyeffect(item,obj,target,trigger,effectType,stacks,value,duration)
    local f1 = {};
    f1["trigger"] = TRIGGER_INSTANT;
    f1["type"] = effectType
    f1["value"] = value;  
   
    CleaveEffect(target,{f1},10,35,percentCleave,0)
end
function ondamaged(item,sourceObj,damagedObj,value)

end