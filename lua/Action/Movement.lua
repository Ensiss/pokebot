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
