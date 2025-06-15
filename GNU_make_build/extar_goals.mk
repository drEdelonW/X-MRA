BIN_NAME     := $(TARGET_NAME)$(EXEC_SUFIX)
REMOTE_PATH  := /opt/hexafpv/
REMOTE_APP   := $(REMOTE_PATH)$(BIN_NAME)
SCRIPT_PATH  := ../scripts_configs/

REMOTE_USER	= root
TRG_HOST ?= $(REMOTE_USER)@$(shell echo $$TRG_HOST)

run: $(BIN_NAME)
	@$(TARGET)$(EXEC_SUFIX) $(ARGS)


deploy: $(BIN_NAME)
	@echo "Killing remote instances on $(TRG_HOST)..."
	@ssh $(TRG_HOST) 'killall -9 $(BIN_NAME) 2>/dev/null || true && rm -f $(REMOTE_APP)'
	@echo "Deploying $(BIN_NAME) to $(TRG_HOST):$(REMOTE_PATH)..."
	@scp $(OUTPUT_DIR)/$(BIN_NAME) $(TRG_HOST):$(REMOTE_PATH)
	@scp $(SCRIPT_PATH)/cam.conf $(TRG_HOST):$(REMOTE_PATH)

rrun: deploy
	@echo "Starting service on $$TRG_HOST..."
	@ssh $(TRG_HOST) "$(REMOTE_APP)"

	# @ssh $(TRG_HOST) "systemctl restart hexafpv && systemctl status --no-pager hexafpv"
