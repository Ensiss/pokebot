-- Waits until the next emulated frame to continue
function misc.nextFrame()
   return coroutine.yield()
end
