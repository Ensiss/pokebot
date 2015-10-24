-- Move one step in a given direction
function move.step(button)
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

   for i = 0, btn.count - 1 do
      pb.releaseButton(i)
   end

   -- press the direction until we start moving
   while ow:getDestX() == xstart and ow:getDestY() == ystart do
      pb.setButton(button, math.mod(counter, 2) == 0)
      misc.nextFrame()
      counter = counter + 1
   end

   -- if the direction key is still down, release it
   pb.releaseButton(button)

   -- wait until all three coordinates have changed
   while ((p:getX() == xstart and p:getY() == ystart) or
         (ow:getX() == xstart and ow:getY() == ystart) or
         (ow:getDestX() == xstart and ow:getDestY() == ystart))
   do
      misc.nextFrame()
   end

   return 0
end
