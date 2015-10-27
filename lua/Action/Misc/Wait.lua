-- Suspends execution during <nbFrames> frames
function misc.wait(nbFrames)
   for _ = 1, nbFrames do
      misc.nextFrame()
   end
end

-- Suspends execution until a condition is met
-- The condition parameter is a function returning the value
function misc.waitUntil(condition)
   repeat
      misc.nextFrame()
   until condition()
end

function misc.waitWhile(condition)
   while condition() do
      misc.nextFrame()
   end
end
