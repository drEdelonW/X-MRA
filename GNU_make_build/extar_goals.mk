BIN_NAME     := $(TARGET_NAME)$(EXEC_SUFIX)
REMOTE_PATH  := /opt/hexafpv/$(BIN_NAME)

TRG_HOST ?= $(shell echo $$TRG_HOST)
REMOTE_USER	= root

run: $(BIN_NAME)
	@$(TARGET)$(EXEC_SUFIX) $(ARGS)


deploy: $(BIN_NAME)
	@echo "Killing remote instances on $(TRG_HOST)..."
	@ssh $(REMOTE_USER)@$(TRG_HOST) 'killall -9 $(BIN_NAME) 2>/dev/null || true && rm -f $(REMOTE_PATH)'
	@echo "Deploying $(BIN_NAME) to $(TRG_HOST):$(REMOTE_PATH)..."
	@scp $(OUTPUT_DIR)/$(BIN_NAME) $(REMOTE_USER)@$(TRG_HOST):$(REMOTE_PATH)

rrun: deploy
	@echo "Starting service on $$TRG_HOST..."
	@ssh $(REMOTE_USER)@$(TRG_HOST) "$(REMOTE_PATH)"

	# @ssh $(REMOTE_USER)@$(TRG_HOST) "systemctl restart hexafpv && systemctl status --no-pager hexafpv"
