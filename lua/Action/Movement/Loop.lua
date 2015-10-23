-- Move in a loop of the given size, and starting in the given direction
function move.loop(init_dir, size, order)
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
         if move.step(dirlist[math.mod(init_idx + i * step + 4, 4) + 1]) == -1 then
            return -1
         end
      end
   end
end
