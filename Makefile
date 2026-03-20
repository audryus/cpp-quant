.PHONY: debug release run run-release clean

.DEFAULT_GOAL := debug

debug:
	./scripts/build_debug.sh
	./build_debug/main

release:
	./scripts/build_release.sh
	./build_release/main

clean:
	rm -rf build_debug build_release
