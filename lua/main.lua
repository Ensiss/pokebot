package.path = "lua/?.lua;" .. package.path

require 'listeners'

-- This seems to fix a weird segfault with luabridge objects
-- I don't know why though
collectgarbage("setstepmul", 0)
