function onEnterFrame()
   p = data.player
   map = data.world:getMap(p:getBank(), p:getMap())
   print("Player coordinates: (" .. p:getX() .. ", " .. p:getY() .. ")")
   print("Map [" .. map:getName() .. "]: " .. map:getWidth() .. "x" .. map:getHeight())
end

function onInit()
   print "Printing on init"
   x = data.player:getX()
   y = data.player:getY()
   bot.moveTo(x, y - 1)
   bot.moveTo(x - 1, y - 1)
   bot.moveTo(x - 1, y)
   bot.moveTo(x, y)
   bot.talkTo(0)
end
