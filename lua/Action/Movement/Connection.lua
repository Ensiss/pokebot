--------------------------------------------------------------------------------
-- LOCAL FUNCTIONS
--------------------------------------------------------------------------------

local function _findConnection(m, dir)
   for i = 0, m:getNbConnections() - 1 do
      local connection = m:getConnection(i)

      if connection:getType() == dir then
         return connection
      end
   end
   return nil
end


--------------------------------------------------------------------------------
-- EXPORTED FUNCTIONS
--------------------------------------------------------------------------------


-- move.connection(dir)         -- Leaves the current map by using the connection in the specified direction
function move.connection(dir)
   if dir == connect.none or dir > connect.right then
      print("useConnection error: only directions Up, Down, Left and Right are currently suported")
      return -1
   end

   local dir2key = {btn.down, btn.up, btn.left, btn.right};
   local p = pb.getPlayer()
   local m = pb.getCurrentMap()
   local x = dir == connect.right and m:getWidth() - 1 or 0
   local y = dir == connect.down and m:getHeight() - 1 or 0
   local vtcl = (dir == connect.up or dir == connect.down)
   local xstep = vtcl and 1 or 0
   local ystep = math.mod(xstep + 1, 2)
   local connection = _findConnection(m, dir)

   if not connection then
      print(string.format("useConnection error: No connection of type %d in map (%d, %d)", dir, p:getBankId(), p:getMapId()))
      return -1
   end

   local dmap = pb.getMap(connection:getBankId(), connection:getMapId())

   while x < m:getWidth() and y < m:getHeight() do
      -- compute index in the destination map
      local dx
      local dy

      if vtcl then
         dx = x - connection:getOffset()
         if dir == connect.up then
            dy = dmap:getHeight() - 1
         else
            dy = 0
         end
      else
         dy = y - connection:getOffset()
         if dir == connect.left then
            dx = dmap:getWidth() - 1
         else
            dx = 0
         end
      end

      if dx >= 0 and dx < dmap:getWidth() and
         dy >= 0 and dy < dmap:getHeight() and -- check out of bounds
         m:getNode(x, y):getStatus() == 0x0c and -- walkable tile in the current map
         dmap:getNode(dx, dy):getStatus() == 0x0c -- walkable in the destination map
      then break end

      x = x + xstep
      y = y + ystep
   end

   if x >= m:getWidth() or y >= m:getHeight() then
      print("useConnection error: couldn't find an exit in the specified direction")
      return -1
   end

   move.to(x, y)
   move.step(dir2key[dir])
   return 0
end
