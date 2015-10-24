-- Moves the cursor to a specific position in a box
-- w     Width of the box
-- h     Height of the box
-- dest  Index of destination position
-- f     Function that returns the current cursor position
function misc.moveCursor(w, h, dest, func)
   local dx = dest % w
   local dy = math.floor(dest / w)

   while true do
      local c = func()
      local cx = c % w
      local cy = math.floor(c / w)
      local button = nil

      if cx > dx then button = btn.left
      elseif cy > dy then button = btn.up
      elseif cx < dx then button = btn.right
      elseif cy < dy then button = btn.down end

      if not button then return 0 end

      pb.pressButton(button)
      misc.nextFrame()
      pb.releaseButton(button)
      misc.nextFrame()
   end
   return 0
end
