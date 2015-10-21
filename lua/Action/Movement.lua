require 'Utils/Pathfinding'

-- Move one step in a given direction
function moveDirection(button)
   if button > btn.down then
      print("moveDirection error: the specified button is not a direction")
      return -1
   end

   local counter = 0
   local p = pb.getPlayer()
   local m = pb.getCurrentMap()
   local ow = pb.getOverWorld(0) -- OverWorld 0 is the player's sprite
   local xstart = p:getX()
   local ystart = p:getY()
   local xend = xstart + ((button == btn.right and 1 or 0) - (button == btn.left and 1 or 0))
   local yend = ystart + ((button == btn.down and 1 or 0) - (button == btn.up and 1 or 0))

   -- if the destination is in the current map but not walkable, error
   if xend >= 0 and xend < m:getWidth() and
      yend >= 0 and yend < m:getHeight() and
      m:getNode(xend, yend):getStatus() ~= 0x0C
   then
      print("moveDirection error: destination is unreachable")
      return -1
   end

   -- try moving until all three coordinates have changed
   while ((p:getX() == xstart and p:getY() == ystart) or
         (ow:getX() == xstart and ow:getY() == ystart) or
         (ow:getDestX() == xstart and ow:getDestY() == ystart))
   do
      pb.setButton(button, math.mod(counter, 2) == 0)
      coroutine.yield()
      counter = counter + 1
   end

   -- if the direction key is still down, release it
   if pb.getButton(button) then
      pb.releaseButton(button)
   end

   return 0
end

-- Move in a loop of the given size, and starting in the given direction
function moveLoop(init_dir, size, order)
   init_dir = init_dir or btn.right
   size = size or 1
   order = order or "CW"

   local dirlist = {btn.right, btn.down, btn.left, btn.up}
   local init_idx = 0
   local step = 0
   if order == "CCW" then step = -1
   elseif order == "CW" then step = 1
   else
      print("moveLoop error: 3rd parameter 'order' must be either 'CW' or 'CCW'")
      return -1
   end

   for i, button in pairs(dirlist) do
      if button == init_dir then
         init_idx = i - 1
         break
      end
   end

   for i = 0, 3 do
      for _ = 1, size do
         if moveDirection(dirlist[math.mod(init_idx + i * step + 4, 4) + 1]) == -1 then
            return -1
         end
      end
   end
end

-- moveTo(x, y[, maxDist])      Moves to a node in the current map
-- moveTo(id)                   Moves to a person defined by its id
function moveTo(x, y, maxDist)
   maxDist = maxDist or 0

   -- if y is nil, x is a person id so we have to stop 1 tile away from the target
   if not y then maxDist = 1 end

   local _initOldOWs = function(oldows)
      for i = 1, 15 do
         local ow = pb.getOverWorld(i)
         table.insert(oldows, {destX = ow:getDestX(), destY = ow:getDestY(), bankId = ow:getBankId(), mapId = ow:getMapId()})
      end
   end

   local _hasNPCMoved = function(oldows)
      local p = pb.getPlayer()
      local owp = pb.getOverWorld(0)
      local ret = false

      for i, oldow in ipairs(oldows) do
         local ow = pb.getOverWorld(i)

         -- Check if something changed
         if not ret and ow:getBankId() == p:getBankId() and ow:getMapId() == p:getMapId() and
            (ow:getDestX() ~= oldow.destX or ow:getDestY() ~= oldow.destY or
             ow:getBankId() ~= oldow.bankId or ow:getMapId() ~= oldow.mapId) then
               ret = true
         end

         -- Update the infos for next time
         oldow.destX = ow:getDestX()
         oldow.destY = ow:getDestY()
         oldow.bankId = ow:getBankId()
         oldow.mapId = ow:getMapId()
      end
      return ret
   end

   local _getTargetPos = function(x, y)
      -- coordinates are specified, return them
      if y then return {x = x, y = y} end

      local p = pb.getPlayer()
      local m = pb.getCurrentMap()
      local pers = m:getPerson(x)

      for i = 1,15 do
         local ow = pb.getOverWorld(i)

         -- last loaded overworld reached, we can stop
         if ow:getMapId() == 0 and ow:getBankId() == 0 then break end

         -- if the person we are looking for is loaded, return its location
         if ow:getMapId() == p:getMapId() and ow:getBankId() == p:getBankId() and ow:getEventNb() == pers:getEventNb() then
            return {x = ow:getDestX(), y = ow:getDestY()}
         end
      end

      -- if the person is not loaded yet, return its default location
      return {x = pers:getX(), y = pers:getY()}
   end

   local p = pb.getPlayer()
   local m = pb.getCurrentMap()
   local oldows = {}

   if not y and (x < 0 or x >= m:getNbPersons()) then
      print(string.format("moveTo error: invalid person id: %d in map [%d, %d]", x, p:getBankId(), p:getMapId()))
      return -1
   end
   _initOldOWs(oldows)

   -- Repeat so a path can be recomputed if something fails
   repeat
      local target = _getTargetPos(x, y)
      local path = Pathfinding.AStar(m, {x = p:getX(), y = p:getY()}, target, maxDist)
      if not path then
         print("No Path found :(")
         return -1
      end

      for i, node in ipairs(path) do
         local dx = node:getX() - p:getX()
         local dy = node:getY() - p:getY()
         local ret

         -- move towards the next node
         if dx == 0 then ret = moveDirection(dy < 0 and btn.up or btn.down)
         else ret = moveDirection(dx < 0 and btn.left or btn.right) end

         -- We were blocked, recompute the path
         if ret == -1 then break end

         -- Someone moved, recompute the path
         if _hasNPCMoved(oldows) then break end
      end

      -- Destination in range, return
      if math.abs(p:getX() - target.x) <= maxDist and math.abs(p:getY() - target.y) <= maxDist then return 0 end
   until false
end
