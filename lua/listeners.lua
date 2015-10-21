require 'Action/Movement'

function printMap()
   local map = pb.getCurrentMap()
   for y = 0, map:getHeight() - 1 do
      for x = 0, map:getWidth() - 1 do
         io.write(string.format("%02x ", map:getNode(x, y):getStatus()))
      end
      io.write("\n")
   end
end

function onRefresh()
   local p = pb.getPlayer()
   local map = pb.getCurrentMap()
   print("Player coordinates: (" .. p:getX() .. ", " .. p:getY() .. ")")
   print("Map [" .. map:getName() .. "]: " .. map:getWidth() .. "x" .. map:getHeight())
end

-- Creates a function callable by the bot
-- It will resume the coroutine when called
-- Returns -1 when there's an error or the coroutine is dead
-- Returns 0 otherwise
function wrap(func)
   co = coroutine.create(func)

   return function()
      local code, msg = coroutine.resume(co)

      if code == false or coroutine.status(co) == 'dead' then
         if code == false then
            print("Lua error:", msg)
         end
         return -1
      end
      return 0
   end
end

function onInit()
   local bot = pb.getBot()

   config.setNumber("clearOnRefresh", 1)

   -- Main script for the bot, called every frame to continue execution
   local botScript = wrap(function()
         moveTo(3, 3)
         moveTo(0)
         moveTo(3, 3)
         repeat
            moveLoop(btn.left)
            moveLoop(btn.up)
            moveLoop(btn.right)
            moveLoop(btn.down)
         until true
   end)

   bot:queue(botScript)
end
