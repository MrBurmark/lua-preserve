# lua-preserve

Influenced by lua-marshal and Data Dumper, lua-preserve is a lua state preservation program meant for embedded lua programs.  
Lua-preserve redefines some basic functions such as require to intercept libraries and reinitialize them, it can also then restore c functions from those libraries, and other functions registered through the lua-preserve c-interface.
Lua-preserve generates a lua string containing everything needed to restore the current lua state, and passing that string to restore will restore the lua state.

Caveats:
	Lightuserdata is not preserved.
	Unregistered c-functions are not preserved.
	Userdata without a __preserve or __persist metamethod are not preserved.
	Coroutines are not preserved.
	Function code may not be architecture or machine independent.
	