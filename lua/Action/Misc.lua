-- Moves the cursor to a specific position in a box
-- w     Width of the box
-- h     Height of the box
-- dest  Index of destination position
-- f     Function that returns the current cursor position
function moveCursor(w, h, dest, func)
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
      coroutine.yield()
      pb.releaseButton(button)
      coroutine.yield()
   end
   return 0
end


-- Releases all keys, and then presses the specified key
function pressButton(button)
   if button < 0 or button >= btn.count then
      print(string.format("pressButton error: %d is not a valid button", button))
      return -1
   end

   -- Release every button
   for i = 0, btn.count - 1 do
      pb.releaseButton(i)
   end
   coroutine.yield()

   -- Press the one we want
   pb.pressButton(button)
   coroutine.yield()

   pb.releaseButton(button)
   return 0
end
