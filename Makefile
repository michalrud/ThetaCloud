all:
	pushd build && cmake  .. && make -j4 && ctest --output-on-failure && popd
	sphinx-build-3 -b html . ./_build
