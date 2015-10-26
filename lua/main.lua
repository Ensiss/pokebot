package.path = "lua/?.lua;" .. package.path
require 'Action/Misc'
require 'Action/Battle'
require 'Action/Movement'
require 'Action/Interaction'
require 'Utils/Wrapper'

require 'Custom/World/SimpleWorldAI'
require 'Custom/Battle/SimpleBattleAI'

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

function onInit()
   local bot = pb.getBot()
   config.setNumber("clearOnRefresh", 1)

   -- Main script for the bot, called every frame to continue execution
   -- You need an AI for the outside world and another to handle battles
   local botScript = wrap(SimpleWorldAI, SimpleBattleAI)
   bot:queue(botScript)
end
