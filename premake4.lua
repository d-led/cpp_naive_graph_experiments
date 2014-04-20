_G.package.path=_G.package.path..[[;./?.lua;./?/?.lua]]

assert( require 'premake.quickstart' )

make_solution 'cpp_naive_graph_experiments'

includedirs {
	'./catch/single_include',
	'./cpplinq/CppLinq'
}

make_console_app('cpp_naive_graph_experiments', { './*.cpp' })

make_cpp11()

run_target_after_build()
