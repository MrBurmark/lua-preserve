require 'preserve'

function a() return 0 end

wrhe = io.open('host_endian.txt', "w+")
wrhe:write(string.dump(a))
wrhe:close()

wrbe = io.open('big_endian.txt', "w+")
wrbe:write(dump_be(a))
wrbe:close()
