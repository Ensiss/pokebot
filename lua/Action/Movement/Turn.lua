-- Turn in a given direction
function move.turn(button)
   if button > btn.down then
      print("moveDirection error: the specified button is not a direction")
      return -1
   end

   local counter = 0
   local ow = pb.getOverWorld(0) -- OverWorld 0 is the player's sprite
   local dir2key = {btn.down, btn.up, btn.left, btn.right};

   -- 0x0203707A: 0 if static
   --             1 if turning
   --             2 if walking
   while mem.get8(0x0203707A) ~= 0 do
      misc.nextFrame()
   end

   if dir2key[ow:getDir()] ~= button then
      repeat
         pb.setButton(button, math.mod(counter, 2) == 0)
         misc.nextFrame()
         counter = counter + 1
      until mem.get8(0x0203707A) == 1
   end

   pb.releaseButton(button)
   while mem.get8(0x0203707A) == 1 do
      misc.nextFrame()
   end

   return 0
end
