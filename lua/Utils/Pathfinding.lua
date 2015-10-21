require 'Utils/Table'

Pathfinding = {}

-- Returns a path from src to dest usting the A* algorithm
-- dist is the maximum acceptable distance from the target
function Pathfinding.AStar(map, src, dest, dist)
   local _walkableTiles = {0x0c, 0x00, 0x10}

   -- Rebuilds the complete path from destination to source, based on the node's parent member
   -- /!\ Separate definition and declaration is needed for recursive local functions
   local _rebuildPath
   _rebuildPath = function(node, set)
      set = set or {}

      local from = node:getFrom()

      if from then
         _rebuildPath(from, set)
         table.insert(set, node)
      end
      return set
   end

   local _checkWalkable = function(node)
      return (table.find(_walkableTiles, node:getStatus()) ~= nil and
                 node:getBehavior() ~= 0x6b and node:getBehavior() ~= 0x6a)
   end

   local _getNodeValue = function(node)
      return node:getX() + (1 / node:getY())
   end

   -- optionnal variables
   dist = dist or 0

   -- function start
   local openset = {}
   local closedset = {}

   if src.x < 0 or src.x >= map:getWidth() or
      src.y < 0 or src.y >= map:getHeight() or
      dest.x < 0 or dest.x >= map:getWidth() or
      dest.y < 0 or dest.y >= map:getHeight()
   then
      print("Pathfinding.AStar error: source or destination out of bounds")
      return -1
   end

   local node = map:getNode(src.x, src.y)
   node:setF(dest.x, dest.y)
   node:setFrom(nil)
   table.insert(openset, node)
   while table.getn(openset) > 0 do
      local mini, curr = table.min(openset, function(x) return x:getF() end)

      table.remove(openset, mini)
      if math.abs(curr:getX() - dest.x) + math.abs(curr:getY() - dest.y) == dist then
         return _rebuildPath(curr)
      end
      table.insert(closedset, curr)

      for i = 0, 3 do
         local dx = (i - 1) * math.mod(i + 1, 2)
         local dy = (i - 2) * math.mod(i, 2)
         local x = curr:getX() + dx
         local y = curr:getY() + dy

         -- 'repeat .. until true' is used here to emulate a continue statement with a break
         repeat
            -- avoid out of bounds nodes
            if x < 0 or x >= map:getWidth() or y < 0 or y >= map:getHeight() then break end

            local nextnode = map:getNode(x, y)
            local nextbehavior = nextnode:getBehavior()

            -- avoid occupied (by NPCs) nodes
            -- if _checkOverWorld(x, y) then break end

            -- tile type check
            if not(--_checkHills(dx, dy, nextnode, curr) or
                      _checkWalkable(nextnode) or
                      -- Block access to hill from a lower level
                      nextbehavior == 0x32 or
                      -- Check is escalator is the final tile
                      (dy == 0 and (nextbehavior == 0x6b or nextbehavior == 0x6a) and x == dest.x and y == dest.y))
            then break end

            local it = table.find(closedset, nextnode, _getNodeValue)
            local g = curr:getG() + 10 -- _getMovmentCost(nextnode)

            if it and g >= nextnode:getG() then break end
            if not it or g < nextnode:getG() then
               nextnode:setFrom(curr)
               nextnode:setG(g)
               nextnode:setF(dest.x, dest.y)
               if not table.find(openset, nextnode, _getNodeValue) then
                  table.insert(openset, nextnode)
               end
            end
         until true
      end
   end
   print("Returning nil")
   return nil
end
