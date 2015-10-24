-- Releases all keys, and then presses the specified key
function misc.pressButton(button)
   if button < 0 or button >= btn.count then
      print(string.format("pressButton error: %d is not a valid button", button))
      return -1
   end

   -- Release every button
   for i = 0, btn.count - 1 do
      pb.releaseButton(i)
   end
   misc.nextFrame()

   -- Press the one we want
   pb.pressButton(button)
   misc.nextFrame()

   pb.releaseButton(button)
   return 0
end
