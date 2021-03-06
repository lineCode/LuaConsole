--[[
	MIT License
 
	Copyright (c) 2017-2018 Cody Tilkins
 
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
 
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
 
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
--]]


-- define v this or use -Dplat=Windows -- Linux -- MacOSX
-- plat = os.types.Windows -- os.types.Linux -- os.types.MacOSX

if(plat == nil)then
	error("Please define plat as Windows Linux or MacOSX")
end


require("build_func")


print(">>> Cleaning bin")
print("> bin\\Debug")
os.execute(rmdir("bin\\Debug"))
os.execute(rmdir("bin\\debug"))
print("> bin\\Release")
os.execute(rmdir("bin\\Release"))
os.execute(rmdir("bin\\release"))


print(">>> Cleaning obj")
print("> *.o")
os.execute(del("*.o"))
os.execute(del("obj\\*.o"))
os.execute(del("src\\*.o"))
print("> *.a")
os.execute(del("*.a"))
os.execute(del("obj\\*.a"))
os.execute(del("src\\*.a"))
print("> *.dll")
os.execute(del("*.dll"))
os.execute(del("obj\\*.dll"))
os.execute(del("src\\*.dll"))

print(">>> Cleaning Debug")
print("> *.gcov")
os.execute(del("*.gcov"))
print("> *.gcda")
os.execute(del("*.gcda"))
print("> *.gcno")
os.execute(del("*.gcno"))

