local _behaviors = {}
_behaviors[0x62] = btn.right    -- Warp to block right
_behaviors[0x63] = btn.left     -- Warp to block left
_behaviors[0x64] = btn.up       -- Warp to block up
_behaviors[0x65] = btn.down     -- Warp to block down
_behaviors[0x6A] = btn.left     -- Pokecenter stairs up
_behaviors[0x6B] = btn.right    -- Pokecenter stairs down
_behaviors[0x6C] = btn.right    -- Stairs up right
_behaviors[0x6D] = btn.right    -- Stairs down right
_behaviors[0x6E] = btn.left     -- Stairs up left
_behaviors[0x6F] = btn.left     -- Stairs down left

-- move.warp(warpId)            -- Uses the warp #warpId in the current map
function move.warp(warpId)
   local p = pb.getPlayer()
   local m = pb.getCurrentMap()

   if warpId < 0 or warpId >= m:getNbWarps() then
      print(string.format("useWarp error: %d is not a valid warp id in map [%d, %d]", warpId, p:getBankId(), p:getMapId()))
      return -1
   end

   local warp = m:getWarp(warpId)
   local tx = warp:getX()
   local ty = warp:getY()
   local node = m:getNode(tx, ty)

   if move.to(tx, ty, node:getStatus() == 0x01 and 1 or 0) == -1 then return -1 end

   local px = p:getX()
   local py = p:getY()
   local key = nil

   if px == tx and py == ty then
      key = _behaviors[node:getBehavior()]
   else
      if py < ty then key = btn.down
      elseif py > ty then key = btn.up
      elseif px > tx then key = btn.left
      else key = btn.right end
   end

   if key == nil then return -1 end

   local ow = pb.getOverWorld(0)
   local owx = ow:getDestX()
   local owy = ow:getDestY()
   -- TODO: change moveDirection script to allow going through doors
   repeat
      misc.pressButton(key)
   until ow:getDestX() ~= owx or ow:getDestY() ~= owy

   -- Wait until screen fade out is done
   -- Fucking animations making my life hard and my code ugly
   misc.wait(150)
end
