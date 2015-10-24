-- Suspends execution during <nbFrames> frames
function misc.wait(nbFrames)
   for _ = 1, nbFrames do
      misc.nextFrame()
   end
end
