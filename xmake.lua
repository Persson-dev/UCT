add_rules("mode.debug", "mode.release")

add_requires("opengl", "glfw >= 3")

target("UCT")
	set_kind("binary")
	add_files("src/*.cpp")
	add_includedirs("include")
	add_includedirs("libs")

	add_packages("opengl", "glfw")

	set_languages("c++17")

	-- ImGui
	add_includedirs("libs/imgui")
	add_files("libs/imgui/*.cpp", "libs/imgui/backends/imgui_impl_opengl3.cpp", "libs/imgui/backends/imgui_impl_glfw.cpp")

	if is_os("linux") then
		add_links("dl")
	elseif is_os("windows") then
		add_ldflags("-static")
		-- Hide console
		if is_mode("release") then
			add_ldflags("mwindows", {force = true})
		end
	end
