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
