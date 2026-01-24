# Root Makefile to build all subdirectory projects

# Find all subdirectories containing Makefiles
MAKEFILE_DIRS := $(shell find . -mindepth 2 -maxdepth 2 -name "Makefile" -type f | xargs dirname | sort)

# Default target
all: $(MAKEFILE_DIRS)

# Phony targets
.PHONY: all clean help $(MAKEFILE_DIRS)

# Rule to build each subdirectory
$(MAKEFILE_DIRS):
	@echo "Building $@..."
	@$(MAKE) -C $@

# Clean all subdirectories
clean:
	@for dir in $(MAKEFILE_DIRS); do \
		echo "Cleaning $$dir..."; \
		@$(MAKE) -C $$dir clean || true; \
	done

# Clean all subdirectories
release:
	@echo "Creating release.zip..."
	@rm -f release.zip 2>/dev/null || true
	@for dir in $(MAKEFILE_DIRS); do \
		base=$$(basename $$dir); \
		file="$$dir/$$base"; \
		if [ -f "$$file" ]; then \
			echo "✓ Adding $$file"; \
			zip -q release.zip "$$file"; \
		else \
			echo "✗ Skipping $$file (not found)"; \
		fi; \
	done
	@if [ -f release.zip ]; then \
		echo "Successfully created release.zip"; \
	else \
		echo "No files were added to release.zip"; \
	fi

# Help target
help:
	@echo "Root Makefile - Build System"
	@echo "============================="
	@echo "Available targets:"
	@echo "  all     - Build all subdirectory projects (default)"
	@echo "  clean   - Clean all subdirectory projects"
	@echo "  release - Build release.zip with all the binaries"
	@echo "  help    - Show this help message"
	@echo ""
	@echo "Found projects in:"
	@for dir in $(MAKEFILE_DIRS); do \
		echo "  - $$dir"; \
	done
