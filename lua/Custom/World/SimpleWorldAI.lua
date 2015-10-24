function SimpleWorldAI()
   local p = pb.getPlayer()

   -- Comment out this block to allow the script to run on any map
   if p:getBankId() ~= 3 or p:getMapId() ~= 0 then
      print('This example bot was made for map PALLET TOWN, go there, save and restart to test')
      print('If you want to test it anyway just comment out the check in lua/Custom/World/SimpleWorldAI.lua')
      print("The Battle script will work even without doing anything")
      repeat
         misc.nextFrame()
      until false
   end

   for i = 1, 3 do
      move.turn(btn.right)
      move.turn(btn.down)
      move.turn(btn.left)
      move.turn(btn.up)
   end

   move.warp(0)
   move.to(3, 3)
   move.warp(1)

   move.to(3, 3)
   move.to(0)
   move.to(3, 3)
   move.connection(connect.up)
   repeat
      if move.loop(btn.up) == -1 then misc.nextFrame() end
   until false
end
