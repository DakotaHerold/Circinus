-- global vars
--var1

function Update()
	print("Update from Lua called: " .. test.var1)
end

function UpdateTransform()
	test.x = test.x + 0.0001
end  
