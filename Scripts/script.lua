--global vars 
testString = "Lua Works!"
number = 5

--Update function
function Update()
	print("Update from Lua called")
end

--getGlobalNamespace (L)
--  .beginNamespace ("test")
--	.addVariable ("var1", &testNumber)
--	.addFunction("Update", Update)
--  .endNamespace ();


	