require 'Utils/Pathfinding'

--------------------------------------------------------------------------------
-- LOCAL FUNCTIONS
--------------------------------------------------------------------------------

local function _initOldOWs(oldows)
   for i = 1, 15 do
      local ow = pb.getOverWorld(i)
      table.insert(oldows, {destX = ow:getDestX(), destY = ow:getDestY(), bankId = ow:getBankId(), mapId = ow:getMapId()})
   end
end

local function _hasNPCMoved(oldows)
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

local function _getTargetPos(x, y)
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


--------------------------------------------------------------------------------
-- EXPORTED FUNCTIONS
--------------------------------------------------------------------------------


-- moveTo(x, y[, maxDist])      Moves to a node in the current map
-- moveTo(id)                   Moves to a person defined by its id
function move.to(x, y, maxDist)
   maxDist = maxDist or 0

   -- if y is nil, x is a person id so we have to stop 1 tile away from the target
   if not y then maxDist = 1 end

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
         if dx == 0 then ret = move.step(dy < 0 and btn.up or btn.down)
         else ret = move.step(dx < 0 and btn.left or btn.right) end

         -- We were blocked, recompute the path
         if ret == -1 then break end

         -- Someone moved, recompute the path
         if _hasNPCMoved(oldows) then break end
      end

      -- Destination in range, return
      if math.abs(p:getX() - target.x) + math.abs(p:getY() - target.y) <= maxDist then return 0 end
   until false
end

-- move.npc(personId)           Moves to a person defined by its id
function move.npc(personId)
   return move.to(personId)
end
